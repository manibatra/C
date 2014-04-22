#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#define _GNU_SOURCE
//@panicking
//for signal handling : an array of pids of children
pid_t pid[1];
//total size of the pid array
int pidSize = 0;
//for signal handling : the string of agents
char *aS;
//for signal handling : to check if the agent has actually
//started the game or just exited with 0, lile for ls in Joel's tests
//i.e. a gaurd value
int won = -1;

//the siganl handler for the game
void catch_fail(int sig) {
    
    sigset_t blockMask;
    sigfillset(&blockMask);
       //* Block other terminal-generated signals while handler runs. 
    sigprocmask(SIG_SETMASK, &blockMask, NULL);
    for(int i = 0; i < pidSize-1; i++) {
	int childStatus;
	waitpid(pid[i], &childStatus, WNOHANG);
    	if(WIFEXITED(childStatus)) {
	    int d = WEXITSTATUS(childStatus);
	    switch(d) { //giving the appropriate status 
		case 7:
		    fprintf(stderr, "Error running agent.\n");
		    sig = 7;
		    exit(7);
		case 0:
		    if(won != -1) {
			sig = 0;
		    } 
		    break;
		case 1:
		    fprintf(stderr, "Agent %c exited with status 1.\n", aS[i]);
		    exit(12);
		case 2:
		    fprintf(stderr, "Agent %c exited with status 2.\n", aS[i]);
		    sig = 12;
		    exit(12);
		case 3:
		    fprintf(stderr, "Agent %c exited with status 3.\n", aS[i]);
		    sig = 12;
		    exit(12);
		case 4:
		    fprintf(stderr, "Agent %c exited with status 4.\n", aS[i]);
		    sig = 12;
		    exit(12);
		default:
		    sig = 13;
		    exit(13);
		    fprintf(stderr, "Agent %c exited due to signal %d.\n", 
			    aS[i], d);

	    }

	}

    }

}

//fucntion used to kill all the children
void kill_all() {

    signal(SIGQUIT, SIG_IGN);

    for(int i = 0; i < pidSize; i++) {
	wait(NULL); 

    }
	

}

//checking a particular row of the map/
void check_row(MapData *m, int r) {
    int c;
   
    for (c = 0; c < m->width; ++c) {
        char p = m->map[r][c];
        if (!(p == '.' || p == '#')) {
	    fprintf(stderr, "Corrupt map.\n");
            exit(4);
	}
    } 
}


//updating the positions of the agents in the map for printitng
void update_map(MapData *m) {

    for(int i = 0; i < m->total; i++) {
	
	m->mapForAgents[m->positions[i][0]-1][m->positions[i][1]-1] 
		= m->agents[i];

    }
}

/*frees the memory allocated to the grid via malloc*/
void free_memory(MapData *m, char **map) {

    int i;
    for(i = 0; i < m->height; i++) {

	free(map[i]);
    }

    free(map);    
    return;

}


//copy a map for the agents or for printing the updated postitions
void copy_map(MapData *m, int x) {

    m->mapForAgents = malloc(m->height * sizeof(char *));
    for(int r = 0; r < m->height; ++r) {
        m->mapForAgents[r] = calloc(m->width + 2, sizeof(char));
	memcpy(m->mapForAgents[r], m->map[r], m->width+2);
    }

    if(x) {
    	for(int i = 0; i < m->height; i++) {

	    for(int j = 0; j < m->width; j++) {

	    	if(m->mapForAgents[i][j] == '.') {
		    m->mapForAgents[i][j] = ' ';
		}	
	    }
	}

    }

}


