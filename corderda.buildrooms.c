//BossRoom BuildRooms.c
//By Ava Cordero
//Creates a series of files that hold descriptions of the in-game rooms and 
//	how the rooms are connected.

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

struct Room
{
	char* name;
}

void BuildRoom(char* path, int i)
{
	//You get to pick the names for those files, which should be hard-coded into your program.
	//For example, the directory, if John Smith was writing the program, should be hard-coded
	//	(except for the process id number) as: corderda.rooms.<pid>

	//initialize string buffers for the rooms path and i counter
	char curpath[BUFSIZ];
	char buf[BUFSIZ];
	//cast i into a string
	sprintf(buf, "%d", i);
	//build the current room path from str (curpath+"/room"+i)
	strcpy(curpath, path);
	strcat(curpath,"/room");
	strcat(curpath,buf);
	// printf("New Room Path: %s\n", curpath);

	//create and open new room file for writing
	FILE *fp;
	fp = fopen(curpath,"w");

	//write the base room data to the current room
	char str[BUFSIZ];
	sprintf(str, "%s", "ROOM NAME: ");

	//close file
	fclose(fp);
	// char greet[] = "Hello World, from Room ";
	// printf("%s%d: %s\n", greet, (i+1), curpath);

	return;
}

void BuildRooms()
{
	//The first thing your rooms program must do is generate 7 different room files,
	//	one room per file, in a directory called "<STUDENT ONID USERNAME>.rooms.<PROCESS ID>".

	//In order to create the directory with the process id in the name, we will require
	//	something like:
	char buf[BUFSIZ];
	//Get process ID
    int p_id = getpid();  
    //Create string for file with <STUDENT ONID USERNAME>.rooms.<PROCESS ID>
    char newdir[] = "./corderda.rooms.";
    sprintf(buf, "%d", p_id);
    strcat(newdir,buf);
	int result = mkdir(newdir, 0755);
	printf("Result of mkdir(): %d\n", result);

	for (int i = 0; i < 7; ++i)
	{
		BuildRoom(newdir,i);
	}

	return;
}

int main(int argc, char const *argv[])
{
	BuildRooms();
	return 0;
}