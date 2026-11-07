#include "heap_release.h"
#include "types.h"
#include "iwram_call.h"

struct OwnerElementStats {
  s16 power;
  s16 res;
};

struct OwnerStats {
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk0f[5];
  struct OwnerElementStats element[4];
};

struct OwnerState {
  u8 unk00[15];
  u8 level;
  struct OwnerStats stats;
};

struct OwnerState *Unit_Get(s32 owner);
s32 __divsi3(s32 numerator, s32 denominator);
void Unit_CalculateFar(s32 owner);

/// Raises an enemy by levels, The Broken Seal's BattleEnemy_AdjustLevel with
/// HP capped at 16383 instead of 9999. Returns nothing, though declared s32 as
/// there.
s32 Func_0812786c(s32 owner, s32 levels) {
  struct OwnerStats *base;
  struct OwnerState *state;
  struct OwnerStats *stats;
  s32 value;
  s32 floor;
  s32 i;

  base = Heap_AllocEwram(sizeof(struct OwnerStats));
  state = Unit_Get(owner);
  stats = &state->stats;
  Iwram_CopyWords(base, stats, sizeof(struct OwnerStats));

  value = stats->maxHp;
  value += __divsi3(levels * 97, 10);
  floor = __divsi3(base->maxHp * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 16383)
    value = 16383;
  stats->maxHp = value;
  /* FAKEMATCH(barrier): an empty asm after the HP store, which the next
   * stat's level term is built after; as plain statements, 2 of 190
   * instructions differ in tla-en. */
  __asm__ volatile("");

  value = state->stats.maxPp;
  value += __divsi3(levels * 15, 10);
  floor = __divsi3(base->maxPp * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 9999)
    value = 9999;
  state->stats.maxPp = value;

  floor = __divsi3(levels * 123, 10);
  value = state->stats.atk;
  value += floor;
  floor = __divsi3(base->atk * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.atk = value;

  floor = __divsi3(levels * 33, 10);
  value = state->stats.def;
  value += floor;
  floor = __divsi3(base->def * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.def = value;

  floor = __divsi3(levels * 51, 10);
  value = state->stats.agi;
  value += floor;
  floor = __divsi3(base->agi * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.agi = value;
  /* FAKEMATCH(barrier): an empty asm after the Agility store, which the
   * element loop's offset is built after; as plain statements, 2 of 190
   * instructions differ in tla-en. */
  __asm__ volatile("");

  for (i = 0; i < 4; i++) {
    value = state->stats.element[i].power + levels * 15;
    floor = __divsi3(base->element[i].power * 7, 10);
    if (value < floor)
      value = floor;
    if (value > 200)
      value = 200;
    state->stats.element[i].power = value;
  }

  state->level += levels;
  Unit_CalculateFar(owner);
  Heap_Release(base);
}
