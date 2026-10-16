// Draft, differ 4 of 59: the native loads both sprite attribute bytes (leader,
// then own) before it builds the mask 12; here the own byte is loaded after the
// mask. The one-pass block around the spin (as overlay 6ab's barrier) fixes the
// order of the self copy and the gPartyState pool load (differ 6 without it).
// Tried: explicit masks (25), a target sprite local (14), permute 3 minutes.
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

struct TundariaTowerSprite {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
  u8 unk0a[8];
  u16 angle;
};

void Func_02002950(struct ObjectRuntime *object) {
  s32 vx = object->bounce;
  s32 vz;
  struct TundariaTowerSprite *sprite;
  struct ObjectRuntime *leader;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  sprite = object->animation;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  /* FAKEMATCH: the one-pass block stores the Y scale before it addresses the
   * spin (differ 6 without it). */
  do {
    sprite->angle += object->action;
  } while (0);
  leader = Object_GetById(gPartyState.currentOwner);
  ((struct TundariaTowerSprite *)object->animation)->priority =
      ((struct TundariaTowerSprite *)leader->animation)->priority;
}
