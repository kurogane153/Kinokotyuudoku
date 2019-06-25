#include "DxLib.h"
#include "math.h"
#include <string>
#define HEIGHT 200
#define WIDTH 15
#define SIZE 32

const float G=9.8067;
const float y_max=2.000;
const float v0=4.0624;
const float v=-0.126953125;

/*列挙体*/
typedef enum GAME_MODE{
	GAME_TITLE,
	GAME_INT,
	GAME_END,
	GAME_MAIN,
	GAME_CLEAR,
	GAME_OVER,
	END=99
};

typedef	enum PlayerDire{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	JUMP,
	NL,
};
//構造体
struct Field{
	int flg;
	int x;
	int y;
	int img[5];
};
struct Player{
	int x;
	float y;
	int dire;
	int JumpFlg;
	int img[15];
};

struct Player	g_player;
struct Field	g_Field[WIDTH][HEIGHT];
char FildDete[HEIGHT][WIDTH];
int FieldDate[HEIGHT][WIDTH]=
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//0.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//1.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//2.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//3.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//4.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//5.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//6.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//7.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//8.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//9.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//10.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//11.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//12.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//13.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//14.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//13.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//14.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//13.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//14.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//13.0

};

//グローバル変数
int g_GameState=GAME_INT;
int g_OldKey;
int g_NowKey;
int g_KeyFlg;
int g_FiledImage[84];

//プロトタイプ宣言
int LoadImages(void);
void GameInit(void);
void FieldInit(void);
void PlayerInit(void);
void GameMain(void);
void DrawField(void);
void DrawPlayer(void);
void PlayerControl(void);
int CollisionDetection(int i);
int ReadSteag(void);

//メイン
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
										 LPSTR lpCmdLine, int nCmdShow )
{
	// タイトルを test に変更
	SetMainWindowText("ブロック崩し");
	ChangeWindowMode(TRUE);		// ウィンドウモードで起動
	SetGraphMode(640,480,16);
	if ( DxLib_Init() == -1 ) return -1;	// DXライブラリの初期化処理
	SetDrawScreen( DX_SCREEN_BACK );	// 描画先画面を裏にする
	if ( LoadImages() == -1 ) return -1; // 画像読込み関数を呼び出し
	SetFontSize( 20 );		// 文字サイズを設定
	// ゲームループ
	while ( ProcessMessage() == 0 && g_GameState != 99){
		// 入力キー取得
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState( DX_INPUT_KEY_PAD1 );
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();		// 画面の初期化

		switch (g_GameState){
			case GAME_TITLE:
				break;
			case GAME_INT:
				GameInit();
				break;
			case GAME_END:
				break;
			case GAME_MAIN:
				GameMain();
				break;
			case GAME_CLEAR:
				break;
			case GAME_OVER:
				break;
			}
		ScreenFlip();			// 裏画面の内容を表画面に反映
	}
	DxLib_End();	// DXライブラリ使用の終了処理
	return 0;	// ソフトの終了
}

//画像読み込み
int LoadImages(void)
{
	if (LoadDivGraph("images/backimage.png",84,12,7,32,32,g_FiledImage)==-1)	return -1;
	if (LoadDivGraph("images/mario_chara.png",15,5,3,32,32,g_player.img)==-1)	return -1;
	return 0;
}

/*ランキングデータ読み込み*/
int ReadSteag(void)
{
	FILE *fp;
	#pragma warning(disable:4996)

	//ファイルオープン
	if((fp = fopen("mario.txt","r"))== NULL){
		//エラー処理
		printf("Ranking Data Error\n");
		return -1;
	}

	//ランキングデータ配分列データを読み込む
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;i<WIDTH;i++){
			fscanf(fp, "%d",FieldDate[j][i]);
			//FieldDate[j][i]=FildDete[i][j];
			g_Field[j][i].x=j;
			g_Field[j][i].y=i;
		}
	}
	
	//ファイルクローズ
	fclose(fp);

	return 0;
}

//初期処理
void GameInit(void)
{
	//ReadSteag();
	//FieldInit();
	PlayerInit();
	g_GameState=GAME_MAIN;
}

//プレイヤー初期処理
void PlayerInit(void)
{
	g_player.x=64;
	g_player.y=384.0;
	g_player.JumpFlg=0;
}


//フィールド初期処理
void FieldInit(void)
{
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			g_Field[j][i].x=j;
			g_Field[j][i].y=i;
			g_Field[j][i].flg=FieldDate[j][i];
		}
	}
}

//ゲームメイン
void GameMain(void)
{
	DrawField();
	DrawPlayer();
	PlayerControl();
}

//フィールド描画
void DrawField(void)
{
	for(int i=0; i < HEIGHT; i++){
		for(int j=0; j < WIDTH; j++){
			if(g_Field[j][i].flg==0){
				DrawGraph(i*SIZE,j*SIZE,g_FiledImage[84],TRUE);
			}else{
				DrawGraph(i*SIZE,j*SIZE,g_FiledImage[FieldDate[i][j]],TRUE);
			}
		}
	}
	for(int i=0; i < HEIGHT; i++){
		for(int j=0; j < WIDTH; j++){
			DrawFormatString(i*SIZE+10,j*SIZE+10, 0xff00000,"%d",FieldDate[i][j]);
		}
	}
	DrawFormatString(500,80, 0xff00000,"%d",g_player.x);
	DrawFormatString(500,100, 0xff00000,"%d",g_player.y);
}

