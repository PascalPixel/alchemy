#include "types.h"
#include "iwram_call.h"
#include "main_routine_work.h"
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

#define ENCOUNTER_STEPS (gMainRoutineWork.encounterSteps)

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
  /* FAKEMATCH(barrier): an empty asm reading pick before it is set, which
   * makes it live from the entry and keeps a register for it across the whole
   * function; without it, 42 of 180 instructions differ in tla-en. */
  asm("" : : "l"(pick));

  work = Ram_HeapSlots->eventWork;
  if (GameFlag_Test(351) == 0) {
    if (GameFlag_Test(352) != 0 || GameFlag_Test(353) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gMainRoutineWork.cloaked != 0)
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
    if (level > 0 && gMainRoutineWork.avoidDistance != 0)
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
  for (i = 0; i < 8; i++) {
    pick -= entry->weights[i];
    if (pick < 0)
      break;
  }
  sum = (u16)entry->enemies[i];
  BattleEffect_SelectBattleCue(zone, i);
  return sum;
}
