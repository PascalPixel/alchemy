// Draft, differ N of M: XXX. Left:
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern const u8 gData_02006050[];
extern const u8 gData_02006054[];
extern const u8 gData_0200605a[];
extern const u8 gData_02006064[];
extern const u8 gData_0200621c[];
extern const u8 gData_02006068[];
extern const u8 gData_02006072[];
extern const u8 gData_0200607e[];
s32 Object_GetById();
void GameFlag_SetBit();
void Func_02002770();
void WaitFrames();
void Func_02005d84(void);
void Scheduler_AddOrUpdateCallback();
void Func_02004eb0();
s32 GameFlag_Test();
void Func_02005e6c();
void Func_02005f6c();
void ObjectMotion_SetActionVariant();
void Object_SetPartAttribute();
void Func_02001084();
void Func_02005d9c();
void Func_02001420();
void ObjectDispatch_SetSingleChildFlags();
void Func_0200505c(void);
void Func_02004cc8(void);
void Func_02005f4c(void);
void Func_02005f54();
void Func_02005f5c();
void Func_02005dac();
void Func_0200062c();
void Object_SetActionCallback();
void Func_02000908();
void Value1(Object_GetById, 12)();

void Func_020051f8(void) {
    str [ldr(0x3000000+108)+428 +0] = 516   ; r2
    sub sp, #8
  Value1(Object_GetById, gPartyState.currentOwner); // ret
    orrs r3, r2
    strb [ret(Object_GetById)+35 +0] = ?   ; r3
  Call1(GameFlag_SetBit, 0x300);
  Call1(GameFlag_SetBit, 0x89f);
    cmp ldrsh(gPartyState+0x1e0+[0]+0), 85
    beq .L_ja_0200d234
    b .L_ja_0200d3fc
.L_ja_0200d234:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
    cmp ldrsh(gPartyState+482+[0]+0), 16
    bne .L_ja_0200d262
    strb [Value1(Object_GetById, gPartyState.currentOwner)+34 +0] = 1   ; r3
  Call1(WaitFrames, 8);
  Func_02005d84();
  Call1(WaitFrames, 1);
.L_ja_0200d262:
    cmp ldrsh(gPartyState+482+[0]+0), 11
    ble .L_ja_0200d26e
    cmp ldrsh(gPartyState+482+[0]+0), 16
    bne .L_ja_0200d28c
.L_ja_0200d26e:
  Call2(Scheduler_AddOrUpdateCallback, Func_02004f3c, 0x480);
    strh [0x4000052 +0] = 0x1008   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call4(Func_02004eb0, ret(Scheduler_AddOrUpdateCallback), ?, 0x4000050, 0x3f10); /* XXX arity */
.L_ja_0200d28c:
    cmp ldrsh(gPartyState+482+[0]+0), 13
    ble .L_ja_0200d2d4
    cmp Value1(GameFlag_Test, 0xa8e), 0
    beq .L_ja_0200d2ce
  Call3(Func_02005e6c, 12, 0x3380000, 0x1480000);
.L_ja_0200d2ce:
  Call1(Func_02005f6c, gData_02006050); /* XXX arity */
.L_ja_0200d2d4:
    strb [Value1(Object_GetById, 10)+85 +0] = 0   ; r5
    str [Value1(Object_GetById, 10) +12] = 0   ; r5
    strb [Value1(Object_GetById, 10)+35 +0] = 2   ; r3
  Call2(ObjectMotion_SetActionVariant, 10, 3);
    cmp Value1(GameFlag_Test, 0x89a), 0
    bne .L_ja_0200d318
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
    b .L_ja_0200d346
.L_ja_0200d318:
  Call3(Func_02001084, Value1(Object_GetById, 10), 8, 0); /* XXX arity */
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 11), 6);
  Call6(Func_02005d9c, 0x40, 78, 18, 105, 3, 5); /* XXX arity */
.L_ja_0200d346:
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_ja_0200d37a
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 8), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 9), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 12), 6);
    b .L_ja_0200d3a4
.L_ja_0200d37a:
  Call3(Func_02001084, Value1(Object_GetById, 8), 180, 0); /* XXX arity */
  Call3(Func_02001084, Value1(Object_GetById, 9), 60, 0); /* XXX arity */
  Call3(Func_02001420, Value1(Object_GetById, 12), 60, 0); /* XXX arity */
.L_ja_0200d3a4:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 8), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 9), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 12), 0);
    cmp ldrsh(gPartyState+482+[0]+0), 11
    beq .L_ja_0200d3dc
.L_ja_0200d3dc:
    cmp Value1(GameFlag_Test, 265), 0
    beq .L_ja_0200d3ec
