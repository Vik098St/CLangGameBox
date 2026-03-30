//
// Created by Vik_S on 10.07.2024.
//
#include "math.h"
#include "drawable.h"


void cls()
{
#ifdef WIN32
    HANDLE hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                    (TCHAR)' ',      // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(hConsole, coordScreen);
#endif
}

void setcur(int x,int y){
#ifdef WIN32
    COORD coret;
    coret.X=x;
    coret.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coret);
#endif
}

////======== DISPLAY ========
void init_display( display *d,int length,int width){
    d->length=length;
    d->width=width;
    d->buf = (char**) malloc( d->length*sizeof(char*));
    for (size_t i = 0; i <  d->length; i++)
    {
        d->buf[i] = (char*) malloc(d->width*sizeof(char));
    }


    for (size_t i = 0; i < d->length; i++)
    {
        for (size_t j = 0; j < d->width; j++)
        {
            d->buf[i][j]='-';
        }
    }


    for (size_t i = 0; i < d->width; i++)
    {
        d->buf[0][i] = '#';
        d->buf[ d->length-1][i]= '#';
    }

    for (size_t i = 0; i < d->length; i++)
    {
        d->buf[i][0] = '#';
        d->buf[i][d->width-1]= '#';
    }
}

void clear(display *d){
    for (size_t i = 0; i < d->length; i++)
    {
        for (size_t j = 0; j < d->width; j++)
        {
            d->buf[i][j]='-';
        }
    }

    for (size_t i = 0; i < d->width; i++)
    {
        d->buf[0][i] = '#';
        d->buf[ d->length-1][i]= '#';
    }

    for (size_t i = 0; i < d->length; i++)
    {
        d->buf[i][0] = '#';
        d->buf[i][d->width-1]= '#';
    }
}

void refresh(display d){
#if WIN32
    setcur(0,0);
    //cls();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    for (int i = d.length-1; i > -1; i--)
    {
        for (int j = 0; j < d.width; j++)
        {
            switch (d.buf[i][j])
            {
                case ' ':            SetConsoleTextAttribute(hConsole, saved_attributes);
                    break;
                case '#':         SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+FOREGROUND_BLUE+4);
                    break;
                case '~':
                case '-':            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY+FOREGROUND_BLUE);
                    break;
                case '*':           SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+FOREGROUND_BLUE+5);
                    break;
                case '?':        SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+FOREGROUND_BLUE+2);
                    break;
                case '$':           SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+ FOREGROUND_GREEN);
                    break;
                case '@':
                case '0':            SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+ FOREGROUND_RED);
                    break;
                case '=':            SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+ FOREGROUND_GREEN);
                    break;
                default:          if(isupper(d.buf[i][j]) && isalpha(d.buf[i][j]))SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+ FOREGROUND_GREEN+5+ BACKGROUND_RED);
                                  else if (isalpha(d.buf[i][j]))SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+ FOREGROUND_GREEN+5 + BACKGROUND_BLUE);
                    else SetConsoleTextAttribute(hConsole,FOREGROUND_INTENSITY+ FOREGROUND_GREEN+5);
                    break;
            }
            printf("%c",d.buf[i][j]);
        }
        printf("\n");
        SetConsoleTextAttribute(hConsole, saved_attributes);
    }
#else
    clrscr();
    //printf("\e[1;1H\e[2J");
    for (int i = d.length-1; i > -1; i--)
    {
        for (int j = 0; j < d.width; j++)
        {
            switch (d.buf[i][j])
            {
                case ' ':            printf(ANSI_COLOR_GREEN  " " ANSI_COLOR_RESET );
                    break;
                case '#':            printf(ANSI_COLOR_MAGENTA "#" ANSI_COLOR_RESET );
                    break;
                case '-':            printf(ANSI_COLOR_BLUE "-" ANSI_COLOR_RESET );
                    break;
                case '*':            printf(ANSI_COLOR_YELLOW "*" ANSI_COLOR_RESET);
                    break;
                case '?':            printf(ANSI_COLOR_CYAN "?" ANSI_COLOR_RESET);
                    break;
                case '$':            printf(ANSI_COLOR_GREEN  "$" ANSI_COLOR_RESET);
                    break;
                case '@':            printf(ANSI_COLOR_RED "@" ANSI_COLOR_RESET);
                    break;
                default:          if(isupper(d.buf[i][j]) && isalpha(d.buf[i][j]))printf(ANSI_COLOR_BLACK ANSI_BACK_RED "%c" ANSI_COLOR_RESET, d.buf[i][j] );
                                  else if (isalpha(d.buf[i][j])) printf(ANSI_COLOR_BLACK ANSI_BACK_MAGENTA "%c" ANSI_COLOR_RESET, d.buf[i][j] );
                                  else printf(ANSI_COLOR_WHITE "%c" ANSI_COLOR_RESET, d.buf[i][j] );
                    break;
            }
            //printf("%c",d.buf[i][j]);
        }
        printf("\n");
    }
    printf( "Available colors:" );
    printf(ANSI_COLOR_WHITE  ANSI_BACK_BLACK   " WHITE"     ANSI_COLOR_RESET );
    printf(ANSI_COLOR_RED   ANSI_BACK_BLUE   " RED"     ANSI_COLOR_RESET );
    printf(ANSI_COLOR_GREEN  ANSI_BACK_MAGENTA " GREEN"   ANSI_COLOR_RESET );
    printf(ANSI_COLOR_YELLOW  ANSI_BACK_CYAN " YELLOW"  ANSI_COLOR_RESET );
    printf(ANSI_COLOR_BLUE    ANSI_BACK_RED " BLUE"    ANSI_COLOR_RESET );
    printf(ANSI_COLOR_MAGENTA ANSI_BACK_GREEN" MAGENTA" ANSI_COLOR_RESET );
    printf(ANSI_COLOR_CYAN  ANSI_BACK_WHITE  " CYAN"    ANSI_COLOR_RESET );
    printf(ANSI_COLOR_BLACK  ANSI_BACK_MAGENTA  " BLACK"    ANSI_COLOR_RESET "\n");
