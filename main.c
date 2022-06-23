#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "jatekmenet.h"
#include "debugmalloc.h"
#include "megjelenit.h"
#include "plants.h"
#include "zombie.h"
/**
 *@file main.c
 */
enum {SOR = 5};
enum {OSZLOP = 9};

/** @brief Az sdl tartozékainak inicializálása ebben a függvényben történik
 *  
 * A függvényben többek között inicializáljuk az sdl-t és IMG_INIT_PNG könyvtárat,
 * létrehozzuk az ablakot, a renderert és hozzáadjuk a felirat kezelő könyvtárat
 * @param felirat az ablakon megjelenő cím
 * @param szeles az ablak szélessége
 * @param magas az ablak magassága
 * @param pwindow az ablakra mutató pointer
 * @param prenderer a rendererre mutató pointer
 */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    if(IMG_Init(IMG_INIT_PNG) < 0) {
        SDL_Log("Nem hozhato letre a kep: %s", SDL_GetError());
        exit(1);
    }
    TTF_Init();

    *pwindow = window;
    *prenderer = renderer;
}
/**
 * @brief Az időzítő függvénye
 * A függvény pushol egy eventet, jelen esetben ez az event fogja végezni a pálya megrajzolását és az objektumok léptetését
 * @param ms hány milisecundumonként hívodjon meg a függvény
 * @param param a függvénynek átadott extra paraméter (jelen esetben ezt nem használjuk)
 * @return Uint32 mikor hívodjon meg legközelebb a függvény
 */
Uint32 idozit(Uint32 ms, void *param){
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    char nev[50];
    game_start(nev);

    Jatek uj;
    uj_jatek(&uj,9,5,640,420);

    SelectedItem selectedItem = NOTHING;

    /** ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Plants vs Zombies nagyon bután", 640, 480, &window, &renderer);

    /** Időzítő hozzáadása*/
    SDL_TimerID id = SDL_AddTimer(20, idozit, NULL);
    
    SDL_Texture* textureP = IMG_LoadTexture(renderer, "peashooter.png");
    SDL_Texture *textureS = IMG_LoadTexture(renderer, "sunflower.png");
    SDL_Texture *textureW = IMG_LoadTexture(renderer, "wallnut.png");
    SDL_Texture *textureZ = IMG_LoadTexture(renderer, "zombie.png");

    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 60);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    /** A növenyek spawnolásánal szükséges pointerek, ezeket adjuk át a van_noveny függvénynek*/
    Peashooter* p = NULL;
    Sunflower* s = NULL;
    Wallnut* w = NULL;
    SDL_Rect current_rect;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    Pont selected;
    /** Ezt a pontot fogjuk megadni a van_növény függvénynek, hogy ne lehessen ugyanarra a helyre több növényt spawnolni*/
    Pont novenyhez;
    Uint32 timeCounter = 0;
    bool quit = false;
    bool gameOver = false;
    while(!quit && !gameOver) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type){
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode) {
                case SDL_SCANCODE_Q:
                    selectedItem = PEASHOOTER;
                    break;
                case SDL_SCANCODE_W:
                    selectedItem = WALLNUT;
                    break;
                case SDL_SCANCODE_E:
                    selectedItem = SUNFLOWER;
                    break;
                default:
                    selectedItem = NOTHING;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(selectedItem){
                case PEASHOOTER:
                    if(uj.napocska >= 100 && !van_noveny(novenyhez,&uj.novenyek,uj.palya,&p,&w,&s)){
                        spawn_peashooter(selected, &uj.novenyek.peashooters_list);
                        uj.napocska -= 100;
                    }
                    break;
                case WALLNUT:
                    if(uj.napocska >= 50 && !van_noveny(novenyhez,&uj.novenyek,uj.palya,&p,&w,&s)) {
                        spawn_wallnut(selected, &uj.novenyek.wallnuts_list);
                        uj.napocska -= 50;
                    }
                    break;
                case SUNFLOWER:
                    if(uj.napocska >= 25 && !van_noveny(novenyhez,&uj.novenyek,uj.palya,&p,&w,&s)){
                        spawn_sunflower(selected, &uj.novenyek.sunflowers_list);
                        uj.napocska -= 25;
                    }
                    break;
                case NOTHING:
                    break;
                }
                break;
            case SDL_USEREVENT:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);                
                SDL_RenderClear(renderer);
                draw_background(renderer,uj.palya, SOR, OSZLOP);
                draw_HUD(renderer,font,uj.elet,uj.pont,uj.time,uj.w,uj.napocska);
                draw_peashooters(renderer, uj.palya, &(uj.novenyek.peashooters_list),textureP);
                draw_sunflowers(renderer, uj.palya, &(uj.novenyek.sunflowers_list),textureS);
                draw_wallnuts(renderer, uj.palya, &(uj.novenyek.wallnuts_list),textureW);
                draw_zombies(renderer, uj.palya,&(uj.zombies_list),textureZ);
                draw_bullets(renderer,&uj.lovedekek_list,uj.palya[0][0].h);
                selected = get_rect(uj.palya, uj.sor, uj.oszlop, uj.w, uj.h, &current_rect);
                novenyhez.x = uj.palya[selected.y][selected.x].x;
                novenyhez.y = selected.y;
                draw_selectedItem(renderer, current_rect ,selectedItem);
                SDL_RenderPresent(renderer);

                if(timeCounter == 50){
                    timeCounter = 0;
                    uj.time++;
                    if(uj.time % 10 == 1){
                        zombie_spawner(uj.time,&uj.zombies_list,uj.w, uj.sor);
                    }
                    if(uj.time == 10)
                        uj.napocska += 25;
                }
                 jatek_kor(&uj);
                 timeCounter++;
                 break;
            default:
                break;

            }
        }
        if(uj.elet <= 0){
            SDL_RemoveTimer(id);
            draw_gameOver(renderer,font,uj.h,uj.w);
        }
    }
    if(quit = true && uj.elet > 0)
        SDL_RemoveTimer(id);
        
    game_over(uj.pont,nev);

    jatek_felszabadit(&uj);
    TTF_CloseFont(font);
    SDL_DestroyTexture(textureS);
    SDL_DestroyTexture(textureW);
    SDL_DestroyTexture(textureP);
    SDL_DestroyTexture(textureZ);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}