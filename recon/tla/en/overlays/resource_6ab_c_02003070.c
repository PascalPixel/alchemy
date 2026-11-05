// Draft, differ 252 of 347, skeleton gap 50/52 (tla-en, 2026-11-01)
// Func_02003390's relative: walks the current owner one 16-pixel cell at a time
// along its input angle (or the quarter turn toward its cell's middle), stopping
// at the value 255 from Func_02004224, a rise over 8 pixels, a solid object or a
// set gData_02004cf0. Control flow, calls, loads and widths are the listing's:
// the else ending in goto puts the first wait loop after it, and the do with an
// entry jump gives the walk's rotated test.
// Left, one cause and one form. Compiler question, K3's and not this draft's:
// - PRE moves &position and &object->terrainId onto the retry loop's entry edge
//   (both expressions are computed in the loop's first block, so they are
//   available on the back edge whatever the C says). The listing keeps both
//   after the retry label and rebuilds sp+28 at all four other sites (r2 per
//   call argument, r6 in the walk). The hoisted pseudo takes r8; everything
//   else rotates: listing object r7, angle r9, value r11, flags r8, p and j
//   r10, q r6, radius spilled, IwramMulQ16 in r8. Nearly all of the gap and
//   differ follow from this. Func_02002dc4 is the same case, and its note
//   holds what was tried there: entry at the bottom test needs a device.
// - ldrh, lsls #16, cmp: only a volatile s16 keeps the shift (combine merges
//   anything else; s16 gives movs and ldrsh, `<< 16` folds now). Only this
//   function reads the flag, so under S2 that is a device: not kept. Measured
//   to scope it: differ 254, gap 46/52.
// Measured without gain, 1 November 2026, compiler K2 as it stands:
// - The second wait loop as labels and goto keeps its pool load in the body,
//   as the listing does: gap 50/51, but differ 261. Take it once the address
//   is solved.
// - Same code: no p, no q, q = p, one pointer, no terrain, inner radius scope,
//   angle or value declared elsewhere.
// - Worse: p set before the loop (267), set-once multiply pointer (262), a
//   signed 16-bit bit-field for the flag (252, gap 56/60), s16 (gap 56/60).
// - q also set at the walk's top: 259, gap 49/50, redundant, so not kept.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "system/input.h"
s32 ArcTan2(s32, s32);
void Func_02002d90(void);
void Func_020041cc(struct Actor *, s32);
void Func_02004204(struct Actor *, s32, s32, s32);
void Func_0200420c(struct Actor *);
s32 Func_02004224(s32, s32, s32);
s32 Func_02004284(struct Actor *);
s32 Func_0200428c(s32 *, s32, s32 *, s32);
s32 Map_GetTerrainHeight(s32, s32, s32);
void Actor_SetAnimationSpeed(struct Actor *, s32);
void Vector_AddPolarOffsetFar(s32, s32, s32 *);
s32 IwramMulQ16(s32, s32) __attribute__((long_call));
void wait(s32);
void Func_02002d04();
extern s16 gData_0200483c[];
extern u16 gData_02004cf0;

void Func_02003070(void) {
  s32 position[3];
  s32 startValue;
  s32 lastX, lastZ, x, z;
  u8 *terrain;
  s32 radius;
  struct Actor *object;
  s32 *p;
  s32 *q;
  s32 dx, dz, distance, i, value, j;
  s32 angle;
  struct Actor *other;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_02004cf0 = 0;
retry:
  Func_02002d90();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  x = (object->x & 0xfff00000) + 0x80000;
  p[0] = x;
  p[1] = object->y;
  z = (object->z & 0xfff00000) + 0x80000;
  p[2] = z;
  lastX = x;
  lastZ = z;
  terrain = &object->terrainId;
  startValue = Func_02004224(*terrain, lastX, lastZ);
  dz = lastZ - object->z;
  dx = lastX - object->x;
  distance = IwramMulQ16(dx, dx);
  distance += IwramMulQ16(dz, dz);
  if (distance < 0x40000) {
    angle = gData_0200483c[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      goto end;
  } else {
    angle = (s16)ArcTan2(lastZ - object->z, lastX - object->x);
    angle = (s16)((angle + 0x2000) & 0xffffc000);
  }
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, position);
  value = Func_02004224(*terrain, p[0], p[2]);
  if (value != 255 &&
      Map_GetTerrainHeight(*terrain, p[0], p[2]) - object->y <= 0x80000) {
    p[0] = x;
    p[2] = z;
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    object->action = 0;
    Func_02004204(object, x, object->y, z);
    Func_020041cc(object, 2);
    Actor_SetAnimationSpeed(object, 48);
    i = 0;
  } else {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  while (i <= 179 && Func_02004284(object) == 0) {
    if (gData_02004cf0)
      goto end;
    wait(1);
    i++;
  }
  object->update = (void (*)(void))Func_02002d04;
  goto next;
  do {
    if (Map_GetTerrainHeight(*terrain, q[0], q[2]) - object->y > 0x80000)
      goto blocked;
    if (gData_02004cf0)
      goto end;
    radius = object->radius - 2;
    other = Ram_HeapSlots->scriptObjects;
    for (j = 0; j <= 63; j++) {
      if (other->script != NULL && (other->collisionFlags & 1) && other != object &&
          Func_0200428c(&other->x, radius, position, other->radius - 2) >= 0)
        goto blocked;
      other++;
    }
    lastX = q[0];
    lastZ = q[2];
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    i = 0;
    Func_02004204(object, q[0], q[1], q[2]);
    while (i <= 179 && Func_02004284(object) == 0 && gData_02004cf0 == 0) {
      wait(1);
      i++;
    }
    if (gData_02004cf0)
      goto end;
    if (value != startValue)
      goto stop;
  next:
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 60 of 347 instructions differ
     * in tla-en. */
    do {
      q = position;
    } while (0);
    value = Func_02004224(*terrain, q[0], q[2]);
  } while (value != 255);
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02004204(object, lastX, object->y, lastZ);
  Func_0200420c(object);
  wait(2);
  goto retry;
stop:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_020041cc(object, 1);
}
