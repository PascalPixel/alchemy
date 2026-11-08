#include "types.h"
#include "owner_state.h"
#include "battle_unit.h"

void Func_080af8cc(void) {}

extern u32 gOwner_ExperienceThresholds[];

/// Experience required per level: eight characters, 99 levels each.
u32 Unit_GetExperienceForLevel(s32 owner, s32 level) {
  struct BattleUnit *state = (struct BattleUnit *)Unit_Get(owner);

  if (state->classIndex != 0) {
    if (level <= 0) {
      return 0;
    }
    if (level <= 99 && state->enemyId <= 7) {
      u32 *table = gOwner_ExperienceThresholds;
      s32 row = state->enemyId * 99 + level;
      s32 offset;

      /* FAKEMATCH: the table and sum held at a barrier load the table between
       * the add and the scaling; as gOwner_ExperienceThresholds[state->enemyId
       * * 99 + level - 1], 2 instructions differ in tla-en. */
      asm volatile("" : "+l"(table));
      offset = row * 4 - 4;
      return *(u32 *)((u8 *)table + offset);
    }
  }
  return -1;
}
