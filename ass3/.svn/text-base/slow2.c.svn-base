#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <limits.h>
#include <unistd.h>
#include <string.h>

//find the index of the + agent
int find_plus(char *agents) {

    for(int i = 0; i < strlen(agents); i++) {

	if(agents[i] == '+') {

	    return i;

	}

    }

    return -1;

}

//find the move based on the direction of the + agent
char find_dir(int *x, int *y, int index, MapData *m) {
//here x and y are being added as the offset
    int diffX = m->positions[index][0] - (*x);
    int diffY = m->positions[index][1] - (*y);

    *x = m->positions[index][0];
    *y = m->positions[index][1];

    if(diffX < 0 && diffY == 0) {

	return 'N';
    } else if(diffX > 0 && diffY == 0) {

	return 'S';

    } else if(diffX == 0 && diffY < 0) {

	return 'W';

    } else if(diffX == 0 && diffY > 0) {

	return 'E';

    } else {

	return 'H';

    }

}
	
     
//starting the movement of the agent 
void start_moves(MapData *m, int steps) {

    int index = find_plus(m->agents);
    if(index == -1) {
	agent_error(3);
    }	
    char c = 'H';
    int x, y; 
    char *buffer = (char*)malloc(1);
    int buffLength = 1;
    int play = 0;//tells when to playback
    int ptr = 0;
    while(1) {
	read_positions(m);
	if(c == 'D') {
	    break;
	}
	    
	if(steps == 1) {

	    x = m->positions[index][0];
	    y = m->positions[index][1];

	} else {

	    if(!play) {
		buffer[ptr] = find_dir(&x, &y, index, m);
		//the agent got an H, time to play
		if(buffer[ptr] == 'H') {
		    buffer[ptr] = 'D';
		    play = 1;
		    ptr = 0;
		    c = buffer[ptr++];
		} else {//no playing just store
		    buffer = (char*)realloc(buffer, buffLength+1);
		    buffLength++;
		    ptr++;
		}

	    } else {//playtime so output stored chars
		c = buffer[ptr++];
	    }
	}
	steps++;
	fprintf(stdout, "%c\n", c);
	fflush(stdout);
    }	

}

	    

int main(int argc, char **argv) {
    //check for no of params
    if(argc != 1) {

	agent_error(1);

    }//struct for storing data
    MapData m;
    int steps = 1;
    //reading from the pipe (initial info)
    read_pipe(&m);
    //initialising array to store positions 
    init_pos(&m);
	 //starting the moves 
    start_moves(&m, steps);
      
    
    return 0;
}

 


