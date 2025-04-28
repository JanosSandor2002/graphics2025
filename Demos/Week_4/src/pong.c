#include "pong.h"

void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, BLUE_THEME);  // Kék színséma alkalmazása
    init_ball(&(pong->ball), width / 2, height / 2);
    pong->left_score = 0;
    pong->right_score = 0;
}

void update_pong(Pong* pong, double time)
{
    update_ball(&(pong->ball), time);
    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    bounce_ball(pong);
}

void render_pong(Pong* pong)
{
    render_ball(&(pong->ball));
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
}

void bounce_ball(Pong* pong)
{
    if (pong->ball.x - pong->ball.radius < 50 && pong->ball.y > pong->left_pad.y && pong->ball.y < pong->left_pad.y + pong->left_pad.height) {
        pong->ball.x = pong->ball.radius + 50;
        pong->ball.speed_x *= -1;
    }
    if (pong->ball.x + pong->ball.radius > pong->width - 50 && pong->ball.y > pong->right_pad.y && pong->ball.y < pong->right_pad.y + pong->right_pad.height) {
        pong->ball.x = pong->width - pong->ball.radius - 50;
        pong->ball.speed_x *= -1;
    }
    if (pong->ball.y - pong->ball.radius < 0) {
        pong->ball.y = pong->ball.radius;
        pong->ball.speed_y *= -1;
    }
    if (pong->ball.y + pong->ball.radius > pong->height) {
        pong->ball.y = pong->height - pong->ball.radius;
        pong->ball.speed_y *= -1;
    }
}
