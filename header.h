#ifndef HEADER_H
#define HEADER_H

#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <string.h>
#include <stdlib.h> 


#define PIECE_SIZE 100                   
#define NUMBER_OF_PIECES 3               
#define NUMBER_OF_PUZZLES 10              
#define SPACING 10                      


typedef struct {
    int x; 
    int y; 
} Point;


typedef struct {
    SDL_Surface* surface;  
    Point position;        
    bool isSelected;       
    char* filename;       
} PuzzlePiece; 

// Externally defined variables
extern PuzzlePiece pieces[NUMBER_OF_PIECES]; 
extern SDL_Surface* incompleteImage;           
extern SDL_Surface* screen;                    

// Function declarations
SDL_Surface* load_image(const char* file_path); 
void initGame();                                   
void drawPuzzle(SDL_Surface* screen);             
void handleInput(SDL_Event* event, bool* running, int puzzleIndex); 
int get_random_unused_puzzle();                   
void cleanup();                                    
bool isPieceCorrectlyPlaced(int puzzleIndex, Point piecePosition); 

#endif // HEADER_H
