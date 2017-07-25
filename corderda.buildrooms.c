//BossRoom BuildRooms.c
//By Ava Cordero
//Creates a series of files that hold descriptions of the in-game rooms and 
//	how the rooms are connected.

//Some pseudocode snippets adapted from 
//	oregonstate.instructure.com/courses/1638966/pages/2-dot-2-program-outlining-in-program-2

//TASKS PENDING:
//	Duplicate Room Name Bug
//	Completes with <3 connections

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define TOTAL_ROOMS 7
#define MAX_CONEX 6

struct Room
{
	char *name;
	int num_conex;
	char *connections[MAX_CONEX];
	char *room_type;
};

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool IsGraphFull(struct Room* adventure[])  
{
	int full_rooms = 0;
	unsigned int i;
	for (i = 0; i < TOTAL_ROOMS; ++i)
	{
		if (adventure[i]->num_conex > 2)
			full_rooms++;
	}
	if (full_rooms == TOTAL_ROOMS)
		return true;
	else
		return false;
}

// Gets a random Room from the array, does NOT validate if connection can be added
struct Room* GetRandomRoom(struct Room* adventure[])
{
	int r = (rand()%TOTAL_ROOMS);
	return adventure[r];
}

// Returns true if a connection can be added from Room x, false otherwise
bool CanAddConnectionFrom(struct Room* x) 
{	
	// if there is space for another connection
	if (x->num_conex < MAX_CONEX)
		return true;		
	else
		return false;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room* x, struct Room* y) 
{
	x->connections[x->num_conex] = y->name;
	
	x->num_conex++;

	return;
}

