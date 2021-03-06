

#define _CRT_SECURE_NO_WARNINGS


#include <mpi.h>
#include "Logger.h"
#include "SearcherCore.h"
#include "SearcherCore_MPI.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void checkArgs(int argc, char *argv[]);
void solve();
void solve_MPI(int argc, char *argv[]);


static bool USE_MPI = false;
static char imgFileName[256];
static char templFileName[256];
static char resultFileName[256];
static string PATH("");


int main(int argc, char *argv[])
{
	checkArgs(argc, argv);

	if (USE_MPI)
		solve_MPI(argc, argv);
	else
		solve();

	return 0;
}



void checkArgs(int argc, char *argv[]) {

	if (argc > 1) {
		PATH.append(argv[1]);
	}
	if (!PATH.empty()) {
		strcpy(imgFileName, string(PATH).append("img.jpg").data());
		strcpy(templFileName, string(PATH).append("templ.jpg").data());
		strcpy(resultFileName, string(PATH).append("result.jpg").data());
	}
	else {
		strcpy(imgFileName, "img.jpg");
		strcpy(templFileName, "templ.jpg");
		strcpy(resultFileName, "result.jpg");
	}

	if (argc > 2) {
		if (strcmp(argv[2], "1") == 0) USE_MPI = true;
	}
}


void solve() {
	Logger logger(PATH);

	logger.logI("\r\n\r\n\r\n\r\n");
	logger.logI(PATH); //log
	logger.logI(imgFileName); //log
	logger.logI(templFileName); //log
	logger.logI(resultFileName); //log

	logger.logI("start search"); //log

	SearcherCore searcherCore;
	searcherCore.search(imgFileName, templFileName, resultFileName);

	logger.logI("end search"); //log
}


void solve_MPI(int argc, char *argv[]) {
	int processId;
	int processCount;
	static int master = 0;
	int ierr;

	Logger logger(PATH);

	logger.logI("\r\n\r\n\r\n\r\n");
	logger.logI(PATH); //log
	logger.logI(imgFileName); //log
	logger.logI(templFileName); //log
	logger.logI(resultFileName); //log

	ierr = MPI_Init(&argc, &argv);

	if (ierr != 0)
	{
		logger.logI("can't init mpi, fatal error");
		return;
	}

	MPI_Comm_size(MPI_COMM_WORLD, &processCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);

	logger.logI(to_string(processId).append("   processId ")); //log
	logger.logI(to_string(processCount).append("   processCount ")); //log

	if (processId == 0) logger.logI("start search"); //log

	SearcherCore_MPI searcherCore(processId, processCount);
	searcherCore.search(imgFileName, templFileName, resultFileName);

	if (processId == 0) logger.logI("end search"); //log

	MPI_Finalize();
}