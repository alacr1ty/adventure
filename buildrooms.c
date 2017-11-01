
//buildrooms.c
	//By Ava Cordero

//Generates a set of files that contain the structure of a room object for the adventure game executable.
	//NAME: <NAME>
	//CONNECTIONS: <CONNECTION>
	//...
	//ROOM TYPE: <ROOM TYPE>

// Generate 7 different room files, one room per file, in a directory called
	// <onid>.rooms.<pid>/
// Each room has a name, 3-6 connections (random), a room type
	// 10 room names will be hardcoded, 7 are randomly chosen at runtime
	// All connections must be 2-way
// There will always be at least one path through the adventure

//Some code adapted from: 
	//oregonstate.instructure.com/courses/1638966/pages/2-dot-2-program-outlining-in-program-2

// Notes:
	// When should start, mid, and end room types be assigned?
		// during BuildAdv() or AddRoomToAdv() ?

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

// Set VERBOSE to 1 for trace statments
#define VERBOSE 0
// Set NUM_ROOMS to choose the number of room files in the adventure
#define NUM_ROOMS 7
#define MAX_ROOMS (NUM_ROOMS-1)

// Room Struct
	// These are generated to output to a formatted room file
struct Room {

	int id;
	char* name;
	int numOutConnects;
	// Rooms are fundamentally linked-lists, because of outConnects
	struct Room* outConnects[MAX_ROOMS];

};

// Adv (Adventure) Struct
	// This is an array of Rooms with NUM_ROOMS elements
	// and an int of how many Rooms are in the array
struct Adv {

	int numAdvRooms;
	struct Room advRooms[NUM_ROOMS];

};

// Function IsGraphFull()
	// Outputs: bool (returns true if all rooms have 3 to 6 outbound connections, false otherwise)
	// Inputs: An Adventure object
bool IsGraphFull (struct Adv adv) {

	struct Room* thisRoom;
	int nodesVisited = 0;

	for (int i = 0; i < NUM_ROOMS; ++i){

		thisRoom = &adv.advRooms[i];

		if (VERBOSE==1){
			printf("IsGraphFull(): Room id: %d\n",
				thisRoom->id); 
			printf("IsGraphFull(): Room name: %s\n",
				thisRoom->name); 
			printf("IsGraphFull(): Number of connects for %s: %d\n",
				thisRoom->name, thisRoom->numOutConnects);
		}

		if (thisRoom->numOutConnects > 2)
			nodesVisited++;
	}

	if (nodesVisited == NUM_ROOMS){
		if (VERBOSE==1)
			printf("IsGraphFull(): Returning True\n");
		return true;
	} else {
		if (VERBOSE==1)
			printf("IsGraphFull(): Returning False\n");
		return false;
	}

}

// Function AddRandomRoom()
	// Outputs: Void
	// Inputs: A random Room object, an array of possible room names, and the length of the array
		// does NOT validate if connection can be added
void AddRandomRoom (struct Adv* adv, char options[10][32], int optionsLen) {

		//creates a new room with the selected name
		// adds the new room to the adventure

		struct Room* thisRoom;
		int position;
		int c;

		// assign to the room pointer the address of the next room in the adventure
		thisRoom = &adv->advRooms[adv->numAdvRooms];
		// choose a random element to use from the options array
		position = rand()%optionsLen;
		// assign the most recent id and room name from the options array
		thisRoom->id = adv->numAdvRooms;
		thisRoom->name = malloc((sizeof(char) * sizeof(options[position])));
		strcpy(thisRoom->name,options[position]);

		if (VERBOSE==1){
			printf("AddRandomRoom(): Options Length: %d\n", optionsLen);
			printf("AddRandomRoom(): Room id:%d\n", adv->numAdvRooms);
		}
		
		// removes the room from the options array
		thisRoom->numOutConnects = 0;
		for (c = position ; c < optionsLen-1 ; c++)
        	strcpy(options[c],options[c+1]); 

		// increments numAdvRooms by 1
		adv->numAdvRooms++;

		return;

}

// Function IsSameRoom()
	// Outputs: Bool (returns true if Rooms x and y are the same Room, false otherwise)
	// Inputs: Two Room objects
bool IsSameRoom (struct Room* x, struct Room* y) {

	if (x->id == y->id){
		if (VERBOSE==1)
			printf("IsSameRoom(): Returning True\n");
		return true;
	} else {
		if (VERBOSE==1)
			printf("IsSameRoom(): Returning False\n");
		return false;
	}

}

// Function CanAddConnectionFrom()
	// Outputs: Bool (returns true if a connection can be added from Room x, false otherwise)
	// Inputs: A Room object
bool CanAddConnectionFrom(struct Room* x){

	// if the Room.outboundConnections:
		// are not full
	if (x->numOutConnects < MAX_ROOMS){
		if (VERBOSE==1)
			printf("CanAddConnectionFrom(): Returning True\n");
		return true;
	// otherwise return false
	} else{
		if (VERBOSE==1)
			printf("CanAddConnectionFrom(): Returning False\n");
		return false;
	}

}

