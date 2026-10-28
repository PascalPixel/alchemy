#include "scene_tables.h"
#include "airs_rock_inside.h"
#include "text/msg_ids.h"

void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004534(void);
#include "event_work.h"
#include "actor.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

s32 Func_02005ea4();
s32 Func_02005e74();
s32 Func_02005eac();
s32 Func_02005e6c();
s32 Func_02005dcc();
s32 Func_02005dd4();
s32 Func_02004138();
s32 Party_LearnAbility();
s32 Actor_TurnToFaceEachOther();
s32 Actor_SetSpeedParameters();
s32 Actor_WaitForMoveById();
s32 Actor_ResetAndSetPosition();
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

static inline void Call0(void (*f)()) { f(); }

/// Checking the tablet shows its message; the first time (flag 0x8d4 clear)
/// the scene with actor 6 follows, as Func_020030cc of Magma Rock does with
/// the Blaze tablet.
void Func_02004534(void) {
  struct FieldActor *object;

  Func_02005e14();
  Call1(Func_02005f2c, 0);
  FieldMessage_Show(MsgCheckedTheTablet, 1);
  if (Value1(GameFlag_Test, 0x8d4) != 0) {
    Func_02005e1c();
  } else {
    Call1(Func_02005ea4, (s32)MsgCheckedTheTablet);
    EVENT_WORK->messageId++;
    Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xb333,
          0x5999);
    Call3(Actor_SetSpeedParameters, 6, 0xb333, 0x5999);
    Call2(Func_02005e74, 6, gMainRoutineWork.currentOwner);
    Call1(FieldEvent_Wait, 1);
    Call3(Actor_ResetAndSetPositionInMode2, gMainRoutineWork.currentOwner, 256,
          136);
    Call3(Actor_ResetAndSetPositionInMode2, 6, 272, 136);
    Call1(Actor_WaitForMoveById,
          gMainRoutineWork.currentOwner);
    Call1(Actor_WaitForMoveById, 6);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Call3(Actor_ArmCallback, 6, FACING_WEST, 0);
    Call1(FieldEvent_Wait, 10);
    Call2(Func_02005eac, 6, 0);
    Call2(Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
    Call1(FieldEvent_Wait, 30);
    Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 256, 152);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    Call1(FieldEvent_Wait, 20);
    Call3(Actor_ArmCallback, 6, FACING_SOUTH, 0);
    Call1(FieldEvent_Wait, 20);
    Call2(Actor_SetModeById, 6, 3);
    Call1(FieldEvent_Wait, 30);
    Call3(Actor_ArmCallback, 6, FACING_NORTH, 0);
    Call1(FieldEvent_Wait, 20);
    Call3(Actor_SetPositionAndReset, 6, 264, 137);
    Call3(Actor_ArmCallback, 6, FACING_NORTH, 0);
    Call1(FieldEvent_Wait, 20);
    FieldMessage_Show(MsgCheckedTheTablet + 2, 1);
    Call3(Func_02005dcc, 0x50000, 0x70000, 0x10000);
    Call3(Func_02005dcc, -1, -1, 0xe666);
    /* FAKEMATCH(wrapper): the call through Call0's pointer; called directly,
     * the two -1s before it are negated in the other order, 2 of 202
     * instructions differ in tla-en. */
    Call0(Func_02005dd4);
    Call2(Func_02004138, 8, 6);
    Call2(Party_LearnAbility, 6, 0x90);
    Call1(GameFlag_SetBit, 0x8d4);
    Call3(Actor_TurnToFaceEachOther, gMainRoutineWork.currentOwner, 6, 0);
    Call1(FieldEvent_Wait, 10);
    Call2(Actor_SetModeById, 6, 3);
    Call2(Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
    Call1(FieldEvent_Wait, 20);
    Call3(Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
    Call2(Actor_SetModeById, 6, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != 0)
      Call3(Actor_ResetAndSetPosition, 6, FIELD_AT(object, s16, 10),
            FIELD_AT(object, s16, 18));
    Call1(Actor_WaitForMoveById, 6);
    Call3(Func_02005e6c, 6, 0, 0);
    Actor_SetActionCallback((struct Actor *)Actor_GetById(8), 9);
    Func_02005e1c();
  }
}

void Func_02004740(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 107);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_020047b0(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 280, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004818(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004880(void) {}
