all:	clear main.o
		@gcc -o main b-tree.o main.o 

main.o:	main.c b-tree.c
		@gcc -c b-tree.c main.c -I.

clear:
		@find -name '*.o' | xargs rm -rf {}
		@find -name '*~'  | xargs rm -rf {}

clearAll:
		@find -name '*.o' | xargs rm -rf {}
		@find -name '*~'  | xargs rm -rf {}
		@find -name '*.dat'  | xargs rm -rf {}

run:
		@./main
