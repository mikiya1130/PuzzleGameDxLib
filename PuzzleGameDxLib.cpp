#include "DxLib.h"

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define PIECE_COL 5
#define PIECE_ROW 5
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

    Color piece[PIECE_COL][PIECE_ROW];
    Origin pieceOrigin;

    // centering
    pieceOrigin.x = DISPLAY_WIDTH / 2 - PIECE_SIZE * PIECE_COL / 2;
    pieceOrigin.y = DISPLAY_HEIGHT / 2 - PIECE_SIZE * PIECE_ROW / 2;

    // init color
    for(int y = 0; y < PIECE_ROW; y++){
        for(int x = 0; x < PIECE_COL; x++){
            if((x + y) % 2 == 0){
                piece[x][y] = RED;
            }
            else{
                piece[x][y] = BLUE;
            }
        }
    }

    // print piece
        for(int y = 0; y < PIECE_ROW; y++){
            for(int x = 0; x < PIECE_COL; x++){
            DrawPiece(x, y, pieceOrigin, piece[x][y]);
        }
    }

    WaitKey();

    DxLib_End();

    return 0;
}
