#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <stdbool.h>
#include "header.h"


SDL_Surface* screen = NULL;
PuzzlePiece pieces[NUMBER_OF_PIECES]; 
SDL_Surface* incompleteImage = NULL; lobal


void initGame();
void handleInput(SDL_Event* event, bool* running, int puzzleIndex); 

int main(int argc, char* argv[]) {
    (void)argc; 
    (void)argv; 

    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    screen = SDL_SetVideoMode(800, 1000, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Screen could not be set! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    initGame();

    // Main loop
    bool running = true;
    SDL_Event event;

    int currentPuzzleIndex = get_random_unused_puzzle(); 
    if (currentPuzzleIndex == -1) {
        printf("No puzzles available. Exiting...\n");
        return 1;
    }

    while (running) {
        while (SDL_PollEvent(&event)) {
            handleInput(&event, &running, currentPuzzleIndex);
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

        drawPuzzle(screen);
        SDL_Flip(screen);
    }

   
    if (incompleteImage) {
        SDL_FreeSurface(incompleteImage); 
    }

    for (int i = 0; i < NUMBER_OF_PIECES; i++) {
        if (pieces[i].surface) {
            SDL_FreeSurface(pieces[i].surface);
        }
        free(pieces[i].filename); 
    }
    
    
    SDL_FreeSurface(screen); 
    IMG_Quit(); 
    SDL_Quit(); 
    return 0;
}
