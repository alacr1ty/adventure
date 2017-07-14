//BossRoom Adventure.c
//By Ava Cordero
//Provides an interface for playing the game using the most recently generated rooms.
//In the game, the player will begin in the "starting room" and will win the game
//	automatically upon entering the "ending room", which causes the game to exit,
//	displaying the path taken by the player.
//During the game, the player can also enter a command that returns the current time
// -- this functionality utilizes mutexes and multithreading.
#include <stdio.h>

void Adventure()
{
	char greet[] = "Hellow der!";
	printf("%s\n", greet);
	return;
}

int main(int argc, char const *argv[])
{
	Adventure();
	return 0;
}