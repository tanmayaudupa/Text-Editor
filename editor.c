#include "list.h"
#include<stdio.h>
#include<string.h>
#include<ncurses.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>

typedef struct mousevents{
	int y,x;
}mousevents;
void edit(PAGE *page,char *filename);
int main(){
	printf("WELCOME TO HYPER TEXT EDITOR\nThe following depict the basic operations:\n1.Save Ctrl+S\n2:Newline Enter\n3:To enable/disable autocomplete press F2\n4:To see auto complete suggestions press F3\n5:Press up and down arrow keys to scroll up and down\n6:Press Tab for identation\n7:Use backspace to delete characters\nPRESS ANY CHARACTER TO CONTINUE\nNote:Text editor only works with the command prompt in full screen\n");
	getch();
	printf("Enter filename to open\n");
	char str[40];
	fgets(str,35,stdin);
	str[strlen(str)-1]='\0';
	int count=0;
	FILE *fp=fopen(str,"r");
	char textStr[50];
	//fgets(textStr,199,fp);
	/*for(int i=0;i<PAGE_SIZE;i++){
		text[i]=(char*)malloc(sizeof(char)*40);
	}*/
	
	//printw("%s\n",str);
	//refresh();
	//printf("%s",str);

	char ele='i';
	int x=0,y=0;
	PAGE page;
	LINE *line=page_initialize(&page);
	mousevents store_coor;
	if(fp!=NULL){
		initscr();
		//char text[200];
		count=-1;
		while(fgets(textStr,199,fp)!=NULL){
			getyx(stdscr,x,y);
			if(line==NULL)
			line=insert_line(&page,line,count+1,0);
			else line=insert_line(&page,line,count+1,line->count-1);
			for(int i=0;textStr[i]!='\0';i++){
				if(textStr[i]=='\t'){
					int j=4;
					while(j>0){ insert_At_End(line,' ');j--;}
				}
				else{
					insert_At_End(line,textStr[i]);
				}
				//printw("%c",textStr[i]);
				//getyx(stdscr,y,x);
				//move(y,x+1);
			}
			count++;
			//getyx(stdscr,y,x);
			//move(y+1,0);
			//refresh();
		}
		display_page(&page,0,43);
		refresh();
		fclose(fp);
		edit(&page,str);
	}

	else{
		char choice;
		printf("Do you want to create a new file?\n");
		scanf("%c",&choice);
		if(toupper(choice)=='Y'){
			edit(&page,str);
		}
	}

	endwin();
	return 0;
}
void edit(PAGE *page,char *filename){
initscr();
scrollok(stdscr,TRUE);
keypad(stdscr,TRUE);
cbreak();
int ch;
int x=0,y=0;
int flag=1;
raw();
cbreak();
//keypad(stdscr,TRUE);
noecho();
//idlok(stdscr,true);
//scrollok(stdscr,true);
getyx(stdscr,x,y);
mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION ,NULL);
MEVENT m_event;
int beg=0,end=43;
LINE *curr=page->first_line;
move(0,0);
auto_tree sequence;
		int check=0;
