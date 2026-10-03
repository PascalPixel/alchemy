#include "RESOURCE.H"
/*
 * Draft: Resource_FindFreeEntry does not yet match; ☀️'s C leaves two load
 * swaps (ldrh of the entry state before lsls of 0xffff).
 * Links as recon/tla/raw/080143ac.s.
 */
#include "TYPES.H"

extern u8 ResourceTableEntries[];

s32 Resource_FindFreeEntry(void)
{
  s32 free_slot;
  s32 slot_index;
  void *table_base;
  int first_slot;
  void *entry_cursor;
  entry_cursor = (void *)((u32)ResourceTableEntries);
  free_slot = 0x60;
  first_slot = 0;
  slot_index = first_slot;
  table_base = (void *)((u32)ResourceTableEntries);
  if ((*((u16 *)(((u8 *)table_base) + 2))) == 0xFFFF)
  {
    return first_slot;
  }
  loop_2:
  slot_index += 1;

  entry_cursor += 4;
  if (slot_index <= 0x5F)
  {
    if ((*((u16 *)(((u8 *)entry_cursor) - -2))) == 0xFFFF)
    {
      free_slot = slot_index;
    } else
    {
      goto loop_2;
    }
  }
  return free_slot;
}

