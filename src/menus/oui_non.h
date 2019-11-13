
#ifndef __MENU_OUI_NON_H__
#define __MENU_OUI_NON_H__
#include "../game_all.h"

/**
 * Créer un pointeur d'un menu oui ou non
 */
Menu* oui_non_menu(char* texte, void (* reponse ) ( int oui, Donnees_Menu* donnee ));

#endif
