#include <PA_BgStruct.h>

extern const char curveScreen2_Tiles[];
extern const char curveScreen2_Map[];
extern const char curveScreen2_Pal[];

const PA_BgStruct curveScreen2 = {
	PA_BgNormal,
	256, 192,

	curveScreen2_Tiles,
	curveScreen2_Map,
	{curveScreen2_Pal},

	10048,
	{1536}
};
