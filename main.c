#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stdarg.h"
#include <conio.h>
#include "drawable.h"
#include "arkanoid.h"
#include "snake.h"
//#include "tetris.h"


enum main_menu_state{EXIT=0,ARKANOID,SNAKE,TETRIS};

//здесь будет функция из либы Arduino
int get_width(){
    return 50;
}

//здесь будет функция из либы Arduino
int get_length(){
    return 20;
}

typedef struct menu{
    char* logo;
    char **strings;
    char *quit_text;
    int num_of_str;
    int state;
}menu;


void init_menu(menu *m,char *logo,char *quit_text,...) {

    va_list ap;
    va_start(ap, quit_text);

    m->logo = (char *) malloc(strlen(logo)*sizeof(char ));
    strcpy(m->logo,logo);

    m->quit_text = (char *) malloc(strlen(quit_text)*sizeof(char ));
    strcpy(m->quit_text,quit_text);

    m->state=1;

    char *tmp;
    if (( m->num_of_str=va_arg(ap,int))) {
        m->strings = (char **) malloc(m->num_of_str * sizeof(char *));
        for (int i = 0; i < m->num_of_str; ++i) {
            tmp = va_arg(ap,char *);
            m->strings[i]=(char *) malloc(strlen(tmp)*sizeof(char ));
            strcpy( m->strings[i],tmp); //memcpy(m->strings[i],tmp,strlen(tmp) * sizeof(char));
        }
    }
    va_end(ap);
}



void draw_menu(menu *m,display *d,int selected_state){
    draw_logo(m->logo,d);//TODO: честно отрисовывай лого
    if (selected_state>0)
    {
        m->state=selected_state;
        if(m->strings[m->state-1]) {
            str_to_upper(m->strings[m->state-1]);
            draw_word(m->strings[m->state-1], d, new_point((int) (d->width / 2 - strlen(m->strings[m->state-1]) / 2),
                                                         (int) (d->length / 3)));//!!!!
        }
        if(m->quit_text) {
            str_to_lower(m->quit_text);
            draw_word(m->quit_text, d, new_point((int) (d->width / 2 - strlen(m->quit_text) / 2),
                                                         (int) (d->length / 3 -2)));//!!!!!!
        }
    } else{
        if(m->strings[m->state-1]) {
            str_to_lower(m->strings[m->state-1]);
            draw_word(m->strings[m->state-1], d, new_point((int) (d->width / 2 - strlen(m->strings[m->state-1]) / 2),
                                                                  (int) (d->length / 3 )));//!!!!
        }
        if(m->quit_text) {
            str_to_upper(m->quit_text);
            draw_word(m->quit_text, d, new_point((int) (d->width / 2 - strlen(m->quit_text) / 2),
                                                         (int) (d->length / 3 -2)));//!!!!!!
        }
    }
}



/// Проверка нажатия клавиши в windows.h: GetKeyState("D")
#ifdef WIN32


int select_menu(display *d){
    int ecode=ESC,select_state=1;
    menu m;

    init_menu(&m,"menu","quit",3," arcaniod >","< snake >","< tetris ");
    do{
        refresh(*d);
        Sleep(70);
        if(GetKeyState(VK_UP)<0 || GetKeyState(VK_DOWN)<0)select_state = (select_state>0)? 0 : m.state;
        if(GetKeyState(VK_LEFT)<0 && (select_state>0 && select_state-1!=0))select_state-=1;
        if(GetKeyState(VK_RIGHT)<0 && (select_state>0 && select_state!=m.num_of_str))select_state+=1;
        if(GetKeyState(VK_RETURN)<0){
            ecode = (select_state==EXIT) ? ESC : select_state;
            break;
        }
        clear(d);
        draw_menu(&m,d,select_state);
    }while (GetKeyState(VK_ESCAPE)>=0);
  //  if (select_state!=EXIT)ecode=select_state;
    return ecode;
}
#else
int select_menu(display *d){
    int ecode=0,select_state=1;
    menu m;
    init_menu(&m,"menu","quit",3,"< arcaniod >","< snake >","< tetris >");
    while (ecode!=ESC && ecode!=ENTER){
       clear(d);
       draw_menu(&m,d,select_state);
       ecode=getch();
      switch (ecode) {
            case KEY_UP: //select_state = (select_state>0)? 0 : m.state;
            case KEY_DOWN: select_state = (select_state>0)? 0 : m.state;
                break;
            case KEY_LEFT: if(select_state>0 && select_state-1!=0)select_state-=1;
                break;
            case KEY_RIGHT: if(select_state>0 && select_state!=m.num_of_str)select_state+=1;
                break;
            case ENTER: if(select_state==EXIT)ecode=ESC;
                break;
            default :  break;
      }
    }
    if (select_state!=EXIT)ecode=select_state;
    return ecode;
}
#endif

