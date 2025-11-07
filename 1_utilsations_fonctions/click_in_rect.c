#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>



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

void print_rect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color, int fill_it, int thickness) {
    // Si on veut remplir le rectangle
    if(fill_it==1 && thickness==0) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }
    // Si on veut un rectangle avec des bords d'épaisseur thickness
    else if(fill_it==0 && thickness>0) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        // On draw thickness fois un rectangle un peu plus petit à chaque fois, pour simuler des bords épais
        for(int i=0 ; i<thickness ; i++) {
            SDL_RenderDrawRect(renderer, &rect);
            rect.x += 1; 
            rect.y += 1;
            rect.w -= 2;
            rect.h -= 2;
        }
    }
    // Si les paramètres sont incohérents, retourner un message d'erreur
    else { 
        printf("Erreur dans les paramètres de la fonction print_rect du rect ??? = {%d, %d, %d, %d}\n", rect.x, rect.y, rect.w, rect.h);
        printf("Veuillez entrez dans les paramètres soit \nfill_it=0 ET thickness=X (avec X>0) OU\nfill_it=1 ET thickness=0\n");
    }
}


//Renvoie 1 si le click est dans le rectangle, 0 sinon
int click_in_rect(SDL_Event event, SDL_Rect rect) {
    if(event.type == SDL_MOUSEBUTTONDOWN && event.type == SDL_BUTTON_LEFT) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        if(mx >= rect.x && mx <= rect.x+rect.w && my >= rect.y && rect.y <= rect.y+rect.h) {
            return 1;
        }
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

    // Charger une police
    TTF_Font* font = TTF_OpenFont("src/font/MedulaOne-Regular.ttf", 300); //Mettre la size à au moins 300 pour avoir une police clair, non pixelisé
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }


    // Déclarations des variables (SDL_rect, running, SDL_Color, ...)
    SDL_Rect rect = {50, 50, 400, 200};
    SDL_Rect rect2 = {500, 50, 400, 200};

    SDL_Color white = {255,255,255,255};

    SDL_Event event;
    int running = 1;
    while(running) {
        //Gestion des évements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
            //------------------ DEBUT Testing -----------------------//
            if(click_in_rect(event, rect)) {
                running = 0;
                printf("Closed from the filled rect\n");
            }

            if(click_in_rect(event, rect2)) {
                running = 0;
                printf("Closed from the drawn rect\n");
            }
          //---------------------- FIN Testing ----------------------//
        }
        // Fin gestion des événements

        // Remplissage du fond en noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        print_rect(renderer, rect, white, 1, 0);
        print_rect(renderer, rect2, white, 0, 25);
        
        SDL_RenderPresent(renderer);
    }
}
