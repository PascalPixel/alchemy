// Draft, differ 2 of 70: the cell stampers' scheduled callback (Func_0200255c
// adds it): once the controlled object stands still on a new cell, it puts back
// the cells saved at (73, 72) over the last stamp and forgets the cell; in room
// 8 it also stores the old cell's value plus 200 in the event work (byte 362).
// The forgotten cell is stored after x and z are taken from it, which leaves
// its address in r2 (stored first: r4, 6), and the value plus 200 goes through
// a temporary so it is computed before the store's address (8 without).
// Left: the native stores -1 straight after the tests; here the store follows
// the first constants of the copy. Permute 8 minutes found the temporary.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"

extern s32 Data_020067ec;
extern s32 Data_020067f0;
extern s32 Data_020067f4;
extern s32 Data_020067f8;

s32 Func_02004f00(void);
void Func_02004cf0(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
s32 Func_02000658(s32 layer, s32 x, s32 z);

static inline void CopyCells(s32 srcX, s32 srcZ, s32 width, s32 height,
                             s32 destX, s32 destZ) {
  Func_02004cf0(srcX, srcZ, width, height, destX, destZ);
}

void Func_020024ac(void) {
  struct Actor *object = Actor_GetById(Func_02004f00());
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  u8 *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gData_02006710;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gData_02006710 = -1;
  CopyCells(73, 72, Data_020067ec, Data_020067f0, Data_020067f4,
            Data_020067f8);
  if (gSequenceParentWork.scene != (s32)&SceneId_Ov692Room8)
    return;
  work = Ram_HeapSlots->eventWork;
  value = Func_02000658(2, x, z) >> 8;
  if (value != 0)
    {
      s32 delay = value + 200;
      *(s16 *)(work + 362) = delay;
    }
}
