#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

void Func_02000c44(s32 id, s32 a, s32 flag, s32 terrain);

// Finds the current target's table row and shrinks it.
void Func_02000bc0(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  while (*list != 0xffff) {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_02000c44(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  }
}