/*read the map from the input file*/
void read_map(MapData *m, char *fileName) {

    //d is used for checking extra char in the input
    char d;
    char mapD[10];

    FILE *input = fopen(fileName, "r");   
 
    if(!input) { /*mapfile not found*/
	fprintf(stderr, "Unable to open map file.\n");
	exit(3);
    }
	//bad map dimesnions
    if(fgets(mapD, 10, input) == NULL) {
	fprintf(stderr, "Corrupt map.\n");
	exit(4);
    }
	//bad map dimensions
    if (sscanf(mapD, "%d %d%1[^\n]\n", &(m->height), &(m->width), &d) != 2 ||
	    m->height < 1 || m->height > INT_MAX 
	    || m->width < 1 || m->width > INT_MAX) {
	fprintf(stderr, "Corrupt map.\n");
        exit(4);
    }

    m->map = malloc(m->height * sizeof(char *));
    for (int r = 0; r < m->height; ++r) {
        m->map[r] = calloc(m->width + 2, sizeof(char));
        if (fgets(m->map[r], m->width + 2, input) == NULL ||
		m->map[r][m->width] != '\n') {
	    fprintf(stderr, "Corrupt map.\n");
	    exit(4);
	    
	}
       
        check_row(m, r);
    }
 
    /* check if too much data */
    if (fgetc(input) != EOF) {
	fprintf(stderr, "Corrupt map.\n");
	exit(4);
    }
    //copy of a map to send to agents
    copy_map(m, 1);
    fclose(input);
}


//write the data from the handler to the pipe
void write_to_pipe(MapData *m, Agent *agent) {

    FILE *f = agent->write;

    if(f) {
    
	fprintf(f, "%d\n", m->total);  
	fprintf(f, "%s\n", m->agents);
	fprintf(f, "%d\n", agent->number);


	fprintf(f, "%d %d\n", m->height, m->width);

	for(int i = 0; i < m->height; i++) {
    
	    fprintf(f, "%s", m->mapForAgents[i]);
	}

    }   

    fflush(f);
}

//count and check if the whitespace is more than one
void count_whitespace(FILE *f) {

    
    int countspace = 0;
    char c;    

    while((c = fgetc(f)) == ' ') {

	countspace++;
    }

    if(countspace > 1) {
	
	fprintf(stderr, "Corrupt agent.\n");
	exit(6); 
    }

    ungetc(c, f);

}

//flush monkey says hello...flush that white space
void flush_it(FILE *f) {

    char c;
    while((c = fgetc(f)) != ' ' && c != EOF && c != '\n') { 

    }


    if(c == EOF) {
	ungetc(c, f);
    
    }

}
 
//read the arguments of the agent if any
char** get_args(FILE *f, char *agentName) {
	
    //creating the initial array with executable name
    //and growing it if there are arguments
    char **input = malloc(sizeof(char*)*2);
    input[0] = malloc(sizeof(char)*500);
    input[1] = malloc(sizeof(char)*500);
    memset(input[0], '\0', 500);
    strcpy(input[0], agentName);
    int count = 1;
    int size = 2;
    char c;
    while(((c = fgetc(f)) != '\n' && c != EOF)) {
	ungetc(c, f);
	fscanf(f, "%s", input[count]);
	while((c = fgetc(f)) == ' ') {
	
	}
	ungetc(c, f);
        input = realloc(input, sizeof(char**)*(size + 1));
        for(int i = 0; i <= count + 1; i++) {
	    input[i] = realloc(input[i], sizeof(char)*500);
	}
	count++;
	size++;

    }
    input[count] = NULL;
    return input;


}

//starts the piping, forking and the exec for each agent
void start_sim(Agent *agent, char **arguments) {
    
    int fromHandler[2];
    int fromAgent[2];
    int forFail[2];
    pipe(forFail);
    int flags = fcntl(forFail[1], F_GETFD);
    if(flags == -1) {
	printf("ARGGGGGGGH\n");
	exit(20);
    }//adding FD_CLOEXEC pipe to avoid sleep 
    flags |= FD_CLOEXEC;
    if(fcntl(forFail[1], F_SETFD, flags) == -1) {
	printf("ARGGGGGGGH.\n");
	exit(20);
    }
    pipe(fromHandler);
    pipe(fromAgent);
    pid[pidSize] = fork();	
    if(pid[pidSize] == 0) {
	close(forFail[0]);//closing pipes in agent
	close(fromHandler[1]);
	dup2(fromHandler[0], 0);
	close(fromHandler[0]);
	close(fromAgent[0]);
	dup2(fromAgent[1], 1);
	close(fromAgent[1]);
	   //starting the agent with the arguments		
	execvp(arguments[0], arguments);//checking for exec fail
	write(forFail[1], &errno, sizeof(errno));
	_exit(0);	
    } else if(pid[pidSize] >0) {
	close(forFail[1]);
	int childErrno;//reading for exec fail n exiting if true
	if(read(forFail[0], &childErrno, sizeof(childErrno)) 
		== sizeof(childErrno)) {
	    fprintf(stderr, "Error running agent.\n");
	    kill_all();
            exit(7);
	}
		
	pidSize++;
    	close(fromHandler[0]);
     	close(fromAgent[1]);//creating a file pointer and storing it
    	agent->write = fdopen(fromHandler[1], "w");
    	agent->read = fdopen(fromAgent[0], "r");
    } else {
	exit(20);
    }
}
//initializes the agent struct
Agent* init_agent() {

    Agent *agent = malloc(sizeof(Agent));
    agent->write = 0;
    agent->read = 0;
    agent->number = 0;
    agent->next = 0;
    agent->posx = 0;
    agent->posy = 0;
    agent->current = 0;
    return agent;

}

