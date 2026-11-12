/// @file
/// Scene code of overlay 0x671, in ROM order, from find_near's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct Orbiter {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[44];
  void **animation;
  u8 kind;
  u8 unk55[13];
  u8 phase;
  u8 unk63;
  s16 centerX;
  s16 centerZ;
  void *link;
};

extern s32 gData_02005af0;
extern const s8 gData_02006b38[];
extern const u8 gData_02006b20[];
extern const u8 gData_02005fd4[];
extern const u8 gData_02006004[];
extern const u8 gData_02006244[];
extern const u8 gData_020064cc[];
extern const u8 gData_020067e4[];
extern const u8 gData_02006898[];
extern const u8 gData_02006a90[];
s32 Func_02005834(s32 value);
void Func_020020f0(s32 value);

#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

void Func_02000550(void) {
  struct Orbiter *actor = ((struct Orbiter *)Actor_Get(9));
  u16 zero = 0;
  s32 direction;
  s32 i;

  if (Func_02005834(0) == 1) {
    direction = gData_02005af0;
    Func_020020f0(0);
    Event_SetFlag(515);
    /* FAKEMATCH(temporary): the 0 stored from a u16 local holding it, so it is
     * built in a register (movs, strh); as the constant 0 stored directly, 11
     * of 40 instructions differ in tla-en. */
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x166) = zero;
    i = 0;
  loop:
    actor->phase += gData_02006b38[i] * direction;
    System_Wait(1);
    if (++i <= 24)
      goto loop;
  }
}

struct Orbiter *Func_020005b4(struct Orbiter *origin, s32 id) {
  s32 i;

  for (i = 8; i < 64; i++) {
    struct Orbiter *object = ((struct Orbiter *)Actor_Get(i));

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, origin->x, 0x17ffff) &&
          DIFF_WITHIN(object->z, origin->z, 0x17ffff))
        return object;
    }
  }
  return NULL;
}

/// As Func_020005b4, but around the point 16 pixels ahead of origin in its
/// facing.
struct Orbiter *Func_02000624(struct Orbiter *origin, s32 id) {
  s32 facing;
  s32 position[3];
  s32 i;

  position[0] = origin->x;
  position[2] = origin->z;
  /* FAKEMATCH(temporary): the facing read into a local before the call; as
   * origin->facing in the call, 9 of 63 instructions differ in tla-en. */
  facing = origin->facing;
  FieldVector_AddPolarOffset(0x100000, facing, position);
  for (i = 8; i < 64; i++) {
    struct Orbiter *object = ((struct Orbiter *)Actor_Get(i));

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, position[0], 0x17ffff) &&
          DIFF_WITHIN(object->z, position[2], 0x17ffff))
        return object;
    }
  }
  return NULL;
}
#undef DIFF_WITHIN

#include "actor.h"

void Func_020057d4(struct Actor *object);
void Func_02005624(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

void Func_020006ac(struct Actor *object, s32 dx, s32 dz, s32 velocity) {
  u8 *flags = &object->motionFlags;
  u8 saved;

  Func_020057d4(object);
  saved = *flags;
  Func_02005624(object, 6);
  System_Wait(6);
  Event_PlaySound(152);
  Func_02005624(object, 7);
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  object->velocityY = velocity;
  *flags &= 0x7e;
  Actor_SetSingleChildFlags(object, 0);
  FieldActor_SetMoveTarget(object, object->x + dx, object->y, object->z + dz);
  Actor_WaitForMove(object);
  *flags = saved;
  Func_02005624(object, 6);
  Actor_SetSingleChildFlags(object, 1);
  System_Wait(3);
}

void Func_02000740(void) {
  s32 step = gData_02005af0;

  if (step != 0) {
    s32 turn = step * 1092;
    s32 delta = -turn;
    struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *near = Func_020005b4(leader, 358);

    if (near != NULL) {
      u16 angle = near->angle;

      if (angle == 0x8000)
        delta = turn;
    }
    leader->angle += delta;
  }
}

#include "iwram_call.h"
#include "system/vector3.h"

s32 Func_02005684(struct Actor *object, struct Vector3 *position);
s32 Math_Cosine(s32 angle);

void Func_02000784(void) {
  struct Actor *actor, *table;
  struct Vector3 test;
  /* FAKEMATCH(temporary): the step kept in a local set at the top; as
   * gData_02005af0 * -364, 43 of 230 instructions differ in tla-en. */
  s32 step = -364;
  s32 turn, x, z, dx, dz, back, tx, tz;

  if (gData_02005af0 == 0)
    return;
  actor = Actor_GetById(Party_GetLeaderActor());
  table = Func_020005b4(actor, 357);
  turn = gData_02005af0 * step;
  if (table != 0) {
    if (table->y + 0x100000 > actor->y)
      return;
    if (table->angle == 0x8000)
      turn = -turn;
    dx = actor->x - table->x;
    dz = actor->z - table->z;
    back = -turn;
    {
      s32 s = Math_Sin(back);
      s32 c = Math_Cosine(back);
      x = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz);
      z = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz);
    }
    test.x = x + actor->velocityX;
    test.y = table->y;
    test.z = z + actor->velocityZ;
    if (Func_02005684(actor, &test) == 0) {
      s32 s, c, angle;
      angle = 0x1555 - turn;
      s = Math_Sin(angle);
      c = Math_Cosine(angle);
      tx = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz);
      tz = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz);
      test.x = tx + actor->velocityX;
      test.z = tz + actor->velocityZ;
      if (Func_02005684(actor, &test) == 0) {
        s32 s, c, angle;
        angle = back - 0x1555;
        s = Math_Sin(angle);
        c = Math_Cosine(angle);
        tx = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz);
        tz = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz);
        test.x = tx + actor->velocityX;
        test.z = tz + actor->velocityZ;
        if (Func_02005684(actor, &test) == 0) {
          actor->x = x;
          actor->z = z;
        }
      }
    }
  }
  actor->angle += turn;
}

