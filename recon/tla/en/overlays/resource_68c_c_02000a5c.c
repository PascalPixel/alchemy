// Draft, differ 107 of 883, skeleton gap 37 removed, 38 added (tla-en, 2026-11-01)
// Plain C leaves one cause: the listing's allocation. The listing has target r8,
// start r10, pos r9, actor r11 and radius spilled to sp+8; here PRE hoists
// pos's address, so the second loop's pos is posAddress in a stack slot, and
// radius (8 refs, 84 insns) outranks the actor (35 refs, 672 insns) for r11.
// floor_log2 makes both a step: pos set in the first loop and kept in the second
// is one pseudo of 30 refs and about 242 insns, ahead of start and target (28)
// until they reach 32 refs, and the actor needs 39 to pass radius.
// Tested to differ 6 of 883, skeleton gap 3, with six devices, taken out since
// they stop short of 0. Re-apply together, on this draft:
// - first loop as a goto loop, no loop notes, so loop.c leaves the set alone:
//   "i = 0; never = 0; top: ... wait(1); i++; if (i <= 191) goto top;"
// - dead test ahead of the set, stops PRE, folded by the second cprop pass:
//   "if (never == 0) pos = posBuf;", no posAddress
// - twice at the top of the third loop, adds refs and emits nothing:
//   asm("" : : "r"(start), "r"(target), "r"(actor));
// - empty do { } while (0): before that dead test (keeps add rN, sp, #12 joined),
//   after "flags = ...", between "angle = 0xb000" and "i = 0", and before
//   "actor->angle = 0x8000" (ends CSE's path, keeps the pooled zero).
// Three asms lift target over i and lose r7, so two is the count.
// Those 6 are swapped pairs and look like the schedule model, not source:
// - second setup: listing strb; mov r2, sl; ldr r3, [r1, #8]; movs r7, #0; str.
//   thumb_load_stored_back_p exempts that load from the load-after-store wait
//   because target[2]'s later str r3, [r1, #8] has the same text, a false
//   read-modify-write. A question for Pascal (K2), not for this draft.
// - first loop: listing adds r3, r3, r1 before lsls r0, r0, #5, then str r0 and
//   str r3 in that order. A temporary, a barrier, a step local and << all fail.
// Tried: 8-byte struct for angle and radius, plain arrays with no pointers, plain
// position array, a pos per loop, operand asms in the second setup.
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
  s32 *start, *target, *pos, *posAddress;
  s32 radius, angle;
  u8 *flags;
  s32 result, i, phase, sine, answer, zero;

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
  flags = &actor->motionFlags;
  zero = 0;
  *flags = zero;
  start = startBuf;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target = targetBuf;
  target[0] = 0x740000;
  target[1] = zero;
  target[2] = 0x6e0000;
  angle = 0xb000;
  posAddress = posBuf;
  for (i = 0; i <= 191; i++) {
    phase = i * 32768 / 192;
    radius = Math_Sine(phase) * 32;
    angle += 512;
    pos = posAddress;
    pos[0] = start[0] + (target[0] - start[0]) * i / 192;
    phase = Math_Sine(phase);
    pos[1] = start[1] + (target[1] - start[1]) * i / 192 + phase * 56;
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
  *flags = 0;
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
