//adventure.c
	//By Ava Cordero

//An interface to play the adventure game with previously generated rooms.
	//The user begins in the "Starting Room" and wins the game upon entering the "Ending
	//Room", this causes the game to exit, and displays the path taken by the user.
	//During gameplay, the user may enter a command to return the current time (using 
	//mutexes and multithreading).

//Some code adapted from: 
	//oregonstate.instructure.com/courses/1638966/pages/2-dot-2-program-outlining-in-program-2

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <assert.h>

#include <sys/stat.h>

// Set VERBOSE to 1 for trace statments
#define VERBOSE 0
// Set NUM_ROOMS and MAX_ROOM to choose the number of room files and connections per room in the adventure
#define NUM_ROOMS 7
#define MAX_ROOMS (NUM_ROOMS-1)
#define ONID "corderda"

// Room Struct
	// These are generated to output to a formatted room file
struct Room {

	int id;
	char* name;
	int numOutConnects;
	char* outConnects[MAX_ROOMS];
};

// Adv (Adventure) Struct
	// This is an array of Room pointers with NUM_ROOMS elements
	// and an int of how many Rooms are in the array
struct Adv {

	int numAdvRooms;
	struct Room advRooms[NUM_ROOMS];
};

char* GetLatestAdvFolderName() {

	// perform a stat() function call on rooms directories in the same directory as the game,
		// and open the one with most recent st_mtime component of the returned stat struct.
	char* folderName;
	folderName = malloc((sizeof(char) * 32));
	char* folderNameBase;
	folderNameBase = malloc((sizeof(char) * 32));
	sprintf(folderNameBase, "%s.rooms.", ONID);

	DIR *dir;
	struct stat stbuf;
	time_t newest = 0;
	struct dirent *ent;

	
	if ((dir = opendir ("./")) != NULL) {
		/* while there are files and directories within directory to check*/
		while ((ent = readdir (dir)) != NULL) {
			// if the filename contains "<ONID>.rooms."
			if(strstr(ent->d_name,folderNameBase)){
				stat(ent->d_name,&stbuf);
				if (stbuf.st_mtime > newest){
					newest = stbuf.st_mtime;
					strcpy(folderName,ent->d_name);
				}
			}
		}

		closedir (dir);
	} else{
		printf("Failed to open working directory, exiting.\n");
		exit(1);
	}
	return folderName;
}

struct Adv GetAdvFromFolder (char* folderName) {

	// initialize an adventure
	struct Adv adv; // = (struct Adv) malloc(sizeof(struct Adv*));
	// initialize an array of rooms
	struct Room* thisRoom = (struct Room*) malloc(sizeof(struct Room*));
	char roomFiles[NUM_ROOMS][255] = {{0}};

	char* fullFilePath = malloc((sizeof(char) * 32));
	
	char* line = NULL;
    size_t len = 0;
    ssize_t read;
	
	FILE *fp;
	DIR *dir;
	
	// initialize the Adventure
	adv.numAdvRooms = 0;

	// for each room in the adventure
	int i,j,k,l,start,end,roomSize;

	if ((dir = opendir (folderName)) != NULL) {
		/* while there are files and directories within directory to check*/
		
		i=0;
		for (i=0; i < NUM_ROOMS; ++i) {
			sprintf(fullFilePath, "%s/%d", folderName, i);
			fp = fopen(fullFilePath, "r");

			while ((read = getline(&line, &len, fp)) != -1) {

				// printf("%s", line);
				strcat(roomFiles[i],line);

			}

			if (VERBOSE==1){
				printf("FileName: %s\n", fullFilePath);
				printf("\tRoom Data (Length: %lu):\n%s\n\n", strlen(roomFiles[i]), roomFiles[i]);
			}
				fclose(fp);
		}

		closedir (dir);
	} else{
		printf("Folder Name: %s\n", folderName);
		printf("Failed to open Room directory, exiting.\n");
		exit(1);
	}

