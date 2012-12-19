#include <PA9.h>
#include "all_gfx.h"
#include "freeRTOS.h"
#include "task.h"
#include "queue.h"
#include <nds.h>
#include "sevencore_io.h"

#define UP_SCREEN 1
#define DOWN_SCREEN 0

#define BACKGROUND_UP 1
#define BACKGROUND_DOWN 0

#define START_NUM 0

void initScreen() {

	PA_Init();
	PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &start);
	PA_WaitForVBL();

}

void mainScreen() {

	while (1) {

		if (Pad.Newpress.Start) {

			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);
			PA_DeleteSprite(DOWN_SCREEN, START_NUM);
			PA_LoadBackground(UP_SCREEN, BACKGROUND_UP, &map);

			break;

		}

		PA_WaitForVBL();

	}
}


