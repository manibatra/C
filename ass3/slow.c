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
//here x and y are added as offsets to the position
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
//+ agent not present
	agent_error(3);

    }
	
    char c = 'H';
    int x, y; 
    char buffer[10];
    int ptr = 0;
    while(1) {
	read_positions(m);
	    
	if(steps == 1) {
	    
	    x = m->positions[index][0];
	    y = m->positions[index][1];
	} else if(steps <= 11) {
	    //storing in the buffer for ititial steps	
	    buffer[ptr++] = find_dir(&x, &y, index, m);
	    ptr = ptr%10;

	} else {

	    c = buffer[ptr];
	    buffer[ptr++] = find_dir(&x, &y, index, m);
	    ptr = ptr%10;			

	}
		
	steps++;
		
	fprintf(stdout, "%c\n", c);
	fflush(stdout);
    }	


}

	    

int main(int argc, char **argv) {
    //for storing the agent info
    MapData m;

    if(argc > 1) {

	agent_error(1);

    }
	

    int steps = 1;
    //reading from the pipe (initial info)
    read_pipe(&m);
    
	 //itnitialise the positions array 
    init_pos(&m);
	 //starting the moves 
    start_moves(&m, steps);
      
    return 0;
}

 


