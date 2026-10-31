// Near miss, differ 2 of 70 (tla-en, 2026-10-30): the native stores -1 to
// gData_02006710 straight after the tests; here the store follows the first
// literal load and constant of the copy. Under -mthumb-slow-loads the store
// ranks 17 on sched2's critical path (2 to the load of Data_020067f4, which
// ranks 15), below that load's address (18) and the 0xffff build (18), so no
// plain form issues it first. An empty asm after the store places it first
// only when the store comes before x and z, and then the address of
// gData_02006710 ranks above the cell key in global allocation and takes r4
// (differ 6). Tried without gain: the store between x and z (6), z before x
// (26), the two tests as separate ifs (2), z as (u16)last (22); permute 4
// minutes (17,341 candidates) found nothing better.
// The cell stampers' scheduled callback (Func_0200255c adds it): once the
// controlled object stands still on a new cell, it puts back the cells saved
// at (73, 72) over the last stamp and forgets the cell; in room 8 it also
// stores the old cell's value plus 200 in the event work (byte 362).
// The forgotten cell is stored after x and z are taken from it, which leaves
// its address in r2 (stored first: r4, 6), and the value plus 200 goes through
// a temporary so it is computed before the store's address (8 without).
// Permute 8 minutes found the temporary.
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
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov692Room8)
    return;
  work = Ram_HeapSlots->eventWork;
  value = Func_02000658(2, x, z) >> 8;
  if (value != 0)
    {
      /* FAKEMATCH(temporary): the value plus 200 held in delay before the
         store; as *(s16 *)(work + 362) = value + 200, 8 of 70 instructions
         differ in tla-en. */
      s32 delay = value + 200;
      *(s16 *)(work + 362) = delay;
    }
}
