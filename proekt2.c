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
                                                                
                                                                
