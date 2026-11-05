// Draft of overlay 674's Func_020014e0, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02003e64();
s32 Func_02003fa4();
s32 PartyInventory_FindOwner();
s32 Actor_GetById();
s32 GameFlag_Test();
s32 Actor_SetSpeedParameters();
s32 Actor_SetPositionAndReset();
s32 Actor_ArmCallback();
s32 Func_02003f3c();
s32 Func_02003f24();
s32 Func_02003f2c();
s32 Func_02003fc4();
s32 Event_AskYesNoWithActorAnimation();
s32 FieldEvent_Wait();
s32 Func_02003df4();
s32 Actor_SetSingleChildFlags();
s32 Func_02003de4();
s32 Func_02003dec();
s32 Func_02003fbc();
s32 Func_02003f54();
s32 Actor_Jump();
s32 Func_0200129c();
s32 Func_02003dfc();
s32 GameFlag_SetBit();
s32 Actor_SetAngleToward();
s32 HeapSlot_AllocEwram();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_SetPositionAndCommit();
s32 Actor_SetPartAttribute();
s32 Actor_SetModeById();
s32 Actor_ResetAndSetPosition();
s32 Func_020034ec();
s32 Actor_ResetAndSetPositionInMode2();
s32 Event_FinishScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02003f7c();
s32 Func_02003e6c();
extern u8 gMainRoutineWork[];
extern u8 Func_02000df8[];
extern u8 MsgOhYouRe[];
extern u8 gData_020044f0[];
extern u8 MsgIMSoVery[];
extern u8 MsgWantToGo[];
extern u8 Func_02001278[];
extern u8 MsgIMSoVeryVery[];

void Func_020014e0(s32 p0) {
  s32 v_r0, v_r1, v_r10, v_r11, v_r2, v_r3, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  Call1((void (*)())Func_02003e64, p0);
  Call1((void (*)())Func_02003fa4, 0);
  v_r0 = PartyInventory_FindOwner(0x1c7);
  if (v_r0 != -1)
    goto L_ja_0200950a;
  goto L_ja_020098ac;
L_ja_0200950a:
  ;
  v_r6 = (s32)gMainRoutineWork + 0x214;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r8 = v_r0;
  v_r0 = GameFlag_Test(0x201);
  v_r10 = v_r0;
  if (v_r0 != 0)
    goto L_ja_0200952c;
  goto L_ja_02009694;
L_ja_0200952c:
  ;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call3((void (*)())Actor_SetSpeedParameters, v_r0, 0xcccc, 0x6666);
  v_r7 = (s32)Func_02000df8;
  FIELD_AT(v_r8, s32, 108) = v_r7;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_SetPositionAndReset, v_r0, 0x13e, 0x1a8, v_r8);
  FIELD_AT(v_r8, s32, 108) = 0;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_ArmCallback, v_r0, 0x4000, 0, 0);
  v_r0 = Func_02003f3c(9, 0xd000);
  v_r5 = (s32)MsgOhYouRe;
  Call1((void (*)())Func_02003f24, v_r5);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Func_02003fc4();
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Event_AskYesNoWithActorAnimation(v_r0, 0);
  v_r10 = v_r0;
  if (v_r0 != 0)
    goto L_ja_02009688;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Func_02003f3c(v_r0, 0x8000);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call3((void (*)())Actor_SetSpeedParameters, v_r0, 0x19999, 0xcccc);
  FIELD_AT(v_r8, s32, 108) = v_r7;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r2 = FIELD_AT(v_r0, u8, 0);
  v_r11 = 254;
  v_r3 = 254 & v_r2;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_SetPositionAndReset, v_r0, 0x12e, 0x1a8, v_r3);
  Call1((void (*)())FieldEvent_Wait, 1);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r3 = FIELD_AT(v_r0, u8, 0);
  v_r9 = 1;
  v_r3 = v_r3 | v_r9;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  v_r0 = Func_02003df4(163, 0x1280000, 0, 0x1a80000);
  v_r7 = v_r0;
  v_r0 = Func_02003df4(14, 0x1280000, 0, 0x1a80000);
  v_r5 = v_r0;
  if (v_r7 == 0)
    goto L_ja_0200961e;
  v_r3 = v_r7 + 85;
  FIELD_AT(v_r3, u8, 0) = v_r10;
  v_r0 = Actor_SetSingleChildFlags(v_r7, 0, v_r10, v_r3);
L_ja_0200961e:
  ;
  if (v_r5 == 0)
    goto L_ja_02009640;
  FIELD_AT(v_r5, s32, 28) = 0x20000;
  v_r0 = Actor_SetSingleChildFlags(v_r5, 0);
  v_r0 = Func_02003de4(v_r5, 1);
  Call2((void (*)())Func_02003dec, v_r5, (s32)gData_020044f0);
