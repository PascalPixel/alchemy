// Differ 4 of 370 (tla-en, 2026-11-07).
// 2026-11-07: a pass-through wrapper on the first turn's first draw (8 to 4);
// left: the ldrsh of the angle scheduled after the square, not before.
// Twin of The Broken Seal's ActorScript_WanderNearHome (SC_WANDER_HOME): the
// same operands, probes, retries, home check and flag bit, on this game's
// headers. Every probe now copies x, y, z in the twin's order (the z-first
// probes were what held this at differ 30). Also tried
// (2026-11-07): angle at every earlier statement, attempts moved, once-only
// blocks, volatile angle reads, a separate unsquared limit (drops the
// dead spill store, 9), declaration orders; 15 minutes of permuting.
// Tried: naming the first draw in a local (s32 spread) gives differ 10, but
// that is a forced temporary, so it is not kept; turn built by statements
// (differ 30 to 115), turn at function scope (56), angle before the division
// (24), tries before the square (22 to 28), dx in two statements (67), dz
// before dx (121), an s32 angle (14, no gain).
// The angle is an s16 (ldrsh, and asrs after ArcTan2) widened to u16 in a
// separate s32 turn: as one u16 expression the inner widening folds away
// (differ 140); ArcTan2's result is cast to u16 before 0x8000 is added (as an
// s16 sum the constant loads from the pool, differ 73).
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

static inline s32 PassThrough(s32 value) { return value; }

s32 Func_08025830(struct ActorScriptView *object) {
  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  s32 travelDistance;
  u16 heading;
  s16 angle;
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
      {
        /* FAKEMATCH(wrapper): the first draw passed through PassThrough; as
         * (Random16() >> 2), 8 of 370 instructions differ in tla-en. */
        s32 turn =
            (u16)angle + PassThrough(Random16() >> 2) - (Random16() >> 2);

        heading = turn;
      }
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
    {
      s32 turn = (u16)angle + (Random16() >> 2) - (Random16() >> 2);

      heading = turn;
    }
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
