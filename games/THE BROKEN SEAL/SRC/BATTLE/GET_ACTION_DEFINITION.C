#include "SCENE.H"
#include "BATTLE_RUNTIME.H"

extern const u8 BattleAction_DefinitionTable[];

struct BattleAction *BattleAction_GetDirect(s32 action_id) {
    u32 entry_index;

    entry_index = action_id & 0x3fff;
    if (entry_index >= 0x208U) {
        entry_index = 0;
    }
    return (struct BattleAction *)(BattleAction_DefinitionTable + entry_index * 0x10);
}
