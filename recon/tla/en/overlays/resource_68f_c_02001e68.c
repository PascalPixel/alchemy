// Near miss, differ 272 of 246 (tla-en, 2026-10-31): calls, branches, loops
// and fields are the native's; what remains is allocation. GCSE's PRE hoists
// the position array's address (sp+20) and &object->terrainId above the retry
// label onto the entry edge and replaces the native's recomputations (add r6,
// sp, #20 in the retry block, add r7 and add r2, sp, #20 in the walk loop)
// with copies of one register: the address holds r8 throughout, &terrainId
// takes sp+4 instead of r11, targetZ sp+8 and targetX a register where the
// native keeps them at sp+4 and sp+8, the overlap loop's counter goes to r4
// with caller saves instead of r9, and radius to r9 instead of sp+0; angle and
// startCell keep the native's sp+16 and sp+12. Compiled with -fno-gcse
// (analysis only) it keeps the native's recomputations and slots, and object
// and the position pointer swap r5 and r6.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void Func_020017dc(void);
void Func_02001868(void);
s32 Func_02002fe0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002fb0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002fb8(struct Actor *object);
s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001e68(void) {
  struct Actor *object;
  s16 angle;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 radius;
  s32 position[3];
  s32 *p;
  s32 x;
  s32 z;
  u8 *layer;
  s32 cell;
  s32 *q;
  struct Actor *entry;
  u8 *flags;
  s32 j;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  angle = (object->angle + 0x2000) & 0xc000;
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
  targetX = x;
  targetZ = z;
  layer = &object->terrainId;
  startCell = Func_02002fe0(*layer, x, z);
  Vector_AddPolarOffsetFar(0x100000, angle, p);
  cell = Func_02002fe0(*layer, p[0], p[2]);
  if (cell != 255 &&
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y <= 0x80000) {
    p[0] = targetX;
    p[2] = targetZ;
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    object->action = 0;
    Func_02002f80(object, 2);
    Actor_SetAnimationSpeed(object, 48);
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
