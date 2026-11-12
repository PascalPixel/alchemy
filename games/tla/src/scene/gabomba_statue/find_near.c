/// @file
/// Scene code of overlay 0x671, in ROM order, from find_near's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field.h"

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
  u8 motionFlags;
  u8 unk56[12];
  u8 phase;
  u8 slot;
  s16 centerX;
  s16 centerZ;
  struct Orbiter *link;
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

#include "actor.h"

void Func_020057d4(struct Actor *object);
void Func_02005624(struct Actor *object, s32 mode);

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
      u16 angle = near->facing;

      if (angle == 0x8000)
        delta = turn;
    }
    leader->facing += delta;
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
    if (table->facing == 0x8000)
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
  actor->facing += turn;
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

      if (found->facing == 0x8000)
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

void Func_020056b4(void);
void Event_FinishScreenTransition(void);

void Func_02000a20(struct Actor *object) {
  u32 i;
  struct MainRoutineWork *party;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = PIXELS(6);
  object->z = 0xf741cf;
  object->facing = FACING_SOUTH;
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
  Func_020056b4();
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

#include "input.h"
#include "text/msg_ids.h"

void Func_020022d0(s32 id, s32 x, s32 z, s32 facing);
void Func_020011bc(s32 message, s32 value);
void Func_0200533c(void);
void Func_020057b4(void);
s32 Func_02004d48(s32 a, s32 b);
void Func_0200581c(void);
void Event_WaitTransitionFrames(void);

void Func_02000f50(s32 id) {
  Actor_SetPosition(id, 0, 0);
  FieldActor_Stop(id);
  Actor_SetAnimation(id, 1);
}

void Func_02000f6c(void) {
  Func_02000f50(11);
  Func_02000f50(12);
  Func_02000f50(13);
  Func_02000f50(14);
  Func_020022d0(11, 11, 9, 0);
  Func_020022d0(12, 11, 9, 0x8000);
  Func_020022d0(13, 10, 10, 0x4000);
  Func_020022d0(14, 10, 10, 0xc000);
}

void Func_02000fc0(void) {
  s32 result;

  Event_SetFlag(0x205);
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(78);
  Func_0200533c();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaAcceptOffering);
  Event_ShowMessage(24, 0);
  FieldCamera_MoveTo(PIXELS(392), -1, PIXELS(256), 1);
  Func_020057b4();
  Event_PlaySound(220);
  System_Wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Func_02000f6c();
  System_Wait(1);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Event_PlaySound(130);
  Actor_SetSpriteColor(Actor_Get(9), 7);
  Actor_SetSpriteColor(Actor_Get(10), 7);
  System_Wait(8);
  Actor_SetSpriteColor(Actor_Get(10), 3);
  Actor_SetSpriteColor(Actor_Get(9), 0);
  System_Wait(8);
  Event_PlaySound(11);
  result = Func_02004d48(11, 1);
  Func_0200581c();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  switch (result) {
  case 3:
    FieldEvent_SetExitRequest(14);
    break;
  case 1:
    FieldEvent_SetExitRequest(15);
    break;
  case 2:
    FieldEvent_SetExitRequest(16);
    break;
  case 0:
    FieldEvent_SetExitRequest(17);
    break;
  }
}

#include "scene_ids.h"

extern const u8 gData_02006e9c[];
extern const u8 gData_02006b54[];
extern const u8 gData_02006b6c[];
extern const u8 gData_02006bd8[];
extern const u8 gData_02006c80[];
extern const u8 gData_02006d1c[];
extern const u8 gData_02006db8[];
extern const u8 gData_02006e78[];

