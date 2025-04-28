#ifndef BALL_H
#define BALL_H

/**
 * Ball position, speed, and spin
 */
typedef struct Ball
{
    float x;
    float y;
    float radius;
    float speed_x;
    float speed_y;
    float spin;  // A labda forgásának sebessége
} Ball;

/**
 * Move the ball to the given position.
 */
void init_ball(Ball* ball, float x, float y);

/**
 * Update the ball.
 * @param time elapsed time in seconds.
 */
void update_ball(Ball* ball, double time);

/**
 * Set the speed of the ball.
 */
void set_ball_speed(Ball* ball, float speed_x, float speed_y);

/**
 * Render the ball.
 */
void render_ball(Ball* ball);

#endif /* BALL_H */
