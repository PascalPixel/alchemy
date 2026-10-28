// Draft, differ 273 of 883: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Object_GetById();
s32 GameFlag_Test();
void GameFlag_SetBit();
void Func_0200282c();
void Func_020028fc();
void Func_020028ac();
void ObjectMotion_SetSpeedParameters();
void Func_020028dc();
void Func_020028bc();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_ArmCallback();
void FieldEvent_WaitFrames();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_CommitPositionAndActivate();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_SetVariantCallback();
void Func_020028e4();
void UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
void Func_02002924();
s32 __divsi3();
s32 Math_Sine();
void Vector_AddPolarOffsetFar();
void WaitFrames();
void Func_02002874();
s32 Func_02002834();

s32 Func_02000a5c(s32 a0, s32 a1, s32 a2) {
  s32 v0, r11, v1, r0, v2, v3, v4, r3, r10, r8, r7, v5, r6, v6, r9, r5, v7, v8,
      v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, r2, v20, v21;
  s32 stack[12];

  /* sub (s32)stack, #48 */
  v0 = Object_GetById(11);
  r11 = v0;
  v1 = GameFlag_Test(0x965);
  r0 = v1;
  if (v1 != 0)
    goto L_02008a86;
  goto L_0200927c;
L_02008a86:
  Call1((void (*)())GameFlag_SetBit, 0x966);
  v2 = Object_GetById(10);
  FIELD_AT(v2, u8, 35) = 1 | (FIELD_AT(v2, u8, 35));
  Func_0200282c();
  Call1((void (*)())Func_020028fc, 0);
  Func_020028ac((s32)MsgLemuriaInteriors10);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Value3((s32 (*)())Func_020028dc, 10, 0x100, 30);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 136, 152);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xa000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x109, 30);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x2000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 4, 8, 16);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x4000, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Value3((s32 (*)())Func_020028dc, 4, 0x100, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 4, 0xa000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 4, -8, -16);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 4, 3);
  Call2((void (*)())Func_020028e4, 4, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 50);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 10, 12, 12);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x101, 30);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 10, 0);
  v3 = Event_AskYesNoWithActorAnimation(4, 0);
  if (v3 == 0) {
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    Call1((void (*)())FieldEvent_WaitFrames, 40);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  }
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x5000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 4, 3);
  Call2((void (*)())Func_020028e4, 4, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 50);
  Value3((s32 (*)())Func_020028dc, 10, 0x101, 30);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 10, 0);
  v4 = Event_AskYesNoWithActorAnimation(4, 0);
  if (v4 == 0) {
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    r3 = (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    Call1((void (*)())FieldEvent_WaitFrames, 40);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  }
  Value3((s32 (*)())Func_020028dc, 10, 0x102, 40);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x106, 40);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 10, 89, 92);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x8000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 10, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x2000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 4);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x8000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Value3((s32 (*)())Func_020028dc, 10, 0x108, 50);
  Call2((void (*)())Object_SetModeById, 10, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call1((void (*)())Func_02002924, 0x26b);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 11, 3);
  Call2((void (*)())Func_020028e4, 11, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 11, 3);
  Call2((void (*)())Func_020028e4, 11, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 50);
  Call2((void (*)())Object_SetModeById, 11, 3);
  stack[0] = r11 + 85;
  FIELD_AT(r11, u8, 85) = 0;
  r10 = 36 + (s32)stack;
  FIELD_AT(r10, s32, 0) = FIELD_AT(r11, s32, 8);
  FIELD_AT(r10, s32, 4) = FIELD_AT(r11, s32, 12);
  FIELD_AT(r10, s32, 8) = FIELD_AT(r11, s32, 16);
  r8 = (s32)((u8 *)stack + 24);
  FIELD_AT(r8, s32, 0) = 0x740000;
  FIELD_AT(r8, s32, 4) = 0;
  FIELD_AT(r8, s32, 8) = 0x6e0000;
  stack[1] = 0xb000;
  r7 = 0;
