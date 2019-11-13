
#include<stdio.h>
#include<stdlib.h>
#include "game_all.h"
#include <math.h>

typedef struct{
	int end, deplacement;
	MLV_Image *player_img, *rocket_img, *poulpy_img;
	Animation_gestion *player, *rocket_player, *poulpy_player,*invaders[3];
} Jeu_data;


/**
 * Essaye de deplacer le joueur d'une certaine vitesse, 
 * 0 : pas d'erreur
 * 1 : pas de mouvement
 * -1 : mouvement mais pas total (fin de carte)
 */
int deplacer_joueur(Configuration* config, double vitesse) {
	double nl;
  /* Inutile de bouger -> 1 */
	if ((config->pos_allie == GAME_MIN_SIZE && vitesse < 0) || (config->pos_allie == GAME_MAX_SIZE && vitesse > 0) )
		return 1;
	
  /* Calcul de la future position */
	nl = config->pos_allie + vitesse;
  
	if (nl < GAME_MIN_SIZE) { /* Trop proche */
		config->pos_allie = GAME_MIN_SIZE;
		return -1;
	} else if (nl > GAME_MAX_SIZE) { /* Trop loin */
		config->pos_allie = GAME_MAX_SIZE;
		return -1;
	} else { /* La position est dans la zone de combat */
		config->pos_allie = nl;
		return 0;
	}
	
}

/**
 * Definir les joueurs d'animation pour les données
 */
void define_player(Jeu_data* data) {
	MLV_resize_image(data->player_img, GAME_SIZE_PLAYER, GAME_SIZE_PLAYER * 4);
	MLV_resize_image(data->rocket_img, GAME_SIZE_ROCKET, GAME_SIZE_ROCKET * 4);
	MLV_resize_image(data->poulpy_img, GAME_SIZE_POULPY, GAME_SIZE_POULPY * 4);
	data->player = creer_joueur_animation(data->player_img, 4, 2, 0, 0, GAME_SIZE_PLAYER, GAME_SIZE_PLAYER);
	data->rocket_player = creer_joueur_animation(data->rocket_img, 4, 2, 0, 0, GAME_SIZE_ROCKET, GAME_SIZE_ROCKET);
  data->poulpy_player = creer_joueur_animation(data->poulpy_img, 4, 10, 0, 0, GAME_SIZE_POULPY, GAME_SIZE_POULPY);
	creer_liste_animation(data->invaders);
}

/**
 * Definir un ennemi
 */
void define_enemis(MLV_Image* enemis_img, Animation_gestion** enemis){
	MLV_resize_image(enemis_img,GAME_SIZE_ENEMIS , GAME_SIZE_ENEMIS  * 4);
	*enemis = creer_joueur_animation(enemis_img, 4, 2, 0, 0, GAME_SIZE_ENEMIS , GAME_SIZE_ENEMIS );
}

/* on ajoute une ligne d'ennemis pendant le jeu */
void ajouter_ennemis(Configuration* config){
	int i,j;
	for(j=0, i=0; j < NB_MAX_ENEMIS && i < ENEMIS_PAR_LIGNE; j++)
		if(!config->enemis[j].vivant) {
			config->enemis[j].position.x= 1.0/ENEMIS_PAR_LIGNE*i;
			config->enemis[j].position.y= 1.0/LIGNE_ENEMIS;
			config->enemis[j].type = rnd(0,3);
			config->enemis[j].vivant=1;
			i++;
		}
}

/* déplacement des Invaders */

void evolution_ligne(Configuration* config){
	int i;
	for(i=0;i<NB_MAX_ENEMIS;i++) {
		config->enemis[i].position.y += 1.0/LIGNE_ENEMIS;
	}
}

/* on créer un tir à partir d'une entité */
Tir creer_tir(Entite e, double decalage) {
	Tir tir;
	tir.tir.x=e.x + (cos(decalage) * GAME_SIZE_ENEMIS) / GAME_SIZE;
	tir.tir.y=e.y;
	tir.nb_impact=1;
	return tir;
}

