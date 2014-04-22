#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* defing a struct for containing the grid, its height
 * and width, the telepoters present in alpha[], and
 * their position on the grid in row[] and col[],
 * and maxsteps
 */
typedef struct {

    char **map;
    int height;
    int width;
    int alpha[26];
    int row[26];
    int column[26];
    int maxSteps;

} MapData;

typedef int Bool;

#define TRUE 1
#define FALSE 0

/*used for checking for error codes, 666 means no error*/
#define RIGHT 666

int valid_input(char input[], char *pos, int *row, MapData *theMap);
int set_max_steps(MapData *theMap, char *entrdSteps);
int read_map(MapData *theMap, char **argv);
void print_map(MapData *theMap);
void start_game(MapData *theMap, char startPos, int startRow, char **argv);
char set_start_direction(char startPos, int row, int *pos, MapData *theMap);
int move(int currentPos[], char *direction, MapData *theMap, int *stepP);
int check_pt(MapData *theMap, int currentPos[], char *direction, int *stepP);
void reflector_nwse(char *direction);
void reflector_nesw(char *direction);
void reflector(char *direction);
int launchpad(char *direction, int currentPos[], MapData *theMap, int *stepP);
int count_white_space(char input[]);
int check_map_dimensions(char input[]);
int check_map_lines(MapData *theMap, int lines);
void check_letters(MapData *theMap);
int teleport(char *direction, int currentPos[], MapData *theMap, int *stepP);
void free_memory(MapData *theMap);

int main(int argc, char **argv) {

    MapData newMap;
    /*pos is used for starting side & row for starting row/column */
    char pos;
    int row;
    int errCode;
    /*array for reading in the user input*/ 
    char input[100];
    /*variable used for flushing stdin*/	
    int ptr;	
    /*exiting with error code 1: incorrect no of arguments*/
    if(argc > 3 || argc < 2) {
	fprintf(stderr, "Usage: thebox mapfile [maxsteps]\n"); 
	return 1;
    }		
	
     /*setting the maxsteps and checking for error code 2: bad max steps*/
    if(set_max_steps(&newMap, argv[2]) == 2) {       	
	return 2;
    }

  /*reading the mapfile and checking for error codes 3, 4, 5, 6, 7*/
    errCode = read_map(&newMap, argv);
    if(errCode != RIGHT) {    
	return errCode;
    }

    print_map(&newMap);
    while(!feof(stdin)) {
    	printf("(side pos)>");
	/*putting the user input in the buffer array*/
	if(fgets(input, 100, stdin) == NULL) {
	    /*checking for ^D during user input*/
	    feof(stdin);
	    free_memory(&newMap);
	    continue;
	}	
	if(input[strlen(input)-1] != '\n') { 
	    /*flushing stdin*/
	    while((ptr = fgetc(stdin)) != '\n' && ptr != EOF);
	}
	/*validating the user input & starting the simulation*/
    	if(valid_input(input, &pos, &row, &newMap)) {
	    start_game(&newMap, pos, row, argv);
	}
    }
			
    return 0;
}

/*count the number of whitespaces in a string*/
int count_white_space(char input[]) {
	
    int count = 0;
    int i;
	
    for(i = 0; i < strlen(input); i++) {

	if(input[i] == 32) {

	    count++;

    	}
    }
    return count;
}

/*setting the maxsteps and checking for error code 2: bad max steps
 * the function checks if the maxsteps are between 1 & 999
 * if not it returns error code 2. If no maxsteps are provided then
 * default value 10 is set
 * */
int set_max_steps(MapData *theMap, char *entrdSteps) {

    if(entrdSteps != NULL) {
	if(atoi(entrdSteps) < 1 || atoi(entrdSteps) > 999) {
	    fprintf(stderr, "Bad max steps.\n");
	    return 2;
	} else {
	    theMap -> maxSteps = atoi(entrdSteps);
	}
    } else {

	theMap -> maxSteps = 10;

    }
    return RIGHT;

}

