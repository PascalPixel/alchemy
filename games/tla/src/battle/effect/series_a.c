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

DEFINE_EFFECT_KIND(BattleFx_RunPly, BattleFx_RunHealingMotes, 0)

DEFINE_EFFECT_KIND(BattleFx_RunWish, BattleFx_RunHealingMotes, 1)

DEFINE_EFFECT_KIND(BattleFx_RunCure, BattleFx_RunHealingMotes, 2)

DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode8, BattleFx_RunHealingMotes, 8)

DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode9, BattleFx_RunHealingMotes, 9)

void BattleFx_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunHealingMotes(effect, 3);
    return;
  }
  BattleFx_RunHealingMotes(effect, 4);
}

DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode4, BattleFx_RunHealingMotes, 4)

DEFINE_EFFECT_KIND(BattleFx_RunTonic, BattleFx_RunHealingMotes, 5)

DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode7, BattleFx_RunHealingMotes, 7)

DEFINE_EFFECT_KIND(BattleFx_RunPsyCrystal, BattleFx_RunHealingMotes, 6)

DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode10, BattleFx_RunHealingMotes, 10)

DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode11, BattleFx_RunHealingMotes, 11)

DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode12, BattleFx_RunHealingMotes, 12)

void BattleFx_RunHealingMotesMode13(struct BattleEffectArgument *effect) {
  effect->prelude = 0;
  effect->variant = 2;
  BattleFx_RunHealingMotes(effect, 13);
}
