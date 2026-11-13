/// @file
/// The settlements' events, continued; settlements.c says which places these
/// are.

#include "types.h"
#include "field_effect.h"
#include "actor.h"
#include "event_inline.h"
#include "../settlements.h"
#include "field.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "module/scene/settlements.h"
#include "module/system/system.h"
#include "module/tla.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Settlements_SpawnSplashRing(s32 objectId) {
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_Get(objectId);
  params.update = FieldEffect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    vec.x = Trig_Cos(angle);
    vec.y = 0;
    z = Math_Sin(angle);
    x = vec.x;
    vec.z = z;
    x += x / 3;
    vec.x = x;
    FieldEffect_Spawn(object->x, object->y, object->z, x, vec.y, z, 0x01000001,
                      &params);
  }
}

void Settlements_LiftBoulder(void) { GameFlag_Set(0x208); }

/* VIEW(s1620): variable gInput as extern u32 gInput; input.h has extern struct
 * InputState gInput; tried as the rest of the object declares it, it does not
 * compile in tla-ja: games/tla/src/scene/settlements/settlements.c:66: `gInput'
 * undeclared (first use in this function). */
extern u32 gInput__s1620 __asm__("gInput");
/* VIEW(settlements): variable gSettlements_SlideAngles as const s16
 * gSettlements_SlideAngles[]; scene/settlements/s1d30 has
 * DEFINE_G_DATA_02002276(gSettlements_SlideAngles) s16
 * gSettlements_SlideAnglesByKey[] = {; tried as the one declaration extern
 * const s16 gSettlements_SlideAngles[], it does not compile in tla-ja:
 * games/tla/src/scene/settlements/settlements.c:64: `gSettlements_SlideAngles'
 * undeclared (first use in this function). */
extern const s16
    gSettlements_SlideAngles__settlements[] __asm__("gSettlements_SlideAngles");
#define gSettlements_SlideAngles gSettlements_SlideAngles__settlements

