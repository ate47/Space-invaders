
#include "../game_all.h"

/*******************
 * Menu sauvegarde *
 ******************/

static void save_menu_action(Menu_Bouton* bouton, Donnees_Menu* donnee) {
  int id = avoir_bouton_id(bouton);
  Jeu* j = avoir_jeu(donnee);
  if (id == NB_MAX_SAVES) /* Retour */
    retour_menu(donnee);
  else { /* Lecture d'une Configuration */
    Configuration c = j->save[id];
    if (!c.termine) {
      /* Pour à la fin du jeu revenir au menu de base */
      retour_menu(donnee);
      /* On place la configuration choisie dans la configuration actuelle */
      clone_config(c, &j->config_actuel);
      /* On lance le jeu avec la nouvelle configuration */
      lancer_jeu(j);
    }
  }
}

/**
 * Créer un pointeur d'un menu des sauvegardes
 */
Menu* save_menu(Donnees_Menu* d) {
  Menu* menu;
  int i;
  char* texte;
  
  /* Creation du menu */
  menu = creer_menu(NB_MAX_SAVES + 1);
  
  /* Creation des boutons */
  for (i = 0; i < NB_MAX_SAVES; i++) {
    Configuration c = avoir_jeu(d)->save[i];
    ALLOC_MEMORY(texte, sizeof(char) * 45, "save-texte");
    if (c.termine) {
      sprintf(texte, MENU_SAVE_EMPTY, i + 1);
      definir_bouton(menu, i, - 200, - 64 + 24 * i, 400, 20, (const char*) texte, Position_Relative_Centre, Position_Relative_Milieu);
    } else {
      sprintf(texte, MENU_SAVE_NOT_EMPTY, i + 1, c.score, c.score > 1 ? "s" : "");
      definir_bouton(menu, i, - 200, - 64 + 24 * i, 400, 20,(const char*) texte, Position_Relative_Centre, Position_Relative_Milieu);
    }
  }
  definir_bouton(  menu, i, - 200, - 64 + 24 * i, 400, 40, MENU_BACK,        Position_Relative_Centre, Position_Relative_Milieu);
  
  definir_fonction_clique_bouton(menu, save_menu_action);
  definir_fonction_pre_dessin(menu, ecrire_noms);
  
  return menu;
}
