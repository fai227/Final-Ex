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
void DrawPuyoPuyo(PuyoPuyo*);
void DrawChar(int, int, char, int);
void DrawString(int, int, const char*, int);
bool IsValidMove(PuyoPuyo*, int);
bool IsValidRotation(PuyoPuyo*, int);
void SetPuyoPositions(PuyoPuyo*, PuyoPuyo*, PuyoPuyo*);



//----------Global Variables----------
int field[HEIGHT][WIDTH] = {
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0,}
};

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
    init_pair(BLUE_PAIR, COLOR_CYAN, COLOR_BLACK);
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
            choose = 2;
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
        Wait(15);

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
        Wait(15);
    }

    MainScreen(0);
}



//Display Game Screen
void GameScreen(int colorNum) {
    //Initialize Field
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = 0;
        }
    }

    char string[BUFFSIZE];
    
    //Set Screen Size
    resize_term(16, 24);

    //Set puyos
    PuyoPuyo nowPuyo = *(new PuyoPuyo(colorNum));
    PuyoPuyo nextPuyo = *(new PuyoPuyo(colorNum));
    PuyoPuyo nextNextPuyo = *(new PuyoPuyo(colorNum));
    SetPuyoPositions(&nowPuyo, &nextPuyo, &nextNextPuyo);

    //Variables
    int score = 0;
    bool isGround;
    int fps = 30;
    int counter = 0;
    int dropDuration = 1000;

    while (true) {
        //Clear Display
        erase();

        //Reset Variables
        isGround = false;
        
        if (GetButtonDown(1)) { //Move Left
            if (IsValidMove(&nowPuyo, 1)) {
                nowPuyo.mainPuyoPositionX--;
            }
        } 
        if (GetButtonDown(3)) { //Move Right
            if (IsValidMove(&nowPuyo, 3)) {
                nowPuyo.mainPuyoPositionX++;
            }
        }

        if (GetButtonDown(4)) { //Rotate Right

        }
        if (GetButtonDown(5)) {  //Rotate Left

        }

        //Inputs
        if (GetButtonDown(0)) { //Fast Drop
            while (IsValidMove(&nowPuyo, 2)) {
                nowPuyo.mainPuyoPositionY++;
            }
            isGround = true;
            counter = 0;
        }
        else if (GetButtonDown(2)) {    //Move Down 
            if (IsValidMove(&nowPuyo, 2)) {
                nowPuyo.mainPuyoPositionY++;
            }
            else {
                isGround = true;
            }
            counter = 0;
        }

        if (GetButtonDown(6)) {  //Finish Game
            break;
        }

        //Move automatically down
        counter += fps;
        if (counter > dropDuration) {
            if (IsValidMove(&nowPuyo, 2)) {
                nowPuyo.mainPuyoPositionY++;
            }
            else {
                isGround = true;
            }
            counter = 0;
        }

        //Draw Border
        DrawString(1, 1, "--------------", WHITE_PAIR);
        DrawString(1, 14, "--------------", WHITE_PAIR);
        for (int i = 2; i <= 13; i++) {
            DrawChar(1, i, '|', WHITE_PAIR);
            DrawChar(14, i, '|', WHITE_PAIR);
        }
        //Draw Next and Next Next Puyo Border
        DrawString(17, 2, "----", WHITE_PAIR);
        DrawString(17, 5, "----", WHITE_PAIR);
        DrawString(17, 6, "----", WHITE_PAIR);
        DrawString(17, 9, "----", WHITE_PAIR);
        DrawChar(17, 3, '|', WHITE_PAIR);
        DrawChar(17, 4, '|', WHITE_PAIR);
        DrawChar(17, 7, '|', WHITE_PAIR);
        DrawChar(17, 8, '|', WHITE_PAIR);
        DrawChar(20, 3, '|', WHITE_PAIR);
        DrawChar(20, 4, '|', WHITE_PAIR);
        DrawChar(20, 7, '|', WHITE_PAIR);
        DrawChar(20, 8, '|', WHITE_PAIR);

        //Draw Fielf
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if(field[y][x])
                    DrawPuyo(x, y, field[y][x]);
            }
        }

        //Draw PuyoPuyos
        DrawPuyoPuyo(&nowPuyo);
        DrawPuyoPuyo(&nextPuyo);
        DrawPuyoPuyo(&nextNextPuyo);

        //Check Combo
        if (isGround) {
            //Set Puyo
            Vector2 directionVec = DirectionToVector(nowPuyo.puyoDirection);
            //Set Main Puyo            
            field[nowPuyo.mainPuyoPositionY][nowPuyo.mainPuyoPositionX] = nowPuyo.mainPuyo->color;
            //Set Sub Puyo
            field[nowPuyo.mainPuyoPositionY + directionVec.y][nowPuyo.mainPuyoPositionX + directionVec.x] = nowPuyo.subPuyo->color;
            //Generate Next Puyo
            nowPuyo = nextPuyo;
            nextPuyo = nextNextPuyo;
            nextNextPuyo = *(new PuyoPuyo(colorNum));
            SetPuyoPositions(&nowPuyo, &nextPuyo, &nextNextPuyo);
        }

        //Display
        refresh();
        Wait(fps);
    }

    //Go Back to Title Screen
    MainScreen(score);
}


