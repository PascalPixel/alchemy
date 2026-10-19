#include "mem.h"
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

struct OwnerState *UnitGetFar(s32 owner);
s32 __divsi3(s32 numerator, s32 denominator);
void UnitCalcFar(s32 owner);

s32 BattleEnemy_AdjustLevel(s32 owner, s32 levels) {
  struct OwnerStats *base;
  struct OwnerState *state;
  struct OwnerStats *stats;
  s32 value;
  s32 floor;
  s32 i;

  base = Runtime_BumpAllocateAlternatePool(sizeof(struct OwnerStats));
  state = UnitGetFar(owner);
  stats = &state->stats;
  Iwram_CopyWords(base, stats, sizeof(struct OwnerStats));

  value = stats->maxHp;
  value += __divsi3(levels * 97, 10);
  floor = __divsi3(base->maxHp * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 9999)
    value = 9999;
  stats->maxHp = value;

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
  UnitCalcFar(owner);
  MemFree(base);
}