void Settlements_KeepSliding(struct Actor *object) {
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
    angle = gSettlements_SlideAngles[(gInput__s1620 >> 4) & 15];
  if (angle == -1) {
    Actor_SelectAnimation(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

#include "event_work.h"
#include "call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"

void Settlements_StopLeader(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Event_SetFlag(0x106);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(0x106);
  }
}

/* VIEW(s173c): routine Settlements_KeepSliding as void
 * Settlements_KeepSliding(void); the definition has void
 * Settlements_KeepSliding(struct Actor *object); tried as the definition's void
 * Settlements_KeepSliding(struct Actor *object), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/src/scene/settlements/settlements.c:171:
 * `Settlements_KeepSliding__s173c' undeclared (first use in this function). */
extern void
Settlements_KeepSliding__s173c(void) __asm__("Settlements_KeepSliding");

void Settlements_SlideOnIce(void) {
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
  Settlements_StopLeader();
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
  startCell = FieldMapGrid_GetCellBehaviorAt(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gSettlements_SlideAnglesByKey[(gInput.held >> 4) & 15];
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
  cell = FieldMapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
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
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  FieldActor_WaitForMove(object);
  object->update = Settlements_KeepSliding__s173c;
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
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
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
  FieldActor_WaitForMove(object);
  if (cell != startCell)
    goto moved;
walk:
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = FieldMapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  FieldActor_SetMoveTarget(object, targetX, object->y, targetZ);
  FieldActor_WaitForMove(object);
  System_Wait(2);
  goto retry;
moved:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

#undef gSettlements_SlideAngles
// ---- scene/settlements/s1a1c.c ----

/// The settlements' events, continued; settlements.c says which places these
/// are.

#include "version.h"
#include "event_inline.h"

#if TLA_PATCH_1

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

/* VIEW(s1a1c): variable gSettlements_SlideStopped as volatile s16
 * gSettlements_SlideStopped; scene/settlements.h has u16
 * gSettlements_SlideStopped; tried as the 2 declarations' extern volatile s16
 * gSettlements_SlideStopped, it does not compile in tla-en:
 * games/tla/src/scene/settlements/settlements.c:243: conflicting types for
 * `gSettlements_SlideStopped'. */
extern volatile s16
    gSettlements_SlideStopped__s1a1c __asm__("gSettlements_SlideStopped");
extern s16 gSettlements_SlideAnglesByKey[];
void Settlements_KeepSliding(struct Actor *object);
void Settlements_StopLeader(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
s32 Actor_HasArrived(struct Actor *object);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

void Settlements_SlideOnIceFromEdge(void) {
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

  /* FAKEMATCH(volatile): gSettlements_SlideStopped read as a volatile s16,
   * reread after each test and for lsls #16; as a plain u16, 54 of 347
   * instructions differ in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gSettlements_SlideStopped__s1a1c = 0;
retry:
  Settlements_StopLeader();
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
    angle = gSettlements_SlideAnglesByKey[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
      goto end;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
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
  Actor_SetMoveTarget(object, x, object->y, z);
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  for (i = 0; i <= 179 && Actor_HasArrived(object) == 0; i++) {
    if (gSettlements_SlideStopped__s1a1c != 0)
      goto end;
    System_Wait(1);
  }
  object->update = (void (*)(void))Settlements_KeepSliding;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gSettlements_SlideStopped__s1a1c != 0)
    goto end;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 347 instructions differ in tla-en. */
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
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
  i = 0;
  goto test;
next:
  System_Wait(1);
  i++;
test:
  if (i <= 179 && Actor_HasArrived(object) == 0 &&
      gSettlements_SlideStopped__s1a1c == 0)
    goto next;
  if (gSettlements_SlideStopped__s1a1c != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 94 of 347 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  FieldActor_WaitForMove(object);
  System_Wait(2);
  goto retry;
changed:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

#else

// The Japanese release's older form.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

extern volatile s16
    gSettlements_SlideStopped__s1a1c __asm__("gSettlements_SlideStopped");
extern s16 gSettlements_SlideAnglesByKey[];
void Settlements_KeepSliding(struct Actor *object);
void Settlements_StopLeader(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
s32 Actor_HasArrived(struct Actor *object);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

void Settlements_SlideOnIceFromEdge(void) {
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

  // gSettlements_SlideStopped is read as a volatile s16: it is reread after
  // each test.
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gSettlements_SlideStopped__s1a1c = 0;
retry:
  Settlements_StopLeader();
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
    angle = gSettlements_SlideAnglesByKey[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
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
  Actor_SetMoveTarget(object, x, object->y, z);
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  FieldActor_WaitForMove(object);
  object->update = (void (*)(void))Settlements_KeepSliding;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gSettlements_SlideStopped__s1a1c != 0)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 332 instructions differ in tla-ja. */
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
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
  i = 0;
  goto test;
next:
  System_Wait(1);
  i++;
test:
  if (i <= 179 && Actor_HasArrived(object) == 0 &&
      gSettlements_SlideStopped__s1a1c == 0)
    goto next;
  if (gSettlements_SlideStopped__s1a1c != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 90 of 332 instructions differ in tla-ja. */
  __asm__("" : : : SFP_CLOBBER);
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  FieldActor_WaitForMove(object);
  System_Wait(2);
  goto retry;
changed:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

#endif

// ---- scene/settlements/s1d30.c ----

/// The settlements' events, continued; settlements.c says which places these
/// are.

#include "../shared/tables.h"
#include "../settlements.h"
#include "call.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "field.h"
#include "module/field/field.h"
#include "module/scene/settlements.h"
#include "module/tla.h"

void Settlements_SetSlideStopped(s32 value) {
  gSettlements_SlideStopped = value;
}

/* VIEW(s1d30): routine Settlements_KeepSliding as void
 * Settlements_KeepSliding(void); scene/settlements/settlements has void
 * Settlements_KeepSliding(struct Actor *object) {; tried as the definition's
 * void Settlements_KeepSliding(struct Actor *object), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/src/scene/settlements/settlements.c:641:
 * `Settlements_KeepSliding__s1d30' undeclared (first use in this function). */
void Settlements_KeepSliding__s1d30(void) __asm__("Settlements_KeepSliding");

void Settlements_SlideOnIceFacing(void) {
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
  s32 j;
  u8 *flags;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  angle = (object->facing + 0x2000) & 0xc000;
retry:
  Settlements_StopLeader();
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
  startCell = FieldMapGrid_GetCellBehaviorAt(*layer, x, z);
  FieldVector_AddPolarOffset(0x100000, angle, p);
  cell = FieldMapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
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
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  object->update = Settlements_KeepSliding__s1d30;
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
    if (Value4((s32 (*)())Field_CheckSpheresOverlap, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
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
  FieldActor_WaitForMove(object);
  if (cell != startCell)
    goto moved;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 43 of 246 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = FieldMapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  FieldActor_SetMoveTarget(object, targetX, object->y, targetZ);
  FieldActor_WaitForMove(object);
  System_Wait(2);
  goto retry;
moved:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Actor_SelectAnimation(object, 1);
}

#include "scene_tables.h"
#include "text/msg_ids.h"

void Settlements_MeetGel(void);
void Settlements_MeetLostJupiterDjinni(void);
void Settlements_ClearSpriteFlags(void);
void Actor_Bob(void);
void Settlements_WarnOfRiddleOfRock(void);
void Settlements_EnterDoor4(void);
void Settlements_GrowVine(void);
void Settlements_EnterCave(void);
void Settlements_PushIntoFalls(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gSettlements_FallsObjects[] = {
    9, 10, -1,
};

s16 gSettlements_WallCrate[] = {
    10, -1,
};

DEFINE_G_DATA_02002276(gSettlements_SlideAngles)

s16 gSettlements_SlideAnglesByKey[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSettlements_BobScript[] = {
    39,
    3,
    22,
    26,
    0,
    23,
    6,
    -0x100000,
    46,
    (s32)Settlements_ClearSpriteFlags,
    17,
};

u32 gSettlements_SmokeScript[] = {
    39, 2, 17,
};

u32 gSettlements_CrateBobScript[] = {
    22, 26, 0, 17,
};

s32 gSettlements_CrateDropScript[] = {
    46,
    (s32)Actor_Bob,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 216, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 320, 0, 830, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSettlements_Room5Regions[] = {
    0x2e02a0, 0x2b00190, 0x1a0003e, 0xeffff, 0xffff, 0,
    0, 0,
};

u32 gSettlements_Room3Regions[] = {
    0xc00d4, 0xdc0084, 0x8c0014, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(213),
    SCENE_EXIT_IF(1, 2, 48, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 214, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 215, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 215, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(214),
    SCENE_EXIT_IF(1, 213, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(215),
    SCENE_EXIT_IF(1, 213, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 216, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(216),
    SCENE_EXIT_IF(1, 2, 49, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 217, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 217, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 217, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 217, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 218, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(218),
    SCENE_EXIT_IF(2, 216, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(217),
    SCENE_EXIT_IF(11, 216, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(12, 217, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(13, 217, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(14, 216, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(219),
    SCENE_EXIT_IF(1, 2, 50, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 220, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(220),
    SCENE_EXIT_IF(1, 219, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(221),
    SCENE_EXIT_IF(1, 2, 47, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 222, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(222),
    SCENE_EXIT_IF(1, 221, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gSettlements_Placements[] = {
    {56, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(808), FACING_NORTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room1Placements[] = {
    {115, CONDITION_ALWAYS, 2, PIXELS(192), 0, PIXELS(240), FACING_NORTH, 1, 0},
    {106, CONDITION_ALWAYS, 2, PIXELS(336), 0, PIXELS(240), FACING_NORTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room2Placements[] = {
    {116, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(632), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room4Placements[] = {
    {114, CONDITION_ALWAYS, 2, PIXELS(392), 0, PIXELS(280), FACING_NORTH, 1, 0},
    {299, CONDITION_ALWAYS, 7, PIXELS(56), 0, PIXELS(240), FACING_NORTH, 2, 0},
    {287, CONDITION_ALWAYS, (s32)gSettlements_CrateBobScript, PIXELS(280), 0, PIXELS(184),
     FACING_NORTH, 2, 0},
    {245, 0x66, 1, PIXELS(312), 0, PIXELS(248), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room5Placements[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(664), 0, PIXELS(168), FACING_SOUTH, 1, 0},
    {106, CONDITION_ALWAYS, 3, PIXELS(696), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room9Placements[] = {
    {93, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(176), FACING_SOUTH, 1, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(200), FACING_SOUTH, 1, 0},
    {288, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(168), FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(120), 0, PIXELS(72),
     FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(72), 0, PIXELS(104),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(88), 0, PIXELS(104),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(120), 0, PIXELS(104),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(72), 0, PIXELS(136),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(104), 0, PIXELS(136),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(104), 0, PIXELS(152),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(120), 0, PIXELS(184),
     FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(152), 0, PIXELS(184),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, PIXELS(152), 0, PIXELS(152),
     FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 0, 0, 0, FACING_SOUTH, 1, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(92), FACING_SOUTH, 1, 0},
    {246, 0x7f, 1, PIXELS(72), 0, PIXELS(104), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room10Placements[] = {
    {93, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(816), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room7Placements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {244, 0x52, 1, PIXELS(312), 0, PIXELS(296), FACING_WEST, 0, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(88), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room8Placements[] = {
    {75, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(216), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(136), FACING_NORTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSettlements_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 9},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gSettlements_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x4602, 3, CONDITION_ALWAYS, EVENT(Settlements_EnterDoor3)},
    {0x602, 4, CONDITION_ALWAYS, EVENT(Settlements_EnterDoor4)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Settlements_WarnOfRiddleOfRock)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSettlements06)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(Settlements_CutLeftBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(Settlements_CutStairsBush)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room2Triggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements02)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements05)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Settlements_EnterCave)},
    {0x4602, 3, CONDITION_ALWAYS, EVENT(Settlements_EnterDoor3)},
    {0x602, 4, CONDITION_ALWAYS, EVENT(Settlements_EnterDoor4)},
    {0x8602, 5, CONDITION_ALWAYS, EVENT(Settlements_EnterDoor5)},
    {0xc401, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 34, 0x220, EVENT(Settlements_StepOntoWall)},
    {TRIGGER_STEP, 35, 0x1220, EVENT(Settlements_StepOffWall)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements10)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Settlements_MeetTinder)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, EVENT(Settlements_UnblockCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS,
     EVENT(Settlements_BlockCrate)},
    {8, 0, CONDITION_ALWAYS, EVENT(Settlements_UnblockCrate)},
    {9, 0, CONDITION_ALWAYS, EVENT(Settlements_BlockCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 9, 0x230, EVENT(Settlements_GrowVine)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room5Triggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {33, 12, CONDITION_ALWAYS, 12},
    {49, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements08)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSettlements12)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room7Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Settlements_EnterCave)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Settlements_StepOntoIce)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Settlements_StepOffIce)},
    {TRIGGER_STEP, 20, 0x201, EVENT(Settlements_StepOnIceSlide)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x203, EVENT(Settlements_LashRope)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Settlements_MeetGel)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, 0xf5d,
     EVENT(Settlements_CatchApple)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room8Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements13)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSettlements16)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSettlements21))},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room9Triggers[] = {
    {0xc401, 1, CONDITION_ALWAYS, 1},
    {0xc401, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements18)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS,
     EVENT(Settlements_MeetLostJupiterDjinni)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements20)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x204, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x204, EVENT(Settlements_MovePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     0x205, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x205, EVENT(Settlements_MoveCrate)},
    {8, 0, CONDITION_ALWAYS, EVENT(Settlements_BlockFallsObject)},
    {9, 0, CONDITION_ALWAYS, EVENT(Settlements_PushIntoFalls)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 22, 0x208,
     EVENT(Settlements_LiftBoulder)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room10Triggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements19)},
    {TRIGGER_TABLE_END},
};

s32 gSettlements_WaterColorFrame[] = {
    -1,
};
