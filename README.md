Kyle Cummins's Mancala Project

Description:
This is a programming project that I started for a machine learning class but have since expanded in my own time. 
This program will play Mancala under the popular Kalah ruleset and allow players to compete against AI of varying complexity or human opponents.
All basic game parameters are customizable including board size and number of starting stones, all AI will adapt to these changes.

How to run:
1. To begin, compile the Mancala.c file with your C compiler of choice. 
   (Alternatively, a pre-compiled Mancala.exe has been provided)
2. Running the resulting executable will open the game menu, selecting option 1 without changing settings will utilize the Q learning AI as an opponent
3. To run in testing mode to compare the various agents, run the executable with command line argument -test
4. To run in verbose mode to see the result of each training episode, run with command line argument -vtest
