#include <stdio.h>
#include <dirent.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

struct for_copy{
  char str[256];
  char name[256];
};

int size_file(char *name){
    struct stat buff;
    int  one;
    stat(name, &buff);
    one=buff.st_size;
    return(one);
}

WINDOW* window1(int y, int x, int py, int px){
    WINDOW* wnd;
    wnd=newwin(y,x,py,px);
    box(wnd, '|','-' );
    wrefresh(wnd);
    refresh();
    return wnd;
}
WINDOW* window2(WINDOW* wnd, int y, int x, int py, int px){
    WINDOW* subwnd;
    subwnd=derwin(wnd,y,x,py,px);
    wrefresh(subwnd);
    scrollok(subwnd, TRUE);
    return subwnd;
}
void sig_winch(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

void copy_move(char *name){
    double  pr, n=0; int  i=0, j=0;
    pr=1000/((double)size_file(name));
    WINDOW *wnd, *subwnd;
    wnd=window1(3,50,10,10);
    subwnd=window2(wnd,1,48,1,1);
    while(n<100){
        n=n+pr;
        if(n>100)
             n=100;
        mvwprintw(subwnd,0,23,"%.1f",n);
        wrefresh(subwnd);
    }
    wrefresh(wnd);
    wrefresh(subwnd);
   // wgetch(subwnd);
    delwin(wnd);
    delwin(subwnd);
    endwin();
}

void copy(struct for_copy *str){
    int f1, f2; char c[10]; int pr;
    f1=open(str->name,O_RDONLY);
    f2=open(str->str, O_WRONLY|O_CREAT,0666);
    while((pr=read(f1,c, sizeof(char)*10))!=NULL){
        write(f2,c, pr);//sizeof(char)*10);
       // read(f1,c, sizeof(char)*10);
    }
   close(f1);
   close(f2);
}

void fileredactor(WINDOW*neww, int i, int j){
    char c;
    c=wgetch(neww);
    while(c!='1'){
        mvwaddch(neww,i,j,c);
        wmove(neww,i,j++);
        c=wgetch(neww);
    }
}
void openfile(char *name, WINDOW*neww){
    FILE* fd; char c; int n=0, log=0; char stroka[64];
    fd=fopen(name,"r");
    fread(&c,1,1,fd);
    while(!feof(fd)){
        waddch(neww,c);
        fread(&c,1,1,fd);
        n++;
    }
    fclose(fd);
    wrefresh(neww);
    c='0'; int i=0, j=0;
    wmove(neww,i,j);
    while(c!='q'){
        c=wgetch(neww);
        if(c=='w')
            wmove(neww,--i,j);
        if(c=='s')
            wmove(neww,++i,j);
        if(c=='d')
            wmove(neww,i,++j);
        if(c=='a')
            wmove(neww,i,--j);
        if(c=='e'){
            log=1;
            fileredactor(neww, i, j);
        }
    }
    if(log==1){
        j=0; c='\n';
        fd=fopen(name,"w");
        for(i=0; i<24; i++){
            mvwinstr(neww,i,j, stroka);
            fwrite(stroka,sizeof(char)*64,1,fd);
            fwrite(&c,sizeof(char),1,fd);
        }
        fclose(fd);
    }
}
void windowfile(char *name){
    WINDOW *neww;
    neww=newwin(24,64,0,0);
    scrollok(neww, TRUE);
    openfile(name, neww);
    wclear(neww);
    wrefresh(neww);
    delwin(neww);
}
void output(char *name, WINDOW*win){
    DIR *dp;
    dp=opendir(name);
    struct dirent *rez;
    wclear(win);
    while((rez=readdir(dp))!=NULL){
        if(rez->d_type==DT_DIR)
            wprintw(win,"_");
        wprintw(win,rez->d_name);
        wprintw(win,"|");
        wprintw(win,"\n");
    }
    wrefresh(win);
    closedir(dp);
}
int summa(char *dir){
    int i=0;
    while(dir[i]!='|'){
        i++;
    }
    return i;
}
char redactor(char *name, int n){
    int i;
    for(i=0; i<n-1; i++){
        name[i]=name[i+1];
    }
    name[n-1]='\0';
    return *name;
}
void navigation(WINDOW *subwnd,WINDOW *subwnd2){
    char c='p'; int i=0, j=0, log=0, n;
    WINDOW *win=subwnd; WINDOW *wnd, *wnd2;
    char dir[256]; char *name; char *parent; char *dir_name;
    parent=malloc(sizeof(char)*2);
    parent[0]='.';
    parent[1]='\0';
    chdir(parent);
    wmove(win,i,j);
    do{
        if(c=='w')
             wmove(win,--i,j);
        if(c=='s')
             wmove(win,++i,j);
        if(c=='d'){
             win=subwnd2;
             i=0; j=0; log=1;
             wmove(win,i,j);
             free(parent);
             parent=malloc(sizeof(dir_name));
             strcpy(parent,dir_name);
             chdir(parent);
         }
         if(c=='a'){
             win=subwnd;
             i=0; j=0; log=0;
             wmove(win,i,j);
             free(parent);
             parent=malloc(sizeof(dir_name));
             strcpy(parent,dir_name);
             chdir(parent);
         }
         if(c=='e'){
             name=malloc(sizeof(char)*(n+1));
             name=dir;
             name[n]='\0';
             delwin(wnd);
             delwin(subwnd);
             delwin(wnd2);
             delwin(subwnd2);
             windowfile(name);
             wnd=window1(20,30,1,1);
             subwnd=window2(wnd,16,18,1,1);
             wnd2=window1(20,30,1,32);
             subwnd2=window2(wnd2,16,18,1,1);
             win=subwnd; log=0; i=0; j=0;
             output(".",win);
             free(parent);
             parent=malloc(sizeof(char)*2);
             parent[0]='.';
             parent[1]='\0';
             chdir(parent);
             wmove(win,i,j);
             wrefresh(wnd);
             wrefresh(wnd2);
             wrefresh(subwnd);
             wrefresh(subwnd2);
         }
         if(c=='r'){
             pid_t  pid;
             name=malloc(sizeof(char)*(n+1));
             name=dir;
             name[n]='\0';
             delwin(wnd);
             delwin(subwnd);
             delwin(wnd2);
             delwin(subwnd2);
             wnd=window1(20,61,1,1);
             subwnd=window2(wnd,16,30,1,1);
             wrefresh(wnd);
             wrefresh(subwnd);
             pid=fork();
             if(pid==0){
                 execl(name,name, NULL);
             }
             else
                 wait(0);
             wgetch(subwnd);
             wclear(subwnd);
             wclear(wnd);
             wrefresh(wnd);
             wrefresh(subwnd);
             delwin(wnd);
             delwin(subwnd);
             wnd=window1(20,30,1,1);
             subwnd=window2(wnd,16,18,1,1);
             wnd2=window1(20,30,1,32);
             subwnd2=window2(wnd2,16,18,1,1);
             win=subwnd; log=0; i=0; j=0;
             output(".",win);
             free(parent);
             parent=malloc(sizeof(char)*2);
             parent[0]='.';
             parent[1]='\0';
             chdir(parent);
             wmove(win,i,j);
             wrefresh(wnd);
             wrefresh(wnd2);
             wrefresh(subwnd);
             wrefresh(subwnd2);

         }
         if(c=='c'){
             struct for_copy stroka; pthread_t tid1, tid2;
             name=malloc(sizeof(char)*(n+1));
             name=dir;
             name[n]='\0';
             delwin(wnd);
             delwin(subwnd);
             delwin(wnd2);
             delwin(subwnd2);
             wnd=window1(20,61,1,1);
             subwnd=window2(wnd,16,30,1,1);
             wprintw(subwnd,"Where to copy?");
             echo();
             wmove(subwnd,3,0);
             wgetstr(subwnd,stroka.str);
             noecho();
             strcat(stroka.str,name);
             strcpy(stroka.name,name);
             pthread_create(&tid1,NULL,copy,&stroka);
             pthread_create(&tid2, NULL, copy_move,&stroka.name);
             pthread_join(tid1,NULL);
             pthread_join(tid2,NULL);
             wrefresh(wnd);
             wrefresh(subwnd);
             delwin(wnd);
             delwin(subwnd);
             wnd=window1(20,30,1,1);
             subwnd=window2(wnd,16,18,1,1);
             wnd2=window1(20,30,1,32);
             subwnd2=window2(wnd2,16,18,1,1);
             win=subwnd; log=0; i=0; j=0;
             output(".",win);
             free(parent);
             parent=malloc(sizeof(char)*2);
             parent[0]='.';
             parent[1]='\0';
             chdir(parent);
             wmove(win,i,j);
             wrefresh(wnd);
             wrefresh(wnd2);
             wrefresh(subwnd);
             wrefresh(subwnd2);

         }
         mvwinstr(win,i,j, dir);
         n=summa(dir);
         if(dir[0]=='_'){
              name=malloc(sizeof(char)*n);
              name=dir;
              redactor(name, n);
              dir_name=malloc(sizeof(char)*n);
              strcpy(dir_name,name);
              if(log==0)
                  output(dir_name, subwnd2);
              else
                  output(dir_name, subwnd);
         }
         c=wgetch(win);
    }while(c!='q');
}
int main(){
    int i,j;
    char name[18];
    name[0]='.';
    name[1]='\0';
    WINDOW *wnd, *wnd2, *win;
    WINDOW *subwnd,*subwnd2;
    initscr();
    cbreak();
    refresh();
    noecho();
    wnd=window1(20,30,1,1);
    subwnd=window2(wnd, 16,18,1,1);
    wnd2=window1(20,30,1,32);
    subwnd2=window2(wnd2,16,18,1,1);
    output(name,subwnd);
    navigation(subwnd,subwnd2);
    delwin(wnd);
    delwin(subwnd);
    delwin(wnd2);
    delwin(subwnd2);
    endwin();
    exit(EXIT_SUCCESS);
}