//start reading a particular agent from the agent file
void start_agent(Agent *agent, FILE *f, char c) {

    ungetc(c, f);
    int x, y;//for positions
    if(fscanf(f, "%d", &x) != 1) {
	fprintf(stderr, "Corrupt agents.\n");
	exit(6);
    }
    agent->posx = x;

    count_whitespace(f);
    if(fscanf(f, "%d", &y) != 1) {
	fprintf(stderr, "Corrupt agents.\n");
	exit(6);
    }
    
    agent->posy = y;

    count_whitespace(f);
    char a;//for current agent
    if(fscanf(f, "%c", &a) != 1) {
	fprintf(stderr, "Corrupt agents.\n");
	exit(6);
    }
    agent->current = a;

    count_whitespace(f);
    char agentName[1300];
    if(fscanf(f, "%s", agentName) != 1) {
	fprintf(stderr, "Corrupt agents.\n");
	exit(6);
    }    

    count_whitespace(f);	
    char **arguments = get_args(f, agentName);
    start_sim(agent, arguments);
    
    int i = 0;	
    //freeing the array of arguments 
    while(arguments[i] != NULL) {

	free(arguments[i]);

        i++;

    }
    free(arguments[i]);
    free(arguments);	

}

//start the reading of the agent file
void read_agent(MapData *m, Agent **current, Agent **head, char *agentFile) {

    FILE *f = fopen(agentFile, "r");
    if(!f) {
	fprintf(stderr, "Unable to open agent file.\n");
	exit(5);
    }   
 
    if(f) {
	//for going through the file char by char
	char c;
	
	while((c = fgetc(f)) != EOF) {

	    if(c == '#') {

		while((c = fgetc(f)) != '\n' && c != EOF) {
		    continue;
		}

	    } else {
		if(c == ' ') {
		    fprintf(stderr, "Corrupt agents.\n");
		    exit(6);
		}
		//creating struct for that particular agent
		Agent *agent = init_agent();
		agent->number = (m->total) + 1;
		
		if(m->total == 0) {
		    (*head)->next = agent;
		    (*current) = agent;
		} else {
		    (*current)->next = agent;
		    (*current) = agent;
		}//reads a particular agent
		start_agent(agent, f, c);
		(m->total)++;
	    }
	}
    }
    fclose(f);
    (*current) = (*head)->next;
}


//checking for collisions with other agents
int check_for_agents(MapData *m, int index) {

    int x = m->positions[index][0];
    int y = m->positions[index][1];
    for(int i = 0; i < m->total; i++) {

	if(i != index) {

	    if(x == m->positions[i][0] && y == m->positions[i][1]) {
    
		return 0;

	    }

	}
    }
    return 1;
}

