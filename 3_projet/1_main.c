#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "0_header.h"

// Création de la page "globale" (titre, solde, temps, onglets des pages, ...)

// NB pour ajouter à chacune des 5 pages l'en-tête sans trop charger le code --> Décalrer tout les rects, les foutte dans une liste et les passer en parametre à une fonction stubborn(...) ?
// Sinon tout foutre dans une fonction et la mettre dans le while, mais beaucoup d'intialisation de SDL_Rect "inutile" --> Recherhcer si cela conseomme beaucoup de ressource ou non ?

// RESULTAT APRES RECHERCHE
// L'intialisation des SDL_Rect ne coûte pratiquemment rien (du fait que c'est simplement créer une structure avec 4 int à assigner).
// Ainsi, on peut "tout" mettre dans une fonction stubborn(...) placé directemment dans le while(running)

// ATTENTION
// NB2 : par contre comment gérer les évenements, puisque l'on ne peut pas faire 2 boucle while(SDL_PollEvent(&event)) ?
// IDEE : Récupérer seulement les rects importants et les garder dans le main (ex : les rects pour switcher entre les pages)
// NB du IDEE : lors du dévellopemment, "marqer" les rects (ou variable) qui pourraient être interessants

typedef struct joueur {
    int balance; // Solde
    int total_salary;
    int hour;
    char day[10];
} Player;



