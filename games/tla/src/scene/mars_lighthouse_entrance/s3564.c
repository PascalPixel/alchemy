#include "edition.h"

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

extern volatile s16 gData_020068b0;
extern s16 gData_02005798[];
void Func_020031f8(struct Actor *object);
void Func_02003284(void);
s32 Func_02004c10(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_02004bf8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02004bc0(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02004c00(struct Actor *object);
s32 Func_02004c70(struct Actor *object);
s32 Func_02004c78(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02003564(void) {
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

  /* FAKEMATCH(volatile): gData_020068b0 read as a volatile s16, reread after
   * each test and for lsls #16; as a plain u16, 54 of 347 instructions differ
   * in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_020068b0 = 0;
retry:
  Func_02003284();
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
  startCell = Func_02004c10(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_02005798[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
#if (LANGUAGE == LANGUAGE_JAPANESE)
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
  cell = Func_02004c10(*layer, p[0], p[2]);
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
  Func_02004bf8(object, x, object->y, z);
  Func_02004bc0(object, 2);
  Actor_SetAnimationSpeed(object, 48);
#if (LANGUAGE == LANGUAGE_JAPANESE)
  Func_02004c00(object);
#else
  for (i = 0; i <= 179 && Func_02004c70(object) == 0; i++) {
    if (gData_020068b0 != 0)
      goto end;
    wait(1);
  }
#endif
  object->update = (void (*)(void))Func_020031f8;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gData_020068b0 != 0)
#if (LANGUAGE == LANGUAGE_JAPANESE)
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
    if (Value4((s32 (*)())Func_02004c78, (s32)&entry->x, radius, (s32)position,
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
  Func_02004bf8(object, q[0], q[1], q[2]);
  i = 0;
  goto test;
next:
  wait(1);
  i++;
test:
  if (i <= 179 && Func_02004c70(object) == 0 && gData_020068b0 == 0)
    goto next;
  if (gData_020068b0 != 0)
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
  cell = Func_02004c10(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02004bf8(object, targetX, object->y, targetZ);
  Func_02004c00(object);
  wait(2);
  goto retry;
changed:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02004bc0(object, 1);
}
