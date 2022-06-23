#include "megjelenit.h"
/**
 * @file megjelenit.c
 * @brief A pálya kirajzolását tartalmazó függvények definíciói  
 */

/**
 * @brief Egy szám, vagy szöveg megjelenítését végző függvény
 * 
 * @param renderer A rendererre mutató ointer
 * @param font A felirat font-jára mutató pointer
 * @param str A string, ami tárolja a felirat szövegét
 * @param hova Hova akarjuk beilleszteni a szöveget
 * @param color A szöveg színe
 */
void draw_numbers(SDL_Renderer* renderer,TTF_Font* font, char* str, SDL_Rect hova, SDL_Color color){
    SDL_Surface* felirat = TTF_RenderUTF8_Solid(font, str, color);
    SDL_Texture* felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t,NULL,&hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}
/**
 * @brief A HUD kirajzolását végző függvény
 * Az ablak tetején látható számokat jeleníti meg
 * @param renderer A rendererre mutató pointer
 * @param font A felirat font-jára mutató pointer
 * @param life Az aktuális életünk
 * @param score Az aktuális pontszámunk
 * @param time Az eltelt idő másodpercben
 * @param width Az ablak szélessége
 * @param napocska A napocskáink száma
 */
void draw_HUD(SDL_Renderer* renderer, TTF_Font* font, int life, int score, int time, int width, int napocska){
    SDL_Rect scoreR = {.x = 10, .y = 0, .h = 60, .w = width/4};
    SDL_Rect lifeR = {.x = (width/10)*3, .y = 0, .h = 60, .w = width/10};
    SDL_Rect timeR  = {.x = (width/10)*4.5, .y = 0, .h = 60, .w = width/4};
    SDL_Rect napocskaR = {.x = (width/10)*7.3, .y = 0, .h = 60, .w = width/4};
    char scoreS[10] = "";
    char timeS[10] = "";
    char lifeS[10];
    char napocskaS[10];
    sprintf(scoreS, "%06d", score);
    sprintf(lifeS, "%d", life);
    sprintf(timeS, "%02d:%02d",time/60, time%60);
    sprintf(napocskaS, "%04d", napocska);
    SDL_Color fekete = {0,0,0};
    SDL_Color sarga = {255,255,0};
    SDL_Color piros = {255,0,0};
    draw_numbers(renderer, font,scoreS,scoreR,fekete);
    draw_numbers(renderer, font,timeS,timeR,fekete);
    draw_numbers(renderer, font,napocskaS,napocskaR,sarga);
    draw_numbers(renderer, font,lifeS,lifeR,piros);
}
/**
 *@brief A háttér kirajzolását végzi el
 *
 *Négyzeteket rajzol a már korábban kiszámolt koordinátákra
 *@param renderer A renderer pointere
 *@param palya A palya négyzeteinek pointereit tartalmazó dinamikus tömb
 *@param sor A sorok száma
 *@param oszlop Az oszlopok száma
 */
void draw_background(SDL_Renderer *renderer, Rects** palya, int sor, int oszlop) {
    for(int i = 0; i < sor; ++i) {
        for(int j = 0; j < oszlop; ++j) {
            SDL_Rect temp = {.x = palya[i][j].x, .y = palya[i][j].y, .w = palya[i][j].w, palya[i][j].h};
            if(i%2 == 0){
                if((j%2 == 0))
                    SDL_SetRenderDrawColor(renderer, 0, 210, 0, 255);
                else
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }
            else {
                if((j%2 == 1))
                    SDL_SetRenderDrawColor(renderer, 0, 210, 0, 255);
                else
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }
        SDL_RenderFillRect(renderer, &temp);
        }
    }
}
/**
 *@brief Az összes peashooter kirajzolását végző függvény
 *
 *@param renderer A rendererre mutató pointer
 *@param palya A palya téglalapjaira mutató, pointerek tömbje, amiből az adatokat szedjük a képek beillesztéséhez
 *@param peashooter_list A peashooterek láncolt listája
 */
