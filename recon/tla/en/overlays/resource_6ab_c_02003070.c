// Draft, differ 8 of 347 (tla-en, 5 November 2026).
// Func_02003390's relative: walks the current owner one 16-pixel cell at a time
// along its input angle (or the quarter turn toward its cell's middle), stopping
// at the value 255 from Func_02004224, a rise over 8 pixels, a solid object or a
// set gData_02004cf0.
// Left: position's address. The listing rebuilds sp+28 as the argument of the last
// two calls and sets q after the call; here cse2 keeps one pseudo across it.
// The flag is a volatile s16: ldrh, lsls, cmp at each poll (5 sites); as a plain u16,
// 42 more instructions differ.
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
extern volatile s16 gData_02004cf0;

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
    for (j = 0; j <= 63; j++, other++) {
      if (other->script != NULL && (other->collisionFlags & 1) &&
          other != object &&
          Func_0200428c(&other->x, radius, position, other->radius - 2) >= 0)
        goto blocked;
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