int start_activity(display *d, int choice){

    switch (choice) {
        case ARKANOID: choice=start_arkanoid(d);
            break;
        case SNAKE: choice=start_snake(d);
            break;
        case TETRIS: draw_meme(2);

            break;
        default: draw_meme(432);
            break;
    }
    return choice;
}

int main() {

    int d_width = get_width();
    int d_length = get_length();
  //int *(*game)(char ***,int *);
    display d;
    int ecode=0;
    init_display(&d,d_length,d_width);
    while (ecode != ESC) {
         ecode = select_menu(&d);
        if(ecode!=ESC) ecode = start_activity(&d,ecode);
        if (ecode != ESC) printf("smth goes wrong, we got ERROR!");
    }
    return 0;
}



void arts_to_future(){
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
           " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~_.,-'=_.-'-._~~~~~~~~~~~~~~~~~~\n"
           "         ~     ~           ~   ._.-'**************'-._   ~\n"
           "                      _.-':_.-'***********************'-._   ~     ~\n"
           "                  _.-'                                   '-._.'-._\n"
           "   ~       .-'.-,'                                                '-.\n"
           "           '-._                       /\\   /\\                    _.-'\n"
           " ~             '-.                   /  \\ /  \\                ._'\n"
           "           ~      '-._                 /\\ (           _.'-._,-'\n"
           "                      '-._            /  \\ )      _.-'   (o o)\n"
           " ~     ) ( ) (    ~     .-'               (     .'       ))~((  ~\n"
           "      ) \" ( \" (        .-'                 )    '-._.,.            ~\n"
           "     )  \"  (\"  (       '-._               /           '-._  ~ \n"
           "    )   \"   (   ( ___      '-._          (                '.   ~\n"
           "        \"   \"    |   | ~      .'          )                '-._\n"
           "  ---._-|--|--|--|--/     _.-'           /                  _.'   ~\n"
           "       \\ o  o  o  o/     '-._           /                  '-._-'-.\n"
           "   ~~~~~~~~~~~~~~~~~         '-._      (                        _.-'\n"
           "  ~          ~             ~     '-.    ) /\\            _.-\"._,'\n"
           "                  ~              _.'   / /\\ /\\         '.  ~    (o o)\n"
           "    (o o)              _.-'-._.-'     / /  \\  \\          '-._._ ))~((\n"
           "    ))~(( ~        _.-'              /                         '-. ~\n"
           "                .-'              .-'('-._                        '-.\n"
           " ~            _.'         _.-'-.-'~   ~  '.             _.'-.-'._  .'\n"
           "     .-'=_.'-'         .-'  ~   ~   _ _.-'          _.-'     ~   '.'\n"
           "  _.-'                 '-._,.-'._.-'    (o)(o)     '_   ~       ~\n"
           ".'                                         \\)         '-._   ~    ~\n"
           "'-.- = .-'.     (o)(o)                                    '=._\n"
           "          '._    (  )                                         '-.\n"
           "LGB     ~    :_                                            _.-'.-' ~\n"
           "     ~     ~   \"._,-'.-'._    .-`-._;'-._.='._          .-'  ~\n"
           "                    ~     '-_.\"      ~    ~   '-._:'=~_.'       ~\n"
           "           ~     ~      ~        ~     ~        ~          ~    ~");

    printf("   -----|----- \n"
           "*>=====[_]L)   \n"
           "      -'-`-     ");
}

