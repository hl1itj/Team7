#include <PA_BgStruct.h>

extern const char gameOver_Tiles[];
extern const char gameOver_Map[];
extern const char gameOver_Pal[];

const PA_BgStruct gameOver = {
	PA_BgNormal,
	256, 192,

	gameOver_Tiles,
	gameOver_Map,
	{gameOver_Pal},

	3776,
	{1536}
};
