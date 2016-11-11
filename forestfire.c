#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TREE 'T'
#define FIRE 'X'
#define SPACE ' '
#define BORDER '%'

#define WIDTH 82    //80 cols + 1 Border left and right
#define HEIGHT 42   //60 rows + 1 Border above and below
#define COL_START 1
#define COL_END 80
#define ROW_START 1
#define ROW_END 40

#define DEBUG

/*
//Requires the acual row and col numbers
void neighborCount(int* count, char condition, int row, int col) {
    int code  = 0;
    
    //Reset counter
    *count = 0;
    
    //Above and below
    if(row == 0 ) {
        //Don't check above
        code += 1;
        
    }
    else if(row == HEIGHT-1 ) {
        //Don't check below
        code += 8;
    }
    
    //Left and right
    if(col == WIDTH-1) {
        //Don't check right
        code += 2;
    }
    else if(col == 0) {
        //Don't check left
        code += 4;
    }
    
    switch(code) {
        case 0:

        case 1:

        case 3:
        case 5:
        
        case 2:
        case 4:

        case 8:
        
        case 10:
        case 12:
    }
    
    if(code % 2 != 0) {
        //Bottom
        
    }
    else {
        //Top is blocked
        
    }
}
*/

/*
void printForest(char forest[HEIGHT][WIDTH]) {
    #ifdef DEBUG
    //Print column number
    printf("   ");
    for(int i = 0; i < COL_END/10; i++) {
        for(int j = 0; j < 10; j++) {
            printf("%d", i);
        }
    }
    printf("\n");
    printf("   ");
    for(int i = 0; i < COL_END/10; i++) {
        for(int j = 0; j < 10; j++) {
            printf("%d", j);
        }
    }
    printf("\n");
    #endif
    
    //Print the actual data
    for(int row = ROW_START; row <= ROW_END; row++) {
        #ifdef DEBUG
        printf("%02d_", row);
        #endif
        //printf("%s\n", forest[row]);
        
        for(int col = COL_START; col <= COL_END; col++) {
            if(col == COL_START || col == COL_END) {
                printf(" (%02d, %02d)", row, col);
            }
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
*/

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
