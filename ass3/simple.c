#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <limits.h>
#include <unistd.h>
#include <string.h>

//starting the movement of the agent 
void start_moves(char dir, MapData *m, int steps) {

    char c; 
    while(1) {
	read_positions(m);
	if(steps == 10) {

	    break;

	}
	if(!valid_move(dir, m)) {
	    c = 'H';
	} else {

	    c = dir;

	}

	   
	steps++;
		
	fprintf(stdout, "%c\n", c);


	fflush(stdout);
    }

    c = 'D';
    fprintf(stdout, "%c\n", c);
    fflush(stdout);

}

	    

int main(int argc, char **argv) {
    //checking for the no of params
    if(argc != 2) {
		
	agent_error(1);

    }

    char *valid = "NSEW";
    //length of the parameter
    int a = strlen(argv[1]);
      //for holding the agent inof
    MapData m;
    char dir = argv[1][0];
    if(a != 1 || strchr(valid, dir) == NULL) {

	agent_error(2);

    }
    int steps = 0;
    //reading from the pipe (initial info)
    read_pipe(&m);
    
	 //initialising initial info 
    init_pos(&m);
	 //starting the moves 
    start_moves(dir, &m, steps);
      
    return 0;
}

 


