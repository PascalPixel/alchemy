#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "actor.h"
#include "task.h"
#include "scene_tables.h"
#include "io_reg.h"
#include "event_inline.h"

extern u32 gData_020007e4;
extern u8 MsgImilVoicesDisbeliefReply[];

u32 Random16Far(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_02000060(struct Actor *object);
void Func_020000f4(void);
void Func_02000568(void);
void Func_02000608(void);

void Func_020001b0(void) {
  struct Actor *actor;

  Func_02000568();
  Event_ResolvePendingActions(0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  wait(1);
  gData_020007e4 = 0;
  Task_Add(Func_020000f4, TASK_PASS_VBLANK);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSpritePriority(9, 2);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Func_080dc978(13, 14);
  Event_SetMessageId(MsgImilVoicesDisbeliefReply);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Actor_ShowEmote(8, 256, 0);
  Actor_ShowEmote(8, 256, 20);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_Jump(8, 4, 10);
  Event_ShowMessage(0x8008, 0);
  Actor_ShowEmote(11, 257, 0);
  Actor_ShowEmote(12, 257, 40);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Actor_SetModeById(9, 3);
  Event_ShowMessage(0x8009, 0);
  Actor_ArmCallbackAndRefresh(12, FACING_EAST);
  Event_ShowMessageAndWait(0x800c, 0);
  Actor_FaceDirectionAndWait(11, FACING_WEST);
  Actor_SetModeById(11, 4);
  Event_ShowMessageAndWait(0x400b, 0);
  actor = Actor_GetById(11);
  *((u8 *)actor + 98) = ((Random16Far() * 80) >> 16) + 40;
  *((u8 *)actor + 99) = 0;
  actor->update = (void (*)(void))Func_02000060;
  actor = Actor_GetById(12);
  {
    /* FAKEMATCH(temporary): the timer's + 40 comes before its address, as in
     * the native; as a direct store, 2 of 303 instructions differ in tla-en. */
    u8 timer = ((Random16Far() * 80) >> 16) + 40;

    *((u8 *)actor + 98) = timer;
  }
  *((u8 *)actor + 99) = 2;
  actor->update = (void (*)(void))Func_02000060;
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Actor_SetModeById(8, 4);
  Event_ShowMessage(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Event_ShowMessage(0x8009, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Event_ShowMessage(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Event_ShowMessage(0x8009, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessage(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(1, 0);
  Func_02000608();
  Actor_Jump(9, 2, 0);
  Func_080d47b4(9, 263, 20);
  Event_ShowMessageAndWait(0x8009, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(3, 0);
  Func_02000608();
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessageAndWait(6, 0);
  Func_02000608();
  Event_Wait(20);
  *(u16 *)BG_PLTT = 0;
  wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 16;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0xa3c);
  Import_EventSetExitRequest(3);
}
