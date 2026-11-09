/// @file
/// Room entry for the five Gaia Rock rooms, chosen by scene, then by entrance
/// in the first room.
#include "version.h"
#include "event_inline.h"

#if TLA_PATCH_1

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "io_reg.h"

#define Test(flag) Event_GetFlag(flag)
#define C1(f, a) Call1((void (*)())f, a)
#define C2(f, a, b) Call2((void (*)())f, a, b)
#define C3(f, a, b, c) Call3((void (*)())f, a, b, c)
#define C4(f, a, b, c, d) Call4((void (*)())f, a, b, c, d)
#define C6(f, a, b, c, d, e, g) Call6((void (*)())f, a, b, c, d, e, g)

struct GaiaRockAnimation {
  u8 unk00[18];
  u16 angle;
};

struct GaiaRockInsideSceneWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

extern char SceneId_Ov681Room1;
extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
extern char MsgFieldYouYouCannotFightThe[];
extern char MsgFieldAtLastTheSerpentHas[];
extern char MsgFieldLikeISaidTheSerpent[];
extern char MsgFieldItLooksLikeTheDragon[];
extern s16 gData_0200530c[][2];
extern u8 gData_02005360[];
extern u8 gData_0200539c[];
extern u8 gData_020053d8[];
extern u8 gData_0200578c[];
extern u8 gData_02005e28[];

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
s32 __modsi3(s32 numerator, s32 denominator);
void Func_02002d30(void);
void Func_02002ac8();
void Func_02002cb4();
void Func_02001c9c(void);
void Func_0200042c(void);
void Func_02004a5c();
void Func_02004d00(void);
void Func_02004d9c();
void Func_02004dac();
void Func_02004dbc();
void Func_02004dd4(void);
void Func_02004ddc(void);
void Func_02004e3c();
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004f54(void);
void Func_02004f5c();
void Func_02004f64();
void Func_02004fc4();
void Func_02004fec();
void Func_02005004();
void Actor_SetAnimationSpeed();
void Actor_SetSingleChildFlags();
void Animation_ApplyChildValues();
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

