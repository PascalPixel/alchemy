#include "izumo.h"
#include "task.h"

void Func_02000954(void) {
  if (!GameFlag_Test(2346)) {
    const u8 *message;

    Func_02000d00();
    Func_02000dd0(0);
    Call3((void (*)())Func_02000d88, 32, 256, 60);
    message = MsgWouldYouBe;
    Func_02000d60(message);
    /* FAKEMATCH: the native schedules this call as a value-returning one (the
     * flags are loaded before the actor: one reordered pair with the header's
     * void prototype). */
    ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(32, 0);
    if (Event_AskYesNoWithActorAnimation(gSequenceParentWork.currentOwner, 0) == 0) {
      Func_02000814();
    } else {
      Func_02000d60(message - 1);
      Func_02000d70(32, 0);
    }
    Func_02000d08();
  } else {
    Func_02000d70((s32)MsgTheLegendSays, 0);
  }
}

s32 Func_020009d0(void) {
  /* FAKEMATCH: Call3 sets Func_02000d40's arguments up in the native's order,
   * as in Func_020006a4 (a direct call: differ 2). */
  GameFlag_ClearBit(0x302);
  WORD_AT(Ram_HeapSlots->eventWork, 428) = 0x100;
  Task_Add(Func_0200060c, TASK_PASS_FRAME);
  Func_02000de0(gData_02000e10);
  if (GameFlag_Test(0x927)) {
    BYTE_AT(Actor_GetById(15), 90) = 2;
    Actor_SetModeById(25, 5);
    Actor_SetModeById(26, 5);
    Actor_SetModeById(27, 5);
    Actor_SetModeById(28, 5);
    Actor_SetModeById(29, 6);
    Actor_SetModeById(30, 6);
    Actor_SetModeById(31, 5);
    if (gSequenceParentWork.entrance == 8 && GameFlag_Test(0x928) &&
        !GameFlag_Test(0x929) && !GameFlag_Test(0x92a)) {
      Event_StartScreenTransition();
      Func_020006a4();
    }
    if (GameFlag_Test(0x929) && !GameFlag_Test(0x92a)) {
      Call3((void (*)())Func_02000d40, 32, 200 << 16, 216 << 16);
      Actor_SetSpritePriority(32, 3);
      Actor_GetById(32)->motionFlags |= 2;
      GameFlag_SetBit(0x302);
    }
  }
  Actor_SetSpritePriority(24, 3);
  Actor_GetById(24)->motionFlags |= 2;
  return 0;
}