.L_ja_0200d3ec:
  Func_0200505c(); /* XXX arity */
.L_ja_0200d3fc:
    cmp ?, 86
    bne .L_ja_0200d4e8
    strh [0x4000052 +0] = 0x1008   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call4(Func_02002770, ?, ?, 0x4000050, 0x3f10); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x897), 0
    beq .L_ja_0200d440
  Call3(Func_02005e6c, 9, 0x1480000, 0x1880000);
    cmp Value1(GameFlag_Test, 0x898), 0
    beq .L_ja_0200d45c
  Call3(Func_02005e6c, 11, 0x680000, 0x1880000);
.L_ja_0200d45c:
  Call1(Func_02005f6c, gData_02006054); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_ja_0200d47e
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 8), 6);
    b .L_ja_0200d4ac
.L_ja_0200d47e:
  Call3(Func_02001084, Value1(Object_GetById, 8), 60, 0); /* XXX arity */
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
  Call6(Func_02005d9c, 68, 84, 12, 77, 3, 6); /* XXX arity */
.L_ja_0200d4ac:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 8), 0);
    cmp ldrsh(gPartyState+482+[0]+0), 3
    beq .L_ja_0200d4ca
    b .L_ja_0200dc7c
.L_ja_0200d4ca:
    cmp Value1(GameFlag_Test, 265), 0
    beq .L_ja_0200d4d8
    b .L_ja_0200dc7c
.L_ja_0200d4d8:
  Func_02004cc8(); /* XXX arity */
    b .L_ja_0200dc7c
.L_ja_0200d4e8:
    cmp ?, 87
    beq .L_ja_0200d4f0
    b .L_ja_0200d61c
.L_ja_0200d4f0:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
  Func_02005f4c();
    strh [0x4000052 +0] = 0xc08   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call2(Scheduler_AddOrUpdateCallback, Func_02001690, 0x480);
    cmp Value1(GameFlag_Test, 0x89b), 0
    beq .L_ja_0200d544
  Call3(Func_02005e6c, 8, 0x1080000, 0x2780000);
    cmp Value1(GameFlag_Test, 0x89c), 0
    beq .L_ja_0200d560
  Call3(Func_02005e6c, 11, 0x2680000, 0x2380000);
.L_ja_0200d560:
    cmp Value1(GameFlag_Test, 0x89d), 0
    beq .L_ja_0200d57c
  Call3(Func_02005e6c, 12, 0x1d80000, 0x2080000);
.L_ja_0200d57c:
  Call1(Func_02005f6c, gData_0200605a); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_ja_0200d5b6
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 9), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 13), 6);
    b .L_ja_0200d5e0
.L_ja_0200d5b6:
  Call3(Func_02001084, Value1(Object_GetById, 9), 60, 0); /* XXX arity */
  Call3(Func_02001084, Value1(Object_GetById, 10), 180, 0); /* XXX arity */
  Call3(Func_02001084, Value1(Object_GetById, 13), 200, 0); /* XXX arity */
.L_ja_0200d5e0:
    strb [Value1(Object_GetById, 9)+90 +0] = 0   ; r5
    strb [Value1(Object_GetById, 10)+90 +0] = 0   ; r5
    strb [Value1(Object_GetById, 13)+90 +0] = 0   ; r5
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 9), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 10), 0);
    b .L_ja_0200db9e
.L_ja_0200d61c:
    cmp ?, 88
    beq .L_ja_0200d624
    b .L_ja_0200d794
.L_ja_0200d624:
  Call4(Func_02005f54, ?, ?, ?, ?); /* XXX arity */
  Call3(Func_02005f5c, 0, 11, 12); /* XXX arity */
  Call3(Func_02005f5c, 1, 13, 14); /* XXX arity */
  Call3(Func_02005f5c, 2, 15, 0x10); /* XXX arity */
  Call3(Func_02005f5c, 3, 17, 18); /* XXX arity */
  Call3(Func_02005f5c, 4, 19, 20); /* XXX arity */
    strb [Value1(Object_GetById, 18)+98 +0] = 1   ; r3
    cmp Value1(GameFlag_Test, 769), 0
    beq .L_ja_0200d6c6
  Call6(Func_02005d9c, 69, 0x40, 29, 85, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 46, 20, 5, 5, 29, 21); /* XXX arity */
  Call6(Func_02005dac, 0x20, 89, 1, 1, 31, 89); /* XXX arity */
  Call6(Func_02005dac, 50, 86, 1, 1, 33, 87); /* XXX arity */
    b .L_ja_0200d716
