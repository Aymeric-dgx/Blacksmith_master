// Vérifier qu'il est bien à jour

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "sqlite3.h"
#include "0_header.h"

/*
Liste des fonctions définies dans ce fichier

    - int sdl_ttf_ini_errer()
        --> Initilialise SDL+TTF (+SDL_image pour les .png et .jpg)
        --> Si aucune erreur, retourne 0. Sinon, retourne 1 et écrit dans le terminal un message d'erreur

    - void print_rect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color, int fill_it, int thickness)
        --> Déssine un SDL_Rect selon plusieurs options
        --> fill_it=1 et thickness=0 : remplit le rectangle
        --> fill_it=0 et thickness>0 : dessine un rectangle avec des bords d'une épaisseur = thickness
        --> s'il y'a une incohérence dans les parmètre (ex: fill_it=1 et thickness=5), renvoie u n message d'erreur

    - void write_text(SDL_Renderer* renderer, SDL_Rect rect, char* message, TTF_Font* font, SDL_Color color, int font_size)
        --> Ecrit un texte centré dans un rectangle donneé, de taille font_size

    - int click_in_rect(SDL_Event event, SDL_Rect rect)
        --> Renvoie 1 ou 0 en fonction de si oui ou non le clic a été fait dans le rectangle


    NB : pour les 3 fonctions qui vont suivres, nous allons utilisé une structure de liste suivant le shéma ci dessous.
    SDL_Rect** = [SDL_Rect*, SDL_Rect*, NULL]. Le NULL à la fin est très important, il marque la "fin" de la liste

    - void add_rect_to_scroll_list(SDL_Rect* rect_to_add, SDL_Rect*** p_rects_to_move)
        --> Ajoute le rect_to_add à la liste des rectangles à déplacer lors du scrolling, tout en conservant la strucuture de la liste
    
    - void remove_rect_from_scroll_list(SDL_Rect* rect_to_add, SDL_Rect*** p_rects_to_move)
        --> Supprime le rect_to_add à la liste des rectangles à déplacer lors du scrolling, tout conservant la strucutre de la liste
    
    - void scrolling(SDL_Event event, SDL_Rect** rects_to_move)
        --> Déplace tous les rectangles présents dans la liste, afin de faire comme du scrolling
*/



//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//




// Initialisation de SDL+TTF avec retour msg d'erreur. 
// Renvoie 0 si aucun problème, 1 sinon
int init_sdl_ttf() {
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
    //Initialisation de SDL_Image pour les .png et .jpg. Pour ajouter d'autre format, les rajouter dans le IMG_Init
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("Erreur IMG_Init: %s\n", IMG_GetError());
        return 1;
    }
}




//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//




// Déssiner un rectangle, avec 2 options : fill_it (le remplir) ou thickness (si on veut jsute des bords épais)
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




//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//




/*
Pour que le texte soit bien dimensionné, il faut respecter un ratio = hauteur/largeur tel que ratio = 3
On créé un msg_rect dimensionné en fonction du nombre de caractères et de la font_size souhaité, et 
    qui est centré dans le rect
*/
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





//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//






//Renvoie 1 si le click est dans le rectangle, 0 sinon
int click_in_rect(SDL_Event event, SDL_Rect rect) {
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            if(mx >= rect.x && mx <= rect.x+rect.w && my >= rect.y && rect.y <= rect.y+rect.h) {
                return 1;
            }
    }
    return 0;
}






//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//




// Ajoute le rect_to_add à la liste des rectangles à déplacer pour le scrolling (liste utilisé dans la fonction scrolling)
// avec OBLIAGTOIREMENT un NULL à la fin. Cette fonction respecte donc cette structure
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





//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//




// Supprime le rect_to_add à la liste des rectangles à déplacer pour le scrolling (liste utilisé dans la fonction scrolling)
// avec OBLIAGTOIREMENT un NULL à la fin. Cette fonction respecte donc cette structure
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
    }
        
}






//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//





// Utiliser les fonctions d'ajout et de suppression de la list des rect_to_move définit ci-dessus (!! ne pas le faire manuellement !!)
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
