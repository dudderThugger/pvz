#ifndef MEGJELENIT_H_INCLUDED
#define MEGJELENIT_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "jatekmenet.h"
#include "debugmalloc.h"
#include <SDL2/SDL_ttf.h>
/**
 *@file megjelenit.h
 *@brief Az egész játék megjelenítését végző függvények deklarációi
 */

/**
 *@brief A kijelölhető elemeket tartalmazza
 */
typedef enum SelectedItem {
    PEASHOOTER,
    SUNFLOWER,
    WALLNUT,
    NOTHING
} SelectedItem;

void draw_HUD(SDL_Renderer* renderer, TTF_Font* font, int life, int score, int time, int width, int napocska);
void draw_background(SDL_Renderer *renderer, Rects** palya, int const sor, int const oszlop);
void draw_peashooters(SDL_Renderer * renderer, Rects** palya, Peashooter_list* peashooter_list, SDL_Texture* texture);
Pont get_rect(Rects** palya, int sor, int oszlop, int width, int height, SDL_Rect* rect);
void draw_selectedItem(SDL_Renderer* renderer, SDL_Rect rect, SelectedItem selectedItem);
void draw_wallnuts(SDL_Renderer * renderer, Rects** palya, Wallnut_list* wallnuts_list, SDL_Texture* texture);
void draw_sunflowers(SDL_Renderer * renderer, Rects** palya, Sunflower_list* sunflower_list, SDL_Texture* texture);
void draw_zombies(SDL_Renderer* renderer, Rects** palya, Zombie_list* zombie_list, SDL_Texture* texture);
void draw_bullets(SDL_Renderer* renderer, Lovedek_list* lovedekek_list, int magas);
void draw_gameOver(SDL_Renderer* renderer, TTF_Font* font,int height, int width);

#endif // MEGJELENIT_H_INCLUDED
