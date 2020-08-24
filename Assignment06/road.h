#ifndef _road_h
#define _road_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "car.h"
#include "queue.h"


typedef struct RoadData
{
    /* TODO - Add data associated with road.  Suggested data: */
    /* length of this road */
    int length;
    /* information used to record/update whether the light at the end of this road is green or red */
    int lightCycles[3];
    int tempLight;
    bool lightState;
    /* intersections this road starts from and moves to */
    int from;
    int to;
    /* array of cars associated with this road */
    Car** carsOnThisRoad;
    /* queue of cars waiting to enter this road */
    Queue* carsWaiting;

}  RoadData;

RoadData* initializeRoad(int toVertex, int fromVertex, int length, int greenOn, int greenOff, int reset, int* longestCycle);
bool moveThruIntersection( RoadData* curr, RoadData* newRoad, int* currCycle, int* carsRem, int* maxStep, int* carsComplete, int* totalSteps);
void updateLights(RoadData** roadArr, int numRoads);
bool moveCarsForward( RoadData* road );
bool addCarToRoad( RoadData* road, int* carsRem);
void printAllRoads( RoadData** roadArr, int numRoads );
void freeLLNodesHelper(LLNode* front);
void freeARoad( RoadData* road );

#endif

