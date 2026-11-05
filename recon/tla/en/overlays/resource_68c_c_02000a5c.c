// Draft, differ 150 of 883, skeleton gap 29 removed, 29 added (tla-en, 2026-11-01)
// Plain source: frame 48 as in the listing, one pos set before the first loop, no
// flags, zero, posAddress or reused phase. The differ is higher than the 107 of
// the two-pointer form (pos copied from a posAddress in the loop, frame 52, gap
// 37), but that is only a register rotation: what is left is two causes.
// 1. Allocation. Listing: target r8, start r10, pos r9, actor r11, radius sp+8.
//    Here pos r8, target r9, start r10, radius r11, actor sp+8. Priorities
//    (floor_log2(refs) * refs / live): pos 29/253, start and target 28/about 313,
//    radius 8/83, actor 35/670. Start and target need 32 refs to pass pos, actor
//    needs 39 to pass radius; the listing has no extra instruction that gives
//    them. Refs are read at lreg, so the extra ones vanish in reload, or the third
//    loop weighs three levels deeper (+6 start, +6 target, +6 actor, pos unchanged
//    fits all four inequalities, but the listing has one back edge there).
//    Radius is not excluded by class: HI_REGS 64 against MEM 128.
// 2. pos's address. The listing has add r2, sp, #12; mov r9, r2 inside the first
//    loop and never sets r9 again. PRE hoists that set even from a goto loop
//    (reg 861, no REG_EQUAL, so reload cannot rematerialize it); a copy from a local
//    in the loop is undone by the final cprop pass, which rewrites the second
//    loop's uses, and cse2 then makes the local canonical. Only a conditional that
//    cprop folds after PRE stopped it, and that is dead code.
// Those devices (goto first loop, dead test, asm refs in the third loop, empty
// do-whiles) reached differ 6, gap 3, and stay out. What they left was two swapped
// pairs: strb/mov/ldr in the second setup, adds r3, r3, r1 and lsls r0, r0, #5 in
// the first loop.
// Tried, no gain: 8-byte struct for angle and radius (frame 56); a radius per loop
// (single-block pseudos go to local-alloc and take r8); a pos per loop (differ 146,
// gap 29, frame 48, the same rotation); start and target set again in each setup;
// goto first loop (gap 31); plain arrays; radius after angle.
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Actor_GetById();
s32 GameFlag_Test();
void GameFlag_SetBit();
void Func_0200282c();
void Func_020028fc();
void Func_020028ac();
void Actor_SetSpeedParameters();
void Func_020028dc();
void Func_020028bc();
void Actor_SetPositionAndReset();
void Actor_ArmCallback();
void FieldEvent_Wait();
void Motion_SetModeAndWaitAnimation();
void Actor_CommitPositionAndActivate();
void Motion_SetVarCbAndRefresh();
void Actor_StartRepeatedMotion();
void Func_020028e4();
void FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void Func_02002924();
s32 Math_Sine();
void Vector_AddPolarOffsetFar();
void wait();
void Func_02002874();
s32 Func_02002834();

