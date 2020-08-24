#include "road.h"


/* Suggested TODO - Write a function to create initialize a road (not the array of roads).
 */

RoadData* initializeRoad(int toVertex, int fromVertex, int length, int greenOn, int greenOff, int reset, int* longestCycle)
{
    RoadData* temp = (RoadData*)malloc(sizeof(RoadData));
    Car** carRay = NULL;
    carRay = (Car**)malloc(sizeof(Car*) * length); //Road is made up of Car*, if NULL, the space is empty
    int i = 0;
    while(i < length)
    {
        carRay[i] = NULL;
        i++;
    }

    temp->to = toVertex;
    temp->from = fromVertex;
    temp->length = length;
    temp->tempLight = 0; //
    temp->carsOnThisRoad = carRay;
    temp->carsWaiting = createQueue();
    temp->lightState = false;
    temp->lightCycles[0] = greenOn;
    temp->lightCycles[1] = greenOff;
    temp->lightCycles[2] = reset;

    if(reset > *longestCycle)
        *longestCycle = reset;

    return temp;

}

/* Suggested TODO - Write a function to free a road.
 */
void freeLLNodesHelper(LLNode* front)
{
    if(front == NULL)
        return;
    freeLLNodesHelper(front->pNext);

    free(front->qt);
    front->qt = NULL;
    free(front);
    front = NULL;
}

void freeARoad( RoadData* road ) //pass in a single address of a road
{
    int i = 0;
    for(i = 0; i < road->length; i++)
    {
        free(road->carsOnThisRoad[i]);
    }
    free(road->carsOnThisRoad);
    freeLLNodesHelper(road->carsWaiting->qFront);
    freeQueue(road->carsWaiting);
    free(road);

}


/* Suggested TODO - Write a function that attempts to move the cars on a given road forward (if space ahead of them was empty).
 */

bool moveCarsForward( RoadData* road )
{
    int i = 0;
    bool res = false;
    for(i = (road->length) - 1; i > 0; i--)
    {
        //printf("1");
        if(road->carsOnThisRoad[i-1] == NULL && road->carsOnThisRoad[i] != NULL)
        {
            road->carsOnThisRoad[i-1] = road->carsOnThisRoad[i];
            road->carsOnThisRoad[i] = NULL;
            res = true;
            i--;
        }
        
    }
    return res;

}

/* Suggested TODO - Write a function to attempt to move the car at the first position on a given road to the last position of a another given road.
 */
bool moveThruIntersection( RoadData* curr, RoadData* newRoad, int* currCycle, int* carsRem, int* maxStep, int* carsComplete, int* totalSteps)
{
    int carStepsResult = 0;
    if(newRoad == NULL)
    {
        //printf("road doesnt exist, therefore car made it\n");
        carStepsResult = *currCycle - curr->carsOnThisRoad[0]->cycleAdded;
        
        if(carStepsResult > *maxStep)
            *maxStep = carStepsResult;
        *totalSteps = *totalSteps + carStepsResult;
        
        printf("\nCYCLE %d - Car successfully traveled from %d to %d in %d time steps.\n", *currCycle, curr->carsOnThisRoad[0]->start, curr->carsOnThisRoad[0]->identifier, carStepsResult);
        
        ++*carsComplete;
        --*carsRem;
        free(curr->carsOnThisRoad[0]);
        curr->carsOnThisRoad[0] = NULL;
        return true;
    }
    else if(newRoad->carsOnThisRoad[(newRoad->length) - 1] == NULL)
    {
        newRoad->carsOnThisRoad[(newRoad->length) - 1] = curr->carsOnThisRoad[0];
        curr->carsOnThisRoad[0] = NULL;
        return true;
    }
    else
    {
        return false;
        //printf("no space to move car\n");
    }
    
}

/* Suggested TODO - Write a function to attempt to move a car from the add car queue (waiting queue) onto the end of this road.
 */
bool addCarToRoad( RoadData* road, int* carsRem)
{
    if(!isEmpty(road->carsWaiting) && road->carsOnThisRoad[(road->length) - 1] == NULL) //car was added
    {
        road->carsOnThisRoad[(road->length) - 1] = dequeue(road->carsWaiting);
        ++*carsRem;
        return true;

        //printf("Cycle added\n");
    }
    return false;
   //printf("%d\n", getNext(road->carsWaiting)->identifier);
}


/* Suggested TODO - Write a function to update the current state of the traffic light based on the current cycle.
 */
void updateLights(RoadData** roadArr, int numRoads)
{
    int i = 0; //set templight to 0 --DONE
    for(i = 0; i < numRoads; i++)
    {
        if(roadArr[i]->tempLight == roadArr[i]->lightCycles[2]) //cycle reset
        {
            roadArr[i]->tempLight = 0;
        }
        if(roadArr[i]->tempLight >= roadArr[i]->lightCycles[0] && roadArr[i]->tempLight < roadArr[i]->lightCycles[1])
        {
            roadArr[i]->lightState = true;
        }
        if(roadArr[i]->tempLight == roadArr[i]->lightCycles[1])
        {
            roadArr[i]->lightState = false;
        }
        roadArr[i]->tempLight++;
        
        /*if(roadArr[0]->lightState == true)
            printf("true\n");
        else
            printf("false\n");*/
    }
}

/* Suggested TODO - Write a function to print the destinations (identifier's) of the cars on the road
 */
void printAllRoads( RoadData** roadArr, int numRoads )
{
    int i = 0, j = 0;
    for(i = 0; i < numRoads; i++)
    {
        printf("Cars on the road from %d to %d: \n", roadArr[i]->from, roadArr[i]->to);
        for(j = 0; j < roadArr[i]->length; j++)
        {
            if(roadArr[i]->carsOnThisRoad[j] != NULL)
                printf("%d ", roadArr[i]->carsOnThisRoad[j]->identifier);
            else
                printf("- ");
        }
        printf("\n");
    }
}
 


