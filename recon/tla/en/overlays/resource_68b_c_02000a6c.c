#include "types.h"
#include "frame.h"

#define OBJ_PALETTE ((volatile u16 *)0x05000000)

void Func_02000a6c(void)
{
    s32 i;
    s16 first;

    if ((gFrameCount & 3) == 0) {
        first = OBJ_PALETTE[0x7e];
        for (i = 14; i != 1; i--)
            OBJ_PALETTE[0x70 + i] = OBJ_PALETTE[0x70 + i - 1];
        OBJ_PALETTE[0x71] = first;
        first = OBJ_PALETTE[0x8e];
        for (i = 14; i != 1; i--)
            OBJ_PALETTE[0x80 + i] = OBJ_PALETTE[0x80 + i - 1];
        OBJ_PALETTE[0x81] = first;
    }
}
