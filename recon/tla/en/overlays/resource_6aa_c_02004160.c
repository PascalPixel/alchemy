// Near miss, differ 8 of 131 (tla-en, 2026-10-31): register allocation at the
// first currentOwner address only. The native loads gMainRoutineWork into r1
// and builds 0x214 in r2 (adds r1, r1, r2); the draft loads it into r3 and
// builds 0x214 in r1, the output reload register. The RTL dumps show why:
// local-alloc ties the base and the address in r6, global alloc gives r6 to
// the object pointer, and the base comes back in r3, which no spill at the
// add forbids; the native's form needs r3 taken as well. Tried here, all 8:
// a struct MainRoutineWork pointer, the object assigned after the locals'
// declarations, the scene's forwarded inline calls at each currentOwner use,
// the -= forms and the EventWork field, alchemy permute 4 minutes.
// Draft of overlay 6aa's Func_02004160, the twin of overlay 69c's
// Func_02001db8: its draft (resource_69c_c_02001db8.c) with this overlay's
// symbols: differ 8 of 131 under -mthumb-slow-loads. Left: what that draft's header records.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"
#include "io_reg.h"
#include "event_work.h"

struct PushEvent {
  u8 unk00[6];
  s16 x;
  s16 z;
  u8 unk0a[4];
  u16 busy;
};

void Func_02004068(void);
void Func_02004c20(s32 x, s32 z, s32 a, s32 b, s32 cellX, s32 cellZ);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02004160(struct PushEvent *event) {
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
  Func_02004068();
  Func_02004de0(229);
  Func_02004c20(event->x, event->z + 2, 1, 1, x, z);
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
      Func_02004de0(204);
    object->scaleX -= 0x400;
    object->scaleY -= 0x300;
    object->y -= 0x9999;
    wait(1);
  }
  do {
    Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  } while (0);
  event->busy = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
