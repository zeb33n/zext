#include "editor.h"

void bspace_normal() {
  cursor_clear();
  cursor_mov_lr(-1);
  int line_end = line_get_end(SCREEN.cursor);
  for (int i = SCREEN.cursor; i < line_end - 1; i++) {
    SCREEN.text[i] = SCREEN.text[i + 1];
  }
  SCREEN.text[line_end - 1] = 0;

  line_render_from(SCREEN.cursor);
  cursor_render();
}

void bspace_linestart() {
  cursor_clear();
  int current_line = SCREEN.cursor / SCREEN.width_cs;
  if (current_line == 0) {
    return;
  }

  cursor_mov_lr(-1);
  int prev_line_end = line_get_end(SCREEN.cursor);

  for (int i = 0; i < prev_line_end - SCREEN.cursor; i++) {
    if (SCREEN.text[i + prev_line_end] == 0) {
      break;
    }
    SCREEN.text[i + SCREEN.cursor] = SCREEN.text[prev_line_end + i];
  }
  line_render_from(SCREEN.cursor);

  for (int line = current_line; line < SCREEN.height_cs - 1; line++) {
    int line_ind = line * SCREEN.width_cs;
    line_clear_from(line_ind);
    line_copy(line_ind + SCREEN.width_cs, line_ind);
    line_render_from(line_ind);
  }

  int last_line_start = (SCREEN.height_cs - 1) * SCREEN.width_cs;
  line_clear_from(last_line_start);
  line_empty_from(last_line_start);
  line_render_from(last_line_start);

  cursor_render();
}

void execute_bspace() {
  if (SCREEN.cursor == 0) {
    return;
  }
  if (SCREEN.cursor % SCREEN.width_cs == 0) {
    bspace_linestart();
  } else {
    bspace_normal();
  }
}

// cursor kinda has one more position per line than the characters
void execute_left() {
  cursor_clear();
  cursor_mov_lr(-1);
  cursor_render();
}

void execute_right() {
  cursor_clear();
  cursor_mov_lr(1);
  cursor_render();
}

void execute_down() {
  cursor_clear();
  cursor_mov_ud(1);
  cursor_render();
}

void execute_up() {
  cursor_clear();
  cursor_mov_ud(-1);
  cursor_render();
}

// TODO what to do when text is pushed off screen.
// we need to think about scrolling.
void execute_enter() {
  int current_line = SCREEN.cursor / SCREEN.width_cs;
  int current_line_end = line_get_end(SCREEN.cursor);
  int next_line_ind = (current_line + 1) * SCREEN.width_cs;
  if (next_line_ind >= SCREEN.width_cs * SCREEN.height_cs) {
    return;
  }
  cursor_clear();

  // copy lines from the end of the screen
  for (int line = SCREEN.height_cs - 1; line > current_line + 1; line--) {
    int line_ind = line * SCREEN.width_cs;
    line_clear_from(line_ind);
    line_copy(line_ind - SCREEN.width_cs, line_ind);
    line_render_from(line_ind);
  }

  // special case for the line where the key was pressed
  line_clear_from(SCREEN.cursor);
  line_clear_from(next_line_ind);
  line_empty_from(next_line_ind);

  for (int i = 0; i < current_line_end - SCREEN.cursor; i++) {
    if (SCREEN.text[i + SCREEN.cursor] == 0) {
      break;
    }
    SCREEN.text[next_line_ind + i] = SCREEN.text[i + SCREEN.cursor];
    SCREEN.text[i + SCREEN.cursor] = 0;
  }

  line_render_from(next_line_ind);
  SCREEN.cursor = next_line_ind;
  cursor_render();
}