.L_ja_0200d6c6:
  Call6(Func_02005d9c, 0x40, 0x40, 29, 85, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 52, 20, 5, 5, 29, 21); /* XXX arity */
  Call6(Func_02005dac, 33, 88, 1, 1, 33, 87); /* XXX arity */
  Call6(Func_02005dac, 0x30, 88, 1, 1, 31, 89); /* XXX arity */
.L_ja_0200d716:
    cmp Value1(GameFlag_Test, 0x300), 0
    beq .L_ja_0200d770
  Call6(Func_02005d9c, 90, 25, 86, 25, 4, 3); /* XXX arity */
  Call6(Func_02005d9c, 21, 88, 24, 89, 1, 2); /* XXX arity */
  Call6(Func_02005dac, 23, 92, 2, 1, 23, 89); /* XXX arity */
  Call6(Func_02005dac, 18, 33, 2, 1, 23, 25); /* XXX arity */
.L_ja_0200d770:
    cmp Value1(GameFlag_Test, 0x89e), 0
    beq .L_ja_0200d78c
  Call3(Func_02005e6c, 9, 0x2880000, 0x1280000);
.L_ja_0200d78c:
  Call1(Func_02005f6c, gData_02006064); /* XXX arity */
    b .L_ja_0200dc7c
.L_ja_0200d794:
    cmp ?, 89
    bne .L_ja_0200d816
  Call1(GameFlag_SetBit, 324);
  Func_02002770(); /* XXX arity */
.L_ja_0200d7a8:
    cmp Value1(GameFlag_Test, 514), 0
    beq .L_ja_0200d7e0
  Call6(Func_02005d9c, 65, 1, 0x50, 24, 1, 2); /* XXX arity */
  Call4(Func_0200062c, 0, 0x10, 24, 0xff00); /* XXX arity */
.L_ja_0200d7e0:
    cmp 1, 6
    bne .L_ja_0200d7a8
    strb [Value1(Object_GetById, 8)+85 +0] = 0   ; r3
  Value1(Object_GetById, 8); // ret
    str [Value1(Object_GetById, 8) +20] = 0x100000   ; r3
    str [ret(Object_GetById)+0 +12] = 0x100000   ; r3
  Call2(Object_SetActionCallback, Value1(Object_GetById, 8), 9);
    b .L_ja_0200dc7c
.L_ja_0200d816:
    cmp ?, 90
    beq .L_ja_0200d81e
    b .L_ja_0200d976
.L_ja_0200d81e:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
  Func_02005f54(); /* XXX arity */
  Call3(Func_02005f5c, 0, 11, 12); /* XXX arity */
  Call3(Func_02005f5c, 1, 13, 14); /* XXX arity */
  Call3(Func_02005f5c, 2, 15, 0x10); /* XXX arity */
    cmp Value1(GameFlag_Test, 770), 0
    bne .L_ja_0200d8b8
  Call6(Func_02005d9c, 69, 0x40, 0x20, 79, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 49, 15, 5, 5, 0x20, 15); /* XXX arity */
  Call6(Func_02005dac, 33, 82, 1, 1, 34, 82); /* XXX arity */
  Call6(Func_02005dac, 52, 81, 1, 1, 36, 81); /* XXX arity */
    b .L_ja_0200d908
.L_ja_0200d8b8:
  Call6(Func_02005d9c, 0x40, 0x40, 0x20, 79, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 49, 21, 5, 5, 0x20, 15); /* XXX arity */
  Call6(Func_02005dac, 35, 81, 1, 1, 36, 81); /* XXX arity */
  Call6(Func_02005dac, 50, 82, 1, 1, 34, 82); /* XXX arity */
.L_ja_0200d908:
    cmp Value1(GameFlag_Test, 771), 0
    beq .L_ja_0200d924
  Call3(Func_02005e6c, 8, 0x2080000, 0x1280000);
.L_ja_0200d924:
    cmp Value1(GameFlag_Test, 0x89f), 0
    bne .L_ja_0200d934
    b .L_ja_0200dc7c
.L_ja_0200d934:
  Call3(Func_02005e6c, 8, 0x1380000, 0x1280000);
  Call2(ObjectMotion_SetActionVariant, 8, 2);
    strb [Value1(Object_GetById, 8)+85 +0] = 2   ; r3
    str [Value1(Object_GetById, 8) +20] = 0xffe00000   ; r3
  Call6(Func_02005dac, 19, 0x10, 1, 1, 19, 18); /* XXX arity */
    b .L_ja_0200dc7c
.L_ja_0200d976:
    cmp ?, 91
    beq .L_ja_0200d97e
    b .L_ja_0200da90
