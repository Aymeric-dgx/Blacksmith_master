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









void page_one(SDL_Renderer* renderer, int* page) {
     
    SDL_Rect fillRect = { 50, 50, 200, 200};
    SDL_Rect drawRect = { 400, 50, 200, 200};

    int running = 1;
    while(running) {
        
        //Gestion des événements
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                *page = 0;
                running = 0;
            }
            //Clic dans un rectangle
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;
                // Rectangle vide --> fermé la fenêtre
                // Rectangle rempli --> page 2
                if(mx >= drawRect.x && mx <= drawRect.x + drawRect.w &&
                   my >= drawRect.y && my <= drawRect.y + drawRect.h) {
                    *page = 0;
                    running = 0;
                    printf("Fermeture de la fenetre depuis la page 1, page = %d\n", *page);
                }
                else if(mx >= fillRect.x && mx <= fillRect.x + fillRect.w &&
                        my >= fillRect.y && my <= fillRect.y + fillRect.h) {
                    *page = 2;
                    running = 0;
                    printf("Passage de la page 1 a la page 2 page = %d\n", *page);
                }
            }
        }
        //Fin gestion des événements


        // Effacer l'écran avec une couleur de fond
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
        SDL_RenderClear(renderer);

        // Dessiner un rectangle rempli
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
        SDL_RenderFillRect(renderer, &fillRect);

        // Dessiner un rectangle vide
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
        SDL_RenderDrawRect(renderer, &drawRect);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }
}



void page_two(SDL_Renderer* renderer, int* page) {
     
    SDL_Rect drawRect = { 50, 50, 200, 200};
    SDL_Rect fillRect = { 400, 50, 200, 200};

    int running = 1;
    while(running) {
        
        //Gestion des événements
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                *page = 0;
                running = 0;
            }
            //Clic dans un rectangle
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;
                // Rectangle vide --> fermé la fenêtre
                // Rectangle rempli --> page 1
                if(mx >= drawRect.x && mx <= drawRect.x + drawRect.w &&
                   my >= drawRect.y && my <= drawRect.y + drawRect.h) {
                    *page = 0;
                    running = 0;
                    printf("Fermeture de la fenetre depuis la page 2, page = %d\n", *page);
                }
                else if(mx >= fillRect.x && mx <= fillRect.x + fillRect.w &&
                        my >= fillRect.y && my <= fillRect.y + fillRect.h) {
                    *page = 1;
                    running = 0;
                    printf("Passage de la page 2 a la page 1, page = %d\n", *page);
                }
            }
        }
        //Fin gestion des événements


        // Effacer l'écran avec une couleur de fond
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
        SDL_RenderClear(renderer);

        // Dessiner un rectangle rempli
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
        SDL_RenderFillRect(renderer, &fillRect);

        // Dessiner un rectangle vide
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
        SDL_RenderDrawRect(renderer, &drawRect);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }
}




// page 0 --> fermé fenetre
// page 1 --> page_one
// page 2 --> page_two


int main(int argc, char* argv[]) {
    // Initialisation de SDL + TTF (police)
    if(sdl_ttf_init_error()) return 0;

    // Récupération de la taille de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    //Création Window + Renderer
    SDL_Window* window = SDL_CreateWindow("Blacksmith Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    // Boucle principale
    int running = 1;
    int page = 1;
    while(running) {
        switch(page) {
            case 0 : running = 0; break;
            case 1 : page_one(renderer, &page); break;
            case 2 : page_two(renderer, &page); break;
        }        
    }

}
