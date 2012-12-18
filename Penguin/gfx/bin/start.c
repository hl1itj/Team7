#include <PA_BgStruct.h>

extern const char start_Tiles[];
extern const char start_Map[];
extern const char start_Pal[];

const PA_BgStruct start = {
	PA_BgNormal,
	256, 192,

	start_Tiles,
	start_Map,
	{start_Pal},

	11648,
	{1536}
};
