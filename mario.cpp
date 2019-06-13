#include "DxLib.h"
#define HEIGHT 20
#define WIDTH 15
#define SIZE 32

const float G=9.8;

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
	int y;
	int dire;
	int img[15];
};

struct Player	g_player;
struct Field	g_Field[HEIGHT][WIDTH];
int FieldDate[HEIGHT][WIDTH]=
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//0.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//1.0
	{0,0,0,0,0,0,0,0,18,30,0,0,0,5,6},//2.0
	{0,0,0,0,0,0,0,0,19,31,0,0,0,5,6},//3.0
	{0,0,0,0,0,0,0,0,19,31,0,0,0,5,6},//4.0
	{0,0,0,0,0,0,0,0,20,32,0,0,0,5,6},//5.0
	{0,0,0,0,0,0,0,0,0,0,0,0,14,5,6},//6.0
	{0,0,0,0,0,0,0,0,0,0,0,0,15,5,6},//7.0
	{0,0,0,0,0,0,0,0,0,0,0,0,16,5,6},//8.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//9.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//10.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//11.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//12.0
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//13.0
	{0,0,0,0,0,0,0,0,0,0,0,0,14,5,6},//14.0
	{0,0,0,0,0,0,0,0,0,0,0,0,15,5,6},//15
	{0,0,0,0,0,0,0,0,0,0,0,0,16,5,6},//16
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//17
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//18
	{0,0,0,0,0,0,0,0,0,0,0,0,0,5,6},//19
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

//初期処理
void GameInit(void)
{
	FieldInit();
	PlayerInit();
	g_GameState=GAME_MAIN;
}

//プレイヤー初期処理
void PlayerInit(void)
{
	g_player.x=64;
	g_player.y=384;
}


//フィールド初期処理
void FieldInit(void)
{
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			g_Field[i][j].x=j;
			g_Field[i][j].y=i;
			g_Field[i][j].flg=FieldDate[i][j];
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
			if(g_Field[i][j].flg==0){
				DrawGraph(i*SIZE,j*SIZE,g_FiledImage[84],TRUE);
			}else if(g_Field[i][j].flg==1){
				DrawGraph(i*SIZE,j*SIZE,g_FiledImage[5],TRUE);
			}else{
				DrawGraph(i*SIZE,j*SIZE,g_FiledImage[FieldDate[i][j]],TRUE);
			}
		}
	}
}

//プレイヤー描画
void DrawPlayer(void)
{
	DrawGraph(g_player.x,g_player.y,g_player.img[0],TRUE);
}

//プレイヤー移動
void PlayerControl(void)
{
	static int x,y,cnt=0;
	int k[4];
	if(g_KeyFlg & PAD_INPUT_UP)	g_player.dire=UP;
	if(g_KeyFlg & PAD_INPUT_RIGHT)	g_player.dire=RIGHT;
	if(g_KeyFlg & PAD_INPUT_DOWN)	g_player.dire=DOWN;
	if(g_KeyFlg & PAD_INPUT_LEFT)	g_player.dire=LEFT;
	if(g_NowKey){
		if(g_player.dire==LEFT)	g_player.x-=2;
		if(g_player.dire==RIGHT)	g_player.x+=2;
	}
	for(int i=0;i<4;i++){
		k[i]=CollisionDetection(i);
	}
	for(int i=0;i<4;i++){
		if(k[i]==1){

		}

	}
}

//当たり判定
int CollisionDetection(int i)
{
	static int x,y;
	switch (i)
	{
	case 0:
		x=g_player.x/SIZE;
		y=g_player.y/SIZE;
		if(g_Field[y][x].flg==0){
			return 0;
		}else{
			return 1;
		}
		break;
	case 1:
		x=g_player.x+SIZE/SIZE;
		y=g_player.y/SIZE;
		if(g_Field[y][x].flg==0){
			return 0;
		}else{
			return 1;
		}
		break;
	case 2:
		x=g_player.x/SIZE;
		y=g_player.y+SIZE/SIZE;
		if(g_Field[y][x].flg==0){
			return 0;
		}else{
			return 1;
		}
		break;
	case 3:
		x=g_player.x+SIZE/SIZE;
		y=g_player.y+SIZE/SIZE;
		if(g_Field[y][x].flg==0){
			return 0;
		}else{
			return 1;
		}
		break;
	}
}