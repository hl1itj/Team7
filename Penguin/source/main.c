#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "all_gfx.h"
#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"
#include "bgm.h"
#include "kkk.h"
#include "jump.h"
#include "sg.h"
#include <nds.h>
#include <PA9.h>

#define UP_SCREEN 1
#define DOWN_SCREEN 0

#define BACKGROUND_UP 1
#define BACKGROUND_DOWN 0

#define START_NUM 0

#define LEVEL1 14
#define LEVEL2 12
#define LEVEL3 10
#define LEVEL4 9
#define LEVEL5 8
#define LEVEL6 7
#define LEVEL7 6

#define MAX_WIDTH 228
#define MIN_WIDTH 5
#define MAX_HIGHT 192
#define MIN_HIGHT 5

#define STRAIGHT 0
#define RIGHT_CURVE 1
#define LEFT_CURVE 2

struct penguin {
	int x, y;
	u8 jump;
};

struct obstacle {
	int x, y;
	int type;
	u8 state;
};

int road;
int roadCnt ;
int limitTime ;
int roadMap[20];
int delay;
int speed;
int level;
struct penguin pen;
struct obstacle obs[2];
int rate[2] ={2048,2048};

void init();
void createObs();
static portTASK_FUNCTION(imgLotation, pvParameters);
static portTASK_FUNCTION(keyInput, pvParameters);
static portTASK_FUNCTION(imgDisplay, pvParameters);
static portTASK_FUNCTION(bgDisplay, pvParameters);

int main() {

	PA_Init();
	PA_InitVBL();

	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);

	PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &start);
	init();

	AS_SoundDefaultPlay((u8*) sg, (u32) sg_size, 127, 64, FALSE, 0);
	while (1) {

		if (Pad.Newpress.Start) {

			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);
			PA_DeleteSprite(DOWN_SCREEN, START_NUM);
			PA_LoadBackground(UP_SCREEN, BACKGROUND_UP, &map);

			break;

		}
		PA_WaitForVBL();
	}


	AS_SoundDefaultPlay((u8*) bgm, (u32) bgm_size, 127, 64, TRUE, 0);

	PA_LoadSpritePal(DOWN_SCREEN, 1, (void*) PenguinSprite_Pal);
	PA_LoadSpritePal(DOWN_SCREEN, 2, (void*) obstacle_Pal);
	PA_CreateSprite(DOWN_SCREEN, // Screen
			0, // Sprite number
			(void*) PenguinSprite_Sprite, // Sprite name
			OBJ_SIZE_32X32, // Sprite size
			1, // 256 color mode
			1, // Sprite palette number
			pen.x, pen.y); // X and Y position on the screen
	PA_SetSpriteAnim(DOWN_SCREEN, 0, 1);

	xTaskCreate(bgDisplay, (const signed char * const)"bgDisplay", 2048,
			(void *)NULL, tskIDLE_PRIORITY +2, NULL);
	xTaskCreate(keyInput, (const signed char * const)"keyInput", 2048,
			(void *)NULL, tskIDLE_PRIORITY +1, NULL);
	xTaskCreate(imgDisplay, (const signed char * const)"imgDisplay", 2048,
			(void *)NULL, tskIDLE_PRIORITY +1, NULL);
	xTaskCreate(imgLotation, (const signed char * const)"imgLotation", 2048,
			(void *)NULL, tskIDLE_PRIORITY +1, NULL);

	vTaskStartScheduler();

	while (1) {
		PA_WaitForVBL();
	}
}

void init() {
	int i;
	for(i=0; i<20; i++)
	{
		roadMap[i] = PA_RandMinMax(0,2);
	}
	delay = 1000;
	speed = 10;
	level = LEVEL1;
	road = STRAIGHT;

	pen.x = 120;
	pen.y = 155;
	pen.jump = FALSE;

	obs[0].x = 120;
	obs[0].y = 55;
	obs[0].type = 0;
	obs[0].state = FALSE;

	obs[1].x = 120;
	obs[1].y = 55;
	obs[1].type = 1;
	obs[1].state = FALSE;


	roadCnt =0;
	limitTime = 100;

}


