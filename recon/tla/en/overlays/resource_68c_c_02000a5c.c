// Draft, differ 218 of 883, skeleton gap 43 removed, 39 added (tla-en, 2026-11-01)
// One cause holds nearly all of the gap: the allocator. The stack frame is three
// s32[3] buffers (start at sp+36, target at sp+24, position at sp+12) with three
// spilled pseudos below them: radius sp+8, angle sp+4, flags sp+0, which is why
// they are declared in that order. The listing keeps the actor in r11; here
// radius takes r11 and the actor spills, since global.c ranks radius (8 refs, 83
// insns: 0.289) over the actor (35 refs, 671 insns: 0.261). One ref fewer or ten
// more live insns on radius would turn it round; splitting radius per loop hands
// it to local-alloc and is worse, temporaries change no live length, and memory
// angle and radius go through a CSE'd pointer. The bl far branch at the head
// should return with the instruction count, the jump sits just inside b range.
// Second cause: the listing sets position inside the first loop and reuses it in
// the second; PRE hoists that set to the preheader and copy-prop splits the
// pseudo, so it is set before the loop here (a goto loop keeps the same hoist).
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
  u8 *flags;
  s32 result, i, phase, base, delta, answer;

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
  Call2((void (*)())Actor_SetModeById, 11, 3);
  flags = &actor->motionFlags;
  *flags = 0;
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
  i = 0;
  do {
    phase = i * 32768 / 192;
    radius = Math_Sine(phase) * 32;
    angle += 512;
    base = start[0];
    delta = target[0] - base;
    base += delta * i / 192;
    pos[0] = base;
    phase = Math_Sine(phase);
    base = start[1];
    delta = target[1] - base;
    base += delta * i / 192;
    base += phase * 56;
    pos[1] = base;
    base = start[2];
    delta = target[2] - base;
    base += delta * i / 192;
    pos[2] = base;
    Vector_AddPolarOffsetFar(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->angle = angle + 16384;
    if (i == 128) {
      Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
    }
    i++;
    Call1((void (*)())wait, 1);
  } while (i <= 191);
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
  i = 0;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x840000;
  target[1] = 0x200000;
  target[2] = 0x640000;
  angle = 0x4200;
  do {
    radius = Math_Sine(i * 16384 / 192) * 40;
    angle += 768;
    base = start[0];
    delta = target[0] - base;
    base += delta * i / 192;
    pos[0] = base;
    phase = Math_Sine(i * 32768 / 192);
    base = start[1];
    delta = target[1] - base;
    base += delta * i / 192;
    base += phase * 32;
    pos[1] = base;
    base = start[2];
    delta = target[2] - base;
    base += delta * i / 192;
    pos[2] = base;
    Vector_AddPolarOffsetFar(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->angle = angle + 16384;
    if (i == 153) {
      Call3((void (*)())Actor_ArmCallback, 10, 0x7000, 0);
    }
    i++;
    Call1((void (*)())wait, 1);
  } while (i <= 191);
  Call3((void (*)())Actor_ArmCallback, 10, 0xb000, 0);
  actor->angle = 0xd000;
  start[0] = actor->x;
  i = 0;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x5a0000;
  target[1] = 0x40000;
  target[2] = 0x400000;
  do {
    base = start[0];
    actor->x = base + (target[0] - base) * i / 16;
    base = start[1];
    actor->y = base + (target[1] - base) * i / 16;
    base = start[2];
    actor->z = base + (target[2] - base) * i / 16;
    i++;
    Call1((void (*)())wait, 1);
  } while (i <= 15);
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
