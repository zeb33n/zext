#include "editor.h"

int line_get_end_str(int cur_pos) {
  int line_start = line_get_end(cur_pos) - SCREEN.width_cs;
  if (SCREEN.text[line_start] == 0) {
    return line_start;
  }
  while (SCREEN.text[cur_pos] == 0) {
    cur_pos--;
  }
  while (SCREEN.text[cur_pos] != 0) {
    cur_pos++;
  }
  return cur_pos;
}

int line_get_end(int cur_pos) {
  return SCREEN.width_cs * (cur_pos / SCREEN.width_cs + 1);
}

void line_empty_from(int cur_pos) {
  int line_end = line_get_end(cur_pos);
  for (int i = cur_pos; i < line_end; i++) {
    if (SCREEN.text[i] == 0)
      break;
    SCREEN.text[i] = 0;
  }
}

void line_clear_from(int cur_pos) {
  int line_end = line_get_end(cur_pos);
  for (int i = cur_pos; i < line_end; i++) {
    if (SCREEN.text[i] == 0)
      break;
    Coord write_pos = cursor_get_coord_px(i);
    js_unwrite_char(write_pos.x, write_pos.y, BGRD_COL, SCREEN.font_px);
  }
}

void line_copy(int from_pos, int to_pos) {
  if (to_pos + SCREEN.width_cs >= LEN_MAX ||
      from_pos + SCREEN.width_cs >= LEN_MAX) {
    return;
  }
  for (int i = 0; i < SCREEN.width_cs; i++) {
    SCREEN.text[to_pos + i] = SCREEN.text[from_pos + i];
  }
}

void line_render_from(int cur_pos) {
  int line_end = line_get_end(cur_pos);
  char c = ' ';
  for (int i = cur_pos; i < line_end && c != 0; i++) {
    c = SCREEN.text[i];
    Coord write_pos = cursor_get_coord_px(i);
    js_unwrite_char(write_pos.x, write_pos.y, BGRD_COL, SCREEN.font_px);
    js_write_char(write_pos.x, write_pos.y, c, TEXT_COL, SCREEN.font_px);
  }
}
