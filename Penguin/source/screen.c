#include <PA9.h>
#include "all_gfx.h"

void screen() {

	PA_Init();
	PA_InitVBL();

	PA_LoadBackground(1, 1, &map);

void initScreen() {

	PA_Init();
	PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &start);
	PA_WaitForVBL();

}

<<<<<<< HEAD
void readyScreen() {

	while (1) {

		if (Pad.Newpress.Start) {

			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);
			PA_DeleteSprite(DOWN_SCREEN, START_NUM);
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &map);
			break;

		}

		PA_WaitForVBL();

	}
=======
>>>>>>> 28c48b7f82845319441174504265659accd63390
}