/*read the map from the input file*/
int read_map(MapData *theMap, char **argv) {

    FILE *input;
    /*i is used as counter, err for error code, M,N for map dimension */
    int i, err, M, N;
    char mapD[100];
    /*extra is used for checking if user has entered extra char in map 
     *dimenisons line*/
    char extra;
    input = fopen(argv[1], "r");
    if(input == 0) { /*mapfile not found*/
	fprintf(stderr, "Missing map file.\n");
	return 3;
    } else {
	fgets(mapD, 100, input);
	/*checking for valid dimensions*/
	if(check_map_dimensions(mapD) == 0 || 
		sscanf(mapD, "%d %d%[^\n]", &M, &N, &extra) != 2 || 
		M > 999 || N > 999 || M < 1 || N < 1) {
	    fprintf(stderr, "Bad map dimensions.\n");
	    return 4;
    	}
	/*allocating memory space for the grid, allocating extra for checking
        *too many characters in map*/
    	theMap -> map = malloc(sizeof(char*)*(M+1));
    	for(i = 0; i <= M; i++) {
            theMap -> map[i] = malloc(sizeof(char)*N+10);
	}

    	theMap -> height = M;
    	theMap -> width = N;
	i = 0;
	/*reading from mapfile, reads M+1*N+10 lines to check for extra */
    	while(i <= M && fgets(theMap -> map[i], N+10, input) != NULL) {
	
	    i++;
    	}
    	err = check_map_lines(theMap, i);/*error 5 & 6 checked*/
	if(err != RIGHT) {
	    return err;
	}
	/*check for error 7 & find out teleporter position*/	
	check_letters(theMap);
    }

    fclose(input);
    return RIGHT;
}

/* check if the input is valid for (side pos)> and sets starting side & pos*/
int valid_input(char input[], char *pos, int *row, MapData *theMap) {
    /* p & r are used for side & pos respectively */
    char p;
    int r;
    int height = theMap -> height;
    int width = theMap -> width;	
    /* extra is used for checking presence of extra char in input*/
    char extra;

    /*returns an error if input is shorter*/
    if(strlen(input) < 2) {

	return 0;
                
    }

    /*checks for whitespace in input*/
    if(count_white_space(input) != 0) {
	return 0;

	    /*checks if the input format is right*/
    } else if(sscanf(input, "%c%d%[^\n]", &p, &r, &extra)!= 2) {
	return 0;
	    /*checks if direction is right*/
    } else if(!(p == 'N' || p == 'E'|| p == 'W' || p == 'S')) {

	return 0;

	    /*checks if the position is valid*/
    } else if((p == 'N' || p == 'S') && (r > width || r < 1)) {
	
	return 0;
    } else if((p == 'W' || p == 'E') && (r > height || r < 1)) {

        return 0;
    } else {
	*pos = p;
	*row = r;
    	return 1;
    } 
}


/*checking map dimensions formatting for whitespace and EOL*/
int check_map_dimensions(char input[]) {

    if(count_white_space(input) !=1) {
	return 0;
    } 
	
    if(input[strlen(input)-1] != '\n') {
    	return 0;
    }

    return RIGHT;
}

/*checking for error 5 and error 6, returns right if no error*/
int check_map_lines(MapData *theMap, int lines) {

    int i, j;
    for(i = 0; i < lines; i++) {
	for(j = 0; j < strlen(theMap -> map[i]); j++) {
	    char pt;
	    pt = theMap ->map[i][j];		

	    /*throwing error 5 as soon as the line length  goes overboard */
	    if(j >= theMap->width && theMap -> map[i][j] != '\n') {

		fprintf(stderr, "Map file is wrong the size.\n");
		return 5;
	    }

	    /* checking the validity of each char in the grid */
	    if(!(pt == '.' || pt == '\\' || pt == '/' || pt == '=' || pt == '@' ||
		    (pt > 64 && pt < 91) || pt == '\n')) {
		fprintf(stderr, "Bad map char.\n");
		return 5;
	    }
	}


	/*checking if EOL line is present */
	if(theMap->map[i][strlen(theMap->map[i])-1] != '\n' || 
		strlen(theMap->map[i]) != theMap->width+1) {
	    fprintf(stderr, "Map file is the wrong size.\n");	
	    return 6;
	}
    } 
	/*checking if the no of lines are valid */
    if(lines > theMap->height || lines < theMap->height) {
	fprintf(stderr, "Map file is the wrong size.\n");	
	return 6;
    }	
		
		
    return RIGHT; 
	
}

