
#ifndef __GAME_ALL_H__
#define __GAME_ALL_H__

/* C'est toujours pratique d'avoir ça à un endroit */
#define GAME_NAME "Space Invaders"
#define AUTHORS "Jeu par Erwin Mehl, Theo Dumas, Antoine Willerval"

/*Fenetre par defaut */
#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600

/* Images par secondes */
#define FRAME_RATE 60

/* !=0 si on veut afficher la journalisation */
#define DEV_MODE 1

/* Fichiers du jeu */
#define GAME_SAVE_FILE "data/save.sho"
#define GAME_FILE_ALFRED "data/alfred.png"
#define GAME_FILE_POULPY "data/poulpy.png"
#define GAME_FILE_ROCKET "data/rocket.png"
#define GAME_FILE_ENNEMIS1 "data/JeanEudes.png"
#define GAME_FILE_ENNEMIS2 "data/Erwin.png"
#define GAME_FILE_ENNEMIS3 "data/Zorgy.png"
#define GAME_FILE_JACQUIE_S_TANK "data/destructor.png"

/* Langage */

#define GAME_LANG_SCORE "Score\n%d pts"
#define GAME_LANG_SCORE_MAX "Max\n%d pts"


#define ERROR_TEXTUEL_LAUNCH "Ahah, non.\n"
#define ERROR_TEXTUEL "Vous etes en mode textuel!\n"
#define ERROR_MAX_SCORE_CHAR_REACH "Erreur critique lors de la lecture du fichier de sauvegarde. Fermeture du liseur...\n"
#define ERROR_CANT_LOAD_FILE "Impossible de lire le fichier '%s', il n'a pas peut-etre pas les permissions ou n'existe pas\n"
#define ERROR_CANT_WRITE_SAVE_FILE "Impossible d'ecrire dans le fichier '%s', il n'a pas peut-etre pas les permissions\n"
#define ERROR_CHECK_FAIL "Impossible de lancer le jeu sans la verification des fichiers\n"


#define POINT(COUNT) (COUNT > 1 ? "points" : "point")
#define MENU_BACK "Retour"
#define MENU_QUIT "Quitter"
#define MENU_RESET "Remise a zero"
#define MENU_RESET_OK "Etes-vous sur(e) ?"
#define MENU_YES "Oui"
#define MENU_NO  "Non"

/* Menu principal */
#define MENU_NEW_GAME "Nouvelle partie"
#define MENU_RESUME_OLD_GAME "Reprendre la derniere\n partie"
#define MENU_RESUME_GAME "Reprendre"

/* Menu des options */
#define MENU_OPT "Options"
#define MENU_OPT_FULLSCREEN "Plein ecran"
#define MENU_OPT_SKIN "Theme Nuit/Jour"

/* Menu des sauvegardes */
#define MENU_SAVE_GAME_END "Votre score: %d %s\nEntrer un pseudo%s"
#define MENU_SAVE "Charger une partie"
#define MENU_SAVE_EMPTY "Sauvegarde %d (Vide)"
#define MENU_SAVE_NOT_EMPTY "Sauvegarde %d (%d point%s)"

/* Menu de sauvegarde */
#define MENU_SAVE_GAME "Sauvegarder"

/* Menu des scores */
#define MENU_SCORES "Tableau des scores"
#define MENU_SCORES_DATA_DRAW "%s: %d %s"
#define MENU_SCORES_DATA_DRAW_EMPTY "%s%d %s"
#define MENU_SCORES_DATA_TEXT "%d - %s: %d %s\n"
#define MENU_SCORES_DATA_TEXT_EMPTY "%d - %s%d %s\n"

/* Journalisation */
#define LOG_STARTING_GAME "Lancement du jeu"
#define LOG_END_GAME "Fin du jeu"
#define LOG_QUIT_GAME "Fermeture du jeu"
#define LOG_CHECK_FILES "Verification des fichiers de jeu..."
#define LOG_LOAD_SAVE "Lecture des sauvegardes du jeu..."

/* Menu textuel (pour tester les menus rapidement) */
#define MENU_TEXTE 0
#define MENU_TEXT_PROMPT "> "

/* Valeurs possible pour Jeu->theme */
#define THEME_NB 2

#define COLOR_BUTTON MLV_COLOR_DARK_CYAN
#define COLOR_BUTTON_TEXTE MLV_COLOR_WHITE

/* Le ciel d'où les méchants viennent (le fond) */
#define COLOR_BLUE_SKY(THEME) (THEME == 1 ? MLV_COLOR_SKY_BLUE : MLV_rgba(0x22,0x22,0x22,255))

/* Couleur du gazon de Jacquie (le sol en jeu) */
#define COLOR_GREEN_GRASS(THEME) (THEME == 1 ? MLV_rgba(0x44, 0xcc, 0, 255) : MLV_rgba(0x22, 0x99, 0, 255))

#define COLOR_LASER() (MLV_rgba(rnd(0,255), rnd(0,255), rnd(0,255), 255))

/* Couleur du texte */
#define COLOR_TEXT(THEME) (THEME == 1 ? MLV_COLOR_BLACK : MLV_COLOR_WHITE)
/* La couleur du texte quand on est dans un menu oui/non */
#define COLOR_OUI_NON(THEME) (THEME == 1 ? MLV_COLOR_RED : MLV_COLOR_SALMON)

