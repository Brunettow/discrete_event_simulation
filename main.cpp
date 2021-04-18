#include "Hacker.h"
#include "Event.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;  

template <typename T>  //for comparing two pointers
struct comparing{
    bool operator()(T *left, T *right){
        return *left < *right;
    }
};

struct Table{  //hoodieTable and stickerTable

public:
    int ID;  //ID of the table
    float serviceTime;  //service time 
    float time; //first available time of the table
    string type; //hoodie table or sticker table 

    Table(int ID, string type, float serviceTime){

        this->ID = ID;
        this->serviceTime = serviceTime;
        this->type = type;
        this->time = 0;
    }
};

int freeTable(const vector<Table*> &tables, int size, float time){  //method to find first available table

    int nextAvailableId = 0;
    for (int i = 1; i < size; ++i) {

        if( time - tables[i]->time > 0.0001 || abs(time-tables[i]->time) <0.0001 ){ //table with ID "i" is available at the moment
            if(time - tables[nextAvailableId]->time > 0.0001 || abs(time - tables[nextAvailableId]->time) < 0.0001){  //table with nextAvailableId is also available
                if(nextAvailableId > i) {  //Table with lower ID is taken  
                nextAvailableId = i;
                } 
            } else { //table with nextAvailableId is occupied
                nextAvailableId = i;
            }
            
        } else { //selected table not available at the moment
            if(tables[nextAvailableId]->time - time > 0.0001){ //previous table also occupied
                if(tables[nextAvailableId]->time - tables[i]->time > 0.0001){  //first table available is id with i
                    nextAvailableId = i;
                } else if(abs(tables[nextAvailableId]->time - tables[i]->time)<0.0001){ //tables availability time is equal
                    if(nextAvailableId>i){ //Table with lower ID is taken  
                        nextAvailableId=i;
                    }
                }
            }
        }
    }

    return nextAvailableId;
}