L_ja_02009640:
  ;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r2 = FIELD_AT(v_r0, u8, 0);
  v_r3 = v_r11 & v_r2;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_SetPositionAndReset, v_r0, 0x13e, 0x1a8, v_r3);
  Call1((void (*)())FieldEvent_Wait, 1);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r3 = FIELD_AT(v_r0, u8, 0);
  v_r3 = v_r3 | v_r9;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  FIELD_AT(v_r8, s32, 108) = v_r10;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_ArmCallback, v_r0, 0x6000, 0, v_r10);
  goto L_ja_0200982c;
L_ja_02009688:
  ;
  v_r0 = Func_02003fbc();
  v_r0 = v_r5 - 1;
  Call1((void (*)())Func_02003f24, v_r0);
  goto L_ja_02009a1c;
L_ja_02009694:
  ;
  Call1((void (*)())Func_02003f24, (s32)MsgIMSoVery);
  Call2((void (*)())Func_02003f2c, 9, 0);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call3((void (*)())Func_02003f54, v_r0, 0x106, 20);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call3((void (*)())Actor_SetSpeedParameters, v_r0, 0xcccc, 0x6666);
  FIELD_AT(v_r8, s32, 108) = (s32)Func_02000df8;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r9 = (s32)Func_02000df8;
  v_r7 = 0;
  Call4((void (*)())Actor_SetPositionAndReset, v_r0, 0x13e, 0x1a8, (s32)Func_02000df8);
  FIELD_AT(v_r8, s32, 108) = v_r7;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Func_02003f3c(v_r0, 0x8000);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call3((void (*)())Actor_SetSpeedParameters, v_r0, 0x19999, 0xcccc);
  FIELD_AT(v_r8, s32, 108) = v_r9;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r2 = FIELD_AT(v_r0, u8, 0);
  v_r3 = 254 & v_r2;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_SetPositionAndReset, v_r0, 0x12e, 0x1a8, v_r3);
  Call1((void (*)())FieldEvent_Wait, 1);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r3 = FIELD_AT(v_r0, u8, 0);
  v_r11 = 1;
  v_r3 = v_r3 | v_r11;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  v_r0 = Func_02003df4(163, 0x1280000, 0, 0x1a80000);
  v_r7 = v_r0;
  v_r0 = Func_02003df4(14, 0x1280000, 0, 0x1a80000);
  v_r5 = v_r0;
  if (v_r7 == 0)
    goto L_ja_0200976a;
  v_r3 = v_r7 + 85;
  FIELD_AT(v_r3, u8, 0) = v_r10;
  v_r0 = Actor_SetSingleChildFlags(v_r7, 0, v_r10, v_r3);
L_ja_0200976a:
  ;
  if (v_r5 == 0)
    goto L_ja_0200978c;
  FIELD_AT(v_r5, s32, 28) = 0x20000;
  v_r0 = Actor_SetSingleChildFlags(v_r5, 0);
  v_r0 = Func_02003de4(v_r5, 1);
  Call2((void (*)())Func_02003dec, v_r5, (s32)gData_020044f0);
L_ja_0200978c:
  ;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r2 = FIELD_AT(v_r0, u8, 0);
  v_r3 = 254 & v_r2;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_SetPositionAndReset, v_r0, 0x13e, 0x1a8, v_r3);
  Call1((void (*)())FieldEvent_Wait, 1);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r0 = v_r0 + 90;
  v_r3 = FIELD_AT(v_r0, u8, 0);
  v_r3 = v_r3 | v_r11;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  FIELD_AT(v_r8, s32, 108) = v_r10;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_ArmCallback, v_r0, 0x4000, 0, v_r10);
  Func_02003f54(9, 0x100, 20);
  v_r0 = Func_02003f3c(9, 0xb000);
  v_r0 = Actor_GetById(9);
  v_r5 = v_r0;
  FIELD_AT(v_r5, s32, 108) = v_r9;
  Call4((void (*)())Actor_Jump, 9, 4, 20, v_r9);
  FIELD_AT(v_r5, s32, 108) = v_r10;
  Call3((void (*)())Func_02003f2c, 9, 0, v_r10);
  v_r0 = Func_02003f3c(9, 0xd000);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Func_02003fc4();
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Event_AskYesNoWithActorAnimation(v_r0, 0);
  if (v_r0 != 0)
    goto L_ja_02009834;
L_ja_0200982c:
  ;
  Call1((void (*)())Func_0200129c, v_r7);
  goto L_ja_02009a3e;
