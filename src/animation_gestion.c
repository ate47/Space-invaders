
#include "MLV/MLV_all.h"
#include "game_all.h"

struct _Animation_gestion{
  MLV_Image** frames;
  int delay;
  int counter;
  int current_frame;
  int frames_nb;
  int w, h;
};

Animation_gestion* creer_joueur_animation(MLV_Image* img, unsigned int frames, int delay, int x, int y, int width, int height) {
    unsigned int i;
    Animation_gestion* joueur;
    /* On alloue la mémoire pour le gestionnaire */
    ALLOC_MEMORY(joueur, sizeof(Animation_gestion), "joueur_anim");
    ALLOC_MEMORY(joueur->frames, sizeof(void*) * frames, "joueur_anim->frames");
    
    /* Initialisation des variables de la structures */
    joueur->frames_nb = frames;
    joueur->delay = delay;
    joueur->counter = 0;
    joueur->current_frame = 0;
    joueur->w = width;
    joueur->h = height;
    
    for (i = 0; i < frames; i++)
      joueur->frames[i] = MLV_copy_partial_image(img, x, y + height * i, width, height);
    
    return joueur;
}

void free_animation(Animation_gestion* gestion) {
	int i;
	for (i = 0; i < gestion->frames_nb; i++) {
		MLV_free_image(gestion->frames[i]);
		gestion->frames[i] = NULL;
	}
	free(gestion);
}

void draw_maj_joueur_animation(Animation_gestion* joueur, int x, int y, double angle) {
  maj_joueur_animation(joueur);
  
  draw_joueur_animation(joueur, x, y, angle);
}

void draw_inv_maj_joueur_animation(Animation_gestion* joueur, int x, int y, double angle) {
  joueur->counter--;
  if (joueur->counter < 0){
    joueur->counter = joueur->delay - 1;
    joueur->current_frame = joueur->current_frame - 1;
		if (joueur->current_frame < 0)
			joueur->current_frame += joueur->frames_nb;
  }
  
  draw_joueur_animation(joueur, x, y, angle);
}

void draw_joueur_animation(Animation_gestion* joueur, int x, int y, double angle) {
  MLV_Image* img;
  
  img = MLV_copy_image(joueur->frames[joueur->current_frame]);
  MLV_rotate_image(img, angle);
  
  MLV_draw_image(img, x, y);
  
  MLV_free_image(img);
}

void maj_joueur_animation(Animation_gestion* joueur) {
  joueur->counter++;
  if (joueur->counter > joueur->delay){
    joueur->counter = 0;
    joueur->current_frame = (joueur->current_frame + 1) % joueur->frames_nb;
  }
}
