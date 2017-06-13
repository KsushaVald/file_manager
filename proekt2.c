#include <stdio.h>
#include <dirent.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

void output(char *name, WINDOW*win){
   DIR *dp;
   dp=opendir(name);
   struct dirent *rez;
   wclear(win);
   while((rez=readdir(dp))!=NULL){
      if(rez->d_type==DT_DIR)
        wprintw(win,"_");
        wprintw(win,rez->d_name);
        wprintw(win, "\0");
        wprintw(win,"\n");
   }
   wrefresh(win);
   closedir(dp);
}

int summa(char* name){
 int sch=0, i=0;
 while(name[i]!='\0'){
  sch++;
 }
 return sch;
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
   char c; int i=0, j=0, log=0, n;
   WINDOW *win=subwnd;
   char im[18];
   wmove(win,i,j);
   mvwinstr(win,i,j, im);
    /* if(im[0]=='_'){
     redactor(im);
     output(im, win);
   }*/
   c=wgetch(win);
   while(c!='q'){
      char dir[18];
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
      }
      if(c=='a'){
         win=subwnd;
         i=0; j=0; log=1;
         wmove(win,i,j);
      }
     mvwinstr(win,i,j,dir);
     char *name;
     n=summa(dir);
     name=malloc(sizeof(char)*n);
     if(name[0]=='_'){
         redactor(name, n);
      /*if(log==1)
             output(name, subwnd2);
      else
             output(name, subwnd);*/
      }
      wprintw(subwnd2, name);
      wrefresh(subwnd2);
   }
}

int main(){
  int i,j;
  char name[18];
  name[0]='.';
 // name[1]='.';
  name[1]='\0';
  WINDOW *wnd, *wnd2, *win;
  WINDOW *subwnd,*subwnd2;
  initscr();
  cbreak();
  refresh();
  noecho();
  wnd=newwin(20,30,1,1);
  wnd2=newwin(20,30,1,32);
  box(wnd, '|','-' );
  box(wnd2,'|','-');
  subwnd=derwin(wnd,16,18,1,1);
  subwnd2=derwin(wnd2,16,18,1,1);

  output(name,subwnd);

  wrefresh(wnd);
  wrefresh(subwnd);
  wrefresh(wnd2);
  wrefresh(subwnd2);
  refresh();

  navigation(subwnd, subwnd2);

  delwin(subwnd);
  delwin(subwnd2);
  delwin(wnd);
  delwin(wnd2);
  endwin();
  exit(EXIT_SUCCESS);
}

