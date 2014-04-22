#include <stdio.h>
//defining the struct to be used for interaction 
//for the handler and agents
typedef struct {

    char **map;
    char **mapForAgents;
    int height;
    int width;
    char *agents;
    int current;
    int **positions;
    int total;
    int maxRounds;
    

} MapData;
//to store info about to be sent to agents
typedef struct AgentNode {

    int posx;
    int posy;
    char current;
    int number;
    struct AgentNode *next;
    FILE *read;
    FILE *write;

} Agent;
    
    
   
//funciton for printing the desired map
void print_my_map(char **map, int height, int width, int x);

//function to check for collison with obstacles 
int check_for_obstacles(char **map, int **positions, int index);

//function to check for updating  position of the agent according to the char
int update_position(char c, int **pos, int height, int width, int index); 

//read the pipe  in the agents
void read_pipe(MapData *m);

//read the map for the agents
void read_agent_map(MapData *m);

//initialise the array for storing positions in the agent
void init_pos(MapData *m);

//read the positions when recieved in the agents
void read_positions(MapData *m);

//error handler for agents
void agent_error(int i);

//checking for valid moves
int valid_move(char c, MapData *m);