#endif
}

////======== OPERATIONS WITH POINT ========

point new_point(int x,int y){
    point p;
    p.x=x,p.y=y;
    return p;
}

void new_pointvp(point *p,int x,int y){    p->x=x,p->y=y;}
void new_pointfp(point *newp,point oldp){    newp->x=oldp.x, newp->y=oldp.y;}

void move_point(point *p,int dir){
    switch (dir)
    {
        case UL:        p->x-=1,p->y+=1;
            break;
        case UP:        p->y+=1;
            break;
        case UR:        p->x+=1,p->y+=1;
            break;
        case LEFT:      p->x-=1;
            break;
        case RIGHT:     p->x+=1;
            break;
        case DL:        p->x-=1,p->y-=1;
            break;
        case DOWN:      p->y-=1;
            break;
        case DR:        p->x+=1,p->y-=1;
            break;
        default:        break;
    }
}

int can_move_point(point p,display *d,int dir){
    switch (dir)
    {
        case UL:        if(p.x-1>1 && p.y+1<d->length)return 1;
            break;
        case UP:        if(p.y+1<d->length)return 1;
            break;
        case UR:        if(p.x+1<d->width && p.y+1<d->length)return 1;
            break;
        case LEFT:      if(p.x-1>1)return 1;
            break;
        case STAY:      return 1;
            break;
        case RIGHT:     if(p.x+1 < d->width-1)return 1;
            break;
        case DL:        if(p.x-1>1 && p.y-1>1)return 1;
            break;
        case DOWN:      if(p.y-1>1)return 1;
            break;
        case DR:        if(p.x+1<d->width-1 && p.y-1>1)return 1;
            break;
        default:        break;
    }
    return 0;
}

int is_free_space(point p,display d){
    if(p.x>0 && p.y>0 && p.x<d.width-1 && p.y<d.length-1 && d.buf[p.y-1][p.x-1]=='-')return 1;
    return 0;
}

int draw_point(point p,display *d,char skin)
{
    if((d->buf) && *(d->buf) && (p.x>0) && (p.y>0)
        && (p.x-1<d->width-1) && (p.y-1<d->length-1))
        d->buf[p.y-1][p.x-1]=skin;
    else return -1;
    return 0;
}
int clear_point(point p,display *d){
    if((d->buf) && *(d->buf)  && (p.x>0) && (p.y>0)
             && (p.x-1<d->width-1) && (p.y-1<d->length-1))
        d->buf[p.y-1][p.x-1]='-';
    else return -1;
    return 0;
}

int is_same_point(point a,point b){
    return (a.x==b.x)&&(a.y==b.y);
}


////======== OPERATIONS WITH LINE ========

line new_line(point a,point b){
    line l;

    new_pointvp(&l.a,a.x,a.y);
    new_pointvp(&l.b,b.x,b.y);
    return l;
}

void new_linefl(line *newl,line oldl){ new_pointfp(&newl->a,oldl.a),new_pointfp(&newl->b,oldl.b);}

void new_linevl(line *l,point a,point b){ new_pointfp(&l->a,a),new_pointfp(&l->b,b);}

int can_move_line(line l,display *d,int dir){
    if(can_move_point(l.a,d,dir)&&
       can_move_point(l.b,d,dir))return 1;
    return 0;
}

void move_line(line *l,int dir){
    move_point(&(l->a),dir);
    move_point(&(l->b),dir);
}

int is_point_in_line(point p,line l){
    int ecode=0;
    if(!(l.b.x-l.a.x) && p.x == l.a.x) {
        if((l.b.y-l.a.y)>=0) ecode= (l.b.y>p.y)&&(p.y>l.a.y) ? 1: 0;
        else  ecode= (l.a.y>p.y)&&(p.y>l.b.y) ? 1: 0;
    }
    else if(!(l.b.y-l.a.y)  && p.y == l.a.y){
        if((l.b.x-l.a.x)>=0) ecode= (l.b.x>p.x)&&(p.x>l.a.x) ? 1: 0;
        else  ecode= (l.a.x>p.x)&&(p.x>l.b.x) ? 1: 0;
    }else {
        for ( int y = 0,x= (l.b.x-l.a.x)<0 ? l.b.x : l.a.x; x <= ((l.b.x-l.a.x)<0 ? l.a.x : l.b.x); ++x) {
            y = x* (int)round((l.b.y-l.a.y)/(l.b.x-l.a.x)) - (int)round((l.b.y-l.a.y)/(l.b.x-l.a.x))*l.a.x + l.a.y;
            ecode =is_same_point(p,new_point(x,y));
        }
    }
    return ecode;
}



