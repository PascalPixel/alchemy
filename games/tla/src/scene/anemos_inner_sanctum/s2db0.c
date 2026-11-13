/// @file
/// Scene code of overlay 0x6b0, in ROM order.

#include "../shared/jupiter_lighthouse_room_3.h"
#include "io_reg.h"
#include "event_work.h"
#include "field.h"
#include "module/scene/anemos_inner_sanctum.h"
#include "module/scene/scene.h"

void Func_02002db0(struct PushEvent *event) {
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
  Func_02002cb8();
  Sound_Play(229);
  Map_CopyCellsRect(event->x, event->z + 2, 1, 1, x, z);
  Event_Wait(12);
  {
    u32 zero = 0;

    object->facing = 0x4000;
    object->motionFlags = zero;
  }
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  {
    u32 winout = 0xf1f;

    REG_WINOUT = winout;
  }
  do {
  } while (0);
  REG_DISPCNT |= 0x8000;
  event->busy = 1;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  Event_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Sound_Play(204);
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
