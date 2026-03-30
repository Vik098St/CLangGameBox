//
// Created by Vik_S on 07.07.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include <time.h>

// ---------------- SNAKE---------------
void snake_add_tail(snake *s,display *d){
    tail *tmp;
    for(tmp=s->t; tmp->segment!=NULL; tmp=tmp->segment);

    tmp->segment= (tail *)malloc(sizeof(tail));

    new_pointfp(&tmp->segment->pos,tmp->pos);
    switch (tmp->dir) {
        case LEFT: if(can_move_point(tmp->pos,d,RIGHT)) move_point(&tmp->segment->pos,RIGHT); else tmp->segment->pos.x = 1;
            break;
        case RIGHT: if(can_move_point(tmp->pos,d,LEFT)) move_point(&tmp->segment->pos,LEFT); else tmp->segment->pos.x = (int)d->width-1;
            break;
        case UP: if(can_move_point(tmp->pos,d,DOWN)) move_point(&tmp->segment->pos,DOWN); else tmp->segment->pos.x= (int)d->length-1;
            break;
        case DOWN: if(can_move_point(tmp->pos,d,UP)) move_point(&tmp->segment->pos,UP); else tmp->segment->pos.x = 1;
            break;
        default:
            break;
    }
    tmp->segment->dir = tmp->dir;
    tmp->segment->segment=NULL;
    tmp->segment->skin=tmp->skin;

}

void init_snake(snake *s,point pos,int dir,char head_skin,char tail_skin){

    s->dir=dir;
    new_pointfp(&s->head_pos,pos); //snake_add_tail(s,tail_skin,d);
    s->t = (tail *)malloc(1*sizeof(tail));
    s->t->segment=NULL;
    s->t->dir=dir;
    new_pointvp(&s->t->pos,pos.x,pos.y-1);
    s->t->skin=tail_skin;
    s->skin=head_skin;
}


void move_snake(snake *s,display *d){
    int cur_dir,tmp_dir;
    point cur_p,tmp_p;

    cur_p = s->head_pos;
    cur_dir=s->dir;

    //TODO: сегменты змеи не доходят до правого края :
    if(!can_move_point(s->head_pos,d,s->dir)){
        switch (s->dir)
        {
            case UP:        s->head_pos.y=2;
                break;
            case LEFT:     s->head_pos.x = (int)d->width-1;
                break;
            case RIGHT:     s->head_pos.x = 2;
                break;
            case DOWN:      s->head_pos.y = (int)d->length-1;
                break;
            default:                 break;
        }
    } else move_point(&s->head_pos,s->dir);

    if(s->t){
        for (tail *tmp = s->t; tmp!=NULL; tmp=tmp->segment) {

            tmp_p=tmp->pos;
            new_pointfp(&tmp->pos,cur_p);
            cur_p=tmp_p;

            tmp_dir=tmp->dir;
            tmp->dir=cur_dir;
            cur_dir=tmp_dir;
        }
    }
}



// игрок клацнул кнопку -> появилось новое направление
// записываем это направление в массив направлений и присваиваем новое направление башке
// обновляем направления сегментов: сперва n-k+1 получают предыдущие направления,
// а затем добавляем новое направление и пересчитываем

int eat_an_apple(apples * a,snake *s){
    int i,ecode=0;
    if(a->positions)for( i=0 ; i<a->quantity;++i){
        if(is_same_point(a->positions[i],s->head_pos)){
            ecode=1;
            break;
        }
    }
    if(ecode){
        for (; i < a->quantity-1; ++i) {
            a->positions[i]=a->positions[i+1];
        }
        a->quantity-=1;
    }
    return ecode;
}

int is_byte_itself(snake *s){
    int ecode=0;
    for (tail * tmp = s->t; tmp->segment!=NULL; tmp=tmp->segment) {
        if(is_same_point(s->head_pos,tmp->pos)){
            ecode=1;
            break;
        }
    }
    return ecode;
}

