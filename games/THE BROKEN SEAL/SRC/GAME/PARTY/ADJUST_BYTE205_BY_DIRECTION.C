#include "TYPES.H"

extern u8 gGameState[];
#define PARTY_STATE gGameState

void Party_AdjustByte205ByDirection(s32 arg0)
{
    u8 value = PARTY_STATE[0x205];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    PARTY_STATE[0x205] = value;
}
