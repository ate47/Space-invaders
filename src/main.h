
#ifndef __MAIN_H__
#define __MAIN_H__
#include "game_all.h"

/**
 * le type d'ennemi
 */
typedef enum{
  enemis1=0,
  enemis2=1,
  enemis3=2
} Dessin;

/**
 * une position x,y
 */
typedef struct{
  double x;
  double y;
} Entite;

/**
 * un ennemi
 */
typedef struct{
  Entite position;
  Dessin type;
  int vivant;
} Enemis;

/**
 * information d'un tir
 */
typedef struct{
  Entite tir;
  int nb_impact;
} Tir;

/**
 * configuration de jeu
 */
typedef struct{
  Enemis enemis[NB_MAX_ENEMIS];
  Tir tirs_enemis[NB_MAX_TIR_ENEMIS];
  Tir tir_allie;
  double pos_allie;
  int score;
  int termine;
  double decalage;
  double dec_bonus;
} Configuration;

/**
 * information d'un score
 */
typedef struct{
  int points;
  int name_size;
  char* name;
} Score;

/**
 * struture global du jeu
 */
typedef struct{
  Configuration config_actuel;
  Configuration save[NB_MAX_SAVES];
  Score scores[NB_MAX_SAVES];
  int theme;
} Jeu;


/**
 * Lit une configuration
 */
void lire_jeu(Jeu* jeu);

/**
 * Sauvegarde une configuration
 */
void sauvegarder_jeu(Jeu* jeu);

/**
 * Verifie si on peut utiliser tout les fichiers de jeu
 */
int check_files();

#endif
