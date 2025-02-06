#include "editor.h"

static Screen SCREEN = {0, 0, 0, 0, 0, 0, 0, NULL};

#include "cursor.c"
#include "line.c"

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

int count_digits(int i) {
  int count = 0;
  while (i) {
    i /= 10;
    count++;
  }
  return count;
}

void editor_init(int w, int h, int font_size) {
  // Check if size is ok
  int vert_pad_px = font_size / 4;
  int h_cs = h / (font_size + vert_pad_px);
  int margin_w_cs = count_digits(h_cs) + 1;
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
  SCREEN.cursor_y_os = font_size + vert_pad_px;

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
    int x = count_digits(num) - 1;
    while (num) {
      char c = num % 10 + 0x30;
      num /= 10;
      write_char(x * SCREEN.cursor_x_os, i * SCREEN.cursor_y_os + font_size, c,
                 TEXT_COL_MARGIN, font_size);
      x--;
    }
  }
  cursor_render();
}
