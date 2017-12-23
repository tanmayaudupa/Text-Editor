HYPER TEXT EDITOR 
The hyper text editor is a text editor which has the basic operations of a text editor .
The text editor is built on three structures - Node which stores each character typed,a line
which stores the address of the first node and a PAGE structure which stores the address of the first line.
Every node points to the next node and the prev node ,every line also points to the next and the previous lines.
The text editor ahs the following operations
	1)Read an exisiting File
	2)Open a new file to write .
	3)Save a new written file.(ctrl + s)
	4)Editing a file
		i)write characters into the editor.
		ii)Enter key will go to the next line
		iii)Backspace will delete the character
		iv)Tab will indent the line
		v)Arrow keys to move around with the cursor
		vi)scroll bar operations
	5)Autocomplete operation on ketwords and identifiers(f2 to enable and f3 to give suggestions)
To display the text editor we have used ncurses library of c . It converts the whole terminal into x and y coordinates.
The interface is done using ncurses.
A ternary search tree has been used for autocomplete which already has keywords like int,float,double,char and void 
in it . All the other identifiers will be added to the tree for autocomplete.

RUN THE EDITOR.C FILE USING GCC COMPILER TO USE THE TEXT EDITOR.
