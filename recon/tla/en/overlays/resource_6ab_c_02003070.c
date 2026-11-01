// Draft, differ 254 of 347, skeleton gap 104 (tla-en, 2026-11-01)
// Func_02003390's relative: walks the current owner one 16-pixel cell at a time
// along its input angle (or the quarter turn toward its cell's middle), stopping
// at the value 255 from Func_02004224, a rise over 8 pixels, a solid object or a
// set gData_02004cf0. Control flow, calls, loads and widths are the listing's:
// the else ending in goto puts the first wait loop after it, and the do with an
// entry jump gives the walk's rotated test.
// Left, in order of size:
// - The listing builds &position and &object->terrainId after the retry label,
//   and re-adds sp+28 at each use (r10, then r6, then r2 per argument). Here
//   gcse's PRE hoists both onto the entry edge; the pseudo takes r8 and moves
//   the whole allocation (listing: object r7, angle r9, value r11, flags r8,
//   p and j r10, q r6, radius spilled). No plain C stops it (see Func_02003390).
// - IwramMulQ16 is in r8 in the listing. Here its pseudo counts 6 refs (3 at
//   loop depth 1), so 4 * 1 call < refs makes caller-save profitable: r3 and a
//   stack slot. Only refs without the depth weight would give r8.
// Both are the compiler question in the PRE notes of other drafts (-O2 here
// looks unlike the listing: no hoist, no depth weight); K1 is Pascal's, not
// tried. Measured to scope it, none kept, since none reaches differ 0:
// - volatile s16 gData_02004cf0, read through a set-once pointer: differ 237,
//   gap 91. It gives the listing's ldrh, lsls and the reread at the loop's end,
//   and the set-once pointer rebuilds the address. Only this function reads it,
//   so under S2 it is a tagged device; here `<< 16` keeps the shift instead.
// - An empty asm clobbering "sfp" before each &position use, and one marking
//   object rewritten at the retry label, stop the hoist outright: with the
//   volatile, differ 199, gap 67. angle and value stay swapped and r8 stays
//   wrong, so they went back out.
// Tried without gain: set-once &position (251), one position pointer (248),
// no pointers (237 with the volatile, identical code), inner scan scope, one
// counter for both loops (289), other[j] (264), x and z declared first (249).
#include "types.h"
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
// Spelled with the shift: this keeps the listing's lsls.
#define FLAG_02004CF0_SET ((gData_02004cf0 << 16) != 0)

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
  u8 *flags;

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
  dx = lastX - object->x;
  dz = lastZ - object->z;
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
  Vector_AddPolarOffsetFar(0x100000, angle, position);
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
    if (FLAG_02004CF0_SET)
      goto end;
    wait(1);
    i++;
  }
  object->update = (void (*)(void))Func_02002d04;
  goto next;
  do {
    if (Map_GetTerrainHeight(*terrain, q[0], q[2]) - object->y > 0x80000)
      goto blocked;
    if (FLAG_02004CF0_SET)
      goto end;
    radius = object->radius - 2;
    other = Ram_HeapSlots->scriptObjects;
    flags = &other->collisionFlags;
    for (j = 0; j <= 63; j++) {
      if (other->script != NULL && (*flags & 1) && other != object &&
          Func_0200428c(&other->x, radius, position, other->radius - 2) >= 0)
        goto blocked;
      flags += sizeof(struct Actor);
      other++;
    }
    lastX = q[0];
    lastZ = q[2];
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    i = 0;
    Func_02004204(object, q[0], q[1], q[2]);
    while (i <= 179 && Func_02004284(object) == 0 && !FLAG_02004CF0_SET) {
      wait(1);
      i++;
    }
    if (FLAG_02004CF0_SET)
      goto end;
    if (value != startValue)
      goto stop;
  next:
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    q = position;
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
