#include "editor.h"

Coord cursor_get_coord_cs(int pos) {
  Coord out = {pos % SCREEN.width_cs, pos / SCREEN.width_cs};
  return out;
}

Coord cursor_get_coord_px(int pos) {
  Coord out = cursor_get_coord_cs(pos);
  out.x *= SCREEN.cursor_x_os;
  out.x += SCREEN.margin_w_cs * SCREEN.cursor_x_os;
  out.y *= SCREEN.cursor_y_os;
  out.y += SCREEN.font_px;
  return out;
}

void cursor_render() {
  Coord cursor_coords = cursor_get_coord_px(SCREEN.cursor);
  js_fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
               SCREEN.font_px, TEXT_COL);
}

void cursor_clear() {
  Coord cursor_coords = cursor_get_coord_px(SCREEN.cursor);
  js_fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
               SCREEN.font_px, BGRD_COL);
}

void cursor_mov_lr(int d) {
  int new_cur_pos;
  // if cursor is at the end of a line of text
  if (SCREEN.text[SCREEN.cursor] == 0 && d > 0) {
    new_cur_pos = line_get_end(SCREEN.cursor);
    // if cursor is at the start of a line
  } else if (SCREEN.cursor % SCREEN.width_cs == 0 && d < 0) {
    new_cur_pos = line_get_end_str(SCREEN.cursor - 1);
  } else {
    new_cur_pos = SCREEN.cursor + d;
  }
  if (new_cur_pos < 0 || new_cur_pos >= SCREEN.width_cs * SCREEN.height_cs) {
    return;
  }
  SCREEN.cursor = new_cur_pos;
}

void cursor_mov_ud(int d) {
  int line = SCREEN.cursor / SCREEN.width_cs;
  if ((line == 0 && d < 0) || (line == SCREEN.height_cs - 1 && d > 0)) {
    return;
  }
  int new_cur_pos = SCREEN.cursor + d * SCREEN.width_cs;
  if (SCREEN.text[new_cur_pos] == 0 || SCREEN.text[SCREEN.cursor] == 0) {
    new_cur_pos = line_get_end_str(new_cur_pos);
  }
  SCREEN.cursor = new_cur_pos;
}