/*check for error in the alphabets*/
void check_letters(MapData *theMap) {

    /* start and end are used to indicate the pos of first & last
    * teleporter respectively in the array alpha used for storing them */
    int i, j, start = RIGHT, end = RIGHT;

    /*initialing the arrays alpha is used for storing teleporters & rows 
     *and columns their pos */	
    for(i = 0; i < 26; i++) {
	theMap -> alpha[i] = 0;
	theMap -> row[i] = 0;
	theMap -> column[i] = 0;
    }

    /* the grid is traversed one char at a time, frequencey of each teleporter 
     *is stored in array(so is its pos)and if it becomes > 1 error 7 
     *is thrown */
    for(i = 0; i < theMap -> height; i++) {
	for(j = 0; j < theMap -> width; j++) {
	    if(theMap -> map[i][j] > 64 && theMap -> map[i][j] < 91) {
		if(theMap-> alpha[theMap -> map[i][j]-'A'] + 1 == 1) {
		    theMap -> alpha[theMap -> map[i][j]-'A']++;
		    theMap -> row[theMap -> map[i][j]-'A'] = i;
		    theMap -> column[theMap -> map[i][j]-'A'] = j;
		} else {
		    fprintf(stderr, "Missing letters.\n");
		    exit(7);
		}
	    }
        }
    }
    /*setting the first & the last teleporter */
    for(i = 0, j = 25; (i < 26) && (j >= 0); i++, j--) {
	if(start == RIGHT && theMap -> alpha[i] == 1) {
	    start = i;
	}
	
	if(end == RIGHT && theMap -> alpha[j] == 1) {
	    end = j;
	}
    }
    /*checking for missing letters between first & last teleporter */
    for(i = start; i <= end; i++) {
	if((theMap -> alpha[i] == 0 || (start == end)) && start!=RIGHT) {
	    fprintf(stderr, "Missing letters.\n");
	    exit(7);
	}
    }	
    return;
}		
	
/* print the current state of the map*/
void print_map(MapData *theMap) {

    int i;
    for(i = 0; i < theMap->height; i++) {
	printf("%s", theMap->map[i]);
    }

    printf("\n");
    return;	

}

/* start the simulation of the game */
void start_game(MapData *theMap, char startPos, int row, char **argv) {

    /* indicates the step number that will take place in the next step*/
    int steps = 1;
    /*array for storing the current pos of the ball on the grid*/ 
    int currentPos[2];
    /* setting the direction the ball moves & pos*/ 
    char direction = set_start_direction(startPos, row, currentPos, theMap); 
    /*indicates if the grid has ended and ball cant travel further*/
    Bool endOfGrid = FALSE;
    /*starts step by step iteration of the ball*/
    while(steps <= theMap->maxSteps && !endOfGrid) {
	
	endOfGrid = move(currentPos, &direction, theMap, &steps);
	
	if(!endOfGrid && steps <= theMap->maxSteps) {
	    print_map(theMap);
	}
	
	steps++;
    }

    printf("End of simulation.\n");
    /*frees the memory*/
    free_memory(theMap);	
    /*reads the map for next simulation*/	
    read_map(theMap, argv);

    print_map(theMap);

    return;
	
}

