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


void write_text(SDL_Renderer* renderer, SDL_Rect rect, char* message, TTF_Font* font, SDL_Color color, int font_size) {
    int nb_char = 0;
    while(message[nb_char] != '\0') nb_char++;
    SDL_Rect msg_rect;

    //Dimensionnement de msg_rect + placement dans rect
    msg_rect.h = font_size*3;
    msg_rect.w = font_size * nb_char;
    msg_rect.x = rect.x + rect.w/2 - msg_rect.w/2;
    msg_rect.y = rect.y + rect.h/2 - msg_rect.h/2;

    // Une fois fais, on écrit le texte
    SDL_Surface* surface_message = TTF_RenderText_Solid(font, message, color);
    SDL_Texture* final_message = SDL_CreateTextureFromSurface(renderer, surface_message);
    SDL_FreeSurface(surface_message);
    SDL_RenderCopy(renderer, final_message, NULL, &msg_rect);
    SDL_DestroyTexture(final_message);
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
    SDL_Rect rect = {50, 50, 800, 200};

    SDL_Color white = {255,255,255,255};

    SDL_Event event;
    int running = 1;
    while(running) {
        //Gestion des évements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
            //Si flèche du haut ou du bas, changer taille de firs_rect en "live"
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_UP) {
                    //
                }
                if(event.key.keysym.sym == SDLK_DOWN) {
                    //
                }
            }
        }
        // Fin gestion des événements

        // Remplissage du fond en noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);


        //-----------------------------Partie testing------------------------------//

        write_text(renderer, rect, "h", font, white, 100);
        rect.y += 100;
        write_text(renderer, rect, "Hello mon big bro", font, white, 25);
        rect.y -= 100;

        //---------------------------Fin partie testing----------------------------//
        
        SDL_RenderPresent(renderer);
    }
}
