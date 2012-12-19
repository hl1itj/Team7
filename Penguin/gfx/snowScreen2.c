#include <PA_BgStruct.h>

extern const char snowScreen2_Tiles[];
extern const char snowScreen2_Map[];
extern const char snowScreen2_Pal[];

const PA_BgStruct snowScreen2 = {
	PA_BgNormal,
	256, 192,

	snowScreen2_Tiles,
	snowScreen2_Map,
	{snowScreen2_Pal},

	6272,
	{1536}
};
