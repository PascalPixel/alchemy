// Draft, differ 269 of 347, skeleton gap 97 (tla-en, 2026-11-01)
// Calls, branches, loops and fields are the native's. Left:
// - GCSE's PRE (edge 0 to 1 in the gcse dump) hoists &position and
//   &object->terrainId above the retry label and makes every later &position a
//   copy of one register, where the native recomputes them (p in r10, q in r6,
//   add sp, #28 at each call). No plain C here stops it: the address is
//   transparent and anticipated at the label, and the same happens in
//   Func_0200189c and Func_02001e68. This costs most of the gap.
// - Natural-loop reference weights make the overlap counter's caller save
//   profitable: it takes r4 with a save round the call, where the native has
//   r10 and radius at sp+0. goto loops change nothing; the weights come from
//   the cfg. gcse.c and flow.c take both behaviours, classic GCSE and unweighted
//   refs, from optimize_size: an untested compiler lead, not this draft's to try.
// - The native rereads gData_02003cd0 after each test (ldrh, lsls #16, cmp #0).
//   Only `volatile s16` gives that (gap 88, differ 247), and it is read in this
//   function alone, so it would be a tagged device (S2); plain s16 is worse (111).
// - Summing the squares into radius (one local for two values, a tagged
//   temporary) gives gap 74, differ 224: layer goes to sp+4 and the counter to a
//   high register. With the volatile, gap 68, differ 220. A separate local gives
//   no gain. A 0 default on radius, an earlier read of it, inner scopes, forty
//   declaration orders and direct polar calls (at either site) gain nothing.
// The refresh wait loop is written with labels: as a for, loop.c hoists the
// flag's address (gap 100).
// Japanese is an older layout: no first wait loop; Func_02002fb8 and the
// update hook come before the walk.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

extern u16 gData_02003cd0;
extern s16 gData_020032b2[];
void Func_020017dc(struct Actor *object);
void Func_02001868(void);
s32 Func_02002fe0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_02002fb0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002fb8(struct Actor *object);
s32 Func_02003018(struct Actor *object);
s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001b48(void) {
  struct Actor *object;
  s32 radius;
  u8 *layer;
  s32 z;
  s32 x;
  s32 targetZ;
  s32 targetX;
  s32 startCell;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *p;
  s32 *q;

  /* FAKEMATCH(wrapper): the polar steps go through Call3; as direct calls,
   * 276 of 347 instructions differ in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_02003cd0 = 0;
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = Func_02002fe0(*layer, targetX, targetZ);
  dx = targetX - object->x;
  dz = targetZ - object->z;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword (as angle == -1, 268).
    if ((u16)angle == 0xffff)
      goto end;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  cell = Func_02002fe0(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->action = 0;
  Func_02002fb0(object, x, object->y, z);
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  for (i = 0; i <= 179 && Func_02003018(object) == 0; i++) {
    if (gData_02003cd0 != 0)
      goto end;
    wait(1);
  }
  object->update = (void (*)(void))Func_020017dc;
  for (;;) {
    Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
    q = position;
    cell = Func_02002fe0(*layer, q[0], q[2]);
    if (cell == 255)
      break;
    if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
      goto blocked;
    if (gData_02003cd0 != 0)
      goto end;
    radius = object->radius - 2;
    entry = Ram_HeapSlots->scriptObjects;
    flags = &entry->collisionFlags;
    for (j = 0; j < 64; j++, entry++, flags += sizeof(*entry)) {
      if (entry->script != NULL && (1 & *flags) && entry != object) {
        if (Func_02003028(&entry->x, radius, position, entry->radius - 2) >= 0)
          goto blocked;
      }
    }
    targetX = q[0];
    targetZ = q[2];
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    Func_02002fb0(object, q[0], q[1], q[2]);
    i = 0;
    goto test;
  next:
    wait(1);
    i++;
  test:
    if (i <= 179 && Func_02003018(object) == 0 && gData_02003cd0 == 0)
      goto next;
    if (gData_02003cd0 != 0)
      goto end;
    if (cell != startCell) {
      wait(10);
      goto end;
    }
  }
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02002fb0(object, targetX, object->y, targetZ);
  Func_02002fb8(object);
  wait(2);
  goto retry;
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02002f80(object, 1);
}
