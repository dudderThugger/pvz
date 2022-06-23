#ifndef JATEKMENET_H_INCLUDED
#define JATEKMENET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "jatekmenet.h"

/**
 *@file jatekmenet.h
 *@brief Tartalmazza a játékmenetért felelõs függvények deklarációját és a játékban használt struktúrák zömét
 */

/**
 *@brief Egy négyzet elhelyezkedését tárolja a pályán
 *
 */
// Egy pont a palyan, ezen lehetnek novenyek, lovedekek, vagy zombik
typedef struct Pont {
    /** A négyzet oszlop és sora*/
    int x, y;
} Pont;

/**
 *@brief Egy téglalap adatait tároló struktúra
 */
typedef struct Rects {
    /** Egy téglalap x, y koordinátája szélessége és magassága*/
    int x, y, w, h;
} Rects;

/**
 *@brief Egy zombi adatait tároló struktúra
 */
typedef struct Zombi {
    /** A zombi élete*/
    int hp;
    /** A zombi elhelyezkedése a pályán (melyik négyzetbe van)*/
    Pont pozicio;
    /** A listában következő zombi-ra mutató pointer*/
    struct Zombi* next;
    /** A lsitában ezt megelőző zombira mutató pointer*/
    struct Zombi* prev;
}Zombi;
/**
 *@brief A zombikat tartalmazó láncolt lista első elemére mutató pointer-ét tároló struktúra
 */
typedef struct Zombie_list {
    /** A zombik láncolt listájának első eleme*/
    Zombi* first;
} Zombie_list;

/**
 *@brief Egy lövedék struktúrája
 */
typedef struct Lovedek {
    /**Poziciója a pályán*/
    Pont pozicio;
    /** A listában következő lövedék pointere*/
    struct Lovedek* next;
    /** A listában  előző lövedék pointer-e*/
    struct Lovedek* prev;
} Lovedek;
/**
 *@brief A lövedékek láncolt listájának első elemének címét tároló struktúra
 */
typedef struct Lovedek_list {
    /** A lovedékek láncolt listájának első eleme*/
    Lovedek* first;
} Lovedek_list;

/**
 *@brief Egy peashooter adatait tartalmazó struktúra
 */
typedef struct Peashooter {
    /**A peashooter élete*/
    int hp;
    /**A peashooter lövéséig hátralévõ idõ*/
    int action_time;
    /**Poziciója a pályán, hányadik oszlop, hányadik sor*/
    Pont pozicio;
    /** A listában következő peashooterre mutató pointer*/
    struct Peashooter* next;
    /** A listában előző peashooterre mutató pointer*/
    struct Peashooter* prev;
} Peashooter;
/**
 *@brief A peashooterek láncolt listájának első elemét tartalmazó struktúra
 */
typedef struct Peashooter_list {
    /**A peashooterek láncolt listájának első eleme*/
    Peashooter* first;
} Peashooter_list;
/**
 *@brief Egy wallnut adatait tartalmazó struktúra
 */
typedef struct Wallnut {
    /** A wallnut aktuális élete*/
    int hp;
    /** A wallnut poziciója, oszlop, sor*/
    Pont pozicio;
    /** A következő wallnut-ra mutató pointer*/
    struct Wallnut* next;
    /** Az előző wallnut-ra mutató pointer*/
    struct Wallnut* prev;
} Wallnut;
/**
 *@brief A wallnut-okat tartalmazó láncolt lista
 */
typedef struct Wallnut_list {
    /** A wallnut-ok listájának első elemére mutató pointer*/
    Wallnut* first;
} Wallnut_list;
/**
 *@brief Egy napraforgó struktúrája
 */
typedef struct Sunflower {
    /** A napraforgó aktuális élete*/
    int hp;
    /** A napraforgó akciójáig mennyi idõ van*/
    int action_time;
    /** A napraforgó poziciója*/
    Pont pozicio;
    /** A lista következő eleme*/
    struct Sunflower* next;
    /** A lista előző eleme*/
    struct Sunflower* prev;
} Sunflower;
/**
 *@brief A sunflower-eket tartalmazó láncolt lista első elemét tartalmazó struktúra
 */
typedef struct Sunflower_list {
    /** A sunflower-ök láncolt istájának első eleme*/
    Sunflower* first;
} Sunflower_list;
/**
 *@brief A különbözõ növények láncol listáit tartalmazó struktúra
 */
typedef struct Novenyek {
    /** A peashooterek láncolt listája*/
    Peashooter_list peashooters_list;
    /** A wallnutok láncolt listája*/
    Wallnut_list wallnuts_list;
    /** A sunbflower-ek láncolt listája*/
    Sunflower_list sunflowers_list;
} Novenyek;

/**
 *@brief Egy játékmenet minden adatát tárolja
 *
 * Ezzel az egy struktúrával leírható a játék
 */
typedef struct Jatek {
    /** A játék téglalapjai, ezen vannak a növények*/
    Rects** palya;
    /** A sorok száma*/
    int sor;
    /** Az oszlopok száma*/
    int oszlop;
    /** Az ablak magassága*/
    int h;
    /** Az ablak szélessége*/
    int w;
    /** Mennyi ideje fut a játék*/
    int time;
    /** Hany eleted maradt*/
    int elet;
    /** Az aktuális pontszámod*/
    int pont;
    /** Az aktuális napocskáidnak a száma*/
    int napocska;
    /** A növények láncolt listáit tároló struktúra*/
    Novenyek novenyek;
    /** A zombik láncolt listája*/
    Zombie_list zombies_list;
    /** A lövedékek láncolt listája*/
    Lovedek_list lovedekek_list;
} Jatek;

bool uj_jatek(Jatek* game, double oslop, double sor, double w, double h);
void jatek_felszabadit (Jatek* game);
void jatek_kor(Jatek* game);
void game_start(char* nev);
void game_over(int pont, char* nev);

#endif // JATEKMENET_H_INCLUDED
