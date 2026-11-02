// Draft (tla-ja)
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

extern volatile s16 gData_02003cd0;
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
  volatile s16 *dataAddress;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;
  volatile s16 *dataCopy;

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
  j = (s32)position;
  ((s32 *)j)[0] = x = (object->x & 0xfff00000) + 0x80000;
  ((s32 *)j)[1] = object->y;
  ((s32 *)j)[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = Func_02002fe0(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  flags = (u8 *)Iwram_MulQ16;
  if (((s32 (*)(s32, s32))flags)(dx, dx) + ((s32 (*)(s32, s32))flags)(dz, dz) <
      0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  cell = Func_02002fe0(*layer, ((s32 *)j)[0], ((s32 *)j)[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, ((s32 *)j)[0], ((s32 *)j)[2]) - object->y >
          0x80000) {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  ((s32 *)j)[0] = x;
  ((s32 *)j)[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->action = 0;
  Func_02002fb0(object, x, object->y, z);
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Func_02002fb8(object);
  object->update = (void (*)(void))Func_020017dc;
  asm("" : "+r"(object) : "g"(radius) : "sfp");
  dataAddress = &gData_02003cd0;
  goto walk;
walkNext:
  asm("" : : "r"(angle), "r"(j) : "sfp");
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  dataCopy = &gData_02003cd0;
  if (*dataCopy != 0)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    if (Value4((s32 (*)())Func_02003028, (s32)&entry->x, radius, (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
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
  dataCopy = dataAddress;
  if (*dataCopy != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  q = position;
  cell = Func_02002fe0(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  do { Func_02002fb0(object, targetX, object->y, targetZ); } while (0);
  Func_02002fb8(object);
  wait(2);
  asm("" : : : "sfp");
  goto retry;
changed:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02002f80(object, 1);
}
