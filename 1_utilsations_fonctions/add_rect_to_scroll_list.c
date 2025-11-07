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
    (*p_rects_to_move)[size_of_list-1] = rect_to_add;
    (*p_rects_to_move)[size_of_list] = NULL;
}




int main(int argc, char* argv[]) {
    if(sdl_ttf_init_error()) return 0;

    SDL_Rect rect1 = {50,50,50,50};
    SDL_Rect rect2 = {50,150,50,50};
    SDL_Rect rect3 = {150,50,50,50};

    // rects_to_move = [&rect1; &rect2; &rect3, NULL] --> rects_to_move est un pointeur de pointeur(s) DONC un SDL_Rect**
    SDL_Rect** rects_to_move = NULL; // On l'initialise à NULL, on lui fera un malloc dans la fonction

    add_rect_to_scroll_list(&rect1, &rects_to_move);
    add_rect_to_scroll_list(&rect2, &rects_to_move);
    add_rect_to_scroll_list(&rect3, &rects_to_move);

    int test_x = rects_to_move[0]->x;
    int test_y = rects_to_move[0]->y;
    printf("hello %d %d\n", test_x, test_y);


    rects_to_move[0]->x = 300;
    printf("testing de bonne modification via rects_to_move x = %d\n", rect1.x);


    free(rects_to_move);

    return 0;
}
