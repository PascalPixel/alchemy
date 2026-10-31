// Near miss, differ 232 of 304 (tla-en, 2026-10-31): control flow, calls, fields
// and spill slots follow the native. GCSE's PRE hoists the position array's
// address (sp+28) and &object->terrainId above the retry label and copy-
// propagates them, so p, q and layer lose their own registers: the hoisted
// address holds r8 throughout and the rest shift. The native recomputes both
// on every pass, as this draft does under -fno-gcse.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"

extern s16 gData_020032b2[];
void Func_020017dc(void);
void Func_02001868(void);
s32 Func_02002fe0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_02002fb0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002fb8(struct Actor *object);
s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_0200189c(void) {
  struct Actor *object;
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
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *p;
  s32 *q;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
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
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  cell = Func_02002fe0(*layer, p[0], p[2]);
  if (cell != 255 &&
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y <= 0x80000) {
    p[0] = x;
    p[2] = z;
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    object->action = 0;
    Func_02002fb0(object, x, object->y, z);
    Func_02002f80(object, 2);
    Actor_SetAnimationSpeed(object, 48);
    Func_02002fb8(object);
    object->update = Func_020017dc;
  } else {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  while (Vector_AddPolarOffsetFar(0x100000, angle, position), q = position,
         (cell = Func_02002fe0(*layer, q[0], q[2])) != 255) {
    if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
      goto blocked;
    radius = object->radius - 2;
    entry = Ram_HeapSlots->scriptObjects;
    flags = &entry->collisionFlags;
    for (j = 0; j <= 63; j++) {
      if (entry->script != NULL && (1 & *flags) && entry != object) {
        if (Func_02003028(&entry->x, radius, position, entry->radius - 2) >= 0)
          goto blocked;
      }
      flags += sizeof(*entry);
      entry++;
    }
    targetX = q[0];
    targetZ = q[2];
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    Func_02002fb0(object, q[0], q[1], q[2]);
    Func_02002fb8(object);
    if (cell != startCell)
      goto moved;
  }
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02002fb0(object, targetX, object->y, targetZ);
  Func_02002fb8(object);
  wait(2);
  goto retry;
moved:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02002f80(object, 1);
}
