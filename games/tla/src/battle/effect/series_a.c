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

void BattleFx_RunPly(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 0);
}

void BattleFx_RunWish(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 1);
}

void BattleFx_RunCure(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 2);
}

void BattleFx_RunHealingMotesMode8(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 8);
}

void BattleFx_RunHealingMotesMode9(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 9);
}

void BattleFx_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunHealingMotes(effect, 3);
    return;
  }
  BattleFx_RunHealingMotes(effect, 4);
}

void BattleFx_RunHealingMotesMode4(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 4);
}

void BattleFx_RunTonic(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 5);
}

void BattleFx_RunHealingMotesMode7(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 7);
}

void BattleFx_RunPsyCrystal(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 6);
}

void BattleFx_RunHealingMotesMode10(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 10);
}

void BattleFx_RunHealingMotesMode11(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 11);
}

void BattleFx_RunHealingMotesMode12(struct BattleEffectArgument *effect) {
  BattleFx_RunHealingMotes(effect, 12);
}

void BattleFx_RunHealingMotesMode13(struct BattleEffectArgument *effect) {
  effect->prelude = 0;
  effect->variant = 2;
  BattleFx_RunHealingMotes(effect, 13);
}
