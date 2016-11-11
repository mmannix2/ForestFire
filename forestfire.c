#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TREE 'T'
#define FIRE 'X'
#define SPACE ' '

#define WIDTH 80
#define HEIGHT 40

#define DEBUG

void printForest(char forest[HEIGHT][WIDTH]) {
    #ifdef DEBUG
    //Print column number
    printf("   ");
    for(int i = 0; i < WIDTH/10; i++) {
        for(int j = 0; j < 10; j++) {
            printf("%d", i);
        }
    }
    printf("\n");
    printf("   ");
    for(int i = 0; i < WIDTH/10; i++) {
        for(int j = 0; j < 10; j++) {
            printf("%d", j);
        }
    }
    printf("\n");
    #endif
    
    //Print the actual data
    for(int row = 0; row < HEIGHT; row++) {
        #ifdef DEBUG
        printf("%02d_", row);
        #endif
        //printf("%s\n", forest[row]);
        
        for(int col = 0; col < WIDTH; col++) {
            //Print the char
            if( forest[row][col] == TREE ||
                forest[row][col] == FIRE ||
                forest[row][col] == SPACE ) {
                printf("%c", forest[row][col]);
            }
            //This should never appear
            else {
                printf("?");
            }
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int gen;
    double ignition, growth;  
    char forest [HEIGHT][WIDTH];
    FILE* file;

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
    
    //Open the file for reading
    file = fopen(argv[1], "r");
    if(file != NULL) {
        for(int row = 0; row < HEIGHT && !feof(file); row++) {
            //Read a whole row at a time
            fread(forest[row], sizeof(char), WIDTH, file);
            fscanf(file, "\n");
        }
        //Close the file
        fclose(file);
    }
    else {
        printf("Failed to open %s.\n", argv[1]);
        //Bail out
        fclose(file);
        MPI_Finalize();
    }

    //Print first generation
    if(rank == 0) {
        printf("First generation:\n");
        //Print the forest
        printForest(forest);
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
