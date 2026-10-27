#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_WaitTicks(s32 ticks);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_LinkPair(s32 a, s32 b, s32 c);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_OffsetPositionAndReset(u32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 d);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002b88(s32 id, s32 a, s32 b);
void Func_02002bd0(const u8 *message);
void Func_02002be0(s32 id, s32 a, s32 b);
void Func_02002bf8(s32 id, s32 a);
struct ObjectRuntime *Func_02002c08(s32 id, s32 emote, s32 frames);
void Func_02002c10(s32 id, s32 a);
void Func_02002c28(void);
void Func_02002c58(s32 a);
void Func_02002be8(s32 id, s32 a);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void Func_02002c68(s32 id, s32 a, s32 b, s32 c);
s32 Func_02002ae8(const void *sequence, s32 a, s32 b);
void Func_02002b90(s32 id, s32 x, s32 z, s32 angle);
void Func_02002b10(s32 item, s32 a);
s32 Func_02002b18(s32 item);
void Func_02002c50(s32 item, s32 a);
void Func_02002c60(void);
void Func_02002c90(s32 cue);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Camera_FollowActor(s32 id, s32 mode);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);

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
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 72 of 475 in Func_02000ac4, 36 of
   * 549 in Func_02000f80, 64 of 490 in Func_020014fc). */
  ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void OffsetPos(u32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 10 of 475 in Func_02000ac4, 12 of
   * 490 in Func_020014fc). */
  ObjectMotion_OffsetPositionAndReset(id, dx, dz);
}

static __inline__ void CommitPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 31 of 475 in Func_02000ac4, 2 of
   * 490 in Func_020014fc). */
  ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Speed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 24 of 475 in Func_02000ac4, 30 of
   * 549 in Func_02000f80, 28 of 490 in Func_020014fc). */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 51 of 475 in Func_02000ac4, 20 of
   * 549 in Func_02000f80). */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 d) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 33 of 475 in Func_02000ac4). */
  Camera_MoveWithinMapBounds(x, y, z, d);
}

static __inline__ void ArmCallbackAndRefresh(s32 id, s32 a) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 44 of 475 in Func_02000ac4, 4 of
   * 549 in Func_02000f80, 48 of 490 in Func_020014fc). */
  Func_02002bf8(id, a);
}

static __inline__ struct ObjectRuntime *Emote(s32 id, s32 emote, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 8 of 475 in Func_02000ac4, 25 of
   * 549 in Func_02000f80, 27 of 490 in Func_020014fc). */
  return Func_02002c08(id, emote, frames);
}

static __inline__ void LaunchFromFocused(s32 id, s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 36 of 475 in Func_02000ac4). */
  Func_02002c68(id, a, b, c);
}

static __inline__ void SetPosInMode2(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 48 of 549 in Func_02000f80). */
  ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void SetHorizontalPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 50 of 549 in Func_02000f80). */
  Func_02002b88(id, x, z);
}

static __inline__ void Madra_Func_02002c10(s32 id, s32 a) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does (direct: differ 31 of 549 in Func_02000f80). */
  Func_02002c10(id, a);
}