s32 Func_02002f30(void) {
  s32 message;
  struct MainRoutineWork *work;
  /* FAKEMATCH(pin): room in r8, which leaves r7 to the owner pointer; as a
   * plain local, 82 of 1969 instructions differ in tla-en. */
  register struct MainRoutineWork *room asm("r8");
  s16 *entrance;
  s16 *early;

  work = &gMainRoutineWork;
  ((struct GaiaRockInsideSceneWork *)Ram_HeapSlots->eventWork)->screenEffect =
      516;
  if (work->scene == (s32)&SceneId_Ov681Room1) {
    early = &work->entrance;
    if (work->entrance > 8) {
      C2(Actor_SetSingleChildFlags, Actor_GetById(8), 0);
      Func_02002d30();
    }
    if (*early == 20) {
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C1(Actor_Stop, 9);
      C1(Actor_Stop, 10);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 60);
      {
        s32 ending = 20;
        if (Test(0x307)) {
          C1(GameFlag_ClearBit, 0x307);
          C1(GameFlag_SetBit, 0x9bc);
          ending = 21;
        }
        if (Test(0x308)) {
          C1(GameFlag_ClearBit, 0x308);
          C1(GameFlag_SetBit, 0x9bd);
          ending = 22;
        }
        if (Test(0x309)) {
          C1(GameFlag_ClearBit, 0x309);
          C1(GameFlag_SetBit, 0x9be);
          ending = 23;
        }
        if (Test(0x30a)) {
          C1(GameFlag_ClearBit, 0x30a);
          C1(GameFlag_SetBit, 0x9bf);
          ending = 24;
        }
        C1(Sound_Play, 564);
        Func_02002d30();
        C1(Event_Wait, 15);
        Actor_SetModeById(8, 3);
        C1(Sound_Play, 530);
        C3(Map_SetScreenShake, 0x20000, 0x20000, 0x10000);
        C3(Map_SetScreenShake, -1, -1, 0xe666);
        C1(Event_Wait, 120);
        Actor_SetModeById(8, 1);
        C1(Event_Wait, 60);
        C1(GameFlag_SetBit, 0x109);
        C1(Import_EventSetExitRequest, ending);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 9) <= 1) {
      C1(GameFlag_SetBit, 0x144);
      if (Test(0x927)) {
        C2(Actor_SetModeById, 8, 4);
        C6(Func_0802b450, 85, 72, 5, 2, 30, 19);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      }
      if (Test(0x9ee) && !Test(0x9ba))
        C3(Actor_SetHorizontalPositionWithTerrain, 12, 0x1d00000, 0x1480000);
    }
    room = &gMainRoutineWork;
    entrance = &room->entrance;
    if (*entrance == 9) {
      s32 none;
      s32 found;

      if (Test(0x927))
        goto end;
      C1(Sound_Play, 53);
      found = FieldPartyInventory_FindOwner(451);
      none = -1;
      if (found != none) {
        s32 flag;
        struct Actor *dragon;
        struct Actor *partner;
        s32 width;
        s32 height;
        s32 mask;
        s32 fast;
        s32 i;
        s32 dx;
        s32 dz;
        s32 movement;
        struct Actor *object;
        struct Actor *shards[4];

        flag = Test(0x9ac);
        if (flag != 0) {
          C2(Actor_SetModeById, 8, 2);
          C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
          C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1b00000, 0x1400000);
          C2(Actor_SetSpritePriority, 10, 3);
          C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
          C2(Actor_SetModeById, 10, 5);
          goto end;
        }
        dragon = Actor_GetById(10);
        partner = Actor_GetById(8);
        C1(Sound_Play, 390);
        C1(GameFlag_SetBit, 0x9ac);
        C1(GameFlag_SetBit, 0x9de);
        Func_02004e5c();
        C1(Event_ResolvePendingActions, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C2(Actor_SetMovementTypeById, 9, 1);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
        C2(Actor_SetMovementTypeById, 10, 1);
        C3(Actor_ArmCallback, 10, 0xe000, 0);
        C2(Actor_SetModeById, 8, 2);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        C4(Camera_MoveWithinMapBounds, 0x1e80000, none, 0x1800000, 1);
        Func_02004f54();
        C3(Func_080d47b4, 10, 264, 0);
        C1(Event_Wait, 40);
        C1(Event_SetMessageId, (s32)MsgFieldAtLastTheSerpentHas);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Motion_SetModeAndWaitAnimation, 10, 3);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Actor_SetModeById, 10, 8);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        dragon->speedLimit = 0x18000;
        C3(Actor_ResetAndSetPosition, 10, 488, 328);
        C1(Event_Wait, 5);
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        dragon->velocityY = 0x60000;
        C1(Event_Wait, 10);
        dragon->movementState = 1;
        partner->movementState = 1;
        C2(Actor_SetAnimationSpeed, dragon, 0);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C2(Animation_ApplyChildValues, dragon, 6);
        C2(Animation_ApplyChildValues, partner, 6);
        width = 30;
        height = 19;
        C6(Func_0802b450, 85, 76, 5, 2, width, height);
        C1(Sound_Play, 572);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
        C1(BattleEffect_StartBufferInterpolation, 10);
        C1(Event_Wait, 10);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x10000, 0);
        C1(BattleEffect_StartBufferInterpolation, 10);
        C1(Event_Wait, 10);
        C1(Event_Wait, 60);
        C2(Animation_ApplyChildValues, dragon, 0);
        C2(Animation_ApplyChildValues, partner, 0);
        C6(Func_0802b450, 85, 74, 5, 2, width, height);
        dragon->movementState = 0;
        partner->movementState = 0;
        C2(Actor_SetAnimationSpeed, dragon, 16);
        C2(Actor_SetAnimationSpeed, partner, 16);
        mask = 254;
        Actor_GetById(10)->actionFlags &= mask;
        C3(Actor_ResetAndSetPositionInMode2, 10, 456, 360);
        fast = 0x40000;
        dragon->velocityY = fast;
        C2(Func_080d489c, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C3(Func_080d47b4, 10, 257, 20);
        C1(Event_Wait, 30);
        Actor_SetModeById(10, 8);
        C1(Event_Wait, 10);
        dragon->speedLimit = 0x18000;
        C3(Actor_ResetAndSetPosition, 10, 488, 328);
        C1(Event_Wait, 14);
        dragon->velocityY = 0x60000;
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        C1(Sound_Play, 572);
        {
          struct Actor *a = Actor_GetById(10);
          mask &= a->actionFlags;
          a->actionFlags = mask;
        }
        C3(Actor_ResetAndSetPositionInMode2, 10, 456, 360);
        dragon->velocityY = fast;
        C2(Func_080d489c, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C2(Event_ShowMessageAndWait, 10, 0);
        C1(Event_Wait, 15);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C1(Event_Wait, 15);
        C1(Sound_Play, 215);
        object = FieldActor_Create(285, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSingleChildFlags, object, 0);
        C2(Func_02004dac, object, gData_0200539c);
        C2(Func_02004d9c, object, 1);
        C2(Animation_ApplyChildValues, object, 6);
        C3(Actor_Jump, 10, 2, 0);
        C1(Event_Wait, 5);
        C3(Func_080d47b4, 10, 258, 0);
        C1(Event_Wait, 55);
        C1(Event_Wait, 5);
        C1(Sound_Play, 136);
        object = FieldActor_Create(412, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSingleChildFlags, object, 0);
        C2(Func_02004dac, object, gData_02005360);
        ((struct GaiaRockAnimation *)object->animation)->angle = 0x2000;
        object->speedLimit = 0x18000;
        *(u8 *)&object->motionFlags = 0;
        object->targetX = 0x1c80000;
        object->targetY = -0x200000;
        object->targetZ = 0x1660000;
        object->scaleX = 0x6000;
        object->scaleY = 0x6000;
        C1(Event_Wait, 10);
        C2(Actor_SetAnimationSpeed, partner, 16);
        C1(Event_Wait, 10);
        C1(Sound_Play, 133);
        object->x = 0;
        object->z = 0;
        C3(Map_SetScreenShake, fast, fast, 0x10000);
        for (i = 0; i < 4; i++) {
          dx = -8;
          if (i > 1)
            dx = 8;
          dz = -8;
          if ((i & 1) == 0)
            dz = 8;
          shards[i] = (struct Actor *)Import_ActorCreate(403, 0x1c80000 + (dx << 16), -0x200000,
                                         0x1660000 + (dz << 16));
          C2(Func_02004d9c, shards[i], 2);
          C2(Actor_SetSingleChildFlags, shards[i], 0);
          C2(Func_02004dac, shards[i], gData_0200539c);
        }
        dragon->velocityY = 0xa0000;
        Actor_GetById(10)->actionFlags &= 254;
        C3(Actor_ResetAndSetPositionInMode2, 10, 432, 320);
        C1(Event_Wait, 10);
        C3(Map_SetScreenShake, -1, -1, 0xe666);
        Actor_SetModeById(10, 5);
        C1(Event_Wait, 30);
        C2(Func_02004f5c, 10, 1);
        Func_02004f54();
        movement = (s32)gData_02005e28;
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Actor_RefreshSelectorById, 10);
        C1(Event_Wait, 20);
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Event_Wait, 20);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        C2(Camera_FollowActor, gMainRoutineWork.currentOwner, 1);
        Func_02004f54();
        C2(Actor_SetSpritePriority, 10, 3);
        C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
        Func_02004e64();
        goto end;
      } else {
        if (Test(0x109))
          goto end;
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1d80000, 0x1c80000);
        C2(Actor_SetMovementTypeById, 10, gData_0200578c);
        goto end;
      }
    } else if (*entrance == 10) {
      s32 flag;
      s32 scale;
      struct Actor *dragon;
      s32 slow;
      s32 *owner;
      struct Actor *other;
      struct Actor *object;
      volatile u16 alpha;
      s32 i;

      C1(Sound_Play, 7);
      flag = Test(0x927);
      if (flag != 0)
        goto end;
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C1(GameFlag_SetBit, 0x927);
      C1(Event_SetMessageId, (s32)MsgFieldItLooksLikeTheDragon);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      Actor_SetModeById(8, 3);
      Actor_GetById(11)->speedLimit = scale = 0x10000;
      slow = 0x8000;
      dragon = Actor_GetById(10);
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x1b80000, 0x1380000);
      owner = &room->currentOwner;
      C3(Actor_SetHorizontalPositionWithTerrain, *owner, 0x1c80000, 0x1480000);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1e80000, 0x1a80000);
      C3(Actor_ArmCallback, *owner, 0, 0);
      C3(Actor_ArmCallback, 11, 0, 0);
      C3(Actor_ArmCallback, 10, 0xc000, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C3(Func_080d47b4, 11, 258, 30);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 10, 0);
      Actor_ShowEmote(*owner, 256, 0);
      C3(Func_080d47b4, 11, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0x4000, 0);
      C3(Actor_ArmCallback, 11, 0x4000, 0);
      C1(Event_Wait, 30);
      C2(Func_02004f5c, 10, 1);
      Func_02004f54();
      C2(Actor_StartRepeatedMotion, 10, 2);
      C2(Camera_FollowActor, 10, 1);
      C3(Actor_ResetAndSetPositionInMode2, 10, 488, 376);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 20);
      C3(Actor_ResetAndSetPositionInMode2, 10, 472, 360);
      C1(Actor_WaitForMoveById, 10);
      C3(Actor_SetAngleToward, *owner, 10, 0);
      C3(Actor_SetAngleToward, 11, 10, 0);
      C3(Actor_SetAngleToward, 10, 8, 0);
      C4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      C2(Actor_SetModeById, 10, 8);
      C2(Event_ShowMessageAndWait, 10, 0);
      dragon->speedLimit = 0x18000;
      C3(Actor_ResetAndSetPosition, 10, 488, 328);
      C1(Event_Wait, 5);
      Actor_SetModeById(10, 6);
      C1(Event_Wait, 10);
      dragon->velocityY = 0x60000;
      C1(Event_Wait, 10);
      C1(Sound_Play, 572);
      dragon->movementState = 1;
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C1(Sound_Play, 530);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 60);
      C2(Actor_SetModeById, 8, 4);
      C6(Func_0802b450, 85, 72, 5, 2, 30, 19);
      dragon->y = -0x200000;
      dragon->velocityY = 0;
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
      C2(Actor_SetModeById, 10, 1);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C3(Actor_SetAngleToward, 10, *owner, 0);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C2(Event_ShowMessageAndWait, 10, 0);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_ResetAndSetPositionInMode2, 10, 488, 392);
      C2(Actor_StartRepeatedMotion, 11, 1);
      C1(Event_Wait, 5);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_SetModeById, 10, 0);
      C3(Actor_SetAngleToward, 10, 11, 0);
      C1(Event_Wait, 15);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_ResetAndSetPositionInMode2, 10, 488, 536);
      C2(Camera_FollowActor, *owner, 1);
      C3(Actor_SetAngleToward, *owner, 11, 0);
      C3(Actor_SetAngleToward, 11, *owner, 0);
      C1(Event_Wait, 30);
      C2(Motion_SetModeAndWaitAnimation, 11, 3);
      C2(Actor_SetModeById, 11, 2);
      {
        struct Actor *ownerActor = Actor_GetById(*owner);

        if (ownerActor != NULL)
          Actor_ResetAndSetPosition(11, ownerActor->x >> 16,
                                    ownerActor->z >> 16);
      }
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C3(Actor_ResetAndSetPositionInMode2, *owner, 488, 376);
      C1(Actor_WaitForMoveById, *owner);
      C3(Map_SetScreenShake, 0x50000, 0x50000, scale);
      C1(Sound_Play, 107);
      C1(Event_Wait, 30);
      C3(Func_080d47b4, *owner, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0xc000, 0);
      C1(Event_Wait, 15);
      other = Actor_GetById(12);
      alpha = REG_BLDALPHA;
      C3(Map_SetScreenShake, 0xa0000, 0xa0000, scale);
      C1(Sound_Play, 107);
      Func_02004ddc();
      alpha = *(u8 *)&alpha;
      while (alpha != 0) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha = alpha + 0xffff;
        C1(wait, 1);
      }
      REG_DISPCNT &= 0xfdff;
      C1(Event_Wait, 30);
      {
        s32 x;
        /* FAKEMATCH(pin): z in r5 and x in r6; as a plain local, 12 of 1969
         * instructions differ in tla-en. */
        register s32 z asm("r5");

        x = 0x1d00000;
        z = 0x15c0000;
        object = OverlayObject_PrepareObject(x, -0x200000, z, 323);
        Actor_SetHorizontalPositionWithTerrain(12, x, z);
      }
      for (i = 0; i < 20; i++) {
        if (i % 5 == 0)
          Func_02001c9c();
        other->z += -0x10000;
        C1(Event_Wait, 2);
      }
      C1(GameFlag_SetBit, 0x9ee);
      C3(Map_SetScreenShake, -1, -1, 0xe666);
      object->x = 0;
      object->z = 0;
      C1(Event_Wait, 1);
      C1(Func_02004dbc, object);
      C1(Sound_Play, 390);
      C1(Event_Wait, 30);
      REG_DISPCNT |= 0xf00;
      REG_BLDALPHA = alpha | 0x1000;
      REG_BLDCNT = 0x3f42;
      while (alpha != 8) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha++;
        C1(wait, 1);
      }
      Func_02004dd4();
      Func_02004e64();
    } else if (*entrance == 11) {
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c00000, 0x1580000);
      {
        s32 *lead = &room->currentOwner;

        C3(Actor_SetAngleToward, 10, *lead, 0);
        Value2((s32 (*)())Actor_SetModeById, *lead, 19);
      }
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      message = (s32)MsgFieldYouYouCannotFightThe;
      goto leave;
    } else if (*entrance == 12) {
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1ae0000, 0x1400000);
      C2(Actor_SetModeById, 10, 5);
      C2(Actor_SetModeById, room->currentOwner, 19);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 30);
      Value2((s32 (*)())Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 30);
      message = (s32)MsgFieldLikeISaidTheSerpent;
    leave:
      C1(Event_SetMessageId, message);
      Event_ShowMessageAndWait(10, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      C2(Func_02004f64, (s16)room->savedScene, (s16)room->savedEntrance);
      C2(Func_02005004, room->scene, *entrance);
      Func_02004e64();
    }
  } else if (work->scene == (s32)&SceneId_Ov681Room2) {
    s32 mask = 32;
    s32 low;
    s32 high;
    struct Actor *object = Actor_Get(8);

    object->priorityFlags |= mask;
    C2(Func_02004e3c, object, 3);
    object = Actor_Get(9);
    object->priorityFlags |= mask;
    C2(Func_02004e3c, object, 3);
    C2(Func_02002ac8, 8, 12);
    C2(Func_02002ac8, 9, 12);
    if (Test(0xa62)) {
      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1b80000, 0x2b00000);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2280000, 0x2b00000);
    } else {
      s32 height = 40;

      C6(Map_CopyCellsRect, 88, 40, 1, 2, 89, height);
      C6(Map_CopyCellsRect, 99, 40, 1, 2, 100, height);
    }
    low = FieldGameFlag_GetByte(0x210) & 15;
    high = (u8)(FieldGameFlag_GetByte(0x210) >> 4);
    if (low != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 10,
         gData_0200530c[low][0] << 19, gData_0200530c[low][1] << 19);
    if ((s8)high != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 11,
         gData_0200530c[(s8)high][0] << 19, gData_0200530c[(s8)high][1] << 19);
  } else if (work->scene == (s32)&SceneId_Ov681Room4) {
    if (Test(0xa63))
      C1(Func_02002cb4, 1);
  } else if (work->scene == (s32)&SceneId_Ov681Room3) {
    struct Actor *object;
    struct Actor *other;

    object = Actor_GetById(10);
    C2(Func_02004e3c, object, 3);
    C2(Func_02002ac8, 8, 10);
    C2(Func_02002ac8, 9, 11);
    if (!Test(0x9bc))
      C6(Map_CopyCellsRect, 25, 77, 1, 15, 26, 77);
    if (Test(0x9bd)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1e00000, 0x1600000);
      width = 28;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 27, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 29, height);
    }
    if (Test(0x9be)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2600000, 0x1600000);
      width = 36;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 35, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 37, height);
    }
    if (Test(0x9bf)) {
      C6(Func_0802b450, 61, 0, 3, 3, 37, 26);
      C6(Map_CopyCellAttributeRect, 36, 23, 1, 1, 38, 25);
    }
    if (work->entrance == 6) {
      s32 frames = 60;

      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      Event_StartScreenTransition();
      C1(Event_Wait, 15);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x2a00000, 0x1800000);
      C2(Actor_SetSingleChildFlags, Actor_GetById(11), 0);
      object = Actor_GetById(11);
      other = FieldActor_Create(324, object->x, object->y, object->z);
      if (other != NULL) {
        C2(Actor_SetSingleChildFlags, other, 0);
        C2(Func_02004dac, other, gData_020053d8);
        C2(Func_02004d9c, other, 1);
        C2(Animation_ApplyChildValues, other, 14);
        ((u8 *)other->animation)[9] |= 12;
        other->scaleX = 0x13333;
        other->scaleY = 0x13333;
      }
      object->y += 0x800000;
      for (; frames != 0; frames--) {
        C1(wait, 1);
        if (object->velocityY > -1)
          break;
      }
      C1(Sound_Play, 188);
      C1(Event_Wait, 30);
      C1(GameFlag_SetBit, 0x109);
      C1(Import_EventSetExitRequest, 3);
    }
    if (!Test(0x9c4))
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
    if (!Test(0x109) && gMainRoutineWork.entrance == 3)
      Func_0200042c();
  } else if (work->scene == (s32)&SceneId_Ov681Room5) {
    C2(Func_02004fec, 10, 1);
    C2(Func_02002ac8, 8, 9);
    if (work->entrance == 1 || work->entrance == 3 || work->entrance == 5)
      Func_02004d00();
    if (Test(0x303))
      Func_02004a5c(11, 0x1800000, 0x4b0000, 0x2a80000);
    if (Test(0x304))
      Func_02004a5c(12, 0x1cf0000, 0x330000, 0x2a80000);
    if (Test(0x305))
      Func_02004a5c(13, 0x2350000, 0x370000, 0x2a80000);
    if (Test(0x9bc)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1800000, 0x1700000);
      width = 25;
      C6(Map_CopyCellsRect, 1, 85, 1, 4, width, 85);
      height = 23;
      C6(Map_CopyCellAttributeRect, 27, 15, 2, 1, height, height);
      C6(Map_CopyCellAttributeRect, 25, 25, 1, 1, width, height);
    }
    C2(Actor_SetSingleChildFlags, Actor_GetById(15), 0);
    if (Test(0x9c4)) {
      s32 height = 20;

      C6(Func_0802b450, 117, 20, 7, 8, 104, height);
      C6(Func_0802b450, 111, 12, 1, 6, 111, height);
      C6(Map_CopyCellAttributeRect, 49, 84, 8, 8, 40, 84);
      C3(Actor_SetHorizontalPositionWithTerrain, 15, 0, 0);
    }
  }