//プレイヤー描画
void DrawPlayer(void)
{
	static int cnt=0;
	if(g_NowKey){
		if(g_player.dire==RIGHT){
			if(cnt<=4){
				DrawGraph(g_player.x,g_player.y,g_player.img[1],TRUE);
			}else if(cnt<=8){
				DrawGraph(g_player.x,g_player.y,g_player.img[2],TRUE);
			}else if(cnt>8){
				DrawGraph(g_player.x,g_player.y,g_player.img[1],TRUE);
				cnt=0;
			}
		}else if(g_player.dire==LEFT){
			if(cnt<=4){
				DrawTurnGraph(g_player.x,g_player.y,g_player.img[1],TRUE);
			}else if(cnt<=8){
				DrawTurnGraph(g_player.x,g_player.y,g_player.img[2],TRUE);
			}else if(cnt>8){
				DrawTurnGraph(g_player.x,g_player.y,g_player.img[1],TRUE);
				cnt=0;
			}
		}else if(g_player.dire==JUMP){
			if(cnt<=4){
				DrawGraph(g_player.x,g_player.y,g_player.img[3],TRUE);
			}else if(cnt<=8){
				DrawGraph(g_player.x,g_player.y,g_player.img[4],TRUE);
			}else if(cnt>8){
				DrawGraph(g_player.x,g_player.y,g_player.img[4],TRUE);
				cnt=5;
			}
		}
	}else{
		DrawGraph(g_player.x,g_player.y,g_player.img[0],TRUE);
	}
	cnt++;
}

//プレイヤー移動
void PlayerControl(void)
{
	static int y_prev,y_temp,faib,cnt=0;
	if(g_KeyFlg & PAD_INPUT_UP)	g_player.dire=UP;
	if(g_KeyFlg & PAD_INPUT_RIGHT)	g_player.dire=RIGHT;
	if(g_KeyFlg & PAD_INPUT_DOWN)	g_player.dire=DOWN;
	if(g_KeyFlg & PAD_INPUT_LEFT)	g_player.dire=LEFT;
	if(g_KeyFlg & PAD_INPUT_M){
		g_player.dire=JUMP;
		if(g_player.JumpFlg==0){
			g_player.JumpFlg=1;
			y_prev=g_player.y;
			g_player.y=g_player.y-16;
		}
	}
	if(!g_NowKey)	g_player.dire=NL;
	switch (g_player.dire)
	{
	case RIGHT:
		if(g_Field[(int)g_player.y/SIZE][g_player.x/32+1].flg==0){ //右の当たり判定
			int posX=g_player.x+32;
			int posY=g_player.y;
			DrawBox(posX,posY,posX+32,posY+32,GetColor(255,0,0),FALSE);
			g_player.x+=4;
		}
		break;
	case LEFT:
		if(g_Field[(int)g_player.y/32][g_player.x/32-1].flg==0){ //左の当たり判定
			int posX=g_player.x-32;
			int posY=g_player.y;
			DrawBox(posX,posY,posX+32,posY+32,GetColor(255,0,0), FALSE );
			g_player.x-=4;
		}
		break;
	case UP:
		if(g_Field[(int)g_player.y/32-1][g_player.x/32].flg==0){ //上の当たり判定
			int posX=g_player.x;
			int posY=g_player.y-32;
			DrawBox(posX,posY,posX+32,posY+32,GetColor(255,0,0), FALSE );
		}
		break;
	case DOWN:
		if(g_Field[(int)g_player.y/32+1][g_player.x/32].flg==0||g_Field[(int)g_player.y/32+1][g_player.x/32].flg==0){ //下の当たり判定
			int posX=g_player.x;
			int posY=g_player.y+32;
			DrawBox(posX,posY,posX+32,posY+32,GetColor(255,0,0), FALSE );
		}
		break;
	case JUMP:
		if(g_Field[(int)g_player.y/32-1][g_player.x/32].flg==0){
			int posX=g_player.x;
			int posY=g_player.y-32;
			DrawBox(posX,posY,posX+32,posY+32,GetColor(255,0,0), FALSE );
		}
		break;
	}
	if(g_Field[(int)g_player.y/32+1][g_player.x/32].flg==0&&g_player.JumpFlg!=1){ //下の当たり判定
		g_player.y+=4;
	}
	if(g_player.JumpFlg==1){
		if(g_Field[(int)g_player.y/32-1][g_player.x/32].flg==0){
			y_temp=g_player.y;
			g_player.y+=(g_player.y-y_prev)+1;
			y_prev=y_temp;
			if(g_Field[(int)g_player.y/32+1][g_player.x/32].flg==5)g_player.JumpFlg=0;
			if(g_player.y==384){
				g_player.JumpFlg=0;
			}
		}
	}
	DrawFormatString(500,120, 0xff00000,"%d",g_player.JumpFlg);
}
