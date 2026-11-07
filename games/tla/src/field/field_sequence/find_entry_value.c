#include "types.h"
#include "field/world_map.h"

void GameFlag_SetBit(s32 flag);

void Func_080c9de0(s32 id, s32 variant) {
  struct FlagEntry *entry = FlagEntries_GetTable();

  for (; entry->flag != 0; entry++) {
    if (entry->id != id)
      continue;
    if (entry->variant != -1 && variant != entry->variant)
      continue;
    GameFlag_SetBit(entry->flag + 0x180);
    break;
  }
}

s32 Func_080c9e1c(s32 flag) {
  struct FlagEntry *entry = FlagEntries_GetTable();

  while (entry->flag != 0) {
    if (entry->flag == flag && entry->value != 0)
      return entry->value;
    entry++;
  }
  return 1;
}
