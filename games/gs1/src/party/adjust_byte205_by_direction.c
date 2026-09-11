#include "types.h"
#include "scene.h"

extern u8 gCell[];

void Party_AdjustByte205ByDirection(s32 arg0)
{
    u8 value = gCell[0x205];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    gCell[0x205] = value;
}
