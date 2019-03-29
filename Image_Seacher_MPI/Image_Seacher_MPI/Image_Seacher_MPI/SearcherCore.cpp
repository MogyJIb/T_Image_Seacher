#include "SearcherCore.h"



void SearcherCore::readImages(char * imgPath, char * templatePath)
{
	img = imread(imgPath, IMREAD_COLOR);
	templ = imread(templatePath, IMREAD_COLOR);
	img.copyTo(img_display);
}

void SearcherCore::createResultImage()
{
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);
}

void SearcherCore::findTemplateByHausdorffDistance()
{
	matchTemplate(img, templ, result, 0);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
}

void SearcherCore::getImageBySlidingWindow(int from, int to)
{
	int cols = to - from;
	int rows = img.rows;
	slidingWindow.create(rows, cols, CV_32FC1);
}

void SearcherCore::search(char *imgPath, char *templatePath, char *resultPath)
{
	readImages(imgPath, templatePath);
	getImageBySlidingWindow(0, img.cols);
	createResultImage();
	findTemplateByHausdorffDistance();

	imwrite(resultPath, img_display);
}

SearcherCore::SearcherCore() {
}


SearcherCore::~SearcherCore()
{
}
