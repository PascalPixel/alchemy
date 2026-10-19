// Draft, differ 4 of 91 (42 before): The Broken Seal's
// BattleFormation_AverageLevel (games/tbs/src/battle/battle/formation.c),
// written as it is there, without its level check: the average level (byte
// 15 of each definition) of a formation's enemy kinds, counting only kinds the
// player has met (flag 0x608 + id, or all with flag 0x174): -1 for an empty
// formation, -2 when one present kind is unmet, -3 when none counts. The
// unused array explains the native's 32-byte frame (differ 8 without it), as
// in The Broken Seal. Left: the native loads the record table from the pool
// right after the index's first shift (lsls r3, r0, #1; ldr r2; adds), this
// draft after the add, and it starts that shift after the two zeros' moves
// into sl and r9. Tried: the zeros swapped (6), in a one-pass block (4, sub
// sp moves instead), the table pinned to r2 behind a barrier (10).
#include "types.h"
#include "battle_formation.h"

struct EnemyDefinition {
  u8 unk00[15];
  u8 level;
};

struct EnemyDefinition *Enemy_GetDefinitionFar(s32 id);
s32 GameFlag_Test(s32 flag);

s32 Func_08127588(s32 recordId) {
  struct BattleFormationRecord *record;
  struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  /* FAKEMATCH(dead): an unused array for the frame's 28 bytes; without it, 8
   * of 91 instructions differ in tla-en. */
  s32 levels[7];

  enemyCount = 0;
  levelTotal = 0;
  record = &gBattleFormation_Records[recordId];
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->minimumCounts[memberIndex] != 0)
      break;
  }
  if (memberIndex == 5)
    return -1;
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->maximumCounts[memberIndex] == 0)
      continue;
    enemyId = record->memberIds[memberIndex];
    enemy = Enemy_GetDefinitionFar(enemyId + 8);
    if (enemy == 0)
      continue;
    if (!GameFlag_Test(0x174) && !GameFlag_Test(enemyId + 0x608))
      return -2;
    levelTotal += enemy->level;
    enemyCount++;
  }
  if (enemyCount == 0)
    return -3;
  return levelTotal / enemyCount;
}
