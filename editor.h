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
#define CHAR_HW_R 1.5

enum KeysSpecial {
  ENTER = 13,
  UP = 38,
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
  int font_size;
  int cursor;
  char* text;
} Screen;

typedef struct Coord {
  int x;
  int y;
} Coord;
