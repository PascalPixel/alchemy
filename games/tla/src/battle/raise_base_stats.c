#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "iwram_call.h"
#include "mem.h"

struct UnitBaseStats {
  s16 hp;
  s16 pp;
  s16 hpGauge;
  s16 ppGauge;
  u16 attack;
  u16 defense;
  u8 unk0c[24];
};

struct BattleUnit *UnitGet(s32 unitId);

/// Raises a unit's base HP by half and base Attack and Defense by a quarter (at
/// least 7/10 of the old values, at most 16383 and 999), from a copy of its base
/// block, then recalculates its stats.
// No ☀️ twin found.
void Func_08127a0c(s32 unitId) {
  struct UnitBaseStats *saved = Runtime_BumpAllocateAlternatePool(36);
  struct BattleUnit *unit = UnitGet(unitId);
  s32 value;
  s32 floor;

  Iwram_CopyWords(saved, &unit->baseHp, 36);
  value = unit->baseHp * 3 / 2;
  floor = saved->hp * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 16383)
    value = 16383;
  unit->baseHp = value;
  value = unit->baseAttack * 5 / 4;
  floor = saved->attack * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  unit->baseAttack = value;
  value = unit->baseDefense * 5 / 4;
  floor = saved->defense * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  unit->baseDefense = value;
  UnitCalcFar(unitId);
  MemFree(saved);
}
