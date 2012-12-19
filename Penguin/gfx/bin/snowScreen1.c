#include <PA_BgStruct.h>

extern const char snowScreen1_Tiles[];
extern const char snowScreen1_Map[];
extern const char snowScreen1_Pal[];

const PA_BgStruct snowScreen1 = {
	PA_BgNormal,
	256, 192,

	snowScreen1_Tiles,
	snowScreen1_Map,
	{snowScreen1_Pal},

	6464,
	{1536}
};
