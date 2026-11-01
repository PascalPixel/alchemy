// Matches: differ 0 (tla-en, 2026-11-01)
// Picks a spot for a wandering object: up to three times, a point 4 to 12
// pixels ahead within a quarter turn of its angle; the first within 16 pixels
// of its home (the halfwords at 0x64 and 0x66) becomes its target
// (Func_02002930); after three misses it turns round and sets its halfword at
// 0x5e. Returns 0.
// What closed it: a goto loop (a real loop hoists object + 100 and + 102), the
// 12 unused bytes of frame, p set inside the loop, the rounding written as a
// test and add on a local, and rereading p->x and p->z at the call, which PRE
// turns into the copies in r1 and r4. Those copies stay loaded from the rounded
// locals only if cse2 does not swap them, so each local's last use must come
// after its copy's: coordinate is reused for y, and x for the return value.
// The return value is the one device left; retiring it needs another reason
// for x to outlive the read of p->z.
#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "system/vector3.h"

u32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, struct Vector3 *v);
void Func_02002930(struct Actor *object, s32 x, s32 y, s32 z);

struct ActorTail {
  u8 head[0x5e];
  u16 unk5e;
};

s32 Func_02000384(struct Actor *object) {
  struct Vector3 spot;
  struct Vector3 unused; // Unused: gives the stack frame its 12 bytes.
  struct Vector3 *p;
  s32 x, coordinate, tries, maxDistanceSquared, distance, dx, dz;
  u32 a, b;

  tries = 0;
  maxDistanceSquared = 256;
again:
  tries++;
  if (tries <= 3) {
    p = &spot;
    p->x = object->x;
    p->y = object->y;
    p->z = object->z;
    distance = Iwram_MulQ16(Random16Far(), 0x80000) + 0x40000;
    a = Random16Far();
    b = Random16Far();
    Vector_AddPolarOffsetFar(distance, object->angle + (a >> 2) - (b >> 2), p);
    x = p->x;
    if (x < 0)
      x += 0xffff;
    dx = (x >> 16) - object->action;
    coordinate = p->z;
    if (coordinate < 0)
      coordinate += 0xffff;
    dz = (coordinate >> 16) - (s16)object->unk66;
    if (dx * dx + dz * dz > maxDistanceSquared)
      goto again;
    goto found;
  }
  object->angle += 0x8000;
  ((struct ActorTail *)object)->unk5e = 1;
  goto end;
found:
  coordinate = p->y;
  Func_02002930(object, p->x, coordinate, p->z);
end:
  /* FAKEMATCH(temporary): the form kept; as return 0, 22 of 107 instructions differ in tla-en. */
  x = 0;
  return x;
}
