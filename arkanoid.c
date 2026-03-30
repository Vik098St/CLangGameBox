//
// Created by Vik_S on 12.07.2024.
//

#include "arkanoid.h"
enum shape_types{LINE_STRAIT=1,LINE_HORIZONTAL,LINE_DIAGONAL,RECTANGLE};

void init_ball(ball *b,point pos,char skin){
    new_pointfp(&b->pos,pos);
    b->dir=UP;
    b->skin=skin;
}

void init_platform(platform *p,int length,point start,char skin){
    p->length=length;
    // p->state=STAY;
    p->skin = skin;
    new_linevl(&p->ln,start,new_point(start.x+length,start.y));
}

void shape_builder(shape *sh,point p,int type,char skin){
    point * points;
    if(type==RECTANGLE){
        points= (point*)calloc(4,sizeof(point));
        if(sh && points){
            new_pointvp(&points[0],p.x,p.y);      //     **
            new_pointvp(&points[1],p.x+1,p.y); //     **
            new_pointvp(&points[2],p.x,p.y+1);
            new_pointvp(&points[3],p.x+1,p.y+1);
            new_shape(sh,new_rectangle(new_point(p.x,p.y),new_point(p.x+1,p.y+1)),4,points,skin);
        }
    } else{
        points= (point*)calloc(2,sizeof(point));
        if(sh && points){
            new_pointvp(&points[0],p.x,p.y);      //     **
            switch (type) {
                case LINE_STRAIT: new_pointvp(&points[1],p.x,p.y+1);
                    break;
                case LINE_HORIZONTAL: new_pointvp(&points[1],p.x+1,p.y);
                    break;
                case LINE_DIAGONAL: new_pointvp(&points[1],p.x+1,p.y+1);
                    break;
                default:
                    break;
            }
            new_shape(sh,new_rectangle(new_point(p.x,p.y),new_point(p.x+1,p.y+1)),2,points,skin);
        }
    }
}

void arkanoid_init_payload(payload *p,int type_of_shape,int num_of_shapes,point *points,char skin){

    if(points && (points+num_of_shapes-1)){
        p->num_of_shapes=num_of_shapes;
        p->dir=STAY;
        p->skin=skin;
        p->shapes = (shape*) calloc(p->num_of_shapes,sizeof(shape));
        for (int i = 0; i < p->num_of_shapes; ++i) {
            shape_builder(&p->shapes[i],points[i],type_of_shape,p->skin);
        }
    }
}

void delete_payload(payload *p){
    p->num_of_shapes=0;
    free(p->shapes);
    free(p);
}

void move_n_draw_platform(platform *p, display *d, int dir){
    if(can_move_line(p->ln,d,dir)){
        clear_line(p->ln, d);

        move_line(&p->ln,dir);
        draw_line(p->ln,d,p->skin);
    }
}

int can_move_payload(payload p,display d,int dir){
    int ecode=0;
    for (int i = 0; i < p.num_of_shapes; ++i) {
        ecode+=can_move_shape(p.shapes[i],&d,dir);
    }
    return (ecode ==p.num_of_shapes) ? 1 : 0;
}

/// Проблемы с отрисовкой при плотном расположении фигур, возможно дело в "модульности" нагрузки,
/// пока одна фигура очищается и отрисовывается -  другая это же место очищает
int move_n_draw_payload(payload *p,display *d,int dir){

    if(!p || !p->shapes || !(p->shapes+(p->num_of_shapes)-1))return 0;

    if(can_move_payload(*p,*d,dir)){
        for (int j = 0; j < p->num_of_shapes; ++j) {
            clear_shape(&p->shapes[j],d);
            move_shape(&p->shapes[j],dir);
            draw_shape(&p->shapes[j],d);
        }
        return 1;
    }

    return 0;
}

