#include "battle_runtime.h"

extern const u8 Data_0807ee58[];

struct BattleAction *BattleAction_GetDirect(s32 action_id) {
    u32 index;

    index = action_id & 0x3fff;
    if (index >= 0x208U) {
        index = 0;
    }
    return (struct BattleAction *)(Data_0807ee58 + index * 0x10);
}
