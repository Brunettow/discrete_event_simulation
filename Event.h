#include <string>
#include "Hacker.h"
#include <cmath>
using namespace std;

#ifndef EVENT_H
#define EVENT_H


class Event {

public:
    string type; //type of the event
    int hackerId; //id of the hacker in the event
    int data; //codeCommitLengt or TableId 
   	float time; //time of the eent
 
    Event(string type, float time, int hackerId=1, int data=0);
    Event(Event& event);
    Event& operator=(const Event& event);
    bool operator<(const Event& other);
    ~Event();
};


#endif //EVENT_H
