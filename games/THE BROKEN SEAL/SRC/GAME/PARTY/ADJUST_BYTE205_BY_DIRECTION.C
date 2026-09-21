#include "TYPES.H"

extern u8 Data_02000240[];
#define PARTY_STATE Data_02000240

void Party_AdjustByte205ByDirection(s32 arg0)
{
    u8 value = PARTY_STATE[0x205];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    PARTY_STATE[0x205] = value;
}
