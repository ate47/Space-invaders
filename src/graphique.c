
#include "game_all.h"

MLV_Image* charger_ennemis(Dessin d){
  MLV_Image* monster;
  
  switch(d){
  case enemis1:
    monster = MLV_load_image (GAME_FILE_ENNEMIS1);
    break;
  case enemis2:
    monster = MLV_load_image (GAME_FILE_ENNEMIS2);
    break;
  default:
    monster = MLV_load_image (GAME_FILE_ENNEMIS3);
    break;
  }
  MLV_resize_image(monster, GAME_SIZE_ENEMIS, GAME_SIZE_ENEMIS * GAME_FRAMES_ENEMIS);
  return monster;
}

void creer_liste_animation(Animation_gestion** invaders){
  invaders[0]=creer_joueur_animation(charger_ennemis(enemis1),3,30,0,0,GAME_SIZE_ENEMIS,GAME_SIZE_ENEMIS);
  invaders[1]=creer_joueur_animation(charger_ennemis(enemis2),3,30,0,0,GAME_SIZE_ENEMIS,GAME_SIZE_ENEMIS);
  invaders[2]=creer_joueur_animation(charger_ennemis(enemis3),3,30,0,0,GAME_SIZE_ENEMIS,GAME_SIZE_ENEMIS);
  return;
}

void anim_ennemis(Configuration config, Animation_gestion *invaders[], Animation_gestion *poulpy_player) {
  int i;
  int dec = cos(config.decalage)*GAME_SIZE_ENEMIS*GAME_SHIFT_FACTOR;
  
  for(i=0;i<NB_MAX_ENEMIS;i++){
    if(config.enemis[i].vivant){
      draw_joueur_animation(invaders[config.enemis[i].type],
			    config.enemis[i].position.x * GAME_SIZE + GAME_LEFT + dec,
			    config.enemis[i].position.y * GAME_SIZE + GAME_TOP
			    , 0);
    }
  }
  if (config.dec_bonus > 0)
    draw_maj_joueur_animation(poulpy_player, 
                            WINDOW_WIDTH / 2 + sin(config.dec_bonus)*GAME_SIZE_POULPY*GAME_SHIFT_POULPY_FACTOR,
                            GAME_TOP + (GAME_UNIT - GAME_SIZE_POULPY) / 2,
                            VERS_DEGRE(config.decalage) /* Parce que yolo (c'est drôle) */
                            );
  maj_joueur_animation(invaders[0]);
  maj_joueur_animation(invaders[1]);
  maj_joueur_animation(invaders[2]);
  return;
}


void afficher_tirs(Configuration config){
  int i;
  Tir t;
  for(i=0;i<NB_MAX_TIR_ENEMIS;i++){
    t=config.tirs_enemis[i];
    if(t.nb_impact){
      MLV_draw_filled_rectangle(t.tir.x * GAME_SIZE + GAME_LEFT - GAME_SIZE_TIR_W/2, t.tir.y * GAME_SIZE + GAME_TOP,GAME_SIZE_TIR_W, GAME_SIZE_TIR_H, COLOR_LASER());
    }
  }
  return;
}


