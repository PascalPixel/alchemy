#include "types.h"
#include "battle_formation.h"

struct EnemyDefinition {
  u8 unk00[15];
  u8 level;
};

struct EnemyDefinition *Enemy_GetDefinitionFar(s32 id);
s32 GameFlag_Test(s32 flag);

/// The Broken Seal's BattleFormation_AverageLevel without its level check:
/// the average level of a formation's enemy kinds, counting only kinds the
/// player has met (flag 0x608 + id, or all with flag 0x174); -1 for an empty
/// formation, -2 when one present kind is unmet, -3 when none counts.
s32 Func_08127588(s32 recordId) {
  struct BattleFormationRecord *record;
  struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  /* FAKEMATCH(dead): an unused array explains the native's 32-byte stack frame; as
   * the function without it, 4 of 91 instructions differ in tla-en. */
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
