// Draft, differ 187 of 160: other, register pressure. The native stack frame is
// 56 bytes with the ids at sp + 8: it spills the store pointer of the
// distinct-id loop (sp 4) across the call and the sort's halfword swap
// value (sp 2), keeps the ids' base in r7 and the -1 end mark in r11; this
// draft needs no spill (frame 48), so allocation differs from the first
// loop on. An explicit store pointer (*out++ = id) measured 182; the swap
// through two s16 locals 178, the swap's form the spill shows is this one.
// Draft: runs the field events an effect touched (heap slot 0xe0 effect
// work). Each cell in the event work's list at 0xcc0 (x, z pairs ending in
// -1) is resolved to an event id at the owner object's layer byte; the
// distinct ids are sorted ascending, and for each one Func_080ce458 finds an
// event for type 0x50000005 and the effect's kind, which runs with the
// effect's two arguments.
#include "types.h"
#include "ram_buffer.h"

struct TouchWork {
  u8 unk00[16];
  u8 *owner;
  u8 unk14[4];
  s16 argument0;
  s16 argument1;
  u8 unk1c[2];
  s16 kind;
};

struct TouchCell {
  s16 x;
  s16 z;
};

s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
s32 Func_080ce458(u32 type, s32 kind, s32 id);
void EventRuntime_RunPendingEvent(s32 event, s32 argument0, s32 argument1);

void Func_080e249c(void) {
  struct TouchWork *work = Ram_HeapSlots->effectWork;
  struct TouchCell *cell =
      (struct TouchCell *)((u8 *)Ram_HeapSlots->eventWork + 0xcc0);
  u8 *owner = work->owner;
  s32 count = 0;
  s16 ids[24];
  s32 i, j;

  for (; cell->x != -1; cell++) {
    s16 id = Func_080dbdc8(cell->x << 20, cell->z << 20, owner[34]);

    for (i = 0; i < count; i++) {
      if (ids[i] == id)
        break;
    }
    if (i == count)
      ids[count++] = id;
  }
  ids[count] = -1;

  if (count > 1) {
    for (i = 0; i < count - 1; i++) {
      for (j = i + 1; j < count; j++) {
        if (ids[i] > ids[j]) {
          s16 swap = ids[j];

          ids[j] = ids[i];
          ids[i] = swap;
        }
      }
    }
  }

  for (i = 0; ids[i] != -1; i++) {
    s32 event = Func_080ce458(0x50000005, work->kind, ids[i]);

    if (event != 0)
      EventRuntime_RunPendingEvent(event, work->argument0, work->argument1);
  }
}
