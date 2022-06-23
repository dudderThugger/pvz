#include <stdio.h>
#include <stdlib.h>
#include "jatekmenet.h"
#include "debugmalloc.h"
#include "plants.h"
#include "bullets.h"
#include "zombie.h"
/**
 * @file zombie.c
 * @brief A zombikhoz köthető függvények definíciója található ebben a fájlban 
 */
/**
 * @brief Ez a függvény megnézi, hogy egy adott helyen
 * 
 * @param p A pont, ahol meg kell nézni, hogy van-e növény (a pont y koordináta a sor sorszámát jelöli, az x koordináta pedig a szélessgi helyzetét)
 * @param novenyek A növények lánolt listáját tartalmazó struktúra
 * @param palya A pálya téglalapjait tartalmazó dinamikus tömbökre mutató pointer
 * @param pea Ha a téglalapon egy peashooter van, akkor ebbe a pointerbe fogjuk beírni a címét 
 * @param wall Ha a téglalapon egy wallnut van, akkor ebbe a pointerbe fogjuk beírni a címét
 * @param sun Ha a téglalapon egy sunflower van, akkor ebbe a pointerbe fogjuk beírni a címét
 * @return true Visszatérési érték igaz, ha volt növény az adott pontban
 * @return false Hamis a visszatérési érték, amikor nem volt növény a pontban
 */
bool van_noveny (Pont p, Novenyek* novenyek, Rects** palya, Peashooter** pea, Wallnut** wall, Sunflower** sun){
    Peashooter* iterP = novenyek->peashooters_list.first;
    Sunflower* iterS = novenyek->sunflowers_list.first;
    Wallnut* iterW = novenyek->wallnuts_list.first;
    while(iterP!=NULL){
        if(iterP->pozicio.y == p.y && fabs(p.x - palya[iterP->pozicio.y][iterP->pozicio.x].x) <= 15){
            *pea = iterP;
            return true;
        }
        iterP = iterP->next;
    }
    while(iterS!=NULL){
        if(iterS->pozicio.y == p.y && fabs(p.x - palya[iterS->pozicio.y][iterS->pozicio.x].x) <= 15){
            *sun = iterS;
            return true;
        }
        iterS = iterS->next;
    }
    while(iterW!=NULL){
        if(iterW->pozicio.y == p.y && fabs(p.x - palya[iterW->pozicio.y][iterW->pozicio.x].x) <= 15){
            *wall = iterW;
            return true;
        }
        iterW = iterW->next;
    }
    return false;
}
/**
 * @brief Egy zombi törlését végző függvény
 * 
 * @param del A törlendő zombira mutató pointer
 * @param zombie_list A zombikat tartalmazó láncolt lista
 */
void delete_zombie(Zombi* del, Zombie_list* zombie_list){
    if(del->prev == NULL) {
        if(del->next == NULL){
            zombie_list->first = NULL;
        } else {
            zombie_list->first = del->next;
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
 * @brief Egy zombi teremtéséért felelős függvény
 * 
 * @param zombie_list A zombikat tartalmazó láncolt lista
 * @param pont A pont, ahova a zombit teremteni szeretnénk
 */
void spawn_zombie(Zombie_list* zombie_list, Pont pont){
    Zombi* uj = (Zombi*) malloc (sizeof(Zombi));
    uj->prev = NULL;
    if(zombie_list->first == NULL){
        uj->next = NULL;
    } else {
        zombie_list->first->prev = uj;
        uj->next = zombie_list->first;
    }
    uj->hp = ZOMBIE_HP;
    uj->pozicio = pont;
    zombie_list->first = uj;
}
/**
 * @brief Az idő függvényében zombikat spawnol a pálya szélére
 * 
 * @param time Az aktuális idő másodpercben
 * @param zombie_list A zombikat tartalmazó láncolt lista
 * @param width Az ablak szélessége
 * @param sor A pálya sorinak száma
 */
void zombie_spawner (int time, Zombie_list* zombie_list, int width,int sor){
    int random = rand()%3;
    for(int i = 0; i < (time/ZOMBIE_SPAWN_TIME) + 1; ++i){
        Pont pont = {.x = width, .y = (random+i)%sor};
        spawn_zombie(zombie_list, pont);
    }
}
/**
 * @brief A zombik akcióját végző függvény
 * 
 * @param zombie_list A zombikattartalmazó láncolt lista
 * @param novenyek A növények láncolt listáit tartalmazó struktúra
 * @param palya A pálya téglalapjainak dinamikus tömbjeira mutató pointer
 * @param life Az aktuális életpontunkra mutató pointer
 * @param score Az eddig szerzett pontjaink száma
 */
void zombie_actions(Zombie_list* zombie_list, Novenyek* novenyek, Rects** palya, int* life, int* score) {
    Zombi* iter = zombie_list->first;
    bool move = true;
    while(iter != NULL){
        if(iter->hp <= 0){
            Zombi* del = iter;
            iter = iter->next;
            delete_zombie(del, zombie_list);
            move = false;
            *score += 1;
        }
        Peashooter* p = NULL;
        Wallnut* w = NULL;
        Sunflower* s = NULL;
        if(move && van_noveny(iter->pozicio, novenyek, palya,&p,&w,&s)){
            move = false;
            if(p != NULL)
                p->hp -= 1;
            else if(w != NULL)
                w->hp -= 1;
            else if(s != NULL)
                s->hp -= 1;
            iter = iter->next;
        }
        if(move && iter->pozicio.x < 0) {
            Zombi* del = iter;
            iter = iter->next;
            delete_zombie(del, zombie_list);
            move = false;
            *life -= 1;
        }
        if(move){
            iter->pozicio.x -= ZOMBIE_MOVE;
            iter = iter->next;
        }
        move = true;
    }
}