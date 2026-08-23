#include "types.h"
#include "global_cells.h"

s32 Resource_FindFreeSlot(void)
{
  s32 result;
  s32 index;
  void *base;
  int zero;
  void *entry;
  entry = (void *) ADDR_03001B10;
  result = 0x60;
  zero = 0;
  index = zero;
  base = (void *) ADDR_03001B10;
  if ((*((u16 *) (((u8 *) base) + 2))) == 0xFFFF)
  {
    return zero;
  }
  loop_2:
  index += 1;

  entry += 4;
  if (index <= 0x5F)
  {
    if ((*((u16 *) (((u8 *) entry) - -2))) == 0xFFFF)
    {
      result = index;
    }
    else
    {
      goto loop_2;
    }
  }
  return result;
}
