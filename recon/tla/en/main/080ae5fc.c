// Draft, differ 77 of 96: restores from a save block the party state bytes
// at 0x225, 0x226, 0x22c, 0x22a and 0x24a (the block holds a PartyState at
// 0x240), calls Func_08038348 with the first two, then for the eight units
// (saved from 0x520, 332 bytes each) restores base HP and PP (also as the
// gauges), base Attack, Defense, Agility and Luck, level and experience and
// recomputes each with Unit_Calculate; last the coins. Left: the native builds
// each party offset from the last with an add (one register stepped from
// 0x465 through 0x466, 0x46c, 0x46a and 0x48a, and its twin on the
// gPartyState side), where the draft rebuilds most constants; the native
// reads each saved HP and PP once (the draft reloads them after the store,
// as unit may alias saved); the 16-byte frame is never used (kept as an
// unused array, as the native reserves it). Tried: hp and pp in u16 locals
// (differ 85); the bound as the constant 8 (differ 81, the native holds 8 in
// r7 and compares with blt).
#include "types.h"
#include "party_state.h"
#include "battle_unit.h"

void *Unit_Get(s32 owner);
void Unit_Calculate(s32 unitId);
void Func_08038348(s32 a, s32 b);

void Func_080ae5fc(u8 *save) {
  struct PartyState *party = (struct PartyState *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  u8 unused[16];
  s32 i;
  s32 count = 8;

  gPartyState.paletteGlow[0] = party->paletteGlow[0];
  gPartyState.paletteGlow[1] = party->paletteGlow[1];
  gPartyState.eventMode = party->eventMode;
  gPartyState.battleFxEnabled = party->battleFxEnabled;
  gPartyState.unk244[6] = party->unk244[6];
  Func_08038348(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(i);
    struct BattleUnit *saved = &units[i];

    unit->baseHp = saved->baseHp;
    unit->hpGauge = saved->baseHp;
    unit->basePp = saved->basePp;
    unit->ppGauge = saved->basePp;
    unit->baseAttack = saved->baseAttack;
    unit->baseDefense = saved->baseDefense;
    unit->baseAgility = saved->baseAgility;
    unit->baseLuck = saved->baseLuck;
    unit->level = saved->level;
    unit->experience = saved->experience;
    Unit_Calculate(i);
  }
  gPartyState.coins = party->coins;
}
