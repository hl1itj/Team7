#include <PA9.h>
#include "all_gfx.h"

void screen() {

	PA_Init();
	PA_InitVBL();

	PA_LoadBackground(1, 1, &map);


}
