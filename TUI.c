//RTEMS Configuration TUI
//Author : Shubham Somani

# include</home/anomaly/TUI/input_provider.h>   //Path needs to be changed according to your system
# include<ncurses.h>
# include<form.h>
# include<menu.h>
# include<stdlib.h>
# include<stdio.h>

//function to check if a substring is present in a complete string or not.
int in(char* complete, char* substring)
{
     int i;
     for(i=0;complete[i]!='\0';i++)
     {
             int flag=1;
             int j;
             for(j=0;substring[j]!='\0';j++)
             {
                     if(substring[j]==complete[i+j])
                     continue;
                     else
                     {
                         flag=0;
                         break;
                     }
             }
             //If substring matches then return the position after the match.
             if(flag)
             return (i+j);
     }
     return 0;
}

int main()
{
  char ** input[100];
  char * default_values[100][100];
  char copy_default_values[100][100][100];
  char * final_values[100][100];
  
  //call to input_provider.h for getting parameters
  int len=return_parameters(input);

  int i=0; 

  /*//loop to check if values have been imported correctly. 
  FILE * Output;
  Output = fopen("output.txt", "a");
  for(i=0;i<len;i++)
  {
       int j=0;
       for(j=0;input[i][j]!="end";j++)
       {
            fprintf(Output, input[i][j]);
	    fprintf(Output, " ");
       }
       fprintf(Output, "\n\n");
  }
  fclose(Output);*/


  // Declaring Menu
  ITEM* items[10000];
  MENU* my_menu;

  //Loop for fetching default values.
  for(i=0;i<len;i++)
  {
	int sec_len=0;
	for(sec_len=0;input[i][sec_len]!="end";sec_len++);

	int j,index=0;
	for(j=4;j<sec_len;j+=6)
	{
		default_values[i][index]=input[i][j];
		if(in(default_values[i][index],"The default value is "))
				{
					char arr[100];
					int pos=in(default_values[i][index],"The default value is ");
					int string_index=0;

					//parsing out default value
					for(string_index=0;((default_values[i][index][pos+string_index]!='\0')&&(default_values[i][index][pos+string_index]!='.')&&(default_values[i][index][pos+string_index]!=','));string_index++)
					arr[string_index]=default_values[i][index][pos+string_index];
					arr[string_index]='\0';

					int j=0;
					for(j=0;arr[j]!='\0';j++)
					{
						default_values[i][index][j]=arr[j];
					}
					default_values[i][index][j]='\0';
				}
				else if(in(default_values[i][index],"This is not defined")||in(default_values[i][index],"This field is not defined"))
				{
					default_values[i][index][0]='\0';
				}
				else
					default_values[i][index][0]='\0';
		index++;
	}
	default_values[i][index]="end";
  }

 //making a copy of the default value array for comparision at the time of header generation
  for(i=0;i<len;i++)
  {
	int j;
	for(j=0;default_values[i][j]!="end";j++)
	{
		int k;
		for(k=0;default_values[i][j][k]!='\0';k++)
		copy_default_values[i][j][k]=(default_values[i][j][k]);
		copy_default_values[i][j][k]='\0';
	}
	copy_default_values[i][j][0]='e';
  }

  //Comes back from other display screen if esc is pressed.
  xyz:;

  int item_len=0;
  char* x="";

  //Assigning elements to be displayed on main menu.
  for(i=0;i<len;i++)
  {
	items[i]=new_item(input[i][0],x);

	/*To print complete list of items in each section
	items[item_len]=new_item(x,input[i][0]);
	item_len++;

	int j;
        for(j=1;j<sec_len;j+=6)
	{
		items[item_len]=new_item(x,input[i][j]);
		item_len++;
	}*/
  }
  
  items[i]=NULL;
  
  //Initializing Curses.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr,TRUE);


  
  //Creating the main menu.
  my_menu=new_menu(items);
  mvprintw(LINES - 6, 0, "RTEMS Configuration Tool");
  mvprintw(LINES - 5, 0, "Use UP, DOWN keys to navigate.");
  mvprintw(LINES - 4, 0, "Press SPACE to go to a section");
  mvprintw(LINES - 3, 0, "Press ESC to come back from a section");
  mvprintw(LINES - 2, 0, "Press G or g to generate the header file!");
  post_menu(my_menu);
  refresh();

  int c;
  int count=0;
  FILE * Output;
   
   while((c = getch()) != KEY_F(1))
   {   
 c=tolower(c);
     switch(c)
     {
        //In case down is pressed.
        case KEY_DOWN:
             menu_driver(my_menu, REQ_DOWN_ITEM);
             break;

        //In case up is pressed.
        case KEY_UP:
             menu_driver(my_menu, REQ_UP_ITEM);
             break;
		//If user presses 'g' or 'G' then generate header.
		case 'g':
			Output = fopen("header.h", "w");
		//Loop to write those values which have been explicitly changed by the user.
			int x,y,z;
			for(x=0;x<len;x++)
			{
				for(y=0;default_values[x][y]!="end";y++)
				{
					for(z=0;default_values[x][y][z]!='\0';z++)
					{
						if(default_values[x][y][z]!=copy_default_values[x][y][z])
						{
							fprintf(Output, "# define ");
							fprintf(Output, (char*)(input[x][1+y*6]));
							fprintf(Output, " ");
							fprintf(Output, default_values[x][y]);
							fprintf(Output, "\n");
							break;
						}
					}
				}
			}
			fclose(Output);

			clear();
			mvprintw(2,0,"Header File Generated!");
			mvprintw(4,0,"Press ESC to go back to main menu");
			int ch1;
			while((ch1 = getch()) != KEY_F(1))
			{
	   if(ch1==27)
				goto xyz;
			}


		// If user presses space then he will go to next screen where he can modify options particular to that field.
		case ' ':
			initscr();
            cbreak();
			noecho();
			keypad(stdscr, TRUE);
			clear();

			int macro_index,selected_index;
			selected_index=0;

			//get current selected item
			const char* macro_name=item_name(current_item(my_menu));

			for(macro_index=0;macro_index<len;macro_index++)
			{
				// If item matches then we can get the index
				if(!strcmp(macro_name, input[macro_index][0]))
				{
					selected_index=macro_index;
					break;
				}
			}

			//calculating size of fields required
        		for(i=0;input[selected_index][i]!="end";i++);

			int section_length=i;
        		int field_length=i/6;
			FIELD *field[field_length+10];
			FORM *form;
			int spacing=1;
			//Dynamically creating fields depending upon section
			for(i=0;i<field_length;i++)
			{
				field[i]= new_field(1, 30, spacing, 40, 0, 0);
				spacing+=2;
				set_field_buffer(field[i],0,default_values[selected_index][i]);

			}
  
			for(i=0;i<field_length;i++)
			{
				set_field_back(field[i], A_UNDERLINE);
				field_opts_off(field[i], O_AUTOSKIP);
				field_opts_off(field[i], O_STATIC);
				field_opts_on(field[i], O_EDIT);
			}

			field[field_length]=NULL;
			// Creating a form with the above declared fields.
			form=new_form(field);
  			post_form(form);
  			refresh();
			
			spacing=1;
			for(i=1;i<section_length;i+=6)
			{
			    mvprintw(spacing,0,input[selected_index][i]);
			    spacing+=2;
   			    refresh();
			}

			int ch;
  			while((ch = getch()) != KEY_F(1))
			{ 
                      
				// If user presses esc key then he will go back to the main menu.
		              if(ch==27)
				{
					clear();
					for(i=0;i<field_length;i++)
					{

						char* buffer_contents=field_buffer(field[i],0);
						char buffer_arr[100];
						int buf_len=0;

						for(buf_len=0; (buffer_contents[buf_len]!='\0')&&((buffer_contents[buf_len]!=' ')||(buffer_contents[buf_len+1]!=' '));buf_len++)
						{
							default_values[selected_index][i][buf_len]=buffer_contents[buf_len];
						}
						default_values[selected_index][i][buf_len]='\0';
					}
					break;
				}
		
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
					case KEY_DC:
						form_driver(form, REQ_DEL_CHAR);
						break;

                    case KEY_BACKSPACE:
						if (form_driver(form, REQ_PREV_CHAR) == E_OK) {form_driver(form, REQ_DEL_CHAR);
						}
						break;

                    case KEY_LEFT:
						form_driver(form, REQ_PREV_CHAR);
						break;

                    case KEY_RIGHT:
						form_driver(form, REQ_NEXT_CHAR);
						break;

                    case KEY_HOME:
						form_driver(form, REQ_BEG_FIELD);
						break;

                    case KEY_END:
						form_driver(form, REQ_END_FIELD);
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

			initscr();
			cbreak();
			noecho();
			refresh();
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