.L_ja_0200d97e:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
    strb [Value1(Object_GetById, 12)+98 +0] = 1   ; r6
    strb [Value1(Object_GetById, 13)+98 +0] = 1   ; r6
    strb [Value1(Object_GetById, 14)+98 +0] = 1   ; r6
    strb [Value1(Object_GetById, 15)+98 +0] = 1   ; r6
    cmp Value1(GameFlag_Test, 265), 0
    bne .L_ja_0200d9cc
  Call1(GameFlag_SetBit, 513);
  Call1(GameFlag_SetBit, 514);
.L_ja_0200d9cc:
  Call1(Func_02000908, gData_0200621c); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x8d0), 0
    beq .L_ja_0200d9ec
  Call3(Func_02005e6c, 8, 0x1580000, 0x2b80000);
.L_ja_0200d9ec:
  Call1(Func_02005f6c, gData_02006068); /* XXX arity */
    cmp Value1(GameFlag_Test, 772), 0
    beq .L_ja_0200da36
  Call6(Func_02005d9c, 39, 82, 38, 82, 1, 3); /* XXX arity */
  Call6(Func_02005dac, 39, 82, 1, 1, 38, 82); /* XXX arity */
  Call6(Func_02005dac, 38, 19, 1, 1, 38, 18); /* XXX arity */
.L_ja_0200da36:
    cmp Value1(GameFlag_Test, 773), 0
    bne .L_ja_0200da46
    b .L_ja_0200dc7c
.L_ja_0200da46:
  Call6(Func_02005d9c, 39, 82, 38, 99, 1, 3); /* XXX arity */
  Call6(Func_02005dac, 38, 98, 1, 1, 38, 99); /* XXX arity */
  Call6(Func_02005dac, 38, 35, 1, 1, 38, 37); /* XXX arity */
    b .L_ja_0200dc7c
.L_ja_0200da90:
    cmp ?, 92
    beq .L_ja_0200da98
    b .L_ja_0200dbaa
.L_ja_0200da98:
  Func_02005f4c();
    strh [0x4000052 +0] = 0xc08   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call2(Scheduler_AddOrUpdateCallback, Func_02001690, 0x480);
    cmp Value1(GameFlag_Test, 0x8d1), 0
    beq .L_ja_0200dae8
  Call3(Func_02005e6c, 8, 0x1180000, 0x1d80000);
    cmp Value1(GameFlag_Test, 0x8d2), 0
    beq .L_ja_0200db04
  Call3(Func_02005e6c, 9, 0x1680000, 0x3480000);
.L_ja_0200db04:
    cmp Value1(GameFlag_Test, 0xa8f), 0
    beq .L_ja_0200db28
  Call3(Func_02005e6c, 12, 0xb80000, 0x2680000);
  Call1(GameFlag_SetBit, 0x200);
.L_ja_0200db28:
  Call2(ObjectMotion_SetActionVariant, 9, 2);
  Call1(Func_02005f6c, gData_02006072); /* XXX arity */
  Value1(Object_GetById, 8); // ret
  Call4(Func_0200062c, 2, (ldr(Value1(Object_GetById, 8)+8) >> 20)+0, (ldr(Value1(Object_GetById, 8)+16) >> 20), 0); /* XXX arity */
  Value1(Object_GetById, 9); // ret
  Call4(Func_0200062c, 2, (ldr(Value1(Object_GetById, 9)+8) >> 20)+0, (ldr(Value1(Object_GetById, 9)+16) >> 20), 0); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_ja_0200db8e
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 11), 6);
    b .L_ja_0200db9c
.L_ja_0200db8e:
  Call3(Func_02001084, Value1(Object_GetById, 11), 8, 0); /* XXX arity */
.L_ja_0200db9c:
.L_ja_0200db9e:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, ?), 0);
    b .L_ja_0200dc7c
.L_ja_0200dbaa:
    cmp ?, 93
    bne .L_ja_0200dc7c
  Call4(Func_02002770, ?, ?, ?, 93); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x8d3), 0
    beq .L_ja_0200dbd0
  Call3(Func_02005e6c, 9, 0x1680000, 0x1080000);
.L_ja_0200dbd0:
  Call1(Func_02005f6c, gData_0200607e); /* XXX arity */
  Value1(Object_GetById, 10); // ret
  Call4(Func_0200062c, 2, (ldr(Value1(Object_GetById, 10)+8) >> 20)+0, (ldr(Value1(Object_GetById, 10)+16) >> 20), 0); /* XXX arity */
    strb [Value1(Object_GetById, 10)+90 +0] = 0   ; r3
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_ja_0200dc1c
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
    b .L_ja_0200dc2a
