#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

void  sig_winch(int signo){
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char*)&size);
  resizeterm(size.ws_row, size.ws_col);
}

int main(){
  WINDOW *wnd;
  WINDOW *subwnd;
  WINDOW *subwnd2;
  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  curs_set(0);
  refresh();
  wnd=newwin(20,20,1,1);
  subwnd=derwin(wnd,8,8,1,1);
  box(subwnd,'+','+');
  subwnd2=derwin(wnd,8,8,9,1);
  box(subwnd2,'-','-');
  wprintw(subwnd, "Hello!\n");
  wrefresh(wnd);
  delwin(subwnd);
  delwin(subwnd2);
  delwin(wnd);
  move(9,0);
  printw("By...");
  refresh();
  getch();
  endwin();
  exit(EXIT_SUCCESS);
}
