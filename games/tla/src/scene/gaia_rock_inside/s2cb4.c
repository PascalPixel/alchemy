/// @file
/// Scene code of overlay 0x681, in ROM order, from s2cb4's offset up to the
/// next object the overlay links.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "event_inline.h"

// Starts the room where persons 8 to 16 sway: unless flag 265 is set, clears
// the sway state and places each person, then starts the sway's task.
extern u16 gSceneState[];
extern const s16 gData_0200533c[][2];

void Func_02002b04(u32 id, s8 mode);
void Func_02002b8c(void);

/// Starts the room where persons 8 to 16 sway: unless flag 265 is set, clears
/// the sway state and places each person, then starts the sway's task.
void Func_02002cb4(u8 mode) {
  u16 *state = gSceneState;
  u16 *pending = state + 1;

  if (!Event_GetFlag(265)) {
    s32 i;
    s32 id;

    *state = 0;
    *pending = 0;
    Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |= 0x40;
    for (i = 0, id = 8; id <= 16; i++, id++) {
      Actor_SetPosition(id, gData_0200533c[i][0] << 19,
                        gData_0200533c[i][1] << 19);
      Func_02002b04(id, mode);
    }
  }
  {
    s32 priority = 0x480;

    Task_AddFrame(Func_02002b8c, priority);
  }
}

