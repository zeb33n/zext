#include "editor.h"

static Screen SCREEN = {0, 0, 0, 0, 0, 0, 0, NULL};

// CURSOR !

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
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, TEXT_COL);
}

void cursor_clear() {
  Coord cursor_coords = cursor_get_coord_px(SCREEN.cursor);
  fill_rect(cursor_coords.x, cursor_coords.y - (SCREEN.font_px / 1.2), 4,
            SCREEN.font_px, BGRD_COL);
}

void cursor_mov_lr(int d) {
  // if cursor is at the edge of the editors space
  if ((SCREEN.cursor == 0 && d < 0) ||
      (SCREEN.cursor >= SCREEN.width_cs * SCREEN.height_cs && d > 0)) {
    return;
    // if cursor is at the end of a line of text
  } else if (SCREEN.text[SCREEN.cursor] == 0 && d > 0) {
    SCREEN.cursor = line_get_end(SCREEN.cursor);
    // if cursor is at the start of a line
  } else if (SCREEN.cursor % SCREEN.width_cs == 0 && d < 0) {
    SCREEN.cursor = line_next_end_str(SCREEN.cursor - 1);
  } else {
    SCREEN.cursor += d;
  }
}

void cursor_mov_ud(int d) {
  int line = SCREEN.cursor / SCREEN.width_cs;
  if ((line == 0 && d < 0) || (line == SCREEN.height_cs - 1 && d > 0)) {
    return;
  }
  int new_cur_pos = SCREEN.cursor + d * SCREEN.width_cs;
  if (SCREEN.text[new_cur_pos] == 0 || SCREEN.text[SCREEN.cursor] == 0) {
    new_cur_pos = line_next_end_str(new_cur_pos);
  }
  SCREEN.cursor = new_cur_pos;
}

// Line !

// TODO only ever go up one string or bad error is going to get you
// JUST rework this function in general
int line_next_end_str(int i) {
  while (SCREEN.text[i] == 0) {
    i--;
  }
  return i + 1;
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
    unwrite_char(write_pos.x, write_pos.y, BGRD_COL, SCREEN.font_px);
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
    unwrite_char(write_pos.x, write_pos.y, BGRD_COL, SCREEN.font_px);
    write_char(write_pos.x, write_pos.y, c, TEXT_COL, SCREEN.font_px);
  }
}

// SPECIAL KEYS !

// TODO delete lines
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

// cursor kinda has one more position per line than the characters
void execute_left() {
  if (SCREEN.cursor == 0) {
    return;
  }
  cursor_clear();
  cursor_mov_lr(-1);
  cursor_render();
}

void execute_right() {
  if (SCREEN.cursor >= SCREEN.width_cs * SCREEN.height_cs) {
    return;
  }
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
  // TODO only copy last defined line +1 -> only have to iterate over the
  // first index of each line from the bottom and check if its 0
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

// PUBLIC FUNCS !

void editor_dump_text() {
  for (int i = 0; i < SCREEN.height_cs; i++) {
    log_str(&(SCREEN.text[i * SCREEN.width_cs]));
  }
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
    case ENTER:
      execute_enter();
      return;
    case UP:
      execute_up();
      return;
    case DOWN:
      execute_down();
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
  int h_cs = h / (font_size + FONT_VP_PX);
  int margin_w_cs = h_cs / 10 + 2;
  int w_cs = (w / (font_size / FONT_HW_R)) - margin_w_cs;
  if (w_cs * h_cs > LEN_MAX) {
    return;
  }

  // Set basic parameters
  SCREEN.width_cs = w_cs;
  SCREEN.height_cs = h_cs;
  SCREEN.margin_w_cs = margin_w_cs;

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
  fill_rect(0, 0, margin_w_cs * SCREEN.cursor_x_os, h, BGRD_COL_MARGIN);
  write_char(0, font_size, '0', TEXT_COL_MARGIN, font_size);
  for (int i = 0; i < h_cs; i++) {
    int num = i;
    while (num) {
      char c = num % 10 + 0x30;
      num /= 10;
      write_char(num * SCREEN.cursor_x_os, i * SCREEN.cursor_y_os + font_size,
                 c, TEXT_COL_MARGIN, font_size);
    }
  }
  cursor_render();
}
