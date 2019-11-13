
#include "game_all.h"

int rnd(int min, int max) {
  return (int)((((double) rand()) / RAND_MAX) * (max - min) + min);
}

int est_dessus(int x, int y, int w, int h, int sx, int sy) {
	return sx >= x && sx <= x + w && sy >= y && sy <= y + h;
}

int est_dessus_double(double x, double y, double w, double h, double sx, double sy) {
	return sx >= x && sx <= x + w && sy >= y && sy <= y + h;
}

int key_pressed(MLV_Keyboard_button b) {
  return MLV_get_keyboard_state(b) == MLV_PRESSED;
}
