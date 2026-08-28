#include "types.h"
#include "global_cells.h"
#undef Resource_FindFreeSlot
#define Resource_FindFreeSlot Func_080b6e7c

extern u16 Data_080c593c_a[];

s32 Resource_FindFreeSlot(s32 key)
{
  s32 index;
  int entry;
  s32 result;
  for (index = 0; ; index += 1)
  {
    entry = Data_080c593c_a[index];
    if (key == (entry & 0x1FF))
    {
      result = *(u16 *) ((u8 *) Data_080c593c_a + index * 2) >> 9;
      goto done;
    }
    if (((s16) entry) == -1)
    {
      break;
    }
  }
  result = 6;
done:
  return result;
}