/*
 * x-a.x       y-a.y
 * ------  == -------  ==> y = ((x-a.x)(b.y-a.y)-a.y)/(b.x-a.x)
 * b.x-a.x    b.y-a.y
 * */
int draw_line(line l,display *d,char skin){
    signed int err=0;
    if((l.b.x-l.a.x)==0){
        for ( int y = l.a.y; y <= l.b.y; y+= ((l.b.y-l.a.y)<=0) ? -1 : 1) {
            err = draw_point(new_point(l.a.x,y),d,skin);
        }
    }else {
        for ( int y = 0,x= (l.b.x-l.a.x)<0 ? l.b.x : l.a.x; x <= ((l.b.x-l.a.x)<0 ? l.a.x : l.b.x); ++x) {
            y = x* (int)round((l.b.y-l.a.y)/(l.b.x-l.a.x)) - (int)round((l.b.y-l.a.y)/(l.b.x-l.a.x))*l.a.x + l.a.y;
            //for (; tmp-y>=0; ++y) err = draw_point(new_point(x,y),d,skin);
            err = draw_point(new_point(x,y),d,skin);
        }
    }
    return err;
}

int clear_line(line l,display *d){
    signed int err=0;
    if((l.b.x-l.a.x)==0){
        for ( int y = l.a.y; y <= l.b.y; y+= ((l.b.y-l.a.y)<=0) ? -1 : 1) {
            err = clear_point(new_point(l.a.x,y),d);
        }
    }else {
        for ( int y = 0,x= (l.b.x-l.a.x)<0 ? l.b.x : l.a.x; x <= ((l.b.x-l.a.x)<0 ? l.a.x : l.b.x); ++x) {
            y = x* (int)ceil((l.b.y-l.a.y)/(l.b.x-l.a.x)) - (int)ceil((l.b.y-l.a.y)/(l.b.x-l.a.x))*l.a.x + l.a.y;
            //for (; tmp-y>=0; ++y) err = draw_point(new_point(x,y),d,skin);
            err = clear_point(new_point(x,y),d);
        }
    }
    return err;
}

////======== OPERATIONS WITH RECTANGLE ========

rectangle new_rectangle(point a, point b){
    rectangle r;
    r.a = a;
    r.b=b;
    return r;
}
void new_rectanglefr(rectangle *newr,rectangle oldr){   new_pointfp(&(newr->a),oldr.a),new_pointfp(&(newr->b),oldr.b);}

void new_rectanglevr(rectangle *r,point a,point b){
    new_pointfp(&(r->a),a);
    new_pointfp(&(r->b),b);
}


int can_move_rectangle(rectangle r,display *d,int dir){
    if(can_move_point(r.a,d,dir)&&
       can_move_point(r.b,d,dir)&&
       can_move_point(new_point(r.a.x,r.b.y),d,dir)&&
       can_move_point(new_point(r.b.x,r.a.y),d,dir))return 1;
    return 0;
}

void move_rectangle(rectangle *r,int dir){
    move_point(&(r->a),dir);
    move_point(&(r->b),dir);
}

/*  xa,yb *------* xb,yb
 *        |      |
 *        |      |
 *  xa,ya *------* xb,ya
 * */
int draw_rectangle(rectangle rec,display *d,char skin){
    int err=0;
    if((rec.a.x-rec.b.x)!=0 && (rec.a.y - rec.b.y)!=0){
        err=draw_line(new_line(rec.a,new_point(rec.a.x,rec.b.y)),d,skin);
        if(!err)err=draw_line(new_line(new_point(rec.a.x,rec.b.y),rec.b),d,skin);
        if(!err)err=draw_line(new_line(new_point(rec.b.x,rec.a.y),rec.b),d,skin);
        if(!err)err=draw_line(new_line(rec.a,new_point(rec.b.x,rec.a.y)),d,skin);
    }else{
        err=draw_line(new_line(rec.a,rec.b),d,skin);
    }
    return err;
}

int clear_rectangle(rectangle rec,display *d){
    int err=0;
    if((rec.a.x-rec.b.x)!=0 && (rec.a.y - rec.b.y)!=0){
        err=clear_line(new_line(rec.a,new_point(rec.a.x,rec.b.y)),d);
        if(!err)err=clear_line(new_line(new_point(rec.a.x,rec.b.y),rec.b),d);
        if(!err)err=clear_line(new_line(new_point(rec.b.x,rec.a.y),rec.b),d);
        if(!err)err=clear_line(new_line(rec.a,new_point(rec.b.x,rec.a.y)),d);
    }else{
        err=clear_line(new_line(rec.a,rec.b),d);
    }
    return err;
}

int is_point_in_rect(point p,rectangle r){
    if((r.a.x-r.b.x)!=0 && (r.a.y - r.b.y)!=0) return ((p.x - r.a.x)>=0&&(p.y-r.a.y)>=0&&(r.b.x -p.x)>=0&&(r.b.y-p.y)>=0);
    return is_point_in_line(p,new_line(r.a,r.b));
}

