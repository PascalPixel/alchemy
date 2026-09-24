#include "TYPES.H"

/* Mode entries of the member beam effect. */

s32 BattleFx_RunMemberBeam(s32, s32);

void BattleFx_RunTwoModeAMode0(s32 arg0)
{
    BattleFx_RunMemberBeam(arg0, 0);
}

void BattleFx_RunTwoModeAMode1(s32 arg0)
{
    BattleFx_RunMemberBeam(arg0, 1);
}