	for (i = 0; i < NUM_ROOMS; ++i) {

		// assign to the room pointer the address of the next room in the adventure
		thisRoom = &adv.advRooms[i];

		// calculate the size of the current string
		roomSize = strlen(roomFiles[i]);

		thisRoom->id = i;

		// count the number of new lines, the number of connections is one less than that
		thisRoom->numOutConnects = -1;
		for (j = 0; j < roomSize; ++j)
			if (roomFiles[i][j]=='\n')
				thisRoom->numOutConnects++;

		// for each connection to the room
		j=0;
		while(j < thisRoom->numOutConnects) {
			// for each char in the room string
			for (k = 0; k < roomSize; ++k) {
				// if it's the first one
				if (j==0) {
					// look for the newline char and 
					if (roomFiles[i][k]=='\n'){
						// when you find it, allocate memory the size of the line minus "ROOM NAME: "
						thisRoom->name = malloc(sizeof(char) * ((k-11)+2));
						for (l = 0; l < k-11; ++l){
							thisRoom->name[l] = roomFiles[i][l+11];
							if (VERBOSE==1)
								printf("%c = %c\n", thisRoom->name[l], roomFiles[i][l+11]);

						}
						thisRoom->name[k-11] = 0;

						if (VERBOSE==1)
							printf("Room Name: %s\n", thisRoom->name);

						j++;
					}
				} else if (j>0) {
					if (roomFiles[i][k]==':'){
						start = k+2;
					} else if (roomFiles[i][k]=='\n'){
						end = k-1;

						thisRoom->outConnects[j-1] = malloc(sizeof(char) * ((end-start)+2));
						for (l = 0; l <= end-start; ++l)
							thisRoom->outConnects[j-1][l] = roomFiles[i][start+l];
						thisRoom->outConnects[j-1][(end-start)+1] = 0;

						if (VERBOSE==1)
							printf("Connection %d Name: %s\n", j, thisRoom->outConnects[j-1]);

						j++;
					}
				}
			}
		}

		adv.numAdvRooms++;
	}

	if (VERBOSE==1)
		printf("Number of Adv Rooms: %d\n", adv.numAdvRooms);


	return adv;
}

void PrintRoomConnections(struct Room input){
	
	// This player interface should list where the player currently is, and list the possible connections
	// that can be followed. It should also then have a prompt.

	// Here is the form that must be used:
		// CURRENT LOCATION: XYZZY
		// POSSIBLE CONNECTIONS: PLOVER, Dungeon, twisty.
		// WHERE TO? >

	printf("CURRENT LOCATION: %s\n", input.name);
	printf("POSSIBLE CONNECTIONS: ");
	int i;
	for (i = 0; i < input.numOutConnects-1; i++){
		printf("%s, ", input.outConnects[i]);
	}

	printf("%s.\n", input.outConnects[input.numOutConnects-1]);

	return;

}

// When the player types in the command "time" at the prompt, and hits enter, a second thread must write
	// the current time (in the format shown below) to a file called "currentTime.txt" in the same directory
	// as the game. The main thread will then read this time value from the file and print it out to the user,
	// with the next prompt on the next line. Keep the second thread running during the execution of the main
	// program, and wake it up with the "time" command. At least one mutex must control execution between the
	//two threads.
void* GetCurrentTime(){

// lock() is called in multiple threads
// A thread can be told to wait for another to complete with the ...join() function, resuming execution
	// when the other thread finally terminates.
// Using the time command does not increment the path history or the step count. Do not delete the
	// currentTime.txt file after your program completes - it will be examined by the grading TA.

// Here is an example of the "time" command being run in-game:
	// CURRENT LOCATION: XYZZY
	// POSSIBLE CONNECTIONS: PLOVER, Dungeon, twisty.
	// WHERE TO? >time

	//  1:03pm, Tuesday, September 13, 2016

	time_t current;
	time ( &current );

	struct tm *info;
	char* buffer;
	buffer = malloc(sizeof(char)*80);

	FILE *fp;
	fp = fopen("currentTime.txt", "w+");
	
	info = localtime( &current );

	strftime(buffer,80," %I:%M%p, %A, %B %d, %Y", info);
	fprintf (fp, "%s", buffer );

	fclose(fp);

	return buffer;

}

