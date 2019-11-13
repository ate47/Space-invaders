
#ifndef __MENU_GESTION_H__
#define __MENU_GESTION_H__
#include "game_all.h"
/**
 * Lance le menu de base
 */
void menu(Jeu* jeu);
/**
 * Affiche le menu en jeu
 */
int game_menu(Jeu* jeu);

/**
 * écrire le noms des créateurs dans le menu
 */
void ecrire_noms(int x, int y, Donnees_Menu* donnee);

#endif
