#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"

void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode);

/* Mode entries of battle effect series A, as ☀️'s SERIES_A.C has them;
   ⚓️ adds modes 7 to 13. */
void BattleFx_RunSeriesAMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 0);
}

void BattleFx_RunSeriesAMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 1);
}

void BattleFx_RunSeriesAMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 2);
}

void BattleFx_RunSeriesAMode8(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 8);
}

void BattleFx_RunSeriesAMode9(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 9);
}

void BattleFx_RunSeriesAMode3Or4(struct BattleEffectArgument *effect)
{
    if (effect->variant == 0) {
        BattleFx_RunSeriesA(effect, 3);
        return;
    }
    BattleFx_RunSeriesA(effect, 4);
}

void BattleFx_RunSeriesAMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 4);
}

void BattleFx_RunSeriesAMode5(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 5);
}

void BattleFx_RunSeriesAMode7(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 7);
}

void BattleFx_RunSeriesAMode6(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 6);
}

void BattleFx_RunSeriesAMode10(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 10);
}

void BattleFx_RunSeriesAMode11(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 11);
}

void BattleFx_RunSeriesAMode12(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 12);
}

/* Mode 13 runs as the second variant with the fourth field cleared. */
void BattleFx_RunSeriesAMode13(struct BattleEffectArgument *effect)
{
    effect->unknown_001c = 0;
    effect->variant = 2;
    BattleFx_RunSeriesA(effect, 13);
}
