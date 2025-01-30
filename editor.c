#include "editor.h"

static Screen SCREEN = {0, 0, 0, 0, 0, 0, 0, 0, NULL};

Coord cursor_get_pos_cs(int pos) {
  Coord out = {pos % SCREEN.width_cs, pos / SCREEN.width_cs};
  return out;
}

Coord cursor_get_pos_px(int pos) {
  Coord out = cursor_get_pos_cs(pos);
  out.x *= SCREEN.cursor_x_os;
  out.y *= SCREEN.cursor_y_os;
  out.y += SCREEN.font_px;
  return out;
}

int cursor_get_line_end(int cur_pos) {
  return SCREEN.width_cs * (cur_pos / SCREEN.width_cs + 1);
}

void cursor_render_line_from(int cur_pos, int line_end) {
  char c = ' ';
  for (int i = cur_pos; i < line_end && c != 0; i++) {
    c = SCREEN.text[i];
    Coord write_pos = cursor_get_pos_px(i);
    unwrite_char(write_pos.x, write_pos.y, BGRD_COL, SCREEN.font_px);
    write_char(write_pos.x, write_pos.y, c, TEXT_COL, SCREEN.font_px);
  }
}

void cursor_render() {
  Coord cursor_coords = cursor_get_pos_px(SCREEN.cursor);
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, TEXT_COL);
}

void cursor_clear() {
  Coord cursor_coords = cursor_get_pos_px(SCREEN.cursor);
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, BGRD_COL);
}

// make sure delete fills the end with 0s
void execute_bspace() {
  if (SCREEN.cursor == 0) {
    return;
  }
  cursor_clear();
  SCREEN.cursor--;
  int line_end = cursor_get_line_end(SCREEN.cursor);
  for (int i = SCREEN.cursor - 1; i < line_end - 1; i++) {
    SCREEN.text[i] = SCREEN.text[i + 1];
  }
  SCREEN.text[line_end - 1] = 0;
  // SCREEN.text[SCREEN.cursor] = ' ';
  cursor_render_line_from(SCREEN.cursor, cursor_get_line_end(SCREEN.cursor));
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
  int line_end = cursor_get_line_end(SCREEN.cursor);
  for (int i = line_end - 1; i > SCREEN.cursor; i--) {
    SCREEN.text[i] = SCREEN.text[i - 1];
  }
  SCREEN.text[SCREEN.cursor] = c;
  cursor_render_line_from(SCREEN.cursor, line_end);
  SCREEN.cursor++;
  cursor_render();
}

void editor_init(int w, int h, int font_size) {
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
