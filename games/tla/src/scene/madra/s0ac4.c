#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Event_Wait(s32 frames);
struct Actor *Actor_GetById(u32 objectId);
void Actor_TurnToFaceEachOther(s32 a, s32 b, s32 c);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_OffsetPositionAndReset(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 d);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02002b28(void);
void Func_02002b30(void);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void Event_SetMessageId(const u8 *message);
void Event_ShowMessageThenWait(s32 id, s32 a, s32 b);
void Actor_ArmCallbackAndRefresh(s32 id, s32 a);
struct Actor *Func_080d47b4(s32 id, s32 emote, s32 frames);
void Func_080d489c(s32 id, s32 a);
void Func_02002c28(void);
void Event_ResolvePendingActions(s32 a);
void Event_ShowMessageAndWait(s32 id, s32 a);
void Actor_Jump(s32 id, s32 a, s32 b);
void Func_080d3070(s32 id, s32 a, s32 b, s32 c);
s32 Map_PlayMapEdit(const void *edit, s32 a, s32 b);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
void Func_02002b10(s32 item, s32 a);
s32 Func_02002b18(s32 item);
void Func_02002c50(s32 item, s32 a);
void Func_02002c60(void);
void Sound_Play(s32 cue);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void FieldMessage_Show(s32 message, s32 position);
void Camera_FollowActor(s32 id, s32 mode);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);

extern const u8 MsgYouThereHold[];
extern const u8 MsgHasEscaped2[];
extern const u8 MsgHeyYouLot[];
extern const u8 MsgItWasThem[];
extern const u8 MsgWasAnythingStolen[];
extern const u8 MsgWait3[];
extern const u8 MsgTheMayorTried[];
extern const u8 MsgICanTBelieveIt[];
extern const u8 MsgWhoIsThis[];

static __inline__ void ArmCallback(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 72 of 475
   * instructions of Func_02000ac4 differ in tla-en, 36 of 549 of Func_02000f80
   * and 64 of 490 of Func_020014fc. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void OffsetPos(u32 id, s32 dx, s32 dz) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 10 of 475
   * instructions of Func_02000ac4 differ in tla-en and 12 of 490 of
   * Func_020014fc. */
  Actor_OffsetPositionAndReset(id, dx, dz);
}

static __inline__ void CommitPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 31 of 475
   * instructions of Func_02000ac4 differ in tla-en and 2 of 490 of
   * Func_020014fc. */
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Speed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 24 of 475
   * instructions of Func_02000ac4 differ in tla-en, 30 of 549 of Func_02000f80
   * and 28 of 490 of Func_020014fc. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 51 of 475
   * instructions of Func_02000ac4 differ in tla-en and 20 of 549 of
   * Func_02000f80. */
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 d) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 33 of 475
   * instructions of Func_02000ac4 differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, d);
}

static __inline__ void ArmCallbackAndRefresh(s32 id, s32 a) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 44 of 475
   * instructions of Func_02000ac4 differ in tla-en, 4 of 549 of Func_02000f80
   * and 48 of 490 of Func_020014fc. */
  Actor_ArmCallbackAndRefresh(id, a);
}

static __inline__ struct Actor *Emote(s32 id, s32 emote, s32 frames) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 8 of 475 instructions
   * of Func_02000ac4 differ in tla-en, 25 of 549 of Func_02000f80 and 27 of 490
   * of Func_020014fc. */
  return Func_080d47b4(id, emote, frames);
}

static __inline__ void LaunchFromFocused(s32 id, s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 36 of 475
   * instructions of Func_02000ac4 differ in tla-en. */
  Func_080d3070(id, a, b, c);
}

static __inline__ void SetPosInMode2(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 48 of 549
   * instructions of Func_02000f80 differ in tla-en. */
  Actor_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void SetHorizontalPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 50 of 549
   * instructions of Func_02000f80 differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Madra_Func_02002c10(s32 id, s32 a) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 31 of 549
   * instructions of Func_02000f80 differ in tla-en. */
  Func_080d489c(id, a);
}

