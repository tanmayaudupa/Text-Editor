#ifndef LIST_IMPL_
#define LIST_IMPL_

#include "list.h"
#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
/*void list_initialize(LINE *list);
int insert_At_End(LINE *list,char ele);
node* create_Node(char ele);
void display_line(LINE *list);
void destroy_List(LINE *list);
int insert_At_Beg(LINE *list,char ele);
int insert_At_Pos(LINE *list,char ele,int pos);
void page_initialize(PAGE *p);
LINE* insert_line(PAGE *p,int key);
void display_page(PAGE *p);
void destroy_page(PAGE *p);*/

void list_initialize(LINE *list){
	list->head=list->tail=NULL;
	list->count=0;	
}
int insert_At_End(LINE *list,char ele){
	node *newnode=create_Node(ele);
	if(list->head==NULL){
		list->head=newnode;
		list->tail=newnode;
	}
	else{
		list->tail->rlink=newnode;
		newnode->llink=list->tail;
		list->tail=newnode;
	}
	newnode->newline=list;
	(list->count)++;
	return 1;
}
node* create_Node(char ele){
	node *temp=(node*)malloc(sizeof(node));
	temp->ch=ele;
	temp->llink=NULL;
	temp->rlink=NULL;
	return temp;
}

void destroy_List(LINE *list){
	node *temp=list->head;
	if(list->head==NULL) return;
	while(temp->rlink!=NULL){
		temp=temp->rlink;
		free(temp->llink);
	}
	free(temp);
}
int insert_At_Beg(LINE *list,char ele){
	node *newnode=create_Node(ele);
	if(list->head==NULL){
		list->head=newnode;
		list->tail=newnode;
	}
	else{
		newnode->rlink=list->head;
		list->head->llink=newnode;
		list->head=newnode;		
	}
	newnode->newline=list;
	(list->count)++;
	return 1;
}
int insert_At_Pos(LINE *list,char ele,int pos){
	if(list==NULL) return insert_At_Beg(list,ele);
	if(pos>list->count||pos<0)
		return 0;
	else if(pos==0){
		return insert_At_Beg(list,ele);
	}
	else if(pos==list->count){
		return insert_At_End(list,ele);
	}
	else{
		node *temp=list->head;
		int i=0;
		while(temp!=NULL && i!=pos){
			temp=temp->rlink;
			i++;
		}
		node* newnode=create_Node(ele);
		newnode->rlink=temp;
		newnode->llink=temp->llink;
		temp->llink->rlink=newnode;
		temp->llink=newnode;
		newnode->newline=list;
		list->count++;
		return 1;
	}
}
LINE * page_initialize(PAGE *p){
	p->num_lines=0;
	p->first_line=NULL;
	return p->first_line;
	//p->first_line=()
}
LINE* insert_line(PAGE *p,LINE *line,int key,int pos){
	/*if(key>p->num_lines+1){
		return 0;
	}
	else if(key==p->num_lines+1){
		(p->newline)[key-1]=(LINE*)malloc(sizeof(LINE));
		list_initialize((p->newline)[key-1]);
		p->num_lines++;
		return (p->newline)[key-1];
		
	}
	else if(key>=1 && key<=p->num_lines){
		key=key-1;
		for(int i=p->num_lines-1;i>=key;i--){
			(p->newline)[i+1]=(p->newline)[i];

		}
		(p->newline)[key]=(LINE*)malloc(sizeof(LINE));
		list_initialize((p->newline)[key]);
		p->num_lines++;
		return (p->newline)[key];
	}*/
	LINE *temp;
	int y=0,x=0;
	getyx(stdscr,y,x);
	if(key>p->num_lines +1){
		return 0;
	}

	else if(key==p->num_lines){
		if(p->first_line==NULL){
			//printw("Hi");
			p->first_line=(LINE *)malloc(sizeof(LINE));
			list_initialize(p->first_line);
			p->first_line->next=NULL;
			p->first_line->prev=NULL;
			p->num_lines++;
			return p->first_line;
		}
		else{
			temp=(LINE*)malloc(sizeof(LINE));
			temp->next=NULL;
			temp->prev=line;
			list_initialize(temp);
			line->next=temp;
			p->num_lines++;
			//return temp;
		}
	}

	else if(key>=1 && key<=p->num_lines){
		temp=(LINE*)malloc(sizeof(LINE));
		list_initialize(temp);
		temp->next=line->next;
		temp->prev=line;
		line->next->prev=temp;
		line->next=temp;
		p->num_lines++;
		//return temp;
	}
	if(line!=NULL &&pos!=line->count-1){
		node *traverse=line->head;
		int j=0;
		while(j!=pos){j++;traverse=traverse->rlink;}
		temp->head=traverse;
		line->tail=traverse->llink;
		line->count=pos;
		if(traverse!=line->head){
			traverse->llink->rlink=NULL;
			traverse->llink=NULL;
		}
		else{
			//if(line->prev==NULL)p->first_line=temp;
			line->head=NULL;
		}
		traverse=temp->head;
		int counter=0;
		while(traverse!=NULL){
			counter++;
			traverse->newline=temp;
			if(traverse->rlink==NULL)temp->tail=traverse;
			traverse=traverse->rlink;
		}
		temp->count=counter;
		
	}
	if(y!=42)return temp;
	else return line;
}

