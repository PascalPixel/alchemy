/// @file
/// Scene code of overlay 0x6a5, in ROM order, from s40f4's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "event_inline.h"
#include "iwram_call.h"

void Func_02005814(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200581c(void);

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

struct EventWork {
  u8 unk000[0x154];
  u16 unk154;
  u8 unk156[0xd44 - 0x156];
  u16 unkD44;
  u16 unkD46;
};

void Func_020040f4(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 8 of 216 instructions differ in
   * tla-en. */
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Func_02005814();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkD46 = i;

  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);

  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkD44 = 1;
  work->unkD46 = 1;
  angle = linked->angle;
  actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(shadow, actor->x,
                      Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y > actor->terrainHeight + 0x100000);
  }
  Event_PlaySound(127);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->velocityY != 0);
  }
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkD46 = 1;
  work->unk154 = 0;
  Func_0200581c();
}

s32 Func_020042e0(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

/// The object this update drives: an Actor with its own bytes over the callback
/// word.
struct Object6a5 {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  u8 unk34[0x1c];
  s8 *sprite;
  u8 unk54[0x0e];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(void);
};

/// The scene's event work, as far as this update reads it.
struct EventWork6a5 {
  u8 unk000[346];
  s16 unk15a;
  u8 unk15c[2];
  s16 unk15e;
  u8 unk160[6];
  s16 unk166;
  s16 unk168;
  s16 unk16a;
};

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

s32 Func_020042e0(s32 *a, s32 *b);
void Func_02005764(struct Object6a5 *object, const void *script);
void Func_02005994(struct Vector3 *position);
s32 Func_020059e4();
s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct Object6a5 *object, s32 value);
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));
extern u8 gData_020069fc[];
extern s32 Data_02006a28;

void Func_0200430c(struct Object6a5 *object) {
  struct EventWork6a5 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Vector3 position;
  s32 distance;
  s32 near;
  s32 timer;
  u8 *flags;

  if (work->unk166 != 0 || work->unk15a != 0 || work->unk15e != 0 ||
      work->unk168 != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  timer = object->unk64 + 1;
  object->unk64 = timer;
  if ((timer & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *from = leader->animation;
    s8 *to = object->sprite;
    s32 priority = from[9] & 12;
    to[9] = (to[9] & ~12) | priority;
  }
  if (Func_020059e4(object->x, object->z, 2) != 255) {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Func_02005994(&position);
    if (position.x <= 0x1100000) {
      if (position.x >= -0x200000) {
      if (object->unk62 == 0) {
        distance = leader->y - object->y;
        if (distance < 0)
          distance = object->y - leader->y;
        near = 0;
        if (Func_020042e0(&leader->x, &object->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near != 0 && Event_GetFlag(0x104) == 0) {
          leader->watchedObject = (struct Actor *)object;
          leader->angle = object->angle;
          flags = &leader->priorityFlags;
          *flags &= ~1;
          work->unk16a = 200;
          gMainRoutineWork.renderMode = 0;
          object->unk62 = 1;
          object->unk63 = 1;
        }
      }
      object->x += IwramMulQ16(Math_Cosine(object->angle), object->speed);
      object->z += IwramMulQ16(Math_Sin(object->angle), object->speed);
      return;
      }
    }
  }
  object->unk63 = 0;
  object->update = 0;
  Func_02005764(object, gData_020069fc);
  Event_PlaySound(228);
  Data_02006a28 = 0;
}