// Opens the passages on each side of the hall by how many of its two pairs of
// flags (0x9bc and 0x9bd, 0x9be and 0x9bf) are set: both open the wide way, one
// a side way, and a single side opened alone also opens its connecting cells.
void Func_02002d30(void) {
  u8 a = GameFlag_Test(0x9bc);
  u8 third = GameFlag_Test(0x9be);
  s32 b = GameFlag_Test(0x9bd);
  s8 first = a;
  s8 sideThird = third;
  u8 sumLeft = b + first;
  s32 d = GameFlag_Test(0x9bf);
  u8 sumRight = sideThird + d;
  s8 left = sumLeft;
  s8 right = sumRight;

  if (left != 0) {
    if (left == 2) {
      MapGrid_CopyCellsLayered(74, 115, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(71, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(67, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(79, 110, 1, 3, 30, 85);
    } else if (first != 0) {
      MapGrid_CopyCellsLayered(66, 95, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(81, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(75, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(65, 110, 1, 3, 30, 85);
    } else {
      MapGrid_CopyCellsLayered(69, 82, 5, 12, 26, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(69, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(73, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(77, 110, 1, 3, 30, 85);
    }
  }
  if (right == 0)
    return;
  if (right == 2) {
    MapGrid_CopyCellsLayered(83, 115, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(82, 124, 1, 3, 30, 85);
  } else if (sideThird != 0) {
    MapGrid_CopyCellsLayered(76, 82, 4, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(75, 91, 1, 3, 30, 85);
  } else {
    MapGrid_CopyCellsLayered(82, 82, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(81, 91, 1, 3, 30, 85);
  }
}

#include "version.h"

#if TLA_PATCH_1

#include "call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "field.h"

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
void Func_02002ac8();
void Func_02001c9c(void);
void Func_0200042c(void);
void Func_02004a5c(s32 id, s32 x, s32 y, s32 z);
void Func_02004d00(void);
void Func_02004d9c(struct Actor *object, s32 mode);
void Func_02004dac(struct Actor *object, const u8 *script);
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
void Animation_ApplyChildValues();
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Room entry for the five Gaia Rock rooms, chosen by scene, then by entrance
/// in the first room.
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
          shards[i] = (struct Actor *)Import_ActorCreate(
              403, 0x1c80000 + (dx << 16), -0x200000, 0x1660000 + (dz << 16));
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

#include "call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "field.h"

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
void Func_02002ac8();
void Func_02001c9c(void);
void Func_02004a5c(s32 id, s32 x, s32 y, s32 z);
void Func_02004d00(void);
void Func_02004d9c(struct Actor *object, s32 mode);
void Func_02004dac(struct Actor *object, const u8 *script);
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
void Animation_ApplyChildValues();
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Room entry for the five Gaia Rock rooms, chosen by scene, then by entrance
/// in the first room.
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
          shards[i] = (struct Actor *)Import_ActorCreate(
              403, 0x1c80000 + (dx << 16), -0x200000, 0x1660000 + (dz << 16));
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

#include "iwram_call.h"
#include "map.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200495c(s32 layer, s32 keep);

void Func_02004274(s32 a0) { Func_0200495c(1, a0); }

extern u8 gData_020023c4[];
extern u32 gData_020053e4[];

u8 *Unit_Get(s32 owner);
void Func_020044f8(s32 level);
void Func_020048a0(void);
s32 Func_02004904(void);
void Func_02004dc4(void);
void Func_02004de4(u16 *script);
void Func_02000aac(void);
void Func_02000a60(void);

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

s32 Func_02004280(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room1) {
    u8 *map = (u8 *)Ram_HeapSlots->mapWork;

    if (gMainRoutineWork.entrance <= 4) {
      u8 *unit;
      s32 total;
      s32 *stage;
      s32 i;

      total = 0;
      unit = Unit_Get(gMainRoutineWork.currentOwner);

      for (i = 0; i <= 13; i++)
        total += unit[i];
      Func_020044f8(total);
      Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
      map[261] = 1;
      {
        volatile u16 control = *(volatile u16 *)0x0400000c & ~3;
        // Once-only block: keeps the sky cell window's store after its read.
        do {
        } while (0);

        *(volatile u16 *)0x0400000c = control | 1;
      }
      Func_020048a0();
      stage = (s32 *)(gData_020023c4 + 0x288);
      /* FAKEMATCH(wrapper): the call forwarded through Call3; as a direct
       * Actor_SetHorizontalPositionWithTerrain call, 2 of 229 instructions
       * differ in tla-en. */
      if (*stage == 3 && !GameFlag_Test(0x3f6))
        Actor_SetPosition(64, 0x680000, 0x580000);
      /* FAKEMATCH(wrapper): the call forwarded through Call3; as a direct
       * Actor_SetHorizontalPositionWithTerrain call, 2 of 229 instructions
       * differ in tla-en. */
      if (*stage == 9)
        Actor_SetPosition(65, 0x680000, 0x480000);
      if (GameFlag_Test(*stage + 0xa50)) {
        struct Actor *climber = Actor_GetById(8);
        u32 angle = Func_02004904();

        climber->collisionFlags = 0;
        Func_02004d9c(climber, ((angle & 0xc000) >> 14) + 2);
        climber->update = (angle & 0x4000) != 0 ? Func_02000aac : Func_02000a60;
        climber->collisionFlags = 1;
        GameFlag_SetBit(0x200);
      }
    } else if (gMainRoutineWork.entrance > 8) {
      u8 *layer;

      *(s32 *)(gData_020023c4 + 0x288) = 1;
      layer = map + 376;
      FIELD_AT(layer, u16, 40) = 127;
      FIELD_AT(layer, s32, 24) = 0;
      FIELD_AT(layer, s32, 28) = 0;
      FIELD_AT(layer, s32, 32) = 0;
      FIELD_AT(layer, s32, 36) = 0;
      FIELD_AT(layer, s32, 16) = 0x10000;
      FIELD_AT(layer, s32, 20) = 0x10000;
      FIELD_AT(layer, u16, 42) = 0x3f80;
      // Once-only block: keeps the scale defaults before the position sums.
      do {
      } while (0);
      ((struct MapLayer *)layer)->x =
          Iwram_MulQ16(((struct MapWork *)map)->viewX,
                       FIELD_AT(layer, s32, 16)) +
          FIELD_AT(layer, s32, 8);
      ((struct MapLayer *)layer)->y =
          Iwram_MulQ16(((struct MapWork *)map)->viewZ,
                       FIELD_AT(layer, s32, 20)) +
          FIELD_AT(layer, s32, 12);
      Func_02004dc4();
      System_Wait(1);
      /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, so the
         blend buffer's address is loaded straight into r0 at each call; as a
         direct Iwram_CopyWords call, 17 of 229 instructions differ in
         tla-en. */
      CopyWords_(Ram_MapCollision + 0x1e00, gData_020053e4, 24);
      Func_02004de4((u16 *)(Ram_MapCollision + 0x1e00));
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 5:
    case 8:
      Func_02004274(100);
      break;
    case 9:
    case 10:
      Func_02004274(102);
      break;
    case 3:
    case 4:
      Func_02004274(103);
      break;
    case 6:
    case 7:
      Func_02004274(101);
      break;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 3:
    case 5:
      Func_02004274(100);
      break;
    case 2:
    case 4:
    case 7:
      Func_02004274(101);
      break;
    case 6:
      Func_02004274(102);
      break;
    }
  }
  return 0;
}

#define GRID_WIDTH 28

// One grid cell: the step that stands on it (0 for none, 99 for the border)
// and the column and row of the next step.
typedef struct {
  s8 unk_0;
  s8 unk_1;
  s8 unk_2;
} Unk_Cell;

typedef struct {
  Unk_Cell cells[GRID_WIDTH * GRID_WIDTH];
} Unk_Grid;

// One step of the path: its number, then the step found below, to the left,
// to the right and above it.
typedef struct {
  u8 unk_0;
  u8 unk_1;
  u8 unk_2;
  u8 unk_3;
  u8 unk_4;
  u8 unk_5;
  u8 unk_6;
  u8 unk_7;
} Unk_Step;

// At gData_020023c4 + 0x288.
typedef struct {
  s32 unk_0;
  Unk_Step steps[15];
} Unk_Path;

#define gPath ((Unk_Path *)(gData_020023c4 + 0x288))

extern u32 gRandomState;
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *buffer);

static __inline__ void *Heap_AllocEwram_(s32 size) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as a
   * direct call, 102 of 418 instructions differ in tla-en. */
  return Heap_AllocEwram(size);
}

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]
#define AT_ADDRESS(x, y)                                                       \
  ((Unk_Cell *)(((y) * GRID_WIDTH + (x)) * 4 + (s32)grid))

void Func_020044f8(s32 seed) {
  Unk_Grid *grid;
  Unk_Path *path;
  Unk_Step *steps;
  Unk_Cell *cell;
  s32 x, y, nx, ny, step, tries, dir, v, next;
  u32 saved;

  saved = gRandomState;
  path = gPath;
  steps = path->steps;
  if (GameFlag_Test(0x109))
    return;
  if (GameFlag_Test(0x3f0))
    return;
  gRandomState = seed;
restart:
  grid = Heap_AllocEwram_(sizeof *grid);
  Iwram_ClearWords(grid, sizeof *grid);
  // BUG: this runs 29 times, so row 28 and column 28 lie past the 28 by 28
  // cells and the last row's marks land after the block.
  {
    Unk_Cell *a, *b, *e;
    s32 o, t;
    x = GRID_WIDTH / 2;
    y = GRID_WIDTH / 2;
    o = 0;
    b = &AT(GRID_WIDTH, 0);
    // step counts the border's rows here, before the walk numbers its steps.
    step = GRID_WIDTH;
    a = grid->cells;
    e = grid->cells;
  border:
    a->unk_0 = 99;
    a += GRID_WIDTH;
    b->unk_0 = 99;
    b += GRID_WIDTH;
    e->unk_0 = 99;
    e++;
    t = o + GRID_WIDTH * GRID_WIDTH * 4;
    ((s8 *)grid)[t] = 99;
    o += sizeof(Unk_Cell);
    step--;
    if (step >= 0)
      goto border;
  }
  for (step = 1; step <= 14;) {
    tries = 0;
  retry:
    if (++tries > 16)
      goto restart;
    nx = x;
    ny = y;
    dir = (Random16Far() * 4) >> 16;
    if (dir == 0 && (step == 1 || step == 13))
      goto retry;
    switch (dir) {
    case 0:
      if (AT(x, y + 1).unk_0 != 0 || AT(x, y + 2).unk_0 != 0 ||
          AT(x - 1, y + 1).unk_0 != 0 || AT(x + 1, y + 1).unk_0 != 0)
        goto retry;
      ny = y + 1;
      break;
    case 1:
      if (AT(x - 1, y).unk_0 != 0 || AT(x - 2, y).unk_0 != 0 ||
          AT(x - 1, y - 1).unk_0 != 0 || AT(x - 1, y + 1).unk_0 != 0)
        goto retry;
      nx = x - 1;
      break;
    case 2:
      if (AT(x + 1, y).unk_0 != 0 || AT(x + 2, y).unk_0 != 0 ||
          AT(x + 1, y - 1).unk_0 != 0 || AT(x + 1, y + 1).unk_0 != 0)
        goto retry;
      nx = x + 1;
      break;
    case 3:
      if (AT(x, y - 1).unk_0 != 0 || AT(x, y - 2).unk_0 != 0 ||
          AT(x - 1, y - 1).unk_0 != 0 || AT(x + 1, y - 1).unk_0 != 0)
        goto retry;
      ny = y - 1;
      break;
    }
    // The same cell variable the second loop reads its steps from.
    cell = AT_ADDRESS(x, y);
    cell->unk_0 = step;
    step++;
    cell->unk_1 = nx;
    cell->unk_2 = ny;
    x = nx;
    y = ny;
  }

  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  for (step = 1; step <= 14; step = next) {
    cell = AT_ADDRESS(x, y);
    next = step + 1;

    {
      s32 c;

      v = step;
      c = AT(x, y + 1).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        Unk_Cell *p = cell;
      scanUp:
        p -= GRID_WIDTH;
        c = p->unk_0;
        if (c == v - 1) {
          v = c;
          goto scanUp;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_2 = v;

    {
      s32 c;

      v = step;
      c = AT(x, y - 1).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        Unk_Cell *p = AT_ADDRESS(x, y);
      scanDown:
        p += GRID_WIDTH;
        c = p->unk_0;
        if (c == v - 1) {
          v = c;
          goto scanDown;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_5 = v;

    {
      s32 c;

      v = step;
      c = AT(x - 1, y).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
      scanRight:
        c = AT(++nx, y).unk_0;
        if (c == v - 1) {
          v = c;
          goto scanRight;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_3 = v;

    {
      s32 c;

      v = step;
      c = AT(x + 1, y).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
      scanLeft:
        c = AT(--nx, y).unk_0;
        if (c == v - 1) {
          v = c;
          goto scanLeft;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_4 = v;
    x = cell->unk_1;
    steps[step].unk_0 = step;
    y = cell->unk_2;
  }
  steps[14].unk_5 = 251;
  steps[1].unk_2 = 250;
  path->unk_0 = 1;
  Heap_Release(grid);
  Event_SetFlag(0x3f0);
  gRandomState = saved;
}

void Func_0200485c(s32 direction) {
  s32 *state = (s32 *)(gData_020023c4 + 0x288);
  u8 *row = (u8 *)state + *state * 8;
  s32 next = row[((direction - 1) & 3) + 6];

  switch (next) {
  case 250:
  case 251:
    FieldEvent_SetExitRequest(next);
    break;
  case 0:
    next = 1;
  default:
    *state = next;
    FieldEvent_SetExitRequest(direction);
    break;
  }
  Event_PlaySound(123);
}

struct GaiaRockInsideLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[0x28];
};

struct GaiaRockInsideMapWork {
  u8 unk000[0x108];
  struct GaiaRockInsideLayer layers[2];
};

extern s8 gData_020053fc[][2];

void Func_02004e0c(s32 x, s32 z, s32 width, s32 height, s32 cellX, s32 cellZ);

/// From the third state on, for each of the two map layers: Func_02004e0c
/// with the state's origin plus the layer's cell, a 15 by 10 size and the
/// layer's cell plus one.
void Func_020048a0(void) {
  s32 state = *(s32 *)(gData_020023c4 + 0x288);
  s8 *origin = gData_020053fc[state];
  struct GaiaRockInsideMapWork *map =
      (struct GaiaRockInsideMapWork *)Ram_HeapSlots->mapWork;

  if (state > 1) {
    s32 originX = origin[0];
    s32 originZ = origin[1];
    struct GaiaRockInsideLayer *layer = map->layers;
    s32 i;

    for (i = 1; i >= 0; i--, layer++) {
      /* FAKEMATCH(pin): x fixed to r2, so x is held in r2 and z in r3; as a
       * plain local, 18 of 45 instructions differ in tla-en. */
      register s32 x asm("r2") = layer->x >> 20;
      s32 z = layer->z >> 20;

      Func_02004e0c(originX + x, originZ + z, 15, 10, x + 1, z + 1);
    }
  }
}

struct GaiaRockInsideNode {
  u8 unk0[2];
  u8 next[4];
  u8 unk6[2];
};

struct GaiaRockInsidePath {
  s32 current;
  struct GaiaRockInsideNode nodes[1];
};

#define GAIA_ROCK_INSIDE_PATH                                                  \
  ((struct GaiaRockInsidePath *)(gData_020023c4 + 0x288))

/// The direction from the current node whose link is 251 or the next node:
/// 0x4000, 0x8000, 0 or 0xc000 for the four links in turn, or -1 if none is.
s32 Func_02004904(void) {
  struct GaiaRockInsidePath *path = GAIA_ROCK_INSIDE_PATH;
  s32 current = path->current;
  /* FAKEMATCH(pin): the node fixed to r3 and each link to r2; as a plain local,
   * 34 of 41 instructions differ in tla-en. */
  register struct GaiaRockInsideNode *node asm("r3") = &path->nodes[current];
  s32 next = current + 1;
  u32 link;

  link = node->next[0];
  if (link == 251 || link == next)
    return 0x4000;
  link = node->next[1];
  if (link == 251 || link == next)
    return 0x8000;
  link = node->next[2];
  if (link == 251 || link == next)
    return 0;
  link = node->next[3];
  if (link == 251 || link == next)
    return 0xc000;
  return -1;
}

void Func_0200495c(s32 layer, s32 keep) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *mapLayer = &map->layers[layer];
  u32 *cells = (u32 *)mapLayer->cells;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 height = (map->maxZ - map->minZ) >> 20;
  u32 fill;
  u16 z;
  u16 x;

  cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  fill = *cells;
  for (z = 0; z < height; z++) {
    u32 *cell = &cells[z * 128];

    for (x = 0; x < width; x++) {
      if (((u8 *)cell)[2] != 0 && ((u8 *)cell)[2] != keep)
        *cell = fill;
      cell++;
    }
  }
}

void Func_020049e4(void) {
  if (gMainRoutineWork.renderMode == 2) {
    u8 *event = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      *(u16 *)(event + 340) = code;
  }
}

void Func_02004a5c(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 column = x / 0x100000;
  s32 row = (z - y + 0x7fff) / 0x100000;

  cell += column + (row << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (row << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSingleChildFlags(object, 0);
}

extern const u8 gData_020062d4[];
extern const u8 gData_02006290[];

s32 Math_Cosine(s32 angle);

void Func_02004af0(s32 id, s32 flag) {
  struct MainRoutineWork *entryWork = &gMainRoutineWork;
  struct Actor *leader;
  s32 gravity;
  s32 value;
  struct Actor *object;
  s32 i;
  struct MainRoutineWork *work;

  /* FAKEMATCH(barrier): the form kept; as plain gMainRoutineWork.currentOwner
   * with no asm, 2 of 218 instructions differ in tla-en. */
  __asm__("" : "=r"(entryWork) : "0"(entryWork));
  /* FAKEMATCH(barrier): the form kept; as plain code with no asm here, 2 of 218
   * instructions differ in tla-en. */
  __asm__("");
  /* FAKEMATCH(temporary): the form kept; as id used directly from entry, 4 of
   * 218 instructions differ in tla-en. */
  value = id;
  leader = Actor_Get(entryWork->currentOwner);
  gravity = leader->gravity;
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object = Actor_Get(value);
  Func_02004a5c(value, leader->x, leader->y, leader->z);
  Func_02004dac(object, gData_020062d4);
  i = 15;
burst:
  object = (FieldActor_Create(255, leader->x, leader->y + 0x100000, leader->z));
  if (object != NULL) {
    u32 angle;
    u32 size;
    s32 scale;

    Func_02004dac(object, gData_02006290);
    angle = Random16Far();
    value = Random16Far();
    size = Random16Far();
    scale = (size >> 1) + 0x2000;
    object->velocityY = Iwram_MulQ16(value << 3, Math_Cosine(angle));
    object->velocityX = Iwram_MulQ16(value << 3, Math_Sin(angle));
    object->acceleration = 0;
    object->scaleX = scale;
    object->scaleY = scale;
    object->gravity = 0x547a;
    object->bounce = 0x8000;
  }
  i--;
  if (i >= 0)
    goto burst;
  Event_PlaySound(590);
  Event_PlaySound(217);
  MapGrid_SetShake(0x20000, 0x20000, 0xe666);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 257);
  Func_02004d9c(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(204);
  leader->motionFlags = 3;
  leader->gravity = 0x547a;
  for (value = 0; value < 180; value++) {
    System_Wait(1);
    if (leader->y <= leader->terrainHeight)
      break;
  }
  Event_PlaySound(188);
  work = &gMainRoutineWork;
  leader->gravity = gravity;
  Actor_SetAttachedEffect(work->currentOwner, 0);
  Func_02004d9c(leader, 38);
  MapGrid_SetShake(0x10000, 0x40000, 0x10000);
  System_Wait(10);
  MapGrid_SetShake(-1, -1, 0xe666);
  System_Wait(20);
  FieldCamera_MoveTo(leader->x, leader->y, leader->z, 1);
  Func_02004f54();
  FieldEvent_Wait(20);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02004d9c(leader, 1);
  leader->velocityY = 0x40000;
  System_Wait(20);
  work->renderMode = 0;
  Func_02004e64();
  Event_SetFlag(flag);
}