void switch_ball_dir(ball *b,display *d){
    switch (b->dir)
    {
        case UL:       if(!can_move_point(b->pos,d,UL)){
                            if(can_move_point(b->pos,d,UR)) b->dir = UR;
                            else if(can_move_point(b->pos,d,DL)) b->dir = DL ;
                            else b->dir=DR;
                       }
            break;
        case UP:        if(!can_move_point(b->pos,d,UP))b->dir = DOWN;
            break;
        case UR:      if(!can_move_point(b->pos,d,UR)){
                        if(can_move_point(b->pos,d,UL)) b->dir = UL;
                        else if(can_move_point(b->pos,d,DR)) b->dir = DR ;
                        else b->dir=DL;
                      }
            break;
        case LEFT:      if(!can_move_point(b->pos,d,LEFT))b->dir = DR;
            break;
        case RIGHT:     if(!can_move_point(b->pos,d,RIGHT))b->dir = DL;
            break;
        case DL:       if(!can_move_point(b->pos,d,DL)) b->dir =(can_move_point(b->pos,d,DR)) ?  DR : DOWN;
            break;
        case DOWN:      if(!can_move_point(b->pos,d,DOWN))b->dir = STAY;
          break;
        case DR:        if(!can_move_point(b->pos,d,DR))b->dir =(can_move_point(b->pos,d,DL)) ?  DL : DOWN;
            break;
        default:
            break;
    }
}

//TODO: протестириовать изменение траектории мяча
void switch_ball_dir_if_surface(ball *b,display *d){
    //srand(rand()%10);
    switch (b->dir)
    {
        case UL:        b->dir = can_move_point(b->pos,d,UR) ? UR : DL;
            break;
        case UP:        b->dir = DOWN;
            break;
        case UR:        b->dir = can_move_point(b->pos,d,UL) ? UL : DR;
            break;
        case LEFT:      b->dir = (can_move_point(b->pos,d,RIGHT)) ? RIGHT :DOWN;
            break;
        case RIGHT:     b->dir =(can_move_point(b->pos,d,LEFT)) ? LEFT : DOWN;
            break;
        case DL:        b->dir =(can_move_point(b->pos,d,DR)) ? b->dir = DR : DOWN;
            break;
        case DOWN:      b->dir = UP;
            break;
        case DR:        b->dir = (can_move_point(b->pos,d,DL)) ?  DL : DOWN;
            break;
        default:
            break;
    }
}

void count_direction(ball *b,platform p){
    if(is_point_in_line(b->pos,new_line(new_point(p.ln.a.x,p.ln.a.y+1),new_point(p.ln.a.x+(p.length/3)+1,p.ln.a.y+1)))){
        b->dir=UL;
    }else if(is_point_in_line(b->pos,new_line(new_point(p.ln.b.x-(p.length/3)-1,p.ln.b.y+1),new_point(p.ln.b.x,p.ln.b.y+1)))){
        b->dir=UR;
    }else b->dir=UP;
}

//TODO:  узнать куда пропадает фигура со временем
int if_hit_payload(payload *p,ball *b,display *d){
    int i=0,ecode=0;
    static shape *tmp;
    if(!p->shapes || p->num_of_shapes==0)return -1;

    for (; i < p->num_of_shapes; ++i) {
        /// Теперь важно не выхвать отрисовку нагрузки ранее этого пересчёта
        //TODO: аккуратно убирать первую и последню фигуры
        if((ecode=is_point_in_shape(p->shapes[i],b->pos))>0)break;
    }
    if(ecode){
        clear_shape(&p->shapes[i],d);
        delete_shape(&p->shapes[i]);
        if(i==p->num_of_shapes-1) {
            p->num_of_shapes -= 1;
            delete_shape(&p->shapes[i]);
        }else if(i==0){
            p->shapes[i] = p->shapes[p->num_of_shapes-1];
            p->num_of_shapes -= 1;
        }
        else {
            for (; i < p->num_of_shapes-1; ++i) {
                p->shapes[i] = p->shapes[i+1];
            }
            p->num_of_shapes -= 1;
        }
        switch_ball_dir_if_surface(b,d);
        p->skin='$';
    }
    return ecode;
}

int check_ball_hit_platform(platform *p, ball *b){
    return is_point_in_line(b->pos,new_line(new_point(p->ln.a.x,p->ln.a.y+1),new_point(p->ln.b.x,p->ln.b.y+1)));
}