////======== OPERATIONS WITH SHAPE ========

void new_shape(shape *obj,rectangle boarders,int p_num, point *points,char skin){

    new_rectanglefr(&obj->boarders,boarders);
    obj->skin = skin;
    obj->p_num=p_num;

    obj->shape_pos= (point*)calloc(obj->p_num,sizeof(point));

    for (int i = 0; i < obj->p_num; ++i) {
        if(points+i!=NULL) new_pointfp(&obj->shape_pos[i],points[i]);
    }
    free(points);
}

int can_move_shape(shape sh,display *d,int dir){
    if(can_move_rectangle(sh.boarders,d,dir))return 1;
    return 0;
}

void move_shape(shape *obj,int dir){
    if((obj->shape_pos)!=NULL){
        for (point *iter=obj->shape_pos; iter< obj->shape_pos+obj->p_num; iter++)move_point(iter,dir);
        move_rectangle(&(obj->boarders),dir);
    }
}

void draw_shape(shape *obj,display *d){
    if((obj->shape_pos)!=NULL){
        for (point *iter = obj->shape_pos;iter< obj->shape_pos+obj->p_num; iter++)
        {
            draw_point(new_point(iter->x,iter->y),d,obj->skin);
        }
    }
}

void clear_shape(shape *obj,display *d){
    if((obj->shape_pos->x)){
        for (point *iter = obj->shape_pos;iter< obj->shape_pos+obj->p_num; iter++)
        {
            clear_point(new_point(iter->x,iter->y),d);
        }

    }
}

void delete_shape(shape *obj){
   obj->p_num=0;free(obj->shape_pos);
   //free(obj);
}

int is_point_in_shape(shape obj, point p){
    int is=0;
    if((obj.shape_pos->x*obj.shape_pos->y)&& p.x*p.y && is_point_in_rect(p,obj.boarders)){
        for (point *iter = obj.shape_pos;iter< obj.shape_pos+obj.p_num; iter++)
        {
            if(is_same_point(*iter,p)){is=1;  break;  }
        }
    }
    return is;
}


void point_breacks_shape(shape *obj, point p){
    point *iter;
    if((obj->shape_pos->x*obj->shape_pos->y)&& p.x*p.y && is_point_in_rect(p,obj->boarders)){
        for ( iter = obj->shape_pos;iter< obj->shape_pos+obj->p_num; iter++)
        {
            if(is_same_point(*iter,p))break;
        }
        for ( iter; iter< obj->shape_pos+obj->p_num-1; iter++)
        {
            new_pointfp(iter,*(iter+1));
        }
        obj->p_num-=1;
    }
}
////======== OTHER ========
void draw_word(char * word,display *d,point start){
    if(*word && *d->buf)
    for (int i = 0; i < strlen(word); ++i) {
        if(word[i]!='\n')d->buf[start.y-1][start.x-1+i]=word[i];
       // if(can_move_point(start,d,RIGHT))move_point(&start,RIGHT);
    }
}


int str_is_upper(const char * str){
    while(*str) {
        if(isalpha(*str)&&islower(*str))return 0;
        ++str;
    }
    return 1;
}


int str_is_lower(const char * str){
    while(*str) {
        if(isalpha(*str)&&isupper(*str))return 0;
        ++str;
    }
    return 1;
}

void str_to_upper(char *str){
   // char* p=str;
    for (int i = 0; i < strlen(str); ++i) {
        str[i]=(char )toupper(str[i]);
    }
   // while(*str){  *str = (char )toupper(*str);  str++;  }
}

void str_to_lower(char *str){
    for (int i = 0; i < strlen(str); ++i) {
        str[i]=(char )tolower(str[i]);
    }
}

int draw_logo_digit(char c,display *d){
    draw_point(new_point(5,6),d,'-');
}

int draw_logo_literal(char c,display *d){
}

