// Under -mthumb-slow-loads (2026-10-26): differ 4 of 54 in tla-en, instruction
// order only. As Func_080d8d68: an empty asm before the call lowers it to 2;
// written with the plain constant 0x80000 (no lift) it differs 6.
// Near miss, differ 6 of 54, instruction order only (8 without the tagged
// barrier). The native copies the first id to r8 after the work's load,
// loads the second end's y after building 0x80000, and adds the raise to the
// first y straight after; this draft loads that y first and adds the first
// raise last. Func_080d9e98 and Func_080d9104 share the pattern.
#include "worka0.h"
#include "object_runtime.h"

s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z);

void Func_080da1b8(s32 index, s32 from, s32 to) {
  struct WorkA0 *work = WORK_A0;
  struct ObjectRuntime *a = ObjectTable_Get(from);
  struct ObjectRuntime *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL) {
    struct WorkA0Slot *slot = &work->slots[index];
    s32 lift;

    slot->first = from;
    slot->second = to;
    lift = 128;
    /* FAKEMATCH: starts 0x80000 before the stack arguments load, as the
     * native does (differ 8 without it). */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080da22c(index, a->x, a->y + lift, a->z, b->x, b->y + lift, b->z);
  }
}
