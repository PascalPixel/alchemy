#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"
#include "io_reg.h"
#include "event_inline.h"


struct PushEvent {
  u8 unk00[6];
  s16 x;
  s16 z;
  u8 unk0a[4];
  u16 busy;
};

void Func_02003b74(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02003c6c(struct PushEvent *event) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
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
  Func_02003b74();
  Event_PlaySound(229);
  MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  {
    u32 zero = 0;

    object->angle = 0x4000;
    object->motionFlags = zero;
  }
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  {
    /* FAKEMATCH(temporary): 0xf1f held in a word local, so it is built with
     * movs, lsls, adds as the native does (a plain store loads it from the
     * pool); as a plain store of the constant, 20 of 131 instructions differ
     * in tla-en. */
    u32 winout = 0xf1f;

    REG_WINOUT = winout;
  }
  do {
  } while (0);
  REG_DISPCNT |= 0x8000;
  event->busy = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Event_PlaySound(204);
    object->scaleX += -0x400;
    object->scaleY += -0x300;
    object->y += -0x9999;
    System_Wait(1);
  }
  do {
    Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  } while (0);
  event->busy = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
