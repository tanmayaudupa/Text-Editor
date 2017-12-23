a.out:list_impl.o editor.o 
	gcc list_impl.o editor.o -lncurses
list_impl.o: list_impl.c list.h
		gcc -c list_impl.c 
editor.o: editor.c list.h 
		gcc -c editor.c 
