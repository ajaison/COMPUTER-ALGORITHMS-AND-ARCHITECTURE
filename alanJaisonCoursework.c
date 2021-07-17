//Alan Jaison Quadtree Program for Computer Algorithms coursework
#include <stdio.h>
#include <stdlib.h>
typedef char bool;
#define true 1
struct Node;
#define false 0
#include <math.h>
#include <time.h>
#define LARGE_NUMBER 50000
void printBlackNodes();

typedef struct Node Node;
typedef enum colour colour;

enum colour {Black, White, Grey};
//Quadtree Node structure, Properties
struct Node{
    Node *NW, *NE, *SE, *SW;
    colour colour;
    unsigned int width;
    unsigned int x, y;
};



Node *newNode() {
    Node* node = malloc(sizeof(Node));
    node->NW = NULL;
    node->NE = NULL;
    node->SW = NULL;
    node->SE = NULL;

    node->colour = Grey;
    node->width = 0;
    node->x = 0;
    node->y = 0;

    return node;
}

//This function loops through the array to check if all the 'colours' values are the same

bool Checkcolour( int width,colour** numberArray, int x, int y){
     int i,j;
    colour colour = numberArray[x][y];

    for (i = x; i < x+width; ++i) {
        for (j = y; j < y+width; ++j) {
            if(numberArray[i][j] != colour)
                return false;
        }
    }
    return true;
}

/*This function will recursivly split through each section of the array and create a node. 
   If the colours are the same then it store it as White or Black, else as Grey */
Node* splitToSubtree(int width,colour** numberArray, int x, int y){
    Node* node = newNode();
 int i,j;

    if(Checkcolour(width, numberArray, x, y)){
        node->colour = numberArray[x][y];
	if (numberArray[x][y] == 1) {
	   node->colour = White ;
                    }
        else if (numberArray[x][y] == 0) {
           node->colour = Black; 
           }

    } else{
     //Recursive calls to split through the array until black/white nodes are found
        unsigned int newwidth = width/2;
        node->colour = Grey;
        node->NW = splitToSubtree(newwidth, numberArray, x,y);
        node->NE = splitToSubtree(newwidth, numberArray, x+newwidth,y);
        node->SW = splitToSubtree(newwidth, numberArray, x,y+newwidth);
        node->SE = splitToSubtree(newwidth, numberArray, x+newwidth,y+newwidth);
      }
    node->width = width;
    node->x = x;
    node->y = y;

    return node;
}


/*Passes array and variables to the splittig function with initial x and y values*/
Node *generateQuadTree(int width,colour **numberArray) {
    return splitToSubtree(width, numberArray,  0, 0);
}


Node *readingfile(const char *filepath) {
    int i, j;
    FILE* file = fopen(filepath, "r");
    char buffer[16];
    fgets(buffer, sizeof(buffer), file);

    char *ptr;
    long width = strtol(buffer, &ptr, 10);

    //error checking to ensure file format is correct
    if(width <= 0){
        fprintf(stderr, "File format error\n");
        exit(-1);
    }

    //Enter Data into Dynamic Array
    colour **numberArray = alloca(width*sizeof(colour*));
    for (i = 0; i < width; ++i) {
        numberArray[i] = alloca(width*sizeof(colour));
        for (j = 0; j < width; ++j) {
            numberArray[i][j] = White;
        }
    }
    fgets(buffer, sizeof(buffer), file);
    long blacks = strtol(buffer, &ptr, 10);

    for (i = 0; i < blacks; ++i) {
        fgets(buffer, sizeof(buffer), file);
        long x = strtol(buffer, &ptr, 10);
        long y = strtol(ptr, &ptr, 10);

    //error checking to ensure file format is correct
        if(x < 0 || y < 0){
            fprintf(stderr, "File format error\n");
            exit(-1);
        }
        numberArray[x][y] = Black;
    }


 return generateQuadTree(width, numberArray);
}

//Function that prints out location and size of black nodes
void printBlackNodes(Node* tree) {
    if(tree != NULL){
	
        if(tree->colour == Black){
            printf("Black terminal node at position (%d,%d) with width %d\n", tree->x, tree->y, tree->width);
        } else if(tree->colour == Grey){
            printBlackNodes(tree->NW);
            printBlackNodes(tree->NE);
            printBlackNodes(tree->SW);
            printBlackNodes(tree->SE);
        }
    }

}

void deleteTree(Node* tree) {
    if(tree != NULL){
        deleteTree(tree->NW);
        deleteTree(tree->NE);
        deleteTree(tree->SW);
        deleteTree(tree->SE);

        free(tree);
    }
}	

int main(int argc, char *argv[]){
    int display,width,i;
  //error checking to ensure files inputted correctly
  //  clock_t start = clock(); 
  //  for (i=0; i<LARGE_NUMBER; i++)   { 
     if(argc != 2){
        fprintf(stderr, "Enter in this format: %s <file_path> ", argv[0]);
        return -1;
    }

 //Code used to find time taken to complete program (Testing)

    Node* tree = readingfile(argv[1]);

    printBlackNodes(tree);
    deleteTree(tree);
//    }
  //   clock_t end = clock();  
 //    printf("Running-time (secs) for %d repeats: %f\n", LARGE_NUMBER, (double) (end-start) / (double) CLOCKS_PER_SEC); 
     return 0;
}







