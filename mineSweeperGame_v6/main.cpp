 /* 
 * File:   main.cpp
 * Author: Nathan Ellett
 * Created on June 3, 2023, 9:27 PM
 * Purpose: Version 6 of Mine Sweeper game, improved input verification
 */

//System Libraries
#include <iostream> //Input/Output Library
#include <cstdlib>  //C-Standard Library
#include <ctime>    //C-Time Library
#include <iomanip>  //Input/Output Manipulation Library
#include <fstream>  //File Stream Library
#include <cmath>    //C-Math Library
#include <string>   //String Library
using namespace std;

//Function Prototypes
void plcMine(int[16][16]); //Randomly places 40 mines on board
void plcHint(int[16][16], const int[16][16]); //Place hints
void prntBrd(const char[16][16],const short flags=0); //Print board
void getIn(char&,char&,int&);
short openSqr(char[16][16],const int[16][16],const int[16][16],int,int); //Opens square and surrounding squares
short plcFlag(char[16][16],int,int); //Toggles Flags on or off, returns updated flag count
bool checWin(const char[16][16],const int[16][16],short); //Checks "flag win" condition
bool checWin(const char[16][16],const int[16][16]); //Checks "opened squares" condition
void hiScore(int); //Retrieves and compares high-scores

//Execution Begins Here!
int main(int argc, char** argv) {
    //Set random number seed
    srand(static_cast<int>(time(0)));
    
    //Declare Variables
    int X,Y; //Board Coordinates
    short flags=0, //Counts flags placed by user
            winCond=0; //Win Condition, 0=in game -1=Loss 1=Win
    char menu,charX; //For reading inputs
    int boardm[16][16]={0},     //Mine Locations, initialize all to 0, mine==1
            boardh[16][16]={0}; //Hints, initialize all to 0
    char board[16][16];
    
    //Welcome the User to the game
    cout<<"Welcome to minesweeper! Press \"Enter\" to begin.";
    //After hitting enter clear screen
    cin.get();
    system("clear");
    
    //Begin time (For high-score)
    int intTime=time(0);
    
    //Create 3 boards, (1)Mine Locations (2)Hints (3)Playable Board
    plcMine(boardm); //Generate Board with mine locations
    plcHint(boardh,boardm); //Generate Board with hints
    //Playable Board
    for(X=0;X<16;X++){
        for(Y=0;Y<16;Y++){
            board[X][Y]='+'; //Fills board with '+' representing unopened spaces
        }
    }
    
    //Play the game
    prntBrd(board);
    cout<<"To begin, would you like too:\n"<<
            " 1. Open a square?\n"<<"\tor\n"<<
            " 2. Toggle a flag?\n";
    cout<<"Enter your choice, then the coordinates, starting with the letter.\n"<<
            "ex. 2 J 7 > Places a flag at (J, 7)\n";
    
    //While the win condition is zero, repeat the game play loop
    while(winCond==0){
        //Get user inputs
        getIn(menu,charX,Y);
        if(charX>=97 && charX<=122)charX-=32; //Lower case to upper case
        X=static_cast<int>(charX)-65; //Convert character to number 0-15
        Y-=1; //1-16 -> 0-15
        
        switch(menu){
            case '1': //Open square
                if(X>=0 && X<16 && Y>=0 && Y<16){
                    //Do open square function, update win condition
                    winCond=openSqr(board,boardm,boardh,X,Y);
                }else{
                    cout<<"Please enter a valid input\n";  
                }
                break;
            case '2': //Place flag
                if(X>=0 && X<16 && Y>=0 && Y<16){
                    //Toggle flag and return updated flag count
                    flags=plcFlag(board,X,Y);
                }else{
                    cout<<"Please enter a valid input\n";  
                }
                break;
            case '3':
                exit(0);
            default:
                cout<<"Please enter a valid input\n";
                break;
        }
        //Check and update win condition
        if(winCond==-1)break; //Game has been lost by opening mine
        if(flags==40){
            if(checWin(board,boardm,flags)){
                winCond=1;
                break; //Game has been won by properly placing each flag
            }
        }
        if(checWin(board,boardm)){
            winCond=1;
            break; //Game has been won by opening all squares
        }
        //Else, game-play continues
        printf("\033c"); //Clear Screen
        prntBrd(board,flags); //Re-print board
        cout<<"(1)Open Square (2)Toggle Flag (3)Exit\n"; //Prompt for input
                                                 //(Received at start of loop)
    }
    //Final Time
    int finTime=time(0)-intTime;
    
    //Game-play concluded, output win/loss screen
    if(winCond==-1){
        cout<<"\n! ! ! ! ! ! ! ! ! !\n"
                "! Y O U   L O S E !\n"
                "! ! ! ! ! ! ! ! ! !";
    }
    if(winCond==1){
        cout<<"\n! ! ! ! ! ! ! ! !\n"
                "! Y O U   W I N !\n"
                "! ! ! ! ! ! ! ! !\n";
        
        cout<<"\nSolved in "<<(finTime/60)<<" mins "<<(finTime%60)<<" sec\n";
        cout<<"\nPress \"Enter\" to continue.";
        //After hitting enter clear screen
        cin.ignore();
        cin.get();
        system("clear");
        hiScore(finTime);
    }
    
    return 0;
}

