#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Event_StartScreenTransition(void);
void Func_02003398(void);
void Func_020033a0(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020033e0_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_02001d68(void) {
  struct Actor *object = Actor_Get(9);

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(9, 2);
  object->y = -0x400000;
  Func_020033e0_(9, PIXELS(728), PIXELS(120));
  Actor_SetPosition(gMainRoutineWork.currentOwner,
                                         PIXELS(728), PIXELS(120));
  Actor_Get(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  Actor_WaitForMove(object);
  Event_SetFlag(0x300);
  Func_020033a0();
}

void Func_02001df4(void) {
  struct Actor *object = Actor_Get(11);

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  object->motionFlags = 0;
  object->y = -0x400000;
  Actor_Get(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  Actor_WaitForMove(object);
  Event_SetFlag(0x304);
  Func_020033a0();
}
