// Near miss, differ 6 of 96 (tla-en, 2026-10-31): scheduler order only (10 on
// 2026-10-26, 16 before). It restores from received save data gMainRoutineWork
// bytes at 0x225, 0x226, 0x22c, 0x22a and 0x24a (the data holds a
// MainRoutineWork at 0x240), calls Func_08038348 with the first two, then for
// the eight units (saved from 0x520 in the BattleUnit layout, 332 bytes each)
// restores base HP and PP, then the gauges from them, base Attack, Defense,
// Agility and Luck, level and experience, and recomputes each with
// Unit_Calculate; last the coins. The saved units read as BattleUnit, so the
// stores keep their order against the loads, and the gauges copy the live
// unit's new HP and PP (16 to 10); the count of 8 set after the call takes r7.
// Left: the native stores the first two party bytes before it steps the next
// save offset (adds r7, #1 and #6), and opens its frame (sub sp, #16) after the
// last byte's store, before the call's two loads. In sched2 a store reaches the
// next load from the save data at cost 1 (-mthumb-slow-loads adds a cycle only
// for the same address), so each offset step (priority 57, then 53) outranks
// its store (56, 52), and the call's two loads (33) are ready the cycle after
// the last store and outrank the frame (32, an anti-dependence on the call). A
// store-to-load cost of 2 would tie both pairs (the store wins on dependants)
// and hold both loads a cycle: the native's order in all three places. A local
// pointer to gMainRoutineWork is far worse (112).
#include "types.h"
#include "main_routine_work.h"
#include "battle_unit.h"

void *Unit_Get(s32 owner);
void Unit_Calculate(s32 unitId);
void Func_08038348(s32 a, s32 b);

void Func_080ae5fc(u8 *save) {
  struct MainRoutineWork *party = (struct MainRoutineWork *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  u8 unused[16]; // Unused: gives the stack frame its 16 bytes.
  s32 i;
  s32 count;

  gMainRoutineWork.paletteGlow[0] = party->paletteGlow[0];
  gMainRoutineWork.paletteGlow[1] = party->paletteGlow[1];
  gMainRoutineWork.eventMode = party->eventMode;
  gMainRoutineWork.battleFxEnabled = party->battleFxEnabled;
  gMainRoutineWork.unk244[6] = party->unk244[6];
  Func_08038348(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  count = 8;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(i);
    struct BattleUnit *saved = &units[i];

    unit->baseHp = saved->baseHp;
    unit->basePp = saved->basePp;
    unit->hpGauge = unit->baseHp;
    unit->ppGauge = unit->basePp;
    unit->baseAttack = saved->baseAttack;
    unit->baseDefense = saved->baseDefense;
    unit->baseAgility = saved->baseAgility;
    unit->baseLuck = saved->baseLuck;
    unit->level = saved->level;
    unit->experience = saved->experience;
    Unit_Calculate(i);
  }
  gMainRoutineWork.coins = party->coins;
}