int is_fill_screen(snake *s,display *d){
    int i = (((int)d->width-1)*((int)d->length-1))-1;
    for (tail *t=s->t; i > 0 && t!=NULL; t=t->segment,--i);
    return (i==0) ? 1 : 0;
}

void draw_snake(snake s,display *d){
    draw_point(s.head_pos,d,s.skin);

    for (tail *t= s.t;  t!=NULL ; t=t->segment) {
        draw_point(t->pos,d,t->skin);
    }
}

int snake_routine(snake *s,apples *a,display *d){

    if(GetKeyState(VK_UP)<0 && s->dir!=DOWN)s->dir=UP;
    if(GetKeyState(VK_DOWN)<0 && s->dir!=UP)s->dir=DOWN;
    if(GetKeyState(VK_LEFT)<0 && s->dir!=RIGHT)s->dir=LEFT;
    if(GetKeyState(VK_RIGHT)<0 && s->dir!=LEFT)s->dir=RIGHT;

    if(is_byte_itself(s))
    {
        snake_gameover(d);
        return 0;
    }

    if(eat_an_apple(a,s))snake_add_tail(s,d);
    move_snake(s,d);
    if(d->buf[s->head_pos.y-1][s->head_pos.x-1]=='~'){snake_gameover(d);         return 0;    }
    
    draw_snake(*s,d);
    if(is_fill_screen(s,d)){
        you_win_logo(d);
        return 0;
    }


    return 1;
}

void init_apples(apples *a,char skin){
 a->skin=skin;
 a->quantity=0;
 a->positions = (point *) calloc(10,sizeof(point));
}

void draw_apples(apples *a,display *d){
   if(a->positions)  for (int i = 0; i < a->quantity; ++i) {
           draw_point(a->positions[i], d, a->skin);
    }
}

int can_spawn_apple(point p,display d){
    if(p.x>0 && p.y>0 && p.x<d.width-1 && p.y<d.length-1
    && d.buf[p.y-1][p.x-1]!=WALL && d.buf[p.y-1][p.x-1]!=HEAD
     && d.buf[p.y-1][p.x-1]!=TAIL && d.buf[p.y-1][p.x-1]!=APPLE
     && d.buf[p.y-1][p.x-1]==SAND)return 1;
    return 0;
}

int apple_spawn(apples *a,display *d){

 srand(time(0)%(d->length+d->width));
 point p;

 p.y=rand()%(d->length-1)+1;
 p.x=rand()%(d->width-1)+1;

 if(can_spawn_apple(p,*d)){
     new_pointfp(&a->positions[a->quantity],p);
     a->quantity+=1;
 }
 return 0;
}


int start_snake(display *d){

    int ecode=0;
    snake s;
    apples a;
    time_t start_time=time(NULL), end_time;

    snake_logo(d);
    int apples_per_level_mount = 30;
    init_snake(&s,new_point((int)(d->width-3),(int)(d->length/2)),UP,HEAD,TAIL);
    init_apples(&a,APPLE);
    do{

        clear(d);
        snake_draw_back(d);

        if( !(ecode=snake_routine(&s,&a,d)))break;


        end_time = time(NULL);
        if(difftime(end_time, start_time)>5 && a.quantity<3){
            apple_spawn(&a,d);
            start_time = time(NULL);
            apples_per_level_mount-=1;
        }
        if(a.positions && a.quantity)draw_apples(&a,d);

        if(!apples_per_level_mount){ you_win_logo(d); break; }
        refresh(*d);
        Sleep(100);
    }while (GetKeyState(VK_ESCAPE)>=0);

    free(s.t);
    free(a.positions);
    return ecode;
}



