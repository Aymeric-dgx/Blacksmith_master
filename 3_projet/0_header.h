// Fichier header pour les définitions globales et les inclusions communes
#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Fonctions "outils" pour SDL (de 0_SDL_personal_functions.c)
int init_sdl_ttf();
void print_rect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color, int fill_it, int thickness);
void write_text(SDL_Renderer* renderer, SDL_Rect rect, char* message, TTF_Font* font, SDL_Color color, int font_size);
int click_in_rect(SDL_Event event, SDL_Rect rect);
void add_rect_to_scroll_list(SDL_Rect* rect_to_add, SDL_Rect*** p_rects_to_move);
void remove_rect_from_scroll_list(SDL_Rect* rect_to_remove, SDL_Rect*** p_rects_to_move);
void scrolling(SDL_Event event, SDL_Rect** rects_to_move);




#endif // HEADER_H
