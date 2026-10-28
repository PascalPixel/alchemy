// Draft, differ 10 of 96 (16 before, 2026-10-26): restores from received save
// data the adventure bytes at 0x225, 0x226, 0x22c, 0x22a and 0x24a (the data
// holds an Adventure at 0x240), calls Func_08038348 with the first two, then
// for the eight units (saved from 0x520 in the BattleUnit layout, 332 bytes
// each) restores base HP and PP, then the gauges from them, base Attack,
// Defense, Agility and Luck, level and experience, and recomputes each with
// Unit_Calculate; last the coins. The saved units read as BattleUnit, so the
// stores keep their order against the loads, and the gauges copy the live
// unit's new HP and PP (16 to 10); the count of 8 set after the call takes r7.
// Left, instruction order only: the native stores the first two party bytes
// before it steps the next offset, opens its frame before the call's two loads,
// and shifts the experience offset before the Attack load. The 16-byte frame
// is never used (kept as an unused array, as the native reserves it).
#include "types.h"
#include "adventure.h"
#include "battle_unit.h"

void *Unit_Get(s32 owner);
void Unit_Calculate(s32 unitId);
void Func_08038348(s32 a, s32 b);

void Func_080ae5fc(u8 *save) {
  struct Adventure *party = (struct Adventure *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  u8 unused[16];
  s32 i;
  s32 count;

  gAdventure.paletteGlow[0] = party->paletteGlow[0];
  gAdventure.paletteGlow[1] = party->paletteGlow[1];
  gAdventure.eventMode = party->eventMode;
  gAdventure.battleFxEnabled = party->battleFxEnabled;
  gAdventure.unk244[6] = party->unk244[6];
  Func_08038348(gAdventure.paletteGlow[0], gAdventure.paletteGlow[1]);
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
  gAdventure.coins = party->coins;
}