int ball_routine(ball *b,platform *ptfm,payload *p,display *d){

    clear_point(b->pos,d);
    if(b->dir==DOWN && !can_move_point(b->pos,d,b->dir)){
        gameover_logo(d);
        return 0;
    }
    else if(can_move_point(b->pos,d,b->dir)){
        if(check_ball_hit_platform(ptfm,b))count_direction(b,*ptfm);
        if(if_hit_payload(p,b,d)<0){
            you_win_logo(d);
            delete_payload(p);
            return 0;
        }
        move_point(&b->pos,b->dir);
        draw_point(b->pos,d,b->skin);
    }else{
        switch_ball_dir(b,d);
        move_point(&b->pos,b->dir);
        draw_point(b->pos,d,b->skin);
    }
    return 1;
}


void payload_routine(payload *p,display *d){
   static int dir = RIGHT;
    if(p->shapes && p->num_of_shapes){
        if(!move_n_draw_payload(p,d,dir))
        {
            dir=(dir==LEFT) ? RIGHT:LEFT;
            move_n_draw_payload(p,d,dir);
        }
    }

}

//TODO: добавить возможность схватить мячь и запускать по нажатию на "W"
int platform_routine(platform *plt,display *d){
#ifdef WIN32
    int pressed_button = 0;
    if(GetKeyState(VK_LEFT)<0)move_n_draw_platform(plt,d,LEFT);
    else if(GetKeyState(VK_RIGHT)<0)move_n_draw_platform(plt,d,RIGHT);
    else move_n_draw_platform(plt,d,STAY);
#else
    pressed_button = getchar();
    switch (pressed_button) {
        case KEY_RIGHT: move_n_draw_platform(plt,d,RIGHT);
            break;
        case KEY_LEFT: move_n_draw_platform(plt,d,LEFT);
            break;
        default: move_n_draw_platform(plt,d,STAY);
            break;
    }
#endif
    return pressed_button;
}

///====================
point * shit_maker(){
    point * points;
    points= (point*)calloc(17,sizeof(point));
    new_pointvp(&points[0],14,10);         new_pointvp(&points[1],14,12);
    new_pointvp(&points[2],16,11);         new_pointvp(&points[3],18,10);
    new_pointvp(&points[4],18,12);

    new_pointvp(&points[5],21,10);         new_pointvp(&points[6],21,12);
    new_pointvp(&points[7],22,11);         new_pointvp(&points[8],24,12);

    new_pointvp(&points[9],27,10);          new_pointvp(&points[10],27,11);
    new_pointvp(&points[11],27,12);         new_pointvp(&points[12],30,10);
    new_pointvp(&points[13],30,13);         new_pointvp(&points[14],33,10);
    new_pointvp(&points[15],33,11);         new_pointvp(&points[16],33,12);
    return points;
}

///====================
int start_arkanoid(display *d){
    int ecode;
    ball Ball;
    platform plat;
    payload payload1;

    arkanoid_logo(d);
    //TODO: адекватно инициировать нагрузку
    arkanoid_init_payload(&payload1,LINE_HORIZONTAL,17,shit_maker(),'?');
    init_ball(&Ball,new_point((int)(d->width/2),5),'@');
    init_platform(&plat,15,new_point((int)(d->width/2-10/2),4),'=');

    do{
        clear(d);
        draw_back(d);
        //TODO: тестируем поведение нагрузки, НЕ ТРОГАТЬ!
        move_n_draw_payload(&payload1,d,STAY);//payload_routine(&payload1,d);
        ecode=platform_routine(&plat,d);

        if( !(ball_routine(&Ball,&plat,&payload1,d)))break;
        refresh(*d);
        Sleep(100);
    }while (GetKeyState(VK_ESCAPE)>=0);

    delete_payload(&payload1);
    return ecode;
}

/// ===================   LOGO PART =========================

