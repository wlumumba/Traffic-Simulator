#include "trafficSimulator.h"


/* trafficSimulator
 * input: char* name of file to read
 * output: N/A
 *
 * Simulate the road network in the given file
 */
void trafficSimulator( char* filename )
{
    /* Read in the traffic data.  You may want to create a separate function to do this. */
    FILE* thefile = NULL;
    thefile = fopen(filename, "r");
    if(thefile == NULL)
    {
        printf("ERROR: File not found\n");
        exit(-1);
    }
    
    TrafficSimData* data = (TrafficSimData*)malloc(sizeof(TrafficSimData)); //REMEMBER TO FREE (CAUSING LAST ERROR)

    int numVertices = 0, numRoads = 0;
    fscanf(thefile, "%d %d", &numVertices, &numRoads);
    
    //Scan in input and set values
    initialize(thefile, data, numVertices, numRoads);
    
    /* Loop until all events processed and either all cars reached destination or gridlock has occurred */
    Event* tmp = NULL;
    int i = 0;
    int cycleCnt = 0;
    int carsRem = 0;
    
    int carsComplete = 0;   //num of completed cars
    int totalSteps = 0;     //total of completed car timesteps
    int maxStep = 0;

    bool check1 = false, check2 = false, check3 = false;
    int lastMovedNum = -1;

    while(!isEmptyPQ(data->events) || carsRem > 0 )
    {
        
        // Loop on events associated with this time step 
        while( isEmptyPQ(data->events) == 0 && getFrontPriority(data->events) == cycleCnt  )
        {
            tmp = dequeuePQ(data->events);
            switch (tmp->type)
            {
                case ADD_CAR_EVENT:
                    mergeQueues(tmp->oneRoad->carsWaiting, tmp->cars);
                    freeQueue(tmp->cars);
                    printf("CYCLE %d - ADD_CAR_EVENT - Cars enqueued on road from %d to %d\n", cycleCnt, tmp->from, tmp->to);
                    break;

                case PRINT_ROADS_EVENT:
                    printf("\nCYCLE %d - PRINT_ROADS_EVENT - Current contents of the roads:\n\n", cycleCnt);
                    printAllRoads(data->roadArray, numRoads);
                    break;

                default:
                    printf("\n\nEvent is neither addCar or printRoads\n\n");
                    break;
            }
            free(tmp);
        }

        // First try to move through the intersection (dependent on roads light cycle) //
        updateLights(data->roadArray, numRoads);

        for(i = 0; i < data->numOfRoads; i++)
        {
            Car* hold = data->roadArray[i]->carsOnThisRoad[0];
            RoadData* nextRoad = NULL;
            int nextI = -1;
            bool result = false;

            if(data->roadArray[i]->lightState == true && hold != NULL)
            {
                result = getNextOnShortestPath(data->network, data->roadArray[i]->to, hold->identifier, &nextI);
                if(result)
                {
                    nextRoad = getEdgeData(data->network, data->roadArray[i]->to, nextI); //should be... end of curr road, to
                    if(nextRoad == NULL || nextRoad->carsOnThisRoad[nextRoad->length - 1] == NULL) //wtf
                    {
                        check1 = moveThruIntersection( data->roadArray[i], nextRoad, &cycleCnt, &carsRem, &maxStep, &carsComplete, &totalSteps);
                    }
                }
            }
        }

        // Second move cars forward on every road //
        for(i = 0; i < data->numOfRoads; i++)
        {
            check2 = moveCarsForward(data->roadArray[i]);
        }

        // Third move cars onto road if possible //
        for(i = 0; i < data->numOfRoads; i++)
        {
            check3 = addCarToRoad(data->roadArray[i], &carsRem);
        }
        if(check1 == false && check2 == false && check3 == false)
            lastMovedNum++;
            //printf("No movement\n");
        else
            lastMovedNum = 0;
            //printf("Movement\n");

        if(lastMovedNum == data->longestCycle)
            break;
    
        cycleCnt++;
    }
    
    if(lastMovedNum == data->longestCycle)
        printf("\nCYCLE %d - Gridlock detected.\n", cycleCnt);
    else
    {
        printf("\nAverage number of time steps to the reach their destination is: %.2lf\n", (double)totalSteps/carsComplete); //ONLY IF NOT GRIDLOCKD
        printf("Maximum number of time steps to the reach their destination is: %d\n", maxStep);
    }

    // FREE STUFF //
    for(i = 0; i < numRoads; i++)
    {
        freeARoad(data->roadArray[i]);
    }
    free(data->roadArray);
    freePQ(data->events);
    freeGraph(data->network);
    free(data);

}


