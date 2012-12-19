#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "all_gfx.h"
#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"
#include <nds.h>
#include <PA9.h>

void initScreen();
void mainScreen();
void sprites();

int i = 0;

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

#define SPRITE_MENU_PAL  0
 #define SPRITE_POINT_PAL 1

#define SPRITE_MENU   0
 #define SPRITE_POINT  1

#define TRUE 1
 #define FALSE 0


struct penguin {
	int x, y;
};

struct obstacle {
	int x, y;
	int type;
	u8 state;
};

int delay;
int speed;
int level;
struct penguin pen;
struct obstacle obs[4];

void init();
void createObs();
static portTASK_FUNCTION(imgLotation, pvParameters);
static portTASK_FUNCTION(keyInput, pvParameters);
static portTASK_FUNCTION(imgDisplay, pvParameters);

void initScreen();
void mainScreen();

int main() {

	PA_Init();
	PA_InitVBL();

	init();

	PA_LoadSpritePal(DOWN_SCREEN, 0, (void*) PenguinSprite_Pal);
	PA_CreateSprite(DOWN_SCREEN, 1, (void*) PenguinSprite_Sprite,
			OBJ_SIZE_32X32, 1, 1, 80, 80);
	PA_SetSpriteAnim(DOWN_SCREEN, 0, 2);
	PA_SetSpriteHflip(DOWN_SCREEN, 1, 1);

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

	delay = 1000;
	speed = 0;
	level = LEVEL1;

	pen.x = 6;
	pen.y = 9;

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

	obs[3].x = 0;
	obs[3].y = 0;
	obs[3].type = 2;
	obs[3].state = FALSE;

}

static portTASK_FUNCTION(keyInput, pvParameters) {

	s32 x = 120;
	s32 y = 160;

	while (1) {

		if (Pad.Newpress.Up) {

			delay = delay - 100;
			speed = speed + 10;

		}

		if (Pad.Newpress.Down) {

			delay = delay + 100;
			speed = speed - 10;
		}

		if (Pad.Held.Left && pen.x > 5) {

			PA_StartSpriteAnim(0, 0, 4, 7, 4);
			PA_SetSpriteHflip(0, 0, 1);

			pen.x -= 2;
		}

		if (Pad.Held.Right && pen.x < 236) {

			PA_StartSpriteAnim(0, 0, 4, 7, 4);
			PA_SetSpriteHflip(0, 0, 0);

			pen.x += 2;
		}

		if (!((Pad.Held.Left) || (Pad.Held.Right)))
			PA_SpriteAnimPause(0, 0, 1);

		// Moving Code

		x += Pad.Held.Right - Pad.Held.Left;
		PA_SetSpriteXY(0, 0, x, y);

		vTaskDelay(30);
	}
}
static portTASK_FUNCTION(imgDisplay, pvParameters) {

	PA_InitText(DOWN_SCREEN, 0);
	PA_SetTextCol(DOWN_SCREEN, 100, 100, 100);

	initScreen();
	mainScreen();

	while (1) {

		PA_OutputSimpleText(1, 1, 2, "Hello World!");
//		PA_OutputText(0, 0, 7, "qwerasdzzzc : %d ", pen.x);

		PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &snowScreen1);
		vTaskDelay(delay);

		PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &snowScreen2);
		vTaskDelay(delay);
	}
}
static portTASK_FUNCTION(imgLotation, pvParameters) {

	int i;
	int cnt = 0;

	while (1) {
		cnt++;
		if (cnt == 50)
			createObs();
		for (i = 0; i < 4; i++) {
			if (obs[i].state) {
				obs[i].y += 2;
				if (obs[i].y > 190)
					obs[i].state = FALSE;
			}
		}
		vTaskDelay(30);
	}

}
void createObs() {
	int obsCreate;
	int i;
	obsCreate = 5;
	for (i = 0; i < 4; i++) {
		if (i == obsCreate)
			obs[i].type = TRUE;
	}
}

//void sprites() {
//
//	s32 x = 120;
//	s32 y = 64;
//
//	while (1) {
//		// Animation code...
//		if (Pad.Newpress.Up)
//			PA_StartSpriteAnim(0, 0, 0, 3, 4);
//		if (Pad.Newpress.Down)
//			PA_StartSpriteAnim(0, 0, 8, 11, 4);
//
//		if (Pad.Newpress.Right) {
//			PA_StartSpriteAnim(0, 0, 4, 7, 4);
//			PA_SetSpriteHflip(0, 0, 0);
//		}
//		if (Pad.Newpress.Left) {
//			PA_StartSpriteAnim(0, 0, 4, 7, 4);
//			PA_SetSpriteHflip(0, 0, 1);
//		}
//
//		if (!((Pad.Held.Left) || (Pad.Held.Up) || (Pad.Held.Down)
//				|| (Pad.Held.Right)))
//			PA_SpriteAnimPause(0, 0, 1);
//
//		// Moving Code
//		y += Pad.Held.Down - Pad.Held.Up;
//		x += Pad.Held.Right - Pad.Held.Left;
//		PA_SetSpriteXY(0, 0, x, y);
//
//		PA_WaitForVBL();
//	}
//
//}

