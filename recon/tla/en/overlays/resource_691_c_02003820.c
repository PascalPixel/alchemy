// Matches: differ 0 (tla-en, 2026-11-01)
// Plain C reaches differ 4 of 88: only the entry order of the argument stores
// and the count's copy. The split constant of Ram_HeapSlots outranks the two
// stores in sched2 (priority 7 against 6), so the barrier holds them back. A
// one-pass block stopped at differ 2, volatile and a wrapper gained nothing.
// The loop is goto so that 0x1600 and 63 stay in the body, and the mask is a
// local in its own block because written in the & it comes from the pool.
#include "types.h"
#include "system/ram_buffer.h"

struct Entry {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 unk18;
};

struct EntryWork {
  u8 unk0[0x500];
  struct Entry entries[64];
  u8 unkc00[0xa00];
  s16 unk1600;
};

u32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, void *position);

void Func_02003820(s32 x, s32 y, s32 z, s32 count) {
  struct EntryWork *work;
  s32 i;

  /* FAKEMATCH(barrier): the empty asm kept; as plain C with no asm, 4 of 88 instructions differ in tla-en. */
  asm("");
  work = Ram_HeapSlots->menuRuntime;
  if (count > 0) {
    i = count;
  next:
    {
      struct Entry *entry;
      s32 angle = Random16Far();

      entry = &work->entries[work->unk1600];
      entry->unk18 = 0;
      entry->x = x;
      entry->y = y;
      entry->z = z;
      Vector_AddPolarOffsetFar(Random16Far() * 8 + 0x80000, angle, entry);
      entry->velocityX = 0;
      entry->velocityY = 0x50000;
      entry->velocityZ = 0;
      Vector_AddPolarOffsetFar(Random16Far() * 2 + 0x20000, angle,
                               &entry->velocityX);
      {
        s32 mask = 63;

        work->unk1600 = (work->unk1600 + 1) & mask;
      }
      i--;
    }
    if (i != 0)
      goto next;
  }
}