s32 Func_02000a5c(s32 a0, s32 a1, s32 a2) {
  struct Actor *actor;
  s32 startBuf[3], targetBuf[3], posBuf[3];
  s32 *start, *target, *pos;
  s32 radius, angle;
  s32 result, i, phase, sine, answer;

  actor = (struct Actor *)Actor_GetById(11);
  result = GameFlag_Test(0x965);
  if (result != 0)
    goto L_02008a86;
  goto L_0200927c;
L_02008a86:
  Call1((void (*)())GameFlag_SetBit, 0x966);
  {
    struct Actor *other = (struct Actor *)Actor_GetById(10);
    other->priorityFlags |= 1;
  }
  Func_0200282c();
  Call1((void (*)())Func_020028fc, 0);
  Func_020028ac((s32)MsgLemuriaInteriors10);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Value3((s32 (*)())Func_020028dc, 10, 0x100, 30);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 136, 152);
  Call3((void (*)())Actor_ArmCallback, 4, 0xa000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x109, 30);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 4, 0x2000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 8, 16);
  Call3((void (*)())Actor_ArmCallback, 4, 0x4000, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Value3((s32 (*)())Func_020028dc, 4, 0x100, 30);
  Call3((void (*)())Actor_ArmCallback, 4, 0xa000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, -8, -16);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Actor_StartRepeatedMotion, 4, 3);
  Call2((void (*)())Func_020028e4, 4, 0x102);
  Call1((void (*)())FieldEvent_Wait, 50);
  Call3((void (*)())Actor_CommitPositionAndActivate, 10, 12, 12);
  Call1((void (*)())FieldEvent_Wait, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x101, 30);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 10, 0);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    Call1((void (*)())FieldEvent_Wait, 30);
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    Call1((void (*)())FieldEvent_Wait, 40);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  }
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Actor_StartRepeatedMotion, 4, 3);
  Call2((void (*)())Func_020028e4, 4, 0x102);
  Call1((void (*)())FieldEvent_Wait, 50);
  Value3((s32 (*)())Func_020028dc, 10, 0x101, 30);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 10, 0);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    Call1((void (*)())FieldEvent_Wait, 30);
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    Call1((void (*)())FieldEvent_Wait, 40);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  }
  Value3((s32 (*)())Func_020028dc, 10, 0x102, 40);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x106, 40);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())Actor_SetPositionAndReset, 10, 89, 92);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0x8000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0x2000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0x8000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Value3((s32 (*)())Func_020028dc, 10, 0x108, 50);
  Call2((void (*)())Actor_SetModeById, 10, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call1((void (*)())Func_02002924, 0x26b);
  Call1((void (*)())FieldEvent_Wait, 60);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_020028e4, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_StartRepeatedMotion, 11, 3);
  Call2((void (*)())Func_020028e4, 11, 0x102);
  Call1((void (*)())FieldEvent_Wait, 50);
  Value2((s32 (*)())Actor_SetModeById, 11, 3);
  actor->motionFlags = 0;
  start = startBuf;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target = targetBuf;
  target[0] = 0x740000;
  target[1] = 0;
  target[2] = 0x6e0000;
  angle = 0xb000;
  pos = posBuf;
  for (i = 0; i <= 191; i++) {
    phase = i * 32768 / 192;
    radius = Math_Sine(phase) * 32;
    angle += 512;
    pos[0] = start[0] + (target[0] - start[0]) * i / 192;
    sine = Math_Sine(phase);
    pos[1] = start[1] + (target[1] - start[1]) * i / 192 + sine * 56;
    pos[2] = start[2] + (target[2] - start[2]) * i / 192;
    Vector_AddPolarOffsetFar(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->angle = angle + 16384;
    if (i == 128) {
      Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
    }
    Call1((void (*)())wait, 1);
  }
  actor->angle = 0x8000;
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Actor_SetModeById, 11, 1);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 10, 100, 112);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0x6666, 0x3333);
  Call3((void (*)())Actor_CommitPositionAndActivate, 10, 8, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_SetModeById, 11, 3);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Actor_SetModeById, 11, 1);
  Call1((void (*)())FieldEvent_Wait, 20);
  {
    struct Actor *other = (struct Actor *)Actor_GetById(10);
    other->actionFlags &= 254;
  }
  Call3((void (*)())Actor_CommitPositionAndActivate, 10, -8, 0);
  Call1((void (*)())FieldEvent_Wait, 1);
  {
    struct Actor *other = (struct Actor *)Actor_GetById(10);
    other->actionFlags |= 1;
  }
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x108, 40);
  Call2((void (*)())Actor_SetModeById, 11, 3);
  Call1((void (*)())FieldEvent_Wait, 40);
  actor->motionFlags = 0;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x840000;
  target[1] = 0x200000;
  target[2] = 0x640000;
  angle = 0x4200;
  for (i = 0; i <= 191; i++) {
    radius = Math_Sine(i * 16384 / 192) * 40;
    angle += 768;
    pos[0] = start[0] + (target[0] - start[0]) * i / 192;
    sine = Math_Sine(i * 32768 / 192);
    pos[1] = start[1] + (target[1] - start[1]) * i / 192 + sine * 32;
    pos[2] = start[2] + (target[2] - start[2]) * i / 192;
    Vector_AddPolarOffsetFar(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->angle = angle + 16384;
    if (i == 153) {
      Call3((void (*)())Actor_ArmCallback, 10, 0x7000, 0);
    }
    Call1((void (*)())wait, 1);
  }
  Call3((void (*)())Actor_ArmCallback, 10, 0xb000, 0);
  actor->angle = 0xd000;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x5a0000;
  target[1] = 0x40000;
  target[2] = 0x400000;
  for (i = 0; i <= 15; i++) {
    actor->x = start[0] + (target[0] - start[0]) * i / 16;
    actor->y = start[1] + (target[1] - start[1]) * i / 16;
    actor->z = start[2] + (target[2] - start[2]) * i / 16;
    Call1((void (*)())wait, 1);
  }
  Call3((void (*)())Func_02002874, 11, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Actor_SetSpritePriority, 10, 2);
  result = Func_02002834();
L_0200927c:
  return result;
}
