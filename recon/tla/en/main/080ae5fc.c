// Draft, differ 30 of 96 (77 before, 2026-10-25): restores from a save block
// the party state bytes at 0x225, 0x226, 0x22c, 0x22a and 0x24a (the block
// holds a PartyState at 0x240), calls Func_08038348 with the first two, then
// for the eight units (saved from 0x520, 332 bytes each) restores base HP and
// PP (also as the gauges), base Attack, Defense, Agility and Luck, level and
// experience and recomputes each with Unit_Calculate; last the coins.
// The saved units read through their own struct type, which alchemy-gcc's
// alias sets keep apart from the live BattleUnit, read each saved HP and PP
// once as the native does (77 to 32), and the count of 8 set after the call
// takes r7 (32 to 30). Left, instruction order only: the native issues each
// constant's shift and the stores one slot later than alchemy-gcc, and
// loads the saved PP before it stores the HP gauge. The 16-byte frame is
// never used (kept as an unused array, as the native reserves it).
#include "types.h"
#include "party_state.h"
#include "battle_unit.h"

void *Unit_Get(s32 owner);
void Unit_Calculate(s32 unitId);
void Func_08038348(s32 a, s32 b);

struct SavedUnit {
  u8 name[0x0f];
  u8 level;
  s16 baseHp;
  s16 basePp;
  s16 unk14[2];
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 unk1f[0x124 - 0x1f];
  u32 experience;
  u8 unk128[0x14c - 0x128];
};

void Func_080ae5fc(u8 *save) {
  struct PartyState *party = (struct PartyState *)(save + 0x240);
  struct SavedUnit *units = (struct SavedUnit *)(save + 0x520);
  u8 unused[16];
  s32 i;
  s32 count;

  gPartyState.paletteGlow[0] = party->paletteGlow[0];
  gPartyState.paletteGlow[1] = party->paletteGlow[1];
  gPartyState.eventMode = party->eventMode;
  gPartyState.battleFxEnabled = party->battleFxEnabled;
  gPartyState.unk244[6] = party->unk244[6];
  Func_08038348(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  count = 8;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(i);
    struct SavedUnit *saved = &units[i];

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
