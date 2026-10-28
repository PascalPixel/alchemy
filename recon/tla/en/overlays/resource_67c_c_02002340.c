// Draft, differ 294 of 931: Aqua Rock's map preparation: for each room and
// entrance it sets the water level, places the statues and boulders the flags
// have moved, opens or closes the cells around them, and in the rooms whose
// water drains it shows the drained state once its flag is set. Translated from
// its listing; names and structure still to do.
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

void Func_02003278();
s32 Func_020032d4();
s32 Actor_GetById();
void Func_02003cc4();
s32 GameFlag_Test();
void Func_02003d24();
s32 Func_02003d1c();
void Func_0200349c();
void Func_02003ddc();
void Func_02003ea4();
void Actor_SetSingleChildFlags();
void Func_02003628();
void Func_02003688();
void Func_020036b8();
void Func_020007a8();
void Func_02003420();
void Func_02000b84();
s32 Func_02000b24();
void GameFlag_SetBit();
void Func_02000ad8();
void Func_02003edc();
void Func_02003ee4();

s32 Func_02002340(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r6, r5, r7, r8, r3, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, r0,
      v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26,
      v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, r1, r2;
  s32 stack[3];

  r6 = (s32)&gMainRoutineWork;
  r5 = FIELD_AT(r6, s16, 480);
  r7 = FIELD_AT(r6, s16, 482);
  r8 = 0x3000000;
  FIELD_AT((s32)Ram_HeapSlots->mapWork, u8, 23) =
      (-13 & (FIELD_AT((s32)Ram_HeapSlots->mapWork, u8, 23))) | 4;
  /* sub (s32)stack, #12 */
  Func_02003278();
  if (r5 == (160))
    goto L_0200a380;
  goto L_0200a52a;
L_0200a380:
  r3 = r7 - 3;
  if ((u32)(r7 - 3) <= (u32)1)
    goto L_0200a38e;
  if (r7 == 6)
    goto L_0200a38e;
  if (r7 != 8)
    goto L_0200a39c;
L_0200a38e:
  Func_020032d4(33, 0, -2);
  goto L_0200a4c8;
L_0200a39c:
  if (r7 != 5) {
    if (r7 != 7)
      goto L_0200a462;
  }
  v0 = Actor_GetById(12);
  r6 = v0;
  r5 = 0;
  FIELD_AT(r6, u8, 85) = r5;
  FIELD_AT(r6, s32, 20) = r5;
  FIELD_AT(r6, s32, 12) = r5;
  Call2((void (*)())Func_02003cc4, v0, 0);
  v1 = Actor_GetById(13);
  r6 = v1;
  FIELD_AT(r6, u8, 85) = r5;
  FIELD_AT(r6, s32, 20) = r5;
  FIELD_AT(r6, s32, 12) = r5;
  v2 = GameFlag_Test(0x230);
  r5 = v2;
  if (r5 == 0) {
    Call6((void (*)())Func_02003d24, 104, 80, 16, 10, 104, 40);
    Call6((void (*)())Func_02003d24, 110, 116, 18, 12, 39, 105);
    Call6((void (*)())Func_02003d1c, 64, 32, 64, 32, r5, 32);
    Func_020032d4(33, 0, 0);
  } else {
    Call3((void (*)())Func_020032d4, 33, 4, 0);
    v3 = Actor_GetById(12);
    r6 = v3;
    Call2((void (*)())Func_0200349c, ((FIELD_AT(r6, s32, 8)) >> 20) - 42, 0);
  }
  v4 = GameFlag_Test(0x250);
  if (v4 == 0)
    goto L_0200a4c8;
  Call6((void (*)())Func_02003d1c, 41, 42, 1, 1, 43, 44);
  goto L_0200a4c8;
L_0200a462:
  if ((u32)(r7 - 9) > (u32)1)
    goto L_0200a4c8;
  v5 = GameFlag_Test(0x231);
  r6 = v5;
  if (r6 != 0)
    goto L_0200a4be;
  stack[0] = 77;
  r5 = 32;
  Call6((void (*)())Func_02003d24, 77, 80, 23, 20, 77, r5);
  Call6((void (*)())Func_02003d24, 112, 96, 16, 20, 12, 98);
  Call6((void (*)())Func_02003d1c, 64, 32, 64, 32, r6, r5);
  Func_020032d4(33, 0, 0);
  goto L_0200a4c8;
L_0200a4be:
  Func_020032d4(33, 4, 0);
L_0200a4c8:
  v6 = GameFlag_Test(0x95a);
  if (v6 != 0) {
    Call3((void (*)())Func_02003ddc, 10, 0xb80000, 0xc80000);
    r5 = 12;
    Call6((void (*)())Func_02003d1c, 0, 12, 1, 1, 9, r5);
    Call6((void (*)())Func_02003d1c, 0, 12, 1, 1, 13, r5);
  }
  v7 = GameFlag_Test(0x95b);
  if (v7 != 0)
    goto L_0200a51a;
  goto L_0200abc4;
L_0200a51a:
  Call3((void (*)())Func_02003ddc, 11, 0x3380000, 0xd80000);
  goto L_0200abc4;
L_0200a52a:
  if (r5 != (161))
    goto L_0200a558;
  r3 = r7 - 1;
  if ((u32)(r7 - 1) <= (u32)1) {
    Func_020032d4(33, 0, -2);
  }
  v8 = Actor_GetById(8);
  r6 = v8;
  FIELD_AT(r6, u8, 85) = 0;
  FIELD_AT(r6, s32, 20) = 0;
  FIELD_AT(r6, s32, 12) = 0;
  r3 = 0;
  goto L_0200abc4;
L_0200a558:
  r3 = 162;
  if (r5 != (162))
    goto L_0200a654;
  Call1((void (*)())Func_02003ea4, 0);
  v9 = Actor_GetById(9);
  Call2((void (*)())Actor_SetSingleChildFlags, v9, 0);
  v10 = Actor_GetById(10);
  Call2((void (*)())Actor_SetSingleChildFlags, v10, 0);
  Func_020032d4(33, 8, -2);
  Func_020032d4(40, 8, 8);
  v11 = GameFlag_Test(0xa66);
  r0 = v11;
  if (v11 == 0)
    goto L_0200a5cc;
  Func_02003628(r0, r1, r2);
  Value4((s32 (*)())Func_02003688, 16, 4, 16, 2);
  Value4((s32 (*)())Func_02003688, 22, 4, 16, 0);
  r5 = 5;
L_0200a5be:
  r5 = r5 - 1;
  Call2((void (*)())Func_020036b8, 33, 55);
  if (r5 >= 0)
    goto L_0200a5be;
L_0200a5cc:
  v12 = GameFlag_Test(0x95d);
  if (v12 == 0)
    goto L_0200a62c;
  Call6((void (*)())Func_02003d24, 75, 96, 7, 13, 80, 27);
  r5 = 16;
  r6 = 31;
  Call6((void (*)())Func_02003d24, 91, 96, 7, 10, r5, r6);
  Call6((void (*)())Func_02003d24, 99, 96, 7, 14, r5, 91);
  v13 = Func_02003d1c(43, 0, 5, 9, 17, r6);
  Func_020007a8();
  goto L_0200abc4;
L_0200a62c:
  Call6((void (*)())Func_02003d1c, 17, 32, 5, 1, 17, 31);
  goto L_0200abc4;
L_0200a654:
  r3 = 163;
  if (r5 == (163))
    goto L_0200a65c;
  goto L_0200a7cc;
L_0200a65c:
  Call1((void (*)())Func_02003ea4, 0);
  r3 = r7 - 6;
  if ((u32)(r7 - 6) <= (u32)1)
    goto L_0200a66a;
  goto L_0200a7aa;
L_0200a66a:
  v14 = Actor_GetById(FIELD_AT(r6, s32, 532));
  FIELD_AT(v14, u8, 35) = 32 | (FIELD_AT(v14, u8, 35));
  Value3((s32 (*)())Func_02003420, 12, 5, 4);
  Value3((s32 (*)())Func_02003420, 13, 3, 4);
  Value3((s32 (*)())Func_02003420, 14, 4, 4);
  Func_020032d4(40, 4, 4);
  v15 = GameFlag_Test(0x249);
  if (v15 != 0)
    goto L_0200a724;
  v16 = GameFlag_Test(0xa64);
  if (v16 != 0)
    goto L_0200a724;
  r6 = FIELD_AT(r8, s32, 32);
  stack[0] = 71;
  r5 = 39;
  Call6((void (*)())Func_02003d24, 64, 64, 16, 13, 71, r5);
  v17 = Func_02003d1c(71, 39, 16, 15, 7, r5);
  Func_02000b84();
  FIELD_AT(r6, u8, 262) = 1;
  FIELD_AT((s32)((u8 *)stack + 10), u16, 0) =
      65532 & (FIELD_AT(67108874, u16, 0));
  FIELD_AT(67108874, u16, 0) = (65532 & (FIELD_AT(67108874, u16, 0))) | (1);
  goto L_0200a78c;
L_0200a724:
  v18 = GameFlag_Test(0xa64);
  if (v18 == 0) {
    v19 = GameFlag_Test(0x248);
    if (v19 == 0)
      goto L_0200a78c;
  }
  v20 = GameFlag_Test(0xa64);
  if (v20 != 0) {
    Call3((void (*)())Func_02003ddc, 12, 0x1180000, 0x2c80000);
  }
  v21 = Func_02000b24(12, 38);
  Func_02003628();
  Value4((s32 (*)())Func_02003688, 12, 2, 45, 1);
  r5 = 7;
L_0200a774:
  r5 = r5 - 1;
  Call2((void (*)())Func_020036b8, 33, 55);
  if (r5 >= 0)
    goto L_0200a774;
  Call1((void (*)())GameFlag_SetBit, 0x249);
L_0200a78c:
  r5 = 12;
L_0200a78e:
  v22 = Actor_GetById(r5);
  r6 = v22;
  r5 = r5 + 1;
  FIELD_AT(r6, u8, 85) = 0;
  FIELD_AT(r6, s32, 20) = 0;
  FIELD_AT(r6, s32, 12) = 0;
  if (r5 <= 14)
    goto L_0200a78e;
  goto L_0200a7b6;
L_0200a7aa:
  Func_020032d4(33, 0, -4);
L_0200a7b6:
  v23 = GameFlag_Test(0xa6a);
  r0 = v23;
  if (v23 != 0)
    goto L_0200a7c6;
  goto L_0200abc4;
L_0200a7c6:
  Func_02000ad8(r0, r1, r2);
  goto L_0200abc4;
L_0200a7cc:
  r3 = 164;
  if (r5 != (164))
    goto L_0200a82c;
  Func_020032d4(33, 0, -2);
  Call3((void (*)())Func_020032d4, 40, 0, 0);
  Call1((void (*)())Func_02003ea4, 0);
  r3 = r7 - 1;
  if ((u32)(r7 - 1) <= (u32)3)
    goto L_0200a7fa;
  if (r7 == 7)
    goto L_0200a7fa;
  goto L_0200abc4;
L_0200a7fa:
  v24 = GameFlag_Test(0xa67);
  r0 = v24;
  if (v24 != 0)
    goto L_0200a80a;
  goto L_0200abc4;
L_0200a80a:
  Func_02003628(r0, r1, r2);
  Value4((s32 (*)())Func_02003688, 44, 0, 34, 2);
  r5 = 0;
L_0200a81c:
  r5 = r5 + 1;
  Call2((void (*)())Func_020036b8, 33, 55);
  if (r5 <= 4)
    goto L_0200a81c;
  goto L_0200abc4;
L_0200a82c:
  r3 = 165;
  if (r5 != (165))
    goto L_0200a900;
  Call1((void (*)())Func_02003ea4, 0);
  if (r7 != 1) {
    if (r7 != 13)
      goto L_0200a84e;
  }
  Func_020032d4(33, 0, -2);
  goto L_0200a866;
L_0200a84e:
  if (r7 == 9)
    goto L_0200a85a;
  if (r7 == 11)
    goto L_0200a85a;
  if (r7 != 12)
    goto L_0200a866;
L_0200a85a:
  Func_020032d4(33, 0, -2);
L_0200a866:
  v25 = GameFlag_Test(0x238);
  if (v25 != 0)
    goto L_0200a890;
  r5 = 13;
L_0200a874:
  v26 = Actor_GetById(r5);
  r6 = v26;
  r5 = r5 + 1;
  FIELD_AT(r6, u8, 85) = 0;
  FIELD_AT(r6, s32, 8) = 0;
  FIELD_AT(r6, s32, 16) = 0;
  r3 = 0;
  if (r5 <= 15)
    goto L_0200a874;
  goto L_0200abc4;
L_0200a890:
  Call6((void (*)())Func_02003d24, 114, 39, 1, 2, 113, 39);
  r5 = 41;
  Call6((void (*)())Func_02003d1c, 105, 41, 12, 13, r5, r5);
  Call6((void (*)())Func_02003d24, 105, 105, 12, 13, 105, r5);
  r5 = 13;
L_0200a8ca:
  v27 = Actor_GetById(r5);
  r6 = v27;
  v28 = GameFlag_Test(r5 + 580);
  if (v28 != 0) {
    Call6((void (*)())Func_02003d1c, 47, 45, 1, 1, (FIELD_AT(r6, s32, 8)) >> 20,
          (FIELD_AT(r6, s32, 16)) >> 20);
  }
  r5 = r5 + 1;
  if (r5 <= 15)
    goto L_0200a8ca;
  goto L_0200abc4;
L_0200a900:
  r3 = 166;
  if (r5 != (166))
    goto L_0200a914;
  Func_020032d4(33, 0, -2);
  goto L_0200abc4;
L_0200a914:
  r3 = 167;
  if (r5 != (167))
    goto L_0200a93c;
  Call1((void (*)())Func_02003ea4, 0);
  v29 = Func_020032d4(33, 0, -2);
  Func_02003edc();
  Call3((void (*)())Func_02003ee4, 0, 8, 9);
  goto L_0200abc4;
L_0200a93c:
  if (r5 == (168))
    goto L_0200a944;
  goto L_0200ab46;
L_0200a944:
  if ((u32)(r7 - 3) <= (u32)1)
    goto L_0200a94c;
  goto L_0200aafe;
L_0200a94c:
  v30 = Actor_GetById(FIELD_AT((s32)(s32)&gMainRoutineWork, s32, 532));
  FIELD_AT(v30, u8, 35) = 32 | (FIELD_AT(v30, u8, 35));
  Value3((s32 (*)())Func_02003420, 10, 6, 4);
  Value3((s32 (*)())Func_02003420, 11, 3, 4);
  Value3((s32 (*)())Func_02003420, 12, 5, 4);
  Value3((s32 (*)())Func_02003420, 13, 6, 4);
  Value3((s32 (*)())Func_02003420, 14, 5, 4);
  Value3((s32 (*)())Func_02003420, 15, 4, 4);
  Value3((s32 (*)())Func_02003420, 16, 4, 4);
  Value3((s32 (*)())Func_02003420, 17, 3, 4);
  Value3((s32 (*)())Func_02003420, 18, 6, 4);
  Func_020032d4(40, 4, 4);
  v31 = GameFlag_Test(0x24b);
  if (v31 != 0)
    goto L_0200aa68;
  v32 = GameFlag_Test(0xa65);
  if (v32 != 0)
    goto L_0200aa68;
  r6 = (s32)Ram_HeapSlots->mapWork;
  stack[0] = 100;
  r5 = 13;
  Call6((void (*)())Func_02003d24, 64, 64, 19, 17, 100, r5);
  Call6((void (*)())Func_02003d24, 35, 105, 23, 24, 35, 73);
  Call6((void (*)())Func_02003d1c, 100, 13, 19, 19, 36, r5);
  FIELD_AT(r6, u8, 262) = 1;
  FIELD_AT((s32)((u8 *)stack + 10), u16, 0) =
      65532 & (FIELD_AT(67108874, u16, 0));
  FIELD_AT(67108874, u16, 0) = (65532 & (FIELD_AT(67108874, u16, 0))) | (1);
  goto L_0200aafe;
L_0200aa68:
  v33 = GameFlag_Test(0xa65);
  if (v33 == 0) {
    v34 = GameFlag_Test(0x24a);
    if (v34 == 0)
      goto L_0200aafe;
  }
  v35 = GameFlag_Test(0xa65);
  if (v35 != 0) {
    Call3((void (*)())Func_02003ddc, 12, 0x3280000, 0x1280000);
    Call3((void (*)())Func_02003ddc, 13, 0x2a80000, 0x1680000);
    Call3((void (*)())Func_02003ddc, 15, 0x2e80000, 0x1a80000);
  }
  Call2((void (*)())Func_02000b24, 12, 38);
  Call2((void (*)())Func_02000b24, 13, 39);
  v36 = Func_02000b24(15, 37);
  Func_02003628();
  Value4((s32 (*)())Func_02003688, 38, 2, 19, 1);
  r5 = 7;
L_0200aae6:
  r5 = r5 - 1;
  Call2((void (*)())Func_020036b8, 33, 55);
  if (r5 >= 0)
    goto L_0200aae6;
  Call1((void (*)())GameFlag_SetBit, 0x24b);
L_0200aafe:
  r5 = 10;
L_0200ab00:
  v37 = Actor_GetById(r5);
  r6 = v37;
  r5 = r5 + 1;
  FIELD_AT(r6, u8, 85) = 0;
  FIELD_AT(r6, s32, 20) = 0;
  FIELD_AT(r6, s32, 12) = 0;
  if (r5 <= 20)
    goto L_0200ab00;
  v38 = Actor_GetById(19);
  r6 = v38;
  Call6((void (*)())Func_02003d1c, 23, 39, 1, 1, (FIELD_AT(r6, s32, 8)) >> 20,
        39);
  r3 = r7 - 1;
  if ((u32)(r7 - 1) > (u32)1)
    goto L_0200abc4;
  Call1((void (*)())Func_02003ea4, 0);
  goto L_0200abc4;
L_0200ab46:
  r3 = 169;
  if (r5 != (169))
    goto L_0200abc4;
  Call1((void (*)())Func_02003ea4, 0);
  Func_020032d4(33, 0, -2);
  Call1((void (*)())GameFlag_SetBit, 0x144);
  v39 = GameFlag_Test(0x349);
  if (v39 == 0)
    goto L_0200ab8a;
  Call6((void (*)())Func_02003d1c, 38, 9, 7, 6, 13, 9);
  goto L_0200abc4;
L_0200ab8a:
  stack[0] = 50;
  r5 = 8;
  Call6((void (*)())Func_02003d24, 77, 8, 7, 6, 50, r5);
  Call6((void (*)())Func_02003d24, 70, 8, 7, 6, 77, r5);
  Call6((void (*)())Func_02003d1c, 38, 17, 7, 6, 13, 9);
L_0200abc4:
  /* add (s32)stack, #12 */
  return 0;
}
