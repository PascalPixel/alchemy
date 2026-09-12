#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "fixed_math.h"

/* resource/initialize.c */
/* resource/table/initialize.c */
struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern u8 ResourceBlockOwners[];
extern struct ResourceTableEntry ResourceTableEntries[];

void Resource_InitializeTable(void)
{
    u32 limit = 0x1ff;
    u8 *occupancy_markers = ResourceBlockOwners;
    u32 count = 0;
    u32 empty_marker = 0xff;

    do {
        *occupancy_markers++ = empty_marker;
        count++;
    } while (count <= limit);

    {
        struct ResourceTableEntry *resource_entry = ResourceTableEntries;

        count = 0;
        do {
            resource_entry->flags |= 0xffff;
            resource_entry->value = 0;
            resource_entry++;
            count++;
        } while (count <= 95);
    }
}

/* resource/table/find_free_slot.c */
s32 find_free_slot(void)
{
  s32 free_slot;
  s32 slot_index;
  void *table_base;
  int first_slot;
  void *entry_cursor;
  entry_cursor = (void *)ADDR_03001B10;
  free_slot = 0x60;
  first_slot = 0;
  slot_index = first_slot;
  table_base = (void *)ADDR_03001B10;
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
