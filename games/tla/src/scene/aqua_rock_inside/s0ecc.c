#include "local.h"
#include "io_write_queue.h"

void Func_02003cfc(void);
void Func_02003d04(void);

void Func_02000ecc(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  if (object->x >> 20 == 49) {
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
    s32 i;

    Func_02003d7c();
    Func_02003eac(0);
    ObjectMotion_ArmCallback_(gPartyState.currentOwner, 0x4000, 0);
    Func_02003ef4(0x22d);
    Object_SetModeById(9, 2);
    Func_02003d24_(114, 39, 1, 2, 113, 39);
    for (i = 31; i >= 0; i--) {
      layer[3] -= 0x10000;
      WaitFrames(4);
    }
    /* FAKEMATCH: the loop is written with goto so that it carries no loop
     * notes: the bytes rebuild the zero and the depth on every pass, and a for
     * loop hoists both. */
    i = 13;
  place:
    object = Object_GetById(i);
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 102) << 16;
    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    ObjectMotion_SetActionVariant(i, 3);
    i++;
    if (i <= 15)
      goto place;
    Func_02003d04();
    /* FAKEMATCH: as above; here a for loop hoists the queue and the interrupt
     * register addresses (differ 24). */
    i = 12;
  blend:
    IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000 | i, 0x20000);
    WaitFrames(4);
    i--;
    if (i >= 0)
      goto blend;
    Func_02003d1c(105, 41, 12, 13, 41, 41);
    Func_02003d24(105, 105, 12, 13, 105, 41);
    Func_02003cfc();
    Func_02003d84();
    GameFlag_SetBit(0x238);
  }
}
