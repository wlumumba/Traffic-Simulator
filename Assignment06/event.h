#ifndef _event_h
#define _event_h
#include <stdbool.h>

#include "queue.h"
#include "road.h"

typedef enum eventType{ ADD_CAR_EVENT, PRINT_ROADS_EVENT } eventType;

typedef struct Event
{
    //TODO:  Add information associated with the different events of your simulation.
    eventType type;
    int timestep;
    
    //for add car event
    int from;
    int to;
    int numCarstoAdd;
    RoadData* oneRoad; //stores ptr of road where Event will enqueue cars to
    Queue* cars;


}  Event;


#endif
