#include <stdio.h>
#include <stdlib.h>
#include "jatekmenet.h"
#include "debugmalloc.h"
#include "plants.h"
#include "bullets.h"
/**
 * @file plants.c
 * @brief Ebben a fájlban tárolom a növényekhez köthető függvények definícióját
 */
/**
 * @brief A függvény megnézi, hogy van-e zombie az adott sorban
 * 
 * @param sor A leellenőrzött sor száma
 * @param zombie_list A zombikat tároló lista
 * @return true Igaz értékkel tér vissza, ha van zombi a sorban
 * @return false Hamis értékkel, ha nincs zombi a sorban
 */
bool van_zombie(int sor, Zombie_list* zombie_list);
/**
 * @brief Egy "peashooter" létrehozását végző függvény
 * 
 * @param pont A pont ahova a peashootert lerakjuk
 * @param peashooter_list A peashootereket tároló láncolt lista
 */
void spawn_peashooter(Pont pont, Peashooter_list* peashooter_list){
    Peashooter* uj = (Peashooter*) malloc (sizeof(Peashooter));
    uj->prev = NULL;
    if(peashooter_list->first == NULL){
        uj->next = NULL;
    } else {
        peashooter_list->first->prev = uj;
        uj->next = peashooter_list->first;
    }
    uj->pozicio = pont;
    uj->hp = 100;
    uj->action_time = PEASHOOTER_ACTION_TIME;
    peashooter_list->first = uj;
}
/**
 * @brief Egy wallnut létrehozását végző függvény
 * 
 * @param pont A pont ahova a wallnutot lerakjuk
 * @param wallnut_list A wallnutokat tároló láncolt lista
 */
void spawn_wallnut(Pont pont, Wallnut_list* wallnut_list){
    Wallnut* uj = (Wallnut*) malloc (sizeof(Wallnut));
    uj->prev = NULL;
    if(wallnut_list->first == NULL){
        uj->next = NULL;
    } else {
        wallnut_list->first->prev = uj;
        uj->next = wallnut_list->first;
    }
    uj->hp = 100;
    uj->pozicio = pont;
    wallnut_list->first = uj;
}
/**
 * @brief Egy sunflower létrehozását végző függvény
 * 
 * @param pont A pont ahova a sunflowert lerakjuk
 * @param sunflower_list A sunflowereket tároló láncol lista
 */
void spawn_sunflower(Pont pont, Sunflower_list* sunflower_list){
    Sunflower* uj = (Sunflower*) malloc (sizeof(Sunflower));
    uj->prev = NULL;
    if(sunflower_list->first == NULL){
        uj->next = NULL;
    } else {
        sunflower_list->first->prev = uj;
        uj->next = sunflower_list->first;
    }
    uj->hp = 10;
    uj->pozicio = pont;
    uj->action_time = PEASHOOTER_ACTION_TIME;
    sunflower_list->first = uj;
}
/**
 * @brief Egy peashooter törlését végző függvény
 * 
 * @param del A törlendő peashooterr mutató pointer
 * @param peashooter_list A peashootereket tároló láncolt lista
 */
void delete_peashooter(Peashooter* del, Peashooter_list* peashooter_list){
    if(del->prev == NULL) {
        if(del->next == NULL){
            peashooter_list->first = NULL;
        } else {
            peashooter_list->first = del->next;
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
 * @brief Egy sunflower törlését végző függvény
 * 
 * @param del A törlendő sunflowerre mutató pointer
 * @param sunflower_list A sunflowereket tároló láncolt lista
 */
void delete_sunflower(Sunflower* del, Sunflower_list* sunflower_list){
    if(del->prev == NULL) {
        if(del->next == NULL){
            sunflower_list->first = NULL;
        } else {
            sunflower_list->first = del->next;
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
 * @brief Egy wallnut törlését végző függvény
 * 
 * @param del A trlendő wallnutra mutató pointer
 * @param wallnut_list A wallnutokat tartalmazó láncolt lista
 */
void delete_wallnut(Wallnut* del, Wallnut_list* wallnut_list){
    if(del->prev == NULL) {
        if(del->next == NULL){
            wallnut_list->first = NULL;
        } else {
            wallnut_list->first = del->next;
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
 * @brief A növények akcióját végző függvény
 * Legelőször foglalkozik azzal, hogy törölni kell-e a növényt, majd a megmaradt növények akcióját végzi
 * 
 * @param novenyek A növények listáit tároló struktúra pointere
 * @param lovedek_list A lövedékeket tároló láncolt lista első eleme
 * @param zombie_list A zombikat tároló láncolt lista első elem
 * @param width Az ablak szélessége
 * @param column A pálya oszlopainak száma
 * @param napocska A napocskák számára mutató pointer
 */
void plant_actions(Novenyek* novenyek, Lovedek_list* lovedek_list,Zombie_list* zombie_list, int width, int column, int* napocska){
    Peashooter* iterP = novenyek->peashooters_list.first;
    Sunflower* iterS = novenyek->sunflowers_list.first;
    Wallnut* iterW = novenyek->wallnuts_list.first;
    while(iterP != NULL){
        if(iterP->hp <= 0){
            Peashooter* del = iterP;
            iterP = iterP->next;
            delete_peashooter(del,&novenyek->peashooters_list);
        }else{
        /** Lövés*/
            if(iterP->action_time == 0) {
                if(van_zombie(iterP->pozicio.y,zombie_list)){
                    Pont p = {.x = (width/column)*(iterP->pozicio.x)+30, .y = (iterP->pozicio.y)};
                    spawn_lovedek(p, lovedek_list);
                    iterP->action_time = PEASHOOTER_ACTION_TIME;
                }
            } else {
                iterP->action_time--;
            }
            iterP = iterP->next;
        }
    }
    while(iterS != NULL){
        if(iterS->hp <= 0){
            Sunflower* del = iterS;
            iterS = iterS->next;
            delete_sunflower(del,&novenyek->sunflowers_list);
        }else{
            if(*napocska < 9975){
                if(iterS->action_time == 0) {
                    *napocska += 25;
                    iterS->action_time = SUNFLOWER_ACTION_TIME;
                } else {
                    iterS->action_time -= 1;
                }
                iterS = iterS->next;
            }
        }
    }
    while(iterW != NULL){
        if(iterW->hp <= 0){
            Wallnut* next = iterW->next;
            delete_wallnut(iterW,&novenyek->wallnuts_list);
            iterW = next;
        } else {
            iterW = iterW->next;
        }
    }
}
bool van_zombie(int sor, Zombie_list* zombie_list){
    Zombi* iter = zombie_list->first;
    while(iter != NULL){
        if(iter->pozicio.y == sor){
            return true;
        }
        iter = iter->next;
    }
    return false;
}