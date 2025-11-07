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

void add_rect_to_scroll_list(SDL_Rect* rect_to_add, SDL_Rect*** p_rects_to_move) {
    // Si rects_to_move est à NULL, l'initialiser pour qu'il point vers une zone mémoire de taille 1xSDL_Rect* (+ &ère valeur à NULL)
    if(*p_rects_to_move == NULL) {
        *p_rects_to_move = malloc(sizeof(SDL_Rect*));
        if(*p_rects_to_move == NULL) {
            perror("Erreur de maloc dans la fonction add_rect_to_scroll_list ");
            exit(EXIT_FAILURE);
        }
        (*p_rects_to_move)[0] = NULL; // 1ère valeur du tableau à NULL
    }

    // On compte le nb d'éléments de la liste 
    int size_of_list = 0;
    while((*p_rects_to_move)[size_of_list] != NULL) size_of_list++;
    size_of_list++; // Afin d'avoir la taille "réelle" de la liste

    // Réallocation de la mémoire, et insertion en fin de liste de l'adresse de rect_to_add puis du NULL pour indiquer la "fin"
    *p_rects_to_move = realloc(*p_rects_to_move, sizeof(SDL_Rect*)*size_of_list);
    if(*p_rects_to_move == NULL) {
            perror("Erreur de realloc dans la fonction add_rect_to_scroll_list ");
            exit(EXIT_FAILURE);
        }
    (*p_rects_to_move)[size_of_list-1] = rect_to_add;
    (*p_rects_to_move)[size_of_list] = NULL;
}

void remove_rect_from_scroll_list(SDL_Rect* rect_to_remove, SDL_Rect*** p_rects_to_move) {
    int index = 0;

    // Parcours de la liste jusqu'à tomber sur le rect à supprimer --Resutlat--> size_of_list pointe l'élément à supprimer (ni avant, ni après)
    while((*p_rects_to_move)[index] != rect_to_remove && (*p_rects_to_move)[index] != NULL) index++;


    // Si on a atteint la fin de la liste (càd le NULL) alors l'élément n'était pas dans la liste de base
    if((*p_rects_to_move)[index] == NULL) {
        printf("Un des rectangle de la liste remove_rect_from_scroll_list était déja absent de base\n");
        return;
    }
    // Sinon on déroule l'algorythme "de base"
    else {
        do {
            (*p_rects_to_move)[index] = (*p_rects_to_move)[index+1];
            index++;
        } while((*p_rects_to_move)[index] != NULL);

        // On a donc rects_to move = [rect1, rect2, rect4, NULL, NULL] (par exemple)
        // On libère le 2eme NULL, et on resize la liste à la taille index
        // En effet, index pointe le last élément de la old list, donc index = old_size-1, et DONC index = new_size
        free((*p_rects_to_move)[index]);
        *p_rects_to_move = realloc(*p_rects_to_move, sizeof(SDL_Rect*)*(index));
        if(*p_rects_to_move == NULL) {
            perror("Erreur de realloc dans la fonction add_rect_to_scroll_list ");
            exit(EXIT_FAILURE);
        }
    }
        
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
        printf("Erreur dans les parametres de la fonction print_rect du rect ??? = {%d, %d, %d, %d}\n", rect.x, rect.y, rect.w, rect.h);
        printf("Veuillez entrez dans les parametres soit \nfill_it=0 ET thickness=X (avec X>0)\n OU\nfill_it=1 ET thickness=0\n");
    }
}




void scrolling(SDL_Event event, SDL_Rect** rects_to_move) {
    if(event.type == SDL_MOUSEWHEEL) {
        int gap = event.wheel.y*10;
        int index_in_list = 0;
        while(rects_to_move[index_in_list] != NULL) {
            rects_to_move[index_in_list]->y += gap;
            index_in_list++;
        }
    }
}







int main(int argc, char* argv[]) {
    if(sdl_ttf_init_error()) return 0;

    // Récupération de la taille de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    //Création Window + Renderer
    SDL_Window* window = SDL_CreateWindow("Blacksmith Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    SDL_Rect rect1 = {50,50,600,200};
    SDL_Rect rect2 = {50,300,600,200};
    SDL_Rect rect3 = {50,550,600,200};

    SDL_Color black = {0,0,0,255};

    SDL_Rect** rects_to_move = NULL;
    add_rect_to_scroll_list(&rect1, &rects_to_move);
    add_rect_to_scroll_list(&rect2, &rects_to_move);
    add_rect_to_scroll_list(&rect3, &rects_to_move);

    remove_rect_from_scroll_list(&rect2, &rects_to_move);
    

    SDL_Event event;
    int running = 1;
    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
            scrolling(event, rects_to_move);
        }
        // Fin gestion évenements

        // Ecriture des rectangles
        SDL_SetRenderDrawColor(renderer, 150,50,255,255);
        SDL_RenderClear(renderer);

        print_rect(renderer, rect1, black, 1, 0);
        print_rect(renderer, rect2, black, 0, 5);
        print_rect(renderer, rect3, black, 1, 0);

        SDL_RenderPresent(renderer);
    }

    return  0;
}
