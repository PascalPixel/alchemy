#include "types.h"
#include "scene.h"

/* battle/effects/series_a/mode_0.c */
s32 BattleEffectA(void *, s32);

void BattleFx_RunSeriesAMode0(s32 effect)
{
    BattleEffectA(effect, 0);
}
