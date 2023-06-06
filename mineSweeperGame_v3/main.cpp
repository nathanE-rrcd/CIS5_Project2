 /*
 * File:   main.cpp
 * Author: Nathan Ellett
 * Created on March 28, 2023, 10:45 AM
 * Purpose: Minesweeper Game
 */

//Libraries
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;

//Function Prototypes

int main(int argc, char** argv) {
    //Random number seed
    srand(time(0));
    
    //Initialize reference array for mine placements
    int mines = 0,
            X, Y,    //Dimensions for array (0-15/1-16)
            flagCnt = 0; //Flag count (0-40)
    int boardm[16][16] = {0}; //Initialize entire 16x16 array to 0 (no mines)
    
    //Randomly place mines
    do {
        do {
            X = rand()%16; //Get random row
            Y = rand()%16; //Get random column 
        } while (boardm[X][Y] != 0); //Do again if mine is already placed
        boardm[X][Y] = 1; //Add mine to selected location
        mines++; //Increment the number of mines
    } while(mines < 40); //Repeat until 40 mines have been placed
    
    
    //Print boardm
    for(X=0; X<16; X++) {
	for(Y=0; Y<16; Y++) {
            cout << boardm[X][Y] << " "; // Print out Mine Sweeper board
        }
        cout << endl; // After print a row, do a new line, and start a new row
    }
    cout << endl;
    
    
    //Initialize reference array for hints
    int boardh[16][16] = {0}; //Array of blank spaces
    X = Y = 0;                  //Reset X and Y to zero
    int hintc = 0;              //Count number of mines around square for hint
    
    //do {
        for(X=0; X<16; X++) {
            for(Y=0; Y<16; Y++) {
                if (boardm[X][Y] == 0) {
                    //Check if any surrounding squares have mines, if true, increment hintc
                    if((X-1)>=0 && (Y-1)>=0 && boardm[X-1][Y-1]==1)hintc++;
                    if((X-1)>=0 && boardm[X-1][Y]==1)hintc++;
                    if((X-1)>=0 && (Y+1)<16 && boardm[X-1][Y+1]==1)hintc++;
                    if((Y-1)>=0 && boardm[X][Y-1]==1)hintc++;
                    if((Y+1)<16 && boardm[X][Y+1]==1)hintc++;
                    if((X+1)<16 && (Y-1)>=0 && boardm[X+1][Y-1]==1)hintc++;
                    if((X+1)<16 && boardm[X+1][Y]==1)hintc++;
                    if((X+1)<16 && (Y+1)<16 && boardm[X+1][Y+1]==1)hintc++;
                  
                    if (hintc > 0)boardh[X][Y] = hintc; //Assigns hintc to square
                  
                    hintc = 0; //Reset hint count
              }  
            }
        } 
    //} while (X < 16 && Y < 16); //Run over entire array
    

    //Print boardh
    for(X=0; X<16; X++) {
	for(Y=0; Y<16; Y++) {
            cout << boardh[X][Y] << " "; // Print out Mine Sweeper board
        }
        cout << endl; // After print a row, do a new line, and start a new row
    }
    
    //Initialize array for board visible to players
    char board[16][16];
    //Fill with '+' char
    for(X=0; X<16; X++) {
	for( Y=0; Y<16; Y++) {
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
                         //1 represents loss, 2 a win
    char charX; //Character version of X
     
    while (winCond == 0) {
        //Print boardh
        for(X=0; X<16; X++) {
            for(Y=0; Y<16; Y++) {
                cout << boardh[X][Y] << " "; // Print out Mine Sweeper board
            }
            cout << endl; // After print a row, do a new line, and start a new row
        }

        //Print Board
        //Print numbers for top markings (1 - 30)
        cout << "    1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16     Flag Count\n" <<
                "---------------------------------------------------     " << flagCnt << endl;
        char letter = 'A'; //For left side markings (A - P)
        for(X=0; X<16; X++) {
            cout << letter << "  ";
            for( Y=0; Y<16; Y++) {
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
        Y -= 1; //Decrement Y by 1 (0 - 29)
        
        int i; //Value for tracking edits made to array
        
        switch (menu) {
            case 1: //Open square
                i = 0;
                if (board[X][Y] != '!') { //If flagged, skip over
                    if(boardm[X][Y] == 0) { //Square is not a mine
                        if(boardh[X][Y] != 0) board[X][Y] = boardh[X][Y]+48; //Square has surrounding mines
                        else { //Square has no surrounding mines
                            board[X][Y]=' ';
                        }
                        do {
                            //Run over array to open any adjacent free squares
                            for(X=0; X<16; X++){
                                for(Y=0; Y<16; Y++){
                                    if(board[X][Y]==' ') { //If square is open, check surrounding
                                        //Top left square
                                        if((X-1)>=0 && (Y-1)>=0 && board[X-1][Y-1]=='+') { //Only edit closed squares 
                                            if(boardh[X-1][Y-1] == 0) board[X-1][Y-1] = ' '; //No adjacent mines
                                            else board[X-1][Y-1] = boardh[X-1][Y-1]+48; //Adjacent mines
                                        }
                                        //Above square
                                        if((X-1)>=0 && board[X-1][Y]=='+') { //Only edit closed squares 
                                            if(boardh[X-1][Y] == 0) board[X-1][Y] = ' '; //No adjacent mines
                                            else board[X-1][Y] = boardh[X-1][Y]+48; //Adjacent mines
                                        }
                                        //Top right square
                                        if((X-1)>=0 && (Y+1)<16 && board[X-1][Y+1]=='+') { //Only edit closed squares 
                                            if(boardh[X-1][Y+1] == 0) board[X-1][Y+1] = ' '; //No adjacent mines
                                            else board[X-1][Y+1] = boardh[X-1][Y+1]+48; //Adjacent mines
                                        }
                                        //Left square
                                        if((Y-1)>=0 && board[X][Y-1]=='+') { //Only edit closed squares 
                                            if(boardh[X][Y-1] == 0) board[X][Y-1] = ' '; //No adjacent mines
                                            else board[X][Y-1] = boardh[X][Y-1]+48; //Adjacent mines
                                        }
                                        //Right square
                                        if((Y+1)<16 && board[X][Y+1]=='+') { //Only edit closed squares 
                                            if(boardh[X][Y+1] == 0) board[X][Y+1] = ' '; //No adjacent mines
                                            else board[X][Y+1] = boardh[X][Y+1]+48; //Adjacent mines
                                        }
                                        //Bottom left square
                                        if((X+1)<16 && (Y-1)>=0 && board[X+1][Y-1]=='+') { //Only edit closed squares 
                                            if(boardh[X+1][Y-1] == 0) board[X+1][Y-1] = ' '; //No adjacent mines
                                            else board[X+1][Y-1] = boardh[X+1][Y-1]+48; //Adjacent mines
                                        }
                                        //Below square
                                        if((X+1)<16 && board[X+1][Y]=='+') { //Only edit closed squares 
                                            if(boardh[X+1][Y] == 0) board[X+1][Y] = ' '; //No adjacent mines
                                            else board[X+1][Y] = boardh[X+1][Y]+48; //Adjacent mines
                                        }
                                        //Bottom right square
                                        if((X+1)<16 && (Y+1)<16 && board[X+1][Y+1]=='+') { //Only edit closed squares 
                                            if(boardh[X+1][Y+1] == 0) board[X+1][Y+1] = ' '; //No adjacent mines
                                            else board[X+1][Y+1] = boardh[X+1][Y+1]+48; //Adjacent mines
                                        }
                                    }
                                }
                            }
                            i++;
                        } while (i < 178); //Check 178 times for 178 max possible open squares
                    } else { //Square is a mine
                        winCond = 1;
                    }
                }
                break;
            case 2: //Place Flag
                if (flagCnt < 40) {
                    if (board[X][Y] == '+') {
                        board[X][Y] = '!';
                        flagCnt++;
                    } else if (board[X][Y] == '!') {
                        board[X][Y] = '+';
                        flagCnt--;
                    }
                }
                    break;
            default:
                    cout << "Please enter a valid input";
        }
        printf("\033c");
        
        //Check for win
        //All 40 flags have been placed
        if(flagCnt == 40) {
            i = 0;
            //Check for proper placement
            for(X=0; X<16; X++) {
                for(Y=0; Y<16; Y++) {
                    if(board[X][Y] == '!' && boardm[X][Y]==1) i++; //If placed correctly, increment i
                }
            }
        }
        if(i == 40) winCond = 2; //If all flags placed correctly, set to "WIN"
        //If win condition is still false, check if all squares have been opened
        if(winCond == 0) {
            //Check if closed square is mine
            for(X=0; X<16; X++) {
                for(Y=0; Y<16; Y++) {
                    if(board[X][Y] == '+' && boardm[X][Y] == 0) i++; //If closed square is not a mine, increment i
                }
            }
            if(i == 0) winCond = 2; //If no closed square was not a mine, set to "WIN"
        }
    }
    
    if (winCond == 1) cout << "You Lose! :(";
    if (winCond == 2) cout << "You Win! :)";

    return 0;
}

