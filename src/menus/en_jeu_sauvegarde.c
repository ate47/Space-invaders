#include "../game_all.h"


/*************************
 * Menu pour sauvegarder *
 *************************/

static void save_menu_save_action(Menu_Bouton* bouton, Donnees_Menu* donnee) {
  int id = avoir_bouton_id(bouton);
  Jeu* j = avoir_jeu(donnee);
  if (id == NB_MAX_SAVES) /* Retour */
    retour_menu(donnee);
  else {
    
    clone_config(j->config_actuel, &j->save[id]);
    
    retour_menu(donnee);
  }
}

Menu* save_menu_save(Donnees_Menu* d) {
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
      definir_bouton(menu, i, - 200, - 64 + 24 * i, 400, 20, (const char*) texte, Position_Relative_Centre, Position_Relative_Milieu, i + 1);
    } else {
      sprintf(texte, MENU_SAVE_NOT_EMPTY, i + 1, c.score, c.score > 1 ? "s" : "");
      definir_bouton(menu, i, - 200, - 64 + 24 * i, 400, 20,(const char*) texte, Position_Relative_Centre, Position_Relative_Milieu);
    }
  }
  definir_bouton(  menu, i, - 200, - 64 + 24 * i, 400, 40, MENU_BACK,        Position_Relative_Centre, Position_Relative_Milieu);
  
  definir_fonction_clique_bouton(menu, save_menu_save_action);
  definir_fonction_pre_dessin(menu, ecrire_noms);
  
  return menu;
}


