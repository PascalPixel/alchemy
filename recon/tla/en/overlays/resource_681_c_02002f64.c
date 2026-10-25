// Draft, differ 246 of 522: translated from its listing; names and structure still
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

extern u8 gData_0200530c[];
extern u8 gData_020053d8[];
s32 Object_GetById();
void Func_02004e3c();
void Func_02002ac8();
s32 GameFlag_Test();
void Func_02004ebc();
void Func_02004df4();
s32 GameFlag_GetByte();
void Func_02002cb4();
s32 Func_02004dfc();
void Func_02004e04();
void Func_02004e5c();
void Func_02004fcc();
void Camera_MoveWithinMapBounds();
void Event_StartScreenTransition();
void FieldEvent_WaitFrames();
void ObjectDispatch_SetSingleChildFlags();
s32 Engine_ObjectCreate();
void Func_02004dac();
void Func_02004d9c();
void Animation_ApplyChildValues();
void WaitFrames();
void Func_02005014();
void GameFlag_SetBit();
void Func_02004f6c();
void Func_0200042c();
void Func_02004fec();
void Func_02004d00();
void Func_02004a5c();
void Func_02004fc4();

s32 Func_02002f64(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r2, r1, v0, r5, v1, v2, v3, v4, r6, r0, r3, v5, v6, v7, v8, v9, v10, v11,
      r8, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, r7;
  s32 stack[16];

  r2 = a2;
  r1 = a1;
  goto L_0200bd8a;
L_0200bd8a:
  if (r2 != (178))
    goto L_0200be74;
  v0 = Object_GetById(8);
  r5 = 32;
  FIELD_AT(v0, u8, 35) = (FIELD_AT(v0, u8, 35)) | r5;
  Call2((void (*)())Func_02004e3c, v0, 3);
  v1 = Object_GetById(9);
  r5 = r5 | (FIELD_AT(v1, u8, 35));
  FIELD_AT(v1, u8, 35) = r5;
  Call2((void (*)())Func_02004e3c, v1, 3);
  Call2((void (*)())Func_02002ac8, 8, 12);
  Call2((void (*)())Func_02002ac8, 9, 12);
  v2 = GameFlag_Test(0xa62);
  if (v2 != 0) {
    Call3((void (*)())Func_02004ebc, 8, 0x1b80000, 0x2b00000);
    Call3((void (*)())Func_02004ebc, 9, 0x2280000, 0x2b00000);
  } else {
    r5 = 40;
    Call6((void (*)())Func_02004df4, 88, 40, 1, 2, 89, r5);
    Call6((void (*)())Func_02004df4, 99, 40, 1, 2, 100, r5);
  }
  v3 = GameFlag_GetByte(0x210);
  r5 = v3;
  r5 = r5 & 15;
  v4 = GameFlag_GetByte(0x210);
  r6 = (u32)((v4 >> 4) << 24) >> 24;
  if (r5 != 0) {
    Call3((void (*)())Func_02004ebc, 10,
          (*(s16 *)(((s32)gData_0200530c) + (r5 << 2))) << 19,
          (*(s16 *)(((s32)gData_0200530c) + ((r5 << 2) + 2))) << 19);
  }
  r0 = (r6 << 24) >> 24;
  r3 = r6 << 24;
  if (((r6 << 24) >> 24) != 0)
    goto L_0200be5e;
  goto L_0200c24c;
L_0200be5e:
  Call3((void (*)())Func_02004ebc, 11,
        (*(s16 *)(((s32)gData_0200530c) + (r0 << 2))) << 19,
        (*(s16 *)(((s32)gData_0200530c) + ((r0 << 2) + 2))) << 19);
  goto L_0200c24c;
L_0200be74:
  if (r2 != (180))
    goto L_0200be92;
  v5 = GameFlag_Test(0xa63);
  if (v5 != 0)
    goto L_0200be8a;
  goto L_0200c24c;
L_0200be8a:
  Call1((void (*)())Func_02002cb4, 1);
  goto L_0200c24c;
L_0200be92:
  if (r2 == (179))
    goto L_0200be9a;
  goto L_0200c110;
L_0200be9a:
  v6 = Object_GetById(10);
  r6 = v6;
  Call2((void (*)())Func_02004e3c, v6, 3);
  Call2((void (*)())Func_02002ac8, 8, 10);
  Call2((void (*)())Func_02002ac8, 9, 11);
  v7 = GameFlag_Test(0x9bc);
  if (v7 == 0) {
    Call6((void (*)())Func_02004df4, 25, 77, 1, 15, 26, 77);
  }
  v8 = GameFlag_Test(0x9bd);
  if (v8 != 0) {
    Call3((void (*)())Func_02004ebc, 8, 0x1e00000, 0x1600000);
    r5 = 28;
    Call6((void (*)())Func_02004df4, 2, 93, 1, 5, r5, 85);
    r6 = 22;
    Call6((void (*)())Func_02004dfc, 27, 23, 1, 1, r5, r6);
    Call6((void (*)())Func_02004dfc, 32, 16, 2, 1, 29, r6);
  }
  v9 = GameFlag_Test(0x9be);
  if (v9 != 0) {
    Call3((void (*)())Func_02004ebc, 9, 0x2600000, 0x1600000);
    r5 = 36;
    Call6((void (*)())Func_02004df4, 2, 93, 1, 5, r5, 85);
    r6 = 22;
    Call6((void (*)())Func_02004dfc, 35, 23, 1, 1, r5, r6);
    Call6((void (*)())Func_02004dfc, 32, 16, 2, 1, 37, r6);
  }
  v10 = GameFlag_Test(0x9bf);
  r0 = v10;
  if (v10 != 0) {
    Call6((void (*)())Func_02004e04, 61, 0, 3, 3, 37, 26);
    v11 = Func_02004dfc(36, 23, 1, 1, 38, 25);
    r0 = v11;
  }
  r1 = 0x1e2;
  r2 = 0;
  if ((FIELD_AT(r7, s16, 482)) == 6)
    goto L_0200bfc8;
  goto L_0200c0d2;
L_0200bfc8:
  r8 = 60;
  Func_02004e5c(r0);
  Call1((void (*)())Func_02004fcc, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call3((void (*)())Func_02004ebc, 11, 0, 0);
  Call3((void (*)())Func_02004ebc, FIELD_AT(r7, s32, 532), 0, 0);
  Event_StartScreenTransition();
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call3((void (*)())Func_02004ebc, 11, 0x2a00000, 0x1800000);
  v12 = Object_GetById(11);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v12, 0);
  v13 = Object_GetById(11);
  r6 = v13;
  v14 = Engine_ObjectCreate(0x144, FIELD_AT(r6, s32, 8), FIELD_AT(r6, s32, 12),
                            FIELD_AT(r6, s32, 16));
  r5 = v14;
  r0 = v14;
  if (r5 != 0) {
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, r0, 0);
    Func_02004dac(r5, (s32)gData_020053d8);
    Call2((void (*)())Func_02004d9c, r5, 1);
    Call2((void (*)())Animation_ApplyChildValues, r5, 14);
    FIELD_AT(FIELD_AT(r5, s32, 80), u8, 9) =
        (FIELD_AT(FIELD_AT(r5, s32, 80), u8, 9)) | 12;
    FIELD_AT(r5, s32, 24) = 0x13333;
    FIELD_AT(r5, s32, 28) = 0x13333;
    r2 = 12;
  }
  FIELD_AT(r6, s32, 12) = (FIELD_AT(r6, s32, 12)) + 8388608;
  r1 = 0x800000;
  goto L_0200c0a2;
