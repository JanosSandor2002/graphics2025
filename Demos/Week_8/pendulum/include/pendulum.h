#ifndef PENDULUM_H
#define PENDULUM_H

typedef struct {
    double angle;
    double time;
} Pendulum;

void init_pendulum(Pendulum* p);
void update_pendulum(Pendulum* p, double delta_time);
void draw_pendulum(const Pendulum* p);

#endif
