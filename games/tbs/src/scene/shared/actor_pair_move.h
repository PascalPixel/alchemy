#include "module/field/field.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

// Imil and Fuchin Falls, and Vale's cave.

/// Moves the character the player controls (in animation 27) and actor
/// id together by (dx, dz) pixels to the centres of their cells, at a pixel a
/// frame, as when pushing it; the actor faces the way it goes.
static void Actor_PlacePairAtOffset(s32 id, s32 dx, s32 dz) {
  struct Actor *actor;
  struct Actor *other;
  s32 x;
  s32 z;

  actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  other = Actor_GetById(id);
  Event_Start();
  {
    x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + px(8);
    z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + px(8);

    actor->speedLimit = px(1);
    actor->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(actor, x, actor->y.fixed, z);
  }
  Actor_SelectAnimation(actor, 27);
  {
    x = ((other->x.fixed + (dx << 16)) & 0xFFF00000) + px(8);
    z = ((other->z.fixed + (dz << 16)) & 0xFFF00000) + px(8);

    other->speedLimit = px(1);
    other->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(other, x, other->y.fixed, z);
  }
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(other, 4);
  } else {
    Actor_SelectAnimation(other, 3);
  }
  Actor_WaitForMove(actor);
  Event_End();
}
