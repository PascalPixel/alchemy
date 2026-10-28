#include "types.h"
#include "field_event.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"

extern s32 gData_02001018[];
extern s32 gData_02001068[];
extern s32 gData_020010d0[];
extern s32 gData_02001238[];
extern s32 gData_02001490[];
extern s32 gData_02001664[];

static inline void CenterAndWalk(s32 actor, s32 priority, s32 dz) {
  /* FAKEMATCH: as in the scene layer of The Broken Seal, the inline parameter
   * keeps the native order (direct: -6 is negated before the priority is
   * loaded, 2 differ) */
  Import_ActorCenterAndWalk(actor, priority, dz);
}

static inline void ReturnHome(s32 mode, s32 actor, s32 flag) {
  /* FAKEMATCH: as in the scene layer of The Broken Seal, the inline parameter
   * keeps the native order (direct: 0x300 is shifted before the actor is
   * loaded, 2 differ) */
  Import_ActorReturnHome(mode, actor, flag);
}

s32 *Func_02000044(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Daila2)
    return gData_02001068;
  else
    return gData_02001018;
}

s32 *Func_02000070(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Daila2)
    return gData_02001238;
  else
    return gData_020010d0;
}

void Func_0200009c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_ActorSetSpeed(gSequenceParentWork.currentOwner, 0x8000, 0x4000);
  Import_ActorSetSpritePriority(gSequenceParentWork.currentOwner, 2);
  Import_ActorGet(gSequenceParentWork.currentOwner)->motionFlags = 0;
  Import_SoundPlay(123);
  Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 2);
  CenterAndWalk(gSequenceParentWork.currentOwner, 2, -6);
  Import_EventWait(10);
  Import_EventSetExitRequest(work->exit);
  Import_EventFinishScreenTransition();
  Import_EventWaitTransitionFrames();
  Import_EventEnd();
}

void Func_02000118(s32 mode) { ReturnHome(mode, 66, 0x300); }

s32 *Func_02000128(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Daila2)
    return gData_02001664;
  else
    return gData_02001490;
}
