#include "types.h"
#include "global_cells.h"
#undef Resource_FindFreeSlot
#define Resource_FindFreeSlot Func_080b6e7c


s32 Resource_FindFreeSlot(s32 key)
{
  u16 *table;
  s32 index;
  int entry;
  table = (u16 *) 0x080C593C;
  index = 0;
  loop_1:
  entry = table[index];
  if (key == (entry & 0x1FF))
  {
    return table[index] >> 9;
  }
  index += 1;
  if (((s16) entry) != -1)
  {
    goto loop_1;
  }
  return 6;
}
