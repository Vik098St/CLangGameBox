//
// Created by Vik_S on 10.07.2024.
//

#ifndef TETRIS_DRAWABLE_H
#define TETRIS_DRAWABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctype.h"


#ifdef WIN32
#include <windows.h>

#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
void Sleep(int milliseconds){ // cross-platform sleep function
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
#else
#include <unistd.h> // for usleep
#endif

void cls();
void setcur(int x,int y);

#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

#define ANSI_BACK_BLACK   "\x1b[40m"
#define ANSI_BACK_RED     "\x1b[41m"
#define ANSI_BACK_GREEN   "\x1b[42m"
#define ANSI_BACK_YELLOW  "\x1b[43m"
#define ANSI_BACK_BLUE    "\x1b[44m"
#define ANSI_BACK_MAGENTA "\x1b[45m"
#define ANSI_BACK_CYAN    "\x1b[46m"
#define ANSI_BACK_WHITE   "\x1b[47m"

#define ANSI_COLOR_RESET   "\x1b[0m"

enum keyboard{KEY_UP='W',KEY_LEFT='A',KEY_RIGHT='D',KEY_DOWN='S',ENTER=32,ESC='Q'};
enum direction{UL,UP,UR,LEFT,STAY,RIGHT,DL,DOWN,DR};
typedef enum bool{F,T} Bool;

typedef struct display
{
    size_t length;
    size_t width;
    char ** buf;
}display;

void init_display( display *d,int length,int width);
void clear(display *d);
void refresh(display d);

typedef struct point
{
    int x;
    int y;
}point;

point new_point(int x,int y);
void new_pointvp(point *p,int x,int y);
void new_pointfp(point *newp,point oldp);
int is_same_point(point a,point b);
void move_point(point *p,int dir);
int can_move_point(point p,display *d,int dir);
int draw_point(point p,display *d,char skin);
int clear_point(point p,display *d);


typedef struct line{
    point a;
    point b;
} line;

line new_line(point a, point b);
void new_linevl(line *l,point a,point b);
void new_linefl(line *newl,line oldl);
int can_move_line(line l,display *d,int dir);
void move_line(line *l,int dir);
int draw_line(line l,display *d,char skin);
int clear_line(line l,display *d);
int is_point_in_line(point p,line l);

typedef struct rectangle
{            //    _____ b
    point a; //   |     |
    point b; //   |     |
}rectangle;  // a |_____|

rectangle new_rectangle(point a, point b);
void new_rectanglevr(rectangle *r,point a,point b);
void new_rectanglefr(rectangle *newr,rectangle oldr);
int can_move_rectangle(rectangle r,display *d,int dir);
void move_rectangle(rectangle *r,int dir);
int draw_rectangle(rectangle rec,display *d,char skin);
int clear_rectangle(rectangle rec,display *d);
int is_point_in_rect(point p,rectangle r);

typedef struct shape
{
    rectangle boarders;
    point *shape_pos;
    int p_num;
    char skin;
}shape;

void new_shape(shape *obj,rectangle boarders,int p_num, point *points,char skin);
int can_move_shape(shape sh,display *d,int dir);
void move_shape(shape *obj,int dir);
void draw_shape(shape *obj,display *d);
void clear_shape(shape *obj,display *d);
int is_point_in_shape(shape obj, point p);
void point_breacks_shape(shape *obj, point p);
void delete_shape(shape *obj);

int str_is_upper(const char * str);
void str_to_upper(char *str);
int str_is_lower(const char * str);
void str_to_lower(char *str);

void draw_word(char * word,display *d,point start);
int draw_logo_literal(char ,display *d);
void draw_logo(char  *,display *d);
void you_win_logo(display *d);
void gameover_logo(display *d);
int is_free_space(point p,display d);

void draw_meme(int which_one);
#endif //TETRIS_DRAWABLE_H
