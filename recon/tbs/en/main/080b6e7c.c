/* Draft, not exact (2026-09-24): 1 differing halfword. The re-read of the entry
   for the result addresses [index, table] where the reference has [table,
   index]; plain indexing reuses the loaded entry (52 bytes), and every cast,
   offset temporary and pointer spelling tried keeps the swapped operands.
   A volatile first read followed by direct indexing makes the address
   explicit and emits 60 bytes (11 edits); it does not preserve the loop. */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
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
