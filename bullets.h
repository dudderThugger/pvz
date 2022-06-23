#ifndef BULLETS_H_INCLUDED
#define BULLETS_H_INCLUDED
/**
 *@file bullets.h
 *@brief A lövedékekhez köthető függvények deklarációja
 */
/** A növények sebességét tároló konstans */
enum {BULLET_SPEED = 5};
void spawn_lovedek(Pont poz, Lovedek_list* lovedekek_list);
void lovedek_mozog(Lovedek_list* lovedekek_list, Zombie_list* zombik_list, int sor);

#endif // BULLETS_H_INCLUDED