/* nb tir ennemis */
int nb_tirs(Configuration config){
	int i,n=0;
	for(i=0;i<NB_MAX_TIR_ENEMIS;i++)
		if(config.tirs_enemis[i].nb_impact)
			n++;
  return n;
}

/**
 * On ajoute un tir dans le tableau
 */
void placer_tir(Configuration *config, Tir t) {
	int i;
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++)
		if (!config->tirs_enemis[i].nb_impact) {
			config->tirs_enemis[i] = t;
			break;
		}
}

/**
 * Calcul de l'évolution des tirs 
 */
void evol_tir(Configuration* config) {
	int i,j,n;
	double max = 1;
	Entite pos[ENEMIS_PAR_LIGNE];
	/*si un tir dépasse l'ecran il est effacer */
	for(i=0;i<NB_MAX_TIR_ENEMIS;i++) {
		if(config->tirs_enemis[i].nb_impact)
			config->tirs_enemis[i].tir.y += GAME_SPEED_LASER;
		if(config->tirs_enemis[i].tir.y > 1.1)
			config->tirs_enemis[i].nb_impact = 0;
	}
	
	/* Si il y a déjà trop de tirs */
	if (!(NB_MAX_TIR_ENEMIS - nb_tirs(*config)) || rnd(0, GAME_PROBA_LASER))
		return;
	
	/* On recupére la position la plus basse */
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++)
		if (config->enemis[i].vivant && config->enemis[i].position.y > max)
			max = config->enemis[i].position.y;
		
  /* On récupére les ennemis à la hauteur minimale */
  for (i = 0, j = 0; i < NB_MAX_ENEMIS && j < ENEMIS_PAR_LIGNE; i++)
    if (config->enemis[i].vivant && config->enemis[i].position.y - max <= 0.05)
      pos[j++] = config->enemis[i].position;
    
  /* Si on a récupéré aucun ennemi alors on quitte */
  if (!j)
    return;
    
  /* On selectionne une position aléatoirement */
  n = rnd(0, j);

		/* Et on ajoute ce tir */
		placer_tir(config, creer_tir(pos[n], config->decalage));
}

/**
 * Lance le menu de fin de jeu et sauvegarde le score
 */
void fin_config_jeu(Jeu* j) {
  char *texte, *pseudo;
  int n = 1;
  MLV_Input_box *input_box = MLV_create_input_box(
                WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2,
                400, 40, 
                COLOR_BUTTON, COLOR_BUTTON_TEXTE,
                COLOR_BUTTON, "Pseudo: "
        );
  
  ALLOC_MEMORY(pseudo, sizeof(char) * 1, "AvantPseudo");
  pseudo[0] = '\0';
  
  j->config_actuel.termine = 1; /* Au cas où on a oublié... */
  
  do {
    MLV_clear_window(COLOR_BLUE_SKY(j->theme));
    
    MLV_draw_text_box(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2-40, 400, 40, MENU_SAVE_GAME_END, 2, 0, COLOR_TEXT(j->theme), 0, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, j->config_actuel.score, POINT(j->config_actuel.score), n==0 ? " non vide !" : n > NB_MAX_CHAR_SCORE ? " pas trop long !" : "");
    
    MLV_draw_input_box(input_box);
    MLV_actualise_window();
    
    MLV_wait_particular_input_box(input_box, &texte);
    
    /* On libére l'ancien pseudo */
    free(pseudo);
    pseudo = texte;
    
    n = strlen(pseudo);
  } while (!(n > 0 && n < NB_MAX_CHAR_SCORE));
  
  MLV_free_input_box( input_box );
  
  /* On place le score avec pseudo dans la sauvegarde */
  if (strcmp(pseudo, "AWT") != 0) /* AW Test*/
    sauvegarde_score(j, pseudo);
}

