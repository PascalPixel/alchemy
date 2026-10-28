#include "types.h"
#include "sequence_parent_work.h"
#include "game_flags.h"

s32 GameFlag_Test(s32 flag);

s32 Party_CountActiveOwners(void) {
  s32 count;
  s32 i;

  count = 0;
  for (i = 0; i <= 7; i++) {
    if (GameFlag_Test(i))
      count++;
  }
  return count;
}

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Party_AddActiveOwner(s32 value) {
  s32 count = Party_CountActiveOwners();
  s32 index;

  GameFlag_SetBit(value);
  index = 0;
  while (index < count) {
    if (gSequenceParentWork.activeOwners[index] == value)
      return count;
    index++;
  }
  gSequenceParentWork.activeOwners[index] = value;
  return count + 1;
}
