void fill_rect(int x, int y, int w, int h, unsigned int colour);
void write_char(int x, int y, unsigned char c, unsigned int color, int size);
void unwrite_char(int x, int y, unsigned int color, int size);
void log(int msg);

void foo(void);
void editor_keypress(char c);
void editor_special_keypress(char c);
void init(int w, int h, int font_size);