static portTASK_FUNCTION(keyInput, pvParameters) {
	int timeCnt = 0;
	while (1) {
		if (Pad.Newpress.Up && (speed < 90)) {
			delay = delay - 100;
			speed = speed + 10;
			PA_OutputText(UP_SCREEN, 8, 19, "SP:%d ", speed);
		}
		if (Pad.Newpress.Down && (speed > 1) ) {
			delay = delay + 100;
			speed = speed - 10;
			PA_OutputText(UP_SCREEN, 8, 19, "SP:%d ", speed);
		}
		if(Pad.Newpress.Right && !(pen.jump))
			PA_StartSpriteAnim(0, 0, 0, 0, 4);
		if(Pad.Newpress.Left && !(pen.jump)) {
			PA_StartSpriteAnim(0, 0, 2, 2, 4);
		}
		if (Pad.Held.Left && pen.x > MIN_WIDTH) {
			pen.x -= 1;
		}
		if (Pad.Held.Right && pen.x < MAX_WIDTH) {
			pen.x += 1;
		}
		if (Pad.Newpress.A && !(pen.jump)) {
			AS_SoundDefaultPlay((u8*)jump, (u32)jump_size, 127, 64, FALSE, 0);
			PA_StartSpriteAnim(0, 0, 3, 3, 10);
			pen.jump = TRUE;
			pen.y -= 20;
			timeCnt = 0;
		}
		if (!((Pad.Held.Left) || (Pad.Held.Right) || (pen.jump))) {
			PA_SpriteAnimPause(0, 0, 1);
			PA_StartSpriteAnim(0, 0, 1, 1, 4);
		}
		timeCnt++;
		if ((timeCnt > 70) && (pen.jump)) {
			PA_StartSpriteAnim(0, 0, 1, 1, 4);
			pen.jump = FALSE;
			pen.y += 20;
			timeCnt = 0;
		}
		if(limitTime <=0){
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &gameOver);
			while(1){
				if (Pad.Newpress.Start){
					init();
					break;
				}
				vTaskDelay(20);
			}
		}
		vTaskDelay(15);
	}
}
static portTASK_FUNCTION(bgDisplay, pvParameters) {
	PA_InitText(UP_SCREEN, 0);
	PA_OutputText(UP_SCREEN, 2, 19, "LV:1");
	PA_OutputText(UP_SCREEN, 8, 19, "SP:%d ", speed);
	PA_OutputText(UP_SCREEN, 16, 19, "0 km");
	PA_OutputText(UP_SCREEN, 22, 19, "Time:%d ", limitTime);

	while (1) {
		roadCnt++;
		if(roadCnt%50 == 0){
			road++;
		}
		PA_OutputText(UP_SCREEN, 16, 19, "%d km", roadCnt/10 );
		if(roadMap[road]==0){
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &snowScreen1);
			vTaskDelay(delay);
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &snowScreen2);
			vTaskDelay(delay);
		}
		if(roadMap[road]==1){
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen1);
			vTaskDelay(delay);
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen2);
			vTaskDelay(delay);
		}
		if(roadMap[road] == 2){
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen3);
			vTaskDelay(delay);
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen4);
			vTaskDelay(delay);
		}
		if(limitTime <=0){
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &gameOver);
			while(1){
				if (Pad.Newpress.Start){
					init();
					break;
				}
				vTaskDelay(20);
			}
		}
	}
}
static portTASK_FUNCTION(imgDisplay, pvParameters) {
	int expand = 18;
	int i;
	int cnt=0;
	PA_InitText(DOWN_SCREEN, 0);
	PA_SetTextCol(DOWN_SCREEN, 100, 100, 100);

	while (1) {
		PA_SetSpriteXY(DOWN_SCREEN, 0, pen.x, pen.y);
		for(i=0; i<2; i++){
			if (obs[i].state) {
				if(obs[i].type == 1){
					PA_SetSpriteXY(DOWN_SCREEN, 1, obs[i].x, obs[i].y);
					PA_SetRotset(DOWN_SCREEN, 0, 0, rate[i], rate[i]);
				}
				else{
					PA_SetSpriteXY(DOWN_SCREEN, 1, obs[i].x, obs[i].y);
					PA_SetRotset(DOWN_SCREEN, 0, 0, rate[i], rate[i]);
				}

				if(rate[i] > 256){
					rate[i] -= expand;
				}
			}
		}
		cnt++;
		if(cnt==50){
			cnt=0;
			limitTime--;
			PA_OutputText(UP_SCREEN, 22, 19, "Time:%d ", limitTime);
		}
		if(limitTime <=0){
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &gameOver);
			while(1){
				if (Pad.Newpress.Start){
					init();
					break;
				}
				vTaskDelay(20);
			}
		}

		vTaskDelay(30);
	}
}


