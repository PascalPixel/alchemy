/// @file
/// The scene's setup: places its actors, then plays what the entrance it was
/// reached by calls for.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

extern char SceneId_Ov688;

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020005a4(s32 scene, s32 entrance);
void Func_020003cc(void);

s32 Func_02000190(void) {
  s32 *screen = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  s32 effect = 0x204;

  *screen = effect;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  Event_SetFlag(0x907);
  Event_ClearFlag(0x9b4);
  if (Event_GetFlag(0x909))
    Actor_SetPosition(19, 0, 0);
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  Actor_SetSingleChildFlags(Actor_Get(13), 0);
  Actor_SetSingleChildFlags(Actor_Get(14), 0);
  Actor_SetSingleChildFlags(Actor_Get(15), 0);
  Actor_SetSingleChildFlags(Actor_Get(16), 0);
  Actor_SetSingleChildFlags(Actor_Get(17), 0);
  Actor_SetSingleChildFlags(Actor_Get(18), 0);
  if (gMainRoutineWork.entrance == 90) {
    gMainRoutineWork.ownerOverride = 1;
    Func_020005a4((s32)&SceneId_Ov688, 1);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5) {
    gMainRoutineWork.currentOwner = 8;
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gMainRoutineWork.currentOwner = 4;
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  }
  /* FAKEMATCH(wrapper): forwarded, so that the flag number is built for each call; as
   * Event_GetFlag(0x96b), 15 of 153 instructions differ in tla-en. */
  if (gMainRoutineWork.entrance == 3 && Event_GetFlag(0x96b)) {
    Event_ClearFlag(0x96b);
    Func_020003cc();
  }
  return 0;
}
