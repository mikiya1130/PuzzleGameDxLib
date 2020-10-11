#include "DxLib.h"

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define BOARD_WIDTH 5
#define BOARD_HEIGHT 5
#define PIECE_SIZE 50

typedef enum {RED, BLUE} Color;

typedef struct {
    int x;
    int y;
} Origin;

void DrawPiece(int x, int y, Origin origin, Color color){
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

    Color piece[BOARD_WIDTH][BOARD_HEIGHT];
    Origin pieceOrigin;

    // centering
    pieceOrigin.x = DISPLAY_WIDTH / 2 - PIECE_SIZE * BOARD_WIDTH / 2;
    pieceOrigin.y = DISPLAY_HEIGHT / 2 - PIECE_SIZE * BOARD_HEIGHT / 2;

    // init color
    for(int y = 0; y < BOARD_HEIGHT; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
            if((x + y) % 2 == 0){
                piece[x][y] = RED;
            }
            else{
                piece[x][y] = BLUE;
            }
        }
    }

    // print piece
    for(int y = 0; y < BOARD_HEIGHT; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
            DrawPiece(x, y, pieceOrigin, piece[x][y]);
        }
    }

    WaitKey();

    DxLib_End();

    return 0;
}
