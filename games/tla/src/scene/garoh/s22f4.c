#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"

s32 Func_020051ac();
s32 Func_020051b4();
s32 Func_020052b4();
extern u8 MsgGaroh13[];

void Func_020022f4(void) {
  s32 v5;
  s32 t1;
  s32 t2;
  s32 t3;
  s32 b1;

  Event_SetFlag(0x91d);
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(((s32)MsgGaroh13));
  Actor_SetPosition(8, PIXELS(144), PIXELS(752));
  Actor_WalkToAndWait(4, 280, 776);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  FieldActor_LaunchFromLeader(9, 16, 0, FACING_NORTHWEST + FACING_STEP);
  FieldActor_WaitForMoveById(9);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(45);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(752), 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_SetPosition(8, PIXELS(144), PIXELS(720));
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 168, 720);
  FieldEvent_Wait(10);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  FieldActor_Jump(8, 6, 0);
  Actor_WalkToAndWait(8, 200, 720);
  FieldEvent_Wait(10);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 216, 712);
  Actor_WalkToAndWait(8, 320, 712);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(8, 0x9999, 0x4ccc);
  t1 = ((s32)Actor_Get(8));
  *(u8 *)((t1 + 90)) = (254 & *(u8 *)((t1 + 90)));
  Actor_WalkToAndWait(8, 296, 712);
  FieldEvent_Wait(1);
  t2 = ((s32)Actor_Get(8));
  /* FAKEMATCH(temporary): a local holding the constant 1, so the value is ORed
   * into the register that holds it; as one |= 1, 4 of 362 instructions differ
   * in tla-en. */
  b1 = 1;
  b1 |= *(u8 *)((t2 + 90));
  *(u8 *)((t2 + 90)) = b1;
  FieldEvent_Wait(15);
  Actor_FaceDirection(8, FACING_WEST + FACING_STEP, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(8, FACING_NORTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(8, FACING_WEST + FACING_STEP, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(25);
  Actor_ShowEmote(8, 0x102, 45);
  FieldActor_Jump(8, 6, 15);
  FieldActor_Jump(8, 6, 23);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  Actor_WalkToAndWait(8, 200, 712);
  FieldEvent_Wait(3);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  FieldActor_Jump(8, 6, 0);
  Actor_WalkToAndWait(8, 168, 712);
  FieldEvent_Wait(3);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  Actor_WalkToAndWait(8, 144, 712);
  {
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a2, so the third argument is loaded
     * first; as the plain call, 2 of 362 instructions differ in tla-en. */
    asm volatile("" : "+l"(a2));
    Actor_SetPosition(8, 0, a2);
  }
  FieldEvent_Wait(30);
  v5 = ((s32)&gMainRoutineWork);
  v5 = (v5 + 0x214);
  {
    s32 a0 = *(s32 *)(v5);
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 362 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldCamera_FollowActor(a0, 1);
  }
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  {
    s32 a0 = 4;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 362 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    Actor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetAnimation(9, 2);
  t3 = ((s32)Actor_Get(*(s32 *)(v5)));
  if (t3 == 0)
    goto L_0200a66c;
  Actor_SetDestination(9, *(s16 *)(t3 + 10), *(s16 *)(t3 + 18));
L_0200a66c:;
  FieldActor_WaitForMoveById(9);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 362 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    Actor_SetPosition(a0, 0, 0);
  }
  Func_020051b4();
}
