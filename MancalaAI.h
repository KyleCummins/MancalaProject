//Author: Kyle Cummins
//Description: Code for basic AI opponents for Mancala game

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "MCTSAI.h"
#include "QLearningAI.h"

int aiSize;
int aiHalf;
int aiSeeds;

//Agent that randomly makes a choice
int randomAgent(int player, int *aiBoard)
{
	int choice;
	int start = (player == 1 ? 0 : aiHalf);
	int choiceGot = 0;
	while(!choiceGot)
	{
		choice = rand() % (aiHalf - 1);
		if(aiBoard[start + (choice)] != 0)
		{
			choiceGot = 1;
		}
	}
	
	//printf("%d", choice);
	return choice + 1;
}

//Agent that attempts to maximize its single turn score
int maxAgent(int player, int *aiBoard)
{
	int rightBest, rightGot = 0;
	int takeBest = 0, takeNum = 0;
	int start = (player == 1 ? 0 : aiHalf);
	int i;
	for(i = start + (aiHalf - 2); i >= start; --i)
	{
		if(aiBoard[i])
		{
			if(!rightGot)
			{
				rightBest = (i - start) + 1;
				rightGot = 1;
			}
			
			//Record if pieces may be taken.
			if(((player == 1 && (i + aiBoard[i]) % aiSize < aiHalf - 1) || (player == 2 && (i + aiBoard[i]) % aiSize < aiSize - 1 && (i + aiBoard[i]) % aiSize > aiHalf - 1)) && aiBoard[(i + aiBoard[i]) % aiSize] == 0)
			{
				if(aiBoard[(((aiSize - 2) - (i + aiBoard[i])) % aiSize)] > takeNum)
				{
					takeNum = aiBoard[(((aiSize - 2) - (i + aiBoard[i])) % aiSize)];
					takeBest = (i - start) + 1;
				}
			}
		}
	}
	
	if(takeBest)
	{
		return takeBest;
	}
	return rightBest;
}

//Agent that attempts to maximize its number of turns, then maximize its score
int expertAgent(int player, int *aiBoard)
{
	int rightBest, rightGot = 0;
	int takeBest = 0, takeNum = 0;
	int start = (player == 1 ? 0 : aiHalf);
	int i;
	for(i = start + (aiHalf - 2); i >= start; --i)
	{
		if(aiBoard[i])
		{
			if(!rightGot)
			{
				rightBest = (i - start) + 1;
				rightGot = 1;
			}
			
			//If agent can take another turn, do so
			if((player == 1 && (i + aiBoard[i]) % aiSize == aiHalf - 1) || (player == 2 && (i + aiBoard[i]) % aiSize == aiSize - 1))
			{
				return (i - start) + 1;
			}
			
			//Record if pieces may be taken.
			if(((player == 1 && (i + aiBoard[i]) % aiSize < aiHalf - 1) || (player == 2 && (i + aiBoard[i]) % aiSize < aiSize - 1 && (i + aiBoard[i]) % aiSize > aiHalf - 1)) && aiBoard[(i + aiBoard[i]) % aiSize] == 0)
			{
				if(aiBoard[(((aiSize - 2) - (i + aiBoard[i])) % aiSize)] > takeNum)
				{
					takeNum = aiBoard[(((aiSize - 2) - (i + aiBoard[i])) % aiSize)];
					takeBest = (i - start) + 1;
				}
			}
		}
	}
	
	if(takeBest)
	{
		return takeBest;
	}
	return rightBest;
}

//Returns one of a variety of utility calculations
float utility(int player, int type, int* aiBoard)
{
	if(player == 1)
	{
		switch(type)
		{
		case 0:
			return aiBoard[aiHalf - 1];
			break;
			
		case 1:
			return aiBoard[aiHalf - 1] - aiBoard[aiSize - 1];
			break;
		
		case 2:
			if(aiBoard[aiSize - 1] == 0)
			{
				return aiBoard[aiHalf - 1];
			}
			return (float)aiBoard[aiHalf - 1] / (float)aiBoard[aiSize - 1];
			break;
		}
	}
	else
	{
		switch(type)
		{
		case 0:
			return aiBoard[aiSize - 1];
			break;
			
		case 1:
			return aiBoard[aiSize - 1] - aiBoard[aiHalf - 1];
			break;
		
		case 2:
			if(aiBoard[aiHalf - 1] == 0)
			{
				return aiBoard[aiSize - 1];
			}
			return (float)aiBoard[aiSize - 1] / (float)aiBoard[aiHalf - 1];
			break;
		}
	}
}

//Initializes agent AI with relevant game settings
void agentInit(int si, int h, int se)
{
	aiSize = si;
	aiHalf = h;
	aiSeeds = se;
	//MCTSInit(si, h, se);
	QInit(si, h, se);
}

//Returns input from mancala agent of selected difficulty
int agentManager(int difficulty, int player, int *aiBoard)
{
	switch(difficulty)
	{
		case 0:
			return randomAgent(player, aiBoard);
			break;
		
		case 1:
			return maxAgent(player, aiBoard);
			break;
		
		case 2:
			return expertAgent(player, aiBoard);
			break;
		
		case 3:
			return qAgent(player, aiBoard);
			break;
		
		//case 3:
			//return mcAgent(player, aiBoard);
			//break;
	}
}