void plcMine(int boardm[16][16]) {
    int X,Y,     //Board Coordinates
        mines=0; //Mines placed, initialized to 0
    
    do{
        do{
            X=rand()%16; //Get random row
            Y=rand()%16; //Get random column
        }while(boardm[X][Y]!=0); //Get random coordinates until empty square selected
        boardm[X][Y]=1; //Add mine
        mines++; //Increment placed mines
    }while(mines<40); //Repeat until 40 mines are placed
    
    return;
}

void plcHint(int boardh[16][16],const int boardm[16][16]) {
    int X,Y,         //Board Coordinates
            mineC=0; //Count of surrounding mines, initialized to 0
    
    //Count number of mines surrounding each square, place count ("hint") in square
    X=Y=0; //Begin at 0,0
    for(X=0;X<16;X++){
        for(Y=0;Y<16;Y++){
            //Check that square IS NOT mine
            if(boardm[X][Y]==0){
                //Check each surrounding square. If mine: increment mine count
                if((X-1)>=0 && (Y-1)>=0 && boardm[X-1][Y-1]==1)mineC++;
                if((X-1)>=0 && boardm[X-1][Y]==1)mineC++;
                if((X-1)>=0 && (Y+1)<16 && boardm[X-1][Y+1]==1)mineC++;
                if((Y-1)>=0 && boardm[X][Y-1]==1)mineC++;
                if((Y+1)<16 && boardm[X][Y+1]==1)mineC++;
                if((X+1)<16 && (Y-1)>=0 && boardm[X+1][Y-1]==1)mineC++;
                if((X+1)<16 && boardm[X+1][Y]==1)mineC++;
                if((X+1)<16 && (Y+1)<16 && boardm[X+1][Y+1]==1)mineC++;
                
                boardh[X][Y]=mineC; //Assign mine count to square
                mineC=0;            //Reset mine count
            }
        }
    }
    
    return;
}

void prntBrd(const char board[16][16],const short flags){
    //Print numbers for top coordinates (1-16)
    cout<<"   1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16     Flag Count\n"<<
          "---------------------------------------------------     "<<flags<<endl;
    char letter='A';//Left hand coordinates (A-P)
    for(int X=0;X<16;X++){
        cout<<letter<<" ";
        for(int Y=0;Y<16;Y++){
            cout<<" "<<board[X][Y]<<" ";
        }
        cout<<endl;
        letter++;
    }
    
    return;
}

void getIn(char &menu,char &charX,int &Y){
    cin>>menu>>charX>>Y;
    return;
}

short openSqr(char board[16][16],const int boardm[16][16],
        const int boardh[16][16],int x,int y){
    if(board[x][y]!='!'){ //If square contains flag, do not open
        if(boardm[x][y]==1){
            return -1;//Square was mine, you lose
        }else{
            //If square has no surrounding mines, put ' ', else put hint value
            board[x][y]=(boardh[x][y]==0?' ':(boardh[x][y]+48)); //Hint int to char
            //Check surrounding squares, check for 178 max possible openings
            for(int i=0;i<178;i++){
                for(x=0;x<16;x++){
                    for(y=0;y<16;y++){
                        if(board[x][y]==' '){//If open, check surrounding squares
                            //Top left square
                            //Only edit closed and in bounds squares 
                            if((x-1)>=0 && (y-1)>=0 && board[x-1][y-1]=='+') {
                                board[x-1][y-1]=(boardh[x-1][y-1]==0?' ':(boardh[x-1][y-1]+48));
                            }
                            //Top center square
                            if((x-1)>=0 && board[x-1][y]=='+') { //Only edit closed squares 
                                board[x-1][y]=(boardh[x-1][y]==0?' ':(boardh[x-1][y]+48));
                           }
                            //Top right square
                            if((x-1)>=0 && (y+1)<16 && board[x-1][y+1]=='+') { //Only edit closed squares 
                                board[x-1][y+1]=(boardh[x-1][y+1]==0?' ':(boardh[x-1][y+1]+48));
                            }
                            //Left square
                            if((y-1)>=0 && board[x][y-1]=='+') { //Only edit closed squares 
                                board[x][y-1]=(boardh[x][y-1]==0?' ':(boardh[x][y-1]+48));
                            }
                            //Right square
                            if((y+1)<16 && board[x][y+1]=='+') { //Only edit closed squares 
                                board[x][y+1]=(boardh[x][y+1]==0?' ':(boardh[x][y+1]+48));
                            }
                            //Bottom left square
                            if((x+1)<16 && (y-1)>=0 && board[x+1][y-1]=='+') { //Only edit closed squares 
                                board[x+1][y-1]=(boardh[x+1][y-1]==0?' ':(boardh[x+1][y-1]+48));
                            }
                            //Bottom center square
                            if((x+1)<16 && board[x+1][y]=='+') { //Only edit closed squares 
                                board[x+1][y]=(boardh[x+1][y]==0?' ':(boardh[x+1][y]+48));
                            }
                            //Bottom right square
                            if((x+1)<16 && (y+1)<16 && board[x+1][y+1]=='+') { //Only edit closed squares 
                                board[x+1][y+1]=(boardh[x+1][y+1]==0?' ':(boardh[x+1][y+1]+48));
                            }
                        }       
                    }
                }
            }
        }
    }
    //Return 0 to continue game-play
    return 0;
}

