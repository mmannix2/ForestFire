#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TREE 'T'
#define FIRE 'X'
#define SPACE ' '

struct Forest {
    int height;
    int width;
    //char trees[width][height];
};

int main(int argc, char** argv) {
    int rank, size;
    int gen;
    double ignition, growth;  
    struct Forest;

    //Setup MPI
    MPI_Init(&argc, &argv);
    
    //Get rank and size
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    //Check for arguments
    if(argc != 5) {
        //Print error message
        if(rank == 0) {
            printf("Usage: %s input_file N I G\n", argv[0]);
        }
        MPI_Finalize();
        exit(-1);
    }
    
    //Get arguments
    gen  = atoi(argv[2]);
    ignition = atof(argv[3]);
    growth = atof(argv[4]);
    
    //DEBUG
    #ifdef DEBUG
    if(rank == 0) {
        printf("N: %d\n", gen);
        printf("I: %.02f\n", ignition);
        printf("G: %.02f\n", growth);
    }
    #endif
    
    //Read in the file
    
    //Print first generation
    if(rank == 0) {
        printf("First generation:\n");
        //Print it
    }
    
    //Run the simulation
    for(int i=0; i < gen; i++) {
        
    }
    
    //Print final generation
    if(rank == 0) {
        printf("Final generation:\n");
        //Print it
    }

    //Close MPI
    MPI_Finalize();

    return 0;
}