int main(int argc, char *argv[]){

    string infile_name = argv[1];  //file input
    string outfile_name = argv[2]; //file output

    ifstream infile;
    infile.open(infile_name);

    ofstream outfile;
    outfile.open(outfile_name);

    int N, C, Q, Ds, Dh; 

    infile >> N; //number of Hackers

    priority_queue<Event *, vector<Event *>, comparing
   <Event>> events;

    for (int i = 1; i < N + 1; ++i) { //arrival time a göre event açıp heap e attım

        float arrivalTime; //arrival time of the hacker
        infile >> arrivalTime; 
        Event *arrival = new Event("arrival", arrivalTime, i, 0); //constructing event arrival
        events.push(arrival);
    }

    infile >> C; //total number of code commits

    for (int i = 0; i < C; ++i) { //code commits of the hackers

        int id, codeCommited; //id of the hacker, code length of the hacker committed
        float time;  //time of the code attempt
        infile >> id >> codeCommited >> time;
        Event *codeCommit = new Event("codeCommit", time, id, codeCommited); //constructing event for code commit
        events.push(codeCommit);
    }

    infile >> Q; //number or queue enterance attempts

    for (int i = 0; i < Q; ++i){

        int id; //id of the hacker
        float time; //time of the enter attempt
        infile >> id >> time;
        Event *enterAttempt = new Event("enterAttempt", time, id, 0); // constructing event for enter queue attempts
        events.push(enterAttempt);
    }

    infile >> Ds; //number of the sticker tables
    vector<Table *> stickerTables; //sticker tables

    for (int i = 0; i < Ds; ++i){ //StickerTable

        float serviceTime;  //servise time of the table
        infile >> serviceTime;
        stickerTables.push_back(new Table(i, "StickerTable", serviceTime)); //constructing sticker tables
     
    }

    infile >> Dh; //number of the hoodie tables
    vector<Table *> hoodieTables;

    for (int i = 0; i < Dh; ++i) { //HoodieTable

        float serviceTime;  //servise time of the table
        infile >> serviceTime;
        hoodieTables.push_back(new Table(i, "HoodieTable", serviceTime)); //constructing hoodie tables

    }

    int maxLengthSticker = 0;  //max length of the sticker queue
    int maxLengthHoodie = 0;  //max length of the hoodie queue

    int totalGifts = 0;           //total gifts or rounds
    int totalCodeCommits = 0;     //length of the codes

    int invalidAttempts = 0;      //all invalid attempths
    int invalidAttemptsGifts = 0; //invalid attempts for try to take more than 3 gifts

    int spentMostTimeId = 1;    //id of the hacker who spent most time in queues
    float mostTimeSpent = 0;     // most time spent in queues of the hacker
    int spentLeastTimeId = -1;    //id of the hacker who spent least time in queues with having 3 gifts
    float leastTimeSpent = -1;   // least time spent in queues of the hacker

    float totalSecondPast = 0;  //total time spent in hackathon

    float totalWaitStickerQ = 0;  //total time spent by all the hackers in sticker queue
    float totalWaitHoodieQ = 0;   //total time spent by all the hackers in hoodie queue

    float totalTurnAround = 0;  //addition of the times that passed during every round of the hackers

    vector<Hacker*> hackers;  //hackers
    hackers.push_back(nullptr);  //thus, hacker with first id is 1 and in position 1
    queue<Hacker*> stickerQueue;   //sticker queue     
    priority_queue<Hacker *, vector<Hacker *>, comparing<Hacker>> hoodieQueue;   //hoodie heap

while (!events.empty()){  //while heap of events is not empty

    Event *event = events.top();
    events.pop();
    string eventName = event->type;  //event type
   
    if (eventName == "arrival") {
        int hackerId = event->hackerId;
        hackers.push_back(new Hacker(hackerId)); //make hackers and put them in a vector
    }

    if (eventName == "codeCommit")  { 
        int hackerId = event->hackerId;
        Hacker *hacker = hackers[hackerId]; //hacker that code committed
         hacker->codeLength += event->data; //length of the code committed
        totalCodeCommits += event->data;  //adding it to total code commits

        if(event->data>=20){ //if code length is equal or larger than 20
        	 hacker->numbCodeCommit++;  //number of the code commit of the hacker
        }
    }


    if (eventName=="enterAttempt"){ 
        int hackerId = event->hackerId;  //id of the hacker attempt to enter the sticker queue
        Hacker* hacker = hackers[hackerId];

        if(hacker->numbCodeCommit>=3 && hacker->round<3 && hacker->codeLength>=20){ //conditions to enter sticker queue

        hacker->roundStartTime = event->time;  //round started for the hacker
        int freeTableId = freeTable(stickerTables,Ds,event->time); //get next free table
        Table* freeTable = stickerTables[freeTableId];  //next free table

            if(stickerQueue.empty() && !(freeTable->time - event->time > 0.0001)) { //directly going sticker table, no line in queue and there are available desks

                float endTime = event->time + freeTable->serviceTime;  //end time of the new event
                Event* stickerTableE = new Event("stickerTable", endTime, hackerId, freeTableId); //make new event for sticker table
                events.push(stickerTableE);
                freeTable->time = endTime;  //table time update

            } else {  //hacker going to queue

                stickerQueue.push(hacker); //push hacker to sticker queue
                    if(maxLengthSticker<stickerQueue.size()){  //checking queue size 
                    maxLengthSticker=stickerQueue.size();   
                    }
            }


        } else if (hacker->round >= 3){  //invalid attempt to get more than 3 gifts
            invalidAttemptsGifts++;
                
            
        } else {
        		invalidAttempts++;
        } 

        if(event->time - totalSecondPast > 0.0001){ //updating last time of the event
        totalSecondPast = event->time;
        }
    }


    if(eventName=="stickerTable"){ 

        int hackerId = event->hackerId; //id of the hacker in the table
        Hacker* hacker = hackers[hackerId];

        Table* table = stickerTables[event->data];  
        float eventBegin = event->time - table->serviceTime;//time of the event begin

        float waitTimeQueue = eventBegin - hacker->roundStartTime;  //time hacker waited in sticker queue
        if(abs(waitTimeQueue)<0.0001 || waitTimeQueue<0){
        	waitTimeQueue=0.0000;
        }
        hacker->totalTimeSpentQ += waitTimeQueue;  //hacker total wait in queues update
        totalWaitStickerQ += waitTimeQueue;  //update total wait time in sticker queue

        int hoodieTableId = freeTable(hoodieTables,Dh,event->time); //next free hoodie table id
        Table* hoodieTable = hoodieTables[hoodieTableId];
        hacker->hoodieQEnterT = event->time; //hoodie queue enter time

        if(hoodieQueue.empty() && !(hoodieTable->time - event->time > 0.0001)) { //directl going to hoodie table, no lines, there are available tables

            float endTime = event->time + hoodieTable->serviceTime; //end time of the new event
            Event* hoodieTableE = new Event("hoodieTable", endTime, hackerId, hoodieTableId); //make new event for sticker table
            events.push(hoodieTableE);
            hoodieTable->time = endTime;  //table time update

        } else {  //hacker going to hoodie queue

            hoodieQueue.push(hacker); //push hacker to hoodie queue
            if(maxLengthHoodie<hoodieQueue.size()){ //checking size of the queue
            maxLengthHoodie=hoodieQueue.size(); 
            }
        }


        if(!stickerQueue.empty()){ //making next event for the sticker table if there are any hackers in the queue
                
            int nextHackerId = stickerQueue.front()->ID;  //next hacker id
            stickerQueue.pop();

            int nextTableId = freeTable(stickerTables, Ds,event->time);  //next free table
            Table* freeTable = stickerTables[nextTableId];

            float nextTime = 0; //time of the next event
                if(freeTable->time - event->time > 0.0001) {   //if table is not free at the moment  
                    nextTime = freeTable->time + freeTable->serviceTime;
                } else { //if table is free at the moment
                    nextTime = event->time + freeTable->serviceTime;
                }

            freeTable->time = nextTime ;  //updating table next availability  time
            Event* stickerTableE = new Event("stickerTable", nextTime , nextHackerId, nextTableId); //making next event for the table
            events.push(stickerTableE);
        }
    }


    if(eventName=="hoodieTable"){ 

        int hackerId = event->hackerId;  //id of the hacker in the hoodie table
        Hacker* hacker = hackers[hackerId];

        hacker->round++;  //hacker round update
        totalGifts++; //total gifts ( total rounds update )

        float waitTimeQueue = event->time- hacker->hoodieQEnterT- hoodieTables[event->data]->serviceTime; //total wait time in hacker queue of the hacler
	        if(abs(waitTimeQueue)<0.0001 || waitTimeQueue<0){
	        	waitTimeQueue=0.0000;
	        } 
	    hacker->totalTimeSpentQ += waitTimeQueue; //updating total wait time passed in queues of the hacker
        totalWaitHoodieQ += waitTimeQueue;  //updating total wait time of the hackers
        totalTurnAround += event->time - hacker->roundStartTime;  //total time passed during the rounds update

            if(hacker->totalTimeSpentQ - mostTimeSpent > 0.0001){ //finding hacker that spent most time in queues
                spentMostTimeId = hackerId;
                mostTimeSpent = hacker->totalTimeSpentQ;
                
            } else if(abs(hacker->totalTimeSpentQ - mostTimeSpent) < 0.0001){ //if times are equal
                if(hacker->ID < hackers[spentMostTimeId]->ID){  //if id is smaller
                    spentMostTimeId = hackerId;
                    mostTimeSpent = hacker->totalTimeSpentQ;
                }
            }
 
        if(hacker->round == 3){ //finding hacker that spent least time in queues
            if(spentLeastTimeId==-1){
            spentLeastTimeId=hacker->ID;
            leastTimeSpent = hacker->totalTimeSpentQ;
            } else {
                if(abs(leastTimeSpent - hacker->totalTimeSpentQ) < 0.0001){ //if times are equal
                    if(hacker->ID < hackers[spentLeastTimeId]->ID){ //hacker with smaller id
                        spentLeastTimeId = hackerId;
                        leastTimeSpent = hacker->totalTimeSpentQ;
                    }
                } else if(leastTimeSpent - hacker->totalTimeSpentQ > 0.0001){ //if hacker has smaller time passed in queues
                        spentLeastTimeId = hackerId;
                        leastTimeSpent = hacker->totalTimeSpentQ;
                }
            }
        }

        if(!hoodieQueue.empty()){ //making next event for the sticker table
            Hacker* nextHacker = hoodieQueue.top();  //next hacker for the table
            hoodieQueue.pop();

            int nextTableId = freeTable(hoodieTables,Dh,event->time);  //next free table
            float nextTime = event->time + hoodieTables[nextTableId]->serviceTime;  //end time of the event
            hoodieTables[nextTableId]->time = nextTime;  //availability time update of the table
            Event* hoodieTable = new Event("hoodieTable", nextTime, nextHacker->ID, nextTableId); //making next event for the table
            events.push(hoodieTable);
            }

        if(event->time - totalSecondPast > 0.0001){  //finding last time of the last event
        totalSecondPast = event->time;
        }
    }
}
      
        float avgTurnAroundT = totalTurnAround / totalGifts;  //avg turn around
        float avgWaitStickerQ = totalWaitStickerQ / totalGifts;  //avg wait int sticker queue
        float avgWaitHoodieQ = totalWaitHoodieQ / totalGifts;   // avg wait hoodie queue
        
        outfile << maxLengthSticker << endl;  // max length sticker
        outfile << maxLengthHoodie << endl;  //max length hoodie queue

        outfile << fixed <<setprecision(3)<< (double)totalGifts/N << endl;  //avg gifts per hacker

        outfile << fixed <<setprecision(3) << avgWaitStickerQ << endl; //avg wait in sticker queue per hacker
        outfile << fixed <<setprecision(3)  << avgWaitHoodieQ << endl;  //avg wait in hoodie queue per hacker

        outfile << fixed <<setprecision(3) << (double)C/N << endl;  //avg number of code commits per hacker
        outfile << fixed <<setprecision(3) << (double)totalCodeCommits / C << endl; //avg number of code length per hacker

        outfile  << fixed <<setprecision(3) << avgTurnAroundT << endl;  //avg tur around time

        outfile << invalidAttempts << endl;  //total number of invalid attempts
        outfile << invalidAttemptsGifts << endl;   //total number of invalid attempts for gifts

        outfile  << spentMostTimeId << " ";  //id of the hacker who spent most time in queues
        outfile  << fixed <<setprecision(3) << mostTimeSpent << endl; //time that hacker spent in queues
        outfile  << spentLeastTimeId << " "; //id of the hacker who spent leastt time in queues
        outfile  << fixed <<setprecision(3) << leastTimeSpent << endl;//time that hacker spent in queues

        outfile << fixed <<setprecision(3) << totalSecondPast;//total time past during hackathon

        for (int i = 0; i < N; ++i){ //deleting hackers
            delete hackers[i];
        }

        for (int i = 0; i < Ds; ++i){ //dleeting tables
            delete stickerTables[i];
        }

        for (int i = 0; i < Dh; ++i){
            delete hoodieTables[i];
        }

        infile.close();
        outfile.close();

        return 0;
}
