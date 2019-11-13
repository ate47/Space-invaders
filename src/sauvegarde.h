
#ifndef __SAUVEGARDE_H__
#define __SAUVEGARDE_H__

#include "game_all.h"

/**
 * Fichier de sauvegarde
 */
typedef FILE* Sauvegardeur;

/**
 * Fichier de lecture
 */
typedef FILE* Liseur;

/**
 * Ouvrir un sauvegardeur
 */
Sauvegardeur ouvrir_sauvegardeur();

/**
 * Ouvrir un liseur
 */
Liseur ouvrir_liseur();

/**
 * Lit une configuration de taille taille_config dans le pointeur variable_a_lire, vrai si la lecture s'est effectué, faux sinon
 */
int lire_config(Liseur liseur, size_t taille_config, void* variable_a_lire);

/**
 * Ecrit une configuration de taille taille_config depuis le pointeur variable_a_ecrire, vrai si l'écriture s'est effectué, faux sinon
 */
int ecrire_config(Sauvegardeur sauvegardeur, size_t taille_config, void* variable_a_ecrire);

/**
 * Fermer un sauvegardeur 
 */
int fermer_sauvegardeur(Sauvegardeur sauvegardeur);

/**
 * Fermer un liseur
 */
int fermer_liseur(Liseur liseur);

/**
 * Clone une configuration config1 dans une configuration config2
 */
void clone_config(Configuration config1, Configuration* config2);

/**
 * Lire un jeu dans le fichier de sauvegarde
 */
void lire_jeu(Jeu* jeu);

/**
 * Sauvegarder le jeu dans le fichier de sauvegarde
 */
void sauvegarder_jeu(Jeu* jeu);

/**
 * Construit une configuration avec les valeurs de base
 */
void vider_config(Configuration* config);

/*
 * Sauve le score en fin de partie
 */
void sauvegarde_score(Jeu *j, char* pseudo);

#endif
