#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

extern const u8 MsgGuardsAnIntruder[];
extern const u8 MsgGrrrrrRrrruffRrufff[];

void Actor_ResetTargetsAndVelocity(s32 id);
void Func_02001084(void);
void Func_0200108c(void);
void Func_0200113c(s32 scene, s32 entrance);

void Func_02000d80(void) {
  s32 moved = GameFlag_Test(0x300);
  s32 scene = (s32)&SceneId_Ov66fRoom2;

  if (moved)
    Func_0200113c(scene, 9);
  else
    Func_0200113c(scene, 8);
}

void Func_02000da8(s32 id) {
  s32 owner = gMainRoutineWork.currentOwner;

  Func_02001084();
  Event_ResolvePendingActions(0);
  Actor_ResetTargetsAndVelocity(id);
  Actor_ShowEmote(id, 0x100, 30);
  Actor_SetAngleToward(id, owner, 20);
  Event_SetMessageId(MsgGuardsAnIntruder);
  Event_ShowMessageAndWait(id, 0);
  Func_080d489c(owner, 0x101);
  Actor_SetModeById(id, 4);
  Event_Wait(30);
  Event_ShowMessageAndWait(id, 0);
  Actor_SetAttachedEffect(owner, 0x100);
  Func_02000d80();
  Func_0200108c();
}

void Func_02000e28(s32 id) {
  s32 owner = gMainRoutineWork.currentOwner;

  Func_02001084();
  Event_ResolvePendingActions(0);
  Actor_Stop(id);
  Actor_ShowEmote(id, 0x100, 30);
  Actor_SetAngleToward(id, owner, 20);
  Event_SetMessageId(MsgGrrrrrRrrruffRrufff);
  Event_ShowMessageAndWait(id, 0);
  Func_080d489c(owner, 0x101);
  Actor_SetModeById(id, 4);
  Event_Wait(30);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAttachedEffect(owner, 0x100);
  Func_02000d80();
  Func_0200108c();
}

void Func_02000ea8(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *guard = Actor_GetById(14);

  if ((guard->z - owner->z >= 0 ? guard->z - owner->z : owner->z - guard->z) <
          0x100000 &&
      guard->x > owner->x)
    return;
  Func_02000da8(10);
}

void Func_02000ef4(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *guard = Actor_GetById(15);

  if ((guard->z - owner->z >= 0 ? guard->z - owner->z : owner->z - guard->z) <
          0x100000 &&
      guard->x < owner->x)
    return;
  Func_02000da8(10);
}