const u8 *Func_02001124(void) {
  s16 scene = gMainRoutineWork.scene;
  const u8 *table = gData_02006e9c;

  if (scene == (s32)&SceneId_Ov671Room1)
    table = gData_02006b54;
  else if (scene == (s32)&SceneId_Ov671Room2)
    table = gData_02006b6c;
  else if (scene == (s32)&SceneId_Ov671Room3)
    table = gData_02006bd8;
  else if (scene == (s32)&SceneId_Ov671Room4)
    table = gData_02006c80;
  else if (scene == (s32)&SceneId_Ov671Room5)
    table = gData_02006d1c;
  else if (scene == (s32)&SceneId_Ov671Room6)
    table = gData_02006db8;
  else if (scene == (s32)&SceneId_Ov671Room7)
    table = gData_02006e78;
  return table;
}

#include "../gabomba_statue_chamber/gabomba_statue_chamber.h"

void Func_020011bc(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      Actor_Get(gMainRoutineWork.currentOwner);

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x, z);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  FieldActor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 13);
  FieldActor_SetMoveTarget(object, x << 16, PIXELS(-12), z << 16);
  Actor_WaitForMove(object);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 10);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02001284(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 11) {
    if (x == 18 && (gInput.held & 16) == 0)
      return;
    if (x == 20 && (gInput.held & 32) == 0)
      return;
  } else if (x == 19 && z == 12 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(312, 180);
}

void Func_020012ec(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 29) {
    if (z == 11 && (gInput.held & 128) == 0)
      return;
    if (z == 13 && (gInput.held & 64) == 0)
      return;
  } else if (z == 12) {
    if (x == 28 && (gInput.held & 16) == 0)
      return;
    if (x == 30 && (gInput.held & 32) == 0)
      return;
  }
  Func_020011bc(472, 196);
}

void Func_02001364(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 7) {
    if (x == 32 && (gInput.held & 16) == 0)
      return;
  } else if (x == 33 && z == 8 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(536, 112);
}

void Func_020013bc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 13) {
    if (x == 5 && (gInput.held & 16) == 0)
      return;
    if (x == 7 && (gInput.held & 32) == 0)
      return;
  } else if (x == 6) {
    if (z == 12 && (gInput.held & 128) == 0)
      return;
    if (z == 14 && (gInput.held & 64) == 0)
      return;
  }
  Func_020011bc(104, 208);
}

void Func_02001430(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42) {
    if (z == 12 && (gInput.held & 128) == 0)
      return;
    if (z == 14 && (gInput.held & 64) == 0)
      return;
  } else if (x == 43 && z == 13 && (gInput.held & 16) == 0) {
    return;
  }
  Func_020011bc(680, 208);
}

void Func_02001498(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 7) {
    if (x == 23 && (gInput.held & 16) == 0)
      return;
    if (x == 25 && (gInput.held & 32) == 0)
      return;
  } else if (x == 24 && z == 8 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(392, 112);
}

void Func_02001500(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 9) {
    if (x == 5 && (gInput.held & 16) == 0)
      return;
    if (x == 7 && (gInput.held & 32) == 0)
      return;
  } else if (x == 6 && z == 10 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(104, 146);
}

void Func_02001564(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42 && z == 10 && (gInput.held & 64) == 0)
    return;
  if (x == 43 && z == 9 && (gInput.held & 32) == 0)
    return;
  Func_020011bc(680, 146);
}

void Func_020015b8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 27) {
    if (z == 17 && (gInput.held & 128) == 0)
      return;
    if (z == 19 && (gInput.held & 64) == 0)
      return;
  } else if (z == 18) {
    if (x == 26 && (gInput.held & 16) == 0)
      return;
    if (x == 28 && (gInput.held & 32) == 0)
      return;
  }
  Func_020011bc(440, 288);
}

struct EventWork154 {
  u8 unk000[0x154];
  s16 exit;
};

/// Jumps down the hole.
void Func_02001630(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct Actor *leader;
  struct EventWork154 *work;

  if (x == 28 && z == 9 && !(gInput.held & 0x10))
    return;
  if (x == 29 && z == 10 && !(gInput.held & 0x40))
    return;
  work = Ram_HeapSlots->eventWork;
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 472, 144);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  FieldActor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 13);
  FieldActor_SetMoveTarget(leader, PIXELS(472), 0, PIXELS(156));
  Actor_WaitForMove(leader);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 10);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02001728(struct Actor *actor) {
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 424);
  Func_020056bc();
}

