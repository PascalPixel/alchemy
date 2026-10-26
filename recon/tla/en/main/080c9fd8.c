// Draft, differ 26 of 180 (44 before; 2026-10-26: the enemies as s16, a trial:
// the native reads the group with ldrh, but the signed read gives the pick
// loop the native's registers; as u16, 44) (100 before, 2026-10-25), ported from The Broken
// Seal's Encounter_CountStepsAndSelectFormation
// (games/tbs/src/overworld/overworld/encounter.c); this game drops the
// Jenna experience check, reads the cloak byte at 0x26d and the encounter
// bias and threshold at 0x194 and 0x198 of the event work. gPartyState read
// by field (the native keeps its address across Party_GetAverageLevelFar and
// loads it again for the step count), the threshold at 0x198, Random16
// unsigned (the draw's lsrs) and the zone test on its own (100 to 44). Left:
// which returns of 0 set r0 before their test (the native presets it for
// the cloak and rate tests and shares the flags' return with the zone test),
// the zone's reload register, and the draw's pick and weight pointer, which
// the native keeps in r2 and r0.
#include "types.h"
#include "iwram_call.h"
#include "party_state.h"
#include "ram_buffer.h"

struct EncounterZone {
  u16 rate;
  u16 level;
  s16 enemies[8];
  u8 weights[8];
};

struct EncounterWork {
  u8 unk000[0x194];
  s32 bias;
  s32 threshold;
};

s32 GameFlag_Test(s32 flag);
s32 Party_GetAverageLevelFar(void);
u32 Random16(void);
void BattleEffect_SelectBattleCue(s32 zone, s32 group);
extern const struct EncounterZone gEncounter_EnemyGroupTable[];

#define ENCOUNTER_STEPS (gPartyState.encounterSteps)

s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps) {
  struct EncounterWork *work;
  const struct EncounterZone *entry;
  s32 rate;
  s32 level;
  s32 bias;
  s32 step;
  s32 total;
  s32 sum;
  s32 pick;
  s32 i;

  work = Ram_HeapSlots->eventWork;
  if (GameFlag_Test(351) == 0) {
    if (GameFlag_Test(352) != 0 || GameFlag_Test(353) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gPartyState.cloaked != 0)
      return 0;
    entry = gEncounter_EnemyGroupTable + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;

    level = Party_GetAverageLevelFar() - entry->level;
    if (level < 0)
      level = 0;
    if (level > 5)
      level = 5;
    if (level > 0 && gPartyState.avoidDistance != 0)
      return 0;
    rate += level * 5;

    bias = work->bias;
    if (bias == 0) {
      s32 a = Random16();
      s32 b = Random16();
      s32 c = Random16();

      bias = (a - b + c - (s32)Random16()) / 2;
      work->bias = bias;
    }
    step =
        Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = ENCOUNTER_STEPS += Iwram_MulQ16(step, steps);
    if (total < work->threshold)
      return 0;
  }

  work->bias = 0;
  sum = 0;
  for (i = 0; i < 8; i++)
    sum += entry->weights[i];
  if (sum == 0)
    return 0;
  pick = (Random16() * sum) >> 16;
  pick -= entry->weights[0];
  i = 0;
  if (pick >= 0) {
    do {
      i++;
      if (i > 7)
        break;
      pick -= entry->weights[i];
    } while (pick >= 0);
  }
  sum = entry->enemies[i];
  BattleEffect_SelectBattleCue(zone, i);
  return sum;
}
