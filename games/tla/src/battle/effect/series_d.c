#include "types.h"

struct BattleEffectArgument;
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Mad Spatter (kind 347).
void BattleFx_RunMadSpatter(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 4);
}

// Abilities: Mud (kind 237).
void BattleFx_RunMud(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 7);
}

// Abilities: Decompose (kind 350).
void BattleFx_RunDecompose(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 5);
}

// Abilities: Bone Chiller (kind 309).
void BattleFx_RunBoneChiller(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 0);
}

// Abilities: Sticky Goo (kind 315).
void BattleFx_RunStickyGoo(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 1);
}

// Abilities: Poison Ink (kind 340).
void BattleFx_RunPoisonInk(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 6);
}

// Abilities: Sticky Poison (kind 322).
void BattleFx_RunStickyPoison(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 2);
}

// Abilities: Rotten Blood (kind 330).
void BattleFx_RunRottenBlood(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 3);
}

// Abilities: Rising Venom (kind 430).
void BattleFx_RunRisingVenom(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 8);
}

// Abilities: Formic Acid (kind 453).
void BattleFx_RunFormicAcid(struct BattleEffectArgument *effect) {
  Func_0814512c(effect, 9);
}
