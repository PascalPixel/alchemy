#include "TYPES.H"

s32 Scheduler_EnableCallbacks(u32 callback);
void Object_EffectSpawnCallback(void);

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id)
{
    struct BattleEffectEntry *entry = BattleFx_DefinitionTable;
    u32 index = 0;

    if (entry->id != id) {
        do {
            index += 1;
            entry++;
            if (index > 0x81)
                break;
        } while (entry->id != id);
    }
    return entry;
}
