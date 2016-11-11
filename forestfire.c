#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TREE 'T'
#define FIRE 'X'
#define SPACE ' '
#define BORDER '%'
#define NUM_CELL_TYPES 4

#define WIDTH 82    //80 cols + 1 Border left and right
#define HEIGHT 42   //60 rows + 1 Border above and below
#define COL_START 1
#define COL_END 80
#define ROW_START 1
#define ROW_END 40

#define DEBUG

//enum Cell { BORDER = (int)'%', TREE = (int)'T', FIRE = (int)'X', SPACE = (int)' ' };

/*
const char BORDER = '%';    //-> 0
const char TREE = 'T';      //-> 1
const char FIRE = 'X';      //-> 2
const char SPACE = ' ';     //-> 3
*/

int charToIndex(char c) {
    switch(c) {
        case (int)BORDER:
            return 0;
        case (int)TREE:
            return 1;
        case (int)FIRE:
            return 2;
        case (int)SPACE:
            return 3;
        default:
            return -1;
    }
}   

//Requires the acual row and col numbers
int* neighborCount(int* count, int* nextCount, char* cell) {
    char* ptr = cell;
    
    if(nextCount != NULL) {
        //There are old values that we can use
        for(int i = 0; i < NUM_CELL_TYPES; i++) {
            count[i] = nextCount[i];
        }
        //Reset nextCount
        for(int i = 0; i < NUM_CELL_TYPES; i++) {
            nextCount[i] = 0;
        }
        
        printf("nextCount is not NULL.\n");
        
        //Subtract cell from count
        printf("Removing cell.\n");
        count[charToIndex(*ptr)]--; 
        //Add the previous cell to count
        printf("Adding the previous cell.\n");
        count[charToIndex(*(ptr-1))]++; 
    }
    else {
        //Reset count
        for(int i = 0; i < NUM_CELL_TYPES; i++) {
            count[i] = 0;
        }
        
        //Allocate space for nextCount
        nextCount = malloc(sizeof(int) * NUM_CELL_TYPES);
        
        //Reset nextCount
        for(int i = 0; i < NUM_CELL_TYPES; i++) {
            nextCount[i] = 0;
        }

        printf("nextCount is NULL.\n");
        
        //Check the col to the left of cell
        for(int row = -1; row < 2; row++) {
            printf("Checking (%d,-1) = %c\n", row, *(ptr - 1 + (WIDTH*row)));
            count[charToIndex(*(ptr - 1 + (WIDTH * row)))]++;            
        }
        
        //Check above and below cell, but not cell itself
        printf("Checking (-1,0) = %c\n", *(ptr-WIDTH));
        printf("Checking (1,0) = %c\n", *(ptr+WIDTH));
        //Add to count
        count[charToIndex(*(ptr - WIDTH))]++; //Check cell above
        count[charToIndex(*(ptr + WIDTH))]++; //Check cell below
        //Add to nextCount
        nextCount[charToIndex(*(ptr - WIDTH))]++; //Check cell above
        nextCount[charToIndex(*(ptr + WIDTH))]++; //Check cell below
    }
    //Check the col to the right of cell
    for(int row = -1; row < 2; row++) {
        printf("Checking (%d,1) = %c\n", row, *(ptr + 1 + (WIDTH*row)));
        count[charToIndex(*(ptr + 1 + (WIDTH * row)))]++;            
        nextCount[charToIndex(*(ptr + 1 + (WIDTH * row)))]++;            
    }
    
    return nextCount;
}

void printForest(char forest[HEIGHT][WIDTH]) {
    for(int row = 0; row < HEIGHT; row++) {
        for(int col = 0; col < WIDTH; col++) {
            printf("%c", forest[row][col]);
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
        //Add a border row above the forest
        for(int col = 0; col <= WIDTH; col++) {
            forest[0][col] = BORDER;
        }
        for(int row = ROW_START; row <= ROW_END && !feof(file); row++) {
            char* ptr = forest[row];

            //Add border to the left of this row of forest
            *ptr = BORDER;
            ptr += sizeof(char);

            //Read a whole row at a time
            fread(ptr, sizeof(char), COL_END, file);
            ptr += sizeof(char) * COL_END;

            //Eat the newline
            fscanf(file, "\n");

            //Add border to the right of this row of forest
            *ptr = BORDER;
        }
        //Add a border row below the forest
        for(int col = 0; col <= WIDTH; col++) {
            forest[HEIGHT-1][col] = BORDER;
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
    
    //Setup
    int NUM_ROWS = ROW_END / size;
    int base = NUM_ROWS * rank + ROW_START;
    int bound = base +  NUM_ROWS;

    //Make newForest
    char newForest [NUM_ROWS][WIDTH];
   
    //Test
    if(rank == 0) {
        printf("\n");
        int count[4] = {0,0,0,0};
        int* nextCount = NULL;

        printf("%c -> %d\n", BORDER, charToIndex(BORDER));
        printf("%c -> %d\n", TREE, charToIndex(TREE));
        printf("%c -> %d\n", FIRE, charToIndex(FIRE));
        printf("%c -> %d\n", SPACE, charToIndex(SPACE));

        nextCount = neighborCount(&count, NULL, &forest[1][1]);
        
        //Print count
        for(int i = 0; i < NUM_CELL_TYPES; i++) {
            printf("i:%d count[i]:%d\n", i , count[i]);
        }
        //Print nextCount
        for(int i = 0; i < NUM_CELL_TYPES; i++) {
            printf("i:%d nextCount[i]:%d\n", i , nextCount[i]);
        }
        
        printf("Second call to neighborCount.\n");
        neighborCount(&count, nextCount, &forest[1][2]);
        printf("\n");
        
        //Print count
        for(int i = 0; i < NUM_CELL_TYPES; i++) {
            printf("i:%d count[i]:%d\n", i , count[i]);
        }
        
    }

    #ifdef DEBUG
    //printf("Rank: %d [%d,%d).\n", rank, base, bound);
    #endif
    
    /*
    //Run the simulation
    for(int i=0; i < gen; i++) {
        for( int row = 0; row < NUM_ROWS; row++ ) {
            for(int col = 0; col < WIDTH; col++) {
                switch(forest[base + row][col]) {
                    case TREE:
                        //Check for burning neighbor(s)
                            if() {
                                //Set this tree on fire
                            }
                            else {
                                //Check ignition chance
                            }
                        break;
                    case FIRE:
                        //Burn this tree down
                        newForest[row][col] = SPACE;
                        break;
                    case SPACE:
                        //Check growth chance
                        break;
                }
            }
        }
    }
    */

    //Send all data to rank 0
    if(rank == 0) {
        //MPI_Recv()
    }
    else {
        //MPI_Send();
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
