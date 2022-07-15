#pragma once

#include <windows.h>

#define BUFFSIZE 16
#define KEYLENGTH 7 

struct Vector2 {
    int x;
    int y;

    Vector2() {
        x = 0;
        y = 0;
    };

    Vector2(int value) {
        x = value;
        y = value;
    };

    Vector2(int xValue, int yValue) {
        x = xValue;
        y = yValue;
    };
};

//Convert direction number to direction vector
Vector2 DirectionToVector(int direction) {
    switch (direction)
    {
    case 0:
        return *(new Vector2(0, -1));
        break;

    case 1:
        return *(new Vector2(-1, 0));
        break;

    case 2:
        return *(new Vector2(0, 1));
        break;

    case 3:
        return *(new Vector2(1, 0));
        break;

    default:
        return *(new Vector2(0));
        break;
    }
}

//----------Input----------
//Section Name
char section[] = "Input";
//Boolean of Button
bool buttonPressed[KEYLENGTH] = { false, false, false , false , false , false , false };
//Button Name
char buttonName[KEYLENGTH] = { 'w', 'a', 's', 'd', 'o', 'i', 'e' };

//Prototype Declaration
void ButtonThread();
void StartInput();
void StopInput();
bool GetButtonDown(int);
void SetOptionData(int, char,const char*);
void GetOptionData(const char*);
void ClearInput();
void Wait(int);

//Thread for Keying
HANDLE thread;
void ButtonThread() {
    char ch;
    while (true) {
        ch = getch();
        for (int i = 0; i < KEYLENGTH; i++) {
            if (ch == buttonName[i]) {
                buttonPressed[i] = true;
            }
        }
    }
}

//Start Input Managing
void StartInput() {
    //Start Button Thread
    DWORD threadID;
    thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ButtonThread, NULL, 0, &threadID);
}

void StopInput() {
    CloseHandle(thread);
}

//Boolean for Getting Button Down
bool GetButtonDown(int n) {
    return buttonPressed[n];
}

//Set Options Keys
void SetOptionData(int keyNum, char keyName, const char* fileDirectory) {
    //Set keyword and value into string
    char keyWord[BUFFSIZE];
    sprintf_s(keyWord, "%d", keyNum);
    char value[BUFFSIZE];
    sprintf_s(value, "%c", keyName);
    
    //Save Option
    writeChar(section, keyWord, value, fileDirectory);

    //Load Option
    GetOptionData(fileDirectory);
}

//Get Option Keys
void GetOptionData(const char* fileDirectory) {
    char keyWord[BUFFSIZE];
    char keyNameChar[BUFFSIZE];

    for (int i = 0; i < KEYLENGTH; i++) {
        sprintf_s(keyWord, "%d", i);
        readChar(section, keyWord, "a", keyNameChar, fileDirectory);
        buttonName[i] = keyNameChar[0];
    }
}

//Clear Input
void ClearInput() {
    for (int i = 0; i < KEYLENGTH; i++) {
        buttonPressed[i] = false;
    }
}

void Wait(int n) {
    ClearInput();
    napms(1000 / n);
}