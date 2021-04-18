#include "Event.h"

Event::Event(string type, float time, int hackerId, int data) { //constructor
	
	this->type = type;
	this->time = time;
	this->data=data;
	this->hackerId=hackerId;
}

Event::Event(Event& event) {  //copy counstructor

	this->type = event.type;
	this->time = event.time;
	this->hackerId=event.hackerId;
	this->data=event.data;
}

Event& Event::operator=(const Event& event) {  //assignment operator
	
	if (this == &event) {
		return *this;
	}

	this->type = event.type;
	this->time = event.time;
	this->hackerId=event.hackerId;
	this->data=event.data;

return *this;

}

bool Event::operator<(const Event& other) {   //overriding "<" operator

	if(this->time - other.time > 0.00001){ //this is larger
		return true;
	} else { //equal or smaller
		if(abs(this->time - other.time) < 0.00001){ //equal
			if(this->hackerId > other.hackerId){  //this id büyük, küçük istiyoruz
				return true;
			}else {
				return false;
			}
		} else {
			return false;
		}
	}

}

Event::~Event() {
	
}


