/// @file
/// Scene code of overlay 0x67b, in ROM order, from exits's offset up to the next object the overlay links.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "iwram_call.h"
#include "system/vector3.h"
#include "event_inline.h"

const u32 *Scene_GetExits(void) { return gSceneExits; }

u32 Random16Far(void);

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
  Actor_SetMoveTarget(object, p->x, coordinate, p->z);
end:
  /* FAKEMATCH(temporary): the form kept; as return 0, 22 of 107 instructions
   * differ in tla-en. */
  x = 0;
  return x;
}