// Moves the controlled character with the platform kind 359 along x by the
// speed in gData_02005af0, backward when the platform faces half a turn; it
// stays within 2 pixels of the platform's z, and past 20 pixels on x it steps
// off (Func_020006ac).
void Func_02000978(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = Func_020005b4(actor, 359);
    s32 dx = *speed << 15;

    if (found != NULL) {
      s32 diff, x, foundX;

      if (found->angle == 0x8000)
        dx = -dx;
      if (found->z - actor->z < -0x20000)
        actor->z = found->z + 0x20000;
      if (found->z - actor->z > 0x20000)
        actor->z = found->z - 0x20000;
      x = actor->x + dx;
      actor->x = x;
      foundX = found->x;
      diff = foundX - x;
      if (diff >= 0 ? diff >= 0x140000 : x - foundX >= 0x140000) {
        s32 sign;
        s32 offset;

        sign = 0;
        offset = actor->x - foundX;
        if (offset != 0) {
          sign = 1;
          if (offset <= 0)
            sign = -sign;
        }
        Func_020006ac(actor, sign * 0xc0000, 0, 0x20000);
      }
    }
  }
}

#include "scene_tables.h"

void Func_020056b4(struct Actor *object);
void Event_FinishScreenTransition(void);

void Func_02000a20(struct Actor *object) {
  u32 i;
  struct MainRoutineWork *party;

  Func_020056b4(object);
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = PIXELS(6);
  object->z = 0xf741cf;
  object->angle = FACING_SOUTH;
  party = &gMainRoutineWork;
  Actor_SetAttachedEffect(party->currentOwner, 258);
  Actor_SetAnimation(party->currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetAnimation(party->currentOwner, 28);
  FieldEvent_Wait(10);
  for (i = 0; i <= 19; i++) {
    object->scaleX -= 0x300;
    object->scaleY -= 0x300;
    object->y -= 0x18000;
    System_Wait(1);
    if (i == 10)
      Event_PlaySound(204);
    if (i == 20) {
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
      Event_FinishScreenTransition();
    }
  }
}

extern char SceneId_Ov671Room4;

// Moves the controlled character with the moving platforms (Func_020005b4 finds
// the one under it, 360 forward and 361 back): it stays within 2 pixels of the
// platform's x and is carried along z by the speed in gData_02005af0; past 20
// pixels from the platform it steps off (Func_020006ac). In Ov671Room4 at cell
// (28, 15) or (30, 15) it ends the ride (Func_02000a20) and runs exit 5 or 6
// (Import_EventSetExitRequest).
void Func_02000ad4(void) {
  struct Actor *actor, *platform;
  s32 speed, d, sign, x, z, platformZ, offset;

  if (gData_02005af0 == 0)
    return;
  actor = Actor_Get(FieldParty_GetLeaderActor());
  speed = gData_02005af0 << 15;
  platform = Func_020005b4(actor, 360);
  if (platform != 0) {
    if (platform->x - actor->x < -0x20000)
      actor->x = platform->x + 0x20000;
    if (platform->x - actor->x > 0x20000)
      actor->x = platform->x - 0x20000;
    z = actor->z + speed;
    actor->z = z;
    platformZ = platform->z;
    d = platformZ - z;
    if (d >= 0 ? d >= 0x140000 : z - platformZ >= 0x140000) {
      sign = 0;
      offset = actor->z - platformZ;
      if (offset != 0) {
        sign = 1;
        if (offset <= 0)
          sign = -sign;
      }
      Func_020006ac(actor, 0, (sign * 3) << 18, 0x20000);
    }
  }
  platform = Func_020005b4(actor, 361);
  if (platform == 0)
    return;
  if (platform->x - actor->x < -0x20000)
    actor->x = platform->x + 0x20000;
  if (platform->x - actor->x > 0x20000)
    actor->x = platform->x - 0x20000;
  z = actor->z - speed;
  actor->z = z;
  platformZ = platform->z;
  d = platformZ - z;
  if (d >= 0 ? d >= 0x140000 : z - platformZ >= 0x140000) {
    sign = 0;
    offset = actor->z - platformZ;
    if (offset != 0) {
      sign = 1;
      if (offset <= 0)
        sign = -sign;
    }
    Func_020006ac(actor, 0, (sign * 3) << 18, 0x20000);
  }
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov671Room4)
    return;
  x = actor->x >> 20;
  if (actor->z >> 20 != 15)
    return;
  if (x == 28) {
    Func_02000a20(actor);
    FieldEvent_SetExitRequest(5);
  } else if (x == 30) {
    Func_02000a20(actor);
    FieldEvent_SetExitRequest(6);
  }
}

