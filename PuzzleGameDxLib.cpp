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
} Coordinate;

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

    Color piece[PIECE_COL][PIECE_ROW];
    Coordinate origin;

    // centering
    origin.x = DISPLAY_WIDTH / 2 - PIECE_SIZE * PIECE_COL / 2;
    origin.y = DISPLAY_HEIGHT / 2 - PIECE_SIZE * PIECE_ROW / 2;

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

    //int mouse.x, mouse.y;
    //int cell.x, cell.y;
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
