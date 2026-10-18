// Draft, differ 42 of 91: the average level (byte 15 of each definition) of
// a formation's enemy kinds, counting only kinds the player has met (flag
// 1544 + id, or all with flag 372): -1 for an empty formation, -2 when one
// present kind is unmet, -3 when none counts. No ☀️ twin found. Left: other:
// the native frame is 32 bytes (sub sp, #32) where this one needs only the
// counter's 4-byte spill, so 28 bytes of an unused local (no device added
// here); and register choice: the native keeps the record in r1 and i in r4,
// here the record in r0 and i in r1, which also moves the prologue's zeros
// below the record's address. Tried: i as s32 (46).
#include "types.h"
#include "battle_formation.h"

struct EnemyDefinition {
  u8 unk00[15];
  u8 level;
};

struct EnemyDefinition *Enemy_GetDefinitionFar(s32 id);
s32 GameFlag_Test(s32 flag);

s32 Func_08127588(s32 formation) {
  struct BattleFormationRecord *record = &gBattleFormation_Records[formation];
  s32 count = 0;
  s32 total = 0;
  u32 i;

  for (i = 0; i <= 4; i++) {
    if (record->minimumCounts[i] != 0)
      break;
  }
  if (i == 5)
    return -1;
  for (i = 0; i <= 4; i++) {
    if (record->maximumCounts[i] != 0) {
      s32 id = record->memberIds[i];
      struct EnemyDefinition *definition = Enemy_GetDefinitionFar(id + 8);

      if (definition != 0) {
        if (!GameFlag_Test(372) && !GameFlag_Test(id + 1544))
          return -2;
        total += definition->level;
        count++;
      }
    }
  }
  if (count == 0)
    return -3;
  return total / count;
}