void draw_peashooters(SDL_Renderer * renderer, Rects** palya, Peashooter_list* peashooter_list, SDL_Texture* texture){
    Peashooter* iter = peashooter_list->first;
    while(iter != NULL) {
        SDL_Rect rect= {.x = palya[iter->pozicio.y][iter->pozicio.x].x,
                        .y = palya[iter->pozicio.y][iter->pozicio.x].y,
                        .h = palya[iter->pozicio.y][iter->pozicio.x].h,
                        .w = palya[iter->pozicio.y][iter->pozicio.x].w};
        SDL_RenderCopy(renderer,texture,NULL,&rect);
        iter = iter->next;
    }
}
/**
 *@brief Az összes sunflower kirajzolását végző függvény
 *
 *@param renderer A rendererre mutató pointer
 *@param palya A palya téglalapjaira mutató, pointerek tömbje, amiből az adatokat szedjük a képek beillesztéséhez
 *@param sunflower_list A sunflower-ek láncolt listája
 */
void draw_sunflowers(SDL_Renderer * renderer, Rects** palya, Sunflower_list* sunflower_list, SDL_Texture* texture){
    Sunflower* iter = sunflower_list->first;
    while(iter != NULL) {
        SDL_Rect rect= {.x = palya[iter->pozicio.y][iter->pozicio.x].x,
                    .y = palya[iter->pozicio.y][iter->pozicio.y].y,
                    .h = palya[iter->pozicio.y][iter->pozicio.y].h,
                    .w = palya[iter->pozicio.y][iter->pozicio.y].w};
        SDL_RenderCopy(renderer,texture,NULL,&rect);
        iter = iter -> next;
    }
}
/**
 *@brief Az összes wallnut kirajzolását végző függvény
 *
 *@param renderer A rendererre mutató pointer
 *@param palya A palya téglalapjaira mutató, pointerek tömbje, amiből az adatokat szedjük a képek beillesztéséhez
 *@param wallnut_list A wallnut-ok láncolt listája
 */
void draw_wallnuts(SDL_Renderer * renderer, Rects** palya, Wallnut_list* wallnut_list, SDL_Texture* texture){
    Wallnut* iter = wallnut_list->first;
    while(iter != NULL) {
        SDL_Rect rect= {.x = palya[iter->pozicio.y][iter->pozicio.x].x,
                    .y = palya[iter->pozicio.y][iter->pozicio.x].y,
                    .h = palya[iter->pozicio.y][iter->pozicio.x].h,
                    .w = palya[iter->pozicio.y][iter->pozicio.x].w};
        SDL_RenderCopy(renderer,texture,NULL,&rect);
        iter = iter->next;
    }
}
/**
 *@brief Az összes zombi kirajzolását végző függvény
 *
 *@param renderer A rendererre mutató pointer
 *@param palya A palya téglalapjaira mutató, pointerek tömbje, amiből az adatokat szedjük a képek beillesztéséhez
 *@param zombies_list A zombik láncolt listája
 */
void draw_zombies(SDL_Renderer * renderer, Rects** palya, Zombie_list* zombie_list, SDL_Texture* texture){
    Zombi* iter = zombie_list->first;
    while(iter != NULL) {
        SDL_Rect rect = {.x = iter->pozicio.x,
                        .y = palya[iter->pozicio.y][iter->pozicio.y].y,
                        .h = palya[0][iter->pozicio.y].h,
                        .w = palya[0][iter->pozicio.y].w};
        SDL_RenderCopy(renderer,texture,NULL,&rect);
        iter = iter->next;
    }
}
/**
 * @brief A függvény meghatározza, hogy a pálya melyik téglalapja fölött van éppen az egerünk
 * 
 * @param palya A pálya téglalapjainak dinamikus tömbjeire mutató pointerek tömbje
 * @param sor A sorok száma
 * @param oszlop Az oszlopok száma
 * @param width Az ablak szélessége
 * @param height Az ablak magassága
 * @param rect A téglalap, amit meghatároz a függvényünk, ebbe a pointerbe íródik
 * @return Pont A téglalap, pont-ban (melyik oszlop, melyik sor)
 */
