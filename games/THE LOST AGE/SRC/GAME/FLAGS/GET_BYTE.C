#include "TYPES.H"

extern u8 GameFlagBytes[];

u8 GameFlag_GetByte(s32 flag)
{
    flag = ((u32)flag << 20) >> 23;
    return GameFlagBytes[flag];
}
