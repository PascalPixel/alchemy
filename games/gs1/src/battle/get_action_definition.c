#include "battle_runtime.h"

extern const u8 Data_0807ee58[];

struct BattleAction *BattleAction_GetDirect(s32 action_id) {
    u32 entry_index;

    entry_index = action_id & 0x3fff;
    if (entry_index >= 0x208U) {
        entry_index = 0;
    }
    return (struct BattleAction *)(Data_0807ee58 + entry_index * 0x10);
}
