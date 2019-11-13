

#include <stdlib.h>
#include <math.h>
#include "game_all.h"

/**
 * Décrit la position, la cible et l'animation d'un missile
 */
typedef struct{
  Animation_gestion* anim;
  double rx, ry, a;
  int lock_x, lock_y;
} Rocket_Data;

/**
 * Decrit un menu
 */
struct _Menu{
    Menu* parent;
    int boutons_nombre; /* Nombre de boutons dans le tableau precedent */
    void (* dessiner_pre )( int x, int y, Donnees_Menu* donnee ); /* addresse de la fonction qui dessine le menu avant les boutons (Peut être NULL) */
    void (* dessiner_post )( int x, int y, Donnees_Menu* donnee ); /* addresse de la fonction qui dessine le menu après les boutons (Peut être NULL) */
    void (* dessiner_texte )( Donnees_Menu* donnee );
    void (* mettre_a_jour )( Donnees_Menu* donnee ); /* addresse de la fonction qui met à jour le menu (Peut être NULL) */
    void (* clique )( int x, int y );/* addresse de la fonction quand on clique sur le menu (Peut être NULL) */
    void (* clique_sur_bouton )( Menu_Bouton* bouton, Donnees_Menu* donnee ); /* addresse de la fonction quand on clique sur un bouton du menu (Peut être NULL) */
    void* data; /* Des données pour l'execution du menu */
    Menu_Bouton* boutons; /* Tableau des boutons */
};

/**
 * Decrit un bouton
 */
struct _Menu_Bouton{
    int id, x, y, largeur, hauteur;
    short souris_survol, souris_clique, souris_traine;
    const char* texte;
    MLV_Color couleur_texte, couleur_fond;
    Position_Relative_x posrx;
    Position_Relative_y posry;
};
/**
 * Donnee du gestionnaire de menu
 */
struct _Donnee_Menu{
    Menu* menu_suivant;
    Menu* menu_actuel;
    Jeu* jeu;
};

int avoir_bouton_id(Menu_Bouton* bouton) {
  return bouton->id;
}
const char* avoir_bouton_texte(Menu_Bouton* bouton) {
  return bouton->texte;
}

MLV_Color avoir_bouton_couleur_fond(Menu_Bouton* bouton) {
  return bouton->couleur_fond;
}
MLV_Color avoir_bouton_couleur_texte(Menu_Bouton* bouton) {
  return bouton->couleur_texte;
}
void* avoir_donnee_menu(Menu* menu) {
  return menu->data;
}
Menu* avoir_menu_actuel(Donnees_Menu* donnee) {
  return donnee->menu_actuel;
}

Jeu* avoir_jeu(Donnees_Menu* donnee) {
  return donnee->jeu;
}

/**
 * Retire les valeurs des états du bouton
 */
static void reset_button(Menu_Bouton* bouton) {
  bouton->souris_clique = 0;
  bouton->souris_survol = 0;
  bouton->souris_traine = 0;
}

/**
 * Recupére la position relative (x,y) par rapport à un point absolu
 */
static void get_relative_location(Position_Relative_x posrx, Position_Relative_y posry, int x, int y, int* tx, int *ty) {
  switch(posrx) { /* Le type de position horizontal */
    case Position_Relative_Centre:
      *tx = WINDOW_WIDTH / 2 + x;
      break;
    case Position_Relative_Droite:
      *tx = WINDOW_WIDTH + x;
      break;
    case Position_Relative_Gauche:
    default: 
      *tx = x;
      break;
  }
  switch(posry) { /* Le type de position vertical */
    case Position_Relative_Milieu:
      *ty = WINDOW_HEIGHT / 2 + y;
      break;
    case Position_Relative_Bas:
      *ty = WINDOW_HEIGHT + y;
      break;
    case Position_Relative_Haut:
    default: 
      *ty = y;
      break;
  }
}

