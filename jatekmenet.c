#include <stdio.h>
#include <stdlib.h>
#include "jatekmenet.h"
#include "debugmalloc.h"
#include "plants.h"
#include "bullets.h"
#include "zombie.h"

/**
 *@file jatekmenet.c
 *@brief A játék állásának manipulálásához köthetõ legfontosabb függvényeket tárolja
 *
 * A játék létrehozását és mentését, a dicsõséglista irányítását, a menü irányítását,
 * a játék által használt dinamikus tömbök felszabadítását végzõ függvények mind itt találhatóak meg
 */
/**
 *@brief A játék legelején ez a függvény végzi el a játék kezdéséhez szükséges lépéseket
 *
 * Létrehozza a pálya téglalapjait tároló dinamikus tömböt és feltölti a téglalapok adataival
 * Beállítja a kezdő NULL értékeket a láncolt lista első elemének
 * Beállítja a kezdõértékeket (pl. idõ, pont)
 *@param game A játékot tartalmazó struktúra, amit módosítunk
 *@param oszlop Az oszlopok száma (azért valós, hogy az osztásnál elkerüljük az esetleges kerekítéseket)
 *@param sor A sorok száma
 *@param width Az ablak szélessége
 *@param height Az ablak magassága
 */
bool uj_jatek (Jatek* game, double oszlop, double sor, double width, double height) {
    bool sikeres = true;
    // Palya letrehozasa
    game -> sor = sor;
    game -> oszlop = oszlop;
    game -> w = width;
    game -> h = height;
    game -> palya = (struct Rects**) malloc(game -> sor * sizeof(struct Rects**));
    if(game -> palya == NULL)
        sikeres = false;
    for(int i = 0; i < game -> sor; ++i) {
        game -> palya[i] = (struct Rects*) malloc(game -> oszlop * sizeof(struct Rects));
        if(game -> palya[i] == NULL)
            sikeres = false;
    }
    if(game -> palya[0] != NULL){
            for(int i = 0; i < sor; ++i){
                for(int j = 0; j < oszlop; ++j) {
                    Rects akt = {.x = ((width/oszlop) * j), .y = ((height/sor) * i)+60, .w = (width/oszlop), .h = (height/sor)};
                    game -> palya[i][j] = akt;
                }
            }
    }

    game -> time = 0;
    game -> pont = 0;
    // Letrehozunk egy cimet a dinamikus tomboknek (a kesobbi fuggvenyekben majd az atmeretezesnel felszabaditjuk es ujracsinaljuk)
    game -> zombies_list.first = NULL;
    game -> lovedekek_list.first = NULL;
    game -> time = 0;
    game -> pont = 0;
    game -> napocska = 150;
    game->elet = 5;
    game->novenyek.peashooters_list.first = NULL;    
    game->novenyek.wallnuts_list.first = NULL;
    game->novenyek.sunflowers_list.first = NULL;
}
/**
 *@brief A játék által használt dinamikus tömbök felszabadítása
 *@param game A játékot tároló struktúra
 */
// Felszabaditja a lefoglalt memoriateruleket
void jatek_felszabadit (Jatek* game) {
    for(int i = 0; i < game -> sor; ++i) {
       free(game-> palya[i]);
    }
    free(game->palya);
    Zombi* iterZ = game->zombies_list.first;
    Zombi* nextZ;
    while(iterZ != NULL){
        nextZ = iterZ->next;
        free(iterZ);
        iterZ = nextZ;
    }
    Peashooter* iterP = game->novenyek.peashooters_list.first;
    Peashooter* nextP;
    while(iterP != NULL){
        nextP = iterP->next;
        free(iterP);
        iterP = nextP;
    }
    Sunflower* iterS = game->novenyek.sunflowers_list.first;
    Sunflower* nextS;
    while(iterS != NULL){
        nextS = iterS->next;
        free(iterS);
        iterS = nextS;
    }
    Wallnut* iterW = game->novenyek.wallnuts_list.first;
    Wallnut* nextW;
    while(iterW != NULL){
        nextW = iterW->next;
        free(iterW);
        iterW = nextW; 
    }
    Lovedek* iterL = game->lovedekek_list.first;
    Lovedek* nextL;
    while(iterL != NULL) {
        nextL = iterL->next;
        free(iterL);
        iterL = nextL;
    }
}
/**
 * @brief A függvény kiírja az irányítást a konzolra * 
 */
