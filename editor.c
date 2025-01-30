#include "editor.h"

static Screen SCREEN = {0, 0, 0, 0, 0, 0, NULL};

Coord cursor_get_pos_cs() {
  Coord out = {SCREEN.cursor % SCREEN.width_cs,
               SCREEN.cursor / SCREEN.width_cs};
  return out;
}

Coord cursor_get_pos_px() {
  Coord out = cursor_get_pos_cs();
  out.x *= (SCREEN.font_size / CHAR_HW_R);
  out.y *= SCREEN.font_size;
  out.y += (SCREEN.font_size);
  return out;
}

void init(int w, int h, int font_size) {
  int w_chars = w / (font_size / CHAR_HW_R);
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
  if (SCREEN.cursor >= SCREEN.width_cs * SCREEN.height_cs) {
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
