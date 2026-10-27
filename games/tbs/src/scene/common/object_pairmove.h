#include "unit/field/field.h"
#include "types.h"
#include "actor.h"
#include "save_block.h"

// Imil and Fuchin Falls, and Vale's cave.

/// Moves the character the player controls (in animation 27) and actor
/// id together by (dx, dz) pixels to the centres of their cells, at a pixel a
/// frame, as when pushing it; the actor faces the way it goes.
static void Actor_PlacePairAtOffset(s32 id, s32 dx, s32 dz) {
  struct Actor *actor;
  struct Actor *other;
  s32 x;
  s32 z;

  actor = Actor_GetById(gSaveBlock.leaderActorId);
  other = Actor_GetById(id);
  FieldScript_Start();
  {
    x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
    z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

    actor->speedLimit = 0x10000;
    actor->acceleration = 0x8000;
    FieldActor_SetMoveTarget(actor, x, actor->y.fixed, z);
  }
  Actor_SelectAnimation(actor, 27);
  {
    x = ((other->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
    z = ((other->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

    other->speedLimit = 0x10000;
    other->acceleration = 0x8000;
    FieldActor_SetMoveTarget(other, x, other->y.fixed, z);
  }
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(other, 4);
  } else {
    Actor_SelectAnimation(other, 3);
  }
  Actor_WaitForMove(actor);
  FieldScript_End();
}