/// Actors 24 and 25 talk with 32 and 4, 25 is moved about with its priority
/// bit cleared and set, and 32 is placed at the current owner before flag
/// 0x910 is set. It opens with "You there, hold".
void Func_02000ac4(void) {
  struct Actor *object;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgYouThereHold);
  Event_ShowMessageThenWait(24, 0, 2);
  CamBounds(238 << 17, -1, 210 << 17, 1);
  OffsetPos(25, 3, 8);
  CommitPos(24, -3, 8);
  Actor_WaitForMoveById(25);
  SetPos(4, 472, 426);
  Event_Wait(10);
  ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  LaunchFromFocused(32, 16, 1, FACING_NORTH);
  Func_02002c28();
  Actor_WaitForMoveById(32);
  ArmCallback(4, FACING_EAST, 0);
  ArmCallback(32, FACING_WEST, 0);
  Event_Wait(30);
  Event_Wait(10);
  ArmCallback(4, FACING_NORTH, 0);
  ArmCallback(32, FACING_NORTH, 0);
  Event_Wait(20);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(32, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_SetAngleToward(24, 25, 0);
  Event_Wait(10);
  Actor_SetAngleToward(25, 24, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(30);
  Actor_SetAngleToward(25, 24, 0);
  Event_Wait(10);
  Actor_SetAngleToward(25, 32, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(30);
  ArmCallback(24, FACING_SOUTH + FACING_STEP, 0);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  SetPos(25, 457, 402);
  CommitPos(25, 16, 0);
  SetPos(25, 472, 420);
  Event_Wait(30);
  Actor_SetPositionAndReset(25, 484, 420);
  Actor_GetById(25)->priorityFlags |= 1;
  ArmCallback(25, FACING_SOUTH, 0);
  Event_Wait(60);
  Event_Wait(10);
  ArmCallback(25, FACING_NORTH, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 4);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_ShowMessageThenWait(24, 0, 2);
  ArmCallbackAndRefresh(25, FACING_SOUTH);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(5);
  Event_ShowMessageThenWait(25, 0, 2);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  CommitPos(25, 0, -16);
  SetPos(25, 475, 400);
  CommitPos(25, -16, 0);
  Actor_GetById(25)->priorityFlags |= 1;
  CamBounds(238 << 17, -1, 198 << 17, 1);
  SetPos(25, 459, 416);
  ArmCallback(25, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Emote(32, 0x101, 50);
  Event_ShowMessageThenWait(32, 0, 2);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(24, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(24, 0, 2);
  Event_Wait(10);
  Emote(32, 0x100, 0);
  Emote(4, 0x100, 50);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(25, 4);
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(4, 32, 0);
  Event_Wait(20);
  Event_Wait(10);
  ArmCallback(4, FACING_NORTH, 0);
  ArmCallback(32, FACING_NORTH, 0);
  Emote(25, 0x109, 50);
  Event_ShowMessageThenWait(24, 0, 2);
  Emote(25, 0x103, 20);
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(30);
  ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessageThenWait(24, 0, 2);
  Speed(24, 0x4ccc, 0x2666);
  Speed(25, 0x4ccc, 0x2666);
  OffsetPos(24, 3, -8);
  Actor_OffsetPositionAndReset(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  ArmCallback(25, FACING_SOUTHEAST + FACING_STEP, 0);
  ArmCallback(24, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetModeById(32, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Actor_SetModeById(32, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != NULL)
    Actor_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  GameFlag_SetBit(0x910);
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  Func_02002b30();
}

/// Once flag 0x911 is set: actors 33 and 34 talk with 4, the prompt's answer
/// is counted in the event work either way, and flag 0x913 is set. The talk
/// includes "... has escaped".
void Func_02000f80(void) {
  if (!GameFlag_Test(0x911))
    return;
  Func_02002b28();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(PIXELS(124), -1, PIXELS(420), 1);
  Func_02002c28();
  Speed(33, 0xcccc, 0x6666);
  Speed(34, 0xcccc, 0x6666);
  Event_StartScreenTransition();
  SetHorizontalPos(33, PIXELS(172), PIXELS(332));
  Actor_SetHorizontalPositionWithTerrain(34, PIXELS(172), PIXELS(324));
  Event_SetMessageId(MsgHasEscaped2);
  Event_WaitTransitionFrames();
  Event_ShowMessageThenWait(33, 0, 10);
  SetPosInMode2(33, 156, 372);
  Event_Wait(20);
  SetPosInMode2(34, 156, 372);
  SetPosInMode2(4, 76, 388);
  Actor_WaitForMoveById(33);
  SetPosInMode2(33, 108, 380);
  Actor_WaitForMoveById(34);
  SetPosInMode2(34, 124, 380);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, FACING_SOUTHEAST, 0);
  Actor_WaitForMoveById(33);
  Actor_WaitForMoveById(34);
  ArmCallback(33, FACING_NORTH, 0);
  ArmCallback(34, FACING_NORTH, 0);
  Event_Wait(20);
  ArmCallback(33, FACING_NORTH, 0);
  ArmCallback(34, FACING_NORTH, 0);
  Madra_Func_02002c10(34, 258);
  Madra_Func_02002c10(33, 258);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(34, 3);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  SetPosInMode2(33, 102, 340);
  Event_Wait(20);
  SetPosInMode2(34, 102, 340);
  Actor_WaitForMoveById(33);
  SetHorizontalPos(33, PIXELS(4), PIXELS(4));
  Actor_WaitForMoveById(34);
  SetHorizontalPos(34, PIXELS(4), PIXELS(4));
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(40);
  Emote(4, 258, 30);
  Event_Wait(40);
  Event_Wait(120);
  SetHorizontalPos(33, PIXELS(102), PIXELS(340));
  SetHorizontalPos(34, PIXELS(102), PIXELS(340));
  SetPos(34, 100, 364);
  SetPosInMode2(34, 124, 380);
  SetPos(33, 100, 380);
  Actor_WaitForMoveById(34);
  SetPosInMode2(34, 132, 396);
  SetPos(33, 108, 396);
  Actor_WaitForMoveById(34);
  Actor_SetAngleToward(4, 33, 0);
  ArmCallback(34, FACING_NORTH, 0);
  ArmCallback(33, FACING_NORTH, 0);
  Emote(33, 258, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetAngleToward(34, 33, 0);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Actor_SetAngleToward(33, 34, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Func_080d47b4(33, 258, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  ArmCallback(33, FACING_NORTH, 0);
  Actor_ArmCallback(34, FACING_NORTH, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Event_Wait(20);
  Actor_ArmCallback(33, FACING_WEST, 0);
  Event_Wait(20);
  Func_080d47b4(33, 257, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  Actor_SetAngleToward(34, 33, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Actor_SetAngleToward(33, 34, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(33, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetAngleToward(33, 4, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(33, 0, 10);
  Func_080d47b4(4, 261, 30);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Event_Wait(20);
  Func_080d47b4(33, 262, 30);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_ShowMessageThenWait(33, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(33, 0, 10);
  }
  Actor_SetAngleToward(33, 34, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Func_080d47b4(34, 258, 30);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(60);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_CommitPositionAndActivate(34, 6, 0);
  SetPos(34, 164, 316);
  Event_Wait(10);
  ArmCallbackAndRefresh(33, FACING_WEST);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetHorizontalPositionWithTerrain(34, 0, 0);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  GameFlag_SetBit(0x913);
  Actor_CommitPositionAndActivate(33, 30, 0);
  SetPos(33, 164, 316);
  Event_Wait(20);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Event_Wait(20);
  Func_02002b30();
}

/// Actors 24, 25 and 32 question 4, whose answers are counted in the event
/// work; after flag 0x98f 32 asks again. It opens with "Hey, you lot"; 32's
/// second question is "It was them". Then 32 is placed at the current owner and
/// flag 0x914 is set.
void Func_020014fc(void) {
  struct Actor *object;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgHeyYouLot);
  Event_ShowMessageThenWait(24, 0, 2);
  Camera_MoveWithinMapBounds(PIXELS(476), -1, PIXELS(400), 1);
  CommitPos(24, -3, 8);
  Actor_SetPositionAndReset(4, 472, 426);
  Event_Wait(10);
  ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  Func_080d3070(32, 16, 1, FACING_NORTH);
  Func_02002c28();
  Actor_WaitForMoveById(32);
  Actor_CommitPositionAndActivate(25, 3, 8);
  Actor_ArmCallbackAndRefresh(25, FACING_EAST);
  Actor_SetModeById(25, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(25, 0, 10);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(32, FACING_WEST, 0);
  Event_Wait(20);
  Func_080d47b4(32, 0x101, 30);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetModeAndWaitAnimation(4, 4);
  ArmCallbackAndRefresh(24, FACING_SOUTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(24, 4);
  Event_ShowMessageThenWait(24, 0, 10);
  ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
  Func_080d47b4(32, 0x103, 0);
  Actor_Jump(32, 6, 15);
  Actor_Jump(32, 6, 23);
  Event_ShowMessageAndWait(32, 0);
  Emote(4, 0x100, 30);
  Actor_SetAngleToward(4, 24, 0);
  Event_Wait(20);
  Func_080d47b4(32, 0x100, 30);
  ArmCallbackAndRefresh(32, FACING_NORTH + FACING_STEP);
  Event_ShowMessageAndWait(32, 0);
  ArmCallbackAndRefresh(25, FACING_SOUTHEAST);
  Motion_SetModeAndWaitAnimation(25, 3);
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(10);
  FieldMessage_ShowAtActor(25, 0);
  ArmCallbackAndRefresh(4, FACING_NORTHWEST + FACING_STEP);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(25);
    ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
    Event_ShowMessageThenWait(25, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Event_Wait(25);
    ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(25, 0, 10);
  }
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  ArmCallbackAndRefresh(32, FACING_NORTHWEST + FACING_STEP);
  Event_ShowMessageAndWait(32, 0);
  Func_080d47b4(25, 0x102, 30);
  Event_ShowMessageThenWait(25, 0, 10);
  Motion_SetVarCbAndRefresh(24, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(24, 0, 10);
  if (GameFlag_Test(0x98f)) {
    Event_SetMessageId(MsgItWasThem);
    Actor_ArmCallback(32, FACING_WEST, 0);
    Event_Wait(20);
    FieldMessage_ShowAtActor(32, 0);
    Actor_ArmCallback(4, FACING_EAST, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Event_Wait(20);
      Motion_SetModeAndWaitAnimation(32, 3);
      Event_ShowMessageAndWait(32, 0);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      Event_Wait(20);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Motion_SetModeAndWaitAnimation(32, 4);
      Event_ShowMessageAndWait(32, 0);
    }
  }
  Event_SetMessageId(MsgWasAnythingStolen);
  ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(4, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Event_Wait(40);
  Emote(24, 0x101, 30);
  ArmCallback(24, FACING_WEST, 0);
  Event_Wait(20);
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  ArmCallback(4, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(25, 0x102, 30);
  Motion_SetModeAndWaitAnimation(25, 4);
  Event_Wait(10);
  ArmCallback(25, FACING_SOUTHEAST, 0);
  Event_ShowMessageThenWait(25, 0, 10);
  Emote(32, 0x101, 0);
  Func_080d47b4(4, 0x101, 30);
  Event_ShowMessageAndWait(32, 0);
  ArmCallbackAndRefresh(24, FACING_SOUTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(24, 4);
  Event_Wait(10);
  Event_ShowMessageThenWait(24, 0, 10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(25, 0, 10);
  Speed(24, 0x4ccc, 0x2666);
  Speed(25, 0x4ccc, 0x2666);
  OffsetPos(24, 3, -8);
  Actor_OffsetPositionAndReset(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  ArmCallback(25, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(24, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(32, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetModeById(32, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(10);
  GameFlag_SetBit(0x914);
  Actor_SetModeById(32, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != NULL)
    Actor_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  ArmCallbackAndRefresh(4, FACING_NORTH);
  Func_02002b30();
}

/// A map edit and where it applies (Map_PlayMapEdit forwards
/// to Map_PlayMapEdit).
struct CellPictureCopyEntry {
  const s16 *steps;
  u16 unk04;
  u16 unk06;
};

extern const struct CellPictureCopyEntry gData_02003308[];

// The macro's do-while starts each mode change's own scheduling region; as
// plain calls, 88 of 1565 instructions differ in tla-en.
#define SET_MODE(id, mode)                                                     \
  do {                                                                         \
    Actor_SetModeById(id, mode);                                               \
  } while (0)

/// Actor 34 questions 4 and the others; the answers are counted in the event
/// work. It opens with "Wait". With room for item 210 it is given and flag
/// 0x918 is set, otherwise the mayor's two lines are shown. 33 then talks with
/// the group, and they all gather at 4 before flags 0x980 and 0x98e are set.
void Func_020019d8(void) {
  struct Actor *object;
  s32 x;
  s32 z;

  /* FAKEMATCH(temporary): x and z held in locals set first, so the place call
   * builds x before the actor id; as constants passed to the call, 4 of 1565
   * instructions differ in tla-en (with z alone a local, 2). */
  x = 153 << 16;
  z = 0x01030000;
  Func_02002b28();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  if (GameFlag_Test(0x825) || GameFlag_Test(0x826))
    GameFlag_SetBit(0xa76);
  Event_SetMessageId(MsgWait3);
  Event_ShowMessageThenWait(34, 0, 0);
  SetPos(4, 150, 280);
  Actor_OffsetPositionAndReset(4, 16, 0);
  LaunchFromFocused(5, 8, 16, FACING_NORTH);
  LaunchFromFocused(32, 0, 0, FACING_NORTH);
  LaunchFromFocused(6, -8, 16, FACING_NORTH);
  LaunchFromFocused(7, -16, 0, FACING_NORTH);
  Actor_WaitForMoveById(4);
  ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(20);
  Map_PlayMapEdit(gData_02003308[6].steps, gData_02003308[6].unk04,
                  gData_02003308[6].unk06);
  Actor_SetHorizontalPositionAndAngleWithTerrain(34, x, z, FACING_SOUTH);
  Actor_CommitPositionAndActivate(34, 0, 4);
  SET_MODE(34, 3);
  /* FAKEMATCH(dead): this wait in a once-only block, which starts its own
   * scheduling region; as a plain call, whose message call then loads its
   * arguments in the other order, 2 of 1565 instructions differ in tla-en. */
  do {
    Event_Wait(20);
  } while (0);
  FieldMessage_ShowAtActor(34, 0);
  Actor_SetAngleToward(32, 4, 0);
  Actor_SetAngleToward(5, 4, 0);
  Actor_SetAngleToward(7, 4, 0);
  Actor_SetAngleToward(6, 4, 0);
  Actor_ArmCallback(34, FACING_SOUTH, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    ArmCallback(32, FACING_NORTH, 0);
    ArmCallback(5, FACING_NORTH, 0);
    ArmCallback(7, FACING_NORTH, 0);
    ArmCallback(6, FACING_NORTH, 0);
    Actor_ArmCallback(34, FACING_SOUTH, 0);
    Event_Wait(20);
    SET_MODE(34, 4);
    Event_Wait(33);
    Event_Wait(20);
    Event_ShowMessageAndWait(34, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    ArmCallback(32, FACING_NORTH, 0);
    ArmCallback(5, FACING_NORTH, 0);
    ArmCallback(7, FACING_NORTH, 0);
    ArmCallback(6, FACING_NORTH, 0);
    Actor_ArmCallback(34, FACING_SOUTH, 0);
    Event_Wait(20);
    SET_MODE(34, 4);
    Event_Wait(33);
    Event_Wait(20);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(34, 0, 0);
  }
  Emote(34, 0x102, 30);
  Event_ShowMessageThenWait(34, 0, 0);
  SET_MODE(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 0);
  Func_080d47b4(6, 0x102, 30);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAngleToward(5, 6, 0);
  Event_Wait(20);
  SET_MODE(5, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(34, 0x100, 30);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_CommitPositionAndActivate(34, 16, 8);
  ArmCallback(34, FACING_SOUTH, 0);
  if (Func_02002b18(210) >= 0) {
    Func_02002c50(210, 3);
    PartyInventory_GiveItem(210, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Actor_ArmCallback(4, FACING_NORTH, 0);
    GameFlag_SetBit(0x918);
  } else {
    s32 count = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) + 1;

    Event_SetMessageId(MsgTheMayorTried);
    Func_02002b10(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    Motion_SetVarCbAndRefresh(34, 2);
    Event_ShowMessageAndWait(34, 0);
    Actor_CommitPositionAndActivate(34, 16, 0);
    Func_02002b10(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    CommitPos(34, -16, 0);
    ArmCallbackAndRefresh(34, FACING_SOUTH);
    SetHorizontalPos(66, PIXELS(192), PIXELS(256));
    *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) = count;
  }
  CommitPos(34, -16, -8);
  ArmCallbackAndRefresh(34, FACING_SOUTH);
  Func_080d47b4(34, 0x105, 30);
  Event_ShowMessageThenWait(34, 0, 0);
  SET_MODE(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  ArmCallback(5, FACING_NORTH, 0);
  ArmCallback(6, FACING_NORTH, 0);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_SetModeById(32, 3);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  ArmCallbackAndRefresh(34, FACING_NORTH);
  SetPos(34, 153, 259);
  Actor_SetHorizontalPositionWithTerrain(34, 0, 0);
  SET_MODE(32, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Event_ShowMessageAndWait(33, 0);
  Sound_Play(78);
  Emote(4, 0x101, 0);
  Emote(5, 0x101, 0);
  Emote(7, 0x101, 0);
  Emote(6, 0x101, 0);
  Func_080d47b4(32, 0x101, 30);
  Actor_SetAngleToward(4, 33, 0);
  Actor_SetAngleToward(7, 33, 0);
  Actor_SetAngleToward(6, 33, 0);
  Actor_SetAngleToward(5, 33, 0);
  Actor_SetAngleToward(32, 33, 0);
  Event_Wait(20);
  Camera_FollowActor(33, 1);
  Event_Wait(60);
  Sound_Play(36);
  Emote(33, 0x107, 30);
  Event_ShowMessageAndWait(33, 0);
  ArmCallback(33, FACING_WEST, 0);
  Actor_TurnToFaceEachOther(6, 5, 0);
  Actor_TurnToFaceEachOther(7, 32, 0);
  Event_Wait(40);
  Emote(4, 0x102, 30);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_SetAngleToward(4, 33, 0);
  Actor_SetAngleToward(7, 33, 0);
  Actor_SetAngleToward(6, 33, 0);
  Actor_SetAngleToward(5, 33, 0);
  Actor_SetAngleToward(32, 33, 0);
  Event_Wait(20);
  ArmCallbackAndRefresh(33, FACING_WEST);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(6, 0x102, 30);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Func_080d47b4(5, 0x100, 30);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(33, 0x107, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_SetAngleToward(33, 4, 0);
  Event_Wait(20);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
    Event_ShowMessageAndWait(33, 0);
    Func_080d47b4(4, 0x102, 30);
    Event_Wait(40);
    SET_MODE(33, 3);
    Event_Wait(20);
    Event_Wait(20);
    Event_ShowMessageAndWait(33, 0);
    Actor_TurnToFaceEachOther(6, 5, 0);
    Event_Wait(40);
    Actor_SetAngleToward(6, 33, 0);
    Actor_SetAngleToward(5, 33, 0);
    Event_Wait(40);
    Actor_SetModeById(5, 4);
    SET_MODE(6, 4);
    Event_Wait(33);
    Event_Wait(20);
    Func_080d47b4(33, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  } else {
    Event_SetMessageId(MsgICanTBelieveIt);
    Event_ShowMessageAndWait(33, 0);
    Actor_TurnToFaceEachOther(6, 5, 0);
    Event_Wait(40);
    Actor_SetAngleToward(6, 33, 0);
    Actor_SetAngleToward(5, 33, 0);
    Event_Wait(40);
    Actor_SetModeById(5, 4);
    SET_MODE(6, 4);
    Event_Wait(33);
    Event_Wait(20);
    Func_080d47b4(6, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  }
  Event_SetMessageId(MsgWhoIsThis);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(32, 4);
  SET_MODE(4, 4);
  SET_MODE(7, 4);
  SET_MODE(6, 4);
  SET_MODE(5, 4);
  Event_Wait(33);
  Event_Wait(20);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(4, 0x109, 0);
  Emote(5, 0x109, 0);
  Emote(6, 0x109, 0);
  Func_080d47b4(32, 0x109, 30);
  Event_Wait(60);
  Func_080d47b4(33, 0x106, 30);
  Event_ShowMessageThenWait(33, 0, 0);
  Emote(4, 0x102, 0);
  Emote(5, 0x102, 0);
  Emote(6, 0x102, 0);
  Func_080d47b4(32, 0x102, 30);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(32, 0x100, 30);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(4, 0x103, 0);
  Emote(6, 0x103, 0);
  Emote(5, 0x103, 0);
  Emote(7, 0x103, 30);
  ArmCallback(33, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(33, 0x108, 30);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  Event_Wait(20);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(4, 0x100, 0);
  Emote(7, 0x100, 0);
  Emote(5, 0x100, 0);
  Emote(6, 0x100, 0);
  Func_080d47b4(4, 0x100, 30);
  Func_080d47b4(33, 0x101, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  ArmCallbackAndRefresh(33, FACING_WEST);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Func_080d47b4(33, 0x108, 30);
    Event_ShowMessageThenWait(33, 0, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Func_080d47b4(33, 0x103, 30);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(33, 0, 0);
  }
  Emote(32, 0x102, 30);
  SET_MODE(32, 4);
  Event_Wait(33);
  SET_MODE(32, 4);
  Event_Wait(33);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Sound_Play(78);
  CamBounds(-1, -1, -1, 0);
  Actor_CommitPositionAndActivate(33, 24, 24);
  Actor_CommitPositionAndActivate(33, 132, 0);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Func_02002c60();
  Camera_FollowActor(4, 1);
  Func_080d47b4(6, 0x102, 30);
  Event_ShowMessageAndWait(6, 0);
  SET_MODE(5, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Emote(7, 0x101, 30);
  Actor_SetAngleToward(7, 4, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 0);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(10);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAngleToward(5, 6, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  ArmCallback(32, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Func_080d47b4(7, 0x101, 20);
  Actor_SetAngleToward(7, 32, 0);
  Actor_SetAngleToward(4, 32, 0);
  Actor_SetAngleToward(6, 32, 0);
  Actor_SetAngleToward(5, 32, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 0);
  Actor_SetAngleToward(32, 7, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 10);
  Event_ShowMessageAndWait(5, 0);
  ArmCallback(5, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Actor_TurnToFaceEachOther(4, 7, 0);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  ArmCallbackAndRefresh(32, FACING_SOUTH);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 0);
  Event_Wait(20);
  Func_080d47b4(5, 0x100, 30);
  Actor_SetAngleToward(5, 32, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Emote(6, 0x108, 30);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 10);
  Actor_SetAngleToward(5, 6, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 0);
  Actor_SetAngleToward(4, 32, 0);
  Actor_SetAngleToward(5, 32, 0);
  Actor_SetAngleToward(6, 32, 0);
  Actor_SetAngleToward(7, 32, 0);
  Event_Wait(40);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(5, 3);
  Event_Wait(20);
  Event_Wait(20);
  Func_080d47b4(32, 0x108, 30);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(32, FACING_EAST);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(4, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(6, 3);
  SET_MODE(5, 3);
  Event_Wait(20);
  SET_MODE(4, 3);
  Event_Wait(20);
  SET_MODE(32, 3);
  /* FAKEMATCH(dead): this wait in a once-only block, which starts its own
   * scheduling region; as a plain call, whose speed call then loads its
   * arguments in the other order, 2 of 1565 instructions differ in tla-en. */
  do {
    Event_Wait(20);
  } while (0);
  Speed(32, 0x00013333, 0x9999);
  SET_MODE(32, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  Speed(5, 0x00013333, 0x9999);
  SET_MODE(5, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(5, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Speed(6, 0x00013333, 0x9999);
  SET_MODE(6, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(6, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Speed(7, 0x00013333, 0x9999);
  SET_MODE(7, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(7, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  GameFlag_SetBit(0x980);
  GameFlag_SetBit(0x98e);
  Func_02002b30();
}
