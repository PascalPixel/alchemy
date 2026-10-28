// Draft, differ 7 of 61: overlay 664's copy of overlay 671's Func_02005428
// (the same listing), whose draft holds its notes.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_02000128(s32 id, s32 a, s32 flag, s32 terrain);

// Finds the current target's table row and shrinks it.
void Func_020000a4(u16 *list, s32 mode, s32 count) {
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
        Func_02000128(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}
