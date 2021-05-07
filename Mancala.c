//Author: Kyle Cummins
//Description: Main functionality of Mancala game

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "MancalaAI.h"

int *board;
int first = 1;
int ai1 = 0;
int ai2 = 1;
int difficulty1 = 1;
int difficulty2 = 3;
int size = 14;
int half = 7;
int seeds = 4;
int p1Wins = 0;
int p2Wins = 0;
int verbose = 0;

//Returns 1 if player is an AI
int isAI(int player){return player == 1 ? ai1 : ai2;} 

//Returns place fit to the board
int p(int place){return place % size;}

//Returns the number of  of player
int score(int player){return (player == 1 ? board[half - 1] : board[size - 1]);}

//Initializes the board with the given number of seeds
void initBoard()
{
	board = malloc(sizeof(int) * size);
	
	int i;
	for(i = 0; i < size; ++i)
	{
		if(i != size - 1 && i != half - 1)
		{
			board[i] = seeds;
		}
		else
		{
			board[i] = 0;
		}
	}
}

//Updates a board given a player's move
int updateBoard(int player, int place)
{	
	if(place < 1 || place >= half){return -1;}
	if(player == 2){place += half;}
	place -= 1; //Convert to zero-index
	if(board[place] < 1){return -2;}
	
	int seeds = board[place];
	board[place] = 0;
	
	for(;seeds > 1; --seeds)
	{
		++place;
		if((player == 1 && p(place) == size - 1) || (player == 2 && p(place) == half - 1))
		{
			++place;
		}
		++board[p(place)]; 
	}
	
	++place;
	if((player == 1 && p(place) == size - 1) || (player == 2 && p(place) == half - 1))
	{
		++place;
	}
	
	if((player == 1 && p(place) == half - 1) || (player == 2 && p(place) == size - 1))
	{
		++board[p(place)];
		return 1;
	}
	else if(((player == 1 && p(place) < half - 1) || (player == 2 && p(place) < size - 1 && p(place) > half - 1)) && board[p(place)] == 0)
	{
		int opposite = (size - 2) - p(place);
		if(player == 1)
		{
			seeds += board[opposite];
			board[opposite] = 0;
			board[half -1] += seeds;
			return 0;
		}
		else
		{
			seeds += board[opposite];
			board[opposite] = 0;	
			board[size -1] += seeds;
			return 0;
		}
	}
	else
	{
		++board[p(place)];
		return 0;
	}
}

//Outputs the board to the terminal
void printBoard(int player)
{
	printf("\n\nPlayer %d's Turn\n", player);
	printf("  1    2  \n");
	printf(" +------+ \n");
	printf(" |  %2d  | \n", board[size - 1]);
	
	int i;
	
	for(i = 0; i < half - 1; ++i)
	{
		printf("%d|%2d  %2d|%d\n", i + 1, board[i], board[(size - 2) - i], (half - 1) - i);
	}
	
	printf(" |  %2d  | \n", board[half - 1]);
	printf(" +------+ \n");
}

//Get human player's move
int getPlace()
{
	int output;
	
	printf("Input place 1 - %d: ", half - 1);
	scanf("%d", &output);
	
	return output;
}

//Return 1 if player has no moves to play
int done(int player)
{
	int start = (player == 1 ? 0 : half);
	int end = start + half - 1;
	for(; start < end; ++start)
	{
		if(board[start])
		{
			return 0;
		}
	}
	
	return 1;
}

//Main mancala game loop
void playMancala()
{
	//agentInit(size, half, seeds);
	initBoard();
	int player = first;
	int playing = 1;
	
	while(playing)
	{
		int myTurn = 1;
		while(myTurn)
		{	
			if(done(player))
			{
				int sum = 0, i, start = (player == 1 ? 0 : half);
				for(i = start; i < start + (half - 1); ++i)
				{
					sum += board[i];
				}
				board[start + (half - 1)] += sum;
				
				playing = 0;
				break;
			}
			
			if(!isAI(player))
			{	
				printBoard(player);
			}
			
			int place;
			if(isAI(player))
			{
				place = agentManager(player == 1 ? difficulty1 : difficulty2, player, board);
			}
			else
			{
				place = getPlace();
			}
			
			int result = updateBoard(player, place);
			
			if(result == 0)
			{
				myTurn = 0;
			}
		}
		
		//Switch player
		if(player == 1){player = 2;}
		else{player = 1;}
	}
	
	if(!isAI(1) || !isAI(2))
	{	
		printf("Player %d Wins!\n Press ENTER to continue.\n", (score(1) > score(2) ? 1 : 2));
		score(1) > score(2) ? ++p1Wins : ++p2Wins;
		getchar();
	}
	else
	{
		score(1) > score(2) ? ++p1Wins : ++p2Wins;
		//printf("\nScore(1): %d, Score(2): %d\n", score(1), score(2));
	}
	
	free(board);
}

