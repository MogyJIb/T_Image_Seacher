#include "SearcherCore_MPI.h"



void SearcherCore_MPI::readImages(char * imgPath, char * templatePath)
{
	img = imread(imgPath, IMREAD_COLOR);
	templ = imread(templatePath, IMREAD_COLOR);
	img.copyTo(img_display);
}

void SearcherCore_MPI::createResultImage()
{
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);
}

void SearcherCore_MPI::findTemplateByHausdorffDistance()
{
	matchTemplate(img, templ, result, 0);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
}

void SearcherCore_MPI::getImageBySlidingWindow(int from, int to)
{
	int cols = to - from;
	int rows = img.rows;
	slidingWindow.create(rows, cols, CV_32FC1);
}

void SearcherCore_MPI::search(char *imgPath, char *templatePath, char *resultPath)
{
	MPI_Status status;
	string message;

	if (processId == 0)
	{
		readImages(imgPath, templatePath);
		size_of_sliding_window = img.cols / (processCount - 1);
		for (int i = 1; i < processCount; i++)
		{
			sliding_window_size_from = size_of_sliding_window * (i - 1);
			if (sliding_window_size_from != 0) {
				sliding_window_size_from -= 10;
			}
			message = to_string(sliding_window_size_from);
			MPI_Send(message.c_str(), strlen(message.c_str()), MPI_CHAR, i, 99, MPI_COMM_WORLD);

			sliding_window_size_to = size_of_sliding_window * i;
			message = to_string(sliding_window_size_to);
			MPI_Send(message.c_str(), strlen(message.c_str()), MPI_CHAR, i, 99, MPI_COMM_WORLD);

		}
	}
	else
	{
		char message[20];
		int from; int to;

		MPI_Recv(message, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
		from = atoi(message);

		MPI_Recv(message, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
		to = atoi(message);

		logger.logI(string("Proces N")
			.append(to_string(processId))
			.append(" : sliding window from ")
			.append(to_string(from))
			.append(" to ")
			.append(to_string(to))); //LOG

		readImages(imgPath, templatePath);
		getImageBySlidingWindow(from, to);
		createResultImage();
		findTemplateByHausdorffDistance();

		imwrite(resultPath, img_display);
	}
}

SearcherCore_MPI::SearcherCore_MPI(int processId, int processCount) {
	this->processId = processId;
	this->processCount = processCount;
}


SearcherCore_MPI::~SearcherCore_MPI()
{
}
