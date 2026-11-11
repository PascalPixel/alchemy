#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

struct LohoPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const struct LohoPicture gData_02001f14;

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020012e0(void);
void Func_020012e8(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020012a0_(const void *data, s32 a, s32 b) {
  Map_PlayMapEdit(data, a, b);
}

static __inline__ void Offset_(s32 id, s32 dx, s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000e54(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Func_020012e0();
  Event_ResolvePendingActions(0);
  // The loop is written with goto so that it carries no loop notes: the bytes
  // rebuild the zero on every pass, and a for loop hoists it and moves the work
  // pointer (differ 4).
  i = 8;
loop:
  {
    struct Actor *object = Actor_GetById(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  work = (u8 *)&((struct EventWork154 *)work)->value;
  Sound_Play(158);
  Func_020012a0_(gData_02001f14.data, gData_02001f14.a, gData_02001f14.b);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Offset_(gMainRoutineWork.currentOwner, 0, -4);
  Sound_Play(123);
  Event_Wait(6);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020012e8();
}
