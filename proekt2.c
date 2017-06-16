#include <stdio.h>
#include <dirent.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <fcntl.h>
#include <string.h>

/*void openfile(char *name, WINDOW*neww){
 int fd; char c;
 fd=open(name,O_RDONLY);
  read(fd,&c,sizeof(char));
 while(c!=EOF){
  wprintw(neww,c);
  read(fd,&c,sizeof(char));
 }
 wrefresh(neww);
 close(fd);
}*/

void windowfile(char *name){
  WINDOW *neww;
  initscr();
  cbreak();
  refresh();
  neww=newwin(24,64,0,0);
 // openfile(name, neww);
  wrefresh(neww);
  delwin(neww);
  getch();
  endwin();
  exit(EXIT_SUCCESS);
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
   char c; int i=0, j=0, log=0, n;
   WINDOW *win=subwnd;
   char dir[18]; char *name; char *parent;
   parent=malloc(sizeof(char)*2);
   parent[0]='.';
   parent[1]='\0';
   wmove(win,i,j);
   mvwinstr(win,i,j, dir);
   n=summa(dir);
   name=malloc(sizeof(char)*n);
   name=dir;
   if(dir[0]=='_'){
       redactor(name, n);
     if(log==0)
        output(name, subwnd2);
     else

        output(name, subwnd);
   }
   else{
    if(c=='e'){
     name[n-1]='\0';
     windowfile(name);
    }
   }
   c=wgetch(win);
   while(c!='q'){
      chdir(parent);
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
         parent=malloc(sizeof(name));
         strcpy(parent,name);         
      }
      if(c=='a'){
         win=subwnd;
         i=0; j=0; log=0;
         wmove(win,i,j);
         free(parent);
         parent=malloc(sizeof(name));
         strcpy(parent,name);         
      }
      mvwinstr(win,i,j,dir);
      n=summa(dir);
      //free(name);
      name=malloc(sizeof(char)*n);
      name=dir;
      if(dir[0]=='_'){
        redactor(name, n);
        if(log==0)
           output(name, subwnd2);
         else
            output(name, subwnd);
      }
      else{
         if(c=='e'){
           name[n-1]='\0';
           windowfile(name);
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

