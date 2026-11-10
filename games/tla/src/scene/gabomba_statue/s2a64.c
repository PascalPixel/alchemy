#include "types.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
extern u32 gData_02005ccc[];
extern u32 gData_02005d24[];
extern u32 gData_02005d8c[];
u8 *Unit_Get(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);
s32 Func_02003604(void);
void Func_020056b4(void);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_0200564c(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057b4(void);
void Func_02004f40(s32 *x_out, s32 *z_out);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02005624(struct Actor *object, s32 mode);
s32 ArcTan2(s32 y, s32 x);
void Func_02005644(struct Actor *object);

extern u32 gData_02005c40[];

void Event_FinishScreenTransition(void);
void Func_0200581c(void);

static __inline__ void Func_0200578c_ArgsFirst(s32 id, s32 a, s32 b) {
  /* FAKEMATCH(pin): the second argument pinned to r1, so the id is set after
   * it and the third; as calls through a plain forwarding inline, 16 of 521
   * instructions differ in tla-en. */
  register s32 a_ asm("r1") = a;
  s32 b_ = b;

  Func_080d47b4(id, a_, b_);
}

void Func_02002a64(void) {
  s32 speed;
  s32 cue;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  {
    /* FAKEMATCH(temporary): the leader's id read into a local before the speed
     * is set; as Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner,
     * ...), 2 of 521 instructions differ in tla-en. */
    s32 owner = gMainRoutineWork.currentOwner;

    speed = 0xc000;
    Actor_SetPositionAndFacing(owner, 174 << 17, 156 << 17, speed);
  }
  Actor_SetPositionAndFacing(7, 165 << 17, 156 << 17, speed);
  {
    /* FAKEMATCH(temporary): actor 23's speed in a local; as the constant in the
     * call, 4 of 521 instructions differ in tla-en. */
    s32 slow = 0xb000;

    Actor_SetPositionAndFacing(23, 216 << 17, 156 << 17, slow);
  }
  Actor_SetPositionAndFacing(6, 224 << 17, 156 << 17, speed);
  Actor_SetPositionAndFacing(5, 234 << 17, 156 << 17, speed);
  System_Wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Func_0200578c_ArgsFirst(23, 256, 20);
  FieldEvent_SetMessageId(MsgGabombaUnderstandsChannel);
  Event_ShowMessage(23, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  /* FAKEMATCH(temporary): the else branch's 0x6007 set here in a local, which
   * gives the event work counter r3 and r2 as the native has them; as
   * Event_ShowMessage(0x6007, 0), 20 of 521 instructions differ in
   * tla-en. */
  cue = 0x6007;
  FieldCamera_MoveTo(196 << 17, -1, 146 << 17, 1);
  Func_020057b4();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(0x4006, 0);
  ((void *)Event_OpenMessage(0x6005, 0));
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_0200578c_ArgsFirst(7, 256, 20);
    Actor_SetSpeed(7, 0x20000, 0x10000);
    Actor_WalkToAndWait(7, 340, 296);
    Actor_WalkToAndWait(7, 354, 296);
    Actor_WalkToAndWait(7, 364, 308);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
    Actor_FaceDirectionAndWait(7, 0x6000);
    Event_ShowMessage(0x6007, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Actor_SetSpeed(7, 0x20000, 0x10000);
    Actor_WalkToAndWait(7, 340, 296);
    Actor_WalkToAndWait(7, 354, 296);
    Actor_WalkToAndWait(7, 364, 308);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
    Actor_FaceDirectionAndWait(7, 0x6000);
    Actor_SetAnimation(7, 3);
    Event_ShowMessage(cue, 0);
  }
  Actor_FaceDirectionAndWait(23, 0x8000);
  Actor_SetAnimationAndWait(23, 4);
  Event_ShowMessage(0x6017, 0);
  Actor_FaceDirection(7, 0, 0);
  Func_0200578c_ArgsFirst(7, 257, 40);
  Event_ShowMessage(0x6017, 0);
  Actor_SetAnimationAndWait(7, 3);
  Func_0200578c_ArgsFirst(23, 265, 20);
  Event_ShowMessage(0x6017, 0);
  Event_ShowMessage(0x4007, 0);
  Func_0200578c_ArgsFirst(7, 256, 40);
  Event_ShowMessage(0x4007, 0);
  Actor_SetAnimation(23, 3);
  Event_ShowMessage(0x4017, 0);
  Func_0200578c_ArgsFirst(7, 265, 10);
  Event_ShowMessage(0x4007, 0);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_WalkToAndWait(6, 440, 296);
  Actor_FaceDirectionAndWait(6, 0x8000);
  Actor_SetAnimationAndWait(6, 3);
  Event_ShowMessage(0x4006, 0);
  Actor_FaceDirection(7, 0xe000, 0);
  Func_0200578c_ArgsFirst(7, 261, 0);
  Actor_FaceDirection(6, 0x2000, 0);
  Actor_FaceDirection(5, 0xa000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 80);
  Func_0200578c_ArgsFirst(7, 261, 40);
  Actor_FaceDirectionAndWait(7, 0);
  Event_ShowMessage(0x4007, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(23, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetAnimationAndWait(7, 3);
  Actor_FaceDirectionAndWait(23, 0xb000);
  Event_ShowMessage(23, 0);
  Func_0200578c_ArgsFirst(23, 257, 0);
  Actor_FaceDirection(23, 0xd000, 20);
  Actor_FaceDirectionAndWait(23, 0xb000);
  Event_ShowMessage(23, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(196 << 17, -1, 176 << 16, 1);
  Func_020057b4();
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(7, 0);
  Event_ShowMessage(0x2005, 0);
  FieldCamera_MoveTo(196 << 17, -1, 146 << 17, 1);
  Func_020057b4();
  Actor_SetAnimation(23, 3);
  Event_ShowMessage(23, 0);
  Actor_FaceDirectionAndWait(23, 0x8000);
  Event_ShowMessage(23, 0);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetSpeed(23, 0x19999, 0xcccc);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetScript(7, gData_02005c40);
  Actor_SetScript(23, gData_02005c40);
  Actor_SetScript(6, gData_02005c40);
  Actor_RunScriptAndWait(5, gData_02005c40);
  Event_SetFlag(0x8f6);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200581c();
  FieldEvent_SetExitRequest(13);
}
