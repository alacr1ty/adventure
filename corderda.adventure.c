//BossRoom Adventure.c
//By Ava Cordero
//Provides an interface for playing the game using the most recently generated rooms.
//In the game, the player will begin in the "starting room" and will win the game
//	automatically upon entering the "ending room", which causes the game to exit,
//	displaying the path taken by the player.
//During the game, the player can also enter a command that returns the current time
// -- this functionality utilizes mutexes and multithreading.
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
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

// Once the user has reached the End Room, the game should indicate that it has been
// reached. It should also print out the path the user has taken to get there, the 
//number of steps taken (not the number of rooms visited, which would be one higher 
//because of the start room), a congratulatory message, and then exit.

// When your program exits, set the exit status code to 0, and leave the rooms 
//directory in place, so that it can be examined.

// Your game program must also be able to return the current time of day by utilizing
//a second thread and mutex(es). I recommend you complete all other portions of this 
//assignment first, then add this mutex-based timekeeping component last of all. Use 
//the classic pthread library for this simple multithreading, which will require you 
//to use the "-lpthread" compile option switch with gcc (see below for compilation example).

// ...lock() is being called in multiple threads (i.e. having the only ...lock() and 
//...unlock() calls occur in one thread isn't using the mutex for control).
// Mutexes allow threads to jockey for contention by having multiple locks (attempt 
//to be) established. So, perhaps the main thread locks a mutex, then spawns the 
//second thread (whose first action is to attempt to gain control of the mutex by 
//calling ...lock(), which blocks the second thread). How is the second thread 
//started up again? By the first thread calling ...unlock().
// A thread can be told to wait for another to complete with the ...join() function, 
//resuming execution when the other thread finally terminates.

time_t get_mtime(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror(path);
        exit(1);
    }
    return statbuf.st_mtime;
}

struct Room* castRoom(char *room, int num_conex)
{
	// printf("%s\n", room);
	// cast room data string into an object
	// create a new Room
	struct Room* this_r_obj;

	// allocate memory for the room
	this_r_obj = (struct Room*) malloc(sizeof(struct Room*));
	// allocate memory for name and number of connections
	this_r_obj->name = malloc(sizeof(char) * sizeof(room));

	// parse Room name
	//
	// assign Room name
	// strcpy(this_r_obj->name, possible[options[i]]);
	// assign number of connections to room
	this_r_obj->num_conex = num_conex;

	char **these_conex;
	these_conex = (char **) {malloc(num_conex * sizeof(char*))};

	unsigned int i;
	for (i = 0; i < num_conex; ++i)
	{
		// parse each Room connection
		// allocate memory for each connection
		these_conex[i] = malloc(20 * sizeof(char));
		char stuff[20] = "STUFFFFF";
		strcpy(these_conex[i],stuff);
		printf("%s\n", these_conex[i]);


		// this_r_obj->connections[i] = malloc(20 * sizeof(char));
		//assign connections
		// strcpy(this_r_obj->connections[i],these_conex[i]);
	}
	// strncpy(this_r_obj->connections,these_conex,200);


	// this_r_obj->connections = (char *[num_conex]) these_conex;
	
	// parse and assign the Room type

	return this_r_obj;
}

struct Room** genRooms(struct Room* adventure[])
{
// Since the rooms program may have been run multiple times before executing
// the game, your game should use the most recently created files: perform a stat() 
// function call on rooms directories in the same directory as the game, and open 
// the one with most recent st_mtime component of the returned stat struct.

	DIR *d;
	DIR *f;

	struct dirent *dir;
	d = opendir(".");

	if (d)
	{
		time_t latest_time = 0;
		char latest_name[30];
		while ((dir = readdir(d)) != NULL)
		{
			if (strstr(dir->d_name, "corderda.rooms.") != NULL)
			{
				if (difftime(get_mtime(dir->d_name),latest_time) > 0)
				{
					latest_time = get_mtime(dir->d_name);
					strcpy(latest_name,dir->d_name);
				}
			}
		}
		// printf("Latest: %s\n", latest_name);

		f = opendir(latest_name);

		if (f)
		{
			int r = 0;
			FILE *fp;
			char buff[255];
			strcat(latest_name,"/");

			while ((dir = readdir(f)) != NULL)
			{
				if (strstr(dir->d_name, "room") != NULL)
				{
					// open and load each roomfile into a new Room object
					char this_f[30];
					char cur_r_str[30];
					char this_r_str[255];

					strcpy(this_f,latest_name);
					strcat(this_f,dir->d_name);
					fp = fopen(this_f,"r");
					// int num_conex = -2;
					// printf("%s\n", this_f);

					if (fp)
					{
						// clear room variables?
						memset(buff, '\0', sizeof(buff));
						memset(cur_r_str, '\0', sizeof(cur_r_str));
						memset(this_r_str, '\0', sizeof(this_r_str));

						int num_conex = -2;

						while(fgets(buff, sizeof(buff), fp))
						{
							// put file contents into a string
							strcpy(cur_r_str,buff);
							strcat(this_r_str,cur_r_str);
							num_conex++;
						}

						// printf("%s\n", this_r_str);
					
						// add room to room array (adventure)
						adventure[r] = castRoom(this_r_str,num_conex);
						// move on to the next room in the adventure
						r++;

						fclose(fp);
					}
					else
						printf("Can't read file!\n");
				}
			}
			// return room array (adventure)
			return adventure;
		}
		else
			printf("Directory 'f' does not exist!\n");
	}
	else
		printf("Directory 'd' does not exist!\n");

	closedir(d);
	closedir(f);
}

void displayPrompt()
{
// CURRENT LOCATION: XYZZY
// POSSIBLE CONNECTIONS: PLOVER, Dungeon, twisty.
// WHERE TO? >Twisty

// HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.
//...
// WHERE TO? >time

//  1:03pm, Tuesday, September 13, 2016

	char location[] = "CURRENT LOCATION: ";
	char connects[] = "\nPOSSIBLE CONNECTIONS: ";
	char where_to[] = "\nWHERE TO? >";

	printf("%s", location);
	printf("%s", connects);
	printf("%s", where_to);
	printf("\n");
	return;
}

int main(int argc, char const *argv[])
{
	struct Room* adventure[TOTAL_ROOMS];

	// unsigned int i;
	// for (i = 0; i < TOTAL_ROOMS; ++i)
	// {
	// 	adventure[i] = (struct Room*) malloc(sizeof(struct Room*));
	// 	adventure[i]->connections = malloc(MAX_CONEX * sizeof(char*));

	// 	unsigned int j;
	// 	for (j = 0; j < MAX_CONEX; ++j)
	// 	{
	// 		adventure[i]->connections[j] = malloc(sizeof(char) * 20);
	// 	}
	// }

	genRooms(adventure);
	displayPrompt();

	return 0;
}
