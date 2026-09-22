#include "TYPES.H"
#include "BATTLE_EFX.H"

extern u8 Data_03001e50[];

void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output)
{
    if (alternate == 0) {
        u8 *state;
        u32 value;

        BattleEffect_LoadWork(alternate = 46, 7, 7, 3, 2);
        state = Data_03001e50;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        BattleEffect_LoadWork(alternate, 7, 7, 3, 3);
        output[1] = *(u32 *)(state += 188);
    } else {
        u8 *state;
        u32 value;

        BattleEffect_LoadWork(alternate = 46, 7, 7, 7, 2);
        state = Data_03001e50;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        BattleEffect_LoadWork(alternate, 7, 7, 7, 3);
        output[1] = *(u32 *)(state += 188);
    }
}
