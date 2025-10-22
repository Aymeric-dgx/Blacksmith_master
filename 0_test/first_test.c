#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "header.h"

int main(int argc, char* argv[]) {

    // Appelle de la fonction depuis le hedear.h
    init_playerr();

    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Texte SDL_ttf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger une police (tu dois avoir un fichier .ttf dans ton projet)
    TTF_Font* font = TTF_OpenFont("src/font/MedulaOne-Regular.ttf", 48);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

    // Créer la surface du texte
    SDL_Color white = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Salut SDL2 !", white);

    // Transformer la surface en texture
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);

    SDL_Rect messageRect = {100, 100, 300, 100}; // position et taille du texte

    // Boucle d’affichage
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, message, NULL, &messageRect);
    messageRect.y += 200;
    SDL_RenderCopy(renderer, message, NULL, &messageRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    // Nettoyage
    SDL_DestroyTexture(message);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
