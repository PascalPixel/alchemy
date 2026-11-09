#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

extern u8 MsgLemuriaStayExploring[];
extern u8 MsgLemuriaStayExploringAlternate[];

void Func_02001914(void);
void Func_0200191c(void);

void Func_02000534(void) {
  struct Actor *object;

  if (!GameFlag_Test(0x961) || GameFlag_Test(0x96a)) {
    Sound_Play(123);
    Import_EventSetExitRequest(2);
  } else {
    Func_02001914();
    Event_ResolvePendingActions(0);
    FieldActor_LaunchFromLeader(11, 0, -16, 0x5000);
    Actor_ArmCallback(4, FACING_NORTH, 0);
    Actor_WaitForMoveById(11);
    Event_Wait(20);
    if (GameFlag_Test(7)) {
      Event_SetMessageId(MsgLemuriaStayExploring);
      Event_ShowMessageThenWait(11, 0, 10);
    } else {
      Event_SetMessageId(MsgLemuriaStayExploringAlternate);
      Event_ShowMessageThenWait(11, 0, 10);
    }
    Actor_SetSpeed(11, 0x13333, 0x9999);
    Actor_SetModeById(11, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_ResetAndSetPosition(11, *(s16 *)((u8 *)object + 10),
                                       *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(11);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_WalkToAndWait(4, 168, 832);
    Func_0200191c();
  }
}
