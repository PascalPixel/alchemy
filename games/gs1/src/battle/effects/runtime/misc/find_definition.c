#include "scene.h"
#include "abi/battle/effects/runtime/misc/find_definition.h"
#include "battle_effect_runtime.h"

extern struct BattleEffectEntry gRom[];

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id)
{
    struct BattleEffectEntry *entry = gRom;
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
