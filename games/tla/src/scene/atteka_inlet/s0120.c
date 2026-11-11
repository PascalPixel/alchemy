#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

extern u8 MsgAreWeSetting[];

void Func_02001010(void);
void Func_02001018(void);

/// Object 18 asks whether to set out: a yes sets the owner override and, once
/// flag 0x9d0 is set, flags 0x8df and 0x9b2; a no counts the refusal and
/// object 18 walks back to the party leader.
void Func_02000120(void) {
  struct Actor *object;

  Func_02001010();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Event_ShowMessageAndWait(18, 0);
    gMainRoutineWork.ownerOverride = 1;
    /* FAKEMATCH(wrapper): the flag test forwarded through event.h's
     * Event_GetFlag; as a direct GameFlag_Test call, gMainRoutineWork's offsets
     * take r3 instead of r2, 22 of 89 instructions differ in tla-en (found
     * by alchemy permute). */
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
    GameFlag_SetBit(0x201);
    Func_02001018();
  }
}
