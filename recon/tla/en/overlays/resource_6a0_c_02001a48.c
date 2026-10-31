// Matches: differ 0 of 61 (tla-en, 2026-10-31).
// Overlay 6a0's copy of the function drafted in resource_671_c_02005428.c
// (overlay 671's Func_02005428), whose header says what it is and what was
// tried. The native's entry is a first test that jump2's cross-jumping merged
// into the loop's own test, as that header describes; written so, gcse's PRE
// hoists the id load onto the edge into the loop unless the effect work is
// read through its own address, which keeps the id load inside the loop.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_02001acc(s32 id, s32 a, s32 flag, s32 terrain);

// Finds the current target's table row and shrinks it.
void Func_02001a48(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
  // A goto loop: as a do-while loop, 42 of 61 instructions differ in tla-en.
top:
  {
    u32 entry = *list;
    /* FAKEMATCH(dead): the effect work read through its own address; as
     * work->objectId, 21 of 61 instructions differ in tla-en. */
    s32 id = (*&work)->objectId;

    if (id == entry) {
      /* FAKEMATCH(pin): the object in r6, as the native keeps it; as a plain
       * local, 18 of 61 instructions differ in tla-en. */
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
        Func_02001acc(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}