L_02008ede:
  v5 = __divsi3(r7 << 15, 192);
  r6 = v5;
  v6 = Math_Sine(v5);
  stack[2] = v6 << 5;
  stack[1] = stack[1] + 512;
  r9 = (s32)((u8 *)stack + 12);
  r5 = FIELD_AT(r10, s32, 0);
  v7 = __divsi3(r7 * ((FIELD_AT(r8, s32, 0)) - r5), 192);
  r5 = r5 + v7;
  FIELD_AT(r9, s32, 0) = r5;
  v8 = Math_Sine(r6);
  r5 = FIELD_AT(r10, s32, 4);
  r6 = v8;
  v9 = __divsi3(r7 * ((FIELD_AT(r8, s32, 4)) - r5), 192);
  r5 = r5 + v9;
  r5 = r5 + (((r6 << 3) - r6) << 3);
  FIELD_AT(r9, s32, 4) = r5;
  r5 = FIELD_AT(r10, s32, 8);
  v10 = __divsi3(r7 * ((FIELD_AT(r8, s32, 8)) - r5), 192);
  r5 = r5 + v10;
  FIELD_AT(r9, s32, 8) = r5;
  Vector_AddPolarOffsetFar(stack[2], stack[1], r9);
  FIELD_AT(r11, s32, 8) = FIELD_AT(r9, s32, 0);
  FIELD_AT(r11, s32, 12) = FIELD_AT(r9, s32, 4);
  FIELD_AT(r11, s32, 16) = FIELD_AT(r9, s32, 8);
  FIELD_AT(r11, u16, 6) = stack[1] + 16384;
  if (r7 == 128) {
    Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
  }
  r7 = r7 + 1;
  Call1((void (*)())WaitFrames, 1);
  if (r7 <= 191)
    goto L_02008ede;
  FIELD_AT(r11, u16, 6) = 0x8000;
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 11, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 10, 100, 112);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x6666, 0x3333);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 10, 8, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Object_SetModeById, 11, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Call2((void (*)())Object_SetModeById, 11, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  v11 = Object_GetById(10);
  FIELD_AT(v11, u8, 90) = 254 & (FIELD_AT(v11, u8, 90));
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 10, -8, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  v12 = Object_GetById(10);
  FIELD_AT(v12, u8, 90) = 1 | (FIELD_AT(v12, u8, 90));
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_020028dc, 10, 0x108, 40);
  Call2((void (*)())Object_SetModeById, 11, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  r5 = 0;
  FIELD_AT(stack[0], u8, 0) = r5;
  r7 = 0;
  FIELD_AT(r10, s32, 0) = FIELD_AT(r11, s32, 8);
  FIELD_AT(r10, s32, 4) = FIELD_AT(r11, s32, 12);
  FIELD_AT(r10, s32, 8) = FIELD_AT(r11, s32, 16);
  FIELD_AT(r8, s32, 0) = 0x840000;
  FIELD_AT(r8, s32, 4) = 0x200000;
  FIELD_AT(r8, s32, 8) = 0x640000;
  stack[1] = 0x4200;
  goto L_020090e0;
L_020090e0:
  v13 = __divsi3(r7 << 14, 192);
  v14 = Math_Sine(v13);
  stack[2] = ((v14 << 2) + v14) << 3;
  stack[1] = stack[1] + 768;
  r5 = FIELD_AT(r10, s32, 0);
  v15 = __divsi3(r7 * ((FIELD_AT(r8, s32, 0)) - r5), 192);
  r5 = r5 + v15;
  FIELD_AT(r9, s32, 0) = r5;
  v16 = __divsi3(r7 << 15, 192);
  v17 = Math_Sine(v16);
  r5 = FIELD_AT(r10, s32, 4);
  r6 = v17;
  v18 = __divsi3(r7 * ((FIELD_AT(r8, s32, 4)) - r5), 192);
  r6 = r6 << 5;
  r5 = r5 + v18;
  r5 = r5 + r6;
  FIELD_AT(r9, s32, 4) = r5;
  r5 = FIELD_AT(r10, s32, 8);
  v19 = __divsi3(r7 * ((FIELD_AT(r8, s32, 8)) - r5), 192);
  r5 = r5 + v19;
  FIELD_AT(r9, s32, 8) = r5;
  Vector_AddPolarOffsetFar(stack[2], stack[1], r9);
  FIELD_AT(r11, s32, 8) = FIELD_AT(r9, s32, 0);
  FIELD_AT(r11, s32, 12) = FIELD_AT(r9, s32, 4);
  FIELD_AT(r11, s32, 16) = FIELD_AT(r9, s32, 8);
  FIELD_AT(r11, u16, 6) = stack[1] + 16384;
  if (r7 == 153) {
    Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x7000, 0);
  }
  r7 = r7 + 1;
  Call1((void (*)())WaitFrames, 1);
  if (r7 <= 191)
    goto L_020090e0;
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0xb000, 0);
  FIELD_AT(r11, u16, 6) = 0xd000;
  FIELD_AT(r10, s32, 0) = FIELD_AT(r11, s32, 8);
  r7 = 0;
  FIELD_AT(r10, s32, 4) = FIELD_AT(r11, s32, 12);
  FIELD_AT(r10, s32, 8) = FIELD_AT(r11, s32, 16);
  FIELD_AT(r8, s32, 0) = 0x5a0000;
  FIELD_AT(r8, s32, 4) = 0x40000;
  FIELD_AT(r8, s32, 8) = 0x400000;
L_020091d8:
  r2 = FIELD_AT(r10, s32, 0);
  r3 = ((FIELD_AT(r8, s32, 0)) - (FIELD_AT(r10, s32, 0))) * r7;
  if ((((FIELD_AT(r8, s32, 0)) - (FIELD_AT(r10, s32, 0))) * r7) < 0) {
    r3 = r3 + 15;
  }
  FIELD_AT(r11, s32, 8) = r2 + (r3 >> 4);
  r2 = FIELD_AT(r10, s32, 4);
  r3 = ((FIELD_AT(r8, s32, 4)) - (FIELD_AT(r10, s32, 4))) * r7;
  if ((((FIELD_AT(r8, s32, 4)) - (FIELD_AT(r10, s32, 4))) * r7) < 0) {
    r3 = r3 + 15;
  }
  FIELD_AT(r11, s32, 12) = r2 + (r3 >> 4);
  r2 = FIELD_AT(r10, s32, 8);
  r3 = ((FIELD_AT(r8, s32, 8)) - (FIELD_AT(r10, s32, 8))) * r7;
  if ((((FIELD_AT(r8, s32, 8)) - (FIELD_AT(r10, s32, 8))) * r7) < 0) {
    r3 = r3 + 15;
  }
  FIELD_AT(r11, s32, 16) = r2 + (r3 >> 4);
  r7 = r7 + 1;
  Call1((void (*)())WaitFrames, 1);
  if (r7 <= 15)
    goto L_020091d8;
  Call3((void (*)())Func_02002874, 11, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_020028bc, 0x400a, 0, 10);
  Call2((void (*)())ObjectMotion_SetActionVariant, 10, 2);
  v21 = Func_02002834();
  r0 = v21;
L_0200927c:
  /* add (s32)stack, #48 */
  return r0;
}
