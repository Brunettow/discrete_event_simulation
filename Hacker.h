#include <cmath>
using namespace std;

#ifndef HACKER_H
#define HACKER_H


class Hacker {

public:
	int ID, codeLength, numbCodeCommit, round; //code length, number of code commmits larger than 20, round
	float hoodieQEnterT, roundStartTime, totalTimeSpentQ; //hoodie enter time, round start time, total time spent in queues

	Hacker(int ID);
	Hacker(Hacker& hacker);
	Hacker& operator=(const Hacker& hacker);
	bool operator<(const Hacker& other);
	~Hacker();
};

#endif //HACKER_H
