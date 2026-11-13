/// @file
/// Scene code of overlay 0x690, in ROM order.

// ---- games/tla/src/scene/settlements/s1620.c ----
#include "types.h"
#include "field_effect.h"
#include "actor.h"
#include "event_inline.h"
#include "../shared/settlements.h"
#include "field.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "module/scene/settlements.h"
#include "module/system/system.h"
#include "module/tla.h"

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02001620(s32 objectId) {
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_Get(objectId);
  params.update = Effect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    vec.x = Math_Cosine(angle);
    vec.y = 0;
    z = Math_Sin(angle);
    x = vec.x;
    vec.z = z;
    x += x / 3;
    vec.x = x;
    Func_020000b8(object->x, object->y, object->z, x, vec.y, z, 0x01000001,
                  &params);
  }
}

void Func_020016a4(void) { GameFlag_SetBit(0x208); }

/* VIEW(s1620): variable gInput as extern u32 gInput; input.h has extern struct InputState gInput; tried as the 3 declarations' extern u32 gInput, it does not compile in tla-ja: games/tla/include/system/input.h:37: conflicting types for `gInput'. */
extern u32 gInput__s1620 __asm__("gInput");
extern const s16 gData_02002276[];

void Func_020016b0(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->timer != 0) {
    object->timer--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(0x106))
    angle = -1;
  else
    angle = gData_02002276[(gInput__s1620 >> 4) & 15];
  if (angle == -1) {
    Func_02001fe0(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Func_02001fe0(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

// ---- games/tla/src/scene/settlements/s173c.c ----
#include "event_work.h"
#include "call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"

void Func_0200173c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Flag_Set(0x106);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(0x106);
  }
}

/* VIEW(s173c): routine Func_020016b0 as void Func_020016b0(void); the definition has void Func_020016b0(struct Actor *object); tried as the definition's void Func_020016b0(void), it does not compile in tla-ja: games/tla/src/scene/settlements.c:96: conflicting types for `Func_020016b0'. */
extern void Func_020016b0__s173c(void) __asm__("Func_020016b0");

void Func_02001770(void) {
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
  Func_0200173c();
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
  startCell = MapGrid_GetCellBehaviorAt(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020022b6[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse's hoist of the
   * position's address; as no asm, 2 of 304 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  FieldVector_AddPolarOffset(0x100000, angle, position);
  cell = MapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
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
  object->timer = 0;
  FieldActor_SetMoveTarget(object, x, object->y, z);
  Func_02001fe0(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Actor_WaitForMove(object);
  object->update = Func_020016b0__s173c;
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
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius, (s32)position,
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
  FieldActor_SetMoveTarget(object, q[0], q[1], q[2]);
  Actor_WaitForMove(object);
  if (cell != startCell)
    goto moved;
walk:
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  FieldActor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  System_Wait(2);
  goto retry;
moved:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02001fe0(object, 1);
}
