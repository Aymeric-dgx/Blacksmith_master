#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "sqlite3.h"


// Renvoie 0 si aucun problème, sinon renvoie 1 + message d'erreur
int sdl_ttf_init_error() {
    // Initiralisation de SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }
    //Initialisation de TTF
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    
    //Initialisation de SDL+TTF
    if(sdl_ttf_init_error()) return 0;

    // Récupération de la taille de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    //Création Window + Renderer
    SDL_Window* window = SDL_CreateWindow("Blacksmith Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    
    SDL_Rect first_rect = {50, 50, 200, 200};
    SDL_Rect second_rect = {300, 50, 200, 200};

    //Boucle principale
    int running = 1;
    int i=0;
    SDL_Event event;
    int time_start = SDL_GetTicks(), current_time=0;

    while(running) {

        //Gestion des évements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
        } 
        // Fin gestion des événements

        // Toute les 5 secondes, deplacer le seconde rectangle de +100 pixels en x
        
        current_time = SDL_GetTicks();
        if(current_time - time_start >= 3000) {
            second_rect.x += 100;
            time_start = current_time;
        }
        
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Blanc
        SDL_RenderClear(renderer);

        // Dessiner les rectangles
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Bleu
        SDL_RenderFillRect(renderer, &first_rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
        SDL_RenderFillRect(renderer, &second_rect);

        SDL_RenderPresent(renderer);

        // Attendre 500 ms
        SDL_Delay(500);
    }
}
