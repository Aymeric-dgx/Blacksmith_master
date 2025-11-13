#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "0_header.h"

int main(int argc, char* argv[]) {
    if(init_sdl_ttf()) return 1;

    // On initialise le SDL_image.h pour qu'il supporte les .png et les .jpg
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("Erreur IMG_Init: %s\n", IMG_GetError());
        return 1;
    }

    // Récupération de la taille de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    //Création Window + Renderer
    SDL_Window* window = SDL_CreateWindow("Blacksmith Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *surface = IMG_Load("./src/image/test_image.jpg");
    if (!surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // plus besoin de la surface
    if (!texture) {
        printf("Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Rect dstRect = { 100, 100, 400, 300 }; // position et taille
    SDL_RenderClear(renderer);                 // efface l’écran
    SDL_RenderCopy(renderer, texture, NULL, &dstRect); // dessine la texture
    SDL_RenderPresent(renderer);

    SDL_Delay(3000); // attendre 3 secondes

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
