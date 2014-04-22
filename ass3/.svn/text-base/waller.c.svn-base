#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <limits.h>
#include <unistd.h>
#include <string.h>


//checking to see the surrounding walls, also tests for boundaries
int surrounding_walls(MapData *m) {

    int i = m->current - 1;

    int test = 0;

    int x = m->positions[i][0];
    int y = m->positions[i][1];

    if(x < 0 || x >= m->height || (y - 1) < 0 ||
	    (y - 1) >= m->width || m->mapForAgents[x][y-1] == '#') {
	
	test++;
    }

    if((x-1) < 0 || (x-1) >= m->height || y < 0 ||
	    y >= m->width || m->mapForAgents[x-1][y] == '#') {
	
	test++;
    }

    if((x-2) < 0 || (x-2) >= m->height || (y - 1) < 0 || 
	    (y - 1) >= m->width || m->mapForAgents[x-2][y-1] == '#') {
	
	test++;
    }

    if((x-1) < 0 || (x-1) >= m->height || (y - 2) < 0 ||
	    (y - 2) >= m->width || m->mapForAgents[x-1][y-2] == '#') {
	
	test++;
    }
    //4 means walls in all directions
    if(test == 4) {

	return 1;

    }
    return 0;    
}

//get current offset position from direction
int get_int(char dir) {

    switch(dir) {

	case 'N':
            return 1;
            break;
        case 'E':
            return 7;
            break;
        case 'W':
            return 3;
            break;
        case 'S':
            return 5;
            break;
        default:
	    return -1;
            break;
    }

}

//get dir from the current offset positon
char get_dir(int curDir) {

    switch(curDir) {

	case 1:
            return 'N';
            break;
        case 7:
	    return 'E';
            break;
        case 3:
            return 'W';
            break;
        case 5:
            return 'S';
            break;
        default:
	    return 'H';
            break;
    }
}

//returns 1 if there is a wall at the position specified by the offset
int check_wall(MapData *m, int x, int y) {

    int i = m->current - 1;
    int posX = m->positions[i][0] - 1 + x;
    int posY = m->positions[i][1] - 1 + y;
    if(posX >= m->height || posX < 0) {

	return 1;

    }

    if(posY >= m->width || posY < 0) {

	return 1;

    }

    char c = m->mapForAgents[posX][posY];
    if(c == '#') {

	return 1;

    }

    return 0;
}

//retruna if there is an agent at the positon specified by the offset
int check_agent(MapData *m, int x, int y) {

    int i = m->current - 1;
    int posX = m->positions[i][0] + x;
    int posY = m->positions[i][1] + y;
    
    for(int j = 0; j < m->total; j++) {

	if(j == i) {

	    continue;

	}

	if(posX == m->positions[j][0] && posY == m->positions[j][1]) {

	    return 1;

	}

    }

    return 0;

}

	
     
//starting the movement of the agent 
void start_moves(char dir, MapData *m) {
    //storing the offsets for all directions	
    int x[] = {-1, -1, -1, 0, 1, 1, 1, 0}; 
    int y[] = {1, 0, -1, -1, -1, 0, 1, 1};
    //getting the current direction
    int curDir = get_int(dir);

    char c = dir; 
    while(1) {
	read_positions(m);
	//check for surrounding walls
	if(surrounding_walls(m)) {
	    c = 'H';//check for wall in direction NW for E( example)
	} else if(check_wall(m, x[(curDir+3)%8], y[(curDir+3)%8]) 
		&& !check_wall(m, x[(curDir+2)%8], y[(curDir+2)%8])) {
	    //rotating about the array of offsets for that
	    curDir = (curDir+2)%8;
	    if(check_agent(m, x[curDir], y[curDir])) {
		c = 'H'; 
	    } else {
		c = get_dir(curDir);	    
	    }//turn if there is wall
	} else if(check_wall(m, x[curDir], y[curDir])) {
	    curDir = (curDir - 2);
	    if(curDir < 0) {
		curDir = 7;
	    }//turn till you find a non walled direction		
	    while(check_wall(m, x[curDir], y[curDir])) {
		curDir = (curDir - 2);
		if(curDir < 0) {
		    curDir = 7;
		}		
	    }
	    c = get_dir(curDir);
	    //check if other agents are blocking
	} else if(check_agent(m, x[curDir], y[curDir])) {

	    c = 'H';

	} else {
	    //move in that direction 
	    c = get_dir(curDir);

	} 
	fprintf(stdout, "%c\n", c);
	fflush(stdout);
    }	
}

	    

int main(int argc, char **argv) {
   //check the no of arguments
    if(argc != 2) {
	agent_error(1);

    }//store the info in struct
    MapData m;
    char dir = argv[1][0];
    char *valid = "NSWE";//checking if valid parameter
    if(strlen(argv[1]) > 1 || strchr(valid, dir) == NULL) {

	agent_error(2);

    }
    //reading from the pipe (initial info)
    read_pipe(&m);
    
	 //initialise the positions 
    init_pos(&m);
	 //starting the moves 
    start_moves(dir, &m);
      
    
    return 0;
}

 


