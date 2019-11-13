#include<stdio.h>
#include<time.h>
#include<string.h>
#include "game_all.h"


int lire(void* pointeur, const size_t taille_a_lire, FILE* f) {
  return fread(pointeur, 1, taille_a_lire, f) == taille_a_lire;
}

int ecrire(void* pointeur, const size_t taille_a_ecrire, FILE* f) {
  return fwrite(pointeur, 1, taille_a_ecrire, f) == taille_a_ecrire;
}

Sauvegardeur ouvrir_sauvegardeur() {
  /* Ouvre le fichier save.dat en ecriture */
	Sauvegardeur s = (Sauvegardeur) fopen(GAME_SAVE_FILE, "w");
	if (s == NULL) {
		WARNING("Le sauvegardeur n'a pas pu trouver le fichier de sauvegarde");
	}
  return s;
}

Liseur ouvrir_liseur() {
  /* Ouvre le fichier save.dat en lecture */
	Liseur l =  (Liseur) fopen(GAME_SAVE_FILE, "r");
	if (l == NULL) {
		WARNING("Le liseur n'a pas pu trouver le fichier de sauvegarde");
	}
  return l;
}

int lire_config(Liseur liseur, size_t taille_config, void* variable_a_lire) {
  return liseur != NULL /* Si le liseur est NULL (problème avec le fichier) on renvoie faux */
    && lire(variable_a_lire, taille_config, liseur); /* Sinon essaye de lire la variable sinon */
}

int ecrire_config(Sauvegardeur sauvegardeur, size_t taille_config, void* variable_a_ecrire) {
  return sauvegardeur != NULL /* Si le sauvegardeur est NULL (problème avec le fichier) on renvoie faux */
    && ecrire(variable_a_ecrire, taille_config, sauvegardeur); /* Sinon essaye d'écrire la variable sinon */
}

int fermer_sauvegardeur(Sauvegardeur sauvegardeur) {
  /* Ferme le fichier si il était ouvert */
  return sauvegardeur == NULL || fclose(sauvegardeur);
}

int fermer_liseur(Liseur liseur) {
  /* Ferme le fichier si il était ouvert */
  return liseur == NULL || fclose(liseur);
}

/*
 * NB: +ou- les mêmes fonctions jusqu'à la ligne 255
 */

/**
 * Lecture d'une entité avec un liseur
 */
void lire_entite(Liseur liseur, Entite* entite) {
	if (!lire_config(liseur, sizeof(double), &entite->x))
		entite->x = 0;
	if (!lire_config(liseur, sizeof(double), &entite->y))
		entite->y = 0;
}

/**
 * Ecriture d'une entité avec un sauvegardeur
 */
void ecrire_entite(Sauvegardeur sauvegardeur, Entite* entite) {
	ecrire_config(sauvegardeur, sizeof(double), &entite->x);
	ecrire_config(sauvegardeur, sizeof(double), &entite->y);
}

/**
 * Lecture d'un enemis avec un liseur
 */
void lire_enemis(Liseur liseur, Enemis* enemis) {
	int dessin;
	lire_entite(liseur, &enemis->position);
	if(!lire_config(liseur, sizeof(int), &dessin))
		dessin = 1;
	switch(dessin){
		case 1:
			enemis->type = enemis1;
			break;
		case 2:
			enemis->type = enemis2;
			break;
		default:
			enemis->type = enemis3;
			break;
	}
	if(!lire_config(liseur, sizeof(int), &enemis->vivant))
		enemis->vivant = 0;
}

/**
 * Ecriture d'un enemis avec un sauvegardeur
 */
void ecrire_enemis(Sauvegardeur sauvegardeur, Enemis* enemis) {
	int i;
	ecrire_entite(sauvegardeur, &enemis->position);
	switch(enemis->type) {
		case enemis1:
			i = 1;
			break;
		case enemis2:
			i = 2;
			break;
		case enemis3:
			i = 3;
			break;
		default:
			i = 4;
			break;
	}
	
	ecrire_config(sauvegardeur, sizeof(int), &i);
	ecrire_config(sauvegardeur, sizeof(int), &enemis->vivant);
}

