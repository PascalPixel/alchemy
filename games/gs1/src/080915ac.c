#include "battle_effect_runtime.h"

struct BattleEffectEntry *Func_08091560(u32 id);

s32 Func_080915ac(u32 id)
{
    u8 value;

    if (Data_02000240.enabled_20a == 0 ||
        (value = Func_08091560(id)->value) == 0xFF) {
        return 0;
    }
    return value + 0x100;
}
