#ifndef PAD_H
#define PAD_H

#include <GL/gl.h>

enum ColorTheme
{
    RED_THEME,
    GREEN_THEME,
    BLUE_THEME  // Kék színséma hozzáadása
};

typedef struct Pad
{
    float x;
    float y;
    float width;
    float height;
    float limit;
    float speed;
    enum ColorTheme color_theme;
} Pad;

void init_pad(Pad* pad, float x, float table_height, enum ColorTheme color_theme);
void set_pad_position(Pad* pad, float position);
void set_pad_speed(Pad* pad, float speed);
void update_pad(Pad* pad, double time);
void render_pad(Pad* pad);

#endif /* PAD_H */
