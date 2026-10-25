#include "types.h"
#include "battle_formation.h"
#include "system.h"

s32 BattleFormation_GetRandomMember(s32 recordId) {
  s32 *choicesBase;
  s32 choices[5];
  struct BattleFormationRecord *record;
  s32 *choiceCursor;
  u8 *maximumCount;
  u16 *memberId;
  s32 remaining;
  u32 count;

  count = 0;
  if ((u32)recordId >= 660)
    recordId = 1;

  record = &gBattleFormation_Records[recordId];
  choicesBase = choices;
  memberId = record->memberIds;
  maximumCount = record->maximumCounts;
  choiceCursor = choicesBase;
  remaining = 4;
  do {
    if (*maximumCount++ != 0) {
      s32 id = *memberId;

      count++;
      *choiceCursor++ = id + 8;
    }
    remaining--;
    memberId++;
  } while (remaining >= 0);

  return choices[(Random16() * count) >> 16];
}
