#include "types.h"
#include "owner_state.h"
#include "battle_unit.h"

void Func_080af8cc(void) {}

extern u32 gOwner_ExperienceThresholds[];

// レベルに必要な経験値。表は 8 人分、1 人 99 段
u32 Unit_GetExperienceForLevel(s32 owner, s32 level) {
  struct BattleUnit *state = (struct BattleUnit *)UnitGet(owner);

  if (state->classIndex != 0) {
    if (level <= 0) {
      return 0;
    }
    if (level <= 99 && state->enemyId <= 7) {
      u32 *table = gOwner_ExperienceThresholds;
      s32 row = state->enemyId * 99 + level;
      s32 offset;

      /* FAKEMATCH: ⚓️ loads the table's address after the level is added to the
       * row and before the sum is scaled; written as
       * gOwner_ExperienceThresholds[state->enemyId * 99 + level - 1] the load's
       * priority is one below the add's and it comes after it (one reordered
       * pair, differ 2). The table and the sum are forced temporaries held at
       * the barrier, and the offset is spelled out because a table in a
       * register is indexed as (table + sum * 4) - 4 (differ 29). */
      asm volatile("" : "+l"(table));
      offset = row * 4 - 4;
      return *(u32 *)((u8 *)table + offset);
    }
  }
  return -1;
}
