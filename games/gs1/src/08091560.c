#include "battle_effect_runtime.h"

extern struct BattleEffectEntry Data_0809e9f0[];

struct BattleEffectEntry *Func_08091560(u32 id)
{
    struct BattleEffectEntry *entry = Data_0809e9f0;
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