void draw_back(display *d){
    draw_word(       "        *   .                  .          .     ",d,new_point(2,19));
     draw_word(      "                  .       .      .        *    .",d,new_point(2,18));
     draw_word(      "  .       .    *       .    .      *          . ",d,new_point(2,17));
     draw_word(      "          .       .  *   /\\  .    .             ",d,new_point(2,16));
     draw_word(      "              . *      /  \\  . .       *     .  ",d,new_point(2,15));
     draw_word(      "     .           .     /    \\    . .            ",d,new_point(2,14));
     draw_word(      "  *        *  .  .   /      \\  .     *          ",d,new_point(2,13));
     draw_word(      "     .      .  .     /        \\        .    .   ",d,new_point(2,12));
     draw_word(      "  .       *        /__________\\     .           ",d,new_point(2,11));
     draw_word(      "       .       .    \\        /         .        ",d,new_point(2,10));
     draw_word(      "          .     .    \\      / .   .             ",d,new_point(2,9));
     draw_word(      "   .            .    \\    /        .        *   ",d,new_point(2,8));
     draw_word(      "        *     .        \\  /  .        .    .    ",d,new_point(2,7));
     draw_word(      "  .  .    .      *     \\/       .  *            ",d,new_point(2,6));
     draw_word(      "           .        .    .     .     .      .   ",d,new_point(2,5));
     draw_word(      "                 .        *     .    .    *     ",d,new_point(2,4));
     draw_word(      "      . .       *   .  .    .  *      .  .      ",d,new_point(2,4));
     draw_word(      "                 .       .      .  .            ",d,new_point(2,3));
     draw_word(      "    .     *            .     *   .          .   ",d,new_point(2,2));
}

void arkanoid_logo(display *d){
    clear(d);
    draw_word( "    ___       ___       ___       ___   ",d,new_point((int)(d->width/2-40/2),(int)d->length-2));
    draw_word( "   /\\  \\     /\\  \\     /\\__\\     /\\  \\  ",d,new_point((int)(d->width/2-40/2),(int)d->length-3));
    draw_word( "  /::\\  \\   /::\\  \\   /:/ _/_   /::\\  \\ ",d,new_point((int)(d->width/2-40/2),(int)d->length-4));
    draw_word( " /::\\:\\__\\ /::\\:\\__\\ /::-\"\\__\\ /::\\:\\__\\",d,new_point((int)(d->width/2-40/2),(int)d->length-5));
    draw_word( " \\/\\::/  / \\;:::/  / \\;:;-\",-\" \\/\\::/  /",d,new_point((int)(d->width/2-40/2),(int)d->length-6));
    draw_word( "   /:/  /   |:\\/__/   |:|  |     /:/  / ",d,new_point((int)(d->width/2-40/2),(int)d->length-7));
    draw_word( "   \\/__/     \\|__|     \\|__|     \\/__/  ",d,new_point((int)(d->width/2-40/2),(int)d->length-8));

    draw_word( "    ___       ___       ___       ___   ",d,new_point((int)(d->width/2-40/2),(int)d->length-9));
    draw_word( "   /\\__\\     /\\  \\     /\\  \\     /\\  \\  ",d,new_point((int)(d->width/2-40/2),(int)d->length-10));
    draw_word( "  /:| _|_   /::\\  \\   _\\:\\  \\   /::\\  \\ ",d,new_point((int)(d->width/2-40/2),(int)d->length-11));
    draw_word( " /::|/\\__\\ /:/\\:\\__\\ /\\/::\\__\\ /:/\\:\\__\\",d,new_point((int)(d->width/2-40/2),(int)d->length-12));
    draw_word( " \\/|::/  / \\:\\/:/  / \\::/\\/__/ \\:\\/:/  /",d,new_point((int)(d->width/2-40/2),(int)d->length-13));
    draw_word( "   |:/  /   \\::/  /   \\:\\__\\    \\::/  / ",d,new_point((int)(d->width/2-40/2),(int)d->length-14));
    draw_word( "   \\/__/     \\/__/     \\/__/     \\/__/  ",d,new_point((int)(d->width/2-40/2),(int)d->length-15));

    refresh(*d);
    Sleep(1000);
}