end:
  C1(Func_02004fc4, 0);
  return 0;
}

#else

// The Japanese release's older form: room 1's entrances 11 and 12 end without
// Func_02005004, room 3 has no flag 0x109 check calling Func_0200042c, and the
// room pointer is a plain local.

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "io_reg.h"

#define Test(flag) Event_GetFlag(flag)
#define C1(f, a) Call1((void (*)())f, a)
#define C2(f, a, b) Call2((void (*)())f, a, b)
#define C3(f, a, b, c) Call3((void (*)())f, a, b, c)
#define C4(f, a, b, c, d) Call4((void (*)())f, a, b, c, d)
#define C6(f, a, b, c, d, e, g) Call6((void (*)())f, a, b, c, d, e, g)

struct GaiaRockAnimation {
  u8 unk00[18];
  u16 angle;
};

struct GaiaRockInsideSceneWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

extern char SceneId_Ov681Room1;
extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
extern char MsgFieldYouYouCannotFightThe[];
extern char MsgFieldAtLastTheSerpentHas[];
extern char MsgFieldLikeISaidTheSerpent[];
extern char MsgFieldItLooksLikeTheDragon[];
extern s16 gData_0200530c[][2];
extern u8 gData_02005360[];
extern u8 gData_0200539c[];
extern u8 gData_020053d8[];
extern u8 gData_0200578c[];
extern u8 gData_02005e28[];

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
s32 __modsi3(s32 numerator, s32 denominator);
void Func_02002d30(void);
void Func_02002ac8();
void Func_02002cb4();
void Func_02001c9c(void);
void Func_02004a5c();
void Func_02004d00(void);
void Func_02004d9c();
void Func_02004dac();
void Func_02004dbc();
void Func_02004dd4(void);
void Func_02004ddc(void);
void Func_02004e3c();
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004f54(void);
void Func_02004f5c();
void Func_02004f64();
void Func_02004fc4();
void Func_02004fec();
void Func_02005004();
void Actor_SetAnimationSpeed();
void Actor_SetSingleChildFlags();
void Animation_ApplyChildValues();
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

