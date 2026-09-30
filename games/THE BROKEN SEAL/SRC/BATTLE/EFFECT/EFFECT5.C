#include "TYPES.H"
#include "BATTLE_EFX.H"

extern u8 gWorkSlot[];

s32 BattleFx_RunFortyEightFrameEffect(s32, s32);

void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output)
{
    if (alternate == 0) {
        u8 *state;
        u32 value;

        BattleEffect_LoadWork(alternate = 46, 7, 7, 3, 2);
        state = gWorkSlot;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        BattleEffect_LoadWork(alternate, 7, 7, 3, 3);
        output[1] = *(u32 *)(state += 188);
    } else {
        u8 *state;
        u32 value;

        BattleEffect_LoadWork(alternate = 46, 7, 7, 7, 2);
        state = gWorkSlot;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        BattleEffect_LoadWork(alternate, 7, 7, 7, 3);
        output[1] = *(u32 *)(state += 188);
    }
}

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
