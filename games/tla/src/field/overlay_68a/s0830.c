#include "scene.h"
#include "party_state.h"

void GameFlag_ClearBit();
void Func_020018f4();
void Func_020018ec();

s32 Func_020007a4(void)
{
    s32 entrance = gPartyState.entrance;

    if (entrance == 1)
        GameFlag_SetBit(0x960);
    else if (entrance == 2)
        GameFlag_ClearBit(0x960);
    else if (entrance == 3)
        GameFlag_ClearBit(0x960);
    else if (entrance == 4)
        GameFlag_SetBit(0x960);
    else if (entrance == 5)
        GameFlag_SetBit(0x960);
    if (GameFlag_Test(0x960)) {
        Func_020018f4(38, 47, 26, 12, 8, 47);
        Func_020018f4(102, 47, 26, 12, 72, 47);
        Func_020018ec(38, 47, 26, 12, 8, 47);
    }
    return 0;
}

void Func_02000830(void)
{
    Func_020019f4();
}
