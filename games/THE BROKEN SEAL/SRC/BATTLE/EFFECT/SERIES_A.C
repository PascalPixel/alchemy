#include "TYPES.H"
#include "SCENE.H"

/* Mode entries of battle effect series A. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 BattleEffectA(void *effect, s32 mode);

void BattleFx_RunSeriesAMode0(void *effect)
{
    BattleEffectA(effect, 0);
}

void BattleFx_RunSeriesAMode1(void *effect)
{
    BattleEffectA(effect, 1);
}

void BattleFx_RunSeriesAMode2(void *effect)
{
    BattleEffectA(effect, 2);
}

void BattleFx_RunSeriesAMode3Or4(void *effect)
{
    if (FIELD_AT_OFFSET(effect, s32 *, 0x18) == 0) {
        BattleEffectA(effect, 3);
        return;
    }
    BattleEffectA(effect, 4);
}

void BattleFx_RunSeriesAMode4(void *effect)
{
    BattleEffectA(effect, 4);
}

void BattleFx_RunSeriesAMode5(void *effect)
{
    BattleEffectA(effect, 5);
}

void BattleFx_RunSeriesAMode6(void *effect)
{
    BattleEffectA(effect, 6);
}
