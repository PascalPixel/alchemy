#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 gData_020019c0[];
extern const u8 gData_020019f0[];

void GameFlag_SetBit(s32 flag);
void Func_0200124c(s32, s32, s32);

void Func_020006f4(s32 unused, s32 id)
{
    Func_0200124c(id, 0, 0);
    GameFlag_SetBit(0x8e6);
}

const u8 *Func_0200070c(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov66bRoom2)
        return gData_020019c0;
    return gData_020019f0;
}