/*frees the memory allocated to the grid via malloc*/
void free_memory(MapData *theMap) {

    int i;
    for(i = 0; i <= theMap -> height; i++) {

	free(theMap -> map[i]);
    }

    free(theMap -> map);	
    return;

}


/*sets the direction &  position in the x, y coordinate system
 * before the first step*/
char set_start_direction(char startPos, int row, int pos[], MapData *theMap) {

    switch(startPos) {
	case 'N':
	    pos[0] = -1;
	    pos[1] = row - 1;
	    return 'S';
	case 'S':
	    pos[0] = (theMap -> height);
	    pos[1] = row - 1;
	    return 'N';
	case 'W':
	    pos[0] = row - 1;
	    pos[1] = -1;
	    return 'E';
	case 'E':
	    pos[0] = row - 1;
	    pos[1] = (theMap -> width);
	    return 'W';
    }

    return ' ';
}		

/* the ball takes the next step & check end of the grid, if grid does not end
 * then then the position is updated and the symbol is checked at that point
 * return TRUE for end of grid*/
int move(int currentPos[], char *direction, MapData *theMap, int *stepP) {

    if(*direction == 'N' && (currentPos[0]-1) >= 0) {

	currentPos[0]--;
	return check_pt(theMap, currentPos, direction, stepP);  
		

    } else if(*direction == 'S' && (currentPos[0]+1) < (theMap -> height)) {

	currentPos[0]++;
    	return check_pt(theMap, currentPos, direction, stepP);		

    } else if(*direction == 'E' && (currentPos[1]+1) < (theMap -> width)) {

	currentPos[1]++;
    	return check_pt(theMap, currentPos, direction, stepP);
		

    } else if(*direction == 'W' && (currentPos[1]-1) >= 0) {

    	currentPos[1]--;
    	return check_pt(theMap, currentPos, direction, stepP);
		

    } else {

	return TRUE;

    } 

}


/* check the symbol at the current postition and take the appropriate action
 * if the symbol is . then it is updated else the control is transferred to 
 * the respective handler*/
int check_pt(MapData *theMap, int currentPos[], char *direction, int *stepP) {

    int x = currentPos[0];
    int y = currentPos[1];

    if(theMap -> map[x][y] == '.') {

	theMap -> map[x][y] = '1';
	return FALSE;
    }

    if(theMap -> map[x][y] >= 49 && theMap -> map[x][y] < 57) {

	theMap -> map[x][y]++;
	return FALSE;
    }
	
    if(theMap -> map[x][y] == '\\') {
	reflector_nwse(direction);
	return FALSE;
    }
	
    if(theMap -> map[x][y] == '/') {
	reflector_nesw(direction);
	return FALSE;
    }
    
    if(theMap -> map[x][y] == '=') {
	reflector(direction);
	return FALSE;
    }
    
    /* print_map is called to show the step of ball on @ */
    if(theMap -> map[x][y] == '@') {
	print_map(theMap);
	return launchpad(direction, currentPos, theMap, stepP);
		
    }
	
    /* print_map is called to show the step of ball on telporter */
    if(theMap -> map[x][y] > 64 && theMap -> map[x][y] < 91) {
	print_map(theMap);
	return teleport(direction, currentPos, theMap, stepP);
		
    }
    return FALSE;
}

/*the ball encounters the synbol '\', changing direction appropriately*/
void reflector_nwse(char *direction) {

    switch(*direction) {

	case 'N':
	    *direction = 'W';
	    break;

    	case 'W':
	    *direction = 'N';
	    break;
	
	case 'S':
	    *direction = 'E';
	    break;
    	
	case 'E':
	    *direction = 'S';
	    break;	
    }

    return;
}

/*the ball encounters the synbol '/', changing direction appropriately*/
void reflector_nesw(char *direction) {

    switch(*direction) {
	case 'N':
	    *direction = 'E';
	    break;

	case 'W':
	    *direction = 'S';
	    break;
	
	case 'S':
	    *direction = 'W';
	    break;
		
	case 'E':
	    *direction = 'N';
	    break;	
    }
    return;
}