void print_control(){
    printf("\nA novenyek az eloholtak ellen iranyitasa nagyon egyszeru! \n\n \"Q\": Borsolovo kivalasztasa\n \"W\": A dio kivalasztasa\n \"E\": A napraforgo kivalasztasa\n"
    "A noveny kivalasztasa utan kattintassal tudja lerakni oket a palya egyik teglalapjara\n\nEs ennyi a szabaly!\n\n");
}
/**
 * @brief A függvény beolvassa a dicsőséglista.txt fájlból az ott lévő eredményeket
 * 
 */
void print_dicsosegLista(){
    FILE* f = fopen("dicsoseglista.txt","r");
    int pont;
    char nev[50];
    int hanyadik = 1;
    while(hanyadik < 11 && fscanf(f,"%s %d\n",nev,&pont) > 0)
        printf("%d. %s %06d\n",hanyadik++,nev,pont);
    printf("\n");
    fclose(f);
}
/**
 * @brief A függvény beírja a dicsőséglitába az eredményt, ha az eléri a legjobb 10 eredmény szintjét
 * 
 * @param pont A szerzett pont
 * @param nev A játék elején bekért név
 */
void game_over(int pont, char* nev){
    int* score = (int*) malloc (10*sizeof(int));
    char** names = (char**) malloc(10*sizeof(char*));
    for(int i = 0; i < 10; ++i){
        names[i] = (char*) malloc(50*sizeof(char));
    }
    FILE* fp = fopen("dicsoseglista.txt","r");
    int db = 0;
    int hova = 0;
    while(db < 10 && fscanf(fp,"%s %d\n",names[db],&score[db]) > 0){
        if(score[db] > pont){
            hova = db;
        }
        db++;
    }
    fp = freopen("dicsoseglista.txt","w",fp);
    int j = 0;
    for(int i = 0; i < db && j < 10; ++i, ++j){
        if(i == hova && j != 9){
            fprintf(fp,"%s\t%06d\n", nev,pont);
            j++;
        }
        fprintf(fp,"%s\t%06d\n", names[i],score[i]);
    }
    fclose(fp);
    for(int i = 0; i < 10; ++i){
        free(names[i]);
    }
    free(names);
    free(score);
}
/**
 * @brief A játék kezdetén meghívott függvény, ami irányítja a konzolon a menüsort
 * 
 * @param nev Pointer, ahova a bekért nevet fogjuk beírni
 */
void game_start(char* nev){
    int valasz;
    bool menu = true;
    while(menu){
        printf("Novenyek az elohalottak ellen!\n[1] Uj jatek\n[2] Iranyitas\n[3] Dicsoseglista\n[4] Kilépés!\n");
        scanf("%d",&valasz);
        switch(valasz){
            case 1:
                printf("A jatek elott adja meg a nevet:\t");
                scanf("%s",nev);
                menu = false;
                break;
            case 2:
                print_control();
                break;
            case 3:
                print_dicsosegLista();
                break;
            case 4:
                exit(0);
            default:
                printf("Tudod kivel szorakozzal!\n");
                menu = false;
                break;
        }
    }
}

/**
 *@brief A játék egy körét elvégzõ függvény
 * Először elvégzi a növények akcióját (lövés, napocska termelés), aztán a lövedékek mozgatását,
 * és legvégül a zombik akcióját (ha beérnek a zombik leszedjük őket, megölik a növényeket... stb)
 *@param game A játék struktúrája
 */
void jatek_kor(Jatek* game) {
    plant_actions(&game->novenyek, &game->lovedekek_list,&game->zombies_list,game->w,game->oszlop,&game->napocska);
    zombie_actions(&game->zombies_list,&game->novenyek,game->palya, &game->elet,&(game->pont));
    lovedek_mozog(&game->lovedekek_list, &game->zombies_list,game->w);
}
