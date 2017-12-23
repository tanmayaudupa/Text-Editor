#include<ncurses.h>
#ifndef LIST_H_
#define LIST_H_

#define PAGE_SIZE 300
#define LINE_SIZE 200
typedef struct list LINE;
typedef struct node{
	struct node *llink;
	struct node* rlink;
	LINE* newline;
	char ch;
}node;

typedef struct list{
	node *head;
	node *tail;
	int count;
	int line_num;
	LINE *next;
	LINE *prev;
}LINE;
/*typedef struct temp_list{
	node *head;
	node *character;
}temp_list;*/
typedef struct page{
	LINE *first_line;
	int num_lines;
}PAGE;

typedef struct character{  //for autocomplete
	char data;;
	int is_End_Of_String;
	int is_part_of_keyword;
	int length_of_keyword;
	struct character *left,*eq,*right;
}character;

typedef struct auto_tree{
	character *first;
}auto_tree;
void list_initialize(LINE *list);
int insert_At_End(LINE *list,char ele);
node* create_Node(char ele);
void display_line(LINE *list);
void destroy_List(LINE *list);
int insert_At_Beg(LINE *list,char ele);
int insert_At_Pos(LINE *list,char ele,int pos);
LINE * page_initialize(PAGE *p);
LINE* insert_line(PAGE *p,LINE *line,int key,int ch);
void display_page(PAGE *,int,int);
void destroy_page(PAGE *p);
void display_line_edit(LINE *list, int start,int line_num);
void delete_At_Pos(LINE* list,int pos);
void paste(char *characters);
void copy(char *characters,PAGE page,int old_linenum,int old_pos,int new_linenum,int new_pos);
void print_subtree(character *root,int index,character **buffer,int *,int *);
int search_autocomplete(character *root,char *word,int i,character **,int *,int*,int);
void insert_autocomplete(character **root,char *word,int *);
void autocomplete_initialize(auto_tree *ptr_tree);
void delete_auto(auto_tree *tree);
void delete_auto_recursive(character* root);
char* find_word_to_autocomplete(LINE *,int,char *);
#endif
