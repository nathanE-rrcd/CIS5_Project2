/*
 * File:   main.cpp
 * Author: Nathan Ellett
 * Created on March 28, 2023, 10:45 AM
 * Purpose: Minesweeper Game
 */

#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char** argv) {
    //Random number seed
    srand(time(0));
    
    //Initialize reference array for mine placements
    int mines = 0, //
            X, Y;  //Array dimensions for board
    int boardm[30][16] = {0}; //Initialize entire 30x16 array to 0 (no mines)
    
    //Randomly place mines
    do {
        do {
            X = rand()%16; //Get random row
            Y = rand()%30; //Get random column 
        } while (boardm[X][Y] != 0); //Do again if mine is already placed
        boardm[X][Y] = 1; //Add mine to selected location
        mines++; //Increment the number of mines
    } while(mines < 99); //Repeat until 99 mines have been placed
    
    /*
    //Print boardm
    for(X=0; X<16; X++) {
	for( Y=0; Y<30; Y++) {
            cout << boardm[X][Y] << " "; // Print out Mine Sweeper board
        }
        cout << endl; // After print a row, do a new line, and start a new row
    }
    */
    
    //Initialize array for board visible to players
    char board[30][16];
    //Fill with '+' char
    for(X=0; X<16; X++) {
	for( Y=0; Y<30; Y++) {
            board[X][Y] = {'+'};
        }
    }
    
    //Welcome User to the game
    cout << "Welcome to minesweeper! Press \"Enter\" to begin";
    cin.get();
    
    system("clear");
        
    //Play the game
    short menu, //Menu selection (1 or 2)
            winCond = 0; //Win condition, initialized to 0, will end with 1 or 2
    char charX; //Character version of X
     
    while (winCond == 0) {
        //Print Board
        //Print numbers for top markings (1 - 30)
        cout << "    1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 "
                "16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\n" <<
                "--------------------------------------------------"
                "-------------------------------------------\n";
        char letter = 'A'; //For left side markings (A - P)
        for(X=0; X<16; X++) {
            cout << letter << "  ";
            for( Y=0; Y<30; Y++) {
                cout << " " << board[X][Y] << " "; // Print out Mine Sweeper board
            }
            cout << endl; // After print a row, do a new line, and start a new row
            letter++;
        }
        
        //Get User input
        cout << "Would you like to:\n"
            "1. Open a square? or\n"
            "2. Place a flag?\n";
        cout << "Enter your selection, then the coordinates, separated by spaces\n"
                "ex. 1 J 7 > Opens square (J, 7)\n";
        cin >> menu >> charX >> Y;
        X = static_cast<int>(charX) - 65; //Convert charX into int (0 - 15)
        Y -= Y; //Decrement Y by 1 (0 - 29)
        
        switch (menu) {
            case 1: //Open square
                    if (boardm[X][Y] == 0) {
                        board[X][Y] = ' ';
                    } else {
                        winCond = 1;
                    }
                    break;
            case 2:
                    break;
            default:
                    cout << "Please enter a valid input";
        }
        printf("\033c");
    }
    
    return 0;
}

