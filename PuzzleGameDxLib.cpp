﻿#include "DxLib.h"

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define PIECE_COL 5
#define PIECE_ROW 5
#define PIECE_SIZE 50
#define REVERSE_NUM 5

typedef enum {RED, BLUE} Color;

typedef struct {
    int x;
    int y;
} Coordinate;

void GenerateReverseCell(Coordinate *cell){
    int n = 0;
    while(n < REVERSE_NUM){
        cell[n].x = GetRand(PIECE_COL - 1);
        cell[n].y = GetRand(PIECE_ROW - 1);

        int flag = 0;
        for(int i = 0; i < n; i++){
            if(cell[i].x == cell[n].x && cell[i].y == cell[n].y){
                flag = -1;
                break;
            }
        }

        if(flag != -1){
            n++;
        }
    }
}

void ReversePiece(Color piece[PIECE_COL][PIECE_ROW], Coordinate cell){
    for(int i = cell.x - 1; i <= cell.x + 1; i++){
        for(int j = cell.y - 1; j <= cell.y + 1; j++){
            if(
                i >= 0 && i < PIECE_COL
                && j >= 0 && j < PIECE_ROW
            ){
                switch(piece[i][j]){
                    case RED:
                        piece[i][j] = BLUE;
                        break;
                    case BLUE:
                        piece[i][j] = RED;
                        break;
                }
            }
        }
    }
}

void InitPieceColor(Color piece[PIECE_COL][PIECE_ROW]){
    for(int y = 0; y < PIECE_ROW; y++){
        for(int x = 0; x < PIECE_COL; x++){
                piece[x][y] = RED;
        }
    }

    Coordinate reverseCell[REVERSE_NUM];
    GenerateReverseCell(reverseCell);
    for(int i = 0; i < REVERSE_NUM; i++){
        ReversePiece(piece, reverseCell[i]);
    }
}

// check mouse left key down
// return 0: true, return -1: false
int isPress(){
    static int flag = 0;

    if(GetMouseInput() && MOUSE_INPUT_LEFT){
        if(flag == 0){  // down
            flag = 1;
            return 0;
        }
        else if(flag == 1){  // hold down
            flag = 2;
        }
    }
    else{
        flag = 0;  // up
    }

    return -1;
}

void DrawPiece(int x, int y, Coordinate origin, Color color){
    switch(color){
        case RED:
            DrawBox(origin.x + x * PIECE_SIZE, origin.y + y * PIECE_SIZE, origin.x + (x + 1) * PIECE_SIZE, origin.y + (y + 1) * PIECE_SIZE, GetColor(255, 0, 0), TRUE);
            break;
        case BLUE:
            DrawBox(origin.x + x * PIECE_SIZE, origin.y + y * PIECE_SIZE, origin.x + (x + 1) * PIECE_SIZE, origin.y + (y + 1) * PIECE_SIZE, GetColor(0, 0, 255), TRUE);
            break;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    ChangeWindowMode(TRUE);
    SetGraphMode(DISPLAY_WIDTH, DISPLAY_HEIGHT, 16);
    SetMainWindowText("PuzzleGameDxLib");

    if (DxLib_Init() == -1){
        return -1;
    }

    Coordinate origin;

    // centering
    origin.x = DISPLAY_WIDTH / 2 - PIECE_SIZE * PIECE_COL / 2;
    origin.y = DISPLAY_HEIGHT / 2 - PIECE_SIZE * PIECE_ROW / 2;

    Color piece[PIECE_COL][PIECE_ROW];

    InitPieceColor(piece);

    Coordinate mouse, cell;

    int downFlag = 0;
    while(CheckHitKey(KEY_INPUT_ESCAPE) == 0 && ProcessMessage() == 0){
        if(isPress() == 0){
            // get pointer
            GetMousePoint(&mouse.x, &mouse.y);

            // reverse check
            cell.x = (mouse.x - origin.x) / PIECE_SIZE;
            cell.y = (mouse.y - origin.y) / PIECE_SIZE;
            if(
                cell.x >= 0 && cell.x < PIECE_COL
                && cell.y >= 0 && cell.y < PIECE_ROW
                && mouse.x >= origin.x
                && mouse.y >= origin.y
            ){
                ReversePiece(piece, cell);
            }
        }

        // print piece
        for(int y = 0; y < PIECE_ROW; y++){
            for(int x = 0; x < PIECE_COL; x++){
                DrawPiece(x, y, origin, piece[x][y]);
            }
        }
    }

    DxLib_End();

    return 0;
}
