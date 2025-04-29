#ifndef BALL_H
#define BALL_H

// Deklarációk
extern double gravity;  // Gravitációs gyorsulás

// Funkciók
void draw_ball(double ball_pos_y);
void update_ball_position(double* ball_pos_y, double* ball_speed_y, double elapsed_time);

#endif
