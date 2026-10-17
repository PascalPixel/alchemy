#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"

void Func_0814fab8(struct BattleEffectArgument *effect, u32 mode);

// Abilities: Impact, High Impact, Demon Spear, Angel Spear (kind 26); Forge
// (kind 210); Kindle (kind 223); kind 46 is used by no ability.
void BattleFx_RunImpact(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 0);
}

// Abilities: Guard, Protect, Guardian, Protector (kind 28); Corona (kind 211).
void BattleFx_RunGuard(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 1);
}

// Abilities: Iron (kind 220).
void BattleFx_RunIron(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 6);
}

void Func_0814fa2c(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 7);
}

// Abilities: Ward, Resist, Magic Shell, Magic Shield (kind 30); Breeze (kind
// 215).
void BattleFx_RunWard(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 2);
}

// Abilities: Steam (kind 221).
void BattleFx_RunSteam(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 11);
}

// Abilities: Granite (kind 200).
void BattleFx_RunGranite(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 3);
}

// Abilities: Flash (kind 213).
void BattleFx_RunFlash(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 5);
}

// Abilities: Shade (kind 222).
void BattleFx_RunShade(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 8);
}

// Abilities: Guard Aura (kind 460).
void BattleFx_RunGuardAura(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 12);
}

// Abilities: Zephyr (kind 216).
void BattleFx_RunZephyr(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 4);
}

// Abilities: Coal (kind 224).
void BattleFx_RunCoal(struct BattleEffectArgument *effect) {
  Func_0814fab8(effect, 9);
}

void Func_0814fa98(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];

  /* FAKEMATCH: ⚓️ loads the effect work before it builds the fade step's
   * offset; the plain order starts the offset first (one pair of instructions
   * swapped). */
  asm volatile("");
  work->fadeStep = 0;
  effect->prelude = 0;
  Func_0814fab8(effect, 10);
}
