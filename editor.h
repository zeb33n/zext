// IMPORTS
void fill_rect(int x, int y, int w, int h, unsigned int colour);
void write_char(int x, int y, unsigned char c, unsigned int color, int size);
void unwrite_char(int x, int y, unsigned int color, int size);
void log(int msg);
void log_str(char* msg);

// EXPORTS
void editor_keypress(char c);
void editor_special_keypress(char c);
void editor_init(int w, int h, int font_size);
void editor_dump_text();

// DEFINITIONS
#define NULL (void*)0
#define LEN_MAX 80 * 100
#define FONT_HW_R 1.7
#define BGRD_COL 0xFF3c3836
#define TEXT_COL 0xFF548588
#define TEXT_COL_MARGIN 0xFF8ec07c
#define BGRD_COL_MARGIN 0xFF3c3836

// INTERNAL DECLARATIONS
enum KeysSpecial {
  TAB = 9,
  ENTER = 13,
  LEFT = 37,
  UP = 38,
  RIGHT = 39,
  DOWN = 40,
  BSPACE = 8,
  DELETE = 46,
};

typedef struct Screen {
  int width_cs;
  int height_cs;
  int font_px;
  int cursor;
  int cursor_x_os;
  int cursor_y_os;
  int margin_w_cs;
  char* text;
} Screen;

typedef struct Coord {
  int x;
  int y;
} Coord;

int line_get_end(int cur_pos);
int line_next_end_str(int i);
