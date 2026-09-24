#include "TYPES.H"

s32 BattleFx_RunFortyEightFrameEffect(s32, s32);

void BattleFx_RunFortyEightFrameMode1(s32 arg0)
{
    BattleFx_RunFortyEightFrameEffect(arg0, 1);
}

void BattleFx_RunFortyEightFrameMode0(s32 arg0)
{
    BattleFx_RunFortyEightFrameEffect(arg0, 0);
}

void BattleFx_RunFortyEightFrameMode2(s32 arg0)
{
    BattleFx_RunFortyEightFrameEffect(arg0, 2);
}