//playing one turn of an agent
void start_rounds(MapData *m, Agent *agent) {
    //set to won to 1 if agent actually plays the game
    won = 1;
    char *valid = "NWESHD";//string of valid responses	
    FILE *read = agent -> read;
    FILE *write = agent -> write;
    if(read && write) {
	int index = agent->number - 1;
	for(int i = 0; i < m->total; i++) {
	    fprintf(write, "%d %d\n", m->positions[i][0], m->positions[i][1]);
	    fflush(write);
	}
	char buff[80];
	fgets(buff, 80, read);
	char x; 
	sscanf(buff, "%c", &x);
	if(x == 'D') {
	    won = m->current -1;
	    printf("Agent %c succeeded.\n", m->agents[index]);
	    exit(0);
	}	
	if(strchr(valid, x) == NULL) {
	    fprintf(stderr, "Agent %c sent invalid response.\n", 
		    m->agents[agent->number - 1]); 
	    exit(11);
	}//checking collision with the wall
	if(!update_position(x, m->positions, m->height, m->width, index) 
		|| !check_for_obstacles(m->map, m->positions, index)) {
	    fprintf(stderr, "Agent %c walled.\n", m->agents[index]); 
	    exit(8);
	}//checking collisons with other agents
	if(!check_for_agents(m, agent->number - 1)) {
	    fprintf(stderr, "Agent %c collided.\n", m->agents[index]); 
	    exit(9);
	}
	//free the memory update the map n print it
	free_memory(m, m->mapForAgents);
	copy_map(m, 0);
	update_map(m);
	print_my_map(m->mapForAgents, m->height, m->width, 1);
	// printf("This is %s\n", buff);
    }

}


//intialise arrays that are required
void init_arrays(MapData *m, Agent *current, Agent *head) {

    m->positions = malloc(sizeof(int*)*(m->total));
    
    for(int i = 0; i < m->total; i++) {

	m->positions[i] = malloc(sizeof(int)*2);
	m->positions[i][0] = current->posx;
	m->positions[i][1] = current->posy;
	current = current->next;

    }

    current = head->next;

    m->agents = malloc(sizeof(char)*(m->total+1));

    for(int i = 0; i < m->total; i++) {

	m->agents[i] = current->current;
	current = current->next;

    }

    m->agents[m->total] = 0;


}
//catch the interupt signal`
void catch_int(int sig) {

    fprintf(stderr, "Exiting due to INT signal.\n");
    exit(14);
}



//catch sigpipe and do nothing
void catch_sig(int sig) {

    fprintf(stderr, "Error running agent.\n");
    exit(7);
}

//checks if maxrounds is an error and throws error if it is
void check_decimal(char *number) {

    int len = strlen(number);
    for(int i = 0; i < len; i++) {

	if(!isdigit(number[i])) {
    
	    fprintf(stderr, "Invalid maxsteps.\n");
	    exit(2);
	}
    }

}
 
int main(int argc, char **argv) {
    //register the signal handlers/
    signal(SIGINT, catch_int);  
    signal(SIGQUIT, SIG_IGN);
    signal(SIGCHLD, catch_fail); 
    if(argc != 4) { //wrong usage error 
	fprintf(stderr, "Usage: handler mapfile maxsteps agentfile\n");
	exit(1);
    }
    MapData m;//Map to hold the info about game
    m.total = 0;
    Agent *current = init_agent();//pointer to current agent
    Agent *head = init_agent();//pointer to head of the agent list
    if(sscanf(argv[2], "%d", &(m.maxRounds)) != 1 || m.maxRounds <= 0) {
	fprintf(stderr, "Invalid maxsteps.\n");
	exit(2);    
    }
    check_decimal(argv[2]);	
//	printf("The:w pid is %d\n", getpid()); 
    char *agentFile = argv[3];
    read_map(&m, argv[1]);//read the map
    read_agent(&m, &current, &head, agentFile);//read the agent file
    if(m.total == 0) {
	fprintf(stderr, "Corrupt agents.\n");
	exit(6);
    }    
    init_arrays(&m, current, head);
    aS = m.agents;//agent string for signal handling
    for(int j = 0; j < m.total; j++) {
	//sending the initial info
	write_to_pipe(&m, current);
	current = current->next;
		

    }
    current = head->next;
    if(m.total > 0) {	//starting the rounds
	for(int i = 0; i < m.maxRounds; i++) {
	    for(int j = 0; j < m.total; j++) {
		start_rounds(&m, current);
		current = current->next;
	    }
	    current = head->next;
	}
    }//exiting if maxrounds are reached
    fprintf(stderr, "Too many steps.\n");
    exit(10);
}

