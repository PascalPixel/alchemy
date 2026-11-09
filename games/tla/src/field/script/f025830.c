/// @file
/// The script command that wanders near home.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "actor_script_view.h"
#include "iwram_call.h"

struct ScriptObjectEntry;

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *position);
s32 Func_0802d87c(struct Actor *object, struct FieldPosition *position);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
s32 ArcTan2(s32 y, s32 x);

/// Moves the object to a random spot within its home radius, as The Broken
/// Seal's ActorScript_WanderNearHome does: up to seven tries at a point a
/// random distance away in a random turn of its heading, each refused if
/// anything overlaps it or blocks the path or the cells beside it. An object
/// outside its home radius (or out of tries) heads back toward home instead.
/// Operands: the minimum distance, the distance range and the
/// home radius.
s32 Func_08025830(struct ActorScriptView *object) {
  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  s32 travelDistance;
  u16 heading;
  u16 angle;
  s32 attempts;
  s32 dx;
  s32 dz;

  operands = &object->script[(s16)object->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / 0x10000;
  angle = object->scriptValue;
  homeRadiusSquared *= homeRadiusSquared;
  attempts = 0;
  dx = object->x / 0x10000 - object->homeX;
  dz = object->z / 0x10000 - object->homeZ;
  if (dx * dx + dz * dz <= homeRadiusSquared) {
  roam:
    attempts++;
    if (attempts <= 7) {
      travelDistance =
          minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                    (s32 *)&candidatePosition) != 0)
        goto roam;
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (Func_0802d87c((struct Actor *)object, &candidatePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      travelDistance += 0x80000;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x2000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x2000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x4000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x4000, &probePosition);
      if (Func_0802d87c((struct Actor *)object, &probePosition) != 0)
        goto roam;
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
      if (dx * dx + dz * dz > homeRadiusSquared)
        goto roam;
      object->flags59 |= 2;
      Actor_SetMoveTarget((struct Actor *)object, candidatePosition.x,
                          candidatePosition.y, candidatePosition.z);
      goto done;
    }
  }
  attempts = 0;
  angle = (u16)ArcTan2(dz, dx) + 0x8000;
back:
  attempts++;
  if (attempts <= 7) {
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0)
      goto back;
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (Func_0802d87c((struct Actor *)object, &candidatePosition) != 0)
      goto back;
    object->flags59 &= ~2;
    Actor_SetMoveTarget((struct Actor *)object, candidatePosition.x,
                        candidatePosition.y, candidatePosition.z);
  }
done:
  object->scriptCursor += 4;
  return 1;
}
