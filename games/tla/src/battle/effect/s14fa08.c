#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"

void Func_0814fab8(struct BattleEffectArgument *effect, u32 mode);

// Abilities: Impact, High Impact, Demon Spear, Angel Spear (kind 26); Forge
// (kind 210); Kindle (kind 223); kind 46 is used by no ability.
DEFINE_EFFECT_KIND(BattleFx_RunImpact, Func_0814fab8, 0)

// Abilities: Guard, Protect, Guardian, Protector (kind 28); Corona (kind 211).
DEFINE_EFFECT_KIND(BattleFx_RunGuard, Func_0814fab8, 1)

// Abilities: Iron (kind 220).
DEFINE_EFFECT_KIND(BattleFx_RunIron, Func_0814fab8, 6)

DEFINE_EFFECT_KIND(Func_0814fa2c, Func_0814fab8, 7)

// Abilities: Ward, Resist, Magic Shell, Magic Shield (kind 30); Breeze (kind
// 215).
DEFINE_EFFECT_KIND(BattleFx_RunWard, Func_0814fab8, 2)

// Abilities: Steam (kind 221).
DEFINE_EFFECT_KIND(BattleFx_RunSteam, Func_0814fab8, 11)

// Abilities: Granite (kind 200).
DEFINE_EFFECT_KIND(BattleFx_RunGranite, Func_0814fab8, 3)

// Abilities: Flash (kind 213).
DEFINE_EFFECT_KIND(BattleFx_RunFlash, Func_0814fab8, 5)

// Abilities: Shade (kind 222).
DEFINE_EFFECT_KIND(BattleFx_RunShade, Func_0814fab8, 8)

// Abilities: Guard Aura (kind 460).
DEFINE_EFFECT_KIND(BattleFx_RunGuardAura, Func_0814fab8, 12)

// Abilities: Zephyr (kind 216).
DEFINE_EFFECT_KIND(BattleFx_RunZephyr, Func_0814fab8, 4)

// Abilities: Coal (kind 224).
DEFINE_EFFECT_KIND(BattleFx_RunCoal, Func_0814fab8, 9)

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