Menu* creer_menu(int nombre_boutons) {
    Menu* menu;
    
    /* On alloue la mémoire requise pour le menu */
    ALLOC_MEMORY(menu, sizeof(Menu), "menu");
    /* ...puis pour les boutons */
    ALLOC_MEMORY(menu->boutons, nombre_boutons * sizeof(Menu_Bouton), "menu->boutons");
    
    menu->boutons_nombre = nombre_boutons;
    
    /* Fonctions par defaut */
    menu->dessiner_pre = NULL;
    menu->dessiner_post = NULL;
    menu->dessiner_texte = NULL;
    menu->mettre_a_jour = NULL;
    menu->clique = NULL;
    menu->clique_sur_bouton = NULL;
    menu->parent = NULL;
    
    return menu;
}

void definir_fonction_pre_dessin(Menu* menu, void (* dessiner_pre )( int x, int y, Donnees_Menu* donnee )) {
    menu->dessiner_pre = dessiner_pre;
}
void definir_fonction_post_dessin(Menu* menu, void (* dessiner_post )( int x, int y, Donnees_Menu* donnee )) {
    menu->dessiner_post = dessiner_post;
}
void definir_fonction_texte_dessin(Menu* menu, void (* dessiner_texte )( Donnees_Menu* donnee )) {
    menu->dessiner_texte = dessiner_texte;
}
void definir_fonction_mettre_a_jour(Menu* menu, void (* mettre_a_jour )( Donnees_Menu* donnee )) {
    menu->mettre_a_jour = mettre_a_jour;
}
void definir_fonction_clique_bouton(Menu* menu, void (* clique_sur_bouton )( Menu_Bouton* bouton, Donnees_Menu* donnee )) {
    menu->clique_sur_bouton = clique_sur_bouton;
}
void definir_fonction_clique(Menu* menu, void (* clique )( int x, int y )) {
    menu->clique = clique;
}
void definir_donnee(Menu* menu, void* data) {
  menu->data = data;
}

void definir_bouton(Menu* menu, int id, int x,int y, int largeur, int hauteur, const char* texte, Position_Relative_x position_x, Position_Relative_y position_y) {
    Menu_Bouton bouton;
    
    if (id < 0 || id >= menu->boutons_nombre) {
      ERR("L'id depasse le nombre max de bouton du menu!");
      exit(EXIT_FAILURE);
    }
    
    /* Valeurs données */
    bouton.id = id;
    bouton.x = x;
    bouton.y = y;
    bouton.largeur = largeur;
    bouton.hauteur = hauteur;
    bouton.texte = texte;
    bouton.posrx = position_x;
    bouton.posry = position_y;
    
    /* Valeurs par defaut */
    bouton.couleur_texte = COLOR_BUTTON_TEXTE;
    bouton.couleur_fond = COLOR_BUTTON;
    bouton.souris_survol = 0;
    bouton.souris_clique = 0;
    bouton.souris_traine = 0;
    
    menu->boutons[id] = bouton;
}

void free_menu_si_absent(Menu* actuel, Menu* ancien) {
  while (actuel != NULL) {
    if (actuel == ancien)
      return;
    actuel = actuel->parent;
  }
  free(ancien);
}

Donnees_Menu* creer_donnee(Jeu* jeu) {
  Donnees_Menu* d;
  
  ALLOC_MEMORY(d, sizeof(Donnees_Menu), "donnee");
  d->jeu = jeu;
  d->menu_actuel = NULL;
  d->menu_suivant = NULL;
  return d;
}

