#ifndef _GRAPHIQUE_H_
#include "MLV/MLV_all.h"
#define _GRAPHIQUE_H_
#include "game_all.h"

/**
 * Affiche les ennemis
 */
void afficher_liste_ennemis(Configuration config);
  
/**
 * Charger les sprites des ennemis 
 */
MLV_Image* charger_ennemis(Dessin d);

/**
 * Liste des animations des 3 enemis possibles
 */
void creer_liste_animation(Animation_gestion** invaders);

/**
 * Anime toute la liste d'enemis 
 */
void anim_ennemis(Configuration config, Animation_gestion *invaders[], Animation_gestion *poulpy_player);


/**
 * Affiche les tirs
 */
void afficher_tirs(Configuration config);

#endif
