//
// Created by Vik_S on 12.07.2024.
//

#ifndef TETRIS_ARKANOID_H
#define TETRIS_ARKANOID_H

#include "drawable.h"

typedef struct ball
{
    point pos;
    int dir;
    char skin;
}ball;

void init_ball(ball *b,point pos,char skin);
void switch_ball_dir(ball *b,display *d);
void switch_ball_dir_if_surface(ball *b,display *d);

typedef struct platform{
    line ln;
    int length;
    char skin;
}platform;

void init_platform(platform *p,int length,point start,char skin);
void count_direction(ball *b,platform p);
void move_n_draw_platform(platform *p, display *d, int dir);
int check_ball_hit_platform(platform *p, ball *b);

typedef struct payload{
    shape *shapes;
    int num_of_shapes;
    char skin;
    int dir;
}payload;

void payload_delete_shape(payload *p,shape *shape1);
void move_payload(payload *p, display *d, int dir);
void draw_payload(payload p,display *d);
int if_hit_payload(payload *p,ball *b,display *d);

void shape_builder(shape *sh,point p,int type,char skin);
void arkanoid_init_payload(payload *p,int type_of_shape,int num_of_shapes,point *points,char skin);
void delete_payload(payload *p);
int ball_routine(ball *b,platform *ptfm,payload *p,display *d);
void payload_routine(payload *p,display *d);
int platform_routine(platform *plt,display *d);
void arkanoid_logo(display *d);

void draw_back(display *d);
int start_arkanoid(display *d);




#endif //TETRIS_ARKANOID_H
