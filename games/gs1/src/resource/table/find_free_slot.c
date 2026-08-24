#include "types.h"
#include "global_cells.h"

s32 Resource_FindFreeSlot(void)
{
  s32 free_slot;
  s32 slot_index;
  void *table_base;
  int first_slot;
  void *entry_cursor;
  entry_cursor = (void *) ADDR_03001B10;
  free_slot = 0x60;
  first_slot = 0;
  slot_index = first_slot;
  table_base = (void *) ADDR_03001B10;
  if ((*((u16 *) (((u8 *) table_base) + 2))) == 0xFFFF)
  {
    return first_slot;
  }
  loop_2:
  slot_index += 1;

  entry_cursor += 4;
  if (slot_index <= 0x5F)
  {
    if ((*((u16 *) (((u8 *) entry_cursor) - -2))) == 0xFFFF)
    {
      free_slot = slot_index;
    }
    else
    {
      goto loop_2;
    }
  }
  return free_slot;
}