void draw_logo(char *label,display *d){
    static int state=DR,counter=0;

    //if(label)draw_word(label,d,new_point((int)(d->width/2-strlen(label)/2),13));

    switch (state) {

        case DR:
           draw_word("  ______   _______    ______   __    __   " ,d,new_point((int)(d->width/2-44/2),(int)d->length-2));
           draw_word(" /      \\ |       \\  /      \\ |  \\  |  \\" ,d,new_point((int)(d->width/2-44/2),(int)d->length-3));
           draw_word("|  $$$$$$\\| $$$$$$$\\|  $$$$$$\\| $$  | $$",d,new_point((int)(d->width/2-44/2),(int)d->length-4));
           draw_word("| $$ __\\$$| $$__/ $$| $$  | $$ \\$$\\/  $$",d,new_point((int)(d->width/2-44/2),(int)d->length-5));
           draw_word("| $$|    \\| $$    $$| $$  | $$  >$$  $$   ",d,new_point((int)(d->width/2-44/2),(int)d->length-6));
           draw_word("| $$ \\$$$$| $$$$$$$\\| $$  | $$ /  $$$$\\ ",d,new_point((int)(d->width/2-44/2),(int)d->length-7));
           draw_word("| $$__| $$| $$__/ $$| $$__/ $$|  $$ \\$$\\ ",d,new_point((int)(d->width/2-44/2),(int)d->length-8));
           draw_word(" \\$$    $$| $$    $$ \\$$    $$| $$  | $$ ",d,new_point((int)(d->width/2-44/2),(int)d->length-9));
           draw_word("  \\$$$$$$  \\$$$$$$$   \\$$$$$$  \\$$   \\$$",d,new_point((int)(d->width/2-44/2),(int)d->length-10));
        state=UR;
            break;
        case UR:
           draw_word("  /$$$$$$  /$$$$$$$   /$$$$$$  /$$   /$$   ",d,new_point((int)(d->width/2-44/2),(int)d->length-2));
           draw_word(" /$$__  $$| $$__  $$ /$$__  $$| $$  / $$   ",d,new_point((int)(d->width/2-44/2),(int)d->length-3));
           draw_word("| $$  \\__/| $$  \\ $$| $$  \\ $$|  $$/ $$/",d,new_point((int)(d->width/2-44/2),(int)d->length-4));
           draw_word("| $$ /$$$$| $$$$$$$ | $$  | $$ \\  $$$$/   ",d,new_point((int)(d->width/2-44/2),(int)d->length-5));
           draw_word("| $$|_  $$| $$__  $$| $$  | $$  >$$  $$    ",d,new_point((int)(d->width/2-44/2),(int)d->length-6));
           draw_word("| $$  \\ $$| $$  \\ $$| $$  | $$ /$$/\\  $$",d,new_point((int)(d->width/2-44/2),(int)d->length-7));
           draw_word("|  $$$$$$/| $$$$$$$/|  $$$$$$/| $$  \\ $$  ",d,new_point((int)(d->width/2-44/2),(int)d->length-8));
           draw_word(" \\______/ |_______/  \\______/ |__/  |__/ ",d,new_point((int)(d->width/2-44/2),(int)d->length-9));
           state=UL;
            break;
        case UL:
           draw_word(" $$$$$$\\  $$$$$$$\\   $$$$$$\\  $$\\   $$\\ ",d,new_point((int)(d->width/2-44/2),(int)d->length-2));
           draw_word("$$  __$$\\ $$  __$$\\ $$  __$$\\ $$ |  $$ |",d,new_point((int)(d->width/2-44/2),(int)d->length-3));
           draw_word("$$ /  \\__|$$ |  $$ |$$ /  $$ |\\$$\\ $$  |",d,new_point((int)(d->width/2-44/2),(int)d->length-4));
           draw_word("$$ |$$$$\\ $$$$$$$\\ |$$ |  $$ | \\$$$$  / ",d,new_point((int)(d->width/2-44/2),(int)d->length-5));
           draw_word("$$ |\\_$$ |$$  __$$\\ $$ |  $$ | $$  $$<   ",d,new_point((int)(d->width/2-44/2),(int)d->length-6));
           draw_word("$$ |  $$ |$$ |  $$ |$$ |  $$ |$$  /\\$$\\  ",d,new_point((int)(d->width/2-44/2),(int)d->length-7));
           draw_word("\\$$$$$$  |$$$$$$$  | $$$$$$  |$$ /  $$ |  ",d,new_point((int)(d->width/2-44/2),(int)d->length-8));
           draw_word(" \\______/ \\_______/  \\______/ \\__|  \\__|",d,new_point((int)(d->width/2-44/2),(int)d->length-9));
           state=DL;
            break;

        case DL:

        default:
             draw_word( "  ______   _______    ______   __    __ ",d,new_point((int)(d->width/2-44/2),(int)d->length-2));
             draw_word( "/$$$$$$  |$$$$$$$  |/$$$$$$  |$$ |  $$ |",d,new_point((int)(d->width/2-44/2),(int)d->length-3));
             draw_word( "$$ | _$$/ $$ |__$$ |$$ |  $$ |$$  \\/$$/",d,new_point((int)(d->width/2-44/2),(int)d->length-4));
             draw_word( "$$ |/    |$$    $$< $$ |  $$ | $$  $$<  ",d,new_point((int)(d->width/2-44/2),(int)d->length-5));
             draw_word( "$$ |$$$$ |$$$$$$$  |$$ |  $$ |  $$$$  \\",d,new_point((int)(d->width/2-44/2),(int)d->length-6));
             draw_word( "$$ \\__$$ |$$ |__$$ |$$ \\__$$ | $$ /$$ |",d,new_point((int)(d->width/2-44/2),(int)d->length-7));
             draw_word( "$$    $$/ $$    $$/ $$    $$/ $$ |  $$|",d,new_point((int)(d->width/2-44/2),(int)d->length-8));
             draw_word( " $$$$$$/  $$$$$$$/   $$$$$$/  $$/   $$/ ",d,new_point((int)(d->width/2-44/2),(int)d->length-9));
             state=DR;
             break;
    }

}


