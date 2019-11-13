
#include "../game_all.h"

/********************
 * Menu score/reset *
 *******************/

static void reset_score(int oui, Donnees_Menu* donnee) {
  int i;
  Jeu* jeu;
  if (!oui)
    return;
  jeu = avoir_jeu(donnee);
  for (i = 0; i < NB_MAX_SAVES; i++) {
    free(jeu->scores[i].name);
    jeu->scores[i].name_size = 0;
    jeu->scores[i].points = 0;
    ALLOC_MEMORY(jeu->scores[i].name, sizeof(char), "FreeChar");
    jeu->scores[i].name[0] = '\0';
  }
}
static void action_score(Menu_Bouton* bouton, Donnees_Menu* donnee) {
  switch (avoir_bouton_id(bouton)) {
      case 0: /* Retour */
        retour_menu(donnee);
        break;
      case 1:
        lancer_menu(oui_non_menu(MENU_RESET_OK, reset_score), donnee);
        break;
  }
}

static void dessiner_score_texte(Donnees_Menu* donnee) {
  int i;
  Score* sc;
  sc = avoir_jeu(donnee)->scores; 
  for (i = 0; i < NB_MAX_SAVES; i++) {
    printf(sc[i].points ? MENU_SCORES_DATA_TEXT : MENU_SCORES_DATA_TEXT_EMPTY, i+1, sc[i].name, sc[i].points, POINT(sc[i].points));
  }
}
static void dessiner_score(int x, int y, Donnees_Menu* donnee) {
  int i;
  Score* sc;
  MLV_Color c1, c2, tmp;
  sc = avoir_jeu(donnee)->scores; 
  for (i = 0; i < NB_MAX_SAVES; i++) {
    /* Alterne entre le blanc et le bleu (pour faire jolie) */
    if (i % 2 == 0) {
      c1 = MLV_COLOR_LIGHT_BLUE;
    } else {
      c1 = MLV_COLOR_WHITE;
    }
    c2 = MLV_COLOR_BLACK;
    
    /* Si on est sur la ligne on inverse les couleurs (toujours pour le jolie) */
    if ( x >= WINDOW_WIDTH/2 - 200 && y >= WINDOW_HEIGHT/2 - 64 + 20 * i && x < WINDOW_WIDTH/2 + 200 && y < WINDOW_HEIGHT/2 - 64 + 20 * (i) + 20) {
      tmp = c1;
      c1 = c2;
      c2 = tmp;
    }
    
    /* Affichage du score */
      MLV_draw_text_box(WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2 - 64 + 20 * i, 380, 20, sc[i].points ? MENU_SCORES_DATA_DRAW : MENU_SCORES_DATA_DRAW_EMPTY, 0, c1, c2, c1, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, sc[i].name, sc[i].points, POINT(sc[i].points));
    
    /* Affichage du numéro de ligne */
    MLV_draw_text_box(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 64 + 20 * i, 20, 20, "%d", 0, c1, c2, c1, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, i+1);
  }
}

/**
 * Créer un pointeur d'un menu des scores
 */
Menu* scores_menu() {
  Menu* menu;
  
  menu = creer_menu(2);
  
  definir_bouton(menu, 0, +   2, - 60 + 20 * NB_MAX_SAVES, 198, 40, MENU_BACK,         Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 1, - 200, -60 + 20 * NB_MAX_SAVES, 198, 40, MENU_RESET,  Position_Relative_Centre, Position_Relative_Milieu);
  
  definir_fonction_clique_bouton(menu, action_score);
  definir_fonction_pre_dessin(menu, ecrire_noms);
  definir_fonction_post_dessin(menu, dessiner_score);
  definir_fonction_texte_dessin(menu, dessiner_score_texte);
  
  return menu;
}