/**
 * Libére la mémoire les animations
 */
void free_anims(Jeu_data data) {
  free_animation(data.player);
  free_animation(data.rocket_player);
  free_animation(data.poulpy_player);
  free_animation(data.invaders[0]);
  free_animation(data.invaders[1]);
  free_animation(data.invaders[2]);
}
/**
 * Libére la mémoire les animations et images
 */
void free_data(Jeu_data data) {
  free_anims(data);
  MLV_free_image(data.player_img);
  MLV_free_image(data.rocket_img);
  MLV_free_image(data.poulpy_img);
}

/**
 * Affiche la configuration actuelle
 */
void afficher(Jeu* j, Jeu_data data) {
	int x,y;
	
	afficher_tirs(j->config_actuel);
	
	/* Le gazon de Jacquie */
  MLV_draw_filled_rectangle(0, GAME_BOTTOM - 1, WINDOW_WIDTH, WINDOW_HEIGHT - GAME_BOTTOM + 1, COLOR_GREEN_GRASS(j->theme));
  
  /* Affichage du score */
  MLV_draw_text_box((WINDOW_WIDTH - GAME_SIZE) / 2, GAME_BOTTOM, GAME_SIZE / 2, WINDOW_HEIGHT - GAME_BOTTOM, GAME_LANG_SCORE, 1, 0, MLV_COLOR_WHITE, 0, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, j->config_actuel.score);
  
  /* Affichage du score max */
  MLV_draw_text_box((WINDOW_WIDTH) / 2, GAME_BOTTOM, GAME_SIZE / 2, WINDOW_HEIGHT - GAME_BOTTOM, GAME_LANG_SCORE_MAX, 1, 0, MLV_COLOR_WHITE, 0, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, j->config_actuel.score > j->scores[0].points ? j->config_actuel.score : j->scores[0].points);
  
  /* Afficher du missile de Jacquie */
  if (j->config_actuel.tir_allie.nb_impact) {
    x = GAME_LEFT + j->config_actuel.tir_allie.tir.x * GAME_SIZE - GAME_SIZE_ROCKET / 2;
    y = GAME_BOTTOM - j->config_actuel.tir_allie.tir.y * GAME_SIZE - GAME_SIZE_ROCKET / 2;
    draw_maj_joueur_animation(data.rocket_player, x, y, 180);
  }
  
  /* Position du tank de Jacquie */
  x = (int) (GAME_LEFT + j->config_actuel.pos_allie * GAME_SIZE) - GAME_SIZE_PLAYER / 2;
  y = (int) (GAME_BOTTOM) - GAME_SIZE_PLAYER;
  /* Suivant si on se deplace jouer l'animation dans un certain sens et la dessiner */
  if (data.deplacement == 1) /* Si on se deplace  */
    draw_maj_joueur_animation(data.player, x, y, 0);
  else if (data.deplacement == 2)
    draw_inv_maj_joueur_animation(data.player, x, y, 0);
  else 
    draw_joueur_animation(data.player, x, y, 0);
  
  anim_ennemis(j->config_actuel, data.invaders, data.poulpy_player);
}
/**
 * Calcul hitbox rockets
 */
