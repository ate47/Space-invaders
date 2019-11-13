
#include "../game_all.h"

/****************
 * Menu de test *
 ****************/

static void data_injector_draw(int x, int y, Donnees_Menu* donnee) {
  MLV_draw_adapted_text_box(0,0,"X/Y: %d, %d\nEcran: %d, %d\nFichier:\nSave=%s",3,0,COLOR_TEXT(avoir_jeu(donnee)->theme),0,MLV_TEXT_LEFT,
                            x,y, WINDOW_WIDTH, WINDOW_HEIGHT,
                           GAME_SAVE_FILE);
}

static void data_injector_button_clique(Menu_Bouton* bouton, Donnees_Menu* donnee) {
  /* Regarde et agit suivant l'id du bouton */
  switch (avoir_bouton_id(bouton)) {
    case 0: /* Configuration */
      
      break;
    case 1: /* Sauvegardes */
      
      break;
    case 2: /* Scores */
      
      break;
    case 3: /* Recharger */
      lire_jeu(avoir_jeu(donnee));
      MLV_draw_text_box(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, "Configurations lues", 0,0,
                        COLOR_TEXT(avoir_jeu(donnee)->theme), 
                        COLOR_BLUE_SKY(avoir_jeu(donnee)->theme), 
                        MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
      MLV_actualise_window();
      
      /* Attendre fin du clique sur le bouton */
      while (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED)
        ; 
      
      /* Attendre qu'on clique */
      while (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) != MLV_PRESSED)
        ; 
      
      /* Attendre fin du clique */
      while (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED)
        ; 
      
      break;
    case 4: /* Retour */
      retour_menu(donnee);
      break;
  }
}

/**
 * Fonction pour essayer des données
 */
Menu* data_injector() {
  Menu* menu = creer_menu(5);
  
  definir_bouton(menu, 0, -200, -64, 400, 40, "Configuration", Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 1, -200, -20, 400, 40, "Sauvegardes", Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 2, -200,  24, 400, 40, "Scores", Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 3, -200,  68, 400, 40, "Recharger les configurations", Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 4, -200,  112, 400, 40, "Retour", Position_Relative_Centre, Position_Relative_Milieu);
  
  definir_fonction_clique_bouton(menu, data_injector_button_clique);
  definir_fonction_post_dessin(menu, data_injector_draw);
  
  return menu;
}
