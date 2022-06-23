#include <stdio.h>
#include <stdlib.h>
#include "jatekmenet.h"
#include "debugmalloc.h"
#include "plants.h"
#include "bullets.h"
/**
 *@file bullets.c
 *@brief A lövedékekhez kapcsolódó függvények definiciója
 *
 * A lövedékek mozgatásáért, idézéséért és törléséért felelős függvények
 */

 /**
  *@brief Egy lövedék idézése
  *@param poz A pont (téglalap helyzete) ahol a lövedék teremtődni fog
  *@param lovedekek_list A lövedékeket tartalmazó láncolt lista
  */
void spawn_lovedek(Pont poz, Lovedek_list* lovedek_list){
    Lovedek* uj = (struct Lovedek*) malloc (sizeof(struct Lovedek));
    uj->prev = NULL;
    uj->pozicio = poz;
    if(lovedek_list->first == NULL){
        uj->next = NULL;
    }else{
        lovedek_list->first->prev = uj;
        uj->next = lovedek_list->first;
    }
    lovedek_list->first = uj;
}
 /**
  *@brief Egy lövedék törlése
  *@param del A törlendő lövedékre mutató pointer
  *@param lovedekek_list A lövedékeket tartalmazó láncolt lista
  */
void lovedek_torol(Lovedek* del, Lovedek_list* lovedek_list) {
    if(del->prev == NULL) {
        if(del->next == NULL){
            lovedek_list->first = NULL;
        } else {
            lovedek_list->first = del->next;
            del->next->prev = NULL;
        }
    }else{
        if(del->next == NULL){
            del->prev->next = NULL;
        }
        else{
            del->prev->next = del->next;
            del->next->prev = del->prev;
        }
    }
    free(del);
}

/**
  *@brief Az összes lövedék mozgatása
  * A függvény először megnézi, hogy a növényünk a, zombit ért, ekkor töröljük
  * b, kiért-e a pálya szélességéből, ekkor szintén töröljük
  * c, vagy csak simán mozgatni kell, ilyenkor meg mozgatjuk
  *@param lovedekek_list A lövedékeket tartalmazó láncolt lista
  *@param zombik_list A zombikat tartalmazó láncolt lista
  *@param oszlop A pálya szélessege, ha ezen túl megy törölni kell
  */
void lovedek_mozog(Lovedek_list* lovedekek_list, Zombie_list* zombik_list, int szeles){
    Lovedek* iterL = lovedekek_list->first;
    bool mozog= true;
    while(iterL != NULL){
        Zombi* iterZ = zombik_list->first;
        while(iterZ != NULL && mozog){
            if(iterZ->pozicio.x - iterL->pozicio.x <= 8 && iterL->pozicio.y == iterZ->pozicio.y){
                Lovedek* del = iterL;
                iterL = iterL->next;
                lovedek_torol(del,lovedekek_list);
                iterZ->hp -= 1;
                mozog = false;
            }
            iterZ = iterZ->next;
        }
        if(mozog && iterL->pozicio.x >= szeles) {
            Lovedek* del = iterL;
            iterL = iterL->next;
            lovedek_torol(del, lovedekek_list);
            mozog = false;

        }
        if(mozog){
            iterL->pozicio.x += BULLET_SPEED;
            iterL = iterL->next;
        }
        mozog = true;
    }
}