.L_ja_0200dc1c:
  Call3(Func_02001084, Value1(Object_GetById, 10), 60, 0); /* XXX arity */
.L_ja_0200dc2a:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 10), 0);
    cmp Value1(GameFlag_Test, 774), 0
    beq .L_ja_0200dc7c
  Call6(Func_02005d9c, 15, 0x50, 15, 77, 2, 3); /* XXX arity */
  Call6(Func_02005dac, 15, 0x50, 1, 1, 15, 79); /* XXX arity */
  Call6(Func_02005dac, 17, 15, 2, 1, 15, 15); /* XXX arity */
.L_ja_0200dc7c:
    sub sp, #-8
	.else
    str [ldr(0x3000000+108)+428 +0] = 516   ; r2
    sub sp, #8
  Value1(Object_GetById, gPartyState.currentOwner); // ret
    orrs r3, r2
    strb [ret(Object_GetById)+35 +0] = ?   ; r3
  Call1(GameFlag_SetBit, 0x300);
  Call1(GameFlag_SetBit, 0x89f);
    cmp ldrsh(gPartyState+0x1e0+[0]+0), 85
    beq .L_0200d248
    b .L_0200d424
.L_0200d248:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
    cmp ldrsh(gPartyState+482+[0]+0), 16
    bne .L_0200d282
    cmp Value1(GameFlag_Test, 265), 0
    bne .L_0200d282
    strb [Value1(Object_GetById, gPartyState.currentOwner)+34 +0] = 1   ; r3
  Call1(WaitFrames, 8);
  Func_02005d84();
  Call1(WaitFrames, 1);
.L_0200d282:
    cmp ldrsh(gPartyState+482+[0]+0), 11
    ble .L_0200d296
    cmp ldrsh(gPartyState+482+[0]+0), 16
    bne .L_0200d2b4
.L_0200d296:
  Call2(Scheduler_AddOrUpdateCallback, Func_02004f3c, 0x480);
    strh [0x4000052 +0] = 0x1008   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call4(Func_02004eb0, ret(Scheduler_AddOrUpdateCallback), ?, 0x4000050, 0x3f10); /* XXX arity */
.L_0200d2b4:
    cmp ldrsh(gPartyState+482+[0]+0), 13
    ble .L_0200d2fc
    cmp Value1(GameFlag_Test, 0xa8e), 0
    beq .L_0200d2f6
  Call3(Func_02005e6c, 12, 0x3380000, 0x1480000);
.L_0200d2f6:
  Call1(Func_02005f6c, gData_02006050); /* XXX arity */
.L_0200d2fc:
    strb [Value1(Object_GetById, 10)+85 +0] = 0   ; r5
    str [Value1(Object_GetById, 10) +12] = 0   ; r5
    strb [Value1(Object_GetById, 10)+35 +0] = 2   ; r3
  Call2(ObjectMotion_SetActionVariant, 10, 3);
    cmp Value1(GameFlag_Test, 0x89a), 0
    bne .L_0200d340
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
    b .L_0200d36e
.L_0200d340:
  Call3(Func_02001084, Value1(Object_GetById, 10), 8, 0); /* XXX arity */
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 11), 6);
  Call6(Func_02005d9c, 0x40, 78, 18, 105, 3, 5); /* XXX arity */
.L_0200d36e:
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_0200d3a2
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 8), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 9), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 12), 6);
    b .L_0200d3cc
.L_0200d3a2:
  Call3(Func_02001084, Value1(Object_GetById, 8), 180, 0); /* XXX arity */
  Call3(Func_02001084, Value1(Object_GetById, 9), 60, 0); /* XXX arity */
  Call3(Func_02001420, Value1(Object_GetById, 12), 60, 0); /* XXX arity */
.L_0200d3cc:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 8), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 9), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 12), 0);
    cmp ldrsh(gPartyState+482+[0]+0), 11
    beq .L_0200d404
  Call4(.L_0200dca4, ret(ObjectDispatch_SetSingleChildFlags), 0, 482, ldrsh(gPartyState+482+[0]+0)); /* XXX arity */
.L_0200d404:
    cmp Value1(GameFlag_Test, 265), 0
    beq .L_0200d414
  .L_0200dca4(); /* XXX arity */
.L_0200d414:
  Func_0200505c(); /* XXX arity */
  .L_0200dca4(); /* XXX arity */