void hitbox_rocket(Jeu* j, Configuration* config) {
	int i;
	Enemis* en;
	Entite e;
	Entite r = config->tir_allie.tir;
	double dec= cos(config->decalage) * GAME_SIZE_ENEMIS * GAME_SHIFT_FACTOR / GAME_SIZE,
	es = ((double) GAME_SIZE_ENEMIS) / GAME_SIZE,
	rsx = ((double) GAME_SIZE_ROCKET_X) / GAME_SIZE,
	rsy = ((double) GAME_SIZE_ROCKET) / GAME_SIZE;
	
  /* On place notre position suivant le decalage des ennemis et haut en bas */
  r.x-=dec;
	r.y = 1 - r.y;
  
  /* Calcul si elle touche un ennemi */
	for (i = 0; i < NB_MAX_ENEMIS; i++) {
		en = &config->enemis[i];
		e = en->position;
    if (en->vivant && (
			est_dessus_double(e.x, e.y, es, es, r.x, r.y) /* Test haut_gauche */
			|| est_dessus_double(e.x, e.y, es, es, r.x, r.y + rsy) /* Test bas_gauche */
			|| est_dessus_double(e.x, e.y, es, es, r.x + rsx, r.y) /* Test haut_droit */
			|| est_dessus_double(e.x, e.y, es, es, r.x + rsx, r.y + rsy) /* Test bas_droit */
		)) {
			en->vivant--;
			config->tir_allie.nb_impact--;
			config->score += GAME_POINTS_ENEMIS;
			break;
		}
	}
	
  /* Calcul si elle touche poulpy */
	if (config->dec_bonus > 0) {
    e.x = (sin(config->dec_bonus)*GAME_SIZE_POULPY*GAME_SHIFT_POULPY_FACTOR + WINDOW_WIDTH / 2) / GAME_SIZE;
    e.y = (GAME_TOP + ((double)GAME_UNIT - GAME_SIZE_POULPY) / 2) / GAME_SIZE;
    es = ((double) GAME_SIZE_POULPY) / GAME_SIZE;
    r.x = (GAME_LEFT + j->config_actuel.tir_allie.tir.x * GAME_SIZE - GAME_SIZE_ROCKET / 2) / GAME_SIZE;
    r.y = (GAME_BOTTOM - j->config_actuel.tir_allie.tir.y * GAME_SIZE - GAME_SIZE_ROCKET / 2) / GAME_SIZE;
    if (
			est_dessus_double(e.x, e.y, es, es, r.x, r.y) /* Test haut_gauche */
			|| est_dessus_double(e.x, e.y, es, es, r.x, r.y + rsy) /* Test bas_gauche */
			|| est_dessus_double(e.x, e.y, es, es, r.x + rsx, r.y) /* Test haut_droit */
			|| est_dessus_double(e.x, e.y, es, es, r.x + rsx, r.y + rsy) /* Test bas_droit */
    ) {
      config->score += GAME_POINTS_POULPY;
      config->dec_bonus = - (GAME_MIN_TIME_BEFORE_POULPY);
			config->tir_allie.nb_impact--;
    }
  }
}

/**
 * Calcul hitbox laser
 */
void hitbox_laser(Configuration* config) {
	int i;
	Tir* t;
	double pl,
	plr = ((double) GAME_SIZE_PLAYER) / GAME_SIZE,
	lsx =  ((double) GAME_SIZE_TIR_W) / GAME_SIZE,
	lsy =  ((double) GAME_SIZE_TIR_H) / GAME_SIZE;
	pl = config->pos_allie - plr/2;
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++)
		if ((t=&config->tirs_enemis[i])->nb_impact && (
			est_dessus_double(pl, 1-plr, plr, plr, t->tir.x, t->tir.y)
			|| est_dessus_double(pl, 1-plr, plr, plr, t->tir.x, t->tir.y + lsy)
			|| est_dessus_double(pl, 1-plr, plr, plr, t->tir.x + lsx, t->tir.y)
			|| est_dessus_double(pl, 1-plr, plr, plr, t->tir.x + lsx, t->tir.y + lsy)
		)) {
			config->termine = 1;
			t->nb_impact = 0;
			return;
		}
}


/**
 * Calcul hitbox rocket + ennemis
 */
void hitbox(Jeu* j, Configuration *config){
  int i;
  /* Si les ennemis sont trop bas -> stop */
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++)
    if (config->enemis->position.y > 1 - 1.0 / LIGNE_ENEMIS) {
      config->termine = 1;
      return;
    }
  
	if (config->tir_allie.nb_impact)
		hitbox_rocket(j, config);
}

