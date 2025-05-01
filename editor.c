#include "editor.h"

static Screen SCREEN = {
    0, 0,    0,
    0, 0,    0,
    0, NULL, {BGRD_COL, TEXT_COL, BGRD_COL_MARGIN, TEXT_COL_MARGIN}};

#include "cursor.c"
#include "keys.c"
#include "line.c"

void editor_dump_text() {
  for (int i = 0; i < SCREEN.height_cs; i++) {
    js_dump_line(&(SCREEN.text[i * SCREEN.width_cs]));
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
// rewrite with cursor functions
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

void editor_set_colours(int bgrd, int txt, int bgrd_margin, int txt_margin) {
  js_log_str("hello from in here");
  SCREEN.colours.bgrd = bgrd;
  SCREEN.colours.txt = txt;
  SCREEN.colours.bgrd_margin = bgrd_margin;
  SCREEN.colours.txt_margin = txt_margin;
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
  js_fill_rect(0, 0, w, h, SCREEN.colours.bgrd);
  js_fill_rect(0, 0, margin_w_cs * SCREEN.cursor_x_os, h,
               SCREEN.colours.bgrd_margin);
  js_write_char(0, font_size, '0', SCREEN.colours.txt_margin, font_size);
  for (int i = 0; i < h_cs; i++) {
    int num = i;
    int x = count_digits(num) - 1;
    while (num) {
      char c = num % 10 + 0x30;
      num /= 10;
      js_write_char(x * SCREEN.cursor_x_os, i * SCREEN.cursor_y_os + font_size,
                    c, SCREEN.colours.txt_margin, font_size);
      x--;
    }
  }
  cursor_render();
}