Pont get_rect(Rects** palya, int sor, int oszlop, int width, int height, SDL_Rect* rect) {
    int mouse_x, mouse_y;
    Pont selected;
    SDL_GetMouseState(&mouse_x,&mouse_y);
    rect->h = palya[(mouse_y-60)/(height/sor)][mouse_x/(width/oszlop)].h;
    rect->y = palya[(mouse_y-60)/(height/sor)][mouse_x/(width/oszlop)].y;
    rect->x = palya[(mouse_y-60)/(height/sor)][mouse_x/(width/oszlop)].x;
    rect->w = palya[(mouse_y-60)/(height/sor)][mouse_x/(width/oszlop)].w;
    selected.y = (mouse_y-60)/(height/sor);
    selected.x = mouse_x/(width/oszlop);
    return selected;
}
/**
 *@brief Az éppen kiválasztott "item" megjelenítését végző függvény
 *
 *A függvény attól függően hogy melyik statebe van a selected Item változó, másik texturát tölt be és jelenít meg
 *@param renderer A rendererre mutató pointer
 *@param rect Az aktuális téglalap amire az egerünk mutat
 *@param selectedItem A kiválasztott item, esetükben peashooter/ wallnut/ sunflower vagy semmi
 */
void draw_selectedItem(SDL_Renderer* renderer, SDL_Rect rect, SelectedItem selectedItem) {

    SDL_Texture* peashooter = IMG_LoadTexture(renderer, "peashooter.png");
    SDL_Texture* wallnut = IMG_LoadTexture(renderer, "wallnut.png");
    SDL_Texture* sunflower = IMG_LoadTexture(renderer, "sunflower.png");

    switch(selectedItem){
    case PEASHOOTER:
        SDL_RenderCopy(renderer,peashooter,NULL,&rect);
        break;
    case WALLNUT:
        SDL_RenderCopy(renderer,wallnut,NULL,&rect);
        break;
    case SUNFLOWER:
        SDL_RenderCopy(renderer,sunflower,NULL,&rect);
        break;
    default:
        return;
    }
    SDL_DestroyTexture(peashooter);
    SDL_DestroyTexture(wallnut);
    SDL_DestroyTexture(sunflower);
}
/**
 * @brief A game over feliratot kiíró függvény
 * 
 * @param renderer A rendererre mutató pointer
 * @param font A betűtpus font-jára mutató pointer
 * @param height Az ablak magassága
 * @param width Az ablak szélessége
 */
void draw_gameOver(SDL_Renderer* renderer, TTF_Font* font,int height, int width){
    char str[] = "Game Over";
    SDL_Rect rect = {.x = 0,.y=0,.h = height, .w = width};
    SDL_Color color = {.r= 255, .g=0,.b = 0, .a= 255};
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    draw_numbers(renderer,font,str,rect,color);
    SDL_RenderPresent(renderer);
}
/**
 *@brief A lövedékek kirajzolását végző függvény
 *
 * A lövedék poziciójának x koordinátája megadja a kör x koordinátáját, az y pedig, hogy melyik sorban legyen
 *@param renderer A rendererre mutató pointer
 *@param lovedekek_list A övedékek láncolt listája
 *@param magas Az ablak magassága
 */
void draw_bullets(SDL_Renderer* renderer, Lovedek_list* lovedekek_list, int magas){
    int x, y;
    Lovedek* iter = lovedekek_list->first;
    while(iter != NULL) {
        x = iter->pozicio.x;
        y = (magas/2)+magas * (iter->pozicio.y) + 40;
        filledCircleRGBA(renderer, x, y, 10, 230, 255, 0, 255);
        circleRGBA(renderer, x, y, 10, 0, 0, 0, 255);
        iter = iter->next;
    }
}