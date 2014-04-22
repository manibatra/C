#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <string.h>

//print the map
void print_my_map(char **map, int height, int width, int x) {
    for (int r = 0; r < height; ++r) {

	if(x == 1) {
	    fprintf(stdout, "%s", map[r]);
	
	} else {

	    fprintf(stderr, "%s", map[r]);
	}
    }

    printf("\n");
}

//update the positon of the agent
int update_position(char c, int **pos, int height, int width, int index) {

    switch (c) {
        case 'N':
            if(pos[index][0] - 1 - 1 >=0) {
                pos[index][0]--;
                return 1;
            } else {
                return 0;
            }
        case 'S':
            if(pos[index][0] - 1 + 1 < height) {
                pos[index][0]++;
                return 1;
            } else {
                return 0;
            }
        case 'W':
            if(pos[index][1] - 1 - 1 >= 0) {
                pos[index][1]--;
                return 1;
            } else {
                return 0;
            }
        case 'E':
            if(pos[index][1] + 1 - 1 < width) {
                pos[index][1]++;
                return 1;
            } else {
                return 0;
            }
	case 'H':
	    return 1;
        default:
            return 0;

    }

}

//do check for obstacles
int check_for_obstacles(char **map, int **positions, int index) {

    if(map[positions[index][0]-1][positions[index][1]-1] != '#') {

        return 1;

    } else {

        return 0;

    }

}

//read the data from the handler
void read_pipe(MapData *m) { 

    int total;
    char d;
    if(fscanf(stdin, "%d%[^\n]\n", &total, &d) != 1) {
	agent_error(4);

    }
    m->total = total;

    m->agents = malloc(sizeof(char)*(m->total+2));
    char bbuff[m->total+2];
    if(fscanf(stdin, "%s%[^\n]\n", m->agents, &d) != 1) {
	agent_error(4);
    }

    int len = strlen(m->agents);
    if(m->total != len) {
	agent_error(4);
    }

    int current;
    if(fscanf(stdin, "%d%[^\n]\n", &current, &d) != 1) {
	agent_error(4);
    }
    m->current = current;

    int h, w;
    if(fscanf(stdin, "%d %d%[^\n]\n", &h, &w, &d) != 2) {
	agent_error(4);
    }
    m->height = h;
    m->width = w;

    fgets(bbuff, 2, stdin);

    read_agent_map(m);
}

//check lines of the map
void check_line(MapData *m, int r) {
    int c;

    for (c = 0; c < m->width; ++c) {
        char p = m->mapForAgents[r][c];
        if (!(p == ' ' || p == '#')) {
	    agent_error(4);
	}
    }
}

//read the map in the agent file
void read_agent_map(MapData *m) {

    m->mapForAgents = malloc(m->height * sizeof(char *));
    for (int r = 0; r < m->height; ++r) {

        m->mapForAgents[r] = calloc(m->width + 2, sizeof(char));

        if(fgets(m->mapForAgents[r], m->width+2, stdin) == NULL
		|| m->mapForAgents[r][m->width] != '\n') {

	    agent_error(4);
	}
	
	check_line(m, r);

    }
       
}

//initialise positions for agents
void init_pos(MapData *m) {

    m->positions = malloc(m->total * sizeof(int *));
    for (int r = 0; r < m->total; r++) {
        m->positions[r] = calloc(2, sizeof(int));
    }

}

//read the positions from the handler
void read_positions(MapData *m) {
    char buff[80];
    for (int r = 0; r < m->total; r++) {
	char c = fgetc(stdin);
	ungetc(c, stdin);
	if(feof(stdin)) {
	    agent_error(4);
	}
	int a, b;
        fgets(buff, 80, stdin);
	char d;
        if(sscanf(buff, "%d %d%1[^\n]\n", &a, &b, &d) != 2) {

	    agent_error(4);


	}
        m->positions[r][0] = a;
        m->positions[r][1] = b;
    }
}

//handler for the errors that the agent can throw
void agent_error(int i) {

    switch(i) {
	case 1:
	    printf("Incorrect number of params.\n");
	    exit(1);
	case 2:
	    printf("Invalid params.\n");
	    exit(2);
	case 3:
	    printf("Dependencies not met.\n");
	    exit(3);
	case 4:
	    printf("Handler communication breakdown.\n");
	    exit(4);
	default:
	    exit(-1);
    }

}

//checking for valid moves 
int valid_move(char c, MapData *m) {
	
    int index = m->current-1;
    if(m->positions[index][0] > m->height
	    || m->positions[index][1] > m->width) {

	return 0;

    }
	
    if(update_position(c, m->positions, m->height, m->width, m->current-1)
	    && check_for_obstacles(m->mapForAgents,
	    m->positions, m->current -1)) {

        return 1;

    } else {

        return 0;

    }

}

