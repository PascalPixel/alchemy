#include "types.h"
#include "owner_state.h"

s32 EnemySlotGet(void) {
  s32 i;
  s32 id;

  for (i = 0; i <= 5; i++) {
    id = i + 0x80;
    if (((u8 *)Unit_Get(id))[0x12A] == 0)
      break;
  }
  if (i == 6)
    return -1;
  return id;
}