// When the user types in the exact name of a connection to another room and then hits return, your
	// program should write a new line, and then continue running as before.

// If the user types anything but a valid room name from this location (case matters!), the game should
	// return an error line that says “HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.”, and repeat the
	// current location and prompt, as follows:

// Trying to go to an incorrect location does not increment the path history or the step count. Once the
	// user has reached the End Room, the game should indicate that it has been reached. It should also
	// print out the path the user has taken to get there, the number of steps taken (not the number of
	// rooms visited, which would be one higher because of the start room), a congratulatory message,
	// and then exit.
// maybe there should be a separate bool IsAdventureOn() to be used instead of the adventureOn
	// variable in the while loop in main(). it could refactor the if (currentRoom==6) return false;
void ExploreAdventure (struct Adv adv) {

	int currentRoom = 0;
	struct Room room;
	char command[32];
	int steps = 0;
	char* path = malloc((sizeof(char) * 255));;
	bool found;
	int i,j;

	do {

		// grab a room according to the current room number
		room = adv.advRooms[currentRoom];
		found = false;
		// print the room connections
		if (strcmp("time",command)!=0)
			PrintRoomConnections(room);
		// prompt the user for the next room to go to
		printf("WHERE TO? >");
		fgets (command, sizeof(char) * 32, stdin);
		// clear out the newline character from the end of it
		command[strcspn(command, "\n")] = 0;

		printf("\n");

		if (strcmp("time",command)==0) {
			printf("%s\n\n", GetCurrentTime());
			found = true;
		// compare command to all adv.advRooms.outConnects[]
		} else {
			for (i = 0; i < room.numOutConnects; ++i){

				if (VERBOSE==1)
					printf("Room array string: %s\nCommand: %s\nstrcmp result: %d\n", room.outConnects[i], command, strcmp(room.outConnects[i],command));

				if(strcmp(room.outConnects[i],command)==0){
					for (j = 0; j < NUM_ROOMS; ++j) {
						if (VERBOSE==1) {
							printf("Adv room name, id, j: %s, %d, %d\n", adv.advRooms[j].name, adv.advRooms[j].id, j);
							printf("strcmp result: %d\n", strcmp(adv.advRooms[j].name,command));
						}
						if (strcmp(adv.advRooms[j].name,command)==0){
							if (VERBOSE==1)
								printf("Returning...%d\n", adv.advRooms[j].id);
							
							currentRoom = adv.advRooms[j].id;
							steps++;
							sprintf(path, "%s\n%s", path, adv.advRooms[j].name);
							found = true;
						}
					}
				}
			}
			if (found==false) {
				// if it doesn't match tell the user to try again
				printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
			}

		}

	} while (currentRoom < NUM_ROOMS-1);

	if (currentRoom == NUM_ROOMS-1){
		printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
		printf("YOU TOOK %d STEP(S). YOUR PATH TO VICTORY WAS:%s\n", steps, path);
	}

	return;
}

int main(int argc, char* argv[]){

	// Declare program variables
	char* folderName;

	// Look for newest room file folder
	// Assign latest room file folder to be the "working directory"
	folderName = GetLatestAdvFolderName();

	// "Cast" the room file directory into an adventure
	struct Adv adv = GetAdvFromFolder(folderName);

	// Explore the adventure until the End Room is found
	ExploreAdventure(adv);


	// When your program exits, set the exit status code to 0, and leave the rooms directory in place, so
		// that it can be examined.
	return 0;

}