autocomplete_initialize(&sequence); //Autocomplete
//char buffer[50];
//search_autocomplete(sequence.first,"i",0,buffer);
//getch();
//delete_auto(&sequence);
int counter_num_spaces=0;
int auto_complete_flag=0;
int is_keyword=0;
char word[50];
character* buffer[50];
char temp_keyword[7];
int key_pointer=0;
if(page->first_line==NULL)printw("Enter the enter key before you start typing");
while(1)
{

	ch=getch();
	if(page->first_line==NULL&&ch!=10)continue;
	if(strcmp(keyname(ch),"^S")==0)break;
	else if(ch=='`')
	{
		x+=1;
		y=0;
		if(curr->next!=NULL)curr=curr->next;
		move(x,y);
	}
	else if(ch==KEY_UP)
	{
	getyx(stdscr,x,y);
	//x+=1;
	if(x>0){
		x-=1;
		if(curr->prev!=NULL){
			curr=curr->prev;
			move(x,y);
		}
	}
	else{
		if(curr->prev!=NULL){
			curr=curr->prev;
			beg-=1;
			end-=1;
			clear();
			move(0,0);
			display_page(page,beg,end);
			move(0,0);
			refresh();
		}			
	}
	}
	else if(ch==KEY_DOWN)
	{
	getyx(stdscr,x,y);
		if(x<42){
		x+=1;
		if(curr->next!=NULL){
			curr=curr->next;
			move(x,y);
		}
	}
	else{
		if(curr->next!=NULL){
			curr=curr->next;
			beg+=1;
			end+=1;
			clear();
			move(0,0);
			display_page(page,beg,end);
			refresh();	
		}	
	}
	}
else if(ch==KEY_LEFT)
{getyx(stdscr,x,y);
y-=1;
move(x,y);
}
else if(ch==KEY_RIGHT)
{getyx(stdscr,x,y);
y+=1;
move(x,y);
}
else if(ch==127)//Backspace
{
getyx(stdscr,x,y);
y-=1;
move(x,y);
delete_At_Pos(curr,y);
delch();
}
else if(ch==KEY_HOME)
{
break;
}
else if(ch==KEY_MOUSE){
	refresh();
	if(getmouse(&m_event)==OK){

		//refresh();
		if(m_event.bstate){

			//refresh();
			switch(m_event.bstate){
				case BUTTON1_CLICKED:
					getyx(stdscr,x,y);

					if(m_event.y>x&&m_event.y<page->num_lines){
						for(int i=0;i<m_event.y-x;i++){
							curr=curr->next;
							if(curr==NULL){move(x,y);break;}
						}
						move(m_event.y,m_event.x);
					}
					else if(m_event.y<x){
						for(int i=0;i<x-m_event.y;i++){
							curr=curr->prev;
							if(curr==NULL){move(x,y);break;}
						}
						move(m_event.y,m_event.x);
					}
					break;	
					
			}
		}
		else printw("Not working\n");
	}
		else printw("Not key_mouse");
}
else if(ch==10){  //Enter key
	getyx(stdscr,x,y);
	if(page->first_line==NULL||y<curr->count){
	int offset;
	if(curr==NULL)offset=x;
	else offset=x+1;
	curr=insert_line(page,curr,offset,y);
	if((curr->prev!=NULL && curr->prev->tail==NULL)||(curr->prev!=NULL)&&(curr->prev->tail->ch!='\n'))insert_At_End(curr->prev,'\n');
	if(curr->tail!=NULL&&curr->tail->ch!='\n')insert_At_End(curr,'\n');
	if(curr->tail==NULL)insert_At_End(curr,'\n');
	clear();
	display_page(page,beg,end);
	move(offset,0);
	}
}
else if(ch==KEY_F(2)){
	if(auto_complete_flag)auto_complete_flag=0;
	else auto_complete_flag=1;
}
else if(ch==KEY_F(3)){
	if(curr!=NULL&&auto_complete_flag&&y<curr->count){
		getyx(stdscr,x,y);
		int word_chosen=0;
	//printw("%s",find_word_to_autocomplete(curr,y,word));
		strcpy(word,find_word_to_autocomplete(curr,y,word));
		move(x,y-strlen(word)+1);
		int i=search_autocomplete(sequence.first,word,0,buffer,&word_chosen,&is_keyword,1);
	//int i=0;
	//move(x,y);
		if(i!=-2&&i!=-1){  //if not a keyword
			while(buffer[i]!=NULL){
				y++;
				insert_At_Pos(curr,buffer[i]->data,y);
				i++; 
			}
	//move(x,0);
			clear();
	//move(0,0);
			display_page(page,beg,end);
			move(x,y);
			word[0]='\0';
			buffer[0]=NULL;
		}
		else{
			move(x,y);
		}
	}
}
else if(ch==9){//Tab key
	if(curr){
		getyx(stdscr,x,y);
		for(int k=0;k<4;k++){
			insert_At_Pos(curr,' ',y);
			y=y+1;
		}
		move(x,0);
		display_line_edit(curr,y-1,x);
		//move(x,y+4);
	}
} 
else
{
getyx(stdscr,x,y);
move(x,y);
/*char buffer[50];
search_autocomplete(sequence.first,ch,0,buffer);*/
	if(curr!=NULL){
		if(insert_At_Pos(curr,ch,y)){
			getyx(stdscr,x,y);
			move(x,0);
			display_line_edit(curr,y,x);
		}
		if(is_keyword&&(ch==' '||ch==';'||ch==','||ch=='('||ch==')'||ch=='{'||ch=='}'||ch=='['||ch==']'||ch=='=')){
			counter_num_spaces++;
			int word_chosen=0;
			find_word_to_autocomplete(curr,y,word);
			word[strlen(word)-1]='\0';
			int status=0;
			int check=search_autocomplete(sequence.first,word,0,buffer,&word_chosen,&status,0);
			//printw("%d",check);
			if(check!=-2){
			if(strcmp(word," ")&&strcmp(word,";")&&strcmp(word,",")&&strcmp(word,"\n")){
				is_keyword=0;
				insert_autocomplete(&(sequence.first),word,&is_keyword);
				counter_num_spaces=0;

			}
			}
			word[0]='\0';
			buffer[0]=NULL;
		}
		else if(!is_keyword){
				if(isalpha(ch))
				{	//printw("hello");
					temp_keyword[key_pointer]=ch;
					key_pointer++;
					temp_keyword[key_pointer]='\0';
					if(key_pointer>=3&&key_pointer<=6){
						//printw("hello");				
						int word_chosen=0;
						int status=0;
						check=search_autocomplete(sequence.first,temp_keyword,0,buffer,&word_chosen,&status,0);
						if(check==-2){
							is_keyword=1;
							temp_keyword[0]='\0';
							key_pointer=0;	
							//buffer[0]=NULL;
						}
				
					}
					else if(key_pointer>=7){
						temp_keyword[0]='\0';
						key_pointer=0;	
						//buffer[0]=NULL;
					}

					//printw("%s",temp_keyword);	
				}
				else{

					/*temp_keyword[0]='\0';
					key_pointer=0;	*/
					temp_keyword[0]='\0';
					key_pointer=0;
				}
					buffer[0]=NULL;
				}
		else{
			if(counter_num_spaces<1){//printw("Hi");
				is_keyword=0;
			}
		}
		refresh();
		//addch(ch);
		//y++;
	}
	}
}


refresh();
FILE *fp2=fopen(filename,"w");
curr=page->first_line;
while(curr!=NULL){
	node *temp=curr->head;
	for(int i=0;i<curr->count;i++){
		fputc(temp->ch,fp2);
		temp=temp->rlink;	
	}
	curr=curr->next;
}
fclose(fp2);
//clear();
delete_auto(&sequence);
destroy_page(page);
}
