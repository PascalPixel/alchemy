#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "version.h"
#include "event_inline.h"

#if !TLA_PATCH_1
#define ISLETS_GIFT_TRANSITION 521
#else
#define ISLETS_GIFT_TRANSITION 513
#endif

struct ActionScript {
  u32 words[9];
};

extern struct ActionScript gData_02004004;
extern u8 MsgAtLongLast[];
void Func_02003fbc(void);
void Func_02003fc4(void);
void Func_02000df8(void);
void Func_02001278(void);
void Func_02003de4(struct Actor *object, s32 mode);
void PartyInventory_Remove(s32 item);
u8 *HeapSlot_AllocEwram(s32 block, s32 size);
void Func_020034ec(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_0200129c(struct Actor *gift) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *guide = Actor_GetById(9);
  struct ActionScript script = gData_02004004;

  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003fbc();
  Func_080d47b4(9, 264, 20);
  guide->update = Func_02000df8;
  Actor_SetMovementTypeAndWaitById(9, &script);
  guide->update = 0;
  Event_SetMessageId(MsgAtLongLast);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(9, 0x5000);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  guide->update = Func_02000df8;
  Actor_SetPositionAndReset(9, 296, 452);
  guide->update = 0;
  Actor_FaceDirection(9, 0x5000, 0);
  gift->speedLimit = 0x19999;
  gift->acceleration = 0xcccc;
  gift->update = Func_02000df8;
  Func_02003de4(gift, 2);
  FieldActor_SetMoveTarget(gift, 0x1160000, 0, 0x1c40000);
  Script_WaitForEventTimeout(gift);
  Func_02003de4(gift, 1);
  gift->update = 0;
  PartyInventory_Remove(455);
  Event_ShowMessageAndWait(9, 0);
  HeapSlot_AllocEwram(88, 664)[661] = 99;
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  leader->update = Func_02000df8;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 312, 446);
  leader->update = 0;
  Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 296, 452);
  leader->update = Func_02001278;
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetModeById(9, 3);
  Event_Wait(20);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  gift->speedLimit = 0xcccc;
  gift->acceleration = 0x6666;
  guide->update = Func_02000df8;
  Actor_ResetAndSetPosition(9, 288, 538);
  Event_Wait(20);
  Func_02003fc4();
  Func_020034ec();
  gift->update = Func_02000df8;
  Func_02003de4(gift, 2);
  FieldActor_SetMoveTarget(gift, 0x1200000, 0, 0x21a0000);
  Event_Wait(20);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = ISLETS_GIFT_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(3);
  GameFlag_SetBit(0xaa5);
}
