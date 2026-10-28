#include "local.h"
#include "call.h"
#include "event_work.h"
#include "edition.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

extern const u8 MsgBlazeTabletInscription[];

void FieldMessage_Show(const u8 *message, s32 mode);
s32 Func_02004d40();
s32 Func_02004d48();
s32 Func_02004dd0();
s32 Func_02004da8();
s32 Func_02004de0();
s32 Func_02004da0();
s32 Func_02004d10();
s32 Func_02004d18();
s32 Func_02002cd0();
s32 Party_LearnAbility();
s32 Actor_TurnToFaceEachOther();
s32 Actor_SetSpeedParameters();
s32 Actor_WaitForMoveById();
s32 Actor_ArmCallback();
s32 Actor_SetPositionAndReset();
s32 Actor_ResetAndSetPosition();

static inline void Call0(void (*f)()) {
  /* FAKEMATCH: the call layer without arguments; called directly, the two -1s
   * before it are negated in the other order (differ 2). */
  f();
}

/// Reading the Blaze tablet shows its inscription; the first time (flag 0x9fa
/// clear) the scene with actor 5 follows.
void Func_020030cc(void) {
  struct Actor *object;

  Func_02004d40();
  Call1(Func_02004e90, 0);
  FieldMessage_Show(MsgBlazeTabletInscription, 1);
  if (Value1(GameFlag_Test, 0x9fa) != 0) {
    Func_02004d48();
  } else {
    Call1(Func_02004dd0, (s32)MsgBlazeTabletInscription);
    EVENT_WORK->messageId++;
    Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xb333,
          0x5999);
    Call3(Actor_SetSpeedParameters, 5, 0xb333, 0x5999);
    Call2(Func_02004da8, 5, gMainRoutineWork.currentOwner);
    Call1(FieldEvent_Wait, 1);
    Call3(Actor_ResetAndSetPositionInMode2, gMainRoutineWork.currentOwner,
          256, 136);
    Call3(Actor_ResetAndSetPositionInMode2, 5, 272, 136);
    Call1(Actor_WaitForMoveById,
          gMainRoutineWork.currentOwner);
    Call1(Actor_WaitForMoveById, 5);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Call3(Actor_ArmCallback, 5, FACING_WEST, 0);
    Call1(FieldEvent_Wait, 10);
    Call2(Func_02004de0, 5, 0);
    Call2(Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
    Call1(FieldEvent_Wait, 30);
    Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 256,
          152);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    Call1(FieldEvent_Wait, 20);
    Call3(Actor_ArmCallback, 5, FACING_SOUTH, 0);
    Call1(FieldEvent_Wait, 20);
    Call2(Actor_SetModeById, 5, 3);
    Call1(FieldEvent_Wait, 30);
    Call3(Actor_ArmCallback, 5, FACING_NORTH, 0);
    Call1(FieldEvent_Wait, 20);
    Call3(Actor_SetPositionAndReset, 5, 264, 137);
    Call3(Actor_ArmCallback, 5, FACING_NORTH, 0);
    Call1(FieldEvent_Wait, 20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
    FieldMessage_Show(MsgBlazeTabletInscription + 3, 1);
#else
    FieldMessage_Show(MsgBlazeTabletInscription + 2, 1);
#endif
    Call3(Func_02004d10, 0x50000, 0x70000, 0x10000);
    Call3(Func_02004d10, -1, -1, 0xe666);
    Call0(Func_02004d18);
    Call2(Func_02002cd0, 8, 5);
    Call2(Party_LearnAbility, 5, 0x9a);
    Call1(GameFlag_SetBit, 0x9fa);
    Call3(Actor_TurnToFaceEachOther, gMainRoutineWork.currentOwner, 5, 0);
    Call1(FieldEvent_Wait, 10);
    Call2(Actor_SetModeById, 5, 3);
    Call2(Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
    Call1(FieldEvent_Wait, 20);
    Call3(Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
    Call2(Actor_SetModeById, 5, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != 0)
      Call3(Actor_ResetAndSetPosition, 5, FIELD_AT(object, s16, 10),
            FIELD_AT(object, s16, 18));
    Call1(Actor_WaitForMoveById, 5);
    Call3(Func_02004da0, 5, 0, 0);
    Actor_SetActionCallback(Actor_GetById(8), 9);
    Func_02004d48();
  }
}