/**
 * Calcul le jeu
 * Vrai si le calcul s'est bien effectué, faux sinon (ex: fin du jeu)
 */
int calcul(Jeu* j, Jeu_data* data) {
  /* Mise à jour */
		data->deplacement = 0;
		if (!j->config_actuel.termine) {
			if (key_pressed(MLV_KEYBOARD_LEFT) || key_pressed(MLV_KEYBOARD_q)) {
				if (deplacer_joueur(&j->config_actuel, -GAME_SPEED_JACQUIE) <= 0)
					data->deplacement = 2;
			} else if (key_pressed(MLV_KEYBOARD_RIGHT) || key_pressed(MLV_KEYBOARD_d) ) {
				if (deplacer_joueur(&j->config_actuel, GAME_SPEED_JACQUIE) <= 0)
					data->deplacement = 1;
			}
			
			if( j->config_actuel.decalage > VERS_RADIAN(360)){
				evolution_ligne(&j->config_actuel);
				j->config_actuel.decalage=0;
				ajouter_ennemis(&j->config_actuel);
			}
			
			evol_tir(&j->config_actuel);
      hitbox_laser(&j->config_actuel);
			
			if (j->config_actuel.tir_allie.nb_impact) {
				j->config_actuel.tir_allie.tir.y += GAME_SPEED_ROCKET;
				if (j->config_actuel.tir_allie.tir.y > 1.1)
					j->config_actuel.tir_allie.nb_impact = 0;
			} else if (key_pressed(MLV_KEYBOARD_SPACE)) {
				j->config_actuel.tir_allie.nb_impact = 1;
				j->config_actuel.tir_allie.tir.x = (double) (j->config_actuel.pos_allie*GAME_SIZE) / GAME_SIZE;
				j->config_actuel.tir_allie.tir.y = ((double) GAME_SIZE_PLAYER) / 2 / GAME_SIZE;
			}
			j->config_actuel.decalage += GAME_SPEED_ENEMIS;
			j->config_actuel.dec_bonus += GAME_SPEED_POULPY;
      
			hitbox(j, &j->config_actuel);
			
			if (j->config_actuel.termine) {
        free_data(*data);
        fin_config_jeu(j);
        return 0;
      }
		}
		if (key_pressed(MLV_KEYBOARD_ESCAPE)) {
			/* On retire de la mémoire les joueurs au cas où la resolution a peut-être été changé pendant qu'on était dans le menu */
      free_anims(*data);
			data->end = game_menu(j);
			if (data->end) {
				define_player(data);
			} else {
				MLV_free_image(data->player_img);
				MLV_free_image(data->rocket_img);
				return 0; /* On quitte le jeu pour eviter l'affichage d'objet non en mémoire */
			}
		}
  return 1;
}


void lancer_jeu(Jeu* j) {
	Jeu_data data;
  
	LOG(LOG_STARTING_GAME);
	if ( MENU_TEXTE ) {
		printf(ERROR_TEXTUEL_LAUNCH);
		return;
	}
	
	data.player_img = MLV_load_image(GAME_FILE_JACQUIE_S_TANK);
	data.rocket_img = MLV_load_image(GAME_FILE_ROCKET);
	data.poulpy_img = MLV_load_image(GAME_FILE_POULPY);
	
	define_player(&data);
	
	data.end = 1;
	data.deplacement = 0;
	
	do {
    /* Vide l'écran (placer ici pour debug dans calcul) */
		MLV_clear_window(COLOR_BLUE_SKY(j->theme));
		
    /* Calcul du jeu */
    if(!calcul(j, &data))
      break;
    
		/* Affichage */
		afficher(j, data);
		
		MLV_actualise_window();
		MLV_delay_according_to_frame_rate();
		
	} while(data.end);
  LOG(LOG_END_GAME);
}

