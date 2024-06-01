#include <iostream>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <conio.h>

using namespace std;


//Size and speed of the game window
const int width = 80;
const int height = 25;
int speed = 20;

bool gameOver;
int x, y, aX, aY, score, color, len;
int tailX[999], tailY[999];
enum eColors {WHITE = 0, RED, GREEN, YELLOW};
enum eDir {NONE = 0, UP, DOWN, LEFT, RIGHT};
eColors colors;
eDir direction;


void drawChar(char c, eColors color){
    switch(color){
        case WHITE:
            cout << "\033[0m";
            cout << c;
            break;
        case RED:
            cout << "\033[31m" << c;
            break;
        case GREEN:
            cout << "\033[32m" << c;
            break;
        case YELLOW:
            cout << "\033[33m" << c;
            break;
    }
}

void newApple(){
    srand(time(NULL));
    while(true){
        int tailTouch = 0;
        aX = (rand() % (width-2))+1;
        aY = (rand() % (height-3))+1;
        for (int i = 0; i< len ; i++) {
            if (tailX [i] == aX && tailY [i] == aY) {
                tailTouch++;
            }
        }
        if((aX != x && aY != y) && (tailTouch==0)){
            return;
        }

    }
}

void Setup(){
    gameOver = false;
    x = width/2;
    y = height/2;
    direction = NONE;
    newApple();
    system("cls");
}

void clrscr(){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)0, (SHORT)0 };
    SetConsoleCursorPosition(hOut, coord);
}

void drawBoard(){
    clrscr();
    for(int bY=0; bY<height-1; bY++){
        if(bY==0||bY==(height-2)){
            for(int i = 0; i < width+1; i++){
                drawChar('$',WHITE);
            }
        }else{
            for(int bX=0; bX<width; bX++){
                bool space = true;
                //Draw the sides
                if(bX==0||bX==width-1){
                    drawChar('$',WHITE);
                }
                //Draw the snake head
                if(bX == x && bY == y){
                    drawChar('+',YELLOW);
                    space = false;
                }
                //Draw the apple
                if(bX == aX && bY == aY){
                    drawChar('@',RED);
                    space = false;
                }
                for (int i = 0; i< len ; i++) {
                    if (tailX[i] == bX && tailY[i] == bY) {
                        drawChar('*',GREEN);
                        space = false;
                    }
                }
                if(space){
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
    cout << "Score: " << score;
}

void input(){
    if(_kbhit()){
        switch(_getch()){
            case 'w':
                direction = UP;
                break;
            case 'a':
                direction = LEFT;
                break;
            case 's':
                direction = DOWN;
                break;
            case 'd':
                direction = RIGHT;
                break;
            case 'q':
                gameOver = true;
                break;
        }
    }
}

void simulate(){
    int prevX = tailX [0];
    int prevY = tailY [0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for(int i = 1;i < len ; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(direction){
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case NONE:
            break;
    }
    if(x<=0||x>=width||y<=0||y>=(height-2)){
        gameOver=true;
        return;
    }
    for(int i = 0; i < len ; i++) {
        if(tailX[i]==x&&tailY[i]==y){
            gameOver=true;
            return;
        }
    }
    if(x==aX&&y==aY){
        newApple();
        score = score + 10;
        speed=speed+4;
        len++;
        return;
    }
}

void wait(){
    #ifdef _WIN32
        Sleep(10000/speed);
    #else
    #endif
}

int main(){
    Setup();
    while(!gameOver){
        drawBoard();
        wait();
        input();
        simulate();
    }
    cout << endl << "Thank you for playing! Color Console Snake by Anthony Clemens 2024\n";
    system("pause");
    return 0;
}