/* Outils pour le menu */
#define WINDOW_WIDTH (MENU_TEXTE ? 1 : MLV_get_window_width())
#define WINDOW_HEIGHT (MENU_TEXTE ? 1 : MLV_get_window_height())
/* Taille minimum entre WINDOW_HEIGHT et WINDOW_WIDTH */
#define __GAME_SIZE__ ((WINDOW_WIDTH > WINDOW_HEIGHT ? WINDOW_HEIGHT : WINDOW_WIDTH))

/* Unité de dessin relative */
#define GAME_UNIT (__GAME_SIZE__ / 15)
/* Taille du carré où le jeu est dessiné */
#define GAME_SIZE (__GAME_SIZE__ - 2 * GAME_UNIT)

/* Taille des objets (Peut être drôle si on change la taille du missile) */
#define GAME_SIZE_ENEMIS (GAME_UNIT)
#define GAME_FRAMES_ENEMIS 3
#define GAME_SIZE_PLAYER (GAME_UNIT)
#define GAME_SIZE_ROCKET (GAME_UNIT/2)
#define GAME_SIZE_POULPY (GAME_UNIT*2/3)
#define GAME_SIZE_ROCKET_X (GAME_SIZE_ROCKET/2)
#define GAME_SIZE_TIR_W (GAME_UNIT / 5)
#define GAME_SIZE_TIR_H (GAME_UNIT / 3) 

/* Missile dans le menu */
#define ROCKET_SIZE 32
#define NB_ROCKETS 3
#define V_MAX 7
#define NB_TRAIL 6

/* Partie de l'écran où le jeu est dessiné */
#define GAME_LEFT   ((WINDOW_WIDTH  - GAME_SIZE) / 2)
#define GAME_RIGHT  ((WINDOW_WIDTH  + GAME_SIZE) / 2)
#define GAME_TOP    ((WINDOW_HEIGHT - GAME_SIZE) / 2)
#define GAME_BOTTOM ((WINDOW_HEIGHT + GAME_SIZE) / 2)

/* Vitesses des objets */
#define GAME_SPEED_ENEMIS 0.03
#define GAME_SPEED_POULPY 0.005
#define GAME_SPEED_JACQUIE 0.02
#define GAME_SPEED_ROCKET 0.02
#define GAME_SPEED_LASER 0.01
#define GAME_PROBA_LASER 30
#define GAME_MAX_SIZE 0.9
#define GAME_MIN_SIZE 0

/* Points dans le jeu */
#define GAME_POINTS_ENEMIS 100
#define GAME_POINTS_POULPY 9999
#define GAME_SHIFT_FACTOR 1.5
#define GAME_SHIFT_POULPY_FACTOR 20
#define GAME_MIN_TIME_BEFORE_POULPY (GAME_SPEED_POULPY * FRAME_RATE * 20)

/* Limiter les pseudos trop long */
#define NB_MAX_CHAR_SCORE 25

/* Gestion ennemis */
#define ENEMIS_PAR_LIGNE 6
#define LIGNE_ENEMIS 10
#define NB_MAX_SAVES 10
#define NB_MAX_ENEMIS (ENEMIS_PAR_LIGNE*LIGNE_ENEMIS)
#define NB_MAX_TIR_ENEMIS 4

/**
 * Affiche dans stdout un message si le mode VERBOSE est actif
 */
#if DEV_MODE
	#define LOG(TEXTE) fprintf(stdout, "[INFO]: %s\n", TEXTE);
#else
	#define LOG(TEXTE) ;
#endif

/**
 * Affiche dans stdout un message d'attention si le mode VERBOSE est actif
 */
#if DEV_MODE
	#define WARNING(TEXTE) fprintf(stdout, "[WARNING]: %s\n", TEXTE);
#else
	#define WARNING(TEXTE) ;
#endif

/**
 * Affiche dans stderr une erreur si le mode VERBOSE est actif
 */
#if DEV_MODE
	#define ERR(TEXTE) fprintf(stdout, "[ERROR]: %s\n", TEXTE);
#else
	#define ERR(TEXTE) ;
#endif

/**
 * Alloue #size de mémoire pour le pointeur #pointer, affiche une erreur si il y a une erreur (logique) puis ferme le programme
*/
#define ALLOC_MEMORY(POINTER, SIZE, NAME) POINTER = malloc(SIZE);\
if (POINTER == NULL){ \
	fprintf(stderr, "Erreur memoire: ");\
	fprintf(stderr, "Impossible d'allouer %lu B de memoire pour le pointeur '%s'!\n", SIZE, NAME);\
	exit(EXIT_FAILURE);\
}

/* 1° en radian pour les outils suivant */
#define DEGRE_RAD 0.0174532925199

/* Passage rapide de Degre <-> Radian */
#define VERS_RADIAN(deg) (DEGRE_RAD * deg)
#define VERS_DEGRE(rad)  (rad / DEGRE_RAD)

/* Les librairies */

#include<stdio.h>
#include<string.h>
#include <math.h>
#include "MLV/MLV_all.h"

/* Les modules */

#include "main.h"
#include "jeu.h"
#include "menu.h"
#include "menu_gestion.h"
#include "sauvegarde.h"
#include "animation_gestion.h"
#include "graphique.h"
#include "utils.h"

/* Les menus */

#include "menus/options.h"
#include "menus/sauvegarde.h"
#include "menus/oui_non.h"
#include "menus/score_menu.h"
#include "menus/injector.h"
#include "menus/principal.h"
#include "menus/en_jeu.h"
#include "menus/en_jeu_sauvegarde.h"

#endif
