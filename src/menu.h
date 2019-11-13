

#ifndef __MENU_H__
#define __MENU_H__

#include "game_all.h"


/**
 * Position pour un objet sur l'axe x
 */
typedef enum{
  Position_Relative_Gauche,
  Position_Relative_Centre,
  Position_Relative_Droite
} Position_Relative_x;

/**
 * Position pour un objet sur l'axe y
 */
typedef enum{
  Position_Relative_Haut,
  Position_Relative_Milieu,
  Position_Relative_Bas
} Position_Relative_y;

/*
 * Objets des/et menus
 */
typedef struct _Menu_Bouton Menu_Bouton;
typedef struct _Donnee_Menu Donnees_Menu;
typedef struct _Menu Menu;

/*
 * 
 *   Fonction pour les menus
 * 
 */

/**
 * Permet de creer un menu
 */
Menu* creer_menu(int nombre_boutons);

/**
 * Permet de creer un bouton dans un certain menu
 */
void definir_bouton(Menu* menu, int id, int x,int y, int largeur, int hauteur, const char* texte, Position_Relative_x position_x, Position_Relative_y position_y);

/**
 * Definir comment le menu evolue avant de dessiner les boutons
 */
void definir_fonction_pre_dessin(Menu* menu, void (* dessiner_pre )( int x, int y, Donnees_Menu* donnee ));

/**
 * Definir comment le menu evolue après avoir dessiner les boutons
 */
void definir_fonction_post_dessin(Menu* menu, void (* dessiner_post )( int x, int y, Donnees_Menu* donnee ));

/**
 * Definir comment le menu evolue lors d'un rendu en mode textuel
 */
void definir_fonction_texte_dessin(Menu* menu, void (* dessiner_texte )( Donnees_Menu* donnee ));

/**
 * Definir comment le menu evolue à chaque image
 */
void definir_fonction_mettre_a_jour(Menu* menu, void (* mettre_a_jour )( Donnees_Menu* donnee ));

/**
 * Definir comment le menu evolue quand on clique sur un bouton
 */
void definir_fonction_clique_bouton(Menu* menu, void (* clique_sur_bouton )( Menu_Bouton* bouton, Donnees_Menu* donnee ));

/**
 * Definir comment le menu evolue lors d'un clique sur le menu
 */
void definir_fonction_clique(Menu* menu, void (* clique )( int x, int y ));

/**
 * Definir la donnée du menu, voir la fonction avoir_donnee_menu
 */
void definir_donnee(Menu* menu, void* data);

/**
 * Obtenir la donnée du menu, voir la fonction definir_donnee
 */
void* avoir_donnee_menu(Menu* menu);

/**
 * Revenir à l'ancien menu
 */
void retour_menu(Donnees_Menu* donnee);

/**
 * Stopper les menus
 */
void quitter_menu(Donnees_Menu* donnee);

/*
 * 
 *   Fonction pour les boutons de menu
 * 
 */

/**
 * Obtenir la couleur de fond d'un bouton 
 */
MLV_Color avoir_bouton_couleur_fond(Menu_Bouton* bouton);

/**
 * Obtenir la couleur du texte d'un bouton 
 */
MLV_Color avoir_bouton_couleur_texte(Menu_Bouton* bouton);

/**
 * Obtenir l'id d'un bouton
 */
int avoir_bouton_id(Menu_Bouton* bouton);

/**
 * Obtenir le texte d'un bouton
 */
const char* avoir_bouton_texte(Menu_Bouton* bouton);

/**
 * Dessine un bouton
 */
void dessiner_bouton(Menu_Bouton* bouton, int x, int y, int srx, int sry);

/*
 * 
 *   Fonction pour les données de menu
 * 
 */


/**
 * Construit une donnée de menu à partir d'un jeu
 */
Donnees_Menu* creer_donnee(Jeu* jeu);

/**
 * Obtenir le menu actuel à partir d'une donnée de menu
 */
Menu* avoir_menu_actuel(Donnees_Menu* donnee);

/**
 * Obtenir le jeu à partir d'une donnée
 */
Jeu* avoir_jeu(Donnees_Menu* donnee);

/**
 * Lancer un menu / si (texte) alors executer le menu textuel
 */
void lancer_menu(Menu* menu, Donnees_Menu* donnee);

/*
 * 
 *   Fonction pour le systéme de menu
 * 
 */

/**
 * Initialiser le systeme de menu avec un menu de base
 */
void init_menu(Menu* menu, Donnees_Menu* donnee);



#endif
