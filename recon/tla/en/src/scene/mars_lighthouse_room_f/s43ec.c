#include "version.h"

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

extern volatile s16 gData_020074c8;
extern s16 gData_02005f38[];
void Func_02004080(struct Actor *object);
void Func_0200410c(void);
s32 Func_020054d0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_020054b8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02005480(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_020054c0(struct Actor *object);
s32 Func_02005530(struct Actor *object);
s32 Func_02005538(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_020043ec(void) {
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
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;
  s32 *p;

  /* FAKEMATCH(volatile): gData_020074c8 read as a volatile s16, reread after
   * each test and for lsls #16; as a plain u16, 54 of 347 instructions differ
   * in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_020074c8 = 0;
retry:
  Func_0200410c();
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
  startCell = Func_020054d0(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_02005f38[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
#if !TLA_PATCH_1
      return;
#else
      goto end;
#endif
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 2
   * of 347 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  cell = Func_020054d0(*layer, p[0], p[2]);
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
  object->acceleration = SPLIT_CONSTANT(0x1999);
  object->action = 0;
  Func_020054b8(object, x, object->y, z);
  Func_02005480(object, 2);
  Actor_SetAnimationSpeed(object, 48);
#if !TLA_PATCH_1
  Func_020054c0(object);
#else
  for (i = 0; i <= 179 && Func_02005530(object) == 0; i++) {
    if (gData_020074c8 != 0)
      goto end;
    wait(1);
  }
#endif
  object->update = (void (*)(void))Func_02004080;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gData_020074c8 != 0)
#if !TLA_PATCH_1
    goto blocked;
#else
    goto end;
#endif
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 347 instructions differ in tla-en. */
    if (Value4((s32 (*)())Func_02005538, (s32)&entry->x, radius, (s32)position,
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
  object->acceleration = SPLIT_CONSTANT(0x1999);
  Func_020054b8(object, q[0], q[1], q[2]);
  i = 0;
  goto test;
next:
  wait(1);
  i++;
test:
  if (i <= 179 && Func_02005530(object) == 0 && gData_020074c8 == 0)
    goto next;
  if (gData_020074c8 != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 94 of 347 instructions differ in tla-en. */
  __asm__("" : : : "sfp");
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 6
   * of 347 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  q = position;
  cell = Func_020054d0(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_020054b8(object, targetX, object->y, targetZ);
  Func_020054c0(object);
  wait(2);
  goto retry;
changed:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02005480(object, 1);
}
