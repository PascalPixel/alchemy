#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Event_StartScreenTransition(void);
void Func_02003398(void);
void Func_020033a0(void);

static __inline__ void Func_020033e0_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_02001d68(void) {
  struct Actor *object = Actor_GetById(9);

  Func_02003398();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(9, 2);
  object->y = -0x400000;
  Func_020033e0_(9, PIXELS(728), PIXELS(120));
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         PIXELS(728), PIXELS(120));
  Actor_GetById(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Actor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  Script_WaitForEventTimeout(object);
  GameFlag_SetBit(0x300);
  Func_020033a0();
}

void Func_02001df4(void) {
  struct Actor *object = Actor_GetById(11);

  Func_02003398();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  object->y = -0x400000;
  Actor_GetById(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Actor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  Script_WaitForEventTimeout(object);
  GameFlag_SetBit(0x304);
  Func_020033a0();
}
