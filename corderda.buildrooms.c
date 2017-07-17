//BossRoom BuildRooms.c
//By Ava Cordero
//Creates a series of files that hold descriptions of the in-game rooms and 
//	how the rooms are connected.

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
};

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool IsGraphFull(struct Room* adventure[])  
{
	int full_rooms = 0;
	for (int i = 0; i < TOTAL_ROOMS; ++i)
	{
		if (adventure[i]->num_conex > 3)
		{
			full_rooms++;
		}
	}
	if (full_rooms == TOTAL_ROOMS)
	{
		return true;
	}
	else
		return false;
}

// // Returns true if all rooms have 3 to 6 outbound connections, false otherwise
// bool IsGraphFull(struct Room* adventure[])  
// {
// 	if (strlen(adventure[6]->name) > 0)
// 	{
// 		printf("Last Room has name: %s\n", adventure[6]->name);
// 		printf("Last Room name has length: %d\n", (int) strlen(adventure[6]->name));		
// 		return true;
// 	}
// 	else
// 		printf("Graph is not yet full\n");
// 		return false;
// }

// Gets a random Room from the array, does NOT validate if connection can be added
struct Room* GetRandomRoom(struct Room* adventure[])
{
	int r = (rand()%TOTAL_ROOMS);
	return adventure[r];
}

// Returns true if a connection can be added from Room x, false otherwise
bool CanAddConnectionFrom(struct Room* x) 
{	
	// if there is space for two more connections, you can include the return connection
	if (x->num_conex < (MAX_CONEX-1))
	{
		return true;		
	}
	else
		return false;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room* x, struct Room* y) 
{
	// make x.connections[next] == y.name
	//THIS REQUIRES STRCPY!
	
	//copying/pointer bug HERE!!!
	//FIX ME!!!

	// char* name;
	// name = malloc(sizeof(char) / sizeof(y.name));
	// name = y.name;
	x->connections[x->num_conex] = y->name;
	
	printf("Testing connection %d: %s = %s\n", x->num_conex, x->connections[x->num_conex], y->name);
	
	int num_conex = x->num_conex;
	num_conex++;
	x->num_conex = num_conex;
	
	printf("Num_Conex after increment: %d\n", x->num_conex);
	return;
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(struct Room* x, struct Room* y) 
{
	if (x->name == y->name)
	{
		return true;
	}
	else
		return false;
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection(struct Room* adventure[])  
{
	struct Room* A;
	struct Room* B;

	while(true)
	{
		A = GetRandomRoom(adventure);

		if (CanAddConnectionFrom(A) == true)
			break;
	}

	do
	{
		B = GetRandomRoom(adventure);
	}
	while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true);

	printf("Adding connection between %s and %s...\n", A->name, B->name);

  ConnectRoom(A, B);
  ConnectRoom(B, A);

	return;
}

void BuildRooms(struct Room* adventure[])
{
	for (int i = 0; i < TOTAL_ROOMS; ++i)
	{
		char RoomName[20] = "Test";
		char CurRoomName[20];
		char CurRoomNum[10];
		adventure[i] = (struct Room*) malloc(sizeof(struct Room*));

		for (int j = 0; j < MAX_CONEX; ++j)
		{
			adventure[i]->connections[j] = malloc(sizeof(char) * 20);
		}

		strcpy(CurRoomName,RoomName);
		sprintf(CurRoomNum, "%d", i);
		strcat(CurRoomName, CurRoomNum);
		
		adventure[i]->name = malloc(sizeof(char) * sizeof(CurRoomName));
		adventure[i]->num_conex = 0;
		// printf("%s\n", CurRoomName);
		strcpy(adventure[i]->name, CurRoomName);
		// printf("%s\n", adventure[i]->name);
	}
}

void WriteRooms()
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
	printf("Result of mkdir(): %d\n", result);

	for (int i = 0; i < TOTAL_ROOMS; ++i)
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
		// char str[BUFSIZ];
		// sprintf(str, "%s", "ROOM NAME: ");

		//close file
		fclose(fp);
		// char greet[] = "Hello World, from Room ";
		// printf("%s%d: %s\n", greet, (i+1), curpath);
	}

	return;
}

//Print Room Name, Number of Connections, Connection refs, and 
void PrintRooms(struct Room* adventure[])
{
	for (int i = 0; i < TOTAL_ROOMS; ++i)
	{
		printf("Room %s address: %p\n", adventure[i]->name, &adventure[i]->name);
		printf("Number of Connections: %d\n", adventure[i]->num_conex);
		for (int j = 0; j < adventure[i]->num_conex; ++j)
		{
			printf("\tConnection %d: %s\n", j, adventure[i]->connections[j]);
		}
	}
}

int main(int argc, char const *argv[])
{
	srand(time(NULL)); // randomize seed

	//declare an array of TOTAL_ROOMS Room structs
	struct Room* adventure[TOTAL_ROOMS];
	BuildRooms(adventure);
	
	// Create all connections in graph
	// while (IsGraphFull(adventure) == false)
	if (IsGraphFull(adventure) == false)
	{
		AddRandomConnection(adventure);
	}

	PrintRooms(adventure);
	// WriteRooms();
	return 0;
}