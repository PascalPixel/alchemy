#include "TYPES.H"
#include "SCENE.H"

struct Entry_080bd3c8 {
    u8 unknown_00[9];
    u8 status;
};

void *BattleAction_Get();

u32 Ability_CheckStatusOrSpecialId(s32 value)
{
    u8 status;

    if (value == 0x7E) {
        return 1U;
    }
    status = ((struct Entry_080bd3c8 *)BattleAction_Get())->status;
    return (u32)((0 - status) | status) >> 0x1F;
}
