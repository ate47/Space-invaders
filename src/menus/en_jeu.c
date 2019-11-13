
#include "../game_all.h"

/***************
 * Menu en jeu *
 ***************/

static void action_game_menu(Menu_Bouton* btn, Donnees_Menu* donnee) {
  switch(avoir_bouton_id(btn)) {
    case 0: /* Reprendre */
      *((int*)avoir_donnee_menu(avoir_menu_actuel(donnee))) = 1;
      retour_menu(donnee);
      break;
    case 1: /* Options */
      lancer_menu(option_menu(), donnee);
      break;
    case 2: /* Quitter */
      *((int*)avoir_donnee_menu(avoir_menu_actuel(donnee))) = 0;
      retour_menu(donnee);
      break;
    case 3: /* Sauvegarder */
      lancer_menu(save_menu_save(donnee), donnee);
      break;
  }
}

Menu* menu_en_jeu(Jeu* j, int *e) {
  Menu* menu;
  int i = (j->config_actuel.termine ? 0 : 1);
  
  menu = creer_menu(3 + i);
  *e = 1;
  definir_donnee(menu, e);
  
  definir_bouton(menu, 0, - 200,  - 64, 400, 40, MENU_RESUME_GAME,  Position_Relative_Centre, Position_Relative_Milieu);
  if (i)
    definir_bouton(menu, 3, - 200, - 20, 400, 40, MENU_SAVE_GAME,   Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 1, - 200,  - 20 + 44 * i, 400, 40, MENU_OPT, Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 2, - 200,  + 24 + 44 * i, 400, 40, MENU_QUIT,Position_Relative_Centre, Position_Relative_Milieu);
  
  definir_fonction_clique_bouton(menu, action_game_menu);
  definir_fonction_pre_dessin(menu, ecrire_noms);
  
  return menu;
}
