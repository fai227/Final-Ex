//----------Include----------
#include <iostream>
#include <curses.h>
#include "Setting.h"
#include "Puyo.h"



//----------Prototype Declaration----------
void MainScreen(int point);
void GameScreen(int colorNum);
void GetOptionData();
void SetOptionData();



//----------Global Variables----------
//key booleans
bool padPressed[4] = { false, false, false, false }; //up, left, down, right
bool buttonPressed[2] = { false, false }; //leftRotate, rightRotate(Enter button)

//key names
char padKeyName[4];
char buttonKeyName[2];

//Currently Directory
char currentDirectory[CHARBUFF];
char settingFile[CHARBUFF];



//----------Main Function----------
int main()
{
    //Initialize Variables
    getCurrentDirectory(currentDirectory);
    sprintf_s(settingFile, "%s\\Setting.ini", currentDirectory);

    //Initialize Screen
    initscr();
    noecho();


    //Start Key Thread

    //Get Settings
    GetOptionData();


    //Start MainScreen Without Point
    MainScreen(-1);    

    //Finish Key Thread
}




//----------Functions----------
//Display Main Screen
void MainScreen(int point) {

    char playString[] = "Play";
    char settingString[] = "Setting";
    char exitString[] = "Exit";


    //Set Screen Size
    resize_term(5, 5);


    //fps 15
    while (true) {
        //Clear Display
        erase();


        //Display
        refresh();
        napms(1000 / 15);
    }
}

//Display Game Screen
void GameScreen(int colorNum) {

}

//Get Option Keys
void GetOptionData() {
    char tmpChar[CHARBUFF];

    readChar("pad", "up", "w", tmpChar, settingFile);
    padKeyName[0] = tmpChar[0];

    readChar("pad", "left", "a", tmpChar, settingFile);
    padKeyName[1] = tmpChar[0];

    readChar("pad", "down", "s", tmpChar, settingFile);
    padKeyName[2] = tmpChar[0];

    readChar("pad", "right", "d", tmpChar, settingFile);
    padKeyName[3] = tmpChar[0];

    readChar("button", "left", "i", tmpChar, settingFile);
    buttonKeyName[0] = tmpChar[0];

    readChar("button", "right", "o", tmpChar, settingFile);
    buttonKeyName[1] = tmpChar[0];
}

//Set Options Keys
void SetOptionData() {
    char tmpChar[CHARBUFF];

    sprintf_s(tmpChar, "%c", padKeyName[0]);
    writeChar("pad", "up", tmpChar, settingFile);

    sprintf_s(tmpChar, "%c", padKeyName[1]);
    writeChar("pad", "left", tmpChar, settingFile);

    sprintf_s(tmpChar, "%c", padKeyName[2]);
    writeChar("pad", "down", tmpChar, settingFile);

    sprintf_s(tmpChar, "%c", padKeyName[3]);
    writeChar("pad", "right", tmpChar, settingFile);

    sprintf_s(tmpChar, "%c", buttonKeyName[0]);
    writeChar("button", "left", tmpChar, settingFile);

    sprintf_s(tmpChar, "%c", buttonKeyName[1]);
    writeChar("button", "right", tmpChar, settingFile);
}