void destroy_page(PAGE *p){
	LINE *line=p->first_line;
	for(int i=0;i<p->num_lines;i++){
		destroy_List(line);
		LINE *temp=line->next;
		free(line);
		line=temp;
	}
}
void display_page(PAGE *p,int start,int end){
	LINE *line=p->first_line;
	int j=0;
	while(j!=start){
		j++;
		line=line->next;
	}
	for(int i=start;i<end;i++){
		//printw("%d",i+1);
		//printw("%d",i+1);
		//waddch(win,i+1)
		if(line==NULL)return;
		display_line(line);
		line=line->next;
	}
	
}
void display_line(LINE *list){
	node *temp=list->head;
	int x=0,y=0;
	while(temp->rlink!=NULL){
		char ele=temp->ch;
		//addch(ele);
		printw("%c",ele);
		//waddch(win,temp->ch);
		temp=temp->rlink;
	}
	//refresh();
	getyx(stdscr,y,x);
	if(y>=42){
		//move(y,list->count+4);
		//printw("\n");
		refresh();
	}
	else{
		
		//move(y,20);
		//getyx(stdscr,y,x);
		//printw("%d %d",x,y);	
		//move(y+1,0);
		move(y,list->count+4);
		printw("%c",temp->ch);
		refresh();
	}
	//printw("h\n");
	//printw("\n");
}
void display_line_edit(LINE *list, int start,int line_num){
	node *temp=list->head;
	int x=0,y=0;
	while(temp->rlink!=NULL){
		char ele=temp->ch;
		//addch(ele);
		printw("%c",ele);
		//waddch(win,temp->ch);
		temp=temp->rlink;
	}
	//refresh();
	move(line_num,start+1);
	//getyx(stdscr,y,x);		
		//move(y,20);
		//getyx(stdscr,y,x);
		//printw("%d %d",x,y);	
		//move(y+1,0);
	refresh();

}	
void delete_At_Pos(LINE* list,int pos)
{ 	
	node* temp=list->head;
	if( pos > list->count || list->count==0 || pos<0)
		{	return;
	}
	else if(pos==0)
	{ 	
		node* temp=list->head;
		list->head=list->head->rlink;
		free(temp);	
		list->count--;
	}
	else
	{
		int i=0;

		while(i!=pos){
			temp=temp->rlink;
			i++;	
		}
		node* prev=temp->llink;
		prev->rlink=temp->rlink;
		if(temp->rlink!=NULL)
		{	temp->rlink->llink=prev;
		}
		free(temp);
		list->count--;
	}
	
}
/*void copy(char *characters,PAGE page,int old_linenum,int old_pos,int new_linenum,int new_pos){
	int total_size=page.newline[old_linenum]->count-old_pos;
	int i=old_linenum;
	i++;
	while(i<new_linenum){
		total_size+=page.newline[i]->count;
		i++;
	}
	total_size+=new_pos+1;
	characters=(char*)malloc(sizeof(char)*total_size);
	//copy(characters);
	int j=0;
	node *beg=page.newline[old_linenum]->head;
	while(j!=old_pos){beg=beg->rlink;j++;}
	int counter=old_linenum;
	for(int i=0;i<total_size;i++){		
		characters[i]=beg->ch;
		if(beg->ch=='\n'){beg=page.newline[++counter]->head;}
		else beg=beg->rlink;
	}
	paste(characters);
}
void paste(char *characters){
	FILE *fp2=fopen("file.txt","w");
	int i=0;
	while(characters[i]!='\0'){
		fputc(characters[i],fp2);i++;
	}
	fclose(fp2);
}*/
/*void copy(char *characters){
	
}*/
void autocomplete_initialize(auto_tree *ptr_tree){
	ptr_tree->first=NULL;
	int key=1;
	insert_autocomplete(&(ptr_tree->first),"int",&key);
	insert_autocomplete(&(ptr_tree->first),"char",&key);
	insert_autocomplete(&(ptr_tree->first),"float",&key);
	insert_autocomplete(&(ptr_tree->first),"double",&key);
	insert_autocomplete(&(ptr_tree->first),"void",&key);
	//insert_autocomplete(&(ptr_tree->first),"if",&key);
	key=0;
	//insert_autocomplete(&(ptr_tree->first),"i_yu",&key);
}
void insert_autocomplete(character **root,char *word,int *keyword){
	if((*(root))==NULL){
		*(root)=(character*)malloc(sizeof(character));
		(*root)->data=*word;
		(*root)->is_End_Of_String=0;
		if(*keyword){
		(*root)->is_part_of_keyword=1;
		(*root)->length_of_keyword=strlen(word);
		}
		else {
		(*root)->is_part_of_keyword=0;
		(*root)->length_of_keyword=0;
		}
		(*root)->left=(*root)->eq=(*root)->right=NULL;
	}
	if((*word)<(*root)->data){
		insert_autocomplete(&((*root)->left),word,keyword);
	}
	else if((*word)>(*root)->data){
		insert_autocomplete(&((*root)->right),word,keyword);
	}
	else{
		if(*(word+1)){
			insert_autocomplete(&((*root)->eq),word+1,keyword);
		}
		else{
			(*root)->is_End_Of_String=1;
			//return 1;
		}
	}	
}
int search_autocomplete(character *root,char *word,int i,character **buffer,int *flag,int *is_keyword,int to_print){
	if(root==NULL){
		return -1;
	}
	if((*word)<root->data){
		return search_autocomplete(root->left,word,i,buffer,flag,is_keyword,to_print);
	}
	else if((*word)>root->data){
		return search_autocomplete(root->right,word,i,buffer,flag,is_keyword,to_print);
	}
	else{
		buffer[i]=root;
		//printf("%c",root->data);
		if(*(word+1)){
			return search_autocomplete(root->eq,word+1,i+1,buffer,flag,is_keyword,to_print);
		}
		else{
			//*flag=0;
			//else move(x,y-strlen(word));
			if(to_print)print_subtree(root->eq,i+1,buffer,flag,is_keyword);
			else{
				(*is_keyword)=1;
				for(int j=0;j<=i;j++){
					if(buffer[j]->is_part_of_keyword==0){
						(*is_keyword)=0;
				        //return i+1;
					}
				}
				if((*is_keyword)&&(buffer[i]->length_of_keyword==1)){
					//(*is_keyword)=0;
					//printw("Hi");
					return -2;
				}
				(*is_keyword)=0;
			}			
			return i+1;
			
		}
		
	}	
}
void print_subtree(character *root,int index,character **buffer,int *flag,int *is_keyword){	
	if(root){
		//printw("Hi");
		print_subtree(root->left,index,buffer,flag,is_keyword);
		if((*flag)==0)buffer[index]=root;
		if(root->is_End_Of_String==1&&((*flag)==0)){
			//int keyword_flag=1;
			//printf("Hi\n");
			(*is_keyword)=1;
			buffer[index+1]=NULL;
			for(int i=0;i<=index;i++){
				printw("%c",buffer[i]->data);
				if(buffer[i]->is_part_of_keyword==0){
					(*is_keyword)=0;
					//break;
				}
			}
			if((*is_keyword)&&(buffer[index]->length_of_keyword!=1)){
				(*is_keyword)=0;
			}
			int ch=getch();
			noecho();
			if(ch==10){
				*flag=1;
				return;
			}
			int x,y;
			getyx(stdscr,x,y);
			move(x,y-index-1);
		}
		if((*flag)==0)print_subtree(root->eq,index+1,buffer,flag,is_keyword);
		
		if((*flag)==0)print_subtree(root->right,index,buffer,flag,is_keyword);
	}
}
void delete_auto(auto_tree *tree){
	character *root=tree->first;
	delete_auto_recursive(root);
}
void delete_auto_recursive(character* root){
	if(root){
		delete_auto_recursive(root->left);
		delete_auto_recursive(root->eq);
		delete_auto_recursive(root->right);
		free(root);
	}
	
}

char* find_word_to_autocomplete(LINE *curr,int y,char *word){
	node *temp=curr->head;
	int i=0;
	while(i!=y&&temp!=NULL){
		i++;
		temp=temp->rlink;
	}
	//*(*word)=temp->ch;
	i=0;
	*(word)=temp->ch;
	i++;
	temp=temp->llink;
	if(temp){
		while(temp!=NULL&&temp->ch!=' '){
			int j=i-1;
			while(j>=0){
				*(word+j+1)=*(word+j);				
				j--;
			}			
			*(word)=temp->ch;
			i++;
			temp=temp->llink;
		}
		*(word+i)='\0';
		
	}
	return word;	
}





#endif
