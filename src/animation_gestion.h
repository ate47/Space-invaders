
#ifndef _ANIMATION_GESTION_H_
#define _ANIMATION_GESTION_H_
#include "MLV/MLV_all.h"

/*
 * 
 * Outils plus simple que celui de MLV, plus rapide et permettant la rotation lors du dessin
 * 
 */

typedef struct _Animation_gestion Animation_gestion;

/**
 * Permet de créer un joueur d'animation d'une image avec le même delai entre les frames
 */
Animation_gestion* creer_joueur_animation(MLV_Image* img, unsigned int frames, int delay, int x, int y, int width, int height);

/**
 * libére de la mémoire un gestionnaire d'animation
 */
void free_animation(Animation_gestion* gestion);

/**
 * Dessine et met à jour l'animation
 */
void draw_maj_joueur_animation(Animation_gestion* joueur, int x, int y, double angle);

/**
 * Met à jour l'animation
 */
void maj_joueur_animation(Animation_gestion* joueur);

/**
 * Dessine sans mettre à jour l'animation
 */
void draw_joueur_animation(Animation_gestion* joueur, int x, int y, double angle);

/**
 * Dessine et met à jour dans le sens inverse l'animation
 */
void draw_inv_maj_joueur_animation(Animation_gestion* joueur, int x, int y, double angle);

#endif