/**
 * Lecture d'un tir avec un liseur
 */
void lire_tir(Liseur liseur, Tir* tir) {
	lire_entite(liseur, &tir->tir);
	if(!lire_config(liseur, sizeof(int), &tir->nb_impact))
		tir->nb_impact = 0;
}

/**
 * Ecriture d'un tir avec un sauvegardeur
 */
void ecrire_tir(Sauvegardeur sauvegardeur, Tir* tir) {
	ecrire_entite(sauvegardeur, &tir->tir);
	ecrire_config(sauvegardeur, sizeof(int), &tir->nb_impact);
}

/**
 * Lecture d'une configuration avec un liseur
 */
void lire_configuration(Liseur liseur, Configuration* config) {
	int i;
	for (i = 0; i < NB_MAX_ENEMIS; i++) {
		lire_enemis(liseur, &config->enemis[i]);
	}
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++) {
		lire_tir(liseur, &config->tirs_enemis[i]);
	}
	lire_tir(liseur, &config->tir_allie);
	if (!lire_config(liseur, sizeof(double), &config->pos_allie))
		config->pos_allie = 0.0;
	if (!lire_config(liseur, sizeof(int), &config->score))
		config->score = 0;
	if (!lire_config(liseur, sizeof(int), &config->termine))
		config->termine = 1;
	if (!lire_config(liseur, sizeof(double), &config->decalage))
		config->decalage= 0.0;
	if (!lire_config(liseur, sizeof(double), &config->dec_bonus))
		config->dec_bonus= 0.0;
}

/**
 * Ecriture d'une configuration avec un sauvegardeur
 */
void ecrire_configuration(Sauvegardeur sauvegardeur, Configuration* config) {
	int i;
	for (i = 0; i < NB_MAX_ENEMIS; i++) {
		ecrire_enemis(sauvegardeur, &config->enemis[i]);
	}
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++) {
		ecrire_tir(sauvegardeur, &config->tirs_enemis[i]);
	}
	ecrire_tir(sauvegardeur, &config->tir_allie);
	
	ecrire_config(sauvegardeur, sizeof(double), &config->pos_allie);
	ecrire_config(sauvegardeur, sizeof(int), &config->score);
	ecrire_config(sauvegardeur, sizeof(int), &config->termine);
	ecrire_config(sauvegardeur, sizeof(double), &config->decalage);
	ecrire_config(sauvegardeur, sizeof(double), &config->dec_bonus);
}

/**
 * Ecriture d'un score avec un sauvegardeur
 */
void ecrire_score(Sauvegardeur sauvegardeur, Score* score) {
	ecrire_config(sauvegardeur, sizeof(int), &score->points);
	ecrire_config(sauvegardeur, sizeof(int), &score->name_size);
	ecrire_config(sauvegardeur, sizeof(char) * (score->name_size+1), score->name);
}
/**
 * Ecriture d'un score avec un sauvegardeur
 */
void lire_score(Liseur liseur, Score* score) {
	if (!lire_config(liseur, sizeof(int), &score->points))
		score->points = 0;
	if (!lire_config(liseur, sizeof(int), &score->name_size))
		score->name_size = 0;
  
  if (score->name_size > NB_MAX_CHAR_SCORE){
    ERR(ERROR_MAX_SCORE_CHAR_REACH);
    fermer_liseur(liseur);
    score->name_size = 0;
  }
  
  ALLOC_MEMORY(score->name, sizeof(char) * (1 + score->name_size), "Pseudo");
  
	if (!lire_config(liseur, sizeof(char) * (1 + score->name_size), score->name)) {
    score->name_size = 0;
  }
  
  score->name[score->name_size] = '\0';
}