void Func_0200176c(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  s32 speed;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  actor->motionFlags = 0;
  speed = 0x19999;
  /* FAKEMATCH(barrier): an empty asm on speed, so the speed is loaded before
   * the owner, whose base is already in a register; as the plain call, 4 of 40
   * instructions differ in tla-en. */
  asm volatile("" : "+l"(speed));
  Actor_SetSpeed(gMainRoutineWork.currentOwner, speed, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 488);
  actor->motionFlags = 3;
  FieldEvent_Wait(8);
  Event_PlaySound(161);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  FieldEvent_Wait(20);
  Func_020056bc();
}

void Func_020017dc(struct Actor *actor) {
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 376, 200);
  Func_020056bc();
}

void Func_02001820(struct Actor *actor) {
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 376, 136);
  Func_020056bc();
}

void Func_02001864(struct Actor *actor) {
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 440, 248);
  Func_020056bc();
}

void Func_020018a8(struct Actor *actor) {
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 440, 184);
  Func_020056bc();
}

void Func_02001950(s32 show, s32 first, u32 count);

void Func_020018ec(s32 show) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov671Room1)
    Func_02001950(show, 8, 1);
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov671Room2)
    Func_02001950(show, 29, 2);
}

struct GateEventWork {
  u8 unk000[362];
  u16 gateTimer;
};

extern const u8 gData_02005d7c[];
void Func_02005428(const u8 *data, s32 a, s32 b);

void Func_02001928(void) {
  Func_020018ec(1);
  Event_SetFlag(512);
}

void Func_0200193c(void) {
  Func_020018ec(0);
  Event_ClearFlag(512);
}

