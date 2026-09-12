#include "battle_effect_runtime.h"

struct BattleEffectEntry *Func_08091560(u32 id);
s32 Func_08091584(void);

u8 BattleFx_GetFlags(void)
{
    return Func_08091560(Func_08091584())->flags;
}
