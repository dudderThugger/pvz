#ifndef PLANTS_H_INCLUDED
#define PLANTS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "jatekmenet.h"
#include "debugmalloc.h"

/**
 *@file plants.h
 *@brief A növényekhez köthető függvények deklarációja található ebben a fileban
 */
/** A sunflower-ek akciójának ideje (hány képkockánként)*/
enum {SUNFLOWER_ACTION_TIME = 750};
/** A peashooter-ek akciójának ideje(hány képkockánként)*/
enum {PEASHOOTER_ACTION_TIME = 50};

void spawn_peashooter(Pont pont, Peashooter_list* peashooter_list);
void spawn_wallnut(Pont pont, Wallnut_list* wallnut_list);
void spawn_sunflower(Pont pont, Sunflower_list* sunflower_list);
void delete_peashooter(Peashooter* del, Peashooter_list* peashooter_list);
void delete_sunflower(Sunflower* del, Sunflower_list* sunflower_list);
void delete_wallnut(Wallnut* del, Wallnut_list* wallnut_list);
void plant_actions(Novenyek* novenyek, Lovedek_list* lovedek_list,Zombie_list* zombie_list, int width, int column, int* napocska);

#endif // PLANTS_H_INCLUDED