void AddRoomTypes(struct Room* adventure[]) 
{
	adventure[0]->room_type = "START_ROOM";
	unsigned int i;
	for (i = 1; i < TOTAL_ROOMS-1; ++i)
	{
		adventure[i]->room_type = "MID_ROOM";
	}
	adventure[TOTAL_ROOMS-1]->room_type = "END_ROOM";
	return;
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(struct Room* x, struct Room* y) 
{
	//if name strings are the same...
	if (!strcmp(x->name, y->name))
		return true;
	else
		return false;
}

bool IsNewConnection(struct Room* x, struct Room* y) 
{
	unsigned int i;
	for (i = 0; i < x->num_conex; ++i)
	{
		// printf("Comparing %s to %s\n", y->name, x->connections[i]);
		//if room y is already a connection in room x
		if (!(strcmp(y->name,x->connections[i])))
			return false;
	}
	for (i = 0; i < y->num_conex; ++i)
	{
		// printf("Comparing %s to %s\n", x->name, y->connections[i]);
		//or vice versa
		if (!(strcmp(x->name,y->connections[i])))
			return false;
	}
	return true;
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection(struct Room* adventure[])  
{
	struct Room* A;
	struct Room* B;
	unsigned int count = 0;

	while(true)
	{
		A = GetRandomRoom(adventure);
		if (CanAddConnectionFrom(A) == true)
			break;
	}

	do
	{
		B = GetRandomRoom(adventure);
		count++;
	}
	while (CanAddConnectionFrom(B) == false ||
		IsSameRoom(A, B) == true);

	// printf("Adding connection between %s and %s...\n", A->name, B->name);
	if (IsNewConnection(A,B))
	{
		ConnectRoom(A, B);
		ConnectRoom(B, A);
	}

	return;
}

void BuildRooms(struct Room* adventure[])
{
	//create array of possible rooms
	//build TOTAL_ROOMS number of rooms from all possible
	char possible[10][20] = {
		"My House", "Crime Lab", "Music Factory", "Octopod Den", "It's a trap!",
		"On the Run", "Mazy World", "Ghost Castle", "Haunted Hospital", "Denny's, Finally!"
	};

	//pick 7 random numbers 1-10
	int options[TOTAL_ROOMS] = {0};
	unsigned int i;
	unsigned int pick;
	int scan;

	//for each room to be generated
	for (i = 0; i < TOTAL_ROOMS; ++i)
	{
		//pick a random number 1-10
		pick = (rand()%10);
		//for each room that has *already been generated*
		for (scan = 0; scan < (sizeof(options) / sizeof(int)); ++scan)
		{
			//if that room number is already scheduled to be generated
			if (options[scan] == pick)
			{
				//change number and restart scan
				pick = (rand()%10);
				scan = -1;
			}
			else if (options[scan] == 0)
			{
				options[i] = pick;
			}
		}
		// allocate memory for the room
		adventure[i] = (struct Room*) malloc(sizeof(struct Room*));

		unsigned int j;
		for (j = 0; j < MAX_CONEX; ++j)
		{
			adventure[i]->connections[j] = malloc(sizeof(char) * 20);
		}
		
		// allocate memory for name and number of connections
		adventure[i]->name = malloc(sizeof(char) * sizeof(possible[i]));
		adventure[i]->num_conex = 0;

		// assign randomized name to Room
		strcpy(adventure[i]->name, possible[options[i]]);
	}

}

void WriteRooms(struct Room* adventure[])
{
	//The first thing your rooms program must do is generate TOTAL_ROOMS different room files,
	//	one room per file, in a directory called "<STUDENT ONID USERNAME>.rooms.<PROCESS ID>".

	//In order to create the directory with the process id in the name, we will require
	//	something like:
	char buf[BUFSIZ];
	memset(buf, '\0', BUFSIZ);
	//Get process ID
    int p_id = getpid();  
    //Create string for file with <STUDENT ONID USERNAME>.rooms.<PROCESS ID>
    char newdir[] = "./corderda.rooms.";
    sprintf(buf, "%d", p_id);
    strcat(newdir,buf);
	int result = mkdir(newdir, 0755);

	unsigned int i;
	for (i = 0; i < TOTAL_ROOMS; ++i)
	{
		//You get to pick the names for those files, which should be hard-coded into your program.
		//For example, the directory, if John Smith was writing the program, should be hard-coded
		//	(except for the process id number) as: corderda.rooms.<pid>

		//initialize string buffers for the rooms path and i counter
		char curpath[BUFSIZ];
		memset(curpath, '\0', BUFSIZ);
		char buf[BUFSIZ];
		memset(buf, '\0', BUFSIZ);
		//cast i into a string
		sprintf(buf, "%d", i);
		//build the current room path from str (curpath+"/room"+i)
		strcpy(curpath, newdir);
		strcat(curpath,"/room");
		strcat(curpath,buf);
		// printf("New Room Path: %s\n", curpath);

		//create and open new room file for writing
		FILE *fp;
		fp = fopen(curpath,"w");

		//write the base room data to the current room
		fprintf(fp, "ROOM NAME: %s\n", adventure[i]->name);
		// printf("Number of Connections: %d\n", adventure[i]->num_conex);
		unsigned int j;
		for (j = 0; j < adventure[i]->num_conex; ++j)
		{
			fprintf(fp, "CONNECTION %d: %s\n", (j+1), adventure[i]->connections[j]);
		}
		fprintf(fp, "ROOM TYPE: %s\n", adventure[i]->room_type);

		//close file
		fclose(fp);
	}

	return;
}

//Print Room Name, Number of Connections, Connection refs, and 
void PrintRooms(struct Room* adventure[])
{
	unsigned int i;
	for (i = 0; i < TOTAL_ROOMS; ++i)
	{
		// printf("\nROOM NAME: %s\n", adventure[i]->name);
		// printf("Number of Connections: %d\n", adventure[i]->num_conex);
		unsigned int j;
		for (j = 0; j < adventure[i]->num_conex; ++j)
		{
			// printf("CONNECTION %d: %s\n", (j+1), adventure[i]->connections[j]);
		}
		// printf("ROOM TYPE: %s\n", adventure[i]->room_type);
	}
}

int main(int argc, char const *argv[])
{
	srand(time(NULL)); // randomize seed

	//declare an array of TOTAL_ROOMS Room structs
	struct Room* adventure[TOTAL_ROOMS];
	BuildRooms(adventure);
	
	unsigned int count = 0;
	// Create all connections in graph
	while (IsGraphFull(adventure) == false && count < 100)
	{
		AddRandomConnection(adventure);
		count++;
	}

	AddRoomTypes(adventure);

	PrintRooms(adventure);
	WriteRooms(adventure);

	return 0;
}
