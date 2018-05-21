#pragma once
#include <vector>

struct AS {
	int Id,fc,mode;
	float x,y;
	float vx, vy;
	float avgcircle;
};
struct IT {
	int Id, fc, mode;
	float x, y;
	float vx, vy;
	bool fire;
	bool move;

};
class Level {

	public:
		int levelId;

		std::vector<AS *> As;
		std::vector<IT *> It;
		int iteAs;
		int iteIt;
		Level::Level(int Id, std::ifstream &iFile);

	protected:
		int Level::initAS(AS *newAS, char *argline);
		int Level::initIT(IT *newIT, char *argline);
		const int ASparams = 8;
		const int ITparams = 9;
};

