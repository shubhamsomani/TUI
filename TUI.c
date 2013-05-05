//RTEMS Configuration TUI
//Author : Shubham Somani

# include</home/anomaly/TUI/input_provider.h>   //Path needs to be changed according to your system
# include<ncurses.h>
# include<form.h>
# include<menu.h>

int main()
{
  char ** input[50];
  
  //call to input_provider.h for getting parameters
  int len=return_parameters(input);

  int i=0; 

  //loop to check if values have been imported correctly. 
  /*for(i=0;i<len;i++)
  {
       int j=0;
       for(j=0;input[i][j]!="end";j++)
       {
            printf("%s",input[i][j]);
       }
       printf("\n");
  }*/
 

  // Declaring Menu
  ITEM* items[len+1];
  MENU* my_menu;

  char* x="->";

  //Assigning elements to be displayed on main menu.
  for(i=0;i<len;i++)
  {
	items[i]=new_item(x,input[i][0]);
  }
  
  items[len]=NULL;
  
  xyz:
  //Initializing Curses.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr,TRUE);
  
  //Creating the main menu.
  my_menu=new_menu(items);
  mvprintw(LINES - 2, 0, "RTEMS Configuration Tool");
  post_menu(my_menu);
  refresh();
  
  int c;
  int count=0;

   
   while((c = getch()) != KEY_F(1))
   {   
        switch(c)
	    {
         //In case down is pressed.	
         case KEY_DOWN:
	        menu_driver(my_menu, REQ_DOWN_ITEM);
			count++;
			break;
        //In case up is pressed	
		case KEY_UP:
			menu_driver(my_menu, REQ_UP_ITEM);
			count--;
			break;
		// If user presses space then he will go to next screen where he can modify options particular to that field.
		case ' ':
			initscr();
            cbreak();
			noecho();
			keypad(stdscr, TRUE);
                        
            //calculating size of fields required
            for(i=0;input[count][i]!="end";i++)
            {}

            int section_length=i;
            int field_length=i/3;

			FIELD *field[field_length+1];
            FORM *form;
                        
			int spacing=1;
			
			//Dynamically creating fields depending upon section
            for(i=0;i<field_length;i++)
			{
                           field[i]= new_field(1, 30, spacing, 40, 0, 0);
                           spacing+=2;
			}
  
			for(i=0;i<field_length;i++)
			{
			   set_field_back(field[i], A_UNDERLINE);
  			   field_opts_off(field[i], O_AUTOSKIP);
			}

			field[field_length]=NULL;

            // Creating a form with the above declared fields.
			form=new_form(field);
  			post_form(form);
  			refresh();
			
			spacing=1;
			for(i=1;i<section_length;i+=3)
			{
			    mvprintw(spacing,0,input[count][i]);
			    spacing+=2;
   			    refresh();
			}

			int ch;
  			while((ch = getch()) != KEY_F(1))
			{ 
                      
                        // If user presses esc key then he will go back to the main menu.
                		if(ch==27)
                		break;
		
				switch(ch)
	 			{
					case KEY_DOWN:
						form_driver(form, REQ_NEXT_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					case KEY_UP:
						form_driver(form, REQ_PREV_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					default:	
						form_driver(form, ch);
						break;
                                }
                 	}

  			//Freeing memory of form
  			unpost_form(form);
  			free_form(form);

  			for(i=0;i<field_length;i++)
  			free_field(field[i]);

			endwin();
			//goes back to main menu if escape is pressed
            goto xyz;
   	    } 
       }

  //Freeing memory of main menu.
  for(i=0;i<len;i++)
     free_item(items[i]);

  free_menu(my_menu);
  endwin();
  

}
