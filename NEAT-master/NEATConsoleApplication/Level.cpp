#include "Level.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
using namespace std;

int getUnitCount(const char *string, const char *set)
{
	int count = 0;
	short last = 0;
	while (*string) {
		last = *string++;

		for (int i = 0; set[i]; i++) {
			if (last == set[i]) {
				count++;
				last = 0;
				break;
			}
		}
	}
	if (last)
		count++;
	return count;
}

int Level::initAS(AS *newAS,char *argline)
{
	std::stringstream ss(argline);
	ss >> newAS->Id;
	ss >> newAS->fc;
	ss >> newAS->x;
	ss >> newAS->y;
	ss >> newAS->vx;
	ss >> newAS->vy;
	ss >> newAS->mode;
	ss >> newAS->avgcircle;

	return 0;
}

int Level::initIT(IT *newIT, char *argline)
{
	std::stringstream ss(argline);
	ss >> newIT->Id;
	ss >> newIT->fc;
	ss >> newIT->x;
	ss >> newIT->y;
	ss >> newIT->vx;
	ss >> newIT->vy;
	ss >> newIT->fire;
	ss >> newIT->mode;
	ss >> newIT->move;
	return 0;
}

Level::Level(int Id, std::ifstream &iFile)
{
	iteAs = 0;
	iteIt = 0;
	char curword[128];  //max word size of 128 characters
	char curline[1024]; //max line size of 1024 characters
	char delimiters[] = " \n";

	int done = 0;

	levelId = Id;

	iFile.getline(curline, sizeof(curline));
	int wordcount = getUnitCount(curline, delimiters);
	int curwordnum = 0;

	//Loop until file is finished, parsing each line
	while (!done) {

		//std::cout << curline << std::endl;

		if (curwordnum > wordcount || wordcount == 0) {
			iFile.getline(curline, sizeof(curline));
			wordcount = getUnitCount(curline, delimiters);
			curwordnum = 0;
		}

		std::stringstream ss(curline);
		ss >> curword;


		if (strcmp(curword, "levelend") == 0) {
			ss >> curword;
			int idcheck = atoi(curword);
			if (idcheck != levelId) printf("ERROR: id mismatch in genome");
			done = 1;
		}
		else if (strcmp(curword, "levelstart") == 0) {
			++curwordnum;
		}
		else if (strcmp(curword, "/*") == 0) {

			ss >> curword;
			while (strcmp(curword, "*/") != 0) {

				ss >> curword;
			}
		}

		else if (strcmp(curword, "AS") == 0) {
			AS *newAS;

			char argline[1024];

			curwordnum = wordcount + 1;

			ss.getline(argline, 1024);
			newAS = new AS;
			initAS(newAS, argline);

			As.push_back(newAS);

		}

		else if (strcmp(curword, "IT") == 0) {
			IT *newIT;

			char argline[1024];

			curwordnum = wordcount + 1;

			ss.getline(argline, 1024);

			newIT = new IT;
			initIT(newIT, argline);

			It.push_back(newIT);

		}


		//ofstream datafile("level.txt");
		//datafile.close();
	}
}