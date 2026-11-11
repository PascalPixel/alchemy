#include "types.h"
#include "event_inline.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"

extern s32 gData_02001018[];
extern s32 gData_02001068[];
extern s32 gData_020010d0[];
extern s32 gData_02001238[];
extern s32 gData_02001490[];
extern s32 gData_02001664[];

static inline void ReturnHome(s32 mode, s32 actor, s32 flag) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, as in the scene
   * layer of The Broken Seal; as a direct call, which shifts 0x300 before
   * the actor is loaded, 2 of 6 instructions differ in tla-en. */
  Import_ActorReturnHome(mode, actor, flag);
}

s32 *Func_02000044(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gData_02001068;
  else
    return gData_02001018;
}

s32 *Func_02000070(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gData_02001238;
  else
    return gData_020010d0;
}

void Func_0200009c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Sound_Play(123);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -6);
  Event_Wait(10);
  Import_EventSetExitRequest(work->exit);
  Import_EventFinishScreenTransition();
  Import_EventWaitTransitionFrames();
  Import_EventEnd();
}

void Func_02000118(s32 mode) { ReturnHome(mode, 66, 0x300); }

s32 *Func_02000128(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gData_02001664;
  else
    return gData_02001490;
}
