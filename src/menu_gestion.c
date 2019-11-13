#include "game_all.h"

void ecrire_noms(int x, int y, Donnees_Menu* donnee) {
  const char noms[] = AUTHORS;
  MLV_get_size_of_text(noms, &x, &y); /* Consomation de x,y -> gcc content */
  avoir_menu_actuel(donnee); /* Consomation de donnee -> gcc content */
  MLV_draw_text(2, WINDOW_HEIGHT - (y + 2), noms, COLOR_TEXT(avoir_jeu(donnee)->theme));
}

int game_menu(Jeu* jeu) {
  Donnees_Menu* donnee;
  int e;
  
  donnee = creer_donnee(jeu);
  
  init_menu(menu_en_jeu(jeu, &e), donnee);
  free(donnee);
  return e;
}

void menu(Jeu* jeu) {
    Donnees_Menu* donnee = creer_donnee(jeu);
    
    /* Lancement du menu */
    init_menu(menu_principal(donnee), donnee);
    free(donnee);
}
