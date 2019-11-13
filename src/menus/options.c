
#include "../game_all.h"

/****************
 * Menu options *
 ****************/

static void option_menu_button_clique(Menu_Bouton* bouton, Donnees_Menu* donnee) {
    switch (avoir_bouton_id(bouton)) {
        case 0: /* Retour */
            retour_menu(donnee);
            break;
        case 1:
            if ( MENU_TEXTE )
                printf(ERROR_TEXTUEL);
            else if ( MLV_is_full_screen() ) {
                MLV_disable_full_screen();
                MLV_change_window_size(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
            } else {
                MLV_change_window_size(MLV_get_desktop_width(), MLV_get_desktop_height());
                MLV_enable_full_screen();
            }
            break;
        case 2:
          avoir_jeu(donnee)->theme = (avoir_jeu(donnee)->theme + 1) % THEME_NB;
          break;
    }
}

Menu* option_menu() {
  Menu* menu;
  
  /* Creation du menu */
  menu = creer_menu(3);
  
  /* Creation des boutons */
  definir_bouton(menu, 1, - 200, - 64, 400, 40, MENU_OPT_FULLSCREEN, Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 2, - 200, - 20, 400, 40, MENU_OPT_SKIN,       Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 0, - 200,   68, 400, 40, MENU_BACK,           Position_Relative_Centre, Position_Relative_Milieu);

  /* Definition du gestionnaire des cliques de bouton */
  definir_fonction_clique_bouton(menu, option_menu_button_clique);
  definir_fonction_pre_dessin(menu, ecrire_noms);
  
  return menu;
}
