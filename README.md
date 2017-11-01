adventure
	A text adventure game in C inspired by the original game Colossal Cave Adventure
	By Ava Cordero

Overview

To initialize and run the game with newly generated room data, run either "./build" or equivalently, "make default && ./buildrooms && ./adventure && make clean" for a fully automated execution and clean up.

The game is split up into two programs written in raw C (no C99 or later was allowed, as a programming excercise) . The first program is contained in a file named "buildrooms.c", which when compiled with the same name (minus the extension) and run creates a series of files that hold descriptions of the in-game rooms and how the rooms are connected.

The second program is called "adventure.c" and when compiled with the same name (minus the extension) and run provides an interface for playing the game using the most recently generated rooms.

Set global constant VERBOSE to 1 in *either* .c file for trace statments (e.g. #define VERBOSE 1)
Set NUM_ROOMS in *both* .c files to choose the number of room files in the adventure (e.g. #define NUM_ROOMS 20)
Set options array strings and/or first index quantifier (e.g. options[2][32] = {"Maze","Custom Room"};) in *buildrooms.c:main()* to set the rooms that the game will use to generate a random adventure

In the game, the player will begin in the "starting room" and will win the game automatically upon entering the "ending room", which causes the game to exit, displaying the path taken by the player.

Example Execution:

	CURRENT LOCATION: Home
	POSSIBLE CONNECTIONS: Monster's Hut, Dark Dungeon, Very Lost, Ghost Town, Poppy Field, Dragon Lair.
	WHERE TO? >Monster's Hut

	CURRENT LOCATION: Monster's Hut
	POSSIBLE CONNECTIONS: Ghost Town, Home, Dark Dungeon, Poppy Field, Dragon Lair.
	WHERE TO? >Dragon Lair

	CURRENT LOCATION: Dragon Lair
	POSSIBLE CONNECTIONS: Dark Dungeon, Monster's Hut, Home.
	WHERE TO? >Home

	CURRENT LOCATION: Home
	POSSIBLE CONNECTIONS: Monster's Hut, Dark Dungeon, Very Lost, Ghost Town, Poppy Field, Dragon Lair.
	WHERE TO? >Poppy Field

	YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!
	YOU TOOK 4 STEP(S). YOUR PATH TO VICTORY WAS:
	Monster's Hut
	Dragon Lair
	Home
	Poppy Field

During the game, the player can also enter a command that returns the current time and creates a local file currentTime.txt containing the system time the command was executed.



