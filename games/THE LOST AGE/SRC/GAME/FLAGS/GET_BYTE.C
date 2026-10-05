#include "GLOBAL.H"
#include "TYPES.H"


u8 GameFlag_GetByte(s32 flag)
{
    flag = ((u32)flag << 20) >> 23;
    return GameFlagBytes[flag];
}
