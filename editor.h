void fill_rect(int x, int y, int w, int h, unsigned int colour);
void write_char(int x, int y, unsigned char c, unsigned int color, int size);
void unwrite_char(int x, int y, unsigned int color, int size);
void log(int msg);

void foo(void);
void editor_keypress(char c);
void editor_special_keypress(char c);
void init(int w, int h, int font_size);

#define NULL (void*)0
#define LEN_MAX 80 * 100
#define FONT_HW_R 1.7
// figure out a better way to calc this
// maybe figure it out in init ratio of font size
#define FONT_VP_PX 10 * FONT_HW_R
#define BGRD_COL 0xFF181818
#define TEXT_COL 0xEEEEEEEE

enum KeysSpecial {
  ENTER = 13,
  LEFT = 37,
  UP = 38,
  RIGHT = 39,
  DOWN = 40,
  BSPACE = 8,
  DELETE = 46,
  TAB = 9,
};

typedef struct Screen {
  int width_px;
  int height_px;
  int width_cs;
  int height_cs;
  int font_px;
  int cursor;
  int cursor_x_os;
  int cursor_y_os;
  char* text;
} Screen;

typedef struct Coord {
  int x;
  int y;
} Coord;
