//----------Include----------
#include <iostream>
#include <curses.h>
#include "Setting.h"
#include "Puyo.h"
#include "Module.h"


//----------Defines----------
#define WIDTH 6
#define HEIGHT 12

//----------Prototype Declaration----------
void MainScreen(int point);
void SettingScreen();
void GameScreen(int colorNum);
void DrawPuyo(int, int, int);
void DrawPuyoPuyo(int, int, int, PuyoPuyo*);
void DrawChar(int, int, char, int);
void DrawString(int, int, const char*, int);


//----------Global Variables----------


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
    start_color();
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(PURPLE_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);
    

    //Start Key Input
    GetOptionData(settingFile);
    StartInput();


    //Start MainScreen Without Point
    //MainScreen(0);    
    //SettingScreen();
    GameScreen(4);

    //Stop Key Input
    StopInput();
}




//----------Functions----------
//Display Main Screen
void MainScreen(int point) {

    char playString[] = "Play";
    char settingString[] = "Setting";
    char exitString[] = "Exit";

    char string[CHARBUFF];

    //Set Screen Size
    resize_term(14, 11);

    //Get Ranking
    int highestPoint = readInt("Rank", "Point", 0, settingFile);
    if (point > highestPoint) {
        highestPoint = point;
        writeInt("Rank", "Point", point, settingFile);
    }

    int choose = 0;
    int difficulty = 4;
    int posArray[] = {3, 10, 12};
    int nextScene = -1;

    //fps 15
    while (true) {
        //Clear Display
        erase();

        //Key Inputs
        if (GetButtonDown(0)) {    //Up
            choose = (choose + 2) % 3;
        }
        if (GetButtonDown(2)) {   //Down
            choose = (choose + 1) % 3;
        }
        if (GetButtonDown(4)) {  //Enter
            nextScene = choose;
        }
        if (GetButtonDown(3)) {  //Right
            if (difficulty < 5) difficulty++;
        }
        if (GetButtonDown(1)) {  //Left
            if (difficulty > 3) difficulty--;
        }
        if (GetButtonDown(6)) {  //Exit
            nextScene = 0;
        }
        if (GetButtonDown(5)) {
            choose = 2;
        }

        //DrawTitle
        DrawString(2, 1, "PuyoPuyo", GREEN_PAIR);

        //Draw Highest Score
        sprintf_s(string, "%06d", highestPoint);
        DrawString(3, 4, "Highest", WHITE_PAIR);
        DrawString(4, 5, string, WHITE_PAIR);

        //Draw Difficulty
        DrawString(1, 7, "PuyoColor", WHITE_PAIR);
        sprintf_s(string, "%d", difficulty);
        DrawString(5, 8, string, WHITE_PAIR);

        //Draw Texts
        DrawString(3, posArray[0], playString, choose == 0 ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, posArray[1], settingString, choose == 1 ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, posArray[2], exitString, choose == 2 ? YELLOW_PAIR : WHITE_PAIR);

        //Draw Pointer
        DrawChar(1, posArray[choose], '*', YELLOW_PAIR);

        //Display
        refresh();
        Wait(1000 / 15);

        if (nextScene >= 0) break;
    }

    //Go to Next Scene
    if (nextScene == 0) GameScreen(difficulty);
    else if (nextScene == 1) SettingScreen();
}

//Display Setting Screen
void SettingScreen() {
    //Set Screen Size
    resize_term(11, 13);

    bool isChoosing = false;
    int choose = 0;

    while (true) {
        //Clear Display
        erase();

        if (GetButtonDown(5) || GetButtonDown(6)) { //Leave Setting
            break;
        }
        if (GetButtonDown(4)) {  //Choose
            isChoosing = !isChoosing;
        }
        if (!isChoosing) {
            if (GetButtonDown(0)) {  //Up
                choose = (choose + 6) % 7;
            }
            if (GetButtonDown(2)) { //Down
                choose = (choose + 1) % 7;
            }
        }

        //Show Title
        DrawString(3, 1, "Setting", GREEN_PAIR);

        //Show Name
        DrawString(3, 3, "Up", choose == 0 && isChoosing ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, 4, "Left", choose == 1 && isChoosing ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, 5, "Down", choose == 2  && isChoosing ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, 6, "Right", choose == 3 && isChoosing ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, 7, "Enter", choose == 4 && isChoosing ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, 8, "Cancel", choose == 5 && isChoosing ? YELLOW_PAIR : WHITE_PAIR);
        DrawString(3, 9, "Exit", choose == 6 && isChoosing ? YELLOW_PAIR : WHITE_PAIR);

        //Show Key Name
        for (int i = 0; i < KEYLENGTH; i++) {
            DrawChar(11, 3 + i, buttonName[i], WHITE_PAIR);
        }

        //Show Pointer(Choose)
        DrawChar(1, 3 + choose, '*', YELLOW_PAIR);

        if (isChoosing) {
            //Set Key
            char ch = getch();
            SetOptionData(choose, ch, settingFile);
            isChoosing = false;
        }

        //Display
        refresh();
        Wait(1000 / 15);
    }

    MainScreen(0);
}

//Display Game Screen
void GameScreen(int colorNum) {
    char string[BUFFSIZE];
    
    //Set Screen Size
    resize_term(16, 24);

    int field[HEIGHT][WIDTH] = {
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 5, 0, 0, 0, 0,},
        {4, 5, 0, 0, 0, 0,},
        {5, 5, 0, 0, 0, 0,},
        {4, 2, 1, 0, 0, 0,},
        {4, 4, 2, 1, 0, 0,},
        {2, 2, 1, 1, 0, 0,}
    };

    while (true) {
        //Clear Display
        erase();

        //Draw Border
        DrawString(1, 1, "--------------", WHITE_PAIR);
        DrawString(1, 14, "--------------", WHITE_PAIR);
        for (int i = 2; i <= 13; i++) {
            DrawChar(1, i, '|', WHITE_PAIR);
            DrawChar(14, i, '|', WHITE_PAIR);
        }

        //Draw Fielf
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if(field[y][x])
                    DrawPuyo(x, y, field[y][x]);
            }
        }

        //Display
        refresh();
        //fps30
        Wait(1000 / 30);
    }
}


//Draw Options
void DrawPuyo(int posX, int posY, int colorNum) {
    DrawString((posX + 1) * 2, posY + 2, "〇", colorNum);
}
void DrawPuyoPuyo(int posX, int posY, int direction, PuyoPuyo* puyoPuyo) {

}
void DrawChar(int posX, int posY, char cr, int colorNum) {
    attron(COLOR_PAIR(colorNum));
    mvaddch(posY, posX, cr);
}
void DrawString(int posX, int posY, const char* string, int colorNum) {
    attron(COLOR_PAIR(colorNum));
    mvaddstr(posY, posX, string);
}
