#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "text/msg_ids.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
void FieldMessage_Show(s32 message, s32 position);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_SetAngleToward(s32 id, s32 target, s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Func_02004df4(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02004dfc(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02004e3c(struct Actor *object, s32 value);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004ebc(s32 id, s32 x, s32 z);
void Func_02004ec4(s32 id, s32 target);
void Func_02004f6c(s32 value);
void Func_02004fcc(s32 value);
void Func_02004fdc(void);
void Func_02005014(s32 cue);

/// Three scene events of one shape, for objects 8 and 12, 9 and 12, and 8 and
/// 14: the first object shakes with random vertical speeds, and while the flag
/// of its event (0x9bd, 0x9be, 0x9bc) is clear it moves, two map copies open
/// the way and the partner returns to the leader.
void Func_0200225c(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(8);
  partner = Actor_GetById(12);
  Func_02004e5c();
  Func_02004fcc(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Func_02005014(193);
  Func_02004e3c(partner, 1);
  Func_02004ec4(12, gAdventure.currentOwner);
  Call3(Actor_ResetAndSetPosition, 12, 528, 372);
  partner->velocityY = 0x60000;
  Actor_WaitForMoveById(12);
  FieldEvent_Wait(45);
  Actor_SetAngleToward(8, gAdventure.currentOwner, 0);
  Func_02005014(576);
  Actor_SetModeById(8, 5);
  wait(30);
  Actor_SetModeById(8, 2);
  FieldEvent_Wait(30);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(40);
  Func_02005014(191);
  if (!GameFlag_Test(0x9bd)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Call3(Actor_SetSpeedParameters, 8, 0x4000, 0xccc);
    Actor_OffsetPositionAndResetMotion(8, 24, 0);
    Call6(Func_02004dfc, 27, 23, 1, 1, 28, 22);
    Func_02004dfc(32, 16, 2, 1, 29, 22);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16Far();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    Call6(Func_02004df4, 2, 93, 1, 5, 28, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Func_02005014(564);
    Actor_SetModeById(8, 4);
    FieldEvent_Wait(30);
    leader = Actor_GetById(gAdventure.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                       *(s16 *)((u8 *)leader + 18));
    partner->velocityY = 0x60000;
    Actor_WaitForMoveById(12);
    Func_02004ebc(12, 0, 0);
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x308);
      FieldEvent_Wait(30);
      Func_02004fdc();
      Func_02004f6c(20);
    } else {
      GameFlag_SetBit(0x9bd);
      Func_02004e64();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    FieldEvent_Wait(30);
    Actor_SetModeById(8, 4);
    FieldEvent_Wait(30);
    leader = Actor_GetById(gAdventure.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                       *(s16 *)((u8 *)leader + 18));
    partner->velocityY = 0x60000;
    Actor_WaitForMoveById(12);
    Func_02004ebc(12, 0, 0);
    Func_02004e64();
  }
}

void Func_020024b8(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(9);
  partner = Actor_GetById(12);
  Func_02004e5c();
  Func_02004fcc(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Func_02005014(193);
  Func_02004e3c(partner, 1);
  Func_02004ec4(12, gAdventure.currentOwner);
  Call3(Actor_ResetAndSetPosition, 12, 672, 372);
  partner->velocityY = 0x60000;
  Actor_WaitForMoveById(12);
  FieldEvent_Wait(45);
  Actor_SetAngleToward(9, gAdventure.currentOwner, 0);
  Func_02005014(576);
  Actor_SetModeById(9, 5);
  wait(30);
  Actor_SetModeById(9, 2);
  FieldEvent_Wait(30);
  Actor_SetModeById(9, 3);
  FieldEvent_Wait(40);
  Func_02005014(191);
  if (!GameFlag_Test(0x9be)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Call3(Actor_SetSpeedParameters, 9, 0x4000, 0xccc);
    Call3(Actor_ResetAndSetPosition, 9, 608, 352);
    Call6(Func_02004dfc, 35, 23, 1, 1, 36, 22);
    Func_02004dfc(32, 16, 2, 1, 37, 22);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16Far();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    Call6(Func_02004df4, 2, 93, 1, 5, 36, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Func_02005014(564);
    Actor_SetModeById(9, 4);
    FieldEvent_Wait(30);
    leader = Actor_GetById(gAdventure.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                       *(s16 *)((u8 *)leader + 18));
    partner->velocityY = 0x60000;
    Actor_WaitForMoveById(12);
    Func_02004ebc(12, 0, 0);
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x309);
      FieldEvent_Wait(30);
      Func_02004fdc();
      Func_02004f6c(20);
    } else {
      GameFlag_SetBit(0x9be);
      Func_02004e64();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    FieldEvent_Wait(30);
    Actor_SetModeById(9, 4);
    FieldEvent_Wait(30);
    leader = Actor_GetById(gAdventure.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                       *(s16 *)((u8 *)leader + 18));
    partner->velocityY = 0x60000;
    Actor_WaitForMoveById(12);
    Func_02004ebc(12, 0, 0);
    Func_02004e64();
  }
}

void Func_02002718(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(8);
  partner = Actor_GetById(14);
  Func_02004e5c();
  Func_02004fcc(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Func_02005014(193);
  Func_02004e3c(partner, 1);
  Func_02004ec4(14, gAdventure.currentOwner);
  Call3(Actor_ResetAndSetPosition, 14, 336, 396);
  partner->velocityY = 0x60000;
  Actor_WaitForMoveById(14);
  FieldEvent_Wait(45);
  Actor_SetAngleToward(8, gAdventure.currentOwner, 0);
  Func_02005014(576);
  Actor_SetModeById(8, 5);
  wait(30);
  Actor_SetModeById(8, 2);
  FieldEvent_Wait(30);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(40);
  Func_02005014(191);
  if (!GameFlag_Test(0x9bc)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Call3(Actor_SetSpeedParameters, 8, 0x4000, 0xccc);
    Call3(Actor_OffsetPositionAndResetMotion, 8, -16, 0);
    Call6(Func_02004dfc, 27, 15, 2, 1, 23, 23);
    Func_02004dfc(25, 25, 1, 1, 25, 23);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16Far();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    Call6(Func_02004df4, 1, 85, 1, 4, 25, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Func_02005014(564);
    Actor_SetModeById(8, 4);
    FieldEvent_Wait(30);
    leader = Actor_GetById(gAdventure.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(14, *(s16 *)((u8 *)leader + 10),
                                       *(s16 *)((u8 *)leader + 18));
    partner->velocityY = 0x60000;
    Actor_WaitForMoveById(14);
    Func_02004ebc(14, 0, 0);
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x307);
      FieldEvent_Wait(30);
      Func_02004fdc();
      Func_02004f6c(20);
    } else {
      GameFlag_SetBit(0x9bc);
      Func_02004e64();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    FieldEvent_Wait(30);
    Actor_SetModeById(8, 4);
    FieldEvent_Wait(30);
    leader = Actor_GetById(gAdventure.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(14, *(s16 *)((u8 *)leader + 10),
                                       *(s16 *)((u8 *)leader + 18));
    partner->velocityY = 0x60000;
    Actor_WaitForMoveById(14);
    Func_02004ebc(14, 0, 0);
    Func_02004e64();
  }
}
