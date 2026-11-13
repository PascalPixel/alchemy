/// @file
/// Scene code of overlay 0x653, in ROM order.

#include "../shared/routines.h"
#include "../shared/tables.h"
#include "../kandorean_temple.h"
#include "field_effect.h"
#include "event_work.h"
#include "event_inline.h"
#include "version.h"
#include "task.h"
#include "text/msg_ids.h"
#include "module/field/field_effect.h"
#include "module/scene/kandorean_temple.h"
#include "module/scene/scene.h"

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02002ec4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Math_Cosine(i << 12);
    velocity[1] = 0;
    velocity[2] = Math_Sine(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
    Func_020000b8(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                  velocity[2], 0x01090001, &options);
  }
  Sound_Play(188);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

void Func_02000cd8(s32 id, s32 offset, s32 limit);

void Func_0200306c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *statue = Actor_GetById(9);
  struct Actor *ride = FieldCamera_GetActor();
  s16 i;
  u16 next;
  s32 limit;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  leader->motionFlags = 0;
  wait(1);
  GameFlag_SetBit(0x208);
  GameFlag_ClearBit(0x209);
  GameFlag_SetBit(0x301);
  GameFlag_ClearBit(0x300);
  Sound_Play(162);
  Func_02000d64();
  i = 0;
  do {
    leader->y += 0x48000;
    statue->y += 0x48000;
    ride->y += 0x1cccc;
    limit = 0x1000;
    Func_02000cd8(11, 0x20000, limit);
    Func_02000cd8(13, 0x18000, limit);
    Func_02000cd8(14, 0x10000, limit);
    Func_02000cd8(15, 0x8000, limit);
    Func_02000cd8(16, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next <= 15);
  Map_CenterOnFollowedPoint();
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  DEFINE_FUNC_02000CF8(11, 13, 14, 15, 16)
  gData_02006298 = 0;
  GameFlag_ClearBit(0x20c);
  Sound_Play(390);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_SetModeById(9, 1);
  Actor_SetPositionAndResetMotion_(Actor_GetById(9), 0xd80000, 0, 0xd80000);
  statue->priorityFlags |= 2;
  Actor_SetSpritePriority(9, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

void Func_02000e60(void);

void Func_020032c8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *statue = Actor_GetById(10);
  struct Actor *ride = FieldCamera_GetActor();
  s16 i;
  u16 next;
  s32 limit;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  leader->motionFlags = 0;
  wait(1);
  GameFlag_ClearBit(0x208);
  GameFlag_SetBit(0x209);
  GameFlag_ClearBit(0x301);
  GameFlag_SetBit(0x300);
  Sound_Play(162);
  Func_02000e60();
  i = 0;
  do {
    leader->y += 0x48000;
    statue->y += 0x48000;
    ride->y += 0x1cccc;
    limit = 0x1000;
    Func_02000cd8(12, 0x20000, limit);
    Func_02000cd8(17, 0x18000, limit);
    Func_02000cd8(18, 0x10000, limit);
    Func_02000cd8(19, 0x8000, limit);
    Func_02000cd8(20, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next <= 15);
  Map_CenterOnFollowedPoint();
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  DEFINE_FUNC_02000CF8(12, 17, 18, 19, 20)
  gData_0200629c = 0;
  GameFlag_ClearBit(0x20d);
  Sound_Play(390);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  Actor_SetModeById(10, 1);
  Actor_SetPositionAndResetMotion_(Actor_GetById(10), 0x1780000, 0, 0xf80000);
  statue->priorityFlags |= 2;
  Actor_SetSpritePriority(10, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

void Func_02003524(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

void Func_02003564(void) {
  Func_02003524();
  Import_EventSetExitRequest(3);
}

void Func_02003574(void) {
  Func_02003524();
  Import_EventSetExitRequest(4);
}

void Func_02003584(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;
  struct MainRoutineWork *party;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  object->facing = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 16);
  for (i = 0; i <= 7; i++) {
    Actor_SetPositionAndResetMotion(object, object->x, object->y,
                                    object->z + PIXELS(3));
    wait(1);
  }
  Event_Wait(20);
  party = &gMainRoutineWork;
  object->facing = FACING_SOUTH;
  FieldEffect_SetAttached(party->currentOwner, 258);
  Actor_SetModeById(party->currentOwner, 27);
  Event_Wait(30);
  Actor_SetModeById(party->currentOwner, 28);
  Event_Wait(10);
  Actor_SetSpritePriority(party->currentOwner, 3);
  for (i = 0; i <= 39; i++) {
    {
      s32 scale = object->scaleX;

      object->scaleX = scale + -0x400;
    }
    object->scaleY += -0x400;
    object->y += -0x20000;
    wait(1);
    if (i == 10)
      Sound_Play(204);
    if (i == 20) {
      ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
      Event_FinishScreenTransition();
    }
  }
  Import_EventSetExitRequest(5);
}

/// The leader and actor 13 rise into view while actor 15 scales.
DEFINE_FUNC_02003684(Func_02003684, 13, 15, 440, 200, 204, 27, 12,
                     gData_020062a0)

/// As Func_02003684, for actors 14 and 16 at another place.
DEFINE_FUNC_02003684(Func_020037e0, 14, 16, 600, 232, 236, 37, 14,
                     gData_020062a4)

/// Actor 13 rises out of view while actor 15 scales, then the scene moves on.
void Func_0200393c(void) {
  struct Actor *object = Actor_GetById(13);

  Event_Start();
  Event_ResolvePendingActions(0);
  // A goto loop: as a do-while, loop.c hoists the scale constant into r6
  // (differ 19).
rise:
  object->y -= PIXELS(4);
  Func_02000cf8(15, 0x4ccc, 0x4ccc);
  wait(1);
  if (object->y > -0x140000)
    goto rise;
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(6);
}

/// As Func_0200393c, for actors 14 and 16.
void Func_020039a8(void) {
  struct Actor *object = Actor_GetById(14);

  Event_Start();
  Event_ResolvePendingActions(0);
  // A goto loop, as in Func_0200393c.
rise:
  object->y -= PIXELS(4);
  Func_02000cf8(16, 0x4ccc, 0x4ccc);
  wait(1);
  if (object->y > -0x140000)
    goto rise;
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(7);
}

void Func_02003a14(void) {
  struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

  gData_02006294 = 0;
  map->mode = 1;
  Func_02005338(0);
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
#endif
}

void Func_02003a5c(void) {
  struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

  gData_02006294 = 0;
  Func_02005388(170);
  map->mode = 1;
  Func_02005338(0);
}

void Func_02003a8c(void) {
  Func_02005390();
  Func_020053a8(0, 0x20f, 8, 9);
  if (GameFlag_Test(0x849)) {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  } else {
    Actor_SetSingleChildFlags(Actor_GetById(10), 0);
    Actor_SetModeById(10, 6);
    Actor_SetMovementTypeById(10, gData_02005550);
  }
  Func_02005338(0);
}

void Func_02003ae4(s32 id, s32 base) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetModeById(id, 5);
  object->timer = base + ((Random16Far() * 400) >> 16);
  object->unk66 = id;
  Actor_SetMovementTypeById(id, gData_02005548);
}

void Func_02003b2c(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x100;
  if (GameFlag_Test(0x842)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  } else {
    Func_020052c0(8);
  }
  if (GameFlag_Test(0x849)) {
    MapGrid_CopyCellAttributes(2, 0, 2, 3, 17, 24);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetPositionAndFacing(11, PIXELS(374), PIXELS(282), FACING_EAST);
    {
      s32 angle = 0xd000;

      Actor_SetPositionAndFacing(12, PIXELS(272), PIXELS(296), angle);
    }
    {
      s32 angle = 0x3000;

      Actor_SetPositionAndFacing(13, PIXELS(216), PIXELS(264), angle);
    }
  } else {
    Func_02003ae4(11, 0);
    Func_02003ae4(12, 240);
    Func_02003ae4(13, 480);
  }
  if (GameFlag_Test(0x212)) {
    MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
    MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4)
    GameFlag_ClearBit(0x12f);
#endif
}

void Func_02003c54(void) {
  if (Event_GetFlag(0x210) == 0) {
    Event_SetFlag(0x210);
    Func_020053a8_(1, 0x210, 10, 11);
  }
  {
    s32 angle = 0x3000;

    Actor_SetPositionAndFacing(12, PIXELS(392), PIXELS(360), angle);
  }
}

void Func_02001c8c(void);

void Func_02003c8c(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Func_02005390();
  Func_020053a8_(0, 527, 8, 9);
  Func_020053a8(1, 528, 10, 11);
  if (Event_GetFlag(0x849) == 0) {
    Actor_SetModeById(13, 5);
    Actor_SetModeById(15, 5);
  }
  Actor_SetModeById(14, 5);
  Task_Add(Func_02001c8c, TASK_PASS_VBLANK);
  if ((gMainRoutineWork.entrance >= 4 && gMainRoutineWork.entrance <= 6) ||
      gMainRoutineWork.entrance == 14) {
    if (GameFlag_Test(0x84a)) {
      s32 angle = 0x5000;

      Actor_SetPositionAndFacing(12, PIXELS(432), PIXELS(394), angle);
    } else {
      Func_02004fb4();
      if (GameFlag_Test(0x849))
        Func_02003c54();
    }
  }
  if ((gMainRoutineWork.entrance >= 1 && gMainRoutineWork.entrance <= 3) ||
      gMainRoutineWork.entrance == 9) {
    if (GameFlag_Test(0x212))
      Func_02001f3c();
    if (GameFlag_Test(0x849)) {
      /* FAKEMATCH(temporary): a local holding the angle, set just before its
       * call, builds it ahead of the other arguments, as above; as a plain
       * argument, 4 of 179 instructions differ in tla-en. */
      s32 angle;

      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      angle = 0xb000;
      Actor_SetPositionAndFacing(15, PIXELS(408), PIXELS(664), angle);
    } else if (GameFlag_Test(0x848)) {
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    } else {
      Func_02003e8c();
    }
  } else if (gMainRoutineWork.entrance == 6) {
    if (GameFlag_Test(0x849) == 0)
      Func_020044d4();
  } else if (gMainRoutineWork.entrance == 13) {
    Func_020046d8();
  } else if (gMainRoutineWork.entrance == 14) {
    if (GameFlag_Test(0x849) == 0)
      Func_0200482c();
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance >= 8 && gMainRoutineWork.entrance <= 9)
    GameFlag_ClearBit(0x12f);
#endif
}

void Func_02003e34(struct FieldSprite *sprite, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    sprite->rotation += step;
    Event_Wait(1);
  }
}

void Func_02003e60(struct Actor *object, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    object->x += step;
    Event_Wait(1);
  }
}

void Func_02003e8c(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         0x1900000, 0x31c0000);
  wait(1);
  Event_StartScreenTransition();
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 400, 752);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(0x1b40000, 0x480000, 0x2980000, 1);
  Func_020052e8();
  Actor_Jump(12, 4, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple04);
  Event_ShowMessageAndWait(12, 0);
  FieldCamera_MoveWithinMapBounds(0x1c80000, 0, 0x2b00000, 1);
  Func_020052e8();
  Motion_SetVarCbAndRefresh(13, 2);
  Sound_Play(131);
  Actor_ConfigureSpriteColor(13, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(13, 0);
  Event_Wait(20);
  object = Actor_GetById(13);
  sprite = object->sprite;
  object->motionFlags = 0;
  i = 0;
rise13:
  object->y += 0xccc;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto rise13;
  Func_02003e34(sprite, 32, 32);
  Event_Wait(20);
  Actor_SetAttachedEffect(13, 0x102);
  Func_02003e34(sprite, 32, -64);
  Func_02003e34(sprite, 32, 64);
  Func_02003e34(sprite, 64, 32);
  FieldEffect_SetAttached(13, 0x102);
  Event_Wait(60);
  object->motionFlags = 3;
  Event_Wait(5);
  Sound_Play(159);
  object->velocityY = 0x30000;
  sprite->rotation = 0;
  Event_Wait(60);
  Event_ShowMessageAndWait(13, 0);
  FieldCamera_MoveTo(0x1b40000, 0x480000, 0x2980000, 1);
  Func_020052e8();
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 476, 664);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(0x100c, 0);
  Actor_WalkToAndWait(12, 474, 664);
  Actor_WalkToAndWait(12, 472, 648);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Camera_SetSpeedParameters(0x13333, 0x2666);
  FieldCamera_MoveWithinMapBounds(0x16c0000, 0x480000, 0x2980000, 1);
  Func_020052e8();
  Actor_SetHorizontalPositionWithTerrain(12, 0x1480000, 0x2880000);
  wait(1);
  Actor_WalkToAndWait(12, 326, 664);
  Actor_WalkToAndWait(12, 320, 664);
  Actor_FaceDirection(12, 0x5000, 0);
  Event_ShowMessageAndWait(0x200c, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(0x1560000, 0, 0x2b00000, 1);
  Func_020052e8();
  Motion_SetVarCbAndRefresh(14, 2);
  Sound_Play(131);
  Actor_ConfigureSpriteColor(14, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(14, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(14, 0x101);
  object = Actor_GetById(14);
  object->motionFlags = 0;
  i = 0;
rise14:
  object->y += 0x2666;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto rise14;
  Func_02003e60(object, 64, 0x1000);
  Func_02003e60(object, 128, -0x1000);
  Func_02003e60(object, 128, 0x1000);
  Func_02003e60(object, 64, -0x1000);
  Event_Wait(20);
  Actor_SetAttachedEffect(14, 0x100);
  object->motionFlags = 3;
  Event_Wait(5);
  Sound_Play(159);
  object->velocityY = 0x30000;
  Event_Wait(60);
  Event_ShowMessageAndWait(14, 0);
  FieldCamera_MoveWithinMapBounds(0x16c0000, 0x480000, 0x2980000, 1);
  Func_020052e8();
  Event_ShowMessageAndWait(0x200c, 0);
  Actor_FaceDirection(12, 0x3000, 10);
  Event_ShowMessageAndWait(12, 0);
  FieldCamera_MoveWithinMapBounds(0x1900000, 0, 0x2b00000, 1);
  Func_020052e8();
  Motion_SetVarCbAndRefresh(15, 2);
  Sound_Play(131);
  Actor_ConfigureSpriteColor(15, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(15, 0);
  Event_Wait(20);
  object = Actor_GetById(15);
  sprite = object->sprite;
  object->motionFlags = 0;
  i = 0;
rise15:
  object->y += 0x3333;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto rise15;
  Func_02003e34(sprite, 32, 64);
  sprite->rotation = 0;
  Motion_SetVarCbAndRefresh(15, 1);
  Event_Wait(60);
  Func_02003e34(sprite, 32, -64);
  sprite->rotation = 0;
  Motion_SetVarCbAndRefresh(15, 1);
  Event_Wait(60);
  i = 0;
fall15:
  object->y += -0x3333;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto fall15;
  Event_Wait(20);
  Actor_ShowEmoteById(15, 0x108, 0);
  Event_ShowMessageThenWait(15, 0, 20);
  Event_ShowMessageAndWait(0x100c, 0);
  Actor_ShowEmoteById(15, 0x103, 20);
  Actor_SetModeById(15, 1);
  Actor_Jump(15, 4, 0);
  Actor_FaceDirection(15, 0xb000, 20);
  Event_ShowMessageAndWait(0x400f, 0);
  FieldCamera_MoveWithinMapBounds(0x16c0000, 0x480000, 0x2980000, 1);
  Func_020052e8();
  Actor_ShowEmoteById(12, 0x102, 20);
  Event_ShowMessage(0x800c, 0);
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceDirection(12, 0xb000, 40);
  Actor_SetModeById(12, 4);
  Event_ShowMessage(0x800c, 0);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmoteById(12, 0x105, 80);
  Event_ShowMessage(0x800c, 0);
  Actor_FaceDirection(12, 0x3000, 20);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(0x800c, 0);
  Actor_WalkToAndWait(12, 326, 664);
  Actor_WalkToAndWaitById(12, 328, 648);
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Actor_SetSpriteColor(Actor_GetById(12), 15);
  Actor_SetPosition(12, 0x1b80000, 0x2580000);
  FieldCamera_MoveTo(0x1900000, 0, 0x2b00000, 1);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_WalkToAndWait(15, 400, 644);
  Event_Wait(20);
  Sound_Play(188);
  Func_02001f3c();
  Event_ShowMessageAndWait(12, 0);
  Task_RemoveFar(Func_02001c8c);
  Motion_SetModeAndWaitAnimation(15, 3);
  Actor_SetSpritePriority(15, 3);
  Actor_WalkToAndWait(15, 400, 640);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  FieldCamera_MoveWithinMapBounds(0x1900000, 0, 0x2f00000, 1);
  Func_020052e8();
  Task_Add(Func_02001c8c, 0x480);
  Event_End();
  GameFlag_SetBit(0x848);
}

void Func_020044d4(void) {
  /* FAKEMATCH(temporary): a local holding the angle, set just before its call,
   * builds it ahead of the other arguments; as a plain argument, 2 of 206
   * instructions differ in tla-en. */
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         PIXELS(328), PIXELS(360));
  angle = 0x5000;
  Actor_SetPositionAndFacing(12, PIXELS(472), PIXELS(408), angle);
  Actor_GetById(16)->facing = 0xc000;
  wait(1);
  Event_StartScreenTransition();
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 328, 392);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveWithinMapBounds(PIXELS(408), -1, PIXELS(400), 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 376, 408);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(12, 1);
  Event_SetMessageId((s32)&MsgKandoreanTemple05);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(12, FACING_WEST, 20);
  Actor_ShowEmoteById(12, 256, 0);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_WalkToAndWait(12, 456, 408);
  Event_ShowMessageAndWait(12, 0);
  Actor_MoveToObject(16, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_WalkToAndWait(16, 376, 392);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(16, FACING_EAST, 20);
  Actor_SetAttachedEffect(12, 258);
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(16, 265, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(16, FACING_EAST, 0);
  Actor_WalkToAndWait(12, 468, 428);
  Actor_WalkToAndWait(12, 472, 454);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(13);
}

void Func_020046d8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetHorizontalPositionWithTerrain(12, PIXELS(472), PIXELS(648));
  wait(1);
  Actor_WalkToAndWait(12, 472, 664);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(PIXELS(456), PIXELS(40), PIXELS(664), 1);
  Func_020052e8();
  Event_Wait(40);
  Event_SetMessageId((s32)&MsgKandoreanTemple06);
  Event_ShowMessageAndWait(0x800c, 0);
  Actor_WalkToAndWait(12, 472, 648);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(14);
}

void Func_020047f0(void) {
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_WalkToAndWait(12, 496, 392);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_ShowMessageAndWait(0x400c, 0);
}

#define WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

void Func_0200482c(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  Actor_SetPositionAndFacing(16, 0x1780000, 0x1880000, 0);
  Actor_SetHorizontalPositionAndAngleWithTerrain(gMainRoutineWork.currentOwner,
                                                 0x1780000, 0x1980000, 0);
  {
    s32 angle = 0xb000;
    Actor_SetPositionAndFacing(12, 0x1d80000, 0x1c60000, angle);
  }
  WORK->screenEffect = 0x202;
  Event_StartScreenTransition();
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_WalkToAndWait(12, 464, 428);
  Actor_WalkToAndWait(12, 432, 408);
  Actor_FaceDirection(12, 0x8000, 0);
  FieldEffect_SetAttached(12, 0x102);
  Event_SetMessageId((s32)&MsgKandoreanTemple07);
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(12, 0x105, 40);
  Event_ShowMessageAndWait(12, 0);
  Actor_ArmCallback(12, 0, 40);
  Event_ShowMessageAndWait(12, 0);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetVarCbAndRefresh(12, 2);
  Actor_FaceDirection(12, 0x8000, 20);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(16, 0x3000, 10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Event_OpenMessage(16, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(12, 4);
    Event_ShowMessageAndWait(12, 0);
    WORK->messageId++;
  } else {
    WORK->messageId++;
    Actor_SetModeById(12, 3);
    Event_ShowMessageAndWait(12, 0);
  }
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_ShowEmote(16, 0x101, 20);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetAttachedEffect(12, 0x102);
  Event_ShowMessageAndWait(12, 0);
  Actor_SetModeById(16, 3);
  Event_ShowMessageThenWait(16, 0, 40);
  Actor_FaceDirection(12, 0x5000, 40);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 40);
  Actor_FaceDirection(12, 0x8000, 20);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(12, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ShowEmoteById(16, 0x101, 0);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(12, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveWithinMapBounds(0x1a80000, 0x100000, 0x1800000, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02005564);
  Actor_SetMovementTypeById(16, gData_020055a0);
  Actor_SetMovementTypeAndWaitById(12, gData_020055e4);
  Event_Wait(40);
  Func_02000bcc();
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 20);
  Event_OpenMessage(0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_020047f0();
    WORK->messageId++;
  } else {
    WORK->messageId++;
    Func_020047f0();
  }
  Actor_ArmCallback(16, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_ShowEmoteById(16, 0x100, 40);
  Event_ShowMessage(0x5010, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 0);
  Event_OpenMessage(0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(12, 3);
    Event_ShowMessageAndWait(0x400c, 0);
    Actor_ArmCallback(16, 0, 0);
    Actor_ShowEmoteById(16, 0x107, 20);
    Event_ShowMessageAndWait(0x5010, 0);
    WORK->messageId += 2;
  } else {
    WORK->messageId += 2;
    Actor_SetModeById(12, 4);
    Event_ShowMessageAndWait(0x400c, 0);
    Actor_ArmCallback(16, 0, 0);
    Actor_ShowEmoteById(16, 0x107, 20);
    Event_ShowMessageAndWait(0x5010, 0);
  }
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  FieldCamera_MoveTo(0x1a80000, 0, 0x1900000, 1);
  FieldCamera_SetSpeed(0x9999, 0x1333);
  Actor_SetSpeed(12, 0x9999, 0x4ccc);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 12);
  Actor_WatchAndSetScript(16, 12);
  Actor_WalkToAndWait(12, 488, 416);
  Actor_WalkToAndWait(12, 472, 432);
  Actor_FaceDirection(12, 0x5000, 0);
  Event_ShowMessage(0x400c, 0);
  Event_ShowMessageThenWait(0x1010, 0, 80);
  Actor_ShowEmoteById(12, 0x108, 0);
  Actor_FaceDirection(12, 0xb000, 20);
  Event_ShowMessage(0x400c, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(0x400c, 0);
  FieldCamera_MoveTo(0x1a80000, 0x100000, 0x1800000, 1);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_WalkToAndWait(12, 488, 416);
  Actor_WalkToAndWait(12, 504, 376);
  Actor_ArmCallback(12, 0x8000, 20);
  object = Actor_GetById(12);
  sprite = object->sprite;
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 492, 376);
  Actor_SetModeById(12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  /* FAKEMATCH(temporary): the motion flags written through a byte pointer, so
   * the store may alias the rotation and stays after it; as
   * object->motionFlags, which keeps the struct's alias set, 51 of 783
   * instructions differ in tla-en. */
  flags = &object->motionFlags;
  *flags = 0;
  object->y += 0x40000;
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Actor_SetSpeed(12, 0x6666, 0x3333);
  Actor_SetMoveTarget((s32)object, 0x1bc0000, 0x280000, 0x1780000);
  Actor_WaitForMoveById(12);
  *flags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(12), 1);
  sprite->rotation = 0;
  object->scaleX = 0x10000;
  object->terrainHeight = 0x200000;
  Actor_SetModeById(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 424, 376);
  FieldCamera_MoveWithinMapBounds(0x1980000, 0x100000, 0x1900000, 1);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 392, 360);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_SetSpritePriority(12, 2);
  Actor_Stop(16);
  Actor_Stop(gMainRoutineWork.currentOwner);
  wait(1);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetModeById(16, 2);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

    if (leader != NULL)
      Actor_SetDestinationById(16, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
  }
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  GameFlag_SetBit(0x849);
#if TLA_PATCH_1
  WORK->screenEffect = 0x209;
#endif
  Event_End();
}

void Func_02004fb4(void) {
  struct Actor **slot = &gData_020062ac;
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(
      0x1e9, 0x1980000, 0x280000, 0x1580000));

  *slot = object;
  if (object) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(198);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02005028(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Sound_Play(121);
  MapGrid_CopyCellPictureIndicesRect(41, 0, 20, 0, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 1, 20, 1, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 2, 20, 2, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 3, 20, 3, 1, 1);
  Event_Wait(10);
  GameFlag_SetBit(0x92d);
  Event_End();
}

#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000280(void);
void Func_0200028c(void);
void Func_020002a0(void);
void Func_020002dc(void);
void Func_0200035c(void);
void Func_02002120(void);
void Func_020023e8(void);

extern u32 gData_02005458[];

// clang-format off
DEFINE_G_DATA_020012A4(gData_020053e0, 22)

DEFINE_G_DATA_020012A4(gData_0200541c, 44)

DEFINE_G_DATA_020012A4(gData_02005458, 126)

s32 gData_02005494[] = {
    0,
    44,
    22,
    38,
    0,
    3,
    0,
    1,
    38,
    46,
    (s32)Func_0200028c,
    22,
    38,
    0,
    3,
    0,
    1,
    38,
};

s16 gData_020054dc[] = {
    12, -1,
};

s16 gData_020054e0[] = {
    8, -1,
};

u32 gData_020054e4[] = {
    0, 24, 38,
};

u32 gData_020054f0[] = {
    0, 15, 3, 0, 1, 38,
};

s32 gData_02005508[] = {
    (s32)gData_020053e0,
    (s32)gData_0200541c,
    (s32)gData_02005458,
};

u32 gData_02005514[] = {
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gData_02005548[] = {
    46,
    (s32)Func_020002a0,
};

s32 gData_02005550[] = {
    SC_CALL(Func_020002dc),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02005564[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(408)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

DEFINE_G_DATA_02002FCC(gData_020055a0, 440, 392, 1)

u32 gData_020055e4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(496), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(504), 0, PIXELS(376)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gData_02005634[] = {
    SC_CALL(Func_0200035c),
    SC_SET(SW_Y, PIXELS(88)),
    SC_END,
};

s32 gData_0200564c[] = {
    SC_CALL(Func_0200035c),
    SC_SET(SW_Y, PIXELS(44)),
    SC_END,
};

s32 gData_02005664[] = {
    SC_CALL(Func_0200035c),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_020056a0[] = {
    0x4a0140, 0x1500060, 0x70005a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gData_020056c0[] = {
    0x2a0200, 0x2100110, 0x120003a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(14),
    SCENE_EXIT(1, 20, 1),
    SCENE_EXIT(2, 20, 3),
    SCENE_EXIT(3, 19, 1),
    SCENE_EXIT(4, 17, 2),
    SCENE_EXIT(5, 17, 5),
    SCENE_EXIT(6, 17, 3),
    SCENE_EXIT(7, 17, 4),
    SCENE_EXITS(15),
    SCENE_EXIT(1, 22, 9),
    SCENE_EXIT(2, 20, 4),
    SCENE_EXIT(3, 16, 1),
    SCENE_EXIT(4, 20, 2),
    SCENE_EXITS(16),
    SCENE_EXIT(1, 15, 3),
    SCENE_EXIT(2, 22, 8),
    SCENE_EXITS(17),
    SCENE_EXIT(1, 19, 2),
    SCENE_EXIT(2, 14, 4),
    SCENE_EXIT(3, 14, 5),
    SCENE_EXIT(4, 14, 6),
    SCENE_EXITS(18),
    SCENE_EXIT(1, 21, 3),
    SCENE_EXIT(2, 21, 4),
    SCENE_EXITS(19),
    SCENE_EXIT(1, 14, 3),
    SCENE_EXIT(2, 17, 1),
    SCENE_EXITS(20),
    SCENE_EXIT(1, 14, 1),
    SCENE_EXIT(2, 15, 4),
    SCENE_EXIT(3, 14, 2),
    SCENE_EXIT(4, 15, 2),
    SCENE_EXITS(21),
    SCENE_EXIT(1, 2, 4),
    SCENE_EXIT(2, 22, 1),
    SCENE_EXIT(3, 18, 1),
    SCENE_EXIT(4, 18, 2),
    SCENE_EXITS(22),
    SCENE_EXIT(1, 21, 2),
    SCENE_EXIT(2, 22, 4),
    SCENE_EXIT(3, 22, 5),
    SCENE_EXIT(4, 22, 2),
    SCENE_EXIT(5, 22, 3),
    SCENE_EXIT(6, 22, 7),
    SCENE_EXIT(7, 22, 6),
    SCENE_EXIT(8, 16, 2),
    SCENE_EXIT(9, 15, 1),
    SCENE_EXIT(13, 22, 13),
    SCENE_EXIT(14, 22, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020057a8[] = {
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_020057C0(gData_020057c0, 407, 1, 2, 3, 4, 272, 5, 351)

struct ActorPlacement gData_020058b0[] = {
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {450, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(504), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(328), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(296), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(360), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005958[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {351, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_WEST, 2, 2},
    {351, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005aa8[] = {
    {407, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},
    {91, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(340), FACING_SOUTH, 2, 0},
    {244, 0x4b, 1, PIXELS(632), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005b20[] = {
    {307, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(224), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, (s32)gData_02005514, PIXELS(264), 0, PIXELS(424), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_SOUTH + FACING_STEP, 1, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(334), 0, PIXELS(317), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(284), 0, PIXELS(328), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(246), 0, PIXELS(318), FACING_SOUTH + FACING_STEP, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005be0[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(676), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(676), FACING_SOUTH, 2, 1},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(376), FACING_SOUTH, 2, 1},
    {21, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(664), FACING_SOUTH, 3, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(712), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005634, PIXELS(439), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005634, PIXELS(359), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200564c, PIXELS(412), 0, PIXELS(324), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005664, PIXELS(90), 0, PIXELS(230), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005664, PIXELS(174), 0, PIXELS(72), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02005d48[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02005d54[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_020020c8)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_020020d8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(Func_020020c8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(Func_020020d8)},
    {TRIGGER_STEP, 10, 0x211, EVENT(Func_02003584)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(Func_0200393c)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(Func_020039a8)},
    {TRIGGERS_END},
};

s16 gData_02005de4[] = {
    1, 0, 1, -1, 1, 0, 49, 0,
    2, -1, 2, 0, 33, 0, 3, -1,
    3, 0, 49, 0, 4, -1, 4, 0,
    -1, -1, 0, 0, 0, 0,
};

struct Trigger gData_02005e20[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x304, EVENT(Func_020021b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x304, EVENT(Func_020021dc)},
    {0x602, 10, 0x304, EVENT(Func_02002248)},
    {0xc602, 11, 0x304, EVENT(Func_02002248)},
    {0x8602, 12, 0x304, EVENT(Func_02002248)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02001d00)},
    {TRIGGER_STEP, 6, 0x92d, EVENT(Func_020026cc)},
    {TRIGGERS_END},
};

struct Trigger gData_02005e8c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02002120)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02002154)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_02002120)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(Func_02002154)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_02003564)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(Func_02003574)},
    {TRIGGERS_END},
};

struct Trigger gData_02005ef8[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02005f10[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Func_02000710)},
    {TRIGGERS_END},
};

struct Trigger gData_02005f40[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x20f, 0},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Func_02002674)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Func_0200269c)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldItSeemsIMayNotHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIsItReallyPossibleForAnyone)},
    {TRIGGERS_END},
};

struct Trigger gData_02005fc4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x4602, 6, CONDITION_ALWAYS, EVENT(Func_02002014)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKandoreanTemple13))},
    {TRIGGER_STEP, 20, 0x230, EVENT(Func_020004a8)},
    {0x4602, 4, 0x212, EVENT(Func_020022c0)},
    {0xc602, 5, 0x212, EVENT(Func_020022c0)},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS, EVENT(Func_02001d20)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS,
     EVENT(Func_02002314)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_020027b4)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_020027d4)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_020027f4)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_02002814)},
    {0x50008905, 34, CONDITION_ALWAYS, EVENT(Func_02002834)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02001d40)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02001db4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1033, 0x845, EVENT(Func_02001e30)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgFieldIDoNotUnderstandMasterPoi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1034, 0x846, EVENT(Func_02001e44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIWasHopingToHearMaster)},
    {TRIGGER_TALK, 11, 0x849, MSGNO(MsgFieldIHopeToEnterTheTemple)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldHeyWhatsAnOutsiderDoingOn)},
    {TRIGGER_TALK, 12, 0x849, MSGNO(MsgFieldPleaseDontBreakMyConcentrationI)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldLooksLikeNoneOfOurBrothers)},
    {TRIGGER_TALK, 13, 0x849, MSGNO(MsgFieldIAmNotJustMeditatingI)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFieldSomedayImGoingToBeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x849,
     MSGNO(MsgFieldIStillCantBelieveThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldGahIKeptMyEyesShut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x849,
     MSGNO(MsgFieldMyMindIsEmptyMyBody)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldIveBeenMeditatingAllDayBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x849,
     MSGNO(MsgFieldEmptyTheMindToLightenThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldWheneverTheMasterGoesToTown)},
    {TRIGGERS_END},
};

// clang-format off
struct Trigger gData_0200615c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {0x202, 8, CONDITION_ALWAYS, EVENT(Func_02001e58)},
    {0xc602, 9, CONDITION_ALWAYS, EVENT(Func_02001f7c)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x20f, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, 0x210, 0},
    {TRIGGER_TALK, 13, 0x849, MSGNO(MsgFieldHeWentInBeforeMeWhat)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_02001e0c)},
    {TRIGGER_TALK, 14, 0x849, MSGNO(MsgFieldEmptyMindEmptyBodyByClearing)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFieldIHearThatSomeonePassedThe)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgFieldWhenIAwokeILearnedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x849, MSGNO(MsgFieldTheresNoWayImRootingFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFieldTheMasterSeemedSoDisappointedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x849, MSGNO(MsgFieldArrghIWasSoCloseI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgFieldIKnewItItShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgFieldBeforeIPassedOutISaw)},
    {TRIGGER_EXAMINE, 16, 0x84a, EVENT(Func_020023e8)},
    {TRIGGER_TALK, 12, 0x84a, MSGNO(MsgFieldTakeThePowerOfLashIt)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWishToPracticeThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFieldHoHoIFeelTheStrength)},
    {TRIGGERS_END},
};
// clang-format on
