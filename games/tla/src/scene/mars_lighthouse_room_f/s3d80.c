#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"

void Func_02005560(void);
void Func_020054b0(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020035b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, const struct EffectOptions *options);
void Func_02005568(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_02003d80(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;

  Func_02005560();
  FieldEvent_ResolvePendingActions(0);
  CamBounds(-1, -1, -1, 0);
  Func_020054b0();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_020035b8(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_02005568();
}

#include "room.h"

struct WarpEntry {
  s16 x;
  s16 z;
  s16 flag;
  s16 unk6;
};

struct WarpWork {
  s16 unk00;
  s16 positionFlag;
  s16 unk04;
  s16 sourceX;
  s16 sourceZ;
  s16 count;
  s16 current;
  s16 busy;
  u8 unk10[0x10];
  struct WarpEntry entries[1];
};

extern struct WarpWork gData_020070a8;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02003af0(void);
void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry);
void Func_02003be8(struct WarpWork *work, struct WarpEntry *entry);

s32 Func_02003f18(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct WarpWork *work = &gData_020070a8;
  struct WarpEntry *entry = work->entries;
  s32 index;

  index = 0;
  if (index >= work->count)
    goto end;
top:
  if (entry->x == x && entry->z == z) {
    if (!Event_GetFlag(entry->flag)) {
      Func_02003b78(work, entry);
      Event_SetFlag(entry->flag);
      work->current = index;
      goto end;
    }
    if (index == work->current)
      goto end;
    work->current = index;
    Func_02003be8(work, entry);
    GameFlag_SetByte(work->positionFlag, x);
    GameFlag_SetByte(work->positionFlag + 8, z);
    return 1;
  }
  index++;
  entry++;
  if (index < work->count)
    goto top;
end:
  return 0;
}

void Func_02003fc8(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct WarpWork *work = &gData_020070a8;
  s32 x = FieldGameFlag_GetByte(work->positionFlag);
  s32 z = FieldGameFlag_GetByte(work->positionFlag + 8);

  if (x != 0 || z != 0) {
    struct Actor *other;

    GameFlag_SetByte(work->positionFlag, 0);
    GameFlag_SetByte(work->positionFlag + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    other = *(struct Actor **)(event + 460);
    other->x = object->x;
    other->z = object->z;
    Func_020054b0();
    Func_02003d80();
    WORD_AT(Ram_HeapSlots->eventWork, 428) = value;
  }
}

#include "input.h"

extern const s16 gData_02005ef8[];

void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);

void Func_02004080(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->action != 0) {
    object->action--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(0x106))
    angle = -1;
  else
    angle = gData_02005ef8[(gInput.held >> 4) & 15];
  if (angle == -1) {
    Func_02005480(object, 9);
    return;
  }
  turn = (s16)(angle - object->angle);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->angle += turn;
  Func_02005480(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

void Func_02005640(void);

void Func_0200410c(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  if (S16_AT(event, 0xca2)) {
    Flag_Set(0x106);
    Func_02005608();
    Func_02005640();
    Flag_Clear(0x106);
  }
}

#include "fixed_math.h"
#include "scene_tables.h"
#include "system/iwram_call.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

s32 ArcTan2(s32 y, s32 x);
s32 Func_02005538(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);
extern s16 gData_02005f38[];

void Func_02004140(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 2 of 304 instructions differ in
   * tla-en. */
  struct Position probe;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 startX;
  s32 startZ;
  u8 *layer;
  s32 radius;
  struct Actor *actor;
  struct Position *position;
  struct Position *next;
  s32 heading;
  s32 nextBehavior;
  s32 distance;
  struct Actor *other;
  u8 *flags;
  s32 i;
  s32 dx;
  s32 dz;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);

top:
  Func_0200410c();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position = &probe;
  position->x = startX = (actor->x & 0xfff00000) + PIXELS(8);
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + PIXELS(8);
  x = startX;
  z = startZ;
  layer = &actor->terrainId;
  startBehavior = MapGrid_GetCellBehaviorAt(*layer, x, z);
  dz = z - actor->z;
  dx = x - actor->x;
  distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz);
  if (distance < PIXELS(4)) {
    heading = gData_02005f38[(gInput.held >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      return;
    }
  } else {
    heading = (s16)ArcTan2(z - actor->z, x - actor->x);
    heading = (s16)((heading + 0x2000) & -0x4000);
  }
  FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
  nextBehavior = MapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >
          PIXELS(8)) {
    actor->angle = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = startX;
  position->z = startZ;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  actor->action = 0;
  FieldActor_SetMoveTarget(actor, startX, actor->y, startZ);
  Func_02005480(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  Actor_WaitForMove(actor);
  actor->update = Func_02004080;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > PIXELS(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
       * next and actor; as no asm, 85 of 304 instructions differ in tla-en. */
      asm(""
          :
          : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
            "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
          : SFP_CLOBBER, "r4");
      if (Func_02005538(&other->x, radius, &probe, other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTarget(actor, next->x, next->y, next->z);
  Actor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  {
    struct Position *step;

    step = &probe;
    FieldVector_AddPolarOffset(PIXELS(16), heading, step);
    /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address, so
     * next rebuilds it; as no store, 59 of 304 instructions differ in tla-en.
     */
    step = NULL;
  }
  next = &probe;
  nextBehavior = MapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  FieldActor_SetMoveTarget(actor, x, actor->y, z);
  Actor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Func_02005480(actor, 1);
}

#include "version.h"

#include "call.h"

extern volatile s16 gData_020074c8;
s32 Func_02005530(struct Actor *object);

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
  startCell = GetMapCellCollision(*layer, targetX, targetZ);
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
  FieldVector_AddPolarOffset(0x100000, angle, position);
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
  Func_02005480(object, 2);
  Actor_SetAnimationSpeed(object, 48);
#if !TLA_PATCH_1
  Actor_WaitForMove(object);
#else
  for (i = 0; i <= 179 && Func_02005530(object) == 0; i++) {
    if (gData_020074c8 != 0)
      goto end;
    System_Wait(1);
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
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  i = 0;
  goto test;
next:
  System_Wait(1);
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
  __asm__("" : : : SFP_CLOBBER);
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 6
   * of 347 instructions differ in tla-en. */
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = GetMapCellCollision(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  System_Wait(2);
  goto retry;
changed:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02005480(object, 1);
}
