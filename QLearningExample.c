#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#define action_size 4
#define state_size 25

//HyperParameters
int total_episodes = 100;
int max_steps = 100;

double learning_rate = 0.7;
double Y = 0.618;

double epsilon = 1.0;
double decay_rate = 0.0125;

struct Action{
	
	char type[10];
};
typedef struct Action action;

struct State{
	
	int state_number;
	char type[10];
	double reward;
};
typedef struct State state;

void printQTable(double qTable[state_size][action_size]);
int findMaxIndex(double qTable[]);
double findMaxValue(double qTable[]);

int main(){
	
	srand(time(0));
	double qTable[state_size][action_size] = {0};
	
	for(int i = 0 ; i < state_size ; i++){
		for(int a = 0 ; a < action_size ; a++){
			qTable[i][a] = -0.1;
		}
	}
	
	printQTable(qTable);
	
	state grid[5][5];
	
	for(int i = 0; i<5 ; i++){
		for(int j = 0 ; j < 5 ; j++){
			strcpy(grid[i][j].type , "blank");
			grid[i][j].reward = 0;
			grid[i][j].state_number = (i+1)*(j+1);
		}
	}
	
	strcpy(grid[0][0].type , "start");
	grid[0][0].reward = 0;
	
	strcpy(grid[4][4].type , "end");
	grid[4][4].reward = 100;
	
	
	//state *curr_state = &grid[0][0];
	//state *next_state;
	
	int curr_state_x;
	int curr_state_y;
	
	int next_state_x;
	int next_state_y;
	


	int step;
	
	for(int episodes = 0; episodes < total_episodes ; episodes++){
		
		printf("1->");
		//strcpy(curr_state->type , "start");
		step = 0;
		double reward;
		int a;
		int collusion_occured;
		curr_state_x = 0;
		curr_state_y = 0;
		
		
		for( ; step < max_steps ; step++){
			
			collusion_occured = 0;
			int exp_chance = (rand() % 100) + 1;
			
			if(exp_chance <= epsilon * 100){
				a = (rand() * 37 ) % 4;
				
				if(a == 0){
					next_state_x = curr_state_x;
					next_state_y = curr_state_y + 1;
					
					if(next_state_y > 4 ){
						next_state_y = curr_state_y;
						collusion_occured = 1;
					}
					
				}
				else if(a == 1){
					next_state_x = curr_state_x - 1;
					next_state_y = curr_state_y;
					
					if(next_state_x < 0){
						next_state_x = curr_state_x;
						collusion_occured = 1;
					}
					
				}
				else if(a == 2){
					next_state_x = curr_state_x;
					next_state_y = curr_state_y - 1;
					
					if(next_state_y < 0){
						next_state_y = curr_state_y;
						collusion_occured = 1;
					}
					
				}
				else if(a == 3){
					next_state_x = curr_state_x + 1;
					next_state_y = curr_state_y;
					
					if(next_state_x > 4){
						next_state_x = curr_state_x;
						collusion_occured = 1;
					}
					
				}
				
			}
			else{
				
				a = findMaxIndex(qTable[5 * curr_state_y + curr_state_x]);
				
				if(a == 0){
					next_state_x = curr_state_x;
					next_state_y = curr_state_y + 1;
					
					if(next_state_y > 4 ){
						next_state_y = curr_state_y;
						collusion_occured = 1;
					}
					
				}
				else if(a == 1){
					next_state_x = curr_state_x - 1;
					next_state_y = curr_state_y;
					
					if(next_state_x < 0){
						next_state_x = curr_state_x;
						collusion_occured = 1;
					}
					
				}
				else if(a == 2){
					next_state_x = curr_state_x;
					next_state_y = curr_state_y - 1;
					
					if(next_state_y < 0){
						next_state_y = curr_state_y;
						collusion_occured = 1;
					}
					
				}
				else if(a == 3){
					next_state_x = curr_state_x + 1;
					next_state_y = curr_state_y;
					
					if(next_state_x > 4){
						next_state_x = curr_state_x;
						collusion_occured = 1;
					}
					
				}
				
			}
			
			int curr_state_number = 5 * curr_state_y + curr_state_x ;
			int next_state_number = 5 * next_state_y + next_state_x ;
			
			if(next_state_x == 4 && next_state_y ==4){
				reward = reward - 1;
				reward = reward + 1;
			}
			
			if(collusion_occured)
			reward = -5;
			else
			reward = grid[next_state_x][next_state_y].reward;
			
			
			qTable[curr_state_number][a] = qTable[curr_state_number][a] + learning_rate * 
			(reward + Y * findMaxValue(qTable[next_state_number]) - qTable[curr_state_number][a] );
			
			printf("%d->" , next_state_number + 1 );
			
			curr_state_x = next_state_x;
			curr_state_y = next_state_y;
			
			if( curr_state_x == 4 && curr_state_y == 4)
			break;
			
		}
		
		printf("\n");
		printQTable(qTable);
		epsilon = epsilon - decay_rate;
		printf("\nEND OF EPISODE %d \n**************************************************\n"  , episodes + 1 );
		
	}
	
	
	
	
}

void printQTable(double qTable[state_size][action_size]){
	
	for(int i = 0 ; i < state_size ; i++){
		for(int a = 0 ; a < action_size ; a++){
			printf("%f " , qTable[i][a]);
		}
		printf("\n");
	}
	return;
}

int findMaxIndex(double qTable[]){
	
	double max = -1000000;
	int index;
	
	for(int i = 0; i < 4 ; i++){
		
		if(qTable[i] > max){
			max = qTable[i];
			index = i;
		}
		
		
	}
	
	return index;
	
}

double findMaxValue(double qTable[]){
	
	double max = -1000000;
	int index;
	
	for(int i = 0; i < 4 ; i++){
		
		if(qTable[i] > max){
			max = qTable[i];
			index = i;
		}
		
		
	}
	
	return max;
	
}
