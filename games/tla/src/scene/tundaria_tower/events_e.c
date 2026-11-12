#include "tundaria_tower.h"
#include "call.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"

void Func_02001868(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Call1((void (*)())GameFlag_SetBit, 0x106);
    Func_02003100();
    Func_02003140();
    GameFlag_ClearBit(0x106);
  }
}

extern s16 gData_020032b2[];
void Func_020017dc(void);
void Func_02001868(void);
s32 GetMapCellCollision(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Script_WaitForEventTimeout(struct Actor *object);
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
  startCell = GetMapCellCollision(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse's hoist of the
   * position's address; as no asm, 2 of 304 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  cell = GetMapCellCollision(*layer, p[0], p[2]);
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
  Actor_SetMoveTarget(object, x, object->y, z);
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Script_WaitForEventTimeout(object);
  object->update = Func_020017dc;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    if (Value4((s32 (*)())Func_02003028, (s32)&entry->x, radius, (s32)position,
               entry->radius - 2) >= 0)
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
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  Script_WaitForEventTimeout(object);
  if (cell != startCell)
    goto moved;
walk:
  /* FAKEMATCH(barrier): as the one above; as no asm, 126 of 304 instructions
   * differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  q = position;
  cell = GetMapCellCollision(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Script_WaitForEventTimeout(object);
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