void Func_02001950(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_Get(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void Func_02001990(s32 a, s32 b) {
  Func_02005428(gData_02005d7c, a, b);
  if (Event_GetFlag(0x8ce) && Event_GetFlag(0x8cf))
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 200;
}

void Func_020019d0(void) {
  struct Actor *actor = Actor_Get(26);

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0x9999, 0x1333);
  FieldCamera_MoveTo(180 << 17, 0, 248 << 17, 1);
  Func_020057b4();
  Event_PlaySound(586);
  Func_020020f0(0);
  FieldEvent_Wait(40);
  Event_PlaySound(582);
  actor->motionFlags = 0;
  actor->y = PIXELS(8);
  FieldEvent_Wait(10);
  actor->y = 0;
  FieldEvent_Wait(10);
  Func_020020f0(-1);
  Event_SetFlag(0x8cd);
  FieldEvent_Wait(20);
  Func_020056bc();
}

struct FieldPart {
  s16 kind;
};

struct FieldAnimation {
  u8 unk00[40];
  struct FieldPart *part;
};

void Func_020043a4(s32 value);
s32 Func_020053a0(void);
void Func_02005784(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02001a6c(s32 value) {
  Func_020043a4(value);
  if (value == 1)
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 200;
}

void Func_02001a8c(void) {
  struct Actor *object = Actor_Get(15);

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(20);
  Event_PlaySound(214);
  Func_02005624(object, 5);
  object->scaleY = 0x10000;
  Func_020056bc();
}

void Func_02001ac0(void) {
  if (Func_020053a0() == 0)
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 201;
}

void Func_02001adc(s32 id) {
  Func_02005784(id);
  Actor_SetSpriteColor(Actor_Get(id), 15);
  FieldActor_SetSpritePriority(id, 1);
  Actor_Get(id)->priorityFlags |= 2;
}

void Func_02001b0c(void) {
  if (Event_GetFlag(265)) {
    if (!Event_GetFlag(512))
      Func_0200193c();
  } else {
    Event_SetFlag(512);
  }
}
void Func_02001b38(void) {
  u32 i = 0;

next:
  {
    s32 id = i + 8;
    struct Actor *object = Actor_Get(id);

    Func_02001adc(id);
    object->motionFlags = 0;
    object->y = PIXELS(36);
    i++;
  }
  if (i < 1)
    goto next;
  Func_02001b0c();
}

void Func_02001b68(void) {
  u32 i = 0;

next:
  {
    s32 id = i + 29;
    struct Actor *object = Actor_Get(id);

    Func_02001adc(id);
    object->motionFlags = 0;
    object->y = PIXELS(36);
    i++;
  }
  if (i < 2)
    goto next;
  Func_02001b0c();
}
void Func_02001b98(void) {
  s32 value = 16;
  s32 open = 0;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 i;

  if (Event_GetFlag(262) || *(s16 *)(event + 358) != 0) {
    open = 1;
    value = 0;
  }
  i = 8;
next:
  {
    struct Actor *object = Actor_Get(i);

    if (object != NULL && object->animationKind == 1) {
      struct FieldPart *part = ((struct FieldAnimation *)object->sprite)->part;

      if (part != NULL) {
        switch (part->kind) {
        case 357:
        case 358:
        case 359:
        case 360:
        case 361:
        case 362:
          Actor_SetAnimationSpeed(object, value);
          break;
        case 407:
          object->movementState = open;
          break;
        }
      }
    }
  }
  i++;
  if (i < 64)
    goto next;
}

#include "task.h"

extern u32 gData_02005910[];
extern u8 Data_0202c001[];

#define MARK(at, cellPicture, shape)                                           \
  (cell = (at), layerA[cell] = (cellPicture), layerB[cell] = (cellPicture),    \
   word = &shapesA[cell], *word |= (shape), word = &shapesB[cell],             \
   *word |= (shape))

void Func_02001c10(void) {
  u32 *dest = (u32 *)(Data_0202c001 + 0x1cf);
  u32 *src = gData_02005910;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *layerA = *(u8 **)(map + 0x13c);
  u8 *layerB = *(u8 **)(map + 0x174);
  u32 *shapesA = *(u32 **)(map + 0x138);
  u32 *shapesB = *(u32 **)(map + 0x170);
  struct Actor *object;
  s16 *frames;
  u32 *word;
  u32 i;
  s32 x, z, cell, found;

  i = 0;
  do {
    *dest++ = *src++;
  } while (++i <= 11);
  for (i = 8; (s32)i <= 63; i++) {
    object = Actor_GetById(i);
    if (object == 0)
      continue;
    if (object->animationKind != 1)
      continue;
    frames = *(s16 **)((u8 *)object->sprite + 40);
    if (frames == 0)
      continue;
    found = 0;
    x = object->x >> 20;
    z = object->z >> 20;
    switch (*frames - 357) {
    case 1:
      cell = x + (z << 7);
      layerA[cell] = 116;
      layerB[cell] = 117;
      word = &shapesA[cell];
      *word |= 0x80500000;
      word = &shapesB[cell];
      *word |= 0x80500000;
      found = 1;
      break;
    case 0:
      MARK(x + (z << 7) - 129, 118, 0x510000);
      MARK(x + (z << 7) - 128, 117, 0x80510000);
      MARK(x + (z << 7) - 127, 119, 0x510000);
      MARK(x + (z << 7) - 1, 117, 0x80510000);
      MARK(x + (z << 7), 117, 0x510000);
      MARK(x + (z << 7) + 1, 117, 0x80510000);
      MARK(x + (z << 7) + 127, 120, 0x510000);
      MARK(x + (z << 7) + 128, 117, 0x80510000);
      MARK(x + (z << 7) + 129, 121, 0x510000);
      found = 1;
      break;
    case 2:
      MARK(x + (z << 7) - 2, 127, 0x540000);
      MARK(x + (z << 7) - 1, 122, 0x529000);
      MARK(x + (z << 7), 117, 0x80526000);
      MARK(x + (z << 7) + 1, 123, 0x529000);
      MARK(x + (z << 7) + 2, 127, 0x540000);
      found = 1;
      break;
    case 4:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 126, 0x80550000);
      found = 1;
      break;
    case 3:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 127, 0x540000);
      found = 1;
      break;
    case 5:
      found = 1;
      break;
    }
    if (found) {
      object->collisionFlags = 0;
      object->motionFlags = 0;
      object->priorityFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
    }
  }
  Task_AddFrame(Func_02001b98, TASK_PASS_VBLANK);
}

