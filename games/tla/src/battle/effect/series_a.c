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

DEFINE_EFFECT_KIND(BattleEffect_RunPly, BattleEffect_RunHealingMotes, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunWish, BattleEffect_RunHealingMotes, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunCure, BattleEffect_RunHealingMotes, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode8, BattleEffect_RunHealingMotes, 8)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode9, BattleEffect_RunHealingMotes, 9)

void BattleEffect_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleEffect_RunHealingMotes(effect, 3);
    return;
  }
  BattleEffect_RunHealingMotes(effect, 4);
}

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode4, BattleEffect_RunHealingMotes, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunTonic, BattleEffect_RunHealingMotes, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode7, BattleEffect_RunHealingMotes, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunPsyCrystal, BattleEffect_RunHealingMotes, 6)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode10, BattleEffect_RunHealingMotes, 10)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode11, BattleEffect_RunHealingMotes, 11)

DEFINE_EFFECT_KIND(BattleEffect_RunHealingMotesMode12, BattleEffect_RunHealingMotes, 12)

void BattleEffect_RunHealingMotesMode13(struct BattleEffectArgument *effect) {
  effect->prelude = 0;
  effect->variant = 2;
  BattleEffect_RunHealingMotes(effect, 13);
}
