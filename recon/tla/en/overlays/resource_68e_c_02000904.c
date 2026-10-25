// Draft, differ 932 of 918: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_02001760[];
extern u8 gData_020017b0[];
extern u8 IwramFillWords[];
extern u8 gData_02001824[];
extern u8 gData_02001828[];
extern u8 gData_020017c0[];
extern u8 gData_02001746[];
extern u8 gData_02001748[];
s32 Object_GetById();
void Func_020012e8();
s32 __divsi3();
void Func_020008f0();
s32 Math_Sine();
s32 Math_Cosine();
void Func_02001408();
s32 __modsi3();
void Func_02000884();

s32 Func_02000904(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r7_1, r11_1, r5_1, r6_1, r8_1, r10_1, r5_2, r5_3, r6_2, r8_2, r10_2, r6_3,
      r8_3, r6_4, r6_5, r8_4, r8_5, r5_4, r10_3, r10_4, r10_5, r10_6, r11_2,
      r6_6, r5_5, r5_6, r5_7, r5_8, r8_6, r10_7, r9_1, r5_9, r11_3, r8_7, r6_7,
      r10_8, r9_2, r5_10, r5_11;
  s32 r7, r11, r2, r0, r1, v0, v1, r5, r6, v2, r8, r10, v3, v4, r3, v5, v6, r4,
      v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,
      r9, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31;
  s32 stack[6];

  r7_1 = (s32)gData_02001760;
  /* sub (s32)stack, #24 */
  r11_1 = 3;
  r2 = r7_1 + 28;
L_0200891e:
  FIELD_AT(r2, s32, 12) = FIELD_AT(r2, s32, 0);
  FIELD_AT(r2, s32, 16) = FIELD_AT(r2, s32, 4);
  r11_1 = r11_1 - 1;
  FIELD_AT(r2, s32, 20) = FIELD_AT(r2, s32, 8);
  r2 = r2 - 12;
  if (r11_1 != 0)
    goto L_0200891e;
  if ((*(s16 *)(r7_1 + 2)) > 31)
    goto L_02008942;
  goto L_02008c94;
L_02008942:
  FIELD_AT(r7_1, s32, 4) = (FIELD_AT(r7_1, s32, 4)) + (FIELD_AT(r7_1, s32, 64));
  FIELD_AT(r7_1, s32, 8) = (FIELD_AT(r7_1, s32, 8)) + (FIELD_AT(r7_1, s32, 68));
  FIELD_AT(r7_1, s32, 12) =
      (FIELD_AT(r7_1, s32, 12)) + (FIELD_AT(r7_1, s32, 72));
  r2 = FIELD_AT(r7_1, s32, 72);
  r0 = FIELD_AT(r7_1, s32, 68);
  if (((FIELD_AT(r7_1, s32, 8)) + (FIELD_AT(r7_1, s32, 68))) <= 0)
    goto L_02008960;
  goto L_02008c8e;
L_02008960:
  FIELD_AT(r7_1, s32, 8) = r11_1;
  r1 = r11_1;
  if (r0 == 0)
    goto L_0200898c;
  FIELD_AT(r7_1, s32, 68) = r1;
  if ((FIELD_AT((s32)gData_020017b0, s32, 0)) != 1)
    goto L_02008980;
  v0 = Object_GetById(17);
  Call2((void (*)())Func_020012e8, v0, 1);
  goto L_0200898c;
L_02008980:
  v1 = Object_GetById(12);
  Call2((void (*)())Func_020012e8, v1, 1);
L_0200898c:
  if ((FIELD_AT(r7_1, s32, 76)) <= 0)
    goto L_02008a0c;
  r5_1 = 240;
  r5_1 = r5_1 << 15;
  r5_1 = r5_1 - (FIELD_AT(r7_1, s32, 4));
  r6_1 = 142;
  r6_1 = r6_1 << 15;
  r6_1 = r6_1 - (FIELD_AT(r7_1, s32, 12));
  r5_1 = r5_1 >> 8;
  r6_1 = r6_1 >> 8;
  v2 = Iwram_Sqrt((r5_1 * r5_1) + (r6_1 * r6_1));
  r8_1 = 0x1999;
  r10_1 = v2;
  v3 = __divsi3(r8_1 * r5_1, r10_1);
  r5_2 = FIELD_AT(r7_1, s32, 64);
  r5_2 = r5_2 + v3;
  FIELD_AT(r7_1, s32, 64) = r5_2;
  v4 = __divsi3(r8_1 * r6_1, r10_1);
  FIELD_AT(r7_1, s32, 72) = (FIELD_AT(r7_1, s32, 72)) + v4;
  r2 = (FIELD_AT(r7_1, s32, 72)) + v4;
  r3 = (((r5_2 << 6) - r5_2) << 2) + r5_2;
  if (((((r5_2 << 6) - r5_2) << 2) + r5_2) < 0) {
    r3 = r3 + 255;
  }
  FIELD_AT(r7_1, s32, 64) = r3 >> 8;
  r3 = (((r2 << 6) - r2) << 2) + r2;
  if (((((r2 << 6) - r2) << 2) + r2) < 0) {
    r3 = r3 + 255;
  }
  FIELD_AT(r7_1, s32, 72) = r3 >> 8;
  FIELD_AT(r7_1, s32, 76) = (FIELD_AT(r7_1, s32, 76)) - 1;
  goto L_02008b20;
L_02008a0c:
  r1 = 220;
  r3 = (FIELD_AT(r7_1, s32, 64)) * 220;
  if (((FIELD_AT(r7_1, s32, 64)) * 220) < 0) {
    r3 = r3 + 255;
  }
  FIELD_AT(r7_1, s32, 64) = r3 >> 8;
  r2 = r3 >> 8;
  r3 = (FIELD_AT(r7_1, s32, 72)) * r1;
  if (((FIELD_AT(r7_1, s32, 72)) * r1) < 0) {
    r3 = r3 + 255;
  }
  FIELD_AT(r7_1, s32, 72) = r3 >> 8;
  r2 = 0x7fe;
  if ((u32)(r2 + 1023) <= (u32)2046) {
    FIELD_AT(r7_1, s32, 64) = 0;
  }
  if ((u32)((FIELD_AT(r7_1, s32, 72)) + 1023) <= (u32)r2) {
    FIELD_AT(r7_1, s32, 72) = 0;
  }
  if ((FIELD_AT(r7_1, s32, 64)) != 0)
    goto L_02008b20;
  if ((FIELD_AT(r7_1, s32, 72)) != 0)
    goto L_02008b20;
  if ((FIELD_AT((s32)gData_020017b0, s32, 0)) == 1) {
    v5 = Object_GetById(17);
    Call2((void (*)())Func_020012e8, v5, 2);
    Call2((void (*)())Func_020008f0, 15, 0);
    Call2((void (*)())Func_020008f0, 14, 0);
    Call2((void (*)())Func_020008f0, 13, 0);
  } else {
    v6 = Object_GetById(12);
    Call2((void (*)())Func_020012e8, v6, 2);
    Call2((void (*)())Func_020008f0, 10, 0);
    Call2((void (*)())Func_020008f0, 9, 0);
    Call2((void (*)())Func_020008f0, 8, 0);
  }
  FIELD_AT((s32)gData_02001824, s32, 0) = 1;
  r0 = (s32)gData_02001828;
  r2 = (((7864320 - (FIELD_AT(r7_1, s32, 4))) >> 16) *
        ((7864320 - (FIELD_AT(r7_1, s32, 4))) >> 16)) +
       (((4653056 - (FIELD_AT(r7_1, s32, 12))) >> 16) *
        ((4653056 - (FIELD_AT(r7_1, s32, 12))) >> 16));
  r1 = 1;
  if (((((7864320 - (FIELD_AT(r7_1, s32, 4))) >> 16) *
        ((7864320 - (FIELD_AT(r7_1, s32, 4))) >> 16)) +
       (((4653056 - (FIELD_AT(r7_1, s32, 12))) >> 16) *
        ((4653056 - (FIELD_AT(r7_1, s32, 12))) >> 16))) > 224)
    goto L_02008ae2;
  r3 = 0;
  goto L_02008b1e;
L_02008ae2:
  if (r2 > 0x270)
    goto L_02008aee;
  FIELD_AT(r0, s32, 0) = r1;
  goto L_02008b20;
L_02008aee:
  if (r2 > 0x440)
    goto L_02008afa;
  r3 = 2;
  goto L_02008b1e;
L_02008afa:
  if (r2 > 0x690)
    goto L_02008b1c;
  r3 = 3;
  goto L_02008b1e;
L_02008b1c:
  r3 = 4;
L_02008b1e:
  FIELD_AT(r0, s32, 0) = r3;
L_02008b20:
  r5_3 = FIELD_AT(r7_1, s32, 12);
  r6_2 = 192;
  r4 = 240;
  r6_2 = r6_2 << 14;
  r8_2 = 0xc00000;
  r10_2 = 0x180000;
  r4 = r4 << 15;
  if (r5_3 >= 0x2a0000)
    goto L_02008b70;
  stack[1] = r4;
  v7 = __divsi3((2752512 - r5_3) * 42, 18);
  r6_2 = v7 + r6_2;
  r4 = stack[1];
  r0 = v7;
  r3 = 0x5a0000;
  if (r6_2 > 0x5a0000) {
    r6_3 = r3;
  }
  r8_2 = r8_2 - r0;
  r0 = 0x960000;
  if (r8_2 >= 0x960000)
    goto L_02008b70;
  r8_3 = r0;
L_02008b70:
  if (r5_3 <= 0x660000)
    goto L_02008bae;
  stack[1] = r4;
  v8 = __divsi3((r5_3 * 42) - 280756224, 18);
  r6_4 = v8 + 3145728;
  r4 = stack[1];
  r0 = v8;
  r3 = 0x5a0000;
  if (r6_4 > 0x5a0000) {
    r6_5 = r3;
  }
  r8_4 = 12582912 - r0;
  r0 = 0x960000;
  if (r8_4 >= 0x960000)
    goto L_02008bae;
  r8_5 = r0;
L_02008bae:
  r5_4 = FIELD_AT(r7_1, s32, 4);
  if (r5_4 >= 0x5a0000)
    goto L_02008bee;
  v9 = __divsi3((((5898240 - r5_4) << 3) + (5898240 - r5_4)) << 1, 42);
  r10_3 = 1572864 + v9;
  r0 = v9;
  r3 = 0x2a0000;
  if (r10_3 > 0x2a0000) {
    r10_4 = r3;
  }
  r4 = 7864320 - r0;
  if (r4 >= 0x660000)
    goto L_02008bee;
  r4 = 204;
  r4 = r4 << 15;
L_02008bee:
  if (r5_4 <= 0x960000)
    goto L_02008c2a;
  v10 = __divsi3((((r5_4 << 3) + r5_4) << 1) - 176947200, 42);
  r10_5 = 1572864 + v10;
  r0 = v10;
  r3 = 0x2a0000;
  if (r10_5 > 0x2a0000) {
    r10_6 = r3;
  }
  r4 = 7864320 - r0;
  if (r4 >= 0x660000)
    goto L_02008c2a;
  r4 = 204;
  r4 = r4 << 15;
L_02008c2a:
  if (r5_4 >= r6_5)
    goto L_02008c40;
  FIELD_AT(r7_1, s32, 4) = r6_5;
  r3 = FIELD_AT(r7_1, s32, 64);
  if ((FIELD_AT(r7_1, s32, 64)) >= 0)
    goto L_02008c40;
  FIELD_AT(r7_1, s32, 64) = (-r3 + ((u32)-r3 >> 31)) >> 1;
L_02008c40:
  if ((FIELD_AT(r7_1, s32, 4)) <= r8_5)
    goto L_02008c5a;
  FIELD_AT(r7_1, s32, 4) = r8_5;
  r3 = FIELD_AT(r7_1, s32, 64);
  if ((FIELD_AT(r7_1, s32, 64)) <= 0)
    goto L_02008c5a;
  FIELD_AT(r7_1, s32, 64) = (-r3 + ((u32)-r3 >> 31)) >> 1;
L_02008c5a:
  if ((FIELD_AT(r7_1, s32, 12)) >= r10_6)
    goto L_02008c74;
  FIELD_AT(r7_1, s32, 12) = r10_6;
  r3 = FIELD_AT(r7_1, s32, 72);
  if ((FIELD_AT(r7_1, s32, 72)) >= 0)
    goto L_02008c74;
  FIELD_AT(r7_1, s32, 72) = (-r3 + ((u32)-r3 >> 31)) >> 1;
L_02008c74:
  if ((FIELD_AT(r7_1, s32, 12)) <= r4)
    goto L_02008c94;
  FIELD_AT(r7_1, s32, 12) = r4;
  r3 = FIELD_AT(r7_1, s32, 72);
  if ((FIELD_AT(r7_1, s32, 72)) <= 0)
    goto L_02008c94;
  FIELD_AT(r7_1, s32, 72) = (-r3 + ((u32)-r3 >> 31)) >> 1;
  goto L_02008c94;
L_02008c8e:
  FIELD_AT(r7_1, s32, 68) = r0 - 16384;
L_02008c94:
  r11_2 = 0;
L_02008c98:
  r6_6 = (((r11_2 << 1) + r11_2) << 3) + ((s32)gData_020017c0);
  r2 = FIELD_AT(r6_6, u16, 18);
  if ((*(s16 *)(r6_6 + 18)) > 0) {
    FIELD_AT(r6_6, u16, 18) = r2 - 1;
  }
  r2 = FIELD_AT(r6_6, u16, 20);
  if ((*(s16 *)(r6_6 + 20)) > 0) {
    FIELD_AT(r6_6, u16, 20) = r2 - 1;
  }
  stack[2] = r11_2 + 1;
  r0 = FIELD_AT(r6_6, u16, 18);
  r1 = FIELD_AT(r6_6, u16, 16);
  if (r11_2 > 1)
    goto L_02008d6c;
  r5_5 = 128;
  r5_5 = r5_5 << 9;
  r3 = (r1 << 16) >> 16;
  if (((r1 << 16) >> 16) == 1) {
    r5_5 = r5_5 << 1;
  }
  if (r3 == 2) {
    r5_5 = (r5_5 << 1) + r5_5;
  }
  if ((r0 << 16) <= 0)
    goto L_02008cfa;
  if (r11_2 != 0)
    goto L_02008cf6;
  r0 = 18;
  goto L_02008d90;
L_02008cf6:
  r0 = 19;
  goto L_02008d90;
L_02008cfa:
  if (r11_2 == 0) {
    v11 = Object_GetById(18);
    Call2((void (*)())Func_020012e8, v11, 1);
  } else {
    v12 = Object_GetById(19);
    Call2((void (*)())Func_020012e8, v12, 1);
  }
  r2 = FIELD_AT(r6_6, u16, 14);
  if ((*(s16 *)(r6_6 + 14)) != 0)
    goto L_02008d68;
  r2 = FIELD_AT(r6_6, s32, 0);
  if ((*(s16 *)(r6_6 + 12)) == 0) {
    r3 = r2 + r5_5;
  } else {
    r3 = r2 - r5_5;
  }
  FIELD_AT(r6_6, s32, 0) = r3;
  if ((FIELD_AT(r6_6, s32, 0)) > 0x400000)
    goto L_02008d4e;
  FIELD_AT(r6_6, u16, 12) = 0;
  if (r11_2 != 1)
    goto L_02008d4e;
  FIELD_AT(r6_6, u16, 14) = 30;
L_02008d4e:
  if ((FIELD_AT(r6_6, s32, 0)) > 0xafffff)
    goto L_02008d58;
  goto L_02008e7c;
L_02008d58:
  FIELD_AT(r6_6, u16, 12) = 1;
  if (r11_2 == 1)
    goto L_02008d64;
  goto L_02008e7c;
L_02008d64:
  r3 = 30;
  goto L_02008e7a;
L_02008d68:
  r3 = r2 - 1;
  goto L_02008e7a;
L_02008d6c:
  if (r11_2 != 2)
    goto L_02008dde;
  r5_6 = 64;
  r5_6 = -r5_6;
  r3 = (r1 << 16) >> 16;
  if (((r1 << 16) >> 16) == 1) {
    r5_6 = r5_6 << 1;
  }
  if (r3 == 2) {
    r5_6 = (r5_6 << 1) + r5_6;
  }
  if ((r0 << 16) <= 0)
    goto L_02008d9c;
  r0 = 20;
L_02008d90:
  v13 = Object_GetById(r0);
  Call2((void (*)())Func_020012e8, v13, 3);
  goto L_02008e7c;
L_02008d9c:
  v14 = Object_GetById(20);
  Call2((void (*)())Func_020012e8, v14, 2);
  v15 = Math_Sine(*(s16 *)(r6_6 + 12));
  FIELD_AT(r6_6, s32, 0) = (((v15 << 1) + v15) << 4) + 7340032;
  v16 = Math_Cosine(*(s16 *)(r6_6 + 12));
  FIELD_AT(r6_6, s32, 8) = (((v16 << 2) + v16) << 3) + 4718592;
  FIELD_AT(r6_6, u16, 12) = (FIELD_AT(r6_6, u16, 12)) + r5_6;
  r3 = FIELD_AT(r6_6, u16, 14);
  goto L_02008e78;
L_02008dde:
  r5_7 = 64;
  r2 = (511) & (FIELD_AT(r6_6, u16, 14));
  r3 = (r1 << 16) >> 16;
  if (((r1 << 16) >> 16) == 1) {
    r5_8 = 128;
  }
  if (r3 == 2) {
    r5_8 = (r5_8 << 1) + r5_8;
  }
  if ((r0 << 16) <= 0)
    goto L_02008e24;
  v17 = Object_GetById(21);
  Call2((void (*)())Func_020012e8, v17, 3);
  goto L_02008e76;
L_02008e24:
  if (r2 > 0x17f)
    goto L_02008e6a;
  v18 = Math_Sine(*(s16 *)(r6_6 + 12));
  FIELD_AT(r6_6, s32, 0) = (52 * v18) + 7340032;
  v19 = Math_Cosine(*(s16 *)(r6_6 + 12));
  FIELD_AT(r6_6, s32, 8) = (((v19 << 1) + v19) << 3) + 4718592;
  FIELD_AT(r6_6, u16, 12) = (FIELD_AT(r6_6, u16, 12)) + r5_8;
  v20 = Object_GetById(21);
  Call2((void (*)())Func_020012e8, v20, 2);
  goto L_02008e76;
L_02008e6a:
  v21 = Object_GetById(21);
  Call2((void (*)())Func_020012e8, v21, 3);
L_02008e76:
  r3 = FIELD_AT(r6_6, u16, 14);
L_02008e78:
  r3 = r3 + 1;
L_02008e7a:
  FIELD_AT(r6_6, u16, 14) = r3;
L_02008e7c:
  if ((*(s16 *)(r6_6 + 20)) != 0)
    goto L_02008f34;
  if ((FIELD_AT(r7_1, s32, 8)) != 0)
    goto L_02008f34;
  r8_6 = ((FIELD_AT(r6_6, s32, 0)) - (FIELD_AT(r7_1, s32, 4))) >> 16;
  r10_7 = ((FIELD_AT(r6_6, s32, 8)) - (FIELD_AT(r7_1, s32, 12))) >> 16;
  r0 = (r8_6 * r8_6) + (r10_7 * r10_7);
  if (((r8_6 * r8_6) + (r10_7 * r10_7)) > 119)
    goto L_02008f34;
  r1 = FIELD_AT(r7_1, s32, 76);
  if ((FIELD_AT(r7_1, s32, 76)) <= 30)
    goto L_02008f34;
  r9_1 = 0x30000;
  r2 = 0x30000;
  if (r11_2 > 1)
    goto L_02008ee8;
  r2 = FIELD_AT(r7_1, s32, 64);
  r3 = *(s16 *)(r6_6 + 12);
  if ((*(s16 *)(r6_6 + 12)) != 0)
    goto L_02008eda;
  if (r2 >= r9_1)
    goto L_02008f16;
  FIELD_AT(r7_1, s32, 64) = r9_1;
  r2 = r9_1;
  r3 = r1 - 100;
  goto L_02008f14;
L_02008eda:
  r3 = -r9_1;
  if (r2 <= -r9_1)
    goto L_02008f16;
  FIELD_AT(r7_1, s32, 64) = r3;
  r3 = r1;
  goto L_02008f12;
L_02008ee8:
  v22 = Iwram_Sqrt(r0);
  r5_9 = v22;
  v23 = __divsi3(r9_1 * -r8_6, r5_9);
  FIELD_AT(r7_1, s32, 64) = v23;
  v24 = __divsi3(r9_1 * -r10_7, r5_9);
  FIELD_AT(r7_1, s32, 72) = v24;
  r3 = FIELD_AT(r7_1, s32, 76);
L_02008f12:
  r3 = r3 - 100;
L_02008f14:
  FIELD_AT(r7_1, s32, 76) = r3;
L_02008f16:
  Call1((void (*)())Func_02001408, 0x12d);
  v25 = __modsi3((*(s16 *)(r6_6 + 16)) + 1, 3);
  FIELD_AT(r6_6, u16, 18) = 36;
  FIELD_AT(r6_6, u16, 16) = v25;
  FIELD_AT(r6_6, u16, 20) = 30;
L_02008f34:
  r0 = r11_2;
  if (r11_2 == 1)
    goto L_02008f5a;
  if (r0 > 1)
    goto L_02008f44;
  if (r0 == 0)
    goto L_02008f50;
  goto L_02008fb8;
L_02008f44:
  r1 = r11_2;
  if (r11_2 == 2)
    goto L_02008f74;
  if (r1 == 3)
    goto L_02008f96;
  goto L_02008fb8;
L_02008f50:
  r0 = 18;
  r2 = *(s16 *)(r6_6 + 16);
  r3 = (s32)gData_02001746;
  goto L_02008f62;
L_02008f5a:
  r0 = 19;
  r2 = *(s16 *)(r6_6 + 16);
  r3 = (s32)gData_02001746;
L_02008f62:
  Func_02000884(r0, r6_6, 0, *(u8 *)(r3 + r2), (r2 << 4) + 16);
  goto L_02008fb8;
L_02008f74:
  Func_02000884(20, r6_6, 32768 - (*(s16 *)(r6_6 + 12)),
                *(u8 *)(((s32)gData_02001748 + 0x1) + (*(s16 *)(r6_6 + 16))),
                ((*(s16 *)(r6_6 + 16)) << 4) + 16);
  goto L_02008fb8;
L_02008f96:
  Func_02000884(21, r6_6, 65535 - (*(s16 *)(r6_6 + 12)),
                *(u8 *)(((s32)gData_02001748 + 0x1) + (*(s16 *)(r6_6 + 16))),
                ((*(s16 *)(r6_6 + 16)) << 4) + 16);
L_02008fb8:
  r11_3 = stack[2];
  if (stack[2] == 4)
    goto L_02008fc2;
  goto L_02008c98;
L_02008fc2:
  FIELD_AT(r7_1, s32, 52) = FIELD_AT(r7_1, s32, 4);
  FIELD_AT(r7_1, s32, 56) = 0;
  FIELD_AT(r7_1, s32, 60) = FIELD_AT(r7_1, s32, 12);
  r8_7 = 16 + r7_1;
  r6_7 = r7_1;
  r6_7 = r6_7 + 52;
  r10_8 = 28 + r7_1;
  r9_2 = 40 + r7_1;
  r1 = r7_1 + 4;
  if ((FIELD_AT((s32)gData_020017b0, s32, 0)) == 1) {
    r5_10 = 16;
    Func_02000884(17, r1, 0, 0, r5_10);
    Func_02000884(16, r6_7, 0, 0, r5_10);
    Func_02000884(15, r8_7, 0, 0, r5_10);
    Func_02000884(14, r10_8, 0, 0, r5_10);
    Func_02000884(13, r9_2, 0, 0, r5_10);
    v26 = Object_GetById(15);
    Call2((void (*)())Func_020012e8, v26, 4);
    v27 = Object_GetById(14);
    Call2((void (*)())Func_020012e8, v27, 4);
    v28 = Object_GetById(13);
    Call2((void (*)())Func_020012e8, v28, 4);
  } else {
    r5_11 = 16;
    Func_02000884(12, r1, 0, 0, r5_11);
    Func_02000884(11, r6_7, 0, 0, r5_11);
    Func_02000884(10, r8_7, 0, 0, r5_11);
    Func_02000884(9, r10_8, 0, 0, r5_11);
    Func_02000884(8, r9_2, 0, 0, r5_11);
    v29 = Object_GetById(10);
    Call2((void (*)())Func_020012e8, v29, 4);
    v30 = Object_GetById(9);
    Call2((void (*)())Func_020012e8, v30, 4);
    v31 = Object_GetById(8);
    Call2((void (*)())Func_020012e8, v31, 4);
  }
  r0 = 2;
  r3 = *(s16 *)(r7_1 + 2);
  r2 = FIELD_AT(r7_1, u16, 2);
  if ((*(s16 *)(r7_1 + 2)) != -1) {
    FIELD_AT(r7_1, u16, 2) = r2 + 1;
    r3 = r2 + 1;
  }
  /* add (s32)stack, #24 */
  return r0;
}