.L_0200d424:
    cmp ?, 86
    bne .L_0200d510
    strh [0x4000052 +0] = 0x1008   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call4(Func_02002770, ?, ?, 0x4000050, 0x3f10); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x897), 0
    beq .L_0200d468
  Call3(Func_02005e6c, 9, 0x1480000, 0x1880000);
    cmp Value1(GameFlag_Test, 0x898), 0
    beq .L_0200d484
  Call3(Func_02005e6c, 11, 0x680000, 0x1880000);
.L_0200d484:
  Call1(Func_02005f6c, gData_02006054); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_0200d4a6
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 8), 6);
    b .L_0200d4d4
.L_0200d4a6:
  Call3(Func_02001084, Value1(Object_GetById, 8), 60, 0); /* XXX arity */
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
  Call6(Func_02005d9c, 68, 84, 12, 77, 3, 6); /* XXX arity */
.L_0200d4d4:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 8), 0);
    cmp ldrsh(gPartyState+482+[0]+0), 3
    beq .L_0200d4f2
    b .L_0200dca4
.L_0200d4f2:
    cmp Value1(GameFlag_Test, 265), 0
    beq .L_0200d500
    b .L_0200dca4
.L_0200d500:
  Func_02004cc8(); /* XXX arity */
    b .L_0200dca4
.L_0200d510:
    cmp ?, 87
    beq .L_0200d518
    b .L_0200d644
.L_0200d518:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
  Func_02005f4c();
    strh [0x4000052 +0] = 0xc08   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call2(Scheduler_AddOrUpdateCallback, Func_02001690, 0x480);
    cmp Value1(GameFlag_Test, 0x89b), 0
    beq .L_0200d56c
  Call3(Func_02005e6c, 8, 0x1080000, 0x2780000);
    cmp Value1(GameFlag_Test, 0x89c), 0
    beq .L_0200d588
  Call3(Func_02005e6c, 11, 0x2680000, 0x2380000);
.L_0200d588:
    cmp Value1(GameFlag_Test, 0x89d), 0
    beq .L_0200d5a4
  Call3(Func_02005e6c, 12, 0x1d80000, 0x2080000);
.L_0200d5a4:
  Call1(Func_02005f6c, gData_0200605a); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_0200d5de
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 9), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 13), 6);
    b .L_0200d608
.L_0200d5de:
  Call3(Func_02001084, Value1(Object_GetById, 9), 60, 0); /* XXX arity */
  Call3(Func_02001084, Value1(Object_GetById, 10), 180, 0); /* XXX arity */
  Call3(Func_02001084, Value1(Object_GetById, 13), 200, 0); /* XXX arity */
.L_0200d608:
    strb [Value1(Object_GetById, 9)+90 +0] = 0   ; r5
    strb [Value1(Object_GetById, 10)+90 +0] = 0   ; r5
    strb [Value1(Object_GetById, 13)+90 +0] = 0   ; r5
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 9), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 10), 0);
    b .L_0200dbc6
.L_0200d644:
    cmp ?, 88
    beq .L_0200d64c
    b .L_0200d7bc
.L_0200d64c:
  Call4(Func_02005f54, ?, ?, ?, ?); /* XXX arity */
  Call3(Func_02005f5c, 0, 11, 12); /* XXX arity */
  Call3(Func_02005f5c, 1, 13, 14); /* XXX arity */
  Call3(Func_02005f5c, 2, 15, 0x10); /* XXX arity */
  Call3(Func_02005f5c, 3, 17, 18); /* XXX arity */
  Call3(Func_02005f5c, 4, 19, 20); /* XXX arity */
    strb [Value1(Object_GetById, 18)+98 +0] = 1   ; r3
    cmp Value1(GameFlag_Test, 769), 0
    beq .L_0200d6ee
  Call6(Func_02005d9c, 69, 0x40, 29, 85, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 46, 20, 5, 5, 29, 21); /* XXX arity */
  Call6(Func_02005dac, 0x20, 89, 1, 1, 31, 89); /* XXX arity */
  Call6(Func_02005dac, 50, 86, 1, 1, 33, 87); /* XXX arity */
    b .L_0200d73e
.L_0200d6ee:
  Call6(Func_02005d9c, 0x40, 0x40, 29, 85, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 52, 20, 5, 5, 29, 21); /* XXX arity */
  Call6(Func_02005dac, 33, 88, 1, 1, 33, 87); /* XXX arity */
  Call6(Func_02005dac, 0x30, 88, 1, 1, 31, 89); /* XXX arity */
.L_0200d73e:
    cmp Value1(GameFlag_Test, 0x300), 0
    beq .L_0200d798
  Call6(Func_02005d9c, 90, 25, 86, 25, 4, 3); /* XXX arity */
  Call6(Func_02005d9c, 21, 88, 24, 89, 1, 2); /* XXX arity */
  Call6(Func_02005dac, 23, 92, 2, 1, 23, 89); /* XXX arity */
  Call6(Func_02005dac, 18, 33, 2, 1, 23, 25); /* XXX arity */