s32 Func_02002f30(void) {
  struct MainRoutineWork *room;
  struct MainRoutineWork *work;
  s16 *entrance;
  s16 *early;

  work = &gMainRoutineWork;
  ((struct GaiaRockInsideSceneWork *)Ram_HeapSlots->eventWork)->screenEffect =
      516;
  if (work->scene == (s32)&SceneId_Ov681Room1) {
    early = &work->entrance;
    if (work->entrance > 8) {
      C2(Actor_SetSingleChildFlags, Actor_GetById(8), 0);
      Func_02002d30();
    }
    if (*early == 20) {
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C1(Actor_Stop, 9);
      C1(Actor_Stop, 10);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 60);
      {
        s32 ending = 20;
        if (Test(0x307)) {
          C1(GameFlag_ClearBit, 0x307);
          C1(GameFlag_SetBit, 0x9bc);
          ending = 21;
        }
        if (Test(0x308)) {
          C1(GameFlag_ClearBit, 0x308);
          C1(GameFlag_SetBit, 0x9bd);
          ending = 22;
        }
        if (Test(0x309)) {
          C1(GameFlag_ClearBit, 0x309);
          C1(GameFlag_SetBit, 0x9be);
          ending = 23;
        }
        if (Test(0x30a)) {
          C1(GameFlag_ClearBit, 0x30a);
          C1(GameFlag_SetBit, 0x9bf);
          ending = 24;
        }
        C1(Sound_Play, 564);
        Func_02002d30();
        C1(Event_Wait, 15);
        Actor_SetModeById(8, 3);
        C1(Sound_Play, 530);
        C3(Map_SetScreenShake, 0x20000, 0x20000, 0x10000);
        C3(Map_SetScreenShake, -1, -1, 0xe666);
        C1(Event_Wait, 120);
        Actor_SetModeById(8, 1);
        C1(Event_Wait, 60);
        C1(GameFlag_SetBit, 0x109);
        C1(Import_EventSetExitRequest, ending);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 9) <= 1) {
      C1(GameFlag_SetBit, 0x144);
      if (Test(0x927)) {
        C2(Actor_SetModeById, 8, 4);
        C6(Func_0802b450, 85, 72, 5, 2, 30, 19);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      }
      if (Test(0x9ee) && !Test(0x9ba))
        C3(Actor_SetHorizontalPositionWithTerrain, 12, 0x1d00000, 0x1480000);
    }
    room = &gMainRoutineWork;
    entrance = &room->entrance;
    if (*entrance == 9) {
      s32 none;
      s32 found;

      if (Test(0x927))
        goto end;
      C1(Sound_Play, 53);
      found = FieldPartyInventory_FindOwner(451);
      none = -1;
      if (found != none) {
        s32 flag;
        struct Actor *dragon;
        struct Actor *partner;
        s32 width;
        s32 height;
        s32 mask;
        s32 fast;
        s32 i;
        s32 dx;
        s32 dz;
        s32 movement;
        struct Actor *object;
        struct Actor *shards[4];

        flag = Test(0x9ac);
        if (flag != 0) {
          C2(Actor_SetModeById, 8, 2);
          C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
          C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1b00000, 0x1400000);
          C2(Actor_SetSpritePriority, 10, 3);
          C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
          C2(Actor_SetModeById, 10, 5);
          goto end;
        }
        dragon = Actor_GetById(10);
        partner = Actor_GetById(8);
        C1(Sound_Play, 390);
        C1(GameFlag_SetBit, 0x9ac);
        C1(GameFlag_SetBit, 0x9de);
        Func_02004e5c();
        C1(Event_ResolvePendingActions, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C2(Actor_SetMovementTypeById, 9, 1);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
        C2(Actor_SetMovementTypeById, 10, 1);
        C3(Actor_ArmCallback, 10, 0xe000, 0);
        C2(Actor_SetModeById, 8, 2);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        C4(Camera_MoveWithinMapBounds, 0x1e80000, none, 0x1800000, 1);
        Func_02004f54();
        C3(Func_080d47b4, 10, 264, 0);
        C1(Event_Wait, 40);
        C1(Event_SetMessageId, (s32)MsgFieldAtLastTheSerpentHas);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Motion_SetModeAndWaitAnimation, 10, 3);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Actor_SetModeById, 10, 8);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        dragon->speedLimit = 0x18000;
        C3(Actor_ResetAndSetPosition, 10, 488, 328);
        C1(Event_Wait, 5);
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        dragon->velocityY = 0x60000;
        C1(Event_Wait, 10);
        dragon->movementState = 1;
        partner->movementState = 1;
        C2(Actor_SetAnimationSpeed, dragon, 0);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C2(Animation_ApplyChildValues, dragon, 6);
        C2(Animation_ApplyChildValues, partner, 6);
        width = 30;
        height = 19;
        C6(Func_0802b450, 85, 76, 5, 2, width, height);
        C1(Sound_Play, 572);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
        C1(BattleEffect_StartBufferInterpolation, 10);
        C1(Event_Wait, 10);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x10000, 0);
        C1(BattleEffect_StartBufferInterpolation, 10);
        C1(Event_Wait, 10);
        C1(Event_Wait, 60);
        C2(Animation_ApplyChildValues, dragon, 0);
        C2(Animation_ApplyChildValues, partner, 0);
        C6(Func_0802b450, 85, 74, 5, 2, width, height);
        dragon->movementState = 0;
        partner->movementState = 0;
        C2(Actor_SetAnimationSpeed, dragon, 16);
        C2(Actor_SetAnimationSpeed, partner, 16);
        mask = 254;
        Actor_GetById(10)->actionFlags &= mask;
        C3(Actor_ResetAndSetPositionInMode2, 10, 456, 360);
        fast = 0x40000;
        dragon->velocityY = fast;
        C2(Func_080d489c, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C3(Func_080d47b4, 10, 257, 20);
        C1(Event_Wait, 30);
        Actor_SetModeById(10, 8);
        C1(Event_Wait, 10);
        dragon->speedLimit = 0x18000;
        C3(Actor_ResetAndSetPosition, 10, 488, 328);
        C1(Event_Wait, 14);
        dragon->velocityY = 0x60000;
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        C1(Sound_Play, 572);
        {
          struct Actor *a = Actor_GetById(10);
          mask &= a->actionFlags;
          a->actionFlags = mask;
        }
        C3(Actor_ResetAndSetPositionInMode2, 10, 456, 360);
        dragon->velocityY = fast;
        C2(Func_080d489c, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C2(Event_ShowMessageAndWait, 10, 0);
        C1(Event_Wait, 15);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C1(Event_Wait, 15);
        C1(Sound_Play, 215);
        object = FieldActor_Create(285, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSingleChildFlags, object, 0);
        C2(Func_02004dac, object, gData_0200539c);
        C2(Func_02004d9c, object, 1);
        C2(Animation_ApplyChildValues, object, 6);
        C3(Actor_Jump, 10, 2, 0);
        C1(Event_Wait, 5);
        C3(Func_080d47b4, 10, 258, 0);
        C1(Event_Wait, 55);
        C1(Event_Wait, 5);
        C1(Sound_Play, 136);
        object = FieldActor_Create(412, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSingleChildFlags, object, 0);
        C2(Func_02004dac, object, gData_02005360);
        ((struct GaiaRockAnimation *)object->animation)->angle = 0x2000;
        object->speedLimit = 0x18000;
        *(u8 *)&object->motionFlags = 0;
        object->targetX = 0x1c80000;
        object->targetY = -0x200000;
        object->targetZ = 0x1660000;
        object->scaleX = 0x6000;
        object->scaleY = 0x6000;
        C1(Event_Wait, 10);
        C2(Actor_SetAnimationSpeed, partner, 16);
        C1(Event_Wait, 10);
        C1(Sound_Play, 133);
        object->x = 0;
        object->z = 0;
        C3(Map_SetScreenShake, fast, fast, 0x10000);
        for (i = 0; i < 4; i++) {
          dx = -8;
          if (i > 1)
            dx = 8;
          dz = -8;
          if ((i & 1) == 0)
            dz = 8;
          shards[i] = (struct Actor *)Import_ActorCreate(403, 0x1c80000 + (dx << 16), -0x200000,
                                         0x1660000 + (dz << 16));
          C2(Func_02004d9c, shards[i], 2);
          C2(Actor_SetSingleChildFlags, shards[i], 0);
          C2(Func_02004dac, shards[i], gData_0200539c);
        }
        dragon->velocityY = 0xa0000;
        Actor_GetById(10)->actionFlags &= 254;
        C3(Actor_ResetAndSetPositionInMode2, 10, 432, 320);
        C1(Event_Wait, 10);
        C3(Map_SetScreenShake, -1, -1, 0xe666);
        Actor_SetModeById(10, 5);
        C1(Event_Wait, 30);
        C2(Func_02004f5c, 10, 1);
        Func_02004f54();
        movement = (s32)gData_02005e28;
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Actor_RefreshSelectorById, 10);
        C1(Event_Wait, 20);
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Event_Wait, 20);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        C2(Camera_FollowActor, gMainRoutineWork.currentOwner, 1);
        Func_02004f54();
        C2(Actor_SetSpritePriority, 10, 3);
        C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
        Func_02004e64();
        goto end;
      } else {
        if (Test(0x109))
          goto end;
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1d80000, 0x1c80000);
        C2(Actor_SetMovementTypeById, 10, gData_0200578c);
        goto end;
      }
    } else if (*entrance == 10) {
      s32 flag;
      s32 scale;
      struct Actor *dragon;
      s32 slow;
      s32 *owner;
      struct Actor *other;
      struct Actor *object;
      volatile u16 alpha;
      s32 i;

      C1(Sound_Play, 7);
      flag = Test(0x927);
      if (flag != 0)
        goto end;
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C1(GameFlag_SetBit, 0x927);
      C1(Event_SetMessageId, (s32)MsgFieldItLooksLikeTheDragon);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      Actor_SetModeById(8, 3);
      Actor_GetById(11)->speedLimit = scale = 0x10000;
      slow = 0x8000;
      dragon = Actor_GetById(10);
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x1b80000, 0x1380000);
      owner = &room->currentOwner;
      C3(Actor_SetHorizontalPositionWithTerrain, *owner, 0x1c80000, 0x1480000);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1e80000, 0x1a80000);
      C3(Actor_ArmCallback, *owner, 0, 0);
      C3(Actor_ArmCallback, 11, 0, 0);
      C3(Actor_ArmCallback, 10, 0xc000, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C3(Func_080d47b4, 11, 258, 30);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 10, 0);
      Actor_ShowEmote(*owner, 256, 0);
      C3(Func_080d47b4, 11, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0x4000, 0);
      C3(Actor_ArmCallback, 11, 0x4000, 0);
      C1(Event_Wait, 30);
      C2(Func_02004f5c, 10, 1);
      Func_02004f54();
      C2(Actor_StartRepeatedMotion, 10, 2);
      C2(Camera_FollowActor, 10, 1);
      C3(Actor_ResetAndSetPositionInMode2, 10, 488, 376);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 20);
      C3(Actor_ResetAndSetPositionInMode2, 10, 472, 360);
      C1(Actor_WaitForMoveById, 10);
      C3(Actor_SetAngleToward, *owner, 10, 0);
      C3(Actor_SetAngleToward, 11, 10, 0);
      C3(Actor_SetAngleToward, 10, 8, 0);
      C4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      C2(Actor_SetModeById, 10, 8);
      C2(Event_ShowMessageAndWait, 10, 0);
      dragon->speedLimit = 0x18000;
      C3(Actor_ResetAndSetPosition, 10, 488, 328);
      C1(Event_Wait, 5);
      Actor_SetModeById(10, 6);
      C1(Event_Wait, 10);
      dragon->velocityY = 0x60000;
      C1(Event_Wait, 10);
      C1(Sound_Play, 572);
      dragon->movementState = 1;
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C1(Sound_Play, 530);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 60);
      C2(Actor_SetModeById, 8, 4);
      C6(Func_0802b450, 85, 72, 5, 2, 30, 19);
      dragon->y = -0x200000;
      dragon->velocityY = 0;
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
      C2(Actor_SetModeById, 10, 1);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(BattleEffect_StartBufferInterpolation, 10);
      C1(Event_Wait, 10);
      C3(Actor_SetAngleToward, 10, *owner, 0);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C2(Event_ShowMessageAndWait, 10, 0);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_ResetAndSetPositionInMode2, 10, 488, 392);
      C2(Actor_StartRepeatedMotion, 11, 1);
      C1(Event_Wait, 5);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_SetModeById, 10, 0);
      C3(Actor_SetAngleToward, 10, 11, 0);
      C1(Event_Wait, 15);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_ResetAndSetPositionInMode2, 10, 488, 536);
      C2(Camera_FollowActor, *owner, 1);
      C3(Actor_SetAngleToward, *owner, 11, 0);
      C3(Actor_SetAngleToward, 11, *owner, 0);
      C1(Event_Wait, 30);
      C2(Motion_SetModeAndWaitAnimation, 11, 3);
      C2(Actor_SetModeById, 11, 2);
      {
        struct Actor *ownerActor = Actor_GetById(*owner);

        if (ownerActor != NULL)
          Actor_ResetAndSetPosition(11, ownerActor->x >> 16,
                                    ownerActor->z >> 16);
      }
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C3(Actor_ResetAndSetPositionInMode2, *owner, 488, 376);
      C1(Actor_WaitForMoveById, *owner);
      C3(Map_SetScreenShake, 0x50000, 0x50000, scale);
      C1(Sound_Play, 107);
      C1(Event_Wait, 30);
      C3(Func_080d47b4, *owner, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0xc000, 0);
      C1(Event_Wait, 15);
      other = Actor_GetById(12);
      alpha = REG_BLDALPHA;
      C3(Map_SetScreenShake, 0xa0000, 0xa0000, scale);
      C1(Sound_Play, 107);
      Func_02004ddc();
      alpha = *(u8 *)&alpha;
      while (alpha != 0) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha = alpha + 0xffff;
        C1(wait, 1);
      }
      REG_DISPCNT &= 0xfdff;
      C1(Event_Wait, 30);
      {
        s32 x;
        /* FAKEMATCH(pin): z in r5 and x in r6; as a plain local, 7 of 1961
         * instructions differ in tla-ja. */
        register s32 z asm("r5");

        x = 0x1d00000;
        z = 0x15c0000;
        object = OverlayObject_PrepareObject(x, -0x200000, z, 323);
        Actor_SetHorizontalPositionWithTerrain(12, x, z);
      }
      for (i = 0; i < 20; i++) {
        if (i % 5 == 0)
          Func_02001c9c();
        other->z += -0x10000;
        C1(Event_Wait, 2);
      }
      C1(GameFlag_SetBit, 0x9ee);
      C3(Map_SetScreenShake, -1, -1, 0xe666);
      object->x = 0;
      object->z = 0;
      C1(Event_Wait, 1);
      C1(Func_02004dbc, object);
      C1(Sound_Play, 390);
      C1(Event_Wait, 30);
      REG_DISPCNT |= 0xf00;
      REG_BLDALPHA = alpha | 0x1000;
      REG_BLDCNT = 0x3f42;
      while (alpha != 8) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha++;
        C1(wait, 1);
      }
      Func_02004dd4();
      Func_02004e64();
    } else if (*entrance == 11) {
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c00000, 0x1580000);
      {
        s32 *lead = &room->currentOwner;

        C3(Actor_SetAngleToward, 10, *lead, 0);
        Value2((s32 (*)())Actor_SetModeById, *lead, 19);
      }
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_SetMessageId, (s32)MsgFieldYouYouCannotFightThe);
      Event_ShowMessageAndWait(10, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      C2(Func_02004f64, (s16)room->savedScene, (s16)room->savedEntrance);
      Func_02004e64();
    } else if (*entrance == 12) {
      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1ae0000, 0x1400000);
      C2(Actor_SetModeById, 10, 5);
      C2(Actor_SetModeById, room->currentOwner, 19);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 30);
      Value2((s32 (*)())Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 30);
      C1(Event_SetMessageId, (s32)MsgFieldLikeISaidTheSerpent);
      Event_ShowMessageAndWait(10, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      C2(Func_02004f64, (s16)room->savedScene, (s16)room->savedEntrance);
      Func_02004e64();
    }
  } else if (work->scene == (s32)&SceneId_Ov681Room2) {
    s32 mask = 32;
    s32 low;
    s32 high;
    struct Actor *object = Actor_Get(8);

    object->priorityFlags |= mask;
    C2(Func_02004e3c, object, 3);
    object = Actor_Get(9);
    object->priorityFlags |= mask;
    C2(Func_02004e3c, object, 3);
    C2(Func_02002ac8, 8, 12);
    C2(Func_02002ac8, 9, 12);
    if (Test(0xa62)) {
      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1b80000, 0x2b00000);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2280000, 0x2b00000);
    } else {
      s32 height = 40;

      C6(Map_CopyCellsRect, 88, 40, 1, 2, 89, height);
      C6(Map_CopyCellsRect, 99, 40, 1, 2, 100, height);
    }
    low = FieldGameFlag_GetByte(0x210) & 15;
    high = (u8)(FieldGameFlag_GetByte(0x210) >> 4);
    if (low != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 10,
         gData_0200530c[low][0] << 19, gData_0200530c[low][1] << 19);
    if ((s8)high != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 11,
         gData_0200530c[(s8)high][0] << 19, gData_0200530c[(s8)high][1] << 19);
  } else if (work->scene == (s32)&SceneId_Ov681Room4) {
    if (Test(0xa63))
      C1(Func_02002cb4, 1);
  } else if (work->scene == (s32)&SceneId_Ov681Room3) {
    struct Actor *object;
    struct Actor *other;

    object = Actor_GetById(10);
    C2(Func_02004e3c, object, 3);
    C2(Func_02002ac8, 8, 10);
    C2(Func_02002ac8, 9, 11);
    if (!Test(0x9bc))
      C6(Map_CopyCellsRect, 25, 77, 1, 15, 26, 77);
    if (Test(0x9bd)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1e00000, 0x1600000);
      width = 28;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 27, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 29, height);
    }
    if (Test(0x9be)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2600000, 0x1600000);
      width = 36;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 35, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 37, height);
    }
    if (Test(0x9bf)) {
      C6(Func_0802b450, 61, 0, 3, 3, 37, 26);
      C6(Map_CopyCellAttributeRect, 36, 23, 1, 1, 38, 25);
    }
    if (work->entrance == 6) {
      s32 frames = 60;

      Func_02004e5c();
      C1(Event_ResolvePendingActions, 0);
      C4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      Event_StartScreenTransition();
      C1(Event_Wait, 15);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x2a00000, 0x1800000);
      C2(Actor_SetSingleChildFlags, Actor_GetById(11), 0);
      object = Actor_GetById(11);
      other = FieldActor_Create(324, object->x, object->y, object->z);
      if (other != NULL) {
        C2(Actor_SetSingleChildFlags, other, 0);
        C2(Func_02004dac, other, gData_020053d8);
        C2(Func_02004d9c, other, 1);
        C2(Animation_ApplyChildValues, other, 14);
        ((u8 *)other->animation)[9] |= 12;
        other->scaleX = 0x13333;
        other->scaleY = 0x13333;
      }
      object->y += 0x800000;
      for (; frames != 0; frames--) {
        C1(wait, 1);
        if (object->velocityY > -1)
          break;
      }
      C1(Sound_Play, 188);
      C1(Event_Wait, 30);
      C1(GameFlag_SetBit, 0x109);
      C1(Import_EventSetExitRequest, 3);
    }
    if (!Test(0x9c4))
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
  } else if (work->scene == (s32)&SceneId_Ov681Room5) {
    C2(Func_02004fec, 10, 1);
    C2(Func_02002ac8, 8, 9);
    if (work->entrance == 1 || work->entrance == 3 || work->entrance == 5)
      Func_02004d00();
    if (Test(0x303))
      Func_02004a5c(11, 0x1800000, 0x4b0000, 0x2a80000);
    if (Test(0x304))
      Func_02004a5c(12, 0x1cf0000, 0x330000, 0x2a80000);
    if (Test(0x305))
      Func_02004a5c(13, 0x2350000, 0x370000, 0x2a80000);
    if (Test(0x9bc)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1800000, 0x1700000);
      width = 25;
      C6(Map_CopyCellsRect, 1, 85, 1, 4, width, 85);
      height = 23;
      C6(Map_CopyCellAttributeRect, 27, 15, 2, 1, height, height);
      C6(Map_CopyCellAttributeRect, 25, 25, 1, 1, width, height);
    }
    C2(Actor_SetSingleChildFlags, Actor_GetById(15), 0);
    if (Test(0x9c4)) {
      s32 height = 20;

      C6(Func_0802b450, 117, 20, 7, 8, 104, height);
      C6(Func_0802b450, 111, 12, 1, 6, 111, height);
      C6(Map_CopyCellAttributeRect, 49, 84, 8, 8, 40, 84);
      C3(Actor_SetHorizontalPositionWithTerrain, 15, 0, 0);
    }
  }
end:
  C1(Func_02004fc4, 0);
  return 0;
}

#endif
