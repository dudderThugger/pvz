#ifndef ZOMBIE_H_INCLUDED
#define ZOMBIE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "jatekmenet.h"
#include "debugmalloc.h"
#include <time.h>
#include <math.h>
/**
 *@file zombie.h
 *@brief A zombikhoz tartozó függvények deklarációja
 *
 */
/**A zombik hány pixelt mozognak másodpercenként 50-szer*/
enum{ZOMBIE_MOVE = 1};
/** A zombik életereje*/
enum{ZOMBIE_HP = 7};
/** Hány másodpercenként spawnoljunk zombikat*/
enum{ZOMBIE_SPAWN_TIME = 15};
void zombie_actions(Zombie_list* zombie_list, Novenyek* novenyek, Rects** palya, int* life,int* score);
bool van_noveny (Pont p, Novenyek* novenyek, Rects** palya, Peashooter** pea, Wallnut** wall, Sunflower** sun);
void zombie_spawner (int time, Zombie_list* zombie_list, int width, int sor);
#endif // ZOMBIE_H_INCLUDED