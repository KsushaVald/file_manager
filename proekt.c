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
  int i;
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
  WINDOW *wnd;
  WINDOW *subwnd;
  initscr();
  cbrerk();
  refresh();
  wnd=newwin(20,30,2,2);
  box(wnd, '|','-' );
  subwnd=derwin(wnd,10,20,2,2);
  for(i=0; i<scht; i++){
    direct[i]=malloc(sizeof(char)*256);
    rez=readdir(dp);
    direct[i]=rez->d_name;
    printf("%s\n", direct[i]);
  }
  wrefresh(wnd);
  delwin(subwnd);
  delwin(wnd);
  refresh();
  getch();
  endwin();
  exit(EXIT_SUCCESS);
 closedir(dp);
}
