#include "DxLib.h"

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define FONT_SIZE 64

#define PIECE_COL 5
#define PIECE_ROW 5
#define PIECE_SIZE 50
#define REVERSE_NUM 5

#define BUTTON_WIDTH 350
#define BUTTON_HEIGHT 80
#define BUTTON_MARGIN 30
#define BUTTON_NUM 2

enum {
    PLAY,
    CLEAR,
    GAMEOVER,
    END,
} state;

void GamePlay();
void GameClear();
void GameOver();
void (*Method[])() = {
    GamePlay,
    GameClear,
    GameOver,
};

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

    // border
    DrawBox(origin.x + x * PIECE_SIZE, origin.y + y * PIECE_SIZE, origin.x + (x + 1) * PIECE_SIZE, origin.y + (y + 1) * PIECE_SIZE, GetColor(255, 255, 255), FALSE);
}

void GamePlay(){
    ClearDrawScreen();

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

        int clearFlag = 0;
        // print piece
        for(int y = 0; y < PIECE_ROW; y++){
            for(int x = 0; x < PIECE_COL; x++){
                DrawPiece(x, y, origin, piece[x][y]);
                if(piece[x][y] != RED){
                    clearFlag = -1;
                }
            }
        }

        if(clearFlag == 0){
            state = CLEAR;
            break;
        }
    }
}

void DrawMessage(const char str[]){
    int x = (DISPLAY_WIDTH - GetDrawStringWidth(str, -1)) / 2;
    int y = 30;
    DrawString(x, y, str, GetColor(255, 255, 0));
}

void DrawButton(Coordinate button[BUTTON_NUM]){
    char str[BUTTON_NUM][20] = {
        "NEW GAME",
        "EXIT GAME",
    };

    Coordinate origin;

    origin.x = DISPLAY_WIDTH / 2 - BUTTON_WIDTH / 2;
    origin.y = DISPLAY_HEIGHT / 2 - BUTTON_HEIGHT / 2 - (BUTTON_HEIGHT + BUTTON_MARGIN) * (BUTTON_NUM / 2);
    if(BUTTON_NUM % 2 == 0){
        origin.y += (BUTTON_HEIGHT + BUTTON_MARGIN) / 2;
    }

    for(int i = 0; i < BUTTON_NUM; i++){
        // button
        int x = origin.x;
        int y = origin.y + (BUTTON_HEIGHT + BUTTON_MARGIN) * i;
        DrawBox(x, y, x + BUTTON_WIDTH, y + BUTTON_HEIGHT, GetColor(100, 100, 100), TRUE);
        DrawBox(x, y, x + BUTTON_WIDTH, y + BUTTON_HEIGHT, GetColor(255, 255, 0), FALSE);  // border

        // memory button origin
        button[i].x = x;
        button[i].y = y;

        // text
        x = (DISPLAY_WIDTH - GetDrawStringWidth(str[i], -1)) / 2;
        y = y + (BUTTON_HEIGHT - FONT_SIZE) / 2;
        DrawString(x, y, str[i], GetColor(255, 255, 0));
    }
}

void SelectButton(Coordinate button[BUTTON_NUM]){
    Coordinate mouse;

    while(CheckHitKey(KEY_INPUT_ESCAPE) == 0 && ProcessMessage() == 0){
        if(isPress() == 0){
            // get pointer
            GetMousePoint(&mouse.x, &mouse.y);

            int i;
            for(i = 0; i < BUTTON_NUM; i++){
                if(
                    mouse.x >= button[i].x && mouse.x < button[i].x + BUTTON_WIDTH
                    && mouse.y >= button[i].y && mouse.y < button[i].y + BUTTON_HEIGHT
                ){
                    break;
                }
            }

            if(i == 0){
                    state = PLAY;
                    break;
            }
            else if(i == 1){
                    state = END;
                    break;
            }
        }
    }
}

void GameClear(){
    DrawMessage("CLEAR!!");
    WaitTimer(1000);

    Coordinate button[BUTTON_NUM];
    DrawButton(button);
    SelectButton(button);
}

void GameOver(){
    DrawMessage("GAME OVER");
    WaitTimer(1000);

    Coordinate button[BUTTON_NUM];
    DrawButton(button);
    SelectButton(button);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    ChangeWindowMode(TRUE);
    SetGraphMode(DISPLAY_WIDTH, DISPLAY_HEIGHT, 16);
    SetMainWindowText("PuzzleGameDxLib");

    if (DxLib_Init() == -1){
        return -1;
    }

    SetFontSize(FONT_SIZE);

    state = PLAY;
    while(CheckHitKey(KEY_INPUT_ESCAPE) == 0 && ProcessMessage() == 0 || state == END){
        Method[state]();
    }

    DxLib_End();

    return 0;
}
