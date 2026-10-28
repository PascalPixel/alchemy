#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_work.h"
#include "actor.h"
#include "objdisp.h"
#include "system.h"

#define Actor_SetAnimationSpeedFar                                 \
  ((void (*)(struct Actor *,                                           \
             s32))Actor_SetAnimationSpeedFar)

void Actor_SetHorizontalPositionWithTerrain(u32, s32, s32);

struct Actor *Actor_GetById(u32 objectId);
void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration);
void Actor_SetMovementTypeById(u32 objectId, s32 action);
void Actor_Stop(u32 objectId);
void Actor_WatchAndSetScript(u32 actorId, u32 watchedActorId);
void Actor_SetMovementTypeAndWaitById(u32 objectId, s32 action);
void Actor_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z);
void Actor_SetPositionAndReset(u32 objectId, s32 x, s32 z);
void Actor_OffsetPositionAndResetMotion(u32 objectId, s32 x_offset,
                                               s32 z_offset);
void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Actor_WaitForMoveById(u32 objectId);
void Actor_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(u32 objectId, s32 action);
void Actor_Jump(u32 objectId, s32 speed, s32 event_id);

void Actor_SetActionById(u32 objectId, s32 action) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL)
    Actor_SetAnimationSpeedFar(object, action);
}
