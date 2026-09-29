/* Draft, not exact (2026-09-24): 1 differing halfword. The re-read of the entry
   for the result addresses [index, table] where the reference has [table,
   index]; plain indexing reuses the loaded entry (52 bytes), and every cast,
   offset temporary and pointer spelling tried keeps the swapped operands.
   A volatile first read followed by direct indexing makes the address
   explicit and emits 60 bytes (11 edits); it does not preserve the loop.
   2026-09-29: the swap is cse.c folding the separate address add of the
   pointer spellings: the table register carries its symbol as a constant
   equivalent, and fold_rtx places constants second. Only an address formed
   inside the load (plain indexing) keeps [table, index], and that load is
   then merged with the first read on the fall-through path. A static
   inline slot helper (3 halfwords), a union of the raw halfword and a
   9/7 bitfield (4-byte elements under ARM struct rounding) and a volatile
   table (60 bytes) do not close it. The table itself (0x080c593c) sits 4
   bytes into BattlePres_ActorObjectScript in unidentified.s and needs its
   own label before adoption.
   2026-09-29 alchemy permute (seed 1, 8 jobs, 5 minutes): 177,561 candidates,
   none below the draft's score (register-only swap plus the unlabelled
   table), 114,673 level with it. No operand swap, temporary, cast, pointer
   or index spelling, statement order or loop form in its rewrite set moves
   the reload to [table, index].
   2026-09-29 slice 4: the [table, index] order comes only from an array
   reference to the global table (pointer and u8 spellings expand as [index,
   table]), and that reread survives CSE only in a block CSE does not reach
   from the first load. A break out of the loop, an else branch or a
   continue form gives exactly that separate [table, index] reread, but
   jump1 then copies the loop head in front of the loop (1085 to 2105). A
   goto loop, a local table pointer (strength-reduced), do/while (0) around
   the load or the reread, loading the value before the key test, and signed
   or masked rereads (merged through the zero-extended load) do not reach it
   either. */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#undef Resource_FindFreeSlot

extern u16 Resource_SlotAssignments[];

s32 Resource_FindFreeSlot(s32 key)
{
  s32 index;
  int entry;
  s32 result;
  for (index = 0; ; index += 1)
  {
    entry = Resource_SlotAssignments[index];
    if (key == (entry & 0x1FF))
    {
      result = *(u16 *) (index * 2 + (u8 *) Resource_SlotAssignments) >> 9;
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
