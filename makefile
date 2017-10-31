default: clean rooms adventure

rooms:
	gcc -g -o corderda.buildrooms corderda.buildrooms.c -coverage -Wall

adventure:
	gcc -g -o corderda.adventure corderda.adventure.c -coverage -Wall

adventurebak:
	gcc -g -o corderda.adventure.bak corderda.adventure.bak.c -coverage -Wall
		
clean:
	rm corderda.buildrooms corderda.buildrooms.gcda corderda.buildrooms.gcno corderda.adventure corderda.adventure.gcda corderda.adventure.gcno ; rm -rf ./corderda.rooms.*/ ./corderda.buildrooms.dSYM/ ./corderda.adventure.dSYM/
