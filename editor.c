#include "editor.h"

static Screen SCREEN = {0, 0, 0, 0, 0, 0, 0, 0, NULL};

Coord cursor_get_pos_cs() {
  Coord out = {SCREEN.cursor % SCREEN.width_cs,
               SCREEN.cursor / SCREEN.width_cs};
  return out;
}

Coord cursor_get_pos_px() {
  Coord out = cursor_get_pos_cs();
  out.x *= SCREEN.cursor_x_os;
  out.y *= SCREEN.cursor_y_os;
  out.y += SCREEN.font_px;
  return out;
}

// TODO perhaps render from cursor will be useful
void render_screen(void) {
  if (SCREEN.text == NULL) {
    return;
  }
  for (int i = 0; i < SCREEN.height_cs; i++) {
    for (int j = 0; j < SCREEN.width_cs; j++) {
      write_char((j * SCREEN.font_px) / 2, i * SCREEN.font_px + SCREEN.font_px,
                 SCREEN.text[j + i * SCREEN.width_cs], TEXT_COL,
                 SCREEN.font_px);
    }
  }
}

void cursor_render_line_from() {}

void cursor_render() {
  Coord cursor_coords = cursor_get_pos_px();
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, TEXT_COL);
}

void cursor_clear() {
  Coord cursor_coords = cursor_get_pos_px();
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, BGRD_COL);
}

void init(int w, int h, int font_size) {
  // Check if size is ok
  int w_chars = w / (font_size / FONT_HW_R);
  int h_chars = h / (font_size + FONT_VP_PX);
  if (w_chars * h_chars > LEN_MAX) {
    return;
  }

  // Set basic parameters
  SCREEN.width_px = w;
  SCREEN.height_px = h;
  SCREEN.width_cs = w_chars;
  SCREEN.height_cs = h_chars;

  // Calibrate font size and spacings
  SCREEN.font_px = font_size;
  SCREEN.cursor_x_os = font_size / FONT_HW_R;
  SCREEN.cursor_y_os = font_size + FONT_VP_PX;

  // allocate and initialise text buffer
  static char text[LEN_MAX];
  SCREEN.text = text;
  for (int i = 0; i < LEN_MAX; i++) {
    SCREEN.text[i] = 0;
  }

  // initial rendering
  fill_rect(0, 0, w, h, BGRD_COL);
  cursor_render();
}

void execute_bspace() {
  if (SCREEN.cursor == 0) {
    return;
  }
  cursor_clear();
  SCREEN.cursor--;
  Coord cursor_coords = cursor_get_pos_px();
  unwrite_char(cursor_coords.x, cursor_coords.y, BGRD_COL, SCREEN.font_px);
  SCREEN.text[SCREEN.cursor] = 0;
  cursor_render();
}

void execute_left() {
  if (SCREEN.cursor == 0) {
    return;
  }
  cursor_clear();
  SCREEN.cursor--;
  cursor_render();
}

void execute_right() {
  if (SCREEN.cursor >= SCREEN.width_cs * SCREEN.height_cs) {
    return;
  }
  cursor_clear();
  SCREEN.cursor++;
  cursor_render();
}

void editor_special_keypress(char c) {
  switch (c) {
    case BSPACE:
      execute_bspace();
      return;
    case LEFT:
      execute_left();
      return;
    case RIGHT:
      execute_right();
      return;
    default:
      return;
  }
}

void editor_keypress(char c) {
  if (SCREEN.cursor >= SCREEN.width_cs * SCREEN.height_cs) {
    return;
  }
  cursor_clear();
  Coord cursor_coords = cursor_get_pos_px();
  write_char(cursor_coords.x, cursor_coords.y, c, TEXT_COL, SCREEN.font_px);
  SCREEN.text[SCREEN.cursor] = c;
  SCREEN.cursor++;
  cursor_render();
}

void foo(void) {
  // for (int i = 0; i < SCREEN.height * SCREEN.width; i++) {
  //   SCREEN.text[i] = 'g';
  // }
  // render_screen();
}
