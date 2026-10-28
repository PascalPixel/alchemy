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
s32 Object_TurnToFaceEachOther();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_ResetAndSetPosition();

static inline void Call0(void (*f)()) {
  /* FAKEMATCH: the call layer without arguments; called directly, the two -1s
   * before it are negated in the other order (differ 2). */
  f();
}

/// Reading the Blaze tablet shows its inscription; the first time (flag 0x9fa
/// clear) the scene with actor 5 follows.
void Func_020030cc(void) {
  struct ObjectRuntime *object;

  Func_02004d40();
  Call1(Func_02004e90, 0);
  FieldMessage_Show(MsgBlazeTabletInscription, 1);
  if (Value1(GameFlag_Test, 0x9fa) != 0) {
    Func_02004d48();
  } else {
    Call1(Func_02004dd0, (s32)MsgBlazeTabletInscription);
    EVENT_WORK->messageId++;
    Call3(ObjectMotion_SetSpeedParameters, gAdventure.currentOwner, 0xb333,
          0x5999);
    Call3(ObjectMotion_SetSpeedParameters, 5, 0xb333, 0x5999);
    Call2(Func_02004da8, 5, gAdventure.currentOwner);
    Call1(FieldEvent_WaitFrames, 1);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, gAdventure.currentOwner,
          0x100, 0x88);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 5, 0x110, 0x88);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate,
          gAdventure.currentOwner);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 5);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
    Call1(FieldEvent_WaitFrames, 0xa);
    Call2(Func_02004de0, 5, 0);
    Call2(Object_SetModeById, gAdventure.currentOwner, 3);
    Call1(FieldEvent_WaitFrames, 0x1e);
    Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 0x100,
          0x98);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xc000, 0);
    Call1(FieldEvent_WaitFrames, 0x14);
    Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
    Call1(FieldEvent_WaitFrames, 0x14);
    Call2(Object_SetModeById, 5, 3);
    Call1(FieldEvent_WaitFrames, 0x1e);
    Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
    Call1(FieldEvent_WaitFrames, 0x14);
    Call3(ObjectMotion_SetPositionAndReset, 5, 0x108, 0x89);
    Call3(ObjectMotion_ArmCallback, 5, 0xc000, 0);
    Call1(FieldEvent_WaitFrames, 0x14);
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
    Call3(Object_TurnToFaceEachOther, gAdventure.currentOwner, 5, 0);
    Call1(FieldEvent_WaitFrames, 0xa);
    Call2(Object_SetModeById, 5, 3);
    Call2(Object_SetModeById, gAdventure.currentOwner, 3);
    Call1(FieldEvent_WaitFrames, 0x14);
    Call3(ObjectMotion_SetSpeedParameters, 5, 0x13333, 0x9999);
    Call2(Object_SetModeById, 5, 2);
    object = Object_GetById(gAdventure.currentOwner);
    if (object != 0)
      Call3(ObjectMotion_ResetAndSetPosition, 5, FIELD_AT(object, s16, 10),
            FIELD_AT(object, s16, 18));
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 5);
    Call3(Func_02004da0, 5, 0, 0);
    Object_SetActionCallback(Object_GetById(8), 9);
    Func_02004d48();
  }
}
