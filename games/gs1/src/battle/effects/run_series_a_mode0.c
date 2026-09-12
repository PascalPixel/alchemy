#include "types.h"
#include "scene.h"

/* battle/effects/series_a/mode_0.c */
s32 BattleEffectA(void *, s32);

void BattleFx_RunSeriesAMode0(s32 effect)
{
    BattleEffectA(effect, 0);
}

/* battle/effects/series_a/mode_1.c */

void BattleFx_RunSeriesAMode1(s32 effect)
{
    BattleEffectA(effect, 1);
}

/* battle/effects/series_a/mode_2.c */

void BattleFx_RunSeriesAMode2(s32 effect)
{
    BattleEffectA(effect, 2);
}

/* battle/effects/series_a/mode_3_or_4.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))


void BattleFx_RunSeriesAMode3Or4(void *effect)
{
    if (FIELD_AT_OFFSET(effect, s32 *, 0x18) == 0) {
        BattleEffectA(effect, 3);
        return;
    }
    BattleEffectA(effect, 4);
}

/* battle/effects/series_a/mode_4.c */

void BattleFx_RunSeriesAMode4(s32 effect)
{
    BattleEffectA(effect, 4);
}

/* battle/effects/series_a/mode_5.c */

void BattleFx_RunSeriesAMode5(s32 effect)
{
    BattleEffectA(effect, 5);
}

/* battle/effects/series_a/mode_6.c */

void BattleFx_RunSeriesAMode6(s32 effect)
{
    BattleEffectA(effect, 6);
}
