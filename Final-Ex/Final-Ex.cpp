//----------Include----------
#include <iostream>
#include <curses.h>
#include "Setting.h"
#include "Puyo.h"
#include "Module.h"
#include <time.h>

//----------Defines----------
#define WIDTH 6
#define HEIGHT 12

//----------Prototype Declaration----------
void MainScreen(int point);
void SettingScreen();
void GameScreen(int colorNum);
void DrawPuyo(int, int, int, bool);
void DrawPuyoPuyo(PuyoPuyo*);
void DrawChar(int, int, char, int);
void DrawString(int, int, const char*, int);
bool IsValidMove(PuyoPuyo*, int);
bool IsValidRotate(PuyoPuyo*, int);
void SetPuyoPositions(PuyoPuyo*, PuyoPuyo*, PuyoPuyo*);
void DrawField(PuyoPuyo*, PuyoPuyo*);
bool DropPuyo();
void CheckField(int[HEIGHT][WIDTH], int, int, int*, int);
void ShowScore(int);

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
    srand((unsigned int)time(NULL));

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
    MainScreen(0);    

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
    int highestPoint = 0;

    FILE* fp;
    errno_t error = fopen_s(&fp, "Rank.txt", "r");
    //Get Highest Point
    if (error == 0) {
        while (fgets(string, CHARBUFF, fp) != NULL) {
            int tmpPoint = atoi(string);
            if (tmpPoint > highestPoint) {
                highestPoint = tmpPoint;
            }
        }
        fclose(fp);
    }

    //Write Highest Point
    if (point > highestPoint) {
        highestPoint = point;
        error = fopen_s(&fp, "Rank.txt", "a");
        if (error == 0) {
            sprintf_s(string, "%d\n", highestPoint);
            fputs(string, fp);
            fclose(fp);
        }
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
    int textPlaceX = 16;
    int textPlaceY = 11;
    int dropDuration = 1000;

    while (true) {
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
            if (IsValidRotate(&nowPuyo, (nowPuyo.puyoDirection + 3) % 4)) {
                nowPuyo.puyoDirection = (nowPuyo.puyoDirection + 3) % 4;
            }
        }
        if (GetButtonDown(5)) {  //Rotate Left
            if (IsValidRotate(&nowPuyo, (nowPuyo.puyoDirection + 1) % 4)) {
                nowPuyo.puyoDirection = (nowPuyo.puyoDirection + 1) % 4;
            }
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

        //Show Display
        DrawField(&nextPuyo, &nextNextPuyo);
        //Show Score
        ShowScore(score);
        DrawPuyoPuyo(&nowPuyo);
        refresh();


        //Check Combo
        if (isGround) {
            //Set Puyo
            Vector2 directionVec = DirectionToVector(nowPuyo.puyoDirection);
            //Set Main Puyo            
            field[nowPuyo.mainPuyoPositionY][nowPuyo.mainPuyoPositionX] = nowPuyo.mainPuyo->color;
            //Set Sub Puyo
            field[nowPuyo.mainPuyoPositionY + directionVec.y][nowPuyo.mainPuyoPositionX + directionVec.x] = nowPuyo.subPuyo->color;

            //Start Combo
            int comboCounter = 0;
            while (true) {
                //Drop All Puyos
                while (DropPuyo()) {
                    DrawField(&nextPuyo, &nextNextPuyo);
                    refresh();
                    napms(250);
                }

                //Check Combo
                bool flag = false;

                int comboField[HEIGHT][WIDTH] = {};  //0:not checked 1:checked 2:no combo 3:combo
                for (int y = 0; y < HEIGHT; y++) {
                    for (int x = 0; x < WIDTH; x++) {
                        if (!comboField[y][x] && field[y][x]) {
                            int counter = 0;
                            CheckField(comboField, x, y, &counter, field[y][x]);
                            if (counter >= 4) flag = true;
                            for (int ty = 0; ty < HEIGHT; ty ++) {
                                for (int tx = 0; tx < WIDTH; tx++) {
                                    if (comboField[ty][tx] == 1) comboField[ty][tx] = counter >= 4 ? 3 : 2;
                                }
                            }
                        }
                    }
                }

                //Combo
                if (flag) {
                    //Change Puyo Style
                    for (int y = 0; y < HEIGHT; y++) {
                        for (int x = 0; x < WIDTH; x++) {
                            if (comboField[y][x] == 3) DrawPuyo(x, y, field[y][x], true);
                        }
                    }

                    //Display Combo
                    comboCounter++;
                    char tmpString[CHARBUFF];
                    sprintf_s(tmpString, "%02dCombo", comboCounter);
                    DrawString(textPlaceX, textPlaceY, tmpString, YELLOW_PAIR);
                    ShowScore(score);

                    refresh();
                    napms(1000);

                    //Destroy Puyo
                    int puyoCounter = 0;
                    for (int y = 0; y < HEIGHT; y++) {
                        for (int x = 0; x < WIDTH; x++) {
                            if (comboField[y][x] == 3) {
                                field[y][x] = 0;
                                puyoCounter++;
                            }
                        }
                    }
                    //Add Score
                    score += puyoCounter * comboCounter;
                }
                //No Combo
                else
                {
                    break;
                }
            }

            //Generate Next Puyo
            nowPuyo = nextPuyo;
            nextPuyo = nextNextPuyo;
            nextNextPuyo = *(new PuyoPuyo(colorNum));
            SetPuyoPositions(&nowPuyo, &nextPuyo, &nextNextPuyo);
            if (field[1][2] || field[0][2]) break;
        }

        Wait(fps);
    }

    erase();
    DrawString(8, 7, "GameOver", YELLOW_PAIR);

    refresh();
    napms(2000);


    //Go Back to Title Screen
    MainScreen(score);
}


