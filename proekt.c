#include <stdio.h>
#include <malloc.h>
#include <dirent.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
int main(){
 DIR*dp;
  int i,j, k;
  int scht=0;
  char **direct;
  struct dirent *rez;
  dp=opendir(".");
   while((rez=readdir(dp))!=NULL){
     scht++;
  }
 // printf("%d\n", scht);
  closedir(dp);
  dp=opendir(".");
  direct=malloc(scht*sizeof(char*));
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
  subwnd=derwin(wnd,8,18,1,1);
  subwnd2=derwin(wnd2,8,18,1,1);
  i=0; j=0;
  wmove(subwnd,i,j);
  wrefresh(wnd);
  wrefresh(subwnd);
  wrefresh(wnd2);
  wrefresh(subwnd2);
  refresh();
  for(k=0; k<scht; k++) {
    direct[i]=malloc(sizeof(char)*256);
    rez=readdir(dp);
    direct[i]=rez->d_name;
    wprintw(subwnd, rez->d_name);
    wprintw(subwnd,"\n");
  }
  char c;  win=subwnd;
  c=wgetch(win);
  while(c!='q'){
   char name[10];
     c=wgetch(win);
     if(c=='w')
       wmove(win,--i,j);
     if(c=='s')
       wmove(win,++i,j);
     if(c=='d'){
       win=subwnd2;
       wmove(win,i,j);
     }
     if(c=='a'){
       win=subwnd;
       wmove(win,i,j);
    }
   mvwinstr(win,i,j,name);
   dp=opendir(name);
  }
  delwin(subwnd);
  delwin(subwnd2);
  delwin(wnd);
  delwin(wnd2);
  endwin();
  exit(EXIT_SUCCESS);
  closedir(dp);
}
