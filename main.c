#include <stdio.h>
#include <stdlib.h>

Simulated for 10000000s with Arrival Rate of 1 packets/sec and Service Rate of 10 packets/sec:
Sojourn time is 0.105554 seconds

Simulated for 10000000s with Arrival Rate of 5 packets/sec and Service Rate of 10 packets/sec:
Sojourn time is 0.149956 seconds

Simulated for 10000000s with Arrival Rate of 7 packets/sec and Service Rate of 10 packets/sec:
Sojourn time is 0.216527 seconds

Simulated for 10000000s with Arrival Rate of 9 packets/sec and Service Rate of 10 packets/sec:
Sojourn time is 0.548064 seconds

Simulated for 10000000s with Arrival Rate of 10 packets/sec and Service Rate of 10 packets/sec:
Sojourn time is 819.938263 seconds




struct Data{
    double arrivalTime;
    double departureTime;
};

struct Node{
    struct Data data;
    struct Node * next;
};

struct Queue{
    int currSize;
    struct Node * front;
    struct Node * rear;

};

struct Queue initQueue();
void enqueue(struct Queue *qPtr, struct Data d);
struct Data dequeue(struct Queue *qPtr);
void freeQueue(struct Queue *qPtr);

///////////

//#include "queue.h"

struct Queue initQueue()
{
    struct Queue Q;
    Q.currSize=0;
    Q.front=NULL;
    Q.rear=NULL;
    return Q;

}

void enqueue(struct Queue *qPtr, struct Data d)
{
    struct Node * newNode;
    newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data=d;
    newNode->next=NULL;
    if (qPtr->rear==NULL){
        qPtr->rear=newNode;
        qPtr->front=newNode;
    }
    else
    {
        qPtr->rear->next=newNode;
        qPtr->rear=newNode;
    }
    qPtr->currSize++;
}



struct Data dequeue(struct Queue *qPtr)
{
   struct Node * temp;
   struct Data * d;
   if (qPtr->currSize != 0){
       temp=qPtr->front;
       *d=temp->data;
       if (qPtr->front == qPtr->rear){
           qPtr->front=NULL;
           qPtr->rear=NULL;
        }
        else{
            qPtr->front=temp->next;
        }
        free(temp);
        qPtr->currSize--;
        return *d;

   }
}

void freeQueue(struct Queue *qPtr)
{
    struct Data temp;
    while (qPtr->currSize != 0){
        temp = dequeue(qPtr);
    }
}

/////////////

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "queue.h"

typedef enum {ARRIVAL, DEPARTURE} Event;

struct Simulation{
    double currTime;
    double arrivalRate;
    double serviceTime;
    double timeForNextArrival;
    double timeForNextDeparture;
    double totalSimTime;
    struct Queue buffer, eventQueue;
    Event e;
};

double runSimulation(double arrivalRate, double serviceTime, double simTime);
struct Simulation initSimulation(double arrivalRate, double serviceTime, double simTime);
double getRandTime(double arrivalRate);
double calcAverageWaitingTime(struct Simulation * S);

/////////

#include "assignment3.h"

double calcAverageWaitingTime(struct Simulation * S)
{
    double totaltime = 0, totalnodes = 0;
    while (S->eventQueue.currSize != 0){
        struct Data d = dequeue(&(S->eventQueue));
        totaltime += d.departureTime - d.arrivalTime;
        totalnodes += 1;
    }
    return totaltime/totalnodes;
}

struct Simulation initSimulation(double arrivalRate, double serviceTime, double simTime)
{
    struct Simulation sim;
    sim.currTime = 0;
    sim.arrivalRate = arrivalRate;
    sim.serviceTime = serviceTime;
    sim.timeForNextArrival = getRandTime(arrivalRate);
    sim.timeForNextDeparture = sim.timeForNextArrival + serviceTime;
    sim.totalSimTime = simTime;
    sim.buffer = initQueue();
    sim.eventQueue = initQueue();
    sim.e = ARRIVAL;
    return sim;

}

double runSimulation(double arrivalRate, double serviceTime, double simTime)
{
    sim = initSimulation(arrivalRate, serviceTime, simTime);
    struct Data d;
    while(sim.currTime < simTime){
        if(sim.e == ARRIVAL){
            d.arrivalTime = sim.currTime;
            d.departureTime = sim.currTime + serviceTime;
            enqueue(&(sim.buffer), d);
            sim.currTime = sim.timeForNextArrival;
            sim.timeForNextArrival = getRandTime(arrivalRate);

        }

        else{
            enqueue(&(sim.eventQueue), dequeue(&(sim.buffer)));
            sim.currTime = sim.timeForNextDeparture;
            if (sim.buffer->front != NULL){
                sim.timeForNextDeparture = sim.buffer->front.data;
            }
        }

        if(sim.timeForNextArrival < sim.timeForNextDeparture){
            sim.e = ARRIVAL;
        }
        else{
            sim.e = DEPARTURE;
        }
    }
}




