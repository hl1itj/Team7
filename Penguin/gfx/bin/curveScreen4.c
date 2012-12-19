#include <PA_BgStruct.h>

extern const char curveScreen4_Tiles[];
extern const char curveScreen4_Map[];
extern const char curveScreen4_Pal[];

const PA_BgStruct curveScreen4 = {
	PA_BgNormal,
	256, 200,

	curveScreen4_Tiles,
	curveScreen4_Map,
	{curveScreen4_Pal},

	6336,
	{1600}
};
