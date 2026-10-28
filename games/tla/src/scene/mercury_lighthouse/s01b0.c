#include "types.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "actor.h"
#include "call.h"
#include "task.h"

extern u32 gData_020007e4;
extern u8 MsgImilVoicesDisbeliefReply[];

struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void GameFlag_SetBit(s32 flag);
u32 Random16Far(void);
s32 Task_Add(void (*func)(void), s32 order);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_Jump(s32 actor, s32 height, s32 frames);
void Func_02000060(struct Actor *object);
void Func_020000f4(void);
void Func_02000568(void);
void Func_02000598(const u8 *message);
void Func_020005a0(s32 a, s32 b);
void Func_020005b0(s32 actor, s32 value);
void Func_020005c0(s32 actor, s32 a, s32 b);
void Func_020005c8(s32 a);
void Func_020005f8(s32 skip);
void Func_02000600(s32 a, s32 b);
void Func_02000608(void);

void Func_020001b0(void) {
  struct Actor *actor;

  Func_02000568();
  Func_020005f8(0);
  Actor_SetPartAttribute(Actor_GetById(gSequenceParentWork.currentOwner), 15);
  wait(1);
  gData_020007e4 = 0;
  Task_Add(Func_020000f4, TASK_PASS_VBLANK);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSpritePriority(9, 2);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Call2((void (*)())Func_02000600, 13, 14);
  Func_02000598(MsgImilVoicesDisbeliefReply);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Value3((s32 (*)())Func_020005c0, 8, 256, 0);
  Value3((s32 (*)())Func_020005c0, 8, 256, 20);
  Call3((void (*)())Actor_ArmCallback, 8, 0x3000, 0);
  Call3((void (*)())Actor_Jump, 8, 4, 10);
  Call2((void (*)())Func_020005a0, 0x8008, 0);
  Value3((s32 (*)())Func_020005c0, 11, 257, 0);
  Value3((s32 (*)())Func_020005c0, 12, 257, 40);
  Call3((void (*)())Actor_ArmCallback, 11, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0xb000, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Actor_SetModeById(9, 3);
  Call2((void (*)())Func_020005a0, 0x8009, 0);
  Call2((void (*)())Func_020005b0, 12, 0);
  Call2((void (*)())Func_020005a0, 0x800c, 0);
  Call2((void (*)())Func_020005b0, 11, 0x8000);
  Actor_SetModeById(11, 4);
  Call2((void (*)())Func_020005a0, 0x400b, 0);
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
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Actor_SetModeById(8, 4);
  Call2((void (*)())Func_020005a0, 0x8008, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Call2((void (*)())Func_020005a0, 0x8009, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Call2((void (*)())Func_020005a0, 0x8008, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Call2((void (*)())Func_020005a0, 0x8009, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Motion_SetModeAndWaitAnimation(8, 3);
  Call2((void (*)())Func_020005a0, 0x8008, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 1, 0);
  Func_02000608();
  Call3((void (*)())Actor_Jump, 9, 2, 0);
  Value3((s32 (*)())Func_020005c0, 9, 263, 20);
  Call2((void (*)())Func_020005a0, 0x8009, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 3, 0);
  Func_02000608();
  Motion_SetModeAndWaitAnimation(8, 3);
  Call2((void (*)())Func_020005a0, 0x8008, 0);
  Call2((void (*)())Func_02000600, 13, 14);
  Call2((void (*)())Func_020005a0, 6, 0);
  Func_02000608();
  FieldEvent_Wait(20);
  *(u16 *)0x05000000 = 0;
  wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 16;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0xa3c);
  Func_020005c8(3);
}