//Draw Options
void DrawPuyo(int posX, int posY, int colorNum, bool flag) {
    DrawString((posX + 1) * 2, posY + 2, flag ? "()":"●", colorNum);
}
void DrawPuyoPuyo(PuyoPuyo* puyoPuyo) {
    Vector2 vec = DirectionToVector(puyoPuyo->puyoDirection);
    DrawPuyo(puyoPuyo->mainPuyoPositionX, puyoPuyo->mainPuyoPositionY, puyoPuyo->mainPuyo->color, false);
    DrawPuyo(puyoPuyo->mainPuyoPositionX + vec.x, puyoPuyo->mainPuyoPositionY + vec.y, puyoPuyo->subPuyo->color, false);
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

//Check the rotaion
bool IsValidRotate(PuyoPuyo* puyoPuyo, int direction) {
    Vector2 vec = DirectionToVector(direction);

    //check sub puyo x position
    if (puyoPuyo->mainPuyoPositionX + vec.x < 0 || puyoPuyo->mainPuyoPositionX + vec.x >= WIDTH) return false;
    //check sub puyo y position
    if (puyoPuyo->mainPuyoPositionY + vec.y < 0 || puyoPuyo->mainPuyoPositionY + vec.y >= HEIGHT) return false;
    //check no puyos in sub puyo position
    if (field[puyoPuyo->mainPuyoPositionY + vec.y][puyoPuyo->mainPuyoPositionX + vec.x]) return false;

    return true;
}

//Set Puyo Positions
void SetPuyoPositions(PuyoPuyo* nowPuyo, PuyoPuyo* nextPuyo, PuyoPuyo* nextNextPuyo) {
    nowPuyo->mainPuyoPositionX = 2;
    nowPuyo->mainPuyoPositionY = 1;

    nextPuyo->mainPuyoPositionX = 8;
    nextPuyo->mainPuyoPositionY = 2;

    nextNextPuyo->mainPuyoPositionX = 8;
    nextNextPuyo->mainPuyoPositionY = 6;
}

//Display Field
void DrawField(PuyoPuyo* nextPuyo, PuyoPuyo* nextNextPuyo) {
    //Clear Display
    erase();

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
            if (field[y][x])
                DrawPuyo(x, y, field[y][x], false);
        }
    }

    DrawPuyoPuyo(nextPuyo);
    DrawPuyoPuyo(nextNextPuyo);
}

//Drop Puyos returns false if no puyos can be dropped
bool DropPuyo() {
    bool flag = false;
    
    for (int x = 0; x < WIDTH; x++) {
        for (int y = HEIGHT - 2; y >= 0; y--) {
            //there is a puyo and position one down is empty
            if (field[y][x] && !field[y + 1][x]) {
                field[y + 1][x] = field[y][x];
                field[y][x] = 0;
                flag = true;
            }
        }
    }

    return flag;
}

//Check Field Rensa
void CheckField(int comboField[HEIGHT][WIDTH], int x, int y, int* counter, int color) {
    //Check Field Border
    if (x < 0 || y < y) return;
    if (x >= WIDTH || y >= HEIGHT) return;

    //Check Color
    if (!comboField[y][x] && field[y][x] == color) {
        comboField[y][x] = 1;
    }
    else {
        return;
    }
    
    *counter = *counter + 1;

    //Check Left
    CheckField(comboField, x - 1, y, counter, color);
    //Check Right
    CheckField(comboField, x + 1, y, counter, color);
    //Check Up
    CheckField(comboField, x, y - 1, counter, color);
    //Check Down
    CheckField(comboField, x, y + 1, counter, color);
}

//Show Score
void ShowScore(int score) {
    int x = 16;
    int y = 13;

    char string[CHARBUFF];
    sprintf_s(string, "%06d", score);
    DrawString(x, y, "Score", WHITE_PAIR);
    DrawString(x, y + 1, string, WHITE_PAIR);
}