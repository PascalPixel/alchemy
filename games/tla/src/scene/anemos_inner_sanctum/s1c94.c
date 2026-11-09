#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

struct AnemosInnerSanctumPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const s16 gData_020043e4[];
extern const struct AnemosInnerSanctumPicture gData_02004410[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003398(void);
void Func_020033a0(void);

static __inline__ void Func_02003320_(const void *data, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Map_PlayMapEdit(data, a, b);
}

/// The scene opening: clears the actors' motion flags, plays the entrance's
/// map edit and steps the leader in.
void Func_02001c94(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Func_02003398();
  Event_ResolvePendingActions(0);
  // A goto loop, as in overlay 6a6's Func_02000e54 (its twin): it carries no
  // loop notes.
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
  Sound_Play(158);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room1) {
    Map_PlayMapEdit(gData_020043e4, 33, 7);
  } else {
    s32 index = work->value / 2 - 1;

    Func_02003320_(gData_02004410[index].data, gData_02004410[index].a,
                   gData_02004410[index].b);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 0, -4);
  Event_Wait(6);
  Import_EventSetExitRequest(work->value);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020033a0();
}