int main(int argc, char* argv[]) {
    // Mise en place de SDL, TTF, du renderer, activation de certaines options, ...
    // Initilisation SDL + TTF
    if(init_sdl_ttf()) return 1;

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

    // Activation d'une option du renderer, qui lui permettra d'ajuster la valeur des caractéristiques des SDL_Rect (ou autre) en fonction de
    // la résolution de l'écran, en prenant comme référence pour le dévellopement un écran 1600x900 (résolution de mon écran)
    // Permet de coder avec des coordonées "fixes", sans se soucier de potentielles "déplacement" à cause d'une résolution différente
    int w_reference = 1600;
    int h_reference = 900;
    SDL_RenderSetLogicalSize(renderer, w_reference, h_reference);

    // Acitvation de l'option du renderer afin de pouvoir modifier l'opacité des couleurs (avec le a du RGBA)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);






    // Initialisation des structure (pour l'instant version temporaire)
    // Pour une version définitive, écrire une fonction qui initiliasie toute les strcuture via la bdd ?
    Player player = {1500, 100, 14, "Mer."};

    



    // Création des "variables" globales (SDL_Rect, SDL_Color, images, ...)
    int rects_thickness = 4;
    SDL_Color black = {0,0,0,255};
    SDL_Color white = {255,255,255,255};
    SDL_Color background_color = {175, 76, 15, 160};
    SDL_Color stubborn_color = {175, 76, 15, 200};
    SDL_Color time_background_color = {250, 228, 183, 255};
    
    SDL_Rect background_rect = {0,0,screen.w, screen.h};
    SDL_Rect stubborn_rect = {0, 0, screen.w, screen.h/3.5}; // Rectangle d'en tête (+ foncé que le background)
    SDL_Rect title_rect = {0,0,screen.w, stubborn_rect.h/2};
    SDL_Rect background_stubborn_separation_rect = {0, stubborn_rect.h-rects_thickness, screen.w, rects_thickness};

    SDL_Rect time_rect = {50, 50, 300, 75};
    SDL_Rect day_rect = {time_rect.x, time_rect.y, time_rect.w/2, time_rect.h};
    SDL_Rect hour_rect = {time_rect.x+time_rect.w/2, time_rect.y, time_rect.w/2.5, time_rect.h};

    SDL_Rect balance_rect = {screen.w-300, 50, 250, 75};
    SDL_Rect value_balance_rect = {balance_rect.x, balance_rect.y, 2*balance_rect.w/3, balance_rect.h};
    SDL_Rect icon_balance_rect = {balance_rect.x+value_balance_rect.w, balance_rect.y, balance_rect.w/3, balance_rect.h};

    SDL_Rect forge_switch_rect = {0, stubborn_rect.h-70, screen.w/5, 70};
    SDL_Rect shop_switch_rect = {forge_switch_rect.w, forge_switch_rect.y, forge_switch_rect.w, forge_switch_rect.h};
    SDL_Rect mine_switch_rect = {2*forge_switch_rect.w, forge_switch_rect.y, forge_switch_rect.w, forge_switch_rect.h};
    SDL_Rect stock_switch_rect = {3*forge_switch_rect.w, forge_switch_rect.y, forge_switch_rect.w, forge_switch_rect.h};
    SDL_Rect tree_switch_rect = {4*forge_switch_rect.w, forge_switch_rect.y, forge_switch_rect.w, forge_switch_rect.h};

    SDL_Rect correcting_switch_rect = {0, forge_switch_rect.y+rects_thickness, rects_thickness, forge_switch_rect.h-2*rects_thickness}; // NB : ici on fait y+4, w=4 etc car le rect à une epaissuer de bordure de 4 pixels (cf plus bas dans le code)

    SDL_Rect all_switch_rect[5] = {forge_switch_rect, shop_switch_rect, mine_switch_rect, stock_switch_rect, tree_switch_rect};
    char switch_rect_text[5][15] = {"Forge", "Magasin", "Mines", "Stocks", "Arbre"};

    // Boucle principale
    int page = 0;
    SDL_Event event;
    int running = 1;
    while(running) {
        // Gestion des évenements (NB : on ne peut pas avoir deux boucle while(SDL_PollEvent(&event)) )
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


        // Peinture du background et du stubborn/en-tête + ecriture du titre + peinture de la ligne de séparation entre les deux
        print_rect(renderer, background_rect, background_color, 1, 0);
        print_rect(renderer, stubborn_rect, stubborn_color, 1, 0);
        write_text(renderer, title_rect, "Blacksmith Master", font, black, 40);
        print_rect(renderer, background_stubborn_separation_rect, black, 1, 0);


        // Peinture/Ecriture du temps + du solde du joueur
        print_rect(renderer, time_rect, time_background_color, 1, 0);
        print_rect(renderer, time_rect, black, 0, rects_thickness);
        write_text(renderer, day_rect, player.day, font, black, 18);
        char tmp_msg[20] = "";
        sprintf(tmp_msg, "%d : 00", player.hour);
        write_text(renderer, hour_rect, tmp_msg, font, black, 18);

        print_rect(renderer, balance_rect, time_background_color, 1, 0);
        print_rect(renderer, balance_rect, black, 0, rects_thickness);
        sprintf(tmp_msg, "%d", player.balance);
        write_text(renderer, value_balance_rect, tmp_msg, font, black, 18);
        print_image(renderer, icon_balance_rect, "./src/image/gold_coin.png"); // Print de la pice d'or dans le solde


        // Peinture/Ecriture des onglets de naviguation entre les differentes page (Forge, Magasin, Mines, Stocks, Arbre)
        for(int i=0 ; i<5 ; i++) {
            print_rect(renderer, all_switch_rect[i], black, 0, rects_thickness);
            write_text(renderer, all_switch_rect[i], switch_rect_text[i], font, black, 18);
        }

        // Correction des doubles barres des switch_rect (et des switch_rect des extremité pour pas faire une bande noire contre l'écran)
        for(int i=0 ; i<6 ; i++) {
            if(i==5) correcting_switch_rect.x-= rects_thickness; // Cas particulier pour la denrière, sinon on print sur le "prochain rect" (qui n'existe pas) et pas la fin du dernier
            print_rect(renderer, correcting_switch_rect, stubborn_color, 1, 0);
            correcting_switch_rect.x += forge_switch_rect.w;
        }
        correcting_switch_rect.x = 0;




        // Actualisation de la page
        SDL_RenderPresent(renderer);
    }

    // Fermeture propre de SDL
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}
