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

struct Room
{
	char *name;
};

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool IsGraphFull(struct Room* adventure[])  
{
	if (lengthof(adventure[6]->name) > 0)
	{
		printf("Last Room has name: %s\n", adventure[6]->name);
		return true;
	}
	else
		printf("Graph is not yet full\n");
		return false;
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection(struct Room* adventure[])  
{
	struct Room A;
	// struct Room B;

	// while(true)
	// {
		// A = GetRandomRoom();

		// if (CanAddConnectionFrom(A) == true)
		// 	break;
	// }

//   do
//   {
//     B = GetRandomRoom();
//   }
//   while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true);

//   ConnectRoom(A, B);
//   ConnectRoom(B, A);

	return;
}

// Returns a random Room, does NOT validate if connection can be added
// Room GetRandomRoom(struct Room* adventure)
// {
// 	char RoomName[20] = "Test";
// 	char CurRoomName[20];
// 	char CurRoomNum[10];
// 	adventure[i] = (struct Room*) malloc(sizeof(struct Room*));

// 	strcpy(CurRoomName,RoomName);
// 	sprintf(CurRoomNum, "%d", i);
// 	strcat(CurRoomName, CurRoomNum);
	
// 	adventure[i]->name = malloc(sizeof(char) * sizeof(CurRoomName));

// 	// printf("%s\n", CurRoomName);
// 	strcpy(adventure[i]->name, CurRoomName);
// 	// printf("%s\n", adventure[i]->name);
// }

// Returns true if a connection can be added from Room x, false otherwise
bool CanAddConnectionFrom(struct Room x) 
{
  return true;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room x, struct Room y) 
{
  return;
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(struct Room x, struct Room y) 
{
  return true;
}

void WriteRooms()
{
	//The first thing your rooms program must do is generate 7 different room files,
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

	for (int i = 0; i < 7; ++i)
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

void PrintRooms(struct Room* adventure[])
{
	for (int i = 0; i < 7; ++i)
	{
		printf("Room %d: %s: %p\n", i, adventure[i]->name, &adventure[i]->name);
	}
}

int main(int argc, char const *argv[])
{
	//declare an array of 7 Room structs
	struct Room* adventure[7];

	//

	//Room struct testing area	

	for (int i = 0; i < (sizeof(adventure)/sizeof(adventure[0])); ++i)
	{
		char RoomName[20] = "Test";
		char CurRoomName[20];
		char CurRoomNum[10];
		adventure[i] = (struct Room*) malloc(sizeof(struct Room*));

		strcpy(CurRoomName,RoomName);
		sprintf(CurRoomNum, "%d", i);
		strcat(CurRoomName, CurRoomNum);
		
		adventure[i]->name = malloc(sizeof(char) * sizeof(CurRoomName));

		// printf("%s\n", CurRoomName);
		strcpy(adventure[i]->name, CurRoomName);
		// printf("%s\n", adventure[i]->name);
	}

	//

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