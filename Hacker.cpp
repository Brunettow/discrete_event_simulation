#include "Hacker.h"
Hacker::Hacker(int ID){  //constructor

	this->ID = ID;
	this->codeLength = 0;
	this->numbCodeCommit = 0;
	this->round = 0;
	this->hoodieQEnterT=0;
	this->roundStartTime = 0.0000;
	this->totalTimeSpentQ = 0.0000;
	
	
}

Hacker::Hacker(Hacker& hacker) {  //copy constructor

	this->ID = hacker.ID;
	this->codeLength = hacker.codeLength;
	this->numbCodeCommit = hacker.numbCodeCommit;
	this->round=hacker.round;
	this->hoodieQEnterT=hacker.hoodieQEnterT;
	this->roundStartTime = hacker.roundStartTime;
	this->totalTimeSpentQ=hacker.totalTimeSpentQ;
	
}

Hacker& Hacker::operator=(const Hacker& hacker){  //assignment operator

	this->ID = hacker.ID;
	this->codeLength = hacker.codeLength;
	this->numbCodeCommit = hacker.numbCodeCommit;
	this->round = hacker.round;
	this->hoodieQEnterT=hacker.hoodieQEnterT;
	this->roundStartTime = hacker.roundStartTime;
	this->totalTimeSpentQ=hacker.totalTimeSpentQ;

return *this;

}

bool Hacker::operator<(const Hacker& other) {  //overriding "<" operator

	if(this->numbCodeCommit != other.numbCodeCommit){
		return(this->numbCodeCommit < other.numbCodeCommit);
	} else { //code commit eşitse
		if(!(abs(this->hoodieQEnterT - other.hoodieQEnterT) < 0.0001) ){  //hoodiequeue enter eşit değilse
			return (this->hoodieQEnterT > other.hoodieQEnterT);
		} else { 
			return (this->ID > other.ID);
		}
	}
}

Hacker::~Hacker(){

}

