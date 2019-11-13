
#ifndef __JEU_H__
#define __JEU_H__

#include "game_all.h"


/**
 * Essaye de deplacer le joueur d'une certaine vitesse, 
 * 0 : pas d'erreur
 * 1 : pas de mouvement
 * -1 : mouvement mais pas total (fin de carte)
 */
int deplacer_joueur(Configuration* config, double vitesse);

/**
 * Lance le jeu
 */
void lancer_jeu(Jeu* j);

#endif
