#include <PA_BgStruct.h>

extern const char curveScreen3_Tiles[];
extern const char curveScreen3_Map[];
extern const char curveScreen3_Pal[];

const PA_BgStruct curveScreen3 = {
	PA_BgNormal,
	256, 192,

	curveScreen3_Tiles,
	curveScreen3_Map,
	{curveScreen3_Pal},

	7488,
	{1536}
};