short plcFlag(char board[16][16],int x,int y){
    static int flags = 0;
    if(flags>=40){
        cout<<"Please remove a flag before placing another.\n";
    }else{
        //Enable Flag
        if(board[x][y]=='+'){
            board[x][y]='!';
            flags++;
        //Disable Flag
        }else if (board[x][y]=='!'){
           board[x][y]='+';
           flags--; 
        }
    }
    return flags;
}

bool checWin(const char board[16][16],const int boardm[16][16],short flags){
    int x,y; //Array coordinates
    
    //If all 40 flags are placed, check placements
    for(x=0;x<16;x++){
        for(y=0;y<16;y++){
            if(board[x][y]=='!' && boardm[x][y]!=1)return 0;//Flag placement incorrect
        }
    }
    return 1; //All flags were placed correctly
}

bool checWin(const char board[16][16],const int boardm[16][16]){
    int x,y; //Array coordinates
    
    //Check if all safe squares are opened
    for(x=0;x<16;x++){
        for(y=0;y<16;y++){
            if(board[x][y]=='+' && boardm[x][y]==0)return 0; //Safe squares still closed
        }
    }
    return 1; //No covered mines remain, return WIN
}

void hiScore(int total){
    //CODE PULLED FROM YAHTZEE PROJECT
    char player[9];
    string name1,name2,name3,name4,name5; //Player names
    int score1,score2,score3,score4,score5; //Player scores
    
    //Read in previous high scores
    ifstream inScr;
    inScr.open("highscores.txt");

    inScr>>name1>>score1>>name2>>score2>>name3>>score3>>name4>>score4>>name5>>score5;

    inScr.close();
    //Compare player's total to high scores, replace if higher
    //If score is greater than score five, congratulate player and get their name
    if(total<score5){
        cout<<"\nCongratulations on a High Score!\n"
            <<"Please enter a name, then press enter (9 characters max)\n";
        cin>>setw(9)>>player;
    }
    //If player score is higher than score one, replace top score and shift down
    if(min(total,score1)==total){
        score5=score4;
        score4=score3;
        score3=score2;
        score2=score1;
        score1=total;

        name5=name4;
        name4=name3;
        name3=name2;
        name2=name1;
        name1=player;
    //If player score is higher than score two, replace second score and shift down
    } else if(total<score2){
        score5=score4;
        score4=score3;
        score3=score2;
        score2=total;

        name5=name4;
        name4=name3;
        name3=name2;
        name2=player;
    //If player score is higher than score three, replace third score and shift down
    } else if(total<score3){
        score5=score4;
        score4=score3;
        score3=total;

        name5=name4;
        name4=name3;
        name3=player;
    //If player score is higher than score four, replace fourth score and shift down
    } else if(total<score4){
        score5=score4;
        score4=total;

        name5=name4;
        name4=player;
    //If player score is higher than score five, replace fifth score
    } else {
        score5=total;

        name5=player;
    }

    //Write new scores to file
    ofstream outScr;
    
    outScr<<name1<<endl<<score1<<endl<<name2<<endl<<score2<<endl<<name3<<endl<<
            score3<<endl<<name4<<endl<<score4<<endl<<name5<<endl<<score5;
    
    outScr.close();

    //Output High Scores and prompt to play again
    cout<<"        High Scores\n"<<
          "1."<<setw(9)<<left<<name1<<setw(10)<<right<<(score1/60)<<" mins "<<(score1%60)<<" sec"<<endl<<
          "2."<<setw(9)<<left<<name2<<setw(10)<<right<<(score2/60)<<" mins "<<(score2%60)<<" sec"<<endl<<
          "3."<<setw(9)<<left<<name3<<setw(10)<<right<<(score3/60)<<" mins "<<(score3%60)<<" sec"<<endl<<
          "4."<<setw(9)<<left<<name4<<setw(10)<<right<<(score4/60)<<" mins "<<(score4%60)<<" sec"<<endl<<
          "5."<<setw(9)<<left<<name5<<setw(10)<<right<<(score5/60)<<" mins "<<(score5%60)<<" sec"<<endl;
}

