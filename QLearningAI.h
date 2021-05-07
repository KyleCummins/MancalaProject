//Author: Kyle Cummins
//Description: Code for Q-Learning AI for Mancala game

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Game state variables
int qSize;
int qHalf;
int qSeeds;

//Q Learning parameters
float *qTable;
int episodes = 100000;
float qGamma = 0.99;
float learnRate = 0.1;
float epsilon = 0.1;
float maxEps = 0.1;
float minEps = 0.01;
float decayRate = 0.01;

//Necessary external declarations
extern void initBoard();
extern int done(int player);
extern int updateBoard(int player, int place);
extern float utility(int player, int type, int* aiBoard);
extern int agentManager(int difficulty, int player, int *aiBoard);
extern int *board;
extern int verbose;


//Return index of q table value for given state and action
int qI(int state, int action) 
{
	return ((state * (qHalf - 1)) + action);
}

//Initializes agent AI with relevant game settings
void QInit(int si, int h, int se)
{
	qSize = si;
	qHalf = h;
	qSeeds = se;
	
	int states = pow(2, (qSize - 2)); //Number of states for binary q learner
	int actions = (qHalf - 1); //Number of possible player actions
	
	//Allocate (states * actions) floats for Q Table
	qTable = malloc(states * actions * sizeof(float)); 
	
	int i;
	for(i = 0; i < states; ++i)
	{
		int j;
		for(j = 0; j < actions; ++j)
		{
			qTable[qI(i,j)] = 0.0;
		}
	}
}


//Returns game state int based on board layout
int calcState(int player, int *qBoard)
{
	int state = 0;
	int bit = 1;
	
	//Translate board state into an int by setting the ith bit if the ith pocket contains stones (ignoring the scoring pockets)
	if(player == 1)
	{
		int i;
		for(i = 0; i < (qHalf - 1); ++i)
		{
			if(qBoard[i] != 0) state |= bit;
			bit *= 2;
		}
		
		for(i = qHalf; i < (qSize - 1); ++i)
		{
			if(qBoard[i] != 0) state |= bit;
			bit *= 2;
		}
	}
	else
	{
		//Mancala is rotationally symetrical, for player 2 just spin the board around
		int i;
		for(i = qHalf; i < (qSize - 1); ++i)
		{
			if(qBoard[i] != 0) state |= bit;
			bit *= 2;
		}
		
		for(i = 0; i < (qHalf - 1); ++i)
		{
			if(qBoard[i] != 0) state |= bit;
			bit *= 2;
		}
	}
	
	return state;
}

//Returns the maximum possible Q for a given game state
float getMaxQ(int state)
{
	float max = 0.0;
	
	//Iterate through possible actions for given state, record max Q
	int i;
	for(i = 0; i < (qHalf - 1); ++i)
	{
		if(qTable[qI(state,i)] > max) max = qTable[qI(state,i)];
	}
	
	return max;
}

//Picks an action based on a learned Q Table
int qAgent(int player, int *qBoard)
{
	float maxQ = 0.0;
	int maxAct = 0;
	
	int state = calcState(player, qBoard);
	//Iterate through possible actions for given state, record best action with nonzero pocket
	int i;
	for(i = 0; i < (qHalf - 1); ++i)
	{
		//In the event of a tie, pick rightmost action
		if(qTable[qI(state,i)] >= maxQ && qBoard[((player - 1) * qHalf) + i] != 0) 
		{
			maxQ = qTable[qI(state,i)];
			maxAct = i;
		}
	}
	
	return maxAct + 1; //Convert to 1 indexed
}

void trainQAgent()
{
	float totReward = 0;
	float avgReward = 0;
	int *curBoard = malloc(qSize * sizeof(int));
	int playing = 1;
	int player = 1;
	
	//For episode count, play game and train q table
	int i;
	for(i = 0; i < episodes; ++i)
	{
		//Initialize new game
		initBoard();
		playing = 1;
		player = 1;
		totReward = 0;
		
		//Simulate game loop
		while(playing)
		{
			int myTurn = 1;
			while(myTurn)
			{	
				if(done(player))
				{
					//Current game is over
					playing = 0;
					break;
				}
				
				int place;
				int result;
				if(player == 1)
				{
					//Q Agent's turn, make play and update result
					memcpy(curBoard, board, (qSize * sizeof(int))); //Record current board state for updating Q Table
					
					//Decide explore vs exploit
					if((rand() % 100) < (epsilon * 100))
					{
						place = agentManager(0, player, board); //Randomly pick an action
					}
					else
					{
						place = qAgent(1, board); //Pick best action based on Q Table
					}
					int action = place - 1; //Convert to 0 index
					//printf("%d", action); //Debug print
					
					//perform action
					result = updateBoard(player, place);
					
					//Calculate reward based on utility (agent score);
					float reward = utility(1, 0, board);
					if(result != 0) reward *= 2; //Double reward if another turn may be taken
					totReward += reward;
					
					//Update Q Table based on action and reward
					int curState = calcState(1, curBoard);
					int nextState = calcState(1, board);
					qTable[qI(curState,action)] = (qTable[qI(curState,action)] + learnRate * (reward + qGamma * getMaxQ(nextState) - qTable[qI(curState,action)]));
					
				}
				else
				{
					//Q Agent's opponent's turn, act randomly (difficulty = 0)?
					place = agentManager(0, player, board);
					result = updateBoard(player, place);
				}
				
				if(result == 0)
				{
					myTurn = 0;
				}
			}
			
			//Switch player
			if(player == 1){player = 2;}
			else{player = 1;}
		}
		
		//Update epsilon
		epsilon = minEps + (maxEps - minEps) * exp((0 - decayRate) * i);
		
		//Update average reward
		avgReward = avgReward + totReward;
		avgReward = avgReward / 2;
		if(verbose) printf("Total Reward for episode %d = %f\n", i, totReward); //Debug print
	}
	
	printf("Number of Episodes = %d\nAverage Reward = %f\n", episodes, avgReward);
	
	free(curBoard);
}