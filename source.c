#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_PUZZLES 10 
#define NUMBER_OF_PIECES 3    
#define PIECE_SIZE 100       
#define SPACING 10          


PuzzlePiece pieces[NUMBER_OF_PIECES]; 
SDL_Surface* incompleteImage; 
const char* puzzleFolders[NUMBER_OF_PUZZLES] = {
    "puzzle1/",
    "puzzle2/",
    "puzzle3/",
    "puzzle4/",
    "puzzle5/",
    "puzzle6/",
    "puzzle7/",
    "puzzle8/",
    "puzzle9/",
    "puzzle10/"
};


bool usedPuzzles[NUMBER_OF_PUZZLES] = {false}; 

SDL_Surface* load_image(const char* file_path) {
    SDL_Surface* loadedImage = IMG_Load(file_path);
    if (loadedImage == NULL) {
        printf("Could not load image! SDL_image Error: %s\n", IMG_GetError());
        return NULL; 
    }
    SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
    SDL_FreeSurface(loadedImage); 
    return optimizedImage; 
}


int get_random_unused_puzzle() {
    int unusedPuzzleCount = 0;

    for (int i = 0; i < NUMBER_OF_PUZZLES; i++) {
        if (!usedPuzzles[i]) {
            unusedPuzzleCount++;
        }
    }

    
    if (unusedPuzzleCount == 0) {
        return -1; 
    }

    int index;
    do {
        index = rand() % NUMBER_OF_PUZZLES; 
    } while (usedPuzzles[index]);

    return index; 
}


void initGame() {
    srand(time(NULL)); 

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }

   
    int puzzleIndex = get_random_unused_puzzle();
    if (puzzleIndex == -1) {
        printf("All puzzles have been used! No more puzzles to load.\n");
        return; 
    }

    
    usedPuzzles[puzzleIndex] = true;

    
    char incompleteImagePath[256];
    snprintf(incompleteImagePath, sizeof(incompleteImagePath), "%s/incomplete.jpeg", puzzleFolders[puzzleIndex]);

    incompleteImage = load_image(incompleteImagePath);
    if (incompleteImage == NULL) {
        printf("Failed to load incomplete image from %s! SDL_image Error: %s\n", incompleteImagePath, IMG_GetError());
        return; 
    }
    printf("Successfully loaded incomplete puzzle image from %s\n", incompleteImagePath);

    
    const char* filenames[NUMBER_OF_PIECES] = {
        "correct.png",
        "wrong1.png",
        "wrong2.png"
    };

    int yOffset = 20; 
    for (int i = 0; i < NUMBER_OF_PIECES; i++) {
        char pieceImagePath[256];
        snprintf(pieceImagePath, sizeof(pieceImagePath), "%s/%s", puzzleFolders[puzzleIndex], filenames[i]);

        pieces[i].surface = load_image(pieceImagePath);
        if (pieces[i].surface == NULL) {
            printf("Failed to load piece image from %s!\n", pieceImagePath);
            return; 
        }

        pieces[i].filename = strdup(filenames[i]); 
        pieces[i].position.x = 600; 
        pieces[i].position.y = yOffset; 
        yOffset += PIECE_SIZE + SPACING; 

        SDL_SetColorKey(pieces[i].surface, SDL_TRUE, SDL_MapRGB(pieces[i].surface->format, 255, 255, 255));
    }
}


void drawPuzzle(SDL_Surface* screen) {
    SDL_BlitSurface(incompleteImage, NULL, screen, NULL); 
    for (int i = 0; i < NUMBER_OF_PIECES; i++) {
        SDL_Rect dest; 
        dest.x = pieces[i].position.x;
        dest.y = pieces[i].position.y;
        SDL_BlitSurface(pieces[i].surface, NULL, screen, &dest); 
    }
    SDL_Flip(screen); 
}


bool isPieceCorrectlyPlaced(int puzzleIndex, Point piecePosition) {
    SDL_Rect rect;
    rect.x = piecePosition.x;
    rect.y = piecePosition.y;
    rect.w = PIECE_SIZE;  
    rect.h = PIECE_SIZE;  
   
    switch (puzzleIndex) {
        case 0: 
            return (rect.x >= 0 && rect.x <= 100 && rect.y >= 150 && rect.y <= 450);
        case 1: 
            return (rect.x >= 150 && rect.x <= 300 && rect.y >= 200 && rect.y <= 400);
        case 2:
            return (rect.x >= 400 && rect.x <= 600 && rect.y >= 350 && rect.y <= 600);
        case 3: 
            return (rect.x >= 100 && rect.x <= 300 && rect.y >= 0 && rect.y <= 150);
        case 4:
            return (rect.x >= 50 && rect.x <= 150 && rect.y >= 100 && rect.y <= 300);
        case 5: 
            return (rect.x >= 200 && rect.x <= 300 && rect.y >= 150 && rect.y <= 350);
        case 6: 
            return (rect.x >= 300 && rect.x <= 400 && rect.y >= 200 && rect.y <= 400);
        case 7: 
            return (rect.x >= 450 && rect.x <= 550 && rect.y >= 250 && rect.y <= 450);
        case 8: 
            return (rect.x >= 600 && rect.x <= 700 && rect.y >= 300 && rect.y <= 500);
        case 9: 
            return (rect.x >= 700 && rect.x <= 800 && rect.y >= 350 && rect.y <= 550);
        default:
            return false; 
    }
}

void handleInput(SDL_Event* event, bool* running, int puzzleIndex) {
    static int selectedPieceIndex = -1;

    if (event->type == SDL_QUIT) {
        *running = false; 
    } else if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            *running = false; 
        }
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            int mouseX = event->button.x; 
            int mouseY = event->button.y;

            for (int i = 0; i < NUMBER_OF_PIECES; i++) {
                if (mouseX >= pieces[i].position.x &&
                    mouseX <= pieces[i].position.x + PIECE_SIZE &&
                    mouseY >= pieces[i].position.y && 
                    mouseY <= pieces[i].position.y + PIECE_SIZE) {
                    selectedPieceIndex = i;
                    printf("Piece %d selected!\n", i);
                    break; 
                }
            }
        }
    }

    if (event->type == SDL_MOUSEMOTION) {
        if (selectedPieceIndex != -1) {
            pieces[selectedPieceIndex].position.x = event->motion.x - PIECE_SIZE / 2;
            pieces[selectedPieceIndex].position.y = event->motion.y - PIECE_SIZE / 2;
        }
    }
    
    if (event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            if (selectedPieceIndex != -1) {
                printf("Piece %d placement finalized!\n", selectedPieceIndex);
                
                
                if (strcmp(pieces[selectedPieceIndex].filename, "correct.png") == 0 &&
                    isPieceCorrectlyPlaced(puzzleIndex, pieces[selectedPieceIndex].position)) {
                    printf("Correct piece placed! Ending game...\n");
                    *running = false; 
                } else {
                    printf("Wrong piece or wrong placement! Try again.\n");
                }

                // Reset selection
                selectedPieceIndex = -1; 
            }
        }
    }
}


void resetUsedPuzzles() {
    for (int i = 0; i < NUMBER_OF_PUZZLES; i++) {
        usedPuzzles[i] = false; 
    }
}
