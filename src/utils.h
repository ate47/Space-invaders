
#ifndef __UTILS_H__
#define __UTILS_H__

#include "game_all.h"

/* Nombre aléatoire entre min et max */
int rnd(int min, int max);

/* Vrai si (sx, sy) est au dessus du carré (x,y,w,h) */
int est_dessus(int x, int y, int w, int h, int sx, int sy);

/**
 * Vrai si (sr,sy) est sur le rectangle (x,y,w,h), Faux sinon
 */
int est_dessus_double(double x, double y, double w, double h, double sx, double sy);

/**
 * Vrai si la touche est appuyé, Faux sinon
 */
int key_pressed(MLV_Keyboard_button b);


#endif
