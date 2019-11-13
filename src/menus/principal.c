
#include "../game_all.h"

/******************
 * Menu principal *
 *****************/

static void init_menu_dessiner_text(Donnees_Menu* donnee) {
    printf("------------------\n%s\n------------------\n", GAME_NAME);
    avoir_menu_actuel(donnee); /* Pour consomer donnee -> gcc soit content <3 */
}
static void init_menu_button_clique(Menu_Bouton* bouton, Donnees_Menu* donnee) {
    /* Regarde et agit suivant l'id du bouton */
    switch (avoir_bouton_id(bouton)) {
        case 0: /* Quitter */
            retour_menu(donnee);
            break;
        case 1: /* Lancer */
            vider_config(&avoir_jeu(donnee)->config_actuel);
#if DEV_MODE
        case 6: /* Reprendre */
#else
        case 5:
#endif
            lancer_jeu(avoir_jeu(donnee));
            /* On retourne à l'ancien menu puis on replace un nouveau menu principal si le jeu n'est pas terminé */
            retour_menu(donnee);
            lancer_menu(menu_principal(donnee), donnee);
            break;
        case 2: /* Charger */
            lancer_menu(save_menu(donnee), donnee);
            break;
        
        case 3: /* Scores */
            lancer_menu(scores_menu(), donnee);
            break;
        
        case 4: /* Options */
            lancer_menu(option_menu(), donnee);
            break;
#if DEV_MODE
        case 5: /* data injector */
            lancer_menu(data_injector(), donnee);
            break;
#endif
    }
}

/**
 * Créer un pointeur d'un menu principal
 */
Menu* menu_principal(Donnees_Menu* donnee) {
    Menu* menu;
    /* On ajoute un autre bouton si la partie d'avant n'était pas terminé */
    int k = (avoir_jeu(donnee)->config_actuel.termine ? 0 : 1),
        z = DEV_MODE ? 1 : 0;
    
    /* Creation du menu */
    menu = creer_menu(5 + k + z);
    
    /* Creation des boutons */
    definir_bouton(menu, 1, - 200 + k * 202, - 64, 400 - k * 202, 40, MENU_NEW_GAME,  Position_Relative_Centre, Position_Relative_Milieu);
    if (z)
      definir_bouton(menu, 5, -200, -30, 200, 30, "Data Modifier", Position_Relative_Droite, Position_Relative_Bas);
    if (k)
      definir_bouton(menu, 5 + z, - 200, - 64, 198, 40, MENU_RESUME_OLD_GAME,  Position_Relative_Centre, Position_Relative_Milieu);
    
    definir_bouton(menu, 2, - 200, - 20, 400, 40, MENU_SAVE,            Position_Relative_Centre, Position_Relative_Milieu);
    definir_bouton(menu, 3, - 200,   24, 400, 40, MENU_SCORES,          Position_Relative_Centre, Position_Relative_Milieu);
    definir_bouton(menu, 4, - 200,   68, 198, 40, MENU_OPT,             Position_Relative_Centre, Position_Relative_Milieu);
    definir_bouton(menu, 0,     2,   68, 198, 40, MENU_QUIT,            Position_Relative_Centre, Position_Relative_Milieu);
    
    
    /* Definition du gestionnaire des cliques de bouton */
    definir_fonction_clique_bouton(menu, init_menu_button_clique);
    definir_fonction_pre_dessin(menu, ecrire_noms);
    definir_fonction_texte_dessin(menu, init_menu_dessiner_text);
    
    return menu;
}