L_ja_02009834:
  ;
  v_r0 = Func_02003fbc();
  Func_02003f54(9, 0x109, 0);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Func_02003f3c(v_r0, 0x8000);
  FIELD_AT(v_r8, s32, 108) = v_r9;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  Call4((void (*)())Actor_SetPositionAndReset, v_r0, 0x12e, 0x1a8, v_r9);
  FIELD_AT(v_r8, s32, 108) = v_r10;
  if (v_r7 == 0)
    goto L_ja_02009886;
  Call1((void (*)())Func_02003dfc, v_r7);
L_ja_02009886:
  ;
  Call1((void (*)())GameFlag_SetBit, 0x201);
  goto L_ja_02009a3e;
L_ja_020098ac:
  ;
  v_r0 = GameFlag_Test(0xaa5);
  if (v_r0 != 0)
    goto L_ja_020098bc;
  goto L_ja_02009a30;
L_ja_020098bc:
  ;
  v_r7 = (s32)gMainRoutineWork + 0x214;
  v_r1 = FIELD_AT(v_r7, s32, 0);
  Call4((void (*)())Actor_SetAngleToward, 9, v_r1, 0, (s32)gMainRoutineWork);
  Call1((void (*)())Func_02003f24, (s32)MsgWantToGo);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Func_02003fc4();
  v_r0 = FIELD_AT(v_r7, s32, 0);
  v_r0 = Event_AskYesNoWithActorAnimation(v_r0, 0);
  if (v_r0 == 0)
    goto L_ja_020098ee;
  goto L_ja_02009a18;
L_ja_020098ee:
  ;
  v_r0 = FIELD_AT(v_r7, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r6 = v_r0;
  v_r0 = Func_02003fbc();
  v_r2 = FIELD_AT(0x3000000, s32, 108);
  v_r5 = 226;
  v_r5 = v_r5 << 1;
  v_r2 = v_r2 + v_r5;
  v_r8 = 0x3000000;
  v_r3 = FIELD_AT(v_r2, u16, 0);
  v_r3 = v_r3 + 1;
  FIELD_AT(v_r2, u16, 0) = v_r3;
  Call4((void (*)())Func_02003f2c, 9, 0, v_r2, v_r3);
  v_r0 = HeapSlot_AllocEwram(88, 0x298);
  v_r0 = v_r0 + 0x295;
  FIELD_AT(v_r0, u8, 0) = 99;
  v_r0 = FIELD_AT(v_r7, s32, 0);
  Call4((void (*)())Motion_SetModeAndWaitAnimation, v_r0, 3, 0x295, 99);
  v_r0 = Func_02003f3c(9, 0x5000);
  v_r0 = FIELD_AT(v_r7, s32, 0);
  Call3((void (*)())Actor_SetSpeedParameters, v_r0, 0x19999, 0xcccc);
  v_r0 = FIELD_AT(v_r7, s32, 0);
  Call3((void (*)())Actor_Jump, v_r0, 6, 0);
  v_r0 = FIELD_AT(v_r7, s32, 0);
  Call3((void (*)())Actor_SetPositionAndCommit, v_r0, 0x128, v_r5);
  v_r0 = FIELD_AT(v_r7, s32, 0);
  FIELD_AT(v_r6, s32, 108) = (s32)Func_02001278;
  v_r0 = Actor_GetById(v_r0);
  Actor_SetPartAttribute();
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Func_02003fc4();
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0xcccc, 0x6666);
  v_r0 = Actor_GetById(9);
  v_r5 = (s32)Func_02000df8;
  FIELD_AT(v_r0, s32, 108) = v_r5;
  Call3((void (*)())Actor_ResetAndSetPosition, 9, 0x120, 0x21a);
  Call1((void (*)())FieldEvent_Wait, 40);
  Func_02003fc4();
  Func_020034ec();
  v_r0 = Actor_GetById(10);
  FIELD_AT(v_r0, s32, 108) = v_r5;
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 10, 0x120, 0x21a);
  Call1((void (*)())FieldEvent_Wait, 20);
  v_r3 = FIELD_AT(v_r8, s32, 108);
  v_r3 = v_r3 + 0x1ac;
  FIELD_AT(v_r3, s32, 0) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  v_r0 = Func_02003fbc();
  Call1((void (*)())Func_02003f7c, 3);
  goto L_ja_02009a3e;
L_ja_02009a18:
  ;
  v_r0 = Func_02003fbc();
L_ja_02009a1c:
  ;
  Call2((void (*)())Func_02003f2c, 9, 0);
  v_r0 = Func_02003f3c(9, 0x5000);
  goto L_ja_02009a3e;
L_ja_02009a30:
  ;
  Call1((void (*)())Func_02003f24, (s32)MsgIMSoVeryVery);
  Call2((void (*)())Func_02003f2c, 9, 0);
L_ja_02009a3e:
  ;
  Func_02003e6c();
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
}
