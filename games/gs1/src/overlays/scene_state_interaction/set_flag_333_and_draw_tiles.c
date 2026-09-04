#include "types.h"

#define FieldScene_SetFlag333AndDrawTiles Func_020002c0

extern void Func_020041b8(s32);
extern void Func_02004164(s32, s32, s32, s32, s32, s32);

void FieldScene_SetFlag333AndDrawTiles(void)
{
    Func_020041b8(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        Func_02004164(32, 37, 1, 4, width, height);
    }
}
