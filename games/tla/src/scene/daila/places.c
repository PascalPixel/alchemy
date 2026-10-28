#include "types.h"
#include "field_event.h"
#include "adventure.h"
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
  Engine_ActorCenterAndWalk(actor, priority, dz);
}

static inline void ReturnHome(s32 mode, s32 actor, s32 flag) {
  /* FAKEMATCH: as in the scene layer of The Broken Seal, the inline parameter
   * keeps the native order (direct: 0x300 is shifted before the actor is
   * loaded, 2 differ) */
  Engine_ActorReturnHome(mode, actor, flag);
}

s32 *Func_02000044(void) {
  if (gAdventure.scene == (s32)&SceneId_Daila2)
    return gData_02001068;
  else
    return gData_02001018;
}

s32 *Func_02000070(void) {
  if (gAdventure.scene == (s32)&SceneId_Daila2)
    return gData_02001238;
  else
    return gData_020010d0;
}

void Func_0200009c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Engine_EventStart();
  Engine_EventResolvePendingActions(0);
  Engine_ActorSetSpeed(gAdventure.currentOwner, 0x8000, 0x4000);
  Engine_ActorSetSpritePriority(gAdventure.currentOwner, 2);
  Engine_ActorGet(gAdventure.currentOwner)->motionFlags = 0;
  Engine_AudioPlayCue(123);
  Engine_ActorSetAnimation(gAdventure.currentOwner, 2);
  CenterAndWalk(gAdventure.currentOwner, 2, -6);
  Engine_EventWait(10);
  Engine_EventSetCountdown(work->exit);
  Engine_EventCloseScreen();
  Engine_EventWaitDelay();
  Engine_EventEnd();
}

void Func_02000118(s32 mode) { ReturnHome(mode, 66, 0x300); }

s32 *Func_02000128(void) {
  if (gAdventure.scene == (s32)&SceneId_Daila2)
    return gData_02001664;
  else
    return gData_02001490;
}
