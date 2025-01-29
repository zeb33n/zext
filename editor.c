#include "editor.h"
#define NULL (void*)0
#define LEN_MAX 80 * 100

enum KeysSpecial {
  ENTER = 13,
  UP = 38,
  DOWN = 40,
  BSPACE = 8,
  DELETE = 46,
  TAB = 9,
};

typedef struct Screen {
  int width_px;
  int height_px;
  int width_cs;
  int height_cs;
  int font_size;
  int cursor;
  char* text;
} Screen;

typedef struct Coord {
  int x;
  int y;
} Coord;

static Screen SCREEN = {0, 0, 0, 0, 0, 0, NULL};

// CHECK thingy dont forget if num cahrs is bigger than len max
void init(int w, int h, int font_size) {
  int w_chars = w / (font_size / 2);
  int h_chars = h / font_size;
  if (w_chars * h_chars > LEN_MAX) {
    return;
  }
  SCREEN.width_px = w;
  SCREEN.height_px = h;
  SCREEN.width_cs = w_chars;
  SCREEN.height_cs = h_chars;
  SCREEN.font_size = font_size;
  static char text[LEN_MAX];
  SCREEN.text = text;
  fill_rect(0, 0, w, h, 0xFF181818);
}

// TODO perhaps render from cursor will be useful
void render_screen(void) {
  if (SCREEN.text == NULL) {
    return;
  }
  for (int i = 0; i < SCREEN.height_cs; i++) {
    for (int j = 0; j < SCREEN.width_cs; j++) {
      write_char(
          (j * SCREEN.font_size) / 2, i * SCREEN.font_size + SCREEN.font_size,
          SCREEN.text[j + i * SCREEN.width_cs], 0xEEEEEEEE, SCREEN.font_size);
    }
  }
}

Coord cursor_get_pos_cs() {
  Coord out = {SCREEN.cursor % SCREEN.width_cs,
               SCREEN.cursor / SCREEN.width_cs};
  return out;
}

Coord cursor_get_pos_px() {
  Coord out = cursor_get_pos_cs();
  out.x *= (SCREEN.font_size / 2);
  out.y *= SCREEN.font_size;
  out.y += (SCREEN.font_size);
  return out;
}

void execute_bspace() {
  if (SCREEN.cursor == 0) {
    return;
  }
  SCREEN.cursor--;
  Coord cursor_coords = cursor_get_pos_px();
  unwrite_char(cursor_coords.x, cursor_coords.y, 0xFF181818, SCREEN.font_size);
  SCREEN.text[SCREEN.cursor] = 0;
}

void editor_special_keypress(char c) {
  switch (c) {
    case BSPACE:
      execute_bspace();
      return;
    default:
      return;
  }
}

void editor_keypress(char c) {
  if (SCREEN.cursor == LEN_MAX - 1) {
    return;
  }
  Coord cursor_coords = cursor_get_pos_px();
  write_char(cursor_coords.x, cursor_coords.y, c, 0xEEEEEEEE, SCREEN.font_size);
  SCREEN.text[SCREEN.cursor] = c;
  SCREEN.cursor++;
}

void foo(void) {
  // for (int i = 0; i < SCREEN.height * SCREEN.width; i++) {
  //   SCREEN.text[i] = 'g';
  // }
  // render_screen();
}
