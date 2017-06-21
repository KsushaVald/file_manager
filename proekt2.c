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

void  sig_winch(int signo){
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char*)&size);
  resizeterm(size.ws_row, size.ws_col);
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
 FILE* fd; char c; int n=0; char stroka[64];
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
    if(c=='e')
        fileredactor(neww, i, j);
  }
    j=0; c='\n';
    fd=fopen(name,"w");
 for(i=0; i<24; i++){
    mvwinstr(neww,i,j, stroka);
    fwrite(stroka,sizeof(char)*64,1,fd);
    fwrite(&c,sizeof(char),1,fd);
 }
 fclose(fd);

}

void windowfile(char *name, char *parent){
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
   if(dp!=NULL){
     while((rez=readdir(dp))!=NULL){
      if(rez->d_type==DT_DIR)
        wprintw(win,"_");
      wprintw(win,rez->d_name);
      wprintw(win,"|");
      wprintw(win,"\n");
     }
    }
   wrefresh(win);
   closedir(dp);
}

int summa(char *dir){
 int  i=0;
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
   WINDOW *win=subwnd;  WINDOW *wnd, *wnd2;
   char dir[256]; char *name; char *parent; char *dir_name;
   parent=malloc(sizeof(char)*2);
   parent[0]='.';
   parent[1]='\0';
   chdir(parent);
   wmove(win,i,j);
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
   while(c!='q'){
      c=wgetch(win);
      if(c=='w'){
         wmove(win,--i,j);
      }
      if(c=='s'){
         wmove(win,++i,j);
      }
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
      mvwinstr(win,i,j,dir);
      n=summa(dir);
      if(dir[0]=='_'){
        name=malloc(sizeof(char)*n);
        name=dir;
        redactor(name, n);
        dir_name=malloc(sizeof(name));
        strcpy(dir_name,name);
        if(log==0)
            output(dir_name, subwnd2);
         else
            output(dir_name, subwnd);
      }
      else{
         if(c=='e'){
           name=malloc(sizeof(char)*(n+1));
           name=dir;
           name[n]='\0';
           delwin(wnd);
           delwin(subwnd);
           delwin(wnd2);
           delwin(subwnd2);
           windowfile(name, parent);
           wnd=window1(20,30,1,1);
           subwnd=window2(wnd,16,18,1,1);
           wnd2=window1(20,30,1,32);
           subwnd2=window2(wnd2,16,18,1,1);
           output(parent,subwnd);
           wrefresh(wnd);
           wrefresh(wnd2);
           wrefresh(subwnd);
           wrefresh(subwnd2);
      }
    }
  }
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

