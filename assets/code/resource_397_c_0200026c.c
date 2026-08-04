#include "resource_397.h"

typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u16 Data_04000006;
extern s32 Data_02008610;
extern u16 Data_02008614;
extern u16 Data_02008616;
extern u16 Data_0400001c;

void Func_0200026c(void)
{
    u16 *source;
    u32 value;

    if (Data_04000006 >= Data_02008610) {
        source = &Data_02008614;
    } else {
        source = &Data_02008616;
    }
    value = *source;
    Data_0400001c = value;
}
