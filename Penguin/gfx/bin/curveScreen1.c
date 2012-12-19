#include <PA_BgStruct.h>

extern const char curveScreen1_Tiles[];
extern const char curveScreen1_Map[];
extern const char curveScreen1_Pal[];

const PA_BgStruct curveScreen1 = {
	PA_BgNormal,
	256, 192,

	curveScreen1_Tiles,
	curveScreen1_Map,
	{curveScreen1_Pal},

	10752,
	{1536}
};
