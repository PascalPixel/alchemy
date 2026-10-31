// Near miss, differ 5 of 61 (tla-en, 2026-10-31): only the entry block
// differs. The native sets 0xffff's first part (movs r1, #255) before the
// entry jump and copies the count to r10 after the mode. This is overlay
// 6aa's copy of overlay 671's Func_02005428: its draft
// (resource_671_c_02005428.c) with this overlay's symbols, and its header
// records the first test that jump2 cross-jumped into the loop's own test
// and the forms already tried. Tried here as well: as a plain while loop, 38;
// reading the effect work inside the loop through a HeapSlots pointer held
// outside it keeps PRE from hoisting the id, but gcse then propagates the
// fixed address into the loop, so the work load stays inside and loop.c
// hoists 0xffff (36, with an explicit first test too). A 4-minute permuter
// run found nothing better.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_02004a60(s32 id, s32 a, s32 flag, s32 terrain);

// Finds the current target's table row and shrinks it.
void Func_020049dc(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  goto test;
  do {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
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
        Func_02004a60(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}
