#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <limits.h>
#include <unistd.h>
#include <string.h>


//starting the movement of the agent 
void start_moves(FILE *read, MapData *m) {

    //valid letters that can be read
    char *valid = "NWESH";
 
    if(read) {
	char c = ' ';
	while(1) {

	    read_positions(m);
	    if((c = fgetc(read)) == EOF) {
	    	c = 'D';
            } else if(strchr(valid, c) == NULL || !valid_move(c, m)) {
		c = 'H';
	    }

	    fprintf(stdout, "%c\n", c);
	    fflush(stdout);
	    if(c == 'D' && feof(read)) {
		break;	
            }
	}	

    }

    fclose(read);

}

	    

int main(int argc, char **argv) {
    
    if(argc != 2) {
	agent_error(1);
    }
    //Struct to hold the info for the agent
    MapData m;
    char *filename = argv[1];
    //file not present
    FILE *read = fopen(filename, "r");
    if(!read) {

	agent_error(2);
    }
   
    char test;
    //testing if the file is empty
    if((test = fgetc(read)) == EOF) {

	agent_error(3);

    }
    ungetc(test, read);
    read_pipe(&m);
	//setting the positions in the struct  
    init_pos(&m);
	 //starting the moves 
    start_moves(read, &m);
      
    return 0;
}

 


