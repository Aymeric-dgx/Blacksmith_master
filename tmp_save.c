#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "0_header.h"

// Création de la page "globale" (titre, solde, temps, onglets des pages, ...)



typedef struct joueur {
    int balance; // Solde
    int total_salary;
    int hour;
    char day[10];
} Player;


int main(int argc, char* argv[]) {
    // Initilisation SDL + TTF
    if(sdl_ttf_init_error()) return 1;

    //Importation de la police
     TTF_Font* font = TTF_OpenFont("src/font/MedulaOne-Regular.ttf", 500); //Mettre la size à au moins 300 pour avoir une police clair, non pixelisé
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

    // Récupération de la taille de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    //Création Window + Renderer
    SDL_Window* window = SDL_CreateWindow("Blacksmith Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Acitvation de l'option du renderer afin de pouvoir modifier l'opacité des couleurs (avec le a du RGBA)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);



    // Initialisation des strucutre (pour l'instant version temporaire)
    // Pour une version définitive, écrire une fonction qui initiliasie toute les strcuture via la bdd ?
    Player player = {1500, 100, 14, "Lundi"};

    
    // Création des "variables" globales (SDL_Rect, SDL_Color, ...)
    SDL_Color black = {0,0,0,255};
    SDL_Color background_color = {175, 76, 15, 160};
    SDL_Color stubborn_color = {175, 76, 15, 200};
    SDL_Color time_background_color = {250, 228, 183, 255};
    
    SDL_Rect background_rect = {0,0,screen.w, screen.h};
    SDL_Rect stubborn_rect = {0, 0, screen.w, screen.h/3.5}; // Rectangle d'en tête (+ foncé que le background)
    SDL_Rect title_rect = {0,0,screen.w, screen.h/(2*3.5)};
    SDL_Rect time_rect = {50, 50, 300, 75};
    SDL_Rect day_rect = {time_rect.x, time_rect.y, time_rect.w/2, time_rect.h};
    SDL_Rect hour_rect = {time_rect.x+time_rect.w/2, time_rect.y, time_rect.w/2.5, time_rect.h};
    SDL_Rect balance_rect = {screen.w-300, 50, 250, 75};
    SDL_Rect value_balance_rect = {balance_rect.x, balance_rect.y, 2*balance_rect.w/3, balance_rect.h};
    SDL_Rect icon_balance_rect = {balance_rect.x+value_balance_rect.w, balance_rect.y, balance_rect.w/3, balance_rect.h};


    // Boucle principale
    int page = 0;
    SDL_Event event;
    int running = 1;
    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
            // Si les flèches sont utilisées, on caugmente/diminue la valeur de 'a'
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_UP) {
                    //
                }
                if(event.key.keysym.sym == SDLK_DOWN) {
                    //
                }
            }
        }
        //Fin gestion des évenements

        // Nettoyage de la page en blanc
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderClear(renderer);

        // Peinture du background et du stubborn/en-tête + ecriture du titre
        print_rect(renderer, background_rect, background_color, 1, 0);
        print_rect(renderer, stubborn_rect, stubborn_color, 1, 0);
        write_text(renderer, title_rect, "Blacksmith Master", font, black, 40);

        // Peinture/Ecriture du temps + du solde du joueur
        print_rect(renderer, time_rect, time_background_color, 1, 0);
        print_rect(renderer, time_rect, black, 0, 4);
        write_text(renderer, day_rect, player.day, font, black, 18);
        char tmp_msg[20] = "";
        sprintf(tmp_msg, "%d : 00", player.hour);
        write_text(renderer, hour_rect, tmp_msg, font, black, 18);

        print_rect(renderer, balance_rect, time_background_color, 1, 0);
        print_rect(renderer, balance_rect, black, 0, 4);
        sprintf(tmp_msg, "%d", player.balance);
        write_text(renderer, value_balance_rect, tmp_msg, font, black, 18);


        // Actualisation de la page
        SDL_RenderPresent(renderer);
    }
}