.L_0200d798:
    cmp Value1(GameFlag_Test, 0x89e), 0
    beq .L_0200d7b4
  Call3(Func_02005e6c, 9, 0x2880000, 0x1280000);
.L_0200d7b4:
  Call1(Func_02005f6c, gData_02006064); /* XXX arity */
    b .L_0200dca4
.L_0200d7bc:
    cmp ?, 89
    bne .L_0200d83e
  Call1(GameFlag_SetBit, 324);
  Func_02002770(); /* XXX arity */
.L_0200d7d0:
    cmp Value1(GameFlag_Test, 514), 0
    beq .L_0200d808
  Call6(Func_02005d9c, 65, 1, 0x50, 24, 1, 2); /* XXX arity */
  Call4(Func_0200062c, 0, 0x10, 24, 0xff00); /* XXX arity */
.L_0200d808:
    cmp 1, 6
    bne .L_0200d7d0
    strb [Value1(Object_GetById, 8)+85 +0] = 0   ; r3
  Value1(Object_GetById, 8); // ret
    str [Value1(Object_GetById, 8) +20] = 0x100000   ; r3
    str [ret(Object_GetById)+0 +12] = 0x100000   ; r3
  Call2(Object_SetActionCallback, Value1(Object_GetById, 8), 9);
    b .L_0200dca4
.L_0200d83e:
    cmp ?, 90
    beq .L_0200d846
    b .L_0200d99e
.L_0200d846:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
  Func_02005f54(); /* XXX arity */
  Call3(Func_02005f5c, 0, 11, 12); /* XXX arity */
  Call3(Func_02005f5c, 1, 13, 14); /* XXX arity */
  Call3(Func_02005f5c, 2, 15, 0x10); /* XXX arity */
    cmp Value1(GameFlag_Test, 770), 0
    bne .L_0200d8e0
  Call6(Func_02005d9c, 69, 0x40, 0x20, 79, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 49, 15, 5, 5, 0x20, 15); /* XXX arity */
  Call6(Func_02005dac, 33, 82, 1, 1, 34, 82); /* XXX arity */
  Call6(Func_02005dac, 52, 81, 1, 1, 36, 81); /* XXX arity */
    b .L_0200d930
.L_0200d8e0:
  Call6(Func_02005d9c, 0x40, 0x40, 0x20, 79, 5, 7); /* XXX arity */
  Call6(Func_02005dac, 49, 21, 5, 5, 0x20, 15); /* XXX arity */
  Call6(Func_02005dac, 35, 81, 1, 1, 36, 81); /* XXX arity */
  Call6(Func_02005dac, 50, 82, 1, 1, 34, 82); /* XXX arity */
.L_0200d930:
    cmp Value1(GameFlag_Test, 771), 0
    beq .L_0200d94c
  Call3(Func_02005e6c, 8, 0x2080000, 0x1280000);
.L_0200d94c:
    cmp Value1(GameFlag_Test, 0x89f), 0
    bne .L_0200d95c
    b .L_0200dca4
.L_0200d95c:
  Call3(Func_02005e6c, 8, 0x1380000, 0x1280000);
  Call2(ObjectMotion_SetActionVariant, 8, 2);
    strb [Value1(Object_GetById, 8)+85 +0] = 2   ; r3
    str [Value1(Object_GetById, 8) +20] = 0xffe00000   ; r3
  Call6(Func_02005dac, 19, 0x10, 1, 1, 19, 18); /* XXX arity */
    b .L_0200dca4
.L_0200d99e:
    cmp ?, 91
    beq .L_0200d9a6
    b .L_0200dab8
.L_0200d9a6:
  Call4(Func_02002770, ?, ?, ?, ?); /* XXX arity */
    strb [Value1(Object_GetById, 12)+98 +0] = 1   ; r6
    strb [Value1(Object_GetById, 13)+98 +0] = 1   ; r6
    strb [Value1(Object_GetById, 14)+98 +0] = 1   ; r6
    strb [Value1(Object_GetById, 15)+98 +0] = 1   ; r6
    cmp Value1(GameFlag_Test, 265), 0
    bne .L_0200d9f4
  Call1(GameFlag_SetBit, 513);
  Call1(GameFlag_SetBit, 514);
.L_0200d9f4:
  Call1(Func_02000908, gData_0200621c); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x8d0), 0
    beq .L_0200da14
  Call3(Func_02005e6c, 8, 0x1580000, 0x2b80000);
