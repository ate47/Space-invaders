
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include "MLV/MLV_all.h"
#include "game_all.h"

void fin_jeu(void* data) {
    LOG(LOG_QUIT_GAME);
    /* Libére la fenetre si elle existe */
#if !MENU_TEXTE
    MLV_free_window();
#endif
    sauvegarder_jeu((Jeu*) data);
    exit(EXIT_SUCCESS);
}

static int check(const char* f) {
  if (access(f, F_OK) || access(f, R_OK)) {
    fprintf(stderr, ERROR_CANT_LOAD_FILE, f);
    return 1;
  }
  return 0;
}

int check_files() {
  int end = 0;
  
  /* On test si on peut ecrire le fichier de sauvegarde */
  if (!access(GAME_SAVE_FILE, F_OK) && access(GAME_SAVE_FILE, W_OK)) {
    fprintf(stderr, ERROR_CANT_WRITE_SAVE_FILE, GAME_SAVE_FILE);
    end = 1;
  }
  
  /* On test l'existance et la possibilité de lecture de chaque fichier du jeu */
  
  end |= check(GAME_FILE_ALFRED);
  end |= check(GAME_FILE_POULPY);
  end |= check(GAME_FILE_ROCKET);
  end |= check(GAME_FILE_ENNEMIS1);
  end |= check(GAME_FILE_ENNEMIS2);
  end |= check(GAME_FILE_ENNEMIS3);
  end |= check(GAME_FILE_JACQUIE_S_TANK);
  
  return end;
}

int main() {
	Jeu j;
  
  LOG(LOG_CHECK_FILES);
  if(check_files()) {
    fprintf(stderr, ERROR_CHECK_FAIL);
    exit(EXIT_FAILURE);
  }
  
	LOG(LOG_LOAD_SAVE);
	lire_jeu(&j);
	
  srand(time(NULL));
  
  /* Creer la fenetre */
  if ( !MENU_TEXTE ) {
			MLV_execute_at_exit(fin_jeu, &j);
      MLV_create_window(GAME_NAME, NULL, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
			MLV_change_frame_rate(FRAME_RATE);
	}
  
  /* Lance le menu */
  menu(&j);
	
  fin_jeu(&j);
  exit(EXIT_FAILURE);
}