void init_menu(Menu* menu, Donnees_Menu* donnee) {
    int i, /* iterateur */
#if MENU_TEXTE
    input; /* input pour le menu textuel */
#else
    sx, sy, /* position absolue de la souris (x,y) */
    srx, sry, /* position relative de la souris (x,y) */
    bouton_clique;  /* si un bouton a cliqué pendant le tick */
    double tsn, angle; Rocket_Data* tmp; /* Outils de calcul pour les missiles */
    
    Menu_Bouton* bouton; /* Le bouton qu'on utilise actuellement */
    MLV_Image* alfred; /* Alfred l'envahisseur */
    MLV_Image* missile; /* Image de missile */
    Animation_gestion *alfred_anim;/* L'animation d'Alfred */
    Rocket_Data rd[NB_ROCKETS]; /* Les missiles */

    /* Charge les images */
    alfred =  MLV_load_image( GAME_FILE_ALFRED );
    missile = MLV_load_image( GAME_FILE_ROCKET );
    /* Change sa taille d'Alfred */
    MLV_resize_image( alfred, 128, 512 );
    
    /* Initialise les missiles  */
    for (i = 0; i < NB_ROCKETS; i++) {
      rd[i].anim = creer_joueur_animation(missile, 4, 5, 0, 0, ROCKET_SIZE, ROCKET_SIZE);
      rd[i].rx = rnd(0, WINDOW_WIDTH);
      rd[i].ry = rnd(0, WINDOW_HEIGHT);
      rd[i].a = 0;
      rd[i].lock_x = rnd(-12, WINDOW_WIDTH + 12);
      rd[i].lock_y = rnd(-12, WINDOW_HEIGHT + 12);
    }
    /* Créer un joueur d'animation*/
    alfred_anim = creer_joueur_animation(alfred, 4, 10, 0, 0, 128, 128);
    
#endif
    
    lancer_menu(menu, donnee); /* Lancer le menu de base */
    while ( donnee->menu_suivant != NULL ) {
        menu = donnee->menu_suivant;
        donnee->menu_actuel = menu;
        if ( !MENU_TEXTE ){
          for (i = 0; i < menu->boutons_nombre; i++)
            reset_button(&menu->boutons[i]);
        }
        do {
#if MENU_TEXTE
            /* Dessine le menu */
            if ( menu->dessiner_texte )
                menu->dessiner_texte(donnee);
            
            /* Affiche les boutons disponibles */
            for (i = 0; i < menu->boutons_nombre; i++)
                printf("%d - %s\n", i+1, menu->boutons[i].texte);
            
            /* Demande à l'utilisateur un des boutons */
            printf(MENU_TEXT_PROMPT);
            if ( fscanf(stdin, "%d", &input) == 1 && input > 0 && input <= menu->boutons_nombre && menu->clique_sur_bouton)
                /* Lance le gestionnaire de bouton si il est defini pour le bouton selectionné */
                    menu->clique_sur_bouton(&menu->boutons[input-1], donnee);
            fflush(stdin);
            
#else
              
            /* Pour l'instant aucun bouton n'a appuyé */
            bouton_clique = 0;
            
            /* Recupére la position du curseur */
            MLV_get_mouse_position(&sx, &sy);
            
            /* Dessine le menu */
            
            /* Le fond */
            MLV_clear_window( COLOR_BLUE_SKY( avoir_jeu(donnee)->theme ));
            
            /* Les dessins speciaux */
            if ( menu->dessiner_pre )
                menu->dessiner_pre(sx, sy, donnee);
            
            /* Calcul la future position pour tous les missiles */
            for (i = 0; i < NB_ROCKETS; i++) {
              /* Le missile en cours */
              tmp = &rd[i];
              /* Distance cible/missile */
              tsn = sqrt((tmp->lock_x - tmp->rx) * (tmp->lock_x - tmp->rx) + (tmp->lock_y - tmp->ry) * (tmp->lock_y - tmp->ry));
              
              /* Le missile est-il suffisament loin de la cible ? */
              if(tsn > V_MAX * 4) {
                /* Calcul de l'angle entre celui actuel et la cible */
                angle = (asin((tmp->lock_x - tmp->rx) / tsn) < 0 ? -1 : 1) * acos((tmp->lock_y - tmp->ry) / tsn);
                
                /* La différence d'angle est suffisament petite ? */
                if (tmp->a - angle < 0.1 && angle - tmp->a < 0.1) {
                  
                  /* On avance vers la cible */
                  tmp->rx += (tmp->lock_x - tmp->rx) * V_MAX / tsn;
                  tmp->ry += (tmp->lock_y - tmp->ry) * V_MAX / tsn;
                  
                } else /* Sinon ajouter 10% de la différence à l'angle actuel pour se rapprocher de la cible */
                  tmp->a += (angle - tmp->a) * 0.1;
              } else { /* Sinon on est trop proche, on en cherche une autre */
                tmp->lock_x = rnd(-12, WINDOW_WIDTH + 12);
                tmp->lock_y = rnd(-12, WINDOW_HEIGHT + 12);
              }
              
              /* On affiche le missile avec son angle (d'où pourquoi pas l'animation de MLV) */
              draw_maj_joueur_animation(tmp->anim, tmp->rx, tmp->ry, VERS_DEGRE(tmp->a));
            }
            
            /* Les boutons */
            for (i = 0; i < menu->boutons_nombre; i++) {
                bouton = &menu->boutons[i];
                
                /* Calcul la position relative du bouton */
                get_relative_location(bouton->posrx, bouton->posry, bouton->x, bouton->y, &srx, &sry);
                
                /* Actualise et dessine le bouton */
                dessiner_bouton(bouton, sx, sy, srx, sry);
                
                /* Lance le gestionnaire de bouton si il est defini et qu'on a cliqué sur le bouton */
                if ( bouton->souris_clique && menu->clique_sur_bouton) {
                    menu->clique_sur_bouton(bouton, donnee);
                    bouton_clique = 1; /* Un bouton a appuyé */
                }
            }
            
            /* affiche l'envahisseur */
            draw_maj_joueur_animation(alfred_anim, WINDOW_WIDTH/2 - 64, WINDOW_HEIGHT/2 - 196, 0);
            
            
            /* Les dessins speciaux */
            if ( menu->dessiner_post ) 
                menu->dessiner_post(sx, sy, donnee);
            
            /* Mets a jour le menu */
            if (menu->mettre_a_jour)
                menu->mettre_a_jour(donnee);
            
            /* L'affiche */
            MLV_actualise_window();
            
            /* Attend le temps entre les images */
            MLV_delay_according_to_frame_rate();
#endif
        } while ( donnee->menu_suivant == menu ); /* Continuer tant qu'on n'a pas changé de menu */
        
        /* Attendre que l'utilisateur a terminé de cliquer sur sa souris */
        
        if ( !MENU_TEXTE )
            while (bouton_clique && MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED);
        
        /* On libére le menu de la mémoire si il n'est plus dans la chaine */
        free_menu_si_absent(donnee->menu_suivant, menu);
    }
}

