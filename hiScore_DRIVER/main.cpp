 /* 
 * File:   main.cpp
 * Author: Nathan Ellett
 * Created on February 14, 2023, 11:48 AM
 * Purpose:
 */

//System Libraries
#include <iostream>  //Input/Output Library
#include <cstdlib>  //C-Standard Library
#include <ctime>    //C-Time Library
#include <iomanip>  //Input/Output Manipulation Library
#include <fstream>  //File Stream Library
using namespace std;

//Execution Begins Here!
int main(int argc, char** argv) {
    int intTime=time(0);
    cout<<"Welcome to minesweeper! Press \"Enter\" to begin.";
    cin.get();
    system("clear");
    
    //Begin time (For high-score)
    int total=time(0)-intTime;
    
    
    //CODE PULLED FROM YAHTZEE PROJECT
    //Five names read in from score file and player name
    char player[9];
    string name1, name2, name3, name4, name5;
    //Five scores read in from file, corresponds with names above
    int score1, score2, score3, score4, score5;
    
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
    if(total<score1){
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
    outScr.open("highscores.txt");
    outScr<<name1<<endl<<score1<<endl<<name2<<endl<<score2<<endl<<
            name3<<endl<<score3<<endl<<name4<<endl<<score4<<endl<<
            name5<<endl<<score5;
    outScr.close();

    //Output High Scores and prompt to play again
    cout<<"     High Scores\n"<<
          "1."<<setw(9)<<left<<name1<<setw(10)<<right<<(score1/60)<<" mins "<<(score1%60)<<" sec"<<endl<<
          "2."<<setw(9)<<left<<name2<<setw(10)<<right<<(score2/60)<<" mins "<<(score2%60)<<" sec"<<endl<<
          "3."<<setw(9)<<left<<name3<<setw(10)<<right<<(score3/60)<<" mins "<<(score3%60)<<" sec"<<endl<<
          "4."<<setw(9)<<left<<name4<<setw(10)<<right<<(score4/60)<<" mins "<<(score4%60)<<" sec"<<endl<<
          "5."<<setw(9)<<left<<name5<<setw(10)<<right<<(score5/60)<<" mins "<<(score5%60)<<" sec"<<endl;
    return 0;
}