#include "macros.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)

/// Makes the scene more translucent the farther inside the object is.
void Func_02002074(void) {
  s32 blend;
  struct Actor *object =
      ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->object;
  s32 level = object->z;

  level = (level - 0xe80000) >> 19;
  CLAMP(level, 0, 16);
  blend = ((16 - level) << 8) + level;
  REG_BLDALPHA->value = blend;
}

extern const s8 gData_02005940[];

void Func_0200561c(void *animation, s32 a);

void Func_020020b0(s32 id, s32 direction, s32 value) {
  struct Actor *object = Actor_Get(id);
  void *animation = object->sprite;

  Func_02005624(object, gData_02005940[(direction + 1) & 3]);
  object->unk5c[0] = 2;
  Func_0200561c(animation, value);
}

void Func_020020f0(s32 direction) {
  s16 scene;

  gData_02005af0 = direction;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov671Room2) {
    Func_020020b0(14, direction, 0);
    Func_020020b0(21, direction, 1);
    Func_020020b0(26, direction, 2);
    if ((u16)(gMainRoutineWork.entrance - 3) > 1)
      Func_020020b0(27, direction, 3);
    if (direction == 0)
      MapGrid_CopyCellAttributes(23, 21, 1, 3, 23, 9);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(24, 21, 1, 3, 23, 9);
  } else if (scene == (s32)&SceneId_Ov671Room3) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(15, direction, 1);
    Func_020020b0(20, direction, 2);
    Func_020020b0(25, direction, 3);
    Func_020020b0(29, direction, 4);
  } else if (scene == (s32)&SceneId_Ov671Room4) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(21, direction, 1);
    Func_020020b0(29, direction, 2);
    Func_020020b0(35, direction, 3);
    Func_020020b0(38, direction, 4);
  } else if (scene == (s32)&SceneId_Ov671Room5) {
    Func_020020b0(10, direction, 0);
    Func_020020b0(12, direction, 1);
    if (direction == 0)
      MapGrid_CopyCellAttributes(34, 12, 1, 3, 27, 12);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(35, 12, 1, 3, 27, 12);
  } else if (scene == (s32)&SceneId_Ov671Room6) {
    Actor_SetAnimation(13, 5);
    Actor_SetSingleChildFlags(Actor_Get(13), 0);
  } else if (scene == (s32)&SceneId_Ov671Room7) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(9, direction, 1);
    Func_020020b0(10, direction, 2);
    Func_020020b0(11, direction, 3);
    Func_020020b0(12, direction, 4);
    if (direction == 0)
      MapGrid_CopyCellAttributes(0, 0, 1, 3, 9, 27);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(1, 0, 1, 3, 9, 27);
  }
}

extern const u8 gData_0200e88c[];

void Func_02005634(struct Orbiter *object, const u8 *script);
void Animation_ApplyChildValues(struct Orbiter *object, s32 value);

void Func_020022d0(s32 slot, s32 value, s32 targetId, s32 angle) {
  struct Orbiter *object = ((struct Orbiter *)Actor_Get(slot));
  struct Orbiter *target = ((struct Orbiter *)Actor_Get(targetId));

  Func_02005624(object, 0);
  Func_02005634(object, gData_0200e88c);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->link = target;
  object->phase = angle / 256;
  object->slot = slot;
  Animation_ApplyChildValues(object, value);
}