void lire_jeu(Jeu* jeu) {
	int i;
	/* Ouvre un liseur de sauvegarde */
	Liseur liseur = ouvrir_liseur();
	
	/* Lecture des scores */
  for (i = 0; i < NB_MAX_SAVES; i++)
    lire_score(liseur, &jeu->scores[i]);
		
		lire_configuration(liseur, &jeu->config_actuel);
	for (i = 0; i < NB_MAX_SAVES; i++)
		lire_configuration(liseur, &jeu->save[i]);
	
	if (!lire_config(liseur, sizeof(int), &jeu->theme))
		jeu->theme = 0;
	
	/* Fermer le liseur de sauvegarde */
	fermer_liseur(liseur);
}

void sauvegarder_jeu(Jeu* jeu) {
	int i;
	/* Ouvre un sauvegardeur */
	Sauvegardeur sauvegardeur = ouvrir_sauvegardeur();
	
	/* Sauvegarde des scores */
  for (i = 0; i < NB_MAX_SAVES; i++)
    ecrire_score(sauvegardeur, &jeu->scores[i]);
	
	ecrire_configuration(sauvegardeur, &jeu->config_actuel);
	for (i = 0; i < NB_MAX_SAVES; i++)
		ecrire_configuration(sauvegardeur, &jeu->save[i]);
	
	ecrire_config(sauvegardeur, sizeof(int), &jeu->theme);
	
	/* Fermer le sauvegardeur */
	fermer_sauvegardeur(sauvegardeur);
}

void clone_config(Configuration config1, Configuration* config2) {
	int i;
	
	config2->termine = config1.termine;
	config2->score = config1.score;
	config2->pos_allie = config1.pos_allie;
	config2->decalage = config1.decalage;
	config2->dec_bonus = config1.dec_bonus;
  
	config2->tir_allie = config1.tir_allie;
	
	for (i = 0; i < NB_MAX_ENEMIS; i++)
		config2->enemis[i] = config1.enemis[i];
	
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++)
		config2->tirs_enemis[i] = config1.tirs_enemis[i];
}

void vider_config(Configuration* config) {
  int i, j,numEnemi=0;
	config->termine = 0;
	config->score = 0;
	config->pos_allie = 0;
	config->tir_allie.nb_impact = 0;
	config->decalage = VERS_RADIAN(90);
	config->dec_bonus = -GAME_MIN_TIME_BEFORE_POULPY;
  
	for (i = 0; i < NB_MAX_TIR_ENEMIS; i++)
		config->tirs_enemis[i].nb_impact = 0;
	
	for (i = 0; i < NB_MAX_ENEMIS; i++)
		config->enemis[i].vivant = 0;

	for(i=1;i<4;i++) {
	  for(j=0;j<ENEMIS_PAR_LIGNE;j++) {
      /* marge de 3 (pour le déplacement) 6 enemis par ligne */
	    config->enemis[numEnemi].position.x= 1.0/ENEMIS_PAR_LIGNE*j;
      
      /* marge de 1 (pour le bonus), 3 ligne d'enemis au départ */
	    config->enemis[numEnemi].position.y= 1.0/LIGNE_ENEMIS*i;

	    config->enemis[numEnemi].type = rnd(0,3);
	    config->enemis[numEnemi].vivant = 1;
	    numEnemi ++;
	  }
	}
}

/* on parcour la chaine en introduisant le nouveau score */
void sauvegarde_score(Jeu *j, char* pseudo) {
  int i,score=j->config_actuel.score;
  Score new;
  Score tmp;
  
  new.name = pseudo;
  new.name_size = strlen(pseudo);
  new.points = score;
  
  for(i = 0; i < NB_MAX_SAVES; i++){
    if(score > j->scores[i].points) {
      tmp = j->scores[i];
      j->scores[i] = new;
      new = tmp;
    }
  }
}


