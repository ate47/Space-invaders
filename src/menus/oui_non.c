
#include "../game_all.h"

/****************
 * Menu oui/non *
 ****************/

typedef struct{
  char* texte;
  void (* reponse ) ( int oui, Donnees_Menu* donnee );
} oui_non_data;

static void oui_non_draw_texte(Donnees_Menu* donnee) {
  oui_non_data* d = (oui_non_data*) avoir_donnee_menu(avoir_menu_actuel(donnee));
  printf("%s\n", d->texte);
}
static void oui_non_draw(int x, int y, Donnees_Menu* donnee) {
  Menu* m = avoir_menu_actuel(donnee);
  x+=y; /* Consomation de x,y -> gcc content */
  MLV_draw_text_box(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 44, 400, 40, 
                    ((oui_non_data*) avoir_donnee_menu(m))->texte, 0, 
                    0, COLOR_OUI_NON(avoir_jeu(donnee)->theme), 0, 
                    MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

static void oui_non_clique_bouton(Menu_Bouton* bouton, Donnees_Menu* donnee) {
  oui_non_data* data;
  switch (avoir_bouton_id(bouton)) {
    case 0:
      retour_menu(donnee);
      break;
    case 1:
      data = avoir_donnee_menu(avoir_menu_actuel(donnee));
      data->reponse(avoir_bouton_id(bouton), donnee);
      free(data);
      definir_donnee(avoir_menu_actuel(donnee), NULL);
      retour_menu(donnee);
      break;
  }
}

/**
 * Créer un pointeur d'un menu oui ou non
 */
Menu* oui_non_menu(char* texte, void (* reponse ) ( int oui, Donnees_Menu* donnee )) {
  Menu* menu;
  oui_non_data* data;
  
  menu = creer_menu(2);
  
  definir_bouton(menu, 1, - 200,   0, 198, 40, MENU_YES, Position_Relative_Centre, Position_Relative_Milieu);
  definir_bouton(menu, 0, + 002,   0, 198, 40, MENU_NO, Position_Relative_Centre, Position_Relative_Milieu);
  
  definir_fonction_clique_bouton(menu, oui_non_clique_bouton);
  definir_fonction_pre_dessin(menu, oui_non_draw);
  definir_fonction_texte_dessin(menu, oui_non_draw_texte);
  
  ALLOC_MEMORY(data, sizeof(oui_non_data), "oui_non_data");
  
  ALLOC_MEMORY(data->texte, (strlen(texte) + 1) * sizeof(char), "oui_non_data->texte");
  
  data->reponse = reponse;
  strcpy(data->texte, texte);
  definir_donnee(menu, data);
  
  return menu;
}