/// Actors 24 and 25 talk with 32 and 4, 25 is moved about with its priority
/// bit cleared and set, and 32 is placed at the current owner before flag
/// 0x910 is set. It opens with "You there, hold".
void Func_02000ac4(void) {
  struct ObjectRuntime *object;

  Func_02002b28();
  Func_02002c58(0);
  Func_02002bd0(MsgYouThereHold);
  Func_02002be0(24, 0, 2);
  CamBounds(238 << 17, -1, 210 << 17, 1);
  OffsetPos(25, 3, 8);
  CommitPos(24, -3, 8);
  ObjectMotion_CommitCurrentPositionAndActivate(25);
  SetPos(4, 472, 426);
  FieldEvent_WaitTicks(10);
  ArmCallbackAndRefresh(gPartyState.currentOwner, 0xc000);
  LaunchFromFocused(32, 16, 1, 0xc000);
  Func_02002c28();
  ObjectMotion_CommitCurrentPositionAndActivate(32);
  ArmCallback(4, 0, 0);
  ArmCallback(32, 0x8000, 0);
  FieldEvent_WaitTicks(30);
  FieldEvent_WaitTicks(10);
  ArmCallback(4, 0xc000, 0);
  ArmCallback(32, 0xc000, 0);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(10);
  Motion_SetVarCbAndRefresh(32, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be0(32, 0, 2);
  FieldEvent_WaitTicks(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  FieldEvent_WaitTicks(20);
  Func_02002be0(25, 0, 2);
  FieldEvent_WaitTicks(10);
  ObjectMotion_SetAngleToward(24, 25, 0);
  FieldEvent_WaitTicks(10);
  ObjectMotion_SetAngleToward(25, 24, 0);
  FieldEvent_WaitTicks(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  FieldEvent_WaitTicks(30);
  ObjectMotion_SetAngleToward(25, 24, 0);
  FieldEvent_WaitTicks(10);
  ObjectMotion_SetAngleToward(25, 32, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  FieldEvent_WaitTicks(30);
  ArmCallback(24, 0x5000, 0);
  Object_GetById(25)->priorityFlags &= 0xfe;
  ObjectMotion_SetActionVariant(25, 3);
  SetPos(25, 457, 402);
  CommitPos(25, 16, 0);
  SetPos(25, 472, 420);
  FieldEvent_WaitTicks(30);
  SetPos(25, 484, 420);
  Object_GetById(25)->priorityFlags |= 1;
  ArmCallback(25, 0x4000, 0);
  FieldEvent_WaitTicks(60);
  FieldEvent_WaitTicks(10);
  ArmCallback(25, 0xc000, 0);
  FieldEvent_WaitTicks(10);
  Motion_SetModeAndWaitAnimation(25, 4);
  Func_02002be0(25, 0, 2);
  FieldEvent_WaitTicks(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Func_02002be0(24, 0, 2);
  ArmCallbackAndRefresh(25, 0x4000);
  FieldEvent_WaitTicks(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  FieldEvent_WaitTicks(5);
  Func_02002be0(25, 0, 2);
  Object_GetById(25)->priorityFlags &= 0xfe;
  ObjectMotion_SetActionVariant(25, 3);
  CommitPos(25, 0, -16);
  SetPos(25, 475, 400);
  CommitPos(25, -16, 0);
  Object_GetById(25)->priorityFlags |= 1;
  CamBounds(238 << 17, -1, 198 << 17, 1);
  SetPos(25, 459, 416);
  ArmCallback(25, 0x2000, 0);
  FieldEvent_WaitTicks(10);
  Emote(32, 0x101, 50);
  Func_02002be0(32, 0, 2);
  FieldEvent_WaitTicks(10);
  Motion_SetVarCbAndRefresh(24, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be0(24, 0, 2);
  FieldEvent_WaitTicks(10);
  Emote(32, 0x100, 0);
  Emote(4, 0x100, 50);
  FieldEvent_WaitTicks(20);
  Motion_SetModeAndWaitAnimation(25, 4);
  ArmCallback(32, 0xb000, 0);
  Func_02002be0(25, 0, 2);
  FieldEvent_WaitTicks(10);
  Object_LinkPair(4, 32, 0);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(10);
  ArmCallback(4, 0xc000, 0);
  ArmCallback(32, 0xc000, 0);
  Emote(25, 0x109, 50);
  Func_02002be0(24, 0, 2);
  Emote(25, 0x103, 20);
  ArmCallback(32, 0xb000, 0);
  Func_02002be0(25, 0, 2);
  FieldEvent_WaitTicks(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  FieldEvent_WaitTicks(30);
  ArmCallback(32, 0xd000, 0);
  Func_02002be0(24, 0, 2);
  Speed(24, 0x4ccc, 0x2666);
  Speed(25, 0x4ccc, 0x2666);
  OffsetPos(24, 3, -8);
  OffsetPos(25, -3, -8);
  ObjectMotion_CommitCurrentPositionAndActivate(24);
  ObjectMotion_CommitCurrentPositionAndActivate(25);
  ArmCallback(25, 0x3000, 0);
  ArmCallback(24, 0x5000, 0);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(10);
  Object_SetModeById(32, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_WaitTicks(30);
  Object_SetModeById(32, 2);
  object = Object_GetById(gPartyState.currentOwner);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(32);
  Func_02002b88(32, 0, 0);
  GameFlag_SetBit(0x910);
  Func_02002b88(26, 0, 0);
  Func_02002b30();
}

/// Once flag 0x911 is set: actors 33 and 34 talk with 4, the prompt's answer
/// is counted in the event work either way, and flag 0x913 is set. The talk
/// includes "... has escaped".
void Func_02000f80(void) {
  if (!GameFlag_Test(0x911))
    return;
  Func_02002b28();
  Func_02002c58(0);
  Camera_MoveWithinMapBounds(0x7c0000, -1, 0x1a40000, 1);
  Func_02002c28();
  Speed(33, 0xcccc, 0x6666);
  Speed(34, 0xcccc, 0x6666);
  Event_StartScreenTransition();
  SetHorizontalPos(33, 0xac0000, 0x14c0000);
  SetHorizontalPos(34, 0xac0000, 0x1440000);
  Func_02002bd0(MsgHasEscaped2);
  Event_WaitTransitionFrames();
  Func_02002be0(33, 0, 10);
  SetPosInMode2(33, 156, 372);
  FieldEvent_WaitTicks(20);
  SetPosInMode2(34, 156, 372);
  SetPosInMode2(4, 76, 388);
  ObjectMotion_CommitCurrentPositionAndActivate(33);
  SetPosInMode2(33, 108, 380);
  ObjectMotion_CommitCurrentPositionAndActivate(34);
  SetPosInMode2(34, 124, 380);
  ObjectMotion_CommitCurrentPositionAndActivate(4);
  ArmCallback(4, 0x2000, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(33);
  ObjectMotion_CommitCurrentPositionAndActivate(34);
  ArmCallback(33, 0xc000, 0);
  ArmCallback(34, 0xc000, 0);
  FieldEvent_WaitTicks(20);
  ArmCallback(33, 0xc000, 0);
  ArmCallback(34, 0xc000, 0);
  Madra_Func_02002c10(34, 258);
  Madra_Func_02002c10(33, 258);
  FieldEvent_WaitTicks(50);
  Motion_SetModeAndWaitAnimation(34, 3);
  Motion_SetModeAndWaitAnimation(33, 3);
  FieldEvent_WaitTicks(20);
  SetPosInMode2(33, 102, 340);
  FieldEvent_WaitTicks(20);
  SetPosInMode2(34, 102, 340);
  ObjectMotion_CommitCurrentPositionAndActivate(33);
  SetHorizontalPos(33, 0x40000, 0x40000);
  ObjectMotion_CommitCurrentPositionAndActivate(34);
  SetHorizontalPos(34, 0x40000, 0x40000);
  ArmCallback(4, 0xe000, 0);
  FieldEvent_WaitTicks(40);
  Emote(4, 258, 30);
  FieldEvent_WaitTicks(40);
  FieldEvent_WaitTicks(120);
  SetHorizontalPos(33, 0x660000, 0x1540000);
  SetHorizontalPos(34, 0x660000, 0x1540000);
  SetPos(34, 100, 364);
  SetPosInMode2(34, 124, 380);
  SetPos(33, 100, 380);
  ObjectMotion_CommitCurrentPositionAndActivate(34);
  SetPosInMode2(34, 132, 396);
  SetPos(33, 108, 396);
  ObjectMotion_CommitCurrentPositionAndActivate(34);
  ObjectMotion_SetAngleToward(4, 33, 0);
  ArmCallback(34, 0xc000, 0);
  ArmCallback(33, 0xc000, 0);
  Emote(33, 258, 30);
  Func_02002be0(33, 0, 10);
  ObjectMotion_SetAngleToward(34, 33, 0);
  Motion_SetModeAndWaitAnimation(34, 3);
  FieldEvent_WaitTicks(20);
  Func_02002be0(34, 0, 10);
  ObjectMotion_SetAngleToward(33, 34, 0);
  FieldEvent_WaitTicks(20);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be0(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  FieldEvent_WaitTicks(20);
  Func_02002be0(34, 0, 10);
  Emote(33, 258, 30);
  Func_02002be0(33, 0, 10);
  ArmCallback(33, 0xc000, 0);
  ArmCallback(34, 0xc000, 0);
  FieldEvent_WaitTicks(20);
  Motion_SetVarCbAndRefresh(34, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be0(34, 0, 10);
  Motion_SetModeAndWaitAnimation(33, 3);
  FieldEvent_WaitTicks(20);
  Func_02002be0(33, 0, 10);
  FieldEvent_WaitTicks(20);
  ArmCallback(33, 0x8000, 0);
  FieldEvent_WaitTicks(20);
  Emote(33, 257, 30);
  Func_02002be0(33, 0, 10);
  Func_02002be0(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  ObjectMotion_SetAngleToward(34, 33, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be0(34, 0, 10);
  ObjectMotion_SetAngleToward(33, 34, 0);
  FieldEvent_WaitTicks(10);
  Motion_SetModeAndWaitAnimation(33, 4);
  FieldEvent_WaitTicks(20);
  Func_02002be0(33, 0, 10);
  ObjectMotion_SetAngleToward(33, 4, 0);
  FieldEvent_WaitTicks(30);
  Func_02002be0(33, 0, 10);
  Emote(4, 261, 30);
  Motion_SetVarCbAndRefresh(34, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be0(34, 0, 10);
  FieldEvent_WaitTicks(20);
  Emote(33, 262, 30);
  UiText_OpenMessageAtObject(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Func_02002be0(33, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_02002be0(33, 0, 10);
  }
  ObjectMotion_SetAngleToward(33, 34, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be0(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  FieldEvent_WaitTicks(20);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be0(33, 0, 10);
  Emote(34, 258, 30);
  Motion_SetModeAndWaitAnimation(33, 3);
  FieldEvent_WaitTicks(20);
  Func_02002be0(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  FieldEvent_WaitTicks(60);
  Motion_SetModeAndWaitAnimation(34, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  ObjectMotion_CommitPositionAndActivate(34, 6, 0);
  SetPos(34, 164, 316);
  FieldEvent_WaitTicks(10);
  ArmCallbackAndRefresh(33, 0x8000);
  Motion_SetModeAndWaitAnimation(33, 3);
  FieldEvent_WaitTicks(20);
  Func_02002be0(33, 0, 10);
  SetHorizontalPos(34, 0, 0);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_WaitTicks(20);
  Motion_SetModeAndWaitAnimation(33, 3);
  FieldEvent_WaitTicks(20);
  GameFlag_SetBit(0x913);
  ObjectMotion_CommitPositionAndActivate(33, 30, 0);
  SetPos(33, 164, 316);
  FieldEvent_WaitTicks(20);
  SetHorizontalPos(33, 0, 0);
  FieldEvent_WaitTicks(20);
  Func_02002b30();
}

/// Actors 24, 25 and 32 question 4, whose answers are counted in the event
/// work; after flag 0x98f 32 asks again. It opens with "Hey, you lot"; 32's
/// second question is "It was them". Then 32 is placed at the current owner and
/// flag 0x914 is set.
void Func_020014fc(void) {
  struct ObjectRuntime *object;

  Func_02002b28();
  Func_02002c58(0);
  Func_02002bd0(MsgHeyYouLot);
  Func_02002be0(24, 0, 2);
  CamBounds(0x1dc0000, -1, 0x1900000, 1);
  CommitPos(24, -3, 8);
  SetPos(4, 472, 426);
  FieldEvent_WaitTicks(10);
  ArmCallbackAndRefresh(gPartyState.currentOwner, 0xc000);
  LaunchFromFocused(32, 16, 1, 0xc000);
  Func_02002c28();
  ObjectMotion_CommitCurrentPositionAndActivate(32);
  CommitPos(25, 3, 8);
  ArmCallbackAndRefresh(25, 0);
  Object_SetModeById(25, 4);
  FieldEvent_WaitTicks(20);
  Func_02002be0(25, 0, 10);
  ArmCallback(4, 0, 0);
  ArmCallback(32, 0x8000, 0);
  FieldEvent_WaitTicks(20);
  Emote(32, 0x101, 30);
  Func_02002be8(32, 0);
  Motion_SetModeAndWaitAnimation(4, 4);
  ArmCallbackAndRefresh(24, 0x5000);
  Motion_SetModeAndWaitAnimation(24, 4);
  Func_02002be0(24, 0, 10);
  ArmCallback(32, 0xd000, 0);
  Emote(32, 0x103, 0);
  ObjectMotion_Launch(32, 6, 15);
  ObjectMotion_Launch(32, 6, 23);
  Func_02002be8(32, 0);
  Emote(4, 0x100, 30);
  ObjectMotion_SetAngleToward(4, 24, 0);
  FieldEvent_WaitTicks(20);
  Emote(32, 0x100, 30);
  ArmCallbackAndRefresh(32, 0xd000);
  Func_02002be8(32, 0);
  ArmCallbackAndRefresh(25, 0x2000);
  Motion_SetModeAndWaitAnimation(25, 3);
  ArmCallback(32, 0xb000, 0);
  FieldEvent_WaitTicks(10);
  UiText_OpenMessageAtObject(25, 0);
  ArmCallbackAndRefresh(4, 0xb000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_WaitTicks(25);
    ArmCallback(32, 0xb000, 0);
    Func_02002be0(25, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    FieldEvent_WaitTicks(25);
    ArmCallback(32, 0xd000, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_02002be0(25, 0, 10);
  }
  Motion_SetVarCbAndRefresh(32, 2);
  FieldEvent_WaitTicks(20);
  ArmCallbackAndRefresh(32, 0xb000);
  Func_02002be8(32, 0);
  Emote(25, 0x102, 30);
  Func_02002be0(25, 0, 10);
  Motion_SetVarCbAndRefresh(24, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be0(24, 0, 10);
  if (GameFlag_Test(0x98f)) {
    Func_02002bd0(MsgItWasThem);
    ArmCallback(32, 0x8000, 0);
    FieldEvent_WaitTicks(20);
    UiText_OpenMessageAtObject(32, 0);
    ArmCallback(4, 0, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      FieldEvent_WaitTicks(20);
      Motion_SetModeAndWaitAnimation(32, 3);
      Func_02002be8(32, 0);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      FieldEvent_WaitTicks(20);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Motion_SetModeAndWaitAnimation(32, 4);
      Func_02002be8(32, 0);
    }
  }
  Func_02002bd0(MsgWasAnythingStolen);
  ArmCallback(32, 0xd000, 0);
  ArmCallback(4, 0xd000, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(32, 0);
  FieldEvent_WaitTicks(40);
  Emote(24, 0x101, 30);
  ArmCallback(24, 0x8000, 0);
  FieldEvent_WaitTicks(20);
  ArmCallback(32, 0xb000, 0);
  ArmCallback(4, 0xb000, 0);
  Emote(25, 0x102, 30);
  Motion_SetModeAndWaitAnimation(25, 4);
  FieldEvent_WaitTicks(10);
  ArmCallback(25, 0x2000, 0);
  Func_02002be0(25, 0, 10);
  Emote(32, 0x101, 0);
  Emote(4, 0x101, 30);
  Func_02002be8(32, 0);
  ArmCallbackAndRefresh(24, 0x5000);
  Motion_SetModeAndWaitAnimation(24, 4);
  FieldEvent_WaitTicks(10);
  Func_02002be0(24, 0, 10);
  Motion_SetModeAndWaitAnimation(25, 3);
  FieldEvent_WaitTicks(10);
  Func_02002be0(25, 0, 10);
  Speed(24, 0x4ccc, 0x2666);
  Speed(25, 0x4ccc, 0x2666);
  OffsetPos(24, 3, -8);
  OffsetPos(25, -3, -8);
  ObjectMotion_CommitCurrentPositionAndActivate(24);
  ObjectMotion_CommitCurrentPositionAndActivate(25);
  ArmCallback(25, 0x3000, 0);
  ArmCallback(24, 0x5000, 0);
  FieldEvent_WaitTicks(20);
  ArmCallback(4, 0, 0);
  ArmCallback(32, 0x8000, 0);
  FieldEvent_WaitTicks(20);
  Object_SetModeById(32, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_WaitTicks(10);
  GameFlag_SetBit(0x914);
  Object_SetModeById(32, 2);
  object = Object_GetById(gPartyState.currentOwner);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(32);
  SetHorizontalPos(32, 0, 0);
  ArmCallbackAndRefresh(4, 0xc000);
  Func_02002b30();
}

/// A metatile copy sequence and where it applies (Func_02002ae8 forwards
/// to Map_PlayMetatileCopySequence).
struct MetatileCopyEntry {
  const s16 *sequence;
  u16 unk04;
  u16 unk06;
};

extern const struct MetatileCopyEntry gData_02003308[];

/* FAKEMATCH: a one-pass block around each mode change starts its own
 * scheduling region; as plain calls, 88 of 1565 instructions differ in
 * tla-en. */
#define SET_MODE(id, mode)                                                     \
  do {                                                                         \
    Object_SetModeById(id, mode);                                              \
  } while (0)

/// Actor 34 questions 4 and the others; the answers are counted in the event
/// work. It opens with "Wait". With room for item 210 it is given and flag
/// 0x918 is set, otherwise the mayor's two lines are shown. 33 then talks with
/// the group, and they all gather at 4 before flags 0x980 and 0x98e are set.
void Func_020019d8(void) {
  struct ObjectRuntime *object;
  s32 x;
  s32 z;

  /* FAKEMATCH: x and z are set first, as forced temporaries; passed as
   * constants, the place call builds x after the actor id (differ 4 of 1565;
   * with z alone, 2). */
  x = 153 << 16;
  z = 0x01030000;
  Func_02002b28();
  Func_02002c58(0);
  Event_StartScreenTransition();
  if (GameFlag_Test(0x825) || GameFlag_Test(0x826))
    GameFlag_SetBit(0xa76);
  Func_02002bd0(MsgWait3);
  Func_02002be0(34, 0, 0);
  SetPos(4, 150, 280);
  OffsetPos(4, 16, 0);
  LaunchFromFocused(5, 8, 16, 0xc000);
  LaunchFromFocused(32, 0, 0, 0xc000);
  LaunchFromFocused(6, -8, 16, 0xc000);
  LaunchFromFocused(7, -16, 0, 0xc000);
  ObjectMotion_CommitCurrentPositionAndActivate(4);
  ArmCallback(4, 0xc000, 0);
  FieldEvent_WaitTicks(20);
  Func_02002ae8(gData_02003308[6].sequence, gData_02003308[6].unk04,
                gData_02003308[6].unk06);
  Func_02002b90(34, x, z, 0x4000);
  CommitPos(34, 0, 4);
  SET_MODE(34, 3);
  /* FAKEMATCH: a one-pass block, so this wait starts its own scheduling region
   * (as a plain call the message call loads its arguments in the other order,
   * differ 2 of 1565). */
  do {
    FieldEvent_WaitTicks(20);
  } while (0);
  UiText_OpenMessageAtObject(34, 0);
  ObjectMotion_SetAngleToward(32, 4, 0);
  ObjectMotion_SetAngleToward(5, 4, 0);
  ObjectMotion_SetAngleToward(7, 4, 0);
  ObjectMotion_SetAngleToward(6, 4, 0);
  ObjectMotion_ArmCallback(34, 0x4000, 0);
  FieldEvent_WaitTicks(20);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    ArmCallback(32, 0xc000, 0);
    ArmCallback(5, 0xc000, 0);
    ArmCallback(7, 0xc000, 0);
    ArmCallback(6, 0xc000, 0);
    ObjectMotion_ArmCallback(34, 0x4000, 0);
    FieldEvent_WaitTicks(20);
    SET_MODE(34, 4);
    FieldEvent_WaitTicks(33);
    FieldEvent_WaitTicks(20);
    Func_02002be8(34, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    ArmCallback(32, 0xc000, 0);
    ArmCallback(5, 0xc000, 0);
    ArmCallback(7, 0xc000, 0);
    ArmCallback(6, 0xc000, 0);
    ObjectMotion_ArmCallback(34, 0x4000, 0);
    FieldEvent_WaitTicks(20);
    SET_MODE(34, 4);
    FieldEvent_WaitTicks(33);
    FieldEvent_WaitTicks(20);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_02002be0(34, 0, 0);
  }
  Emote(34, 0x102, 30);
  Func_02002be0(34, 0, 0);
  SET_MODE(34, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be0(34, 0, 0);
  Emote(6, 0x102, 30);
  ObjectMotion_SetAngleToward(6, 5, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(6, 0);
  ObjectMotion_SetAngleToward(5, 6, 0);
  FieldEvent_WaitTicks(20);
  SET_MODE(5, 4);
  FieldEvent_WaitTicks(33);
  FieldEvent_WaitTicks(20);
  Func_02002be8(5, 0);
  Emote(34, 0x100, 30);
  FieldEvent_WaitTicks(20);
  Func_02002be0(34, 0, 0);
  CommitPos(34, 16, 8);
  ArmCallback(34, 0x4000, 0);
  if (Func_02002b18(210) >= 0) {
    Func_02002c50(210, 3);
    PartyInventory_GiveItem(210, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    ArmCallback(4, 0xc000, 0);
    GameFlag_SetBit(0x918);
  } else {
    s32 count = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) + 1;

    Func_02002bd0(MsgTheMayorTried);
    Func_02002b10(210, 2);
    UiText_ShowPositionedMessageAndWait(
        (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    Motion_SetVarCbAndRefresh(34, 2);
    Func_02002be8(34, 0);
    CommitPos(34, 16, 0);
    Func_02002b10(210, 2);
    UiText_ShowPositionedMessageAndWait(
        (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    CommitPos(34, -16, 0);
    ArmCallbackAndRefresh(34, 0x4000);
    SetHorizontalPos(66, 0xc00000, 0x1000000);
    *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) = count;
  }
  CommitPos(34, -16, -8);
  ArmCallbackAndRefresh(34, 0x4000);
  Emote(34, 0x105, 30);
  Func_02002be0(34, 0, 0);
  SET_MODE(34, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  ArmCallback(5, 0xc000, 0);
  ArmCallback(6, 0xc000, 0);
  Func_02002be0(34, 0, 0);
  Object_SetModeById(32, 3);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  ArmCallbackAndRefresh(34, 0xc000);
  SetPos(34, 153, 259);
  SetHorizontalPos(34, 0, 0);
  SET_MODE(32, 4);
  FieldEvent_WaitTicks(33);
  FieldEvent_WaitTicks(20);
  Func_02002be8(32, 0);
  Func_02002be8(33, 0);
  Func_02002c90(78);
  Emote(4, 0x101, 0);
  Emote(5, 0x101, 0);
  Emote(7, 0x101, 0);
  Emote(6, 0x101, 0);
  Emote(32, 0x101, 30);
  ObjectMotion_SetAngleToward(4, 33, 0);
  ObjectMotion_SetAngleToward(7, 33, 0);
  ObjectMotion_SetAngleToward(6, 33, 0);
  ObjectMotion_SetAngleToward(5, 33, 0);
  ObjectMotion_SetAngleToward(32, 33, 0);
  FieldEvent_WaitTicks(20);
  Camera_FollowActor(33, 1);
  FieldEvent_WaitTicks(60);
  Func_02002c90(36);
  Emote(33, 0x107, 30);
  Func_02002be8(33, 0);
  ArmCallback(33, 0x8000, 0);
  Object_LinkPair(6, 5, 0);
  Object_LinkPair(7, 32, 0);
  FieldEvent_WaitTicks(40);
  Emote(4, 0x102, 30);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  ObjectMotion_SetAngleToward(4, 33, 0);
  ObjectMotion_SetAngleToward(7, 33, 0);
  ObjectMotion_SetAngleToward(6, 33, 0);
  ObjectMotion_SetAngleToward(5, 33, 0);
  ObjectMotion_SetAngleToward(32, 33, 0);
  FieldEvent_WaitTicks(20);
  ArmCallbackAndRefresh(33, 0x8000);
  ArmCallbackAndRefresh(33, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Emote(6, 0x102, 30);
  ObjectMotion_SetAngleToward(6, 5, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(6, 0);
  Emote(5, 0x100, 30);
  Func_02002be8(5, 0);
  Emote(33, 0x107, 30);
  Func_02002be8(33, 0);
  ObjectMotion_SetAngleToward(33, 4, 0);
  FieldEvent_WaitTicks(20);
  UiText_OpenMessageAtObject(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
    Func_02002be8(33, 0);
    Emote(4, 0x102, 30);
    FieldEvent_WaitTicks(40);
    SET_MODE(33, 3);
    FieldEvent_WaitTicks(20);
    FieldEvent_WaitTicks(20);
    Func_02002be8(33, 0);
    Object_LinkPair(6, 5, 0);
    FieldEvent_WaitTicks(40);
    ObjectMotion_SetAngleToward(6, 33, 0);
    ObjectMotion_SetAngleToward(5, 33, 0);
    FieldEvent_WaitTicks(40);
    Object_SetModeById(5, 4);
    SET_MODE(6, 4);
    FieldEvent_WaitTicks(33);
    FieldEvent_WaitTicks(20);
    Emote(33, 0x109, 30);
    Func_02002be8(33, 0);
  } else {
    Func_02002bd0(MsgICanTBelieveIt);
    Func_02002be8(33, 0);
    Object_LinkPair(6, 5, 0);
    FieldEvent_WaitTicks(40);
    ObjectMotion_SetAngleToward(6, 33, 0);
    ObjectMotion_SetAngleToward(5, 33, 0);
    FieldEvent_WaitTicks(40);
    Object_SetModeById(5, 4);
    SET_MODE(6, 4);
    FieldEvent_WaitTicks(33);
    FieldEvent_WaitTicks(20);
    Emote(6, 0x109, 30);
    Func_02002be8(33, 0);
  }
  Func_02002bd0(MsgWhoIsThis);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  SET_MODE(32, 4);
  SET_MODE(4, 4);
  SET_MODE(7, 4);
  SET_MODE(6, 4);
  SET_MODE(5, 4);
  FieldEvent_WaitTicks(33);
  FieldEvent_WaitTicks(20);
  SET_MODE(33, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Emote(4, 0x109, 0);
  Emote(5, 0x109, 0);
  Emote(6, 0x109, 0);
  Emote(32, 0x109, 30);
  FieldEvent_WaitTicks(60);
  Emote(33, 0x106, 30);
  Func_02002be0(33, 0, 0);
  Emote(4, 0x102, 0);
  Emote(5, 0x102, 0);
  Emote(6, 0x102, 0);
  Emote(32, 0x102, 30);
  SET_MODE(33, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  ArmCallback(33, 0, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Emote(32, 0x100, 30);
  Func_02002be8(32, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  SET_MODE(33, 4);
  FieldEvent_WaitTicks(33);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Emote(4, 0x103, 0);
  Emote(6, 0x103, 0);
  Emote(5, 0x103, 0);
  Emote(7, 0x103, 30);
  ArmCallback(33, 0x8000, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Emote(33, 0x108, 30);
  Func_02002be8(33, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  FieldEvent_WaitTicks(20);
  SET_MODE(33, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  SET_MODE(33, 4);
  FieldEvent_WaitTicks(33);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Emote(4, 0x100, 0);
  Emote(7, 0x100, 0);
  Emote(5, 0x100, 0);
  Emote(6, 0x100, 0);
  Emote(4, 0x100, 30);
  Emote(33, 0x101, 30);
  Func_02002be8(33, 0);
  ArmCallbackAndRefresh(33, 0);
  FieldEvent_WaitTicks(20);
  ArmCallbackAndRefresh(33, 0x8000);
  FieldEvent_WaitTicks(20);
  ArmCallbackAndRefresh(33, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  ArmCallback(33, 0x8000, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  SET_MODE(33, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  UiText_OpenMessageAtObject(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Emote(33, 0x108, 30);
    Func_02002be0(33, 0, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Emote(33, 0x103, 30);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_02002be0(33, 0, 0);
  }
  Emote(32, 0x102, 30);
  SET_MODE(32, 4);
  FieldEvent_WaitTicks(33);
  SET_MODE(32, 4);
  FieldEvent_WaitTicks(33);
  Func_02002be8(32, 0);
  SET_MODE(33, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  ArmCallback(33, 0, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(33, 0);
  Func_02002c90(78);
  CamBounds(-1, -1, -1, 0);
  CommitPos(33, 24, 24);
  CommitPos(33, 132, 0);
  SetHorizontalPos(33, 0, 0);
  Func_02002c60();
  Camera_FollowActor(4, 1);
  Emote(6, 0x102, 30);
  Func_02002be8(6, 0);
  SET_MODE(5, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be8(5, 0);
  Emote(7, 0x101, 30);
  ObjectMotion_SetAngleToward(7, 4, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be0(7, 0, 0);
  SET_MODE(32, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(10);
  Func_02002be8(32, 0);
  Motion_SetVarCbAndRefresh(6, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(6, 0);
  ObjectMotion_SetAngleToward(5, 6, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(5, 0);
  SET_MODE(32, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  ArmCallback(32, 0x4000, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(32, 0);
  Emote(7, 0x101, 20);
  ObjectMotion_SetAngleToward(7, 32, 0);
  ObjectMotion_SetAngleToward(4, 32, 0);
  ObjectMotion_SetAngleToward(6, 32, 0);
  ObjectMotion_SetAngleToward(5, 32, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be0(7, 0, 0);
  ObjectMotion_SetAngleToward(32, 7, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(32, 0);
  ObjectMotion_SetAngleToward(6, 5, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(6, 0);
  Object_SetPartAttribute(Object_GetById(5), 10);
  Func_02002be8(5, 0);
  ArmCallback(5, 0x4000, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(5, 0);
  Object_LinkPair(4, 7, 0);
  FieldEvent_WaitTicks(40);
  Motion_SetVarCbAndRefresh(32, 2);
  FieldEvent_WaitTicks(20);
  ArmCallbackAndRefresh(32, 0x4000);
  Func_02002be8(32, 0);
  Object_SetPartAttribute(Object_GetById(5), 0);
  FieldEvent_WaitTicks(20);
  Emote(5, 0x100, 30);
  ObjectMotion_SetAngleToward(5, 32, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(5, 0);
  Emote(6, 0x108, 30);
  Func_02002be8(6, 0);
  Object_SetPartAttribute(Object_GetById(5), 10);
  ObjectMotion_SetAngleToward(5, 6, 0);
  FieldEvent_WaitTicks(20);
  Func_02002be8(5, 0);
  Motion_SetVarCbAndRefresh(32, 2);
  FieldEvent_WaitTicks(20);
  Func_02002be8(32, 0);
  Object_SetPartAttribute(Object_GetById(5), 0);
  ObjectMotion_SetAngleToward(4, 32, 0);
  ObjectMotion_SetAngleToward(5, 32, 0);
  ObjectMotion_SetAngleToward(6, 32, 0);
  ObjectMotion_SetAngleToward(7, 32, 0);
  FieldEvent_WaitTicks(40);
  SET_MODE(32, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be8(32, 0);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  SET_MODE(32, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Func_02002be8(32, 0);
  SET_MODE(5, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  Emote(32, 0x108, 30);
  Func_02002be8(32, 0);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  SET_MODE(32, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  ArmCallbackAndRefresh(32, 0);
  Func_02002be8(32, 0);
  SET_MODE(4, 3);
  FieldEvent_WaitTicks(20);
  FieldEvent_WaitTicks(20);
  SET_MODE(6, 3);
  SET_MODE(5, 3);
  FieldEvent_WaitTicks(20);
  SET_MODE(4, 3);
  FieldEvent_WaitTicks(20);
  SET_MODE(32, 3);
  /* FAKEMATCH: a one-pass block, so this wait starts its own scheduling region
   * (as a plain call the speed call loads its arguments in the other order,
   * differ 2 of 1565). */
  do {
    FieldEvent_WaitTicks(20);
  } while (0);
  Speed(32, 0x00013333, 0x9999);
  SET_MODE(32, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(32);
  SetHorizontalPos(32, 0, 0);
  Speed(5, 0x00013333, 0x9999);
  SET_MODE(5, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(5, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(5);
  SetHorizontalPos(5, 0, 0);
  Speed(6, 0x00013333, 0x9999);
  SET_MODE(6, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(6, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(6);
  SetHorizontalPos(6, 0, 0);
  Speed(7, 0x00013333, 0x9999);
  SET_MODE(7, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(7, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(7);
  SetHorizontalPos(7, 0, 0);
  GameFlag_SetBit(0x980);
  GameFlag_SetBit(0x98e);
  Func_02002b30();
}
