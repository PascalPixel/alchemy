#include "../../../../../../games/tla/src/scene/magma_rock_inside/local.h"

void Func_020026c0(void);
extern s32 gData_02006c0c;

extern char SceneId_Ov664Room5;
extern s32 gData_020073dc;
extern s32 gData_020073e0;
extern s32 gData_020073e4;
extern s32 gData_020073e8;

s32 Func_02005f84(void);
void Func_02005db4(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
s32 Func_02000690(s32 layer, s32 x, s32 z);

static inline void CopyCells(s32 srcX, s32 srcZ, s32 width, s32 height,
                             s32 destX, s32 destZ) {
  /* FAKEMATCH(wrapper): CopyCells forwarded inline; as direct calls to
   * Func_02005db4, 12 of 70 instructions differ in tla-en. */
  Func_02005db4(srcX, srcZ, width, height, destX, destZ);
}

void Func_020026c0(void) {
  struct Actor *object = Actor_GetById(Func_02005f84());
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  u8 *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gData_02006c0c;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gData_02006c0c = -1;
  CopyCells(73, 72, gData_020073dc, gData_020073e0, gData_020073e4, gData_020073e8);
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov664Room5)
    return;
  work = Ram_HeapSlots->eventWork;
  value = Func_02000690(2, x, z) >> 8;
  if (value != 0) {
    /* FAKEMATCH(temporary): the value plus 200 held in delay before the
       store; as *(s16 *)(work + 362) = value + 200, 6 of 70 instructions
       differ in tla-en. */
    s32 delay = value + 200;
    *(s16 *)(work + 362) = delay;
  }
}
