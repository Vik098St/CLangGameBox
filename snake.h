//
// Created by Vik_S on 07.07.2024.
//

#ifndef TETRIS_SNAKE_H
#define TETRIS_SNAKE_H

#include "drawable.h"


enum snake_game_skin{WALL='#',TAIL='0',HEAD='@',APPLE='$',SAND='*'};



typedef struct tail{
    struct tail * segment;
    point pos;
    int dir;
    char skin;
} tail;

typedef struct snake{
    tail * t;
    point head_pos;
    int dir;
    char skin;
} snake;

void init_snake(snake *s,point pos,int dir,char head,char tail)
;

void move_snake(snake *s,display *d);
void draw_snake(snake s,display *d);
//void delete_snake(snake *s){ free(s->t);}

typedef struct apples{
    point *positions;
    int quantity;
    char skin;

} apples;

void init_apples(apples *a,char skin);
//void delete_apples(apples *a){    free(a->positions);}
int eat_an_apple(apples * a,snake *s);

int is_byte_itself(snake *s);
int is_fill_screen(snake *s,display *d);


void snake_draw_back(display *d);
void snake_logo(display *d);
void snake_gameover(display *d);

/*
 * Eternal cycle
 * player input is an interruption that changes snake's head destination
 * if snake eats apple -> snake + 1
 * if snake intersect field or self -> game over
 * */
int snake_routine(snake *s,apples *a,display *d);
int start_snake(display *d);


#endif //TETRIS_SNAKE_H