void Func_020056bc(void);

void Func_02000c38(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = Func_02000624(actor, 359);
    s32 dx = *speed << 20;

    if (found != NULL) {
      u16 facing = *(u16 *)((u8 *)found + 6);

      if (facing == 0x8000)
        dx = -dx;
      facing = *(u16 *)((u8 *)actor + 6);
      if ((facing == 0 && dx > 0) || (facing == 0x8000 && dx < 0))
        Func_020006ac(actor, dx, 0, 0x40000);
    }
  }
}

void Func_02000c8c(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = Func_02000624(actor, 360);
    s32 dz = *speed << 20;
    u16 facing;

    if (found == NULL) {
      found = Func_02000624(actor, 361);
      dz = -dz;
      if (found == NULL)
        return;
    }
    facing = *(u16 *)((u8 *)actor + 6);
    if ((facing == 0x4000 && dz > 0) || (facing == 0xc000 && dz < 0))
      Func_020006ac(actor, 0, dz, 0x40000);
  }
}

void Func_02000cf0(struct Actor *object) {
  Func_020056b4(object);
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Actor_SetPosition(39, 164 << 17, 148 << 17);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(15);
  FieldActor_WatchAndSetScript(gMainRoutineWork.currentOwner, 39);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 30);
  FieldCamera_FollowActor(39, 1);
  FieldEvent_Wait(30);
  Actor_Get(39)->priorityFlags |= 0x20;
  Actor_SetAnimation(39, 1);
  Actor_WalkToAndWait(39, 320, 240);
  Actor_WalkToAndWait(39, 304, 216);
  Actor_WalkToAndWait(39, 280, 216);
  Actor_WalkToAndWait(39, 280, 184);
  FieldEvent_Wait(20);
  Actor_FaceActor(39, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  Event_PlaySound(262);
  {
    s32 a = 256, b = 30;

    Actor_ShowEmote(39, a, b);
  }
  Actor_WalkToAndWait(39, 344, 184);
  Actor_WalkToAndWait(39, 344, 200);
  FieldEvent_Wait(2);
  Actor_FaceDirection(39, FACING_WEST, 0);
  FieldEvent_Wait(15);
  Event_PlaySound(152);
  Actor_Get(39)->velocityY = PIXELS(4);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 312, 200);
  Event_PlaySound(152);
  Actor_Get(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 312, 168);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_WalkToAndWait(39, 328, 152);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldEvent_Wait(15);
  Event_PlaySound(152);
  Actor_Get(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 360, 152);
  FieldEvent_Wait(15);
  Event_PlaySound(152);
  Actor_Get(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_WalkToAndWait(39, 360, 184);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_WalkToAndWait(39, 376, 184);
  Actor_WalkToAndWait(39, 392, 200);
  Actor_WalkToAndWait(39, 408, 184);
  FieldEvent_Wait(15);
  Event_PlaySound(152);
  Actor_Get(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 440, 184);
  Actor_SetPosition(39, 0, 0);
  Event_SetFlag(0xa6c);
  Func_020056bc();
}
