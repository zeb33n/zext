#include "editor.h"
#define NULL (void*)0
#define LEN_MAX 80 * 100

typedef struct Screen {
  int width_px;
  int height_px;
  int width;
  int height;
  int font_size;
  int cursor;
  char* text;
} Screen;

static Screen SCREEN = {0, 0, 0, 0, 0, 0, NULL};

void init(int w, int h, int font_size) {
  SCREEN.width_px = w;
  SCREEN.height_px = h;
  SCREEN.width = w / (font_size / 2);
  SCREEN.height = h / font_size;
  SCREEN.font_size = font_size;
  static char text[LEN_MAX];
  SCREEN.text = text;
  fill_rect(0, 0, w, h, 0xFF181818);
}

void render_screen(void) {
  if (SCREEN.text == NULL) {
    return;
  }
  for (int i = 0; i < SCREEN.height; i++) {
    for (int j = 0; j < SCREEN.width; j++) {
      write_char(
          (j * SCREEN.font_size) / 2, i * SCREEN.font_size + SCREEN.font_size,
          SCREEN.text[j + i * SCREEN.width], 0xEEEEEEEE, SCREEN.font_size);
    }
  }
}

void editor_keypress(char c) {
  if (SCREEN.cursor == LEN_MAX - 1) {
    return;
  }
  SCREEN.text[SCREEN.cursor] = c;
  SCREEN.cursor++;
  render_screen();
}

void foo(void) {
  // for (int i = 0; i < SCREEN.height * SCREEN.width; i++) {
  //   SCREEN.text[i] = 'g';
  // }
  // render_screen();
}
