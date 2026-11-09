#include "jupiter_lighthouse_room_m.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020047d4(void);
void Func_020047dc(void);
void Func_020048f4(void);

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 2 of 292
   * instructions differ in tla-en. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_0200475c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 46 of 292
   * instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

static __inline__ void Func_020048e4_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 26 of 292
   * instructions differ in tla-en. */
  Camera_SetSpeedParameters(a, b);
}

static __inline__ void Func_0200474c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 12 of 292
   * instructions differ in tla-en. */
  Func_0802b450(a, b, c, d, e, f);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 63 of 292
   * instructions differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 21 of 292
   * instructions differ in tla-en. */
  return GameFlag_Test(flag);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                 s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 2 of 292
   * instructions differ in tla-en. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

/// Opens the floor under (49, 103) and (49, 106) once (flag 514, until 2591).
/// Object 23, standing on the ground at x cell 49 to 52 with flag 2573 clear,
/// falls through and sets 2573: from cells 50 and 51 it drops, lands with the
/// screen shaken and slides off sideways; from 49 and 52 it simply drops.
void Func_0200060c(void) {
  struct Actor *object = Actor_GetById(23);
  s32 spot = 0;
  s32 x;
  s32 i;

  if (GameFlag_Test_(2591))
    return;
  GameFlag_SetBit(514);
  Func_020047d4();
  Event_ResolvePendingActions(0);
  Sound_Play(124);
  Camera_MoveWithinMapBounds_(194 << 18, -1, PIXELS(208), 1);
  Func_020048f4();
  Event_Wait(6);
  Sound_Play(217);
  Func_0200474c_(49, 103, 4, 3, 49, 76);
  Event_Wait(6);
  Func_0200474c_(49, 106, 4, 3, 49, 76);
  x = object->x >> 20;
  if (x == 49)
    spot = 1;
  if (x == 50)
    spot = 2;
  if (x == 51)
    spot = 3;
  if (x == 52)
    spot = 4;
  if (object->y != 0)
    spot = 0;
  if (GameFlag_Test_(2573))
    spot = 0;
  if (spot != 0)
    GameFlag_SetBit(2573);
  if ((u32)(spot - 2) <= 1) {
    Func_020048e4_(0x40000, 0x8000);
    FieldCamera_FollowActor(23, 1);
    Func_020048f4();
    Sound_Play(310);
    object->gravity = 0x8000;
    object->motionFlags = 2;
    object->terrainHeight = PIXELS(-80);
    i = 0;
    if (object->y > object->terrainHeight) {
      do {
        System_Wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight);
    }
    Func_0200475c_(0x30000, 0x30000, 0xf333);
    Sound_Play(134);
    object->gravity = 0x10000;
    object->velocityY = PIXELS(6);
    Actor_SetSpeedParameters_(23, 0x18000, 0xc000);
    if (spot == 2)
      Actor_OffsetPositionAndResetMotion_(23, -32, 0);
    else
      Actor_SetDestinationOffset(23, 32, 0);
    Event_Wait(7);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    object->gravity = 0x8000;
    object->terrainHeight = PIXELS(-240);
    Event_Wait(80);
    Func_0200475c_(0x50000, 0x50000, 0x10000);
    Sound_Play(144);
    Event_Wait(20);
    Func_0200475c_(-1, -1, 0xe666);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  } else if (spot == 1 || spot == 4) {
    FieldActor_SetSpritePriority(23, 1);
    Func_020048e4_(0x40000, 0x8000);
    FieldCamera_FollowActor(23, 1);
    Func_020048f4();
    Sound_Play(310);
    object->gravity = 0x8000;
    object->motionFlags = 2;
    object->terrainHeight = PIXELS(-240);
    Event_Wait(20);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    Event_Wait(60);
    Func_0200475c_(0x50000, 0x50000, 0x10000);
    Sound_Play(144);
    Event_Wait(20);
    Func_0200475c_(-1, -1, 0xe666);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  }
  Event_Wait(20);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020048f4();
  Func_020047dc();
}
