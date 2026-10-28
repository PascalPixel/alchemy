// Draft of overlay 69C Func_02001db8 (first written 2026-10-12); overlay 6A3
// has the same function at 0200c1ac. Its sibling Func_02001d48 is adopted in
// games/tla/src/scene/jupiter_lighthouse_room_3/s1cc0.c.
// Func_02001db8: differ 8 of 131 under -mthumb-slow-loads, one place left: the
// first currentOwner address builds its pool load in r3 and the 0x214 offset in
// r1 (adds r1, r1, r3), where the native loads the pool word into r1 and the
// offset into r2 (adds r1, r1, r2). Combine folds the offset into the add here,
// so reload builds it in the output register. A one-pass block around the
// animationKind reset now orders its zero as the native does (differ 12 to 8).
// What the bytes needed so far: x reused as the loop counter (differ 83 to 34),
// a word temporary for 0xf1f, the mask in a variable set before a one-pass
// boundary after the x shift, a one-pass boundary after the WINOUT store and
// the one around animationKind. Those devices need FAKEMATCH tags in the S2
// form when the function is adopted. Tried: the owner through a struct MainRoutineWork
// pointer (44) or an s32 pointer (58), a byte-offset read of 0x214 (9), the
// object declared after the mask, alchemy permute 8 minutes (8).
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"
#include "io_reg.h"

struct PushEvent {
  u8 unk00[6];
  s16 x;
  s16 z;
  u8 unk0a[4];
  u16 busy;
};

void Func_02001cc0(void);
void Func_02002328(s32 x, s32 z, s32 a, s32 b, s32 cellX, s32 cellZ);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02001db8(struct PushEvent *event) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x;
  s32 z;
  s32 mask;

  mask = 64;
  x = object->x >> 20;
  do {
  } while (0);
  x |= (s16)(event->x & mask);
  z = object->z >> 20;
  z |= (s16)(event->z & mask);
  Func_02001cc0();
  Func_02002470(229);
  Func_02002328(event->x, event->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  {
    u32 zero = 0;

    object->angle = 0x4000;
    object->motionFlags = zero;
  }
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  {
    /* FAKEMATCH: a word temporary builds 0xf1f with movs, lsls, adds as the
     * native does; a halfword constant store loads it from the pool. */
    u32 winout = 0xf1f;

    REG_WINOUT = winout;
  }
  do {
  } while (0);
  REG_DISPCNT |= 0x8000;
  event->busy = 1;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Func_02002470(204);
    object->scaleX += -0x400;
    object->scaleY += -0x300;
    object->y += -0x9999;
    wait(1);
  }
  do {
    Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  } while (0);
  event->busy = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
