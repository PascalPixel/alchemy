// Near miss, differ 7 of 61 (tla-en, 2026-10-31): the entry's first test
// (movs r1, #255 before the jump), the list word read before the id, and the
// count copied after the mode. Overlay 668's copy of overlay 671's
// Func_02005428 (the same listing), whose draft holds its earlier notes.
// Written with a first test (while (1) { if (*list == 0xffff) break; ... },
// or the if before a goto loop), the prologue matches the listing up to the
// jump, but gcse's PRE moves the id load onto the split entry edge (ldrh, its
// sign extension apart, 11; the permuter found nothing under 16 from there).
// An asm memory barrier after list += 3 stops PRE but lets loop.c move the
// end mark and the id out of the loop (42). So the listing's loop head had no
// entry edge when gcse ran: its first test came later and jump2 merged it
// into the loop's own test after sched2.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020048ec(s32 id, s32 a, s32 flag, s32 terrain);

// Finds the current target's table row and shrinks it.
void Func_02004868(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  goto test;
  do {
    s32 id = work->objectId;

    if (id == *list) {
      /* FAKEMATCH(pin): the object in r6, as the native keeps it; as a plain
       * local, 27 of 61 instructions differ in tla-en. */
      register struct Actor *object asm("r6") = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_020048ec(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}