//Draw Options
void DrawPuyo(int posX, int posY, int colorNum) {
    DrawString((posX + 1) * 2, posY + 2, "●", colorNum);
}
void DrawPuyoPuyo(PuyoPuyo* puyoPuyo) {
    Vector2 vec = DirectionToVector(puyoPuyo->puyoDirection);
    DrawPuyo(puyoPuyo->mainPuyoPositionX, puyoPuyo->mainPuyoPositionY, puyoPuyo->mainPuyo->color);
    DrawPuyo(puyoPuyo->mainPuyoPositionX + vec.x, puyoPuyo->mainPuyoPositionY + vec.y, puyoPuyo->subPuyo->color);
}
void DrawChar(int posX, int posY, char cr, int colorNum) {
    attron(COLOR_PAIR(colorNum));
    mvaddch(posY, posX, cr);
}
void DrawString(int posX, int posY, const char* string, int colorNum) {
    attron(COLOR_PAIR(colorNum));
    mvaddstr(posY, posX, string);
}

//Check the border and puyos in the positions given
bool IsValidMove(PuyoPuyo* puyoPuyo, int direction) {
    //Get direction vector
    Vector2 vec = DirectionToVector(direction);
    Vector2 subPuyoLocalPosition = DirectionToVector(puyoPuyo->puyoDirection);

    //check main puyo x position
    if (puyoPuyo->mainPuyoPositionX + vec.x < 0 || puyoPuyo->mainPuyoPositionX + vec.x >= WIDTH) return false;
    //check main puyo y position
    if (puyoPuyo->mainPuyoPositionY + vec.y < 0 || puyoPuyo->mainPuyoPositionY + vec.y >= HEIGHT) return false;
    //check no puyos in main puyo position
    if (field[puyoPuyo->mainPuyoPositionY + vec.y][puyoPuyo->mainPuyoPositionX + vec.x]) return false;

    //check sub puyo x position
    if (puyoPuyo->mainPuyoPositionX + subPuyoLocalPosition.x + vec.x < 0 || puyoPuyo->mainPuyoPositionX + subPuyoLocalPosition.x + vec.x >= WIDTH) return false;
    //check sub puyo y position
    if (puyoPuyo->mainPuyoPositionY + subPuyoLocalPosition.y + vec.y < 0 || puyoPuyo->mainPuyoPositionY + subPuyoLocalPosition.y + vec.y >= HEIGHT) return false;
    //check no puyos in sub puyo position
    if (field[puyoPuyo->mainPuyoPositionY + subPuyoLocalPosition.y + vec.y][puyoPuyo->mainPuyoPositionX + subPuyoLocalPosition.x + vec.x]) return false;

    return true;
}

void SetPuyoPositions(PuyoPuyo* nowPuyo, PuyoPuyo* nextPuyo, PuyoPuyo* nextNextPuyo) {
    nowPuyo->mainPuyoPositionX = 2;
    nowPuyo->mainPuyoPositionY = 1;

    nextPuyo->mainPuyoPositionX = 8;
    nextPuyo->mainPuyoPositionY = 2;

    nextNextPuyo->mainPuyoPositionX = 8;
    nextNextPuyo->mainPuyoPositionY = 6;
}
