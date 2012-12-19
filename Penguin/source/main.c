#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "all_gfx.h"
#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"
#include "bgm.h"
#include <nds.h>
#include <PA9.h>

#define UP_SCREEN 1
#define DOWN_SCREEN 0

#define BACKGROUND_UP 1
#define BACKGROUND_DOWN 0

#define START_NUM 0

#define LEVEL1 20
#define LEVEL2 18
#define LEVEL3 16
#define LEVEL4 14
#define LEVEL5 12
#define LEVEL6 10
#define LEVEL7 8

#define MAX_WIDTH 236
#define MIN_WIDTH 5
#define MAX_HIGHT 186
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

u32 level;

int road = 0;
int delay;
int speed;

struct penguin pen;
struct obstacle obs[3];

void init();
void createObs();
void UnLoad_Screen();

static portTASK_FUNCTION(imgLotation, pvParameters);
static portTASK_FUNCTION(keyInput, pvParameters);
static portTASK_FUNCTION(imgDisplay, pvParameters);

int main() {

	PA_Init();
	PA_InitVBL();

	PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &start);
	init();

	while (1) {

		if (Pad.Newpress.Start) {

			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);
			PA_DeleteSprite(DOWN_SCREEN, START_NUM);
			PA_LoadBackground(UP_SCREEN, BACKGROUND_UP, &map);

			break;

		}

		PA_WaitForVBL();

	}

	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);

	AS_SoundDefaultPlay((u8*) bgm, (u32) bgm_size, 127, 64, TRUE, 0);

	PA_LoadSpritePal(DOWN_SCREEN, 1, (void*) PenguinSprite_Pal);
	PA_CreateSprite(DOWN_SCREEN, // Screen
			0, // Sprite number
			(void*) PenguinSprite_Sprite, // Sprite name
			OBJ_SIZE_32X32, // Sprite size
			1, // 256 color mode
			1, // Sprite palette number
			pen.x, pen.y); // X and Y position on the screen

	PA_LoadSpritePal(DOWN_SCREEN, 2, (void*) obstacle_Pal);
	PA_CreateSprite(DOWN_SCREEN, // Screen
			1, // Sprite number
			(void*) obstacle_Sprite, // Sprite name
			OBJ_SIZE_64X32, // Sprite size
			1, // 256 color mode
			2, // Sprite palette number
			90, 55); // X and Y position on the screen

	PA_SetSpriteRotEnable(BACKGROUND_DOWN, 1, 0);
	PA_SetRotset(DOWN_SCREEN, 0, 0, 2048, 2048);

	PA_SetSpriteAnim(DOWN_SCREEN, 0, 1);

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

	UnLoad_Screen();

	return 0;
}

void init() {

	delay = 1000;
	speed = 0;
	level = LEVEL1;
	road = STRAIGHT;

	pen.x = 120;
	pen.y = 155;
	pen.jump = FALSE;

	obs[0].x = 0;
	obs[0].y = 0;
	obs[0].type = 0;
	obs[0].state = FALSE;

	obs[1].x = 0;
	obs[1].y = 0;
	obs[1].type = 0;
	obs[1].state = FALSE;

	obs[2].x = 0;
	obs[2].y = 0;
	obs[2].type = 1;
	obs[2].state = FALSE;

}

static portTASK_FUNCTION(keyInput, pvParameters) {
	int timeCnt = 0;
	while (1) {
		if (Pad.Newpress.Up) {
			delay = delay - 100;
			speed = speed + 10;
		}
		if (Pad.Newpress.Down) {
			delay = delay + 100;
			speed = speed - 10;
		}
		if (Pad.Held.Left && pen.x > MIN_WIDTH) {
			pen.x -= 2;
		}
		if (Pad.Held.Right && pen.x < MAX_WIDTH) {
			pen.x += 2;
		}
		if (Pad.Newpress.A) {
			pen.jump = TRUE;
		}
		if (!((Pad.Held.Left) || (Pad.Held.Right))) {
			PA_SpriteAnimPause(0, 0, 1);
		}
		pen.x += Pad.Held.Right - Pad.Held.Left;
		PA_SetSpriteXY(0, 0, pen.x, pen.y);
		timeCnt++;
		if (timeCnt > 50)
			pen.jump = FALSE;
		vTaskDelay(30);
	}
}
static portTASK_FUNCTION(imgDisplay, pvParameters) {

	PA_InitText(UP_SCREEN, 0);
	PA_OutputText(UP_SCREEN, 2, 19, "STAGE:1");
	PA_OutputText(UP_SCREEN, 10, 19, "SPEED:%d ", speed);
	PA_OutputText(UP_SCREEN, 20, 19, "ROAD:1");

	while (1) {

		int i = 0;

		if (i == 0) {

			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);

			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &snowScreen1);
			vTaskDelay(delay);

			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &snowScreen2);
			vTaskDelay(delay);

			i = 1;
		}

		if (i == 1) {

			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);

			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen1);
			vTaskDelay(delay);

			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen2);
			vTaskDelay(delay);

			i = 2;
		}

		if (i == 2) {

			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);

			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen3);
			vTaskDelay(delay);

			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &curveScreen4);
			vTaskDelay(delay);

			i = 0;
		}

	}

	PA_WaitForVBL();
}

/*
 static portTASK_FUNCTION(imgDisplay, pvParameters){
 PA_InitText(1, 0);
 PA_SetTextCol(1, 31,31,31);
 initScreen();
 //	readyScreen();
 while(1){
 //		screen();
 PA_OutputSimpleText(1, 1, 2, "Hello World!");
 //		PA_OutputText(0, 0, 7, "qwerasdzzzc : %d ", pen.x);
 vTaskDelay(30);
 }
 }
 */
static portTASK_FUNCTION(imgLotation, pvParameters) {

	int i;
	int cnt = 0;

	while (1) {

		cnt++;

		if (cnt == 50)
			createObs();

		if (road == RIGHT_CURVE && pen.x > MIN_WIDTH)
			pen.x -= 1;

		if (road == LEFT_CURVE && pen.x < MAX_WIDTH)
			pen.x += 1;

		for (i = 0; i < 4; i++) {

			if (obs[i].state) {

				obs[i].y += 2;

				if (obs[i].y > MAX_HIGHT)
					obs[i].state = FALSE;

				if (PA_Distance(pen.x, obs[i].x, pen.y, obs[i].y) < 20) {

					if (pen.jump) {
						//장애물 통과
						if (obs[i].type == 2) {
							//장애물 걸림
						}
					} else {
						//장애물 걸림
					}
				}
			}
		}

		vTaskDelay(30);
	}

}
void createObs() {

	u32 obsCreate;
	u32 i;

	obsCreate = 1;

	for (i = 0; i < 3; i++) {

		if (i == obsCreate) {
			obs[i].type = TRUE;
			obs[i].x = PA_RandMinMax(35, 220);

		}

	}
}

void UnLoad_Screen() {
	int i;

	for (i = 0; i < 2; ++i) {

		PA_ResetBgSysScreen(i);
		PA_ResetSpriteSysScreen(i);
	}
}