//Menu for changing game settings
void changeSettings()
{
	int input;
	printf("How many pockets? ");
	scanf("%d", &input);
	half = input + 1;
	size = half * 2;
	
	printf("How many seeds per pocket? ");
	scanf("%d", &input);
	seeds = input;
	
	printf("Which player goes first? ");
	scanf("%d", &input);
	first = input;
	
	printf("\n");
}

//Change agents for players 1 and 2
void changePlayers()
{
	int input;
	printf("Player 1 is (0 Human, 1 AI): ");
	scanf("%d", &input);
	ai1 = input;
	
	printf("Player 2 is (0 Human, 1 AI): ");
	scanf("%d", &input);
	ai2 = input;
}

//Main mancala menu
void menuMancala()
{
	int trapped = 1;
	while(trapped)
	{
		printf("WELCOME TO KYLE CUMMINS' PROTOTYPE MANCALA GAME\n");
		printf("Pockets: %d | Seeds: %d | Difficulty: %d\n", half - 1, seeds, difficulty2);
		printf("1: Play vs Computer\n");
		printf("2: Play vs Player\n");
		printf("3: Change Settings\n");
		printf("4: Instructions\n");
		printf("5: Exit\n");
		printf("Please input selection number: ");
		
		int input;
		scanf("%d", &input);
		printf("\n");
		
		switch(input)
		{
			case 1:
				agentInit(size, half, seeds);
				//Train Q Learner if player 2 is Q agent
				if(difficulty2 == 3)
				{
					printf("Now training Q Learning Agent\n");
					trainQAgent();
				}
				ai1 = 0;
				ai2 = 1;
				playMancala();
				break;
			
			case 2:
				ai1 = 0;
				ai2 = 0;
				playMancala();
				break;
			
			case 3:
				changeSettings();
				break;
			
			case 4:
				printf("Pockets: the number of holes on each player's side not including the scoring hole.\nSeeds: the number of pebbles in each pocket at the start of the game.\nDifficulty: the level of difficulty from 0 to 3.\n\n");
				break;
				
			case 5:
				trapped = 0;
				break;
		}
	}
}

void testMancala()
{
	int record[4][4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Win record of each AI against each AI
	ai1 = ai2 = 1;
	first = 1;
	
	agentInit(size, half, seeds);
	printf("Now training Q Learning Agent\n");
	trainQAgent();
	
	int i, j, k;
	for(i = 0; i < 4; ++i)
	{
		difficulty1 = i;
		for(j = 0; j < 4; ++j)
		{
			difficulty2 = j;
			//printf("i=%d j=%d\n", i, j);
			for(k = 0; k < 100; ++k)
			{
				//printf("%d", k);
				playMancala();
			}
			record[i][j] = p1Wins; //Record victories
			p1Wins = 0;
		}
	}
	
	//Print results
	printf("When going first, the Random Agent won %d%% of matches against itself, %d%% against Max, %d%% against Expert, and %d%% against Q.\n", record[0][0], record[0][1], record[0][2], record[0][3]);
	printf("When going first, the Max Agent won %d%% of matches against Random, %d%% against itself, %d%% against Expert, and %d%% against Q.\n", record[1][0], record[1][1], record[1][2], record[1][3]);
	printf("When going first, the Expert Agent won %d%% of matches against Random, %d%% against Max, %d%% against itself, and %d%% against Q.\n", record[2][0], record[2][1], record[2][2], record[2][3]);
	printf("When going first, the Q Agent won %d%% of matches against Random, %d%% against Max, %d%% against Expert, and %d%% against itself.\n", record[3][0], record[3][1], record[3][2], record[3][3]);
	
	printf("(Many matchups will be either 0%% or 100%%, this is because all present agents besides random are deterministic)\nPress Enter to exit program.");
	getchar();
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	if(argc > 1 && strcmp(argv[1], "-test") == 0)
	{
		testMancala();
	}
	else if(argc > 1 && strcmp(argv[1], "-vtest") == 0)
	{
		verbose = 1;
		testMancala();
	}
	else
	{
		menuMancala();
	}
	
	free(qTable);
	return 0;
}