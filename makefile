default: clean rooms adventure

rooms:
	gcc -g -o buildrooms buildrooms.c -coverage -Wall

adventure:
	gcc -g -o adventure adventure.c -coverage -Wall
		
clean:
	rm buildrooms buildrooms.gcda buildrooms.gcno adventure adventure.gcda adventure.gcno ; rm -rf ./rooms.*/ ./buildrooms.dSYM/ ./adventure.dSYM/