void gameover_logo(display *d){

    draw_word( "    ________    _____      _____  ___________ ",d,new_point((int)(d->width/2-46/2),(int)d->length-2));
    draw_word( "   /  _____/   /  _  \\    /     \\ \\_   _____/ ",d,new_point((int)(d->width/2-46/2),(int)d->length-3));
    draw_word( "  /   \\  ___  /  /_\\  \\  /  \\  / \\ |    __)_  ",d,new_point((int)(d->width/2-46/2),(int)d->length-4));
    draw_word( "  \\    \\_\\  \\/    |    \\/    \\/   \\|        \\ ",d,new_point((int)(d->width/2-46/2),(int)d->length-5));
    draw_word(  "   \\______  /\\____|__  /\\_____|__ /_______  / ",d,new_point((int)(d->width/2-46/2),(int)d->length-6));
    draw_word(  "  ___________     ________________________    ",d,new_point((int)(d->width/2-46/2),(int)d->length-7));
    draw_word(  "  \\_____ \\   \\  /  /\\_   _____/\\______   \\   ",d,new_point((int)(d->width/2-46/2),(int)d->length-8));
    draw_word( "   /   |   \\  \\/  /  |    __)_  |       _/   ",d,new_point((int)(d->width/2-46/2),(int)d->length-9));
    draw_word( "  /    |    \\    /  |        \\ |    |   \\   ",d,new_point((int)(d->width/2-46/2),(int)d->length-10));
    draw_word( "  \\_______  /\\__/   /_______  / |____|_  /   ",d,new_point((int)(d->width/2-46/2),(int)d->length-11));
    draw_word( "          \\/                 \\/         \\/    ",d,new_point((int)(d->width/2-46/2),(int)d->length-12));
    refresh(*d);
    Sleep(3000);
}

void you_win_logo(display *d){

    draw_word( " _____.___.________   ____ ___ ",d,new_point((int)(d->width/2-36/2),(int)d->length-2));
    draw_word( " \\__  |   |\\_____  \\ |    |   \\",d,new_point((int)(d->width/2-36/2),(int)d->length-3));
    draw_word( "  /   |   | /   |   \\|    |   /",d,new_point((int)(d->width/2-36/2),(int)d->length-4));
    draw_word( "  \\____   |/    |    \\    |  / ",d,new_point((int)(d->width/2-36/2),(int)d->length-5));
    draw_word( "  / ______|\\_______  /______/  ",d,new_point((int)(d->width/2-36/2),(int)d->length-6));
    draw_word( "  \\/               \\/          ",d,new_point((int)(d->width/2-36/2),(int)d->length-7));
    draw_word( " __      __.___ _______ ._.    ",d,new_point((int)(d->width/2-36/2),(int)d->length-8));
    draw_word( "/  \\    /  \\   |\\      \\| |  ",d,new_point((int)(d->width/2-36/2),(int)d->length-9));
    draw_word( "\\   \\/\\/   /   |/   |   \\ |  ",d,new_point((int)(d->width/2-36/2),(int)d->length-10));
    draw_word( " \\        /|   /    |    \\|    ",d,new_point((int)(d->width/2-36/2),(int)d->length-11));
    draw_word( "  \\__/\\  / |___\\____|__  /_  ",d,new_point((int)(d->width/2-36/2),(int)d->length-12));
    draw_word( "       \\/              \\/\\/  ",d,new_point((int)(d->width/2-36/2),(int)d->length-13));
    refresh(*d);
    Sleep(3000);
}


