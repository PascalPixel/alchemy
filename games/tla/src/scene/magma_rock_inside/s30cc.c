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

s32 Func_02004d40();
s32 Func_02004d48();
s32 Func_02004da8();
s32 Func_02002cd0();
s32 Party_LearnAbility();

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
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgBlazeTabletInscription, 1);
  if (Event_GetFlag(0x9fa) != 0) {
    Func_02004d48();
  } else {
    FieldEvent_SetMessageId((s32)MsgBlazeTabletInscription);
    EVENT_WORK->messageId++;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xb333, 0x5999);
    Actor_SetSpeed(5, 0xb333, 0x5999);
    Func_02004da8(5, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 136);
    Actor_WalkTo(5, 272, 136);
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
    FieldActor_WaitForMoveById(5);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_FaceDirection(5, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Event_ShowMessage(5, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 256, 152);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirection(5, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(5, 264, 137);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
    Message_ShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
    Message_ShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
    MapGrid_SetShake(0x50000, 0x70000, 0x10000);
    MapGrid_SetShake(-1, -1, 0xe666);
    MapGrid_WaitForShakeToSettle();
    Call2(Func_02002cd0, 8, 5);
    Party_LearnAbility(5, 0x9a);
    Event_SetFlag(0x9fa);
    Actor_FaceEachOther(gMainRoutineWork.currentOwner, 5, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Actor_SetSpeed(5, 0x13333, 0x9999);
    Actor_SetAnimation(5, 2);
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_SetDestination(5, FIELD_AT(object, s16, 10),
                                FIELD_AT(object, s16, 18));
    FieldActor_WaitForMoveById(5);
    Actor_SetPosition(5, 0, 0);
    Actor_SetActionCallback(Actor_Get(8), 9);
    Func_02004d48();
  }
}
