#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

extern u8 MsgAreWeSetting[];

void Func_02001010(void);
void Func_02001018(void);
void Func_02001070(s32 id, s32 target);

/// Object 18 comes to the party leader and asks whether to set out, as in
/// Func_02000120; after a no the leader steps back.
void Func_02000640(void) {
  struct Actor *object;

  Func_02001010();
  Event_ResolvePendingActions(0);
  Func_02001070(18, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(18, 532, 248);
  Actor_ArmCallbackAndRefresh(18, 0x3000);
  Event_SetMessageId(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Event_ShowMessageAndWait(18, 0);
    gMainRoutineWork.ownerOverride = 1;
    if (Event_GetFlag(0x9d0)) {
      GameFlag_SetBit(0x8df);
      GameFlag_SetBit(0x9b2);
    }
    Import_EventSetExitRequest(4);
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageAndWait(18, 0);
    Actor_SetSpeed(18, 0x19999, 0xcccc);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_ResetAndSetPosition(18, *(s16 *)((u8 *)object + 10),
                                       *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(18);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 528, 276);
    Func_02001018();
  }
}
