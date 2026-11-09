#include "types.h"
#include "field.h"
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
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);

s32 Func_08025698(struct ActorScriptView *object) {
  struct FieldPosition pos;
  struct FieldPosition probe;
  s32 base;
  const s32 *args;
  s32 range;
  s32 limit;
  s32 radius;
  s32 heading;
  s32 tries;
  s32 dx;
  s32 dz;

  args = &object->script[(s16)object->scriptCursor + 1];
  base = *args++;
  range = *args++;
  limit = *args / 0x10000;
  // The tries are a loop written with goto: as while (++tries <= 7) with
  // continue, 116 of 188 instructions differ in tla-en.
  tries = 0;
  limit = limit * limit;
retry:
  tries++;
  if (tries <= 7) {
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = object->scriptValue + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(radius, heading, &pos);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&pos) != 0)
      goto retry;
    if (Func_0802d87c((struct Actor *)object, &pos) != 0)
      goto retry;
    radius += 0x80000;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Func_0802d87c((struct Actor *)object, &probe) != 0)
      goto retry;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Func_0802d87c((struct Actor *)object, &probe) != 0)
      goto retry;
    // The offsets in a once-only block keep the division on the loaded x; as
    // plain statements, 16 of 188 instructions differ in tla-en.
    do {
      dx = pos.x / 0x10000 - object->homeX;
      dz = pos.z / 0x10000 - object->homeZ;
    } while (0);
    if (dx * dx + dz * dz > limit)
      goto retry;
    goto found;
  }
  object->scriptValue += 0x8000;
  object->turnedBack = 1;
  return 0;
found:
  {
    /* FAKEMATCH(pin): the object in r0 ahead of the target's words; as a plain
     * argument, 2 of 188 instructions differ in tla-en. */
    register struct Actor *moved asm("r0") = (struct Actor *)object;

    /* FAKEMATCH(barrier): an empty asm after the pin, which holds it ahead; without
     * it, 2 of 188 instructions differ in tla-en. */
    asm("");
    Actor_SetMoveTarget(moved, pos.x, pos.y, pos.z);
  }
  object->scriptCursor += 4;
  return 1;
}
