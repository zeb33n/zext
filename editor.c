#include "editor.h"

static Screen SCREEN = {0, 0, 0, 0, 0, 0, 0, 0, NULL};

// CURSOR !

Coord cursor_get_coord_cs(int pos) {
  Coord out = {pos % SCREEN.width_cs, pos / SCREEN.width_cs};
  return out;
}

Coord cursor_get_coord_px(int pos) {
  Coord out = cursor_get_coord_cs(pos);
  out.x *= SCREEN.cursor_x_os;
  out.y *= SCREEN.cursor_y_os;
  out.y += SCREEN.font_px;
  return out;
}

void cursor_render() {
  Coord cursor_coords = cursor_get_coord_px(SCREEN.cursor);
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, TEXT_COL);
}

void cursor_clear() {
  Coord cursor_coords = cursor_get_coord_px(SCREEN.cursor);
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, BGRD_COL);
}

// Line !

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
    unwrite_char(write_pos.x, write_pos.y, BGRD_COL, SCREEN.font_px);
  }
}

void line_render_from(int cur_pos) {
  int line_end = line_get_end(cur_pos);
  char c = ' ';
  for (int i = cur_pos; i < line_end && c != 0; i++) {
    c = SCREEN.text[i];
    Coord write_pos = cursor_get_coord_px(i);
    unwrite_char(write_pos.x, write_pos.y, BGRD_COL, SCREEN.font_px);
    write_char(write_pos.x, write_pos.y, c, TEXT_COL, SCREEN.font_px);
  }
}

// SPECIAL KEYS !

// make sure delete fills the end with 0s
void execute_bspace() {
  if (SCREEN.cursor == 0) {
    return;
  }
  cursor_clear();

  SCREEN.cursor--;
  int line_end = line_get_end(SCREEN.cursor);
  for (int i = SCREEN.cursor - 1; i < line_end - 1; i++) {
    SCREEN.text[i] = SCREEN.text[i + 1];
  }
  SCREEN.text[line_end - 1] = 0;

  line_render_from(SCREEN.cursor);
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

// TODO what to do when text is pushed off screen.
// we need to think about scrolling.
void execute_enter() {
  cursor_clear();
  int current_line = SCREEN.cursor / SCREEN.width_cs;
  for (int line = SCREEN.height_cs - 1; line > current_line; line--) {
    int line_ind = line * SCREEN.width_cs;
    for (int i = 0; i <= SCREEN.width_cs; i++) {
      SCREEN.text[line_ind + i] = SCREEN.text[(line_ind - SCREEN.width_cs) + i];
    }
    line_render_from(line_ind);
  }
}

// PUBLIC FUNCS !

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
    case ENTER:
      execute_enter();
      return;
    default:
      return;
  }
}

// TODO what to do when we push text off screen?
void editor_keypress(char c) {
  if (SCREEN.cursor >= SCREEN.width_cs * SCREEN.height_cs) {
    return;
  }
  cursor_clear();
  int line_end = line_get_end(SCREEN.cursor);

  for (int i = line_end - 1; i > SCREEN.cursor; i--) {
    SCREEN.text[i] = SCREEN.text[i - 1];
  }
  SCREEN.text[SCREEN.cursor] = c;

  line_render_from(SCREEN.cursor);
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
  // TODO add one extra 0 delimited column to the text
  static char text[LEN_MAX];
  SCREEN.text = text;
  for (int i = 0; i < LEN_MAX; i++) {
    SCREEN.text[i] = 0;
  }

  // initial rendering
  fill_rect(0, 0, w, h, BGRD_COL);
  cursor_render();
}