void snake_logo(display *d){



    clear(d);
    draw_word( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",d,new_point((int)(d->width/2-42/2),(int)(d->length/2+3)));
    draw_word( "~~****~~~**~~**~~~****~~~**~~**~~******~",d,new_point((int)(d->width/2-42/2),(int)(d->length/2+2)));
    draw_word( "~**~~~~~~***~**~~**~~**~~**~**~~~**~~~~~",d,new_point((int)(d->width/2-42/2),(int)(d->length/2+1)));
    draw_word( "~~****~~~**~***~~******~~****~~~~****~~~",d,new_point((int)(d->width/2-42/2),(int)(d->length/2)));
    draw_word( "~~~~~**~~**~~**~~**~~**~~**~**~~~**~~~~~",d,new_point((int)(d->width/2-42/2),(int)(d->length/2-1)));
    draw_word( "~~****~~~**~~**~~**~~**~~**~~**~~******.",d,new_point((int)(d->width/2-42/2),(int)(d->length/2-2)));
    draw_word( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",d,new_point((int)(d->width/2-42/2),(int)(d->length/2-3)));
    refresh(*d);
    Sleep(1000);
}

void snake_draw_back(display *d){


    draw_word(                "~~~~~~~~~~~~~~~~~ _.-'**************************",d,new_point(2,19));
    draw_word(                "~~~~~~~~~~~.-'.-,'******************************",d,new_point(2,18));
    draw_word(                "~~~~~~~~~~~'-._********************** /\\***/\\***",d,new_point(2,17));
    draw_word(                "~~~~~~~~~~~~~~~'-.*******************/__\\*/__\\**",d,new_point(2,16));
    draw_word(                "~~~~~~~~~~~~~~~~~~'-._*****************/\\*******",d,new_point(2,15));
    draw_word(                "~~~~~~( 9 9 )~~~~~~~~~'-._,***********/__\\******",d,new_point(2,14));
    draw_word(                "~~~~~~~).!.(~~~~~~~~~~~_.-'*********************",d,new_point(2,13));
    draw_word(                "~~~~~~'/|||\\`~~~~~~_.-'*************************",d,new_point(2,12));
    draw_word(                "~~~~~~~~'|`~~~~ .-'***************.-'-'-._ *****",d,new_point(2,11));
    draw_word(                "~~~~~~~~~~~~~~_.'**********_.-'-.-'~~~~~~~'.****",d,new_point(2,10));
    draw_word(                "~~~~~.-'._.'-'**********.-'~~~~~~~~~~_ _.-'*****",d,new_point(2,9));
    draw_word(                "~ _.-'******************'-._,.-'._.-' **********",d,new_point(2,8));
    draw_word(                "~'**********************************************",d,new_point(2,7));
    draw_word(                ".'**********************************************",d,new_point(2,6));
    draw_word(                "'-.___.-'.**************************************",d,new_point(2,5));
    draw_word(                "~~~~~~~~~~'._***********************************",d,new_point(2,4));
    draw_word(                "~~~~~~~~~~~~~'_*********************************",d,new_point(2,3));
    draw_word(                "~~~~~~~~~~~~~~~:_*******************************",d,new_point(2,2));

}

void snake_gameover(display *d){

    draw_word(".._______........................",d,new_point((int)(d->width/2-34/2),(int)d->length/2+4));
    draw_word(".|     __|.___._.________._____..",d,new_point((int)(d->width/2-34/2),(int)d->length/2+3));
    draw_word(".|    |  ||  _  |        |  +__|.",d,new_point((int)(d->width/2-34/2),(int)d->length/2+2));
    draw_word(".|_______||___._|__|__|__|_____|.",d,new_point((int)(d->width/2-34/2),(int)d->length/2+1));
    draw_word("..........................__.....",d,new_point((int)(d->width/2-34/2),(int)d->length/2));
    draw_word(".._____.__.__._____.____.|  |....",d,new_point((int)(d->width/2-34/2),(int)d->length/2-1));
    draw_word(".|  _  |  |  |  +__|   _||__|....",d,new_point((int)(d->width/2-34/2),(int)d->length/2-2));
    draw_word(".|_____|\\___/|_____|__|..|__|....",d,new_point((int)(d->width/2-34/2),(int)d->length/2-3));
    draw_word(".................................",d,new_point((int)(d->width/2-34/2),(int)d->length/2-4));
    refresh(*d);
    Sleep(3000);

}


