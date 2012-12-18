#include <PA_BgStruct.h>

extern const char map_Tiles[];
extern const char map_Map[];
extern const char map_Pal[];

const PA_BgStruct map = {
	PA_BgNormal,
	256, 192,

	map_Tiles,
	map_Map,
	{map_Pal},

	10560,
	{1536}
};