void lancer_menu(Menu* menu, Donnees_Menu* donnee) {
    /* Place le menu actuel comme parent de l'ancien */
    menu->parent = donnee->menu_suivant;
    /* Change le nouveau menu */
    donnee->menu_suivant = menu;
}

void retour_menu(Donnees_Menu* donnee) {
    /* Place l'ancien menu dans celui actuel */
    donnee->menu_suivant = donnee->menu_actuel->parent;
}

void quitter_menu(Donnees_Menu* donnee) {
    /* ne pas dire de nouveau menu <=> Fermeture */
    donnee->menu_suivant = NULL;
}

void dessiner_bouton(Menu_Bouton* bouton, int x, int y, int srx, int sry) {
    MLV_Button_state state;
    /* Place la souris à sa position relative */
    x -= srx;
    y -= sry;
    state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);
    /* Regarde si la souris est sur le bouton */
    if (est_dessus(0, 0, bouton->largeur, bouton->hauteur, x, y)) {
        /* Regarde si le bouton etait deja sous la souris avant, si ce n'etait pas le cas */
        bouton->souris_survol = 1;
        /* Regarde si la souris appuyait avant sur le bouton, si ce n'etait pas le cas */
        if ( state == MLV_PRESSED ) {
            if ( !bouton->souris_clique && !bouton->souris_traine ) {
                bouton->souris_clique = 1;
            } else {
                bouton->souris_clique = 0;
                bouton->souris_traine = 1;   
            }
        }
    } else /* Indique si c'etait le cas que la souris n'est plus dessus */
        bouton->souris_survol = 0;
    /* Indique si c'etait le cas que la souris n'est plus appuye */
    if ( (bouton->souris_clique || bouton->souris_traine) && state != MLV_PRESSED) {
        bouton->souris_clique = 0;
        bouton->souris_traine = 0;
    }
    /* Dessine le bouton avec sa position relative */
    MLV_draw_text_box(srx, sry, bouton->largeur, bouton->hauteur, bouton->texte, 0, 
                      bouton->couleur_fond, 
                      /* Inverse les couleurs si la souris est au dessus du bouton */
                      bouton->souris_survol ? bouton->couleur_fond : bouton->couleur_texte, 
                      bouton->souris_survol ? bouton->couleur_texte : bouton->couleur_fond, 
                      MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}