bool IsAlreadyConnected(struct Room* x, struct Room* y){

	for (int i = 0; i < x->numOutConnects; ++i)
		if (x->outConnects[i]->name == y->name)
			return true;
	for (int i = 0; i < y->numOutConnects; ++i)
		if (y->outConnects[i]->name == x->name)
			return true;
	return false;

}

// Function ConnectRoom()
	// Outputs: Void (connects Rooms x and y together, does not check if this connection is valid)
	// Inputs: Two Room objects
void ConnectRoom (struct Room* x, struct Room* y) {

	x->outConnects[x->numOutConnects] = y;
	y->outConnects[y->numOutConnects] = x;

	x->numOutConnects++;
	y->numOutConnects++;

	return;

}

// Function AddRandomConnection()
	// Outputs: Void (adds a random, valid connection from between two Rooms)
	// Inputs: An adventure object
void AddRandomConnection (struct Adv* adv) {

	struct Room* A;
	struct Room* B;

	while(true){
		A = &adv->advRooms[rand()%NUM_ROOMS];
		if (CanAddConnectionFrom(A))
			break;
	}

	do{
		B = &adv->advRooms[rand()%NUM_ROOMS];
	}while(CanAddConnectionFrom(B) == false ||
		IsSameRoom(A, B) == true || IsAlreadyConnected(A,B) == true);

	if (VERBOSE==1)
		printf("AddRandomConnection(): Connecting Rooms %d and %d!\n", A->id, B->id);

	ConnectRoom(A, B);

	return;

}

// Function IsAdventureFull()
	// Outputs: Bool (returns true to show there are NUM_ROOMS in the Adventure)
	// Inputs: An Adventure object
bool IsAdventureFull (struct Adv adv) {

	if (adv.numAdvRooms == NUM_ROOMS){
		return true;
	} else {
		return false;
	}
	
}

// Function BuildFolder()
	// Outputs: Void (makes a directory with name of <onid>.rooms.<pid>/)
	// Inputs: None
void BuildFolder () {

	// make a directory with the name "<onid>.rooms.<pid>/"
	char folderName[32];
	sprintf(folderName, "rooms.%d", getpid());
    mkdir(folderName, 0755);
	return;

}

// Function BuildAdvFolder()
	// Outputs: Void (prints object data to file(s))
	// Input: An Adventure object
void BuildAdvFolder (struct Adv adv) {

	BuildFolder();

	char fileNameBase[32];
	sprintf(fileNameBase, "rooms.%d/", getpid());
	char fileName[32];
	FILE *fp;

	for (int i = 0; i < NUM_ROOMS; ++i){

		sprintf(fileName, "%s%d", fileNameBase, i);

		if (VERBOSE==1)
			printf("BuildRoomFolder(): File Name: %s\n", fileName);

		fp = fopen(fileName, "w+");
	
		fprintf(fp, "ROOM NAME: %s\n", adv.advRooms[i].name);
		for (int j = 0; j < adv.advRooms[i].numOutConnects; ++j)
			fprintf(fp, "CONNECTION %d: %s\n", j, adv.advRooms[i].outConnects[j]->name);
		if (i==0)
			fprintf(fp, "ROOM TYPE: START_ROOM");
		else if (i==(NUM_ROOMS-1))
			fprintf(fp, "ROOM TYPE: END_ROOM");
		else
			fprintf(fp, "ROOM TYPE: MID_ROOM");

		fclose(fp);
	}

	return;

}

int main (int argc, char const *argv[]) {

	// Generate a folder with name <onid>.rooms.<pid>/
	// Generate An adventure object that has been fully graphed (i.e connected)
	// Generate 7 different room files, one room per file, in a directory called
	// Prints out all the formatted room objects to the room files


	// an array of NUM_ROOMS names chosen from this array
	// change strings and/or first index quantifier (i.e. options[<x>][32]) of the
		// array to set the rooms that the game will use to generate a random adventure
	char options[10][32] = {
		"Home", "Dragon Lair", "Poppy Field",
		"Ghost Town", "Brooklyn", "Dark Dungeon",
		"Monster's Hut", "House of Mirrors", "Very Lost",
		"Happy Place"
	};


	// seed randomization pool
	srand(time(NULL)); 

	// build the room array ("adventure") object
		// declare variables for the program
	struct Adv adv;
	adv.numAdvRooms = 0;
	int optionsLen = sizeof(options) / sizeof(options[0]);

	// Add all rooms to the adventure
	while (IsAdventureFull(adv) == false){
		AddRandomRoom(&adv, options, optionsLen);
		// decrement optionsLen each time
		optionsLen--;
	}

	// Fill the adventure graphs with 3-6 connections per node
	while (IsGraphFull(adv) == false)
		AddRandomConnection(&adv);

	// "Cast" the Adventure struct into a folder of complete room files.
	BuildAdvFolder(adv);

	return 0;

}




