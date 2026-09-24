#include "TYPES.H"

/* Mode entries of the column reveal effect. */

s32 BattleFx_RunRevealColumn(s32, s32);

void BattleFx_RunRevealColumnMode1(s32 arg0)
{
    BattleFx_RunRevealColumn(arg0, 1);
}

void BattleFx_RunRevealColumnMode2(s32 arg0)
{
    BattleFx_RunRevealColumn(arg0, 2);
}
