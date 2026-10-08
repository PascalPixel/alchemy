#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect_work.h"


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

void BattleFx_RunSeriesAMode13(struct BattleEffectArgument *effect)
{
    effect->prelude = 0;
    effect->variant = 2;
    BattleFx_RunSeriesA(effect, 13);
}