/*the ball encounters the synbol '=', changing direction appropriately*/
void reflector(char *direction) {

    switch(*direction) {

	case 'N':
	    *direction = 'S';
	    break;

    	case 'W':
	    *direction = 'E';
	    break;
	
    	case 'S':
	    *direction = 'N';
	    break;

    	case 'E':
	    *direction = 'W';
	    break;	
    }

    return;

}

/*the ball encounters the symbol '@', and jumps accordingly
 * For each direction end of grid after jump is checked and as the 
 * jump is instantaneous a step is incremented. The ball appears and symbol at
 * the new point is checked*/
int launchpad(char *direction, int currentPos[], MapData *theMap, int *stepP) {
	
    switch(*direction) {

	case 'N':
	    if(currentPos[0]-5 >= 0) {
		currentPos[0] = currentPos[0]-5;
		(*stepP)++;
		return check_pt(theMap, currentPos, direction, stepP);
	    } else {
		return TRUE;
	    }	
			 
	    break;

	case 'W':
	    if(currentPos[1]-5 >= 0) {
		currentPos[1] = currentPos[1]-5;
		(*stepP)++;
		return check_pt(theMap, currentPos, direction, stepP);
	    } else {
		return TRUE;
	    }	
	    break;
	
	case 'S':
	    if(currentPos[0]+5 < (theMap->height)) {
		currentPos[0] = currentPos[0]+5;
	        (*stepP)++;
	        return check_pt(theMap, currentPos, direction, stepP);
	    } else {
	        return TRUE;
	    }	
	    break;
	
	case 'E':
	    if(currentPos[1]+5 < (theMap->width)) {
		currentPos[1] = currentPos[1]+5;
		(*stepP)++;
		return check_pt(theMap, currentPos, direction, stepP);
	    } else {
		return TRUE;
	    }	
	    break;	
    }

    return FALSE;
}

/*the ball landed on a teleporter and an appropriate action is taken
 * the new position of the ball is calculated
 * For each direction end of grid after jump is checked and as the 
 * jump is instantaneous a step is incremented. The ball appears and symbol at
 * the new point is checked*/
int teleport(char *direction, int currentPos[], MapData *theMap, int *stepP) {

    /*finding the position of current alphabet in the array of teleporters*/
    int current = theMap->map[currentPos[0]][currentPos[1]] - 'A';
    /*incrementing to the next alphabet for ball origination*/
    int next = (current + 1) % 26;
    while(theMap->alpha[next] != 1) {
	next = (next + 1) % 26;
    }
	
    currentPos[0] = theMap->row[next];
    currentPos[1] = theMap->column[next];
    /*as mentioned in the comment before function*/
    switch(*direction) {
	case 'N':
	    if(currentPos[0]-1 >= 0) {
		currentPos[0] = currentPos[0]-1;
		(*stepP)++;
		return check_pt(theMap, currentPos, direction, stepP);
	    } else {
		return TRUE;
	    }	
	case 'W':
	    if(currentPos[1]-1 >= 0) {
		currentPos[1] = currentPos[1]-1;
		(*stepP)++;
		return check_pt(theMap, currentPos, direction, stepP);
	    } else {
		return TRUE;
	    }	
	case 'S':
	    if(currentPos[0]+1 < (theMap->height)) {
		currentPos[0] = currentPos[0]+1;
		(*stepP)++;
		return check_pt(theMap, currentPos, direction, stepP);
	    } else {
		return TRUE;
	    }	
	case 'E':
	    if(currentPos[1]+1 < (theMap->width)) {
		currentPos[1] = currentPos[1]+1;
		(*stepP)++;
	    	return check_pt(theMap, currentPos, direction, stepP);
	    } else {
		return TRUE;
	    }	
    }
    return FALSE;
}