L_0200c09c:
  r8 = r8 - 1;
  r2 = -1;
L_0200c0a2:
  r3 = r8;
  if (r8 != 0) {
    Call1((void (*)())WaitFrames, 1);
    r3 = FIELD_AT(r6, s32, 40);
    if ((FIELD_AT(r6, s32, 40)) <= -1)
      goto L_0200c09c;
  }
  Call1((void (*)())Func_02005014, 188);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call1((void (*)())GameFlag_SetBit, 0x109);
  Call1((void (*)())Func_02004f6c, 3);
L_0200c0d2:
  v15 = GameFlag_Test(0x9c4);
  if (v15 == 0) {
    Call3((void (*)())Func_02004ebc, 11, 0, 0);
  }
  v16 = GameFlag_Test(0x109);
  r0 = v16;
  if (v16 == 0)
    goto L_0200c0f8;
  goto L_0200c24c;
L_0200c0f8:
  r1 = 0x1e2;
  r2 = 0;
  r3 = FIELD_AT((s32)(s32)&gPartyState, s16, 482);
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 482)) == 3)
    goto L_0200c10a;
  goto L_0200c24c;
L_0200c10a:
  Func_0200042c();
  goto L_0200c24c;
L_0200c110:
  r3 = 181;
  if (r2 == (181))
    goto L_0200c118;
  goto L_0200c24c;
L_0200c118:
  Call2((void (*)())Func_02004fec, 10, 1);
  Call2((void (*)())Func_02002ac8, 8, 9);
  r0 = 0x1e2;
  r1 = 0;
  r3 = FIELD_AT(r7, s16, 482);
  if ((FIELD_AT(r7, s16, 482)) == 1)
    goto L_0200c13e;
  if (r3 == 3)
    goto L_0200c13e;
  if (r3 != 5)
    goto L_0200c142;
L_0200c13e:
  Func_02004d00();
L_0200c142:
  v17 = GameFlag_Test(0x303);
  if (v17 != 0) {
    Func_02004a5c(11, 0x1800000, 0x4b0000, 0x2a80000);
  }
  v18 = GameFlag_Test(0x304);
  if (v18 != 0) {
    Func_02004a5c(12, 0x1cf0000, 0x330000, 0x2a80000);
  }
  v19 = GameFlag_Test(0x305);
  if (v19 != 0) {
    Func_02004a5c(13, 0x2350000, 0x370000, 0x2a80000);
  }
  v20 = GameFlag_Test(0x9bc);
  if (v20 != 0) {
    Call3((void (*)())Func_02004ebc, 8, 0x1800000, 0x1700000);
    r5 = 25;
    Call6((void (*)())Func_02004df4, 1, 85, 1, 4, r5, 85);
    r6 = 23;
    Call6((void (*)())Func_02004dfc, 27, 15, 2, 1, r6, r6);
    Call6((void (*)())Func_02004dfc, 25, 25, 1, 1, r5, r6);
  }
  v21 = Object_GetById(15);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v21, 0);
  v22 = GameFlag_Test(0x9c4);
  if (v22 == 0)
    goto L_0200c24c;
  r5 = 20;
  Call6((void (*)())Func_02004e04, 117, 20, 7, 8, 104, r5);
  Call6((void (*)())Func_02004e04, 111, 12, 1, 6, 111, r5);
  Call6((void (*)())Func_02004dfc, 49, 84, 8, 8, 40, 84);
  Call3((void (*)())Func_02004ebc, 15, 0, 0);
L_0200c24c:
  Call1((void (*)())Func_02004fc4, 0);
  /* add (s32)stack, #28 */
  return 0;
}
