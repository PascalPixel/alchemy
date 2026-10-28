#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern char SceneId_Ov6B0Room4;

struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 value, s32 frames);
void FieldEvent_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003310(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003318(struct Actor *object);
void Func_02003398(void);
void Func_020033a0(void);
void Func_02003450(s32 countdown);
void Func_02003488(s32 a);
void Func_020034b8(s32 cue);

/// Centres the party leader on its cell and raises actor 8 (in Ov6B0Room4) or
/// actor 9 with cue 198, 4 pixels at once and then 128 more, before the screen
/// transition finishes.
void Func_02001518(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object;

  Func_02003398();
  Func_02003488(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    object = Actor_GetById(8);
    Func_02003450(13);
  } else {
    object = Actor_GetById(9);
    Func_02003450(11);
  }
  {
    s32 x = ((leader->x >> 20) << 4) + 8;
    s32 z = ((leader->z >> 20) << 4) + 8;

    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  }
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  FieldEvent_Wait(30);
  object->motionFlags = 0;
  /* FAKEMATCH: through Call4 each -1 is built on its own, as in the native;
   * called directly the three share one register, two instructions shorter
   * (differ 12). */
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  object->speedLimit = 0x20000;
  object->acceleration = 0x800;
  Func_020034b8(198);
  Func_02003310(object, object->x, object->y + 0x40000, object->z);
  Func_02003318(object);
  Scene_SetActorSpritePriority(leader, 1);
  Scene_SetActorSpritePriority(object, 1);
  Func_02003310(object, object->x, object->y + 0x800000, object->z);
  FieldEvent_Wait(80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020033a0();
}
