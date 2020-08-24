#include <stdlib.h>
#include <stdio.h>

#include "trafficSimulator.h"

int main( int argc, char *argv[] )
{
    int i;
    int numFiles = 5;
    char *defaultFilenames[] = {"data-Trivial.txt", "data-Simple.txt", "data-Gridlock.txt", "data-NearGridlock.txt", "data.txt"};

    if( argc==2 ) /* Usage: ./driver <filename> */
        trafficSimulator( argv[1] );
    else{
        printf("Running default test files: \n\n");

        for( i=0; i<numFiles; i++ ){
            printf("\n--------------- START OF OUTPUT FOR %s -----------------\n\n", defaultFilenames[i]);
            trafficSimulator( defaultFilenames[i] ); //is only gonna be called one per file
            printf("\n--------------- END OF OUTPUT FOR %s -----------------\n\n", defaultFilenames[i]);
        };
    }

    return 0;
}