static portTASK_FUNCTION(imgLotation, pvParameters) {
	int i;
	int cnt = 0;
	int disX;
	int disY;

	while (1) {
		cnt++;
		if (cnt > 80){
			createObs();
			cnt= 0;
		}
		if (roadMap[road] == RIGHT_CURVE && pen.x > MIN_WIDTH)
			pen.x -= 1;
		if (roadMap[road] == LEFT_CURVE && pen.x < MAX_WIDTH)
			pen.x += 1;
		for (i = 0; i < 2; i++) {
			if (obs[i].state) {
				if (obs[i].y == 55) {
					PA_CreateSprite(DOWN_SCREEN, // Screen
							1, // Sprite number
							(void*) obstacle_Sprite, // Sprite name
							OBJ_SIZE_64X32, // Sprite size
							1, // 256 color mode
							2, // Sprite palette number
							obs[i].x, obs[i].y); // X and Y position on the screen
					if(obs[i].type == 1)
						PA_StartSpriteAnim(0, 1, 0, 0, 4);
					else
						PA_StartSpriteAnim(0, 1, 1, 1, 4);
//					PA_SetSpriteAnim(DOWN_SCREEN, 0, 1);
					PA_SetSpriteRotEnable(BACKGROUND_DOWN, 1, 0);
					PA_SetRotset(DOWN_SCREEN, 0, 0, 2048, 2048);
					PA_SetSpriteXY(0, 1, obs[i].x, obs[i].y);
				}
				obs[i].y += 2;
				if (obs[i].y >= MAX_HIGHT){
					obs[i].state = FALSE;
					obs[i].y = 55;
					rate[i] = 2048;
					PA_DeleteSprite(DOWN_SCREEN,1);
				}
				disX = pen.x - (obs[i].x+30);
				if(pen.jump)
					disY = (pen.y+20) - (obs[i].y);
				else
					disY = pen.y - (obs[i].y-16);

				if(disX <0)
					disX *= -1;
				if(disY <0)
					disY *= -1;
				if ((disX < 20) && (disY < 10)) {
					if (pen.jump) {
						//장애물 통과
						if (obs[i].type == 2) {
							//장애물 걸림
							AS_SoundDefaultPlay((u8*)kkk, (u32)kkk_size, 127, 64, FALSE, 0);
							speed = 0;
							delay = 1000;
							PA_OutputText(UP_SCREEN, 8, 19, "SP:%d ", speed);
							vTaskDelay(500);
							pen.x +=10;
							vTaskDelay(500);
							pen.x +=10;
							vTaskDelay(500);
							pen.x +=10;
						}
					}
					else {
						//장애물 걸림
					AS_SoundDefaultPlay((u8*)kkk, (u32)kkk_size, 127, 64, FALSE, 0);
						speed = 0;
						delay = 1000;
						PA_OutputText(UP_SCREEN, 8, 19, "SP:%d ", speed);
						vTaskDelay(500);
						pen.x +=10;
						vTaskDelay(500);
						pen.x +=10;
						vTaskDelay(500);
						pen.x +=10;
					}
				}
			}
		}

		vTaskDelay(50);

		if(limitTime <=0){
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &gameOver);
			while(1){
				if (Pad.Newpress.Start){
					init();
					break;
				}
				vTaskDelay(20);
			}
		}

	}

}
void createObs() {

	u32 obsCreate;
	u32 i;

	obsCreate = PA_RandMinMax(0,2);
	for (i = 0; i < 2; i++) {

		if (i == obsCreate && !(obs[i].state)) {
			obs[i].state = TRUE;
			obs[i].x = PA_RandMinMax(35, 100);
		}

	}
}

