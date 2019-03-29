#pragma once

#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <math.h>
#include "Logger.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;


class SearcherCore_MPI
{
private:
	int processId;
	int processCount = 1;
	Logger logger;
	Mat img; Mat templ; Mat result;  Mat img_display; Mat slidingWindow;
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	int size_of_sliding_window; int sliding_window_size_from; int sliding_window_size_to;

	void readImages(char *imgPath, char *templatePath);
	void createResultImage();
	void findTemplateByHausdorffDistance();
	void getImageBySlidingWindow(int from, int to);
public:
	void search(char *imgPath, char *templatePath, char *resultPath);
	SearcherCore_MPI(int processId, int processCount);
	~SearcherCore_MPI();
};