void draw_meme(int which_one){
    switch (which_one) {
        case 1: printf("___________________$$$$$$\n"
                       "_______________$$$__$$_$$$\n"
                       "_______________$____00___$$§§\n"
                       "_______________WWWW________$§§§\n"
                       "________________$$$$$$___$__$§§§\n"
                       "__________________§§§$$___$_$§§§§\n"
                       "_________________§§§§§$___$_$§§§§§\n"
                       "_________________§§§§§$___$$$§§§§§\n"
                       "_________________§§§§$$_$___$§§§§§\n"
                       "________________§§§§§§$__$_$$§§§§§\n"
                       "________________§§§§§$$___$$§§§§§§\n"
                       "_______________§§§§§§$__$_$$§§§§§\n"
                       "_______________§§§§$$___$_$§§§§§\n"
                       "_______________§§§$$___$_$$§§§§\n"
                       "________________§$$____$_$§§§§\n"
                       "_________________$____$_$$§§\n"
                       "________________$$___$__$$\n"
                       "_______________$$______$$\n"
                       "_______________$______$$_$\n"
                       "______________$$_____$$_$$$______$$$$$$$$$$$\n"
                       "______________$______$___$$$$$$$$___________$\n"
                       "_______$$$$$_$$___$_$$____$$$$$$____$$$______$\n"
                       "___$$$$____$$$$_$_$_$$$$$$$$____$$$$$$$$$$$__$\n"
                       "__$$____$____$$___$_$$______$$$$__________$$$$$\n"
                       "_$_____$______$_$___$$_$$$$$____________$_____$$$\n"
                       "$______$$$$$$$$____$_$$$_________________$$_____$$\n"
                       "$______$$_____$_$____$$__________$$$$$$$$$$______$\n"
                       "$$_______$$$$_$$_$$_$_$$_____$$$$$$_______$______$\n"
                       "_$$____________$$____$_$$$$$$$__________$$_______$\n"
                       "___$$___________$_____$__$$$_______$$$$$$_______$$\n"
                       "____$$$$$_ ___$$$________$$$$$$$$$___________$$\n"
                       "_______$$$$$$$$$$_$$$________________________$$$\n"
                       "____________________$$$$__________________$$$$\n"
                       "________________________$$$$$$$$$$$$$$$$$$$\n");
            Sleep(5000);
            break;
        case 2:    printf("__________________________________________________\n"
                          "___________________$$$$$$$$$$$$__________________\n"
                          "______________$$$$$$$________$$$$$$$_______________\n"
                          "____________$$$$__________________$$$$____________\n"
                          "__________$$$________________________$$$__________\n"
                          "_________$$____________________________$$$________\n"
                          "_______$$$_______________________________$$_______\n"
                          "______$$__________________________________$$______\n"
                          "_____$$_____________________________________$_____\n"
                          "____$$______________________________________$$____\n"
                          "___$$_______________________________¶________$$___\n"
                          "___$$_____________________________¶¶¶¶¶¶¶_____$$__\n"
                          "___$$____________________________¶¶_____¶¶_____$__\n"
                          "___$$____________________________________¶¶____$$_\n"
                          "___$$¶¶__________________________________¶_____$$_\n"
                          "___$$¶¶¶¶________________________¶¶¶¶¶¶¶_¶¶_____$_\n"
                          "__$$_____¶_________________¶¶____¶¶____¶_¶______$_\n"
                          "_$$______________________________¶______________$_\n"
                          "_$$___¶¶¶¶¶_____________¶¶_____¶¶¶¶¶¶___________$_\n"
                          "__$$_¶__¶¶¶¶¶_____¶__¶¶¶____¶¶¶_____¶__¶¶_______$_\n"
                          "___$$___¶___¶¶¶____¶¶¶____¶¶¶_______¶_¶¶_______¶$_\n"
                          "_____¶¶¶¶¶____¶¶¶__¶¶¶¶_¶¶¶_________¶_¶¶_______$__\n"
                          "_______¶¶¶_______¶¶¶__¶¶____________¶_¶¶______¶$__\n"
                          "_______¶¶_¶______¶¶__¶_¶__________¶¶___¶_____¶$___\n"
                          "_______¶¶___¶¶¶¶¶_¶__¶_¶¶¶¶_____¶¶¶____¶¶¶¶¶¶¶$___\n"
                          "______¶¶__________¶¶¶¶¶___¶¶¶¶_¶¶____________¶$___\n"
                          "______¶________$_$$__$$$$$________________¶¶¶$____\n"
                          "______¶¶______$$$______$$_$$____________¶¶¶$______\n"
                          "_______¶¶______$__________$___________¶¶¶_$_______\n"
                          "________$$$_$$$________________$____$$$___$_______\n"
                          "_________¶¶¶__¶¶_____________¶¶¶¶¶¶¶¶_____¶_______\n"
                          "___________¶__¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶____¶¶______¶¶______\n"
                          "___________¶__¶_¶¶_¶¶¶_¶¶¶_¶¶¶____¶_______¶_______\n"
                          "___________¶¶_¶¶___¶_¶_¶_¶_¶__¶___¶______¶¶_______\n"
                          "____________¶¶_¶_¶¶¶__¶_¶¶¶_¶_¶__¶_____¶¶¶________\n"
                          "_____________¶__¶¶_¶__¶_¶_¶_¶¶¶¶¶_____¶¶__________\n"
                          "______________¶__¶_¶¶_¶_¶¶¶¶¶__¶¶____¶¶___________\n"
                          "______________¶¶____________________¶_____________\n"
                          "_______________¶¶__________________¶______________\n"
                          "_______________¶¶_________________¶_______________\n"
                          "_______________¶¶________________¶________________\n"
                          "________________¶_______________¶_________________\n"
                          "________________¶¶¶____________¶__________________\n"
                          "_________________¶¶¶¶¶¶¶¶¶__¶¶¶¶__________________\n"
                          "__________________________¶¶______________________\n");
            Sleep(5000);
            break;
        default: printf("_____/\\\\\\\\\\\\\\\\\\\\\\\\_____/\\\\\\\\\\\\\\\\\\_____/\\\\\\\\____________/\\\\\\\\__/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\____________/\\\\\\\\\\\\\\\\\\\\\\\\\\_________/\\\\\\\\\\_______/\\\\\\_______/\\\\\\_        \n"
                        " ___/\\\\\\//////////____/\\\\\\\\\\\\\\\\\\\\\\\\\\__\\/\\\\\\\\\\\\________/\\\\\\\\\\\\_\\/\\\\\\///////////____________\\/\\\\\\/////////\\\\\\_____/\\\\\\///\\\\\\____\\///\\\\\\___/\\\\\\/__       \n"
                        "  __/\\\\\\______________/\\\\\\/////////\\\\\\_\\/\\\\\\//\\\\\\____/\\\\\\//\\\\\\_\\/\\\\\\_______________________\\/\\\\\\_______\\/\\\\\\___/\\\\\\/__\\///\\\\\\____\\///\\\\\\\\\\\\/____      \n"
                        "   _\\/\\\\\\____/\\\\\\\\\\\\\\_\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\\\///\\\\\\/\\\\\\/_\\/\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\\\_______________\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\___/\\\\\\______\\//\\\\\\_____\\//\\\\\\\\______     \n"
                        "    _\\/\\\\\\___\\/////\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\__\\///\\\\\\/___\\/\\\\\\_\\/\\\\\\///////________________\\/\\\\\\/////////\\\\\\_\\/\\\\\\_______\\/\\\\\\______\\/\\\\\\\\______    \n"
                        "     _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\/////////\\\\\\_\\/\\\\\\____\\///_____\\/\\\\\\_\\/\\\\\\_______________________\\/\\\\\\_______\\/\\\\\\_\\//\\\\\\______/\\\\\\_______/\\\\\\\\\\\\_____   \n"
                        "      _\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_____________\\/\\\\\\_\\/\\\\\\_______________________\\/\\\\\\_______\\/\\\\\\__\\///\\\\\\__/\\\\\\_______/\\\\\\////\\\\\\___  \n"
                        "       _\\//\\\\\\\\\\\\\\\\\\\\\\\\/__\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_____________\\/\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\___________\\/\\\\\\\\\\\\\\\\\\\\\\\\\\/_____\\///\\\\\\\\\\/______/\\\\\\/___\\///\\\\\\_ \n"
                        "        __\\////////////____\\///________\\///__\\///______________\\///__\\///////////////____________\\/////////////_________\\/////_______\\///_______\\///__\n"
                        "____________________________________________/\\\\\\____________________________/\\\\\\______________________                                                \n"
                        " ___________________________________________\\/\\\\\\___________________________\\/\\\\\\______________________                                               \n"
                        "  ___________________________________________\\/\\\\\\___________________________\\/\\\\\\___________/\\\\\\__/\\\\\\_                                              \n"
                        "   ____/\\\\\\\\\\__/\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\___________\\/\\\\\\______/\\\\\\\\\\\\\\\\____________\\/\\\\\\__________\\//\\\\\\/\\\\\\__                                             \n"
                        "    __/\\\\\\///\\\\\\\\\\///\\\\\\_\\////////\\\\\\_____/\\\\\\\\\\\\\\\\\\____/\\\\\\/////\\\\\\___________\\/\\\\\\\\\\\\\\\\\\_____\\//\\\\\\\\\\___                                            \n"
                        "     _\\/\\\\\\_\\//\\\\\\__\\/\\\\\\___/\\\\\\\\\\\\\\\\\\\\___/\\\\\\////\\\\\\___/\\\\\\\\\\\\\\\\\\\\\\____________\\/\\\\\\////\\\\\\_____\\//\\\\\\____                                           \n"
                        "      _\\/\\\\\\__\\/\\\\\\__\\/\\\\\\__/\\\\\\/////\\\\\\__\\/\\\\\\__\\/\\\\\\__\\//\\\\///////_____________\\/\\\\\\__\\/\\\\\\__/\\\\_/\\\\\\_____                                          \n"
                        "       _\\/\\\\\\__\\/\\\\\\__\\/\\\\\\_\\//\\\\\\\\\\\\\\\\/\\\\_\\//\\\\\\\\\\\\\\/\\\\__\\//\\\\\\\\\\\\\\\\\\\\___________\\/\\\\\\\\\\\\\\\\\\__\\//\\\\\\\\/______                                         \n"
                        "        _\\///___\\///___\\///___\\////////\\//___\\///////\\//____\\//////////____________\\/////////____\\////________                                        \n"
                        "_____/\\\\\\\\\\\\\\\\\\\\\\______________________/\\\\\\________/\\\\\\__/\\\\\\\\\\\\\\\\\\\\\\__/\\\\\\________/\\\\\\_                                                              \n"
                        " ___/\\\\\\/////////\\\\\\___________________\\/\\\\\\_______\\/\\\\\\_\\/////\\\\\\///__\\/\\\\\\_____/\\\\\\//__                                                             \n"
                        "  __\\//\\\\\\______\\///____________________\\//\\\\\\______/\\\\\\______\\/\\\\\\_____\\/\\\\\\__/\\\\\\//_____                                                            \n"
                        "   ___\\////\\\\\\____________________________\\//\\\\\\____/\\\\\\_______\\/\\\\\\_____\\/\\\\\\\\\\\\//\\\\\\_____                                                           \n"
                        "    ______\\////\\\\\\__________________________\\//\\\\\\__/\\\\\\________\\/\\\\\\_____\\/\\\\\\//_\\//\\\\\\____                                                          \n"
                        "     _________\\////\\\\\\________________________\\//\\\\\\/\\\\\\_________\\/\\\\\\_____\\/\\\\\\____\\//\\\\\\___                                                         \n"
                        "      __/\\\\\\______\\//\\\\\\________________________\\//\\\\\\\\\\__________\\/\\\\\\_____\\/\\\\\\_____\\//\\\\\\__                                                        \n"
                        "       _\\///\\\\\\\\\\\\\\\\\\\\\\/____/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\______\\//\\\\\\________/\\\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\______\\//\\\\\\_                                                       \n"
                        "        ___\\///////////_____\\///////////////________\\///________\\///////////__\\///________\\///__  \n");
    }


}