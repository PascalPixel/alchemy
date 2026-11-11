#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern char SceneId_Ov6B0Room4;

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003398(void);
void Func_020033a0(void);

/// Centres the party leader on its cell and raises actor 8 (in Ov6B0Room4) or
/// actor 9 with cue 198, 4 pixels at once and then 128 more, before the screen
/// transition finishes.
void Func_02001518(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object;

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    object = Actor_Get(8);
    FieldEvent_SetExitRequest(13);
  } else {
    object = Actor_Get(9);
    FieldEvent_SetExitRequest(11);
  }
  {
    s32 x = ((leader->x >> 20) << 4) + 8;
    s32 z = ((leader->z >> 20) << 4) + 8;

    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x, z);
  }
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  FieldEvent_Wait(30);
  object->motionFlags = 0;
  /* FAKEMATCH(wrapper): the camera move forwarded through Call4, so each -1 is
   * built on its own, as in the ROM; as a direct call, the three share one
   * register, two instructions shorter, and 12 instructions differ. */
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object->speedLimit = 0x20000;
  object->acceleration = 0x800;
  Event_PlaySound(198);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x40000, object->z);
  Actor_WaitForMove(object);
  Scene_SetActorSpritePriority(leader, 1);
  Scene_SetActorSpritePriority(object, 1);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x800000, object->z);
  FieldEvent_Wait(80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020033a0();
}