.L_0200da14:
  Call1(Func_02005f6c, gData_02006068); /* XXX arity */
    cmp Value1(GameFlag_Test, 772), 0
    beq .L_0200da5e
  Call6(Func_02005d9c, 39, 82, 38, 82, 1, 3); /* XXX arity */
  Call6(Func_02005dac, 39, 82, 1, 1, 38, 82); /* XXX arity */
  Call6(Func_02005dac, 38, 19, 1, 1, 38, 18); /* XXX arity */
.L_0200da5e:
    cmp Value1(GameFlag_Test, 773), 0
    bne .L_0200da6e
    b .L_0200dca4
.L_0200da6e:
  Call6(Func_02005d9c, 39, 82, 38, 99, 1, 3); /* XXX arity */
  Call6(Func_02005dac, 38, 98, 1, 1, 38, 99); /* XXX arity */
  Call6(Func_02005dac, 38, 35, 1, 1, 38, 37); /* XXX arity */
    b .L_0200dca4
.L_0200dab8:
    cmp ?, 92
    beq .L_0200dac0
    b .L_0200dbd2
.L_0200dac0:
  Func_02005f4c();
    strh [0x4000052 +0] = 0xc08   ; r3
    strh [0x4000050 +0] = 0x3f10   ; r3
  Call2(Scheduler_AddOrUpdateCallback, Func_02001690, 0x480);
    cmp Value1(GameFlag_Test, 0x8d1), 0
    beq .L_0200db10
  Call3(Func_02005e6c, 8, 0x1180000, 0x1d80000);
    cmp Value1(GameFlag_Test, 0x8d2), 0
    beq .L_0200db2c
  Call3(Func_02005e6c, 9, 0x1680000, 0x3480000);
.L_0200db2c:
    cmp Value1(GameFlag_Test, 0xa8f), 0
    beq .L_0200db50
  Call3(Func_02005e6c, 12, 0xb80000, 0x2680000);
  Call1(GameFlag_SetBit, 0x200);
.L_0200db50:
  Call2(ObjectMotion_SetActionVariant, 9, 2);
  Call1(Func_02005f6c, gData_02006072); /* XXX arity */
  Value1(Object_GetById, 8); // ret
  Call4(Func_0200062c, 2, (ldr(Value1(Object_GetById, 8)+8) >> 20)+0, (ldr(Value1(Object_GetById, 8)+16) >> 20), 0); /* XXX arity */
  Value1(Object_GetById, 9); // ret
  Call4(Func_0200062c, 2, (ldr(Value1(Object_GetById, 9)+8) >> 20)+0, (ldr(Value1(Object_GetById, 9)+16) >> 20), 0); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_0200dbb6
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 11), 6);
    b .L_0200dbc4
.L_0200dbb6:
  Call3(Func_02001084, Value1(Object_GetById, 11), 8, 0); /* XXX arity */
.L_0200dbc4:
.L_0200dbc6:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, ?), 0);
    b .L_0200dca4
.L_0200dbd2:
    cmp ?, 93
    bne .L_0200dca4
  Call4(Func_02002770, ?, ?, ?, 93); /* XXX arity */
    cmp Value1(GameFlag_Test, 0x8d3), 0
    beq .L_0200dbf8
  Call3(Func_02005e6c, 9, 0x1680000, 0x1080000);
.L_0200dbf8:
  Call1(Func_02005f6c, gData_0200607e); /* XXX arity */
  Value1(Object_GetById, 10); // ret
  Call4(Func_0200062c, 2, (ldr(Value1(Object_GetById, 10)+8) >> 20)+0, (ldr(Value1(Object_GetById, 10)+16) >> 20), 0); /* XXX arity */
    strb [Value1(Object_GetById, 10)+90 +0] = 0   ; r3
    cmp Value1(GameFlag_Test, 0x899), 0
    bne .L_0200dc44
  Call2(Object_SetPartAttribute, Value1(Object_GetById, 10), 6);
    b .L_0200dc52
.L_0200dc44:
  Call3(Func_02001084, Value1(Object_GetById, 10), 60, 0); /* XXX arity */
.L_0200dc52:
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 10), 0);
    cmp Value1(GameFlag_Test, 774), 0
    beq .L_0200dca4
  Call6(Func_02005d9c, 15, 0x50, 15, 77, 2, 3); /* XXX arity */
  Call6(Func_02005dac, 15, 0x50, 1, 1, 15, 79); /* XXX arity */
  Call6(Func_02005dac, 17, 15, 2, 1, 15, 15); /* XXX arity */
.L_0200dca4:
	.endif
}