void initialize( FILE* thefile, TrafficSimData* data, int numVertices, int numRoads)
{
    data->network = createGraph(numVertices);
    data->events = createPQ();
    data->roadArray = (RoadData**)malloc(sizeof(RoadData*) * numRoads);
    data->numOfRoads = numRoads;
    data->longestCycle = -1;

    int count = 0, i = 0, roadArrIndex = 0;
    int toVertex = 0, fromVertex = 0, numofIncomingRoads = 0, length = 0, greenOn = 0, greenOff = 0, reset = 0;
    int addCarCommands = 0, printCommands = 0, tempTimestep = 0, numofCarsForEvent = 0, destinationOfCar = 0, tempPrintCycle = 0;
    
    //Adds intersections(vertecies) to graph, and roads/road data
    while(count < numVertices)
    {
        fscanf(thefile, "%d %d", &toVertex, &numofIncomingRoads);
        for(i = 0; i < numofIncomingRoads; i++)
        {
            fscanf(thefile, "%d %d %d %d %d", &fromVertex, &length, &greenOn, &greenOff, &reset);

            RoadData* temp = initializeRoad(toVertex, fromVertex, length, greenOn, greenOff, reset, &data->longestCycle);
            
            data->roadArray[roadArrIndex] = temp;
            setEdge(data->network, fromVertex, toVertex, length);
            setEdgeData(data->network, fromVertex, toVertex, temp);
            //printf("to: %d, from: %d\n", data->roadArray[roadArrIndex]->to, data->roadArray[roadArrIndex]->from);
            //printf("to: %d, from: %d\n", getEdgeData(data->network, fromVertex, toVertex)->to, getEdgeData(data->network, fromVertex, toVertex)->from);

            roadArrIndex++;
        }
        count++;
    }
    
    //Adds car events to priority queue
    count = 0;
    i = 0;
    fscanf(thefile, "%d", &addCarCommands);
    while(count < addCarCommands)
    {
        fscanf(thefile, "%d %d %d", &fromVertex, &toVertex, &tempTimestep);
        fscanf(thefile, "%d", &numofCarsForEvent);
        
        Event* temp = (Event*)malloc(sizeof(Event));
        
        temp->type = ADD_CAR_EVENT;
        temp->timestep = tempTimestep; //priority
        temp->cars = createQueue();
        temp->numCarstoAdd = numofCarsForEvent;
        temp->from = fromVertex;
        temp->to = toVertex;
        
        //Find the road which we will enqueue cars to, stores it in Event
        temp->oneRoad = getEdgeData(data->network, fromVertex, toVertex);
        
        for(i = 0; i < numofCarsForEvent; i++)
        {
            Car* tempCar = (Car*)malloc(sizeof(Car));
            fscanf(thefile, "%d", &destinationOfCar);
            tempCar->start = fromVertex;
            tempCar->identifier = destinationOfCar;
            tempCar->cycleAdded = tempTimestep;
            enqueue(temp->cars, tempCar);
        }
        
        enqueueByPriority(data->events, temp, tempTimestep);
        
        count++;
    }
    //printf("%d\n", getNext(getNextPQ(data->events)->cars)->identifier);
    
    //Adds print events to priority queue
    count = 0;
    fscanf(thefile, "%d", &printCommands);
    while(count < printCommands)
    {
        fscanf(thefile, "%d", &tempPrintCycle);
        Event* temp = (Event*)malloc(sizeof(Event));
        temp->type = PRINT_ROADS_EVENT;
        temp->timestep = tempPrintCycle;  //priority
        enqueueByPriority(data->events, temp, tempPrintCycle);

        count++;
    }
    fclose(thefile);
    //printf("count: %d\n", count);
    
}