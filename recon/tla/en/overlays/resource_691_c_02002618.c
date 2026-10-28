// Draft, differ 531 of 732: translated from its listing; names and structure still
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

extern u8 gData_02003d9e[];
extern u8 gData_02003d98[];
extern u8 gData_02003dba[];
extern u8 gData_02003db2[];
extern u8 gData_02003dac[];
extern u8 gData_02003dd2[];
extern u8 gData_02003dc4[];
extern u8 gData_02003de4[];
extern u8 gData_02003dd6[];
extern u8 gData_02003dee[];
extern u8 gData_02003de8[];
extern u8 gData_02003e02[];
s32 Object_GetById();
s32 Func_02000668();
s32 GameFlag_Test();
void Func_02000ee4();
void Func_02003b0c();
void Func_02003c0c();
void Func_02003aa4();
s32 Func_020006cc();
void ObjectDispatch_SetSingleChildFlags();
void Func_02002518();
void Func_02002d08();
void Func_02003bec();
void Func_02003bf4();
s32 Func_0200244c();
s32 Func_020022ac();
void Func_02003be4();
void Func_02003bfc();
void Func_0200079c();
void Func_020024c8();
void Func_02003a94();

s32 Func_02002618(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r8, r7, r2, v0, r5, v1, v2, v3, v4, v5, r6, v6, v7, r3, r0, r10, v8, r9,
      v9, v10, v11, v12, r1, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22,
      v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,
      v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52,
      v53, v54, v55, v56, v57;
  s32 stack[16];

  FIELD_AT(((s32)Ram_HeapSlots->eventWork), s32, 428) = 0x204;
  r8 = (s32)&gAdventure;
  FIELD_AT(r8, u16, 608) = 223;
  FIELD_AT(r8, u16, 610) = 1;
  /* sub (s32)stack, #8 */
  r7 = 0;
  r2 = FIELD_AT(r8, s16, 480);
  if ((FIELD_AT(r8, s16, 480)) == (223))
    goto L_0200a65e;
  goto L_0200a780;
L_0200a65e:
  goto L_0200a66c;
L_0200a66c:
  v0 = Object_GetById(FIELD_AT(r8, s32, 532));
  r5 = 32;
  FIELD_AT(v0, u8, 35) = (FIELD_AT(v0, u8, 35)) | r5;
  v1 = Object_GetById(8);
  FIELD_AT(v1, u8, 35) = (FIELD_AT(v1, u8, 35)) | r5;
  v2 = Object_GetById(10);
  r5 = r5 | (FIELD_AT(v2, u8, 35));
  FIELD_AT(v2, u8, 35) = r5;
  Func_02000668((s32)gData_02003d9e);
  v3 = GameFlag_Test(0x109);
  if (v3 == 0) {
    Call3((void (*)())Func_02000ee4, 21, 13, 0);
  }
  v4 = GameFlag_Test(0x8d5);
  if (v4 != 0) {
    Call3((void (*)())Func_02003b0c, 8, 0x1080000, 0x1680000);
  }
  v5 = GameFlag_Test(0x8d6);
  if (v5 != 0) {
    Call3((void (*)())Func_02003b0c, 10, 0xf80000, 0x1980000);
  }
  r6 = (s32)gData_02003d98;
  r5 = 78;
  Func_02003c0c(r6);
  Call6((void (*)())Func_02003aa4, 98, 21, 4, 3, r5, 21);
  Call6((void (*)())Func_02003aa4, 98, 27, 4, 3, r5, 24);
  Func_020006cc(r6);
  v6 = GameFlag_Test(0x109);
  if (v6 == 0) {
    v7 = Object_GetById(20);
    FIELD_AT(v7, s32, 8) = (FIELD_AT(v7, s32, 8)) + 65536;
    FIELD_AT(v7, u8, 85) = r7;
    FIELD_AT((v7 + 85), u8, 4) = r7;
    FIELD_AT(v7, s32, 12) = (FIELD_AT(v7, s32, 12)) + 1572864;
    FIELD_AT(v7, s32, 20) = (FIELD_AT(v7, s32, 12)) + 1572864;
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v7, 0);
  }
  Call1((void (*)())Func_02002518, 20);
  r2 = FIELD_AT(r8, s16, 482);
  r3 = 482 + r8;
  r0 = 0;
  if ((FIELD_AT(r8, s16, 482)) <= 4)
    goto L_0200a774;
  goto L_0200ac20;
L_0200a774:
  if (r2 >= 3)
    goto L_0200a77a;
  goto L_0200ac20;
L_0200a77a:
  r0 = 0x100000;
  goto L_0200ac1c;
L_0200a780:
  if (r2 == (224))
    goto L_0200a788;
  goto L_0200ac26;
L_0200a788:
  r10 = 532 + r8;
  v8 = Object_GetById(FIELD_AT(r8, s32, 532));
  FIELD_AT(v8, u8, 35) = 32 | (FIELD_AT(v8, u8, 35));
  r9 = 482 + r8;
  r2 = 482 + r8;
  r3 = FIELD_AT(r8, s16, 482);
  if ((FIELD_AT(r8, s16, 482)) > 3)
    goto L_0200a848;
  Func_02002d08((s32)gData_02003dba);
  v9 = Func_02000668((s32)gData_02003db2);
  Func_02003bec();
  Call3((void (*)())Func_02003bf4, 0, 11, 12);
  v10 = GameFlag_Test(0x8d7);
  if (v10 != 0) {
    Call3((void (*)())Func_02003b0c, 15, 0x1b80000, 0x2180000);
  }
  Func_02003c0c((s32)gData_02003dac);
  r3 = FIELD_AT(r9, s16, 0);
  if ((FIELD_AT(r9, s16, 0)) != 1)
    goto L_0200a81c;
  v11 = GameFlag_Test(0x109);
  if (v11 == 0) {
    v12 = Object_GetById(FIELD_AT(r10, s32, 0));
    FIELD_AT(v12, s32, 16) = (FIELD_AT(v12, s32, 16)) + 1048576;
    r1 = 0x100000;
    r3 = (FIELD_AT(v12, s32, 16)) + 1048576;
  }
  Call1((void (*)())Func_0200244c, -0x100000);
  goto L_0200acda;
L_0200a81c:
  if (r3 == 2)
    goto L_0200a822;
  goto L_0200acda;
L_0200a822:
  v13 = GameFlag_Test(0x109);
  if (v13 == 0) {
    v14 = Object_GetById(FIELD_AT(r10, s32, 0));
    FIELD_AT(v14, s32, 16) = (FIELD_AT(v14, s32, 16)) + 1048576;
    r1 = 0x100000;
    r3 = (FIELD_AT(v14, s32, 16)) + 1048576;
  }
  Call1((void (*)())Func_0200244c, -0x600000);
  goto L_0200acda;
L_0200a848:
  if (r3 <= 8)
    goto L_0200a84e;
  goto L_0200aa28;
L_0200a84e:
  v15 = GameFlag_Test(0x109);
  if (v15 == 0) {
    v16 = Object_GetById(8);
    FIELD_AT(v16, u8, 85) = r7;
    v17 = Object_GetById(8);
    r5 = 160;
    r5 = r5 << 15;
    FIELD_AT(v17, s32, 12) = (FIELD_AT(v17, s32, 12)) + r5;
    v18 = Object_GetById(9);
    FIELD_AT(v18, u8, 85) = r7;
    v19 = Object_GetById(9);
    FIELD_AT(v19, s32, 12) = (FIELD_AT(v19, s32, 12)) + r5;
    Call3((void (*)())Func_02000ee4, 19, 8, 0);
    v20 = Object_GetById(10);
    FIELD_AT(v20, u8, 85) = r7;
    v21 = Object_GetById(10);
    r5 = 160;
    r5 = r5 << 16;
    FIELD_AT(v21, s32, 12) = (FIELD_AT(v21, s32, 12)) + r5;
    v22 = Object_GetById(11);
    FIELD_AT(v22, u8, 85) = r7;
    v23 = Object_GetById(11);
    FIELD_AT(v23, s32, 12) = (FIELD_AT(v23, s32, 12)) + r5;
    Call3((void (*)())Func_02000ee4, 20, 10, 1);
    v24 = Object_GetById(12);
    FIELD_AT(v24, u8, 85) = r7;
    v25 = Object_GetById(12);
    r5 = 240;
    r5 = r5 << 16;
    FIELD_AT(v25, s32, 12) = (FIELD_AT(v25, s32, 12)) + r5;
    v26 = Object_GetById(13);
    FIELD_AT(v26, u8, 85) = r7;
    v27 = Object_GetById(13);
    FIELD_AT(v27, s32, 12) = (FIELD_AT(v27, s32, 12)) + r5;
    Call3((void (*)())Func_02000ee4, 21, 12, 0);
  }
  v28 = GameFlag_Test(0x8d8);
  if (v28 != 0) {
    Call3((void (*)())Func_02003b0c, 16, 0x3780000, 0x2380000);
  }
  Func_02003c0c((s32)gData_02003dd2);
  Func_02000668((s32)gData_02003dc4);
  v29 = GameFlag_Test(0x109);
  if (v29 == 0) {
    v30 = Object_GetById(15);
    r6 = 128;
    r6 = r6 << 9;
    FIELD_AT(v30, s32, 8) = (FIELD_AT(v30, s32, 8)) + r6;
    FIELD_AT(v30, u8, 85) = r7;
    FIELD_AT((v30 + 85), u8, 4) = r7;
    r5 = 192;
    r5 = r5 << 13;
    FIELD_AT(v30, s32, 12) = (FIELD_AT(v30, s32, 12)) + r5;
    FIELD_AT(v30, s32, 20) = (FIELD_AT(v30, s32, 12)) + r5;
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v30, 0);
    v31 = Object_GetById(18);
    FIELD_AT(v31, s32, 8) = (FIELD_AT(v31, s32, 8)) + r6;
    FIELD_AT(v31, u8, 85) = r7;
    FIELD_AT((v31 + 85), u8, 4) = r7;
    FIELD_AT(v31, s32, 12) = (FIELD_AT(v31, s32, 12)) + r5;
    FIELD_AT(v31, s32, 20) = (FIELD_AT(v31, s32, 12)) + r5;
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v31, 0);
  }
  Call1((void (*)())Func_02002518, 15);
  Call1((void (*)())Func_02002518, 18);
  r2 = 0;
  r3 = FIELD_AT(r9, s16, 0);
  r0 = r9;
  if ((FIELD_AT(r9, s16, 0)) != 5)
    goto L_0200a9ac;
  v32 = Func_0200244c(0x1200000);
  r0 = v32;
  goto L_0200ac20;
L_0200a9ac:
  if (r3 != 6)
    goto L_0200a9c4;
  v33 = GameFlag_Test(0x109);
  r0 = v33;
  if (v33 == 0)
    goto L_0200a9be;
  goto L_0200ac20;
L_0200a9be:
  v34 = Func_020022ac(r0, r1);
  r0 = v34;
  goto L_0200ac20;
L_0200a9c4:
  if (r3 != 7)
    goto L_0200a9ee;
  v35 = GameFlag_Test(0x109);
  if (v35 == 0) {
    v36 = Object_GetById(FIELD_AT(r10, s32, 0));
    FIELD_AT(v36, s32, 16) = (FIELD_AT(v36, s32, 16)) + 1048576;
    r2 = 0x100000;
    r3 = (FIELD_AT(v36, s32, 16)) + 1048576;
  }
  v37 = Func_0200244c(-0x300000);
  r0 = v37;
  goto L_0200ac20;
L_0200a9ee:
  if (r3 == 8)
    goto L_0200a9f4;
  goto L_0200ac20;
L_0200a9f4:
  r0 = 0x680000;
  goto L_0200ac1c;
L_0200aa28:
  if (r3 > 12)
    goto L_0200aaee;
  v38 = GameFlag_Test(0x8d9);
  if (v38 != 0) {
    Call3((void (*)())Func_02003b0c, 8, 0x1080000, 0x2480000);
  }
  r5 = (s32)gData_02003de4;
  Func_02003c0c(r5);
  v39 = Func_020006cc(r5);
  Func_02003be4();
  Call4((void (*)())Func_02003bfc, 0, 0x250, 14, 13);
  Func_02000668((s32)gData_02003dd6);
  v40 = GameFlag_Test(0x109);
  if (v40 == 0) {
    v41 = Object_GetById(10);
    r6 = 128;
    r6 = r6 << 9;
    FIELD_AT(v41, s32, 8) = (FIELD_AT(v41, s32, 8)) + r6;
    FIELD_AT(v41, u8, 85) = r7;
    FIELD_AT((v41 + 85), u8, 4) = r7;
    r5 = 192;
    r5 = r5 << 13;
    FIELD_AT(v41, s32, 12) = (FIELD_AT(v41, s32, 12)) + r5;
    FIELD_AT(v41, s32, 20) = (FIELD_AT(v41, s32, 12)) + r5;
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v41, 0);
    v42 = Object_GetById(12);
    FIELD_AT(v42, s32, 8) = (FIELD_AT(v42, s32, 8)) + r6;
    FIELD_AT(v42, u8, 85) = r7;
    FIELD_AT((v42 + 85), u8, 4) = r7;
    FIELD_AT(v42, s32, 12) = (FIELD_AT(v42, s32, 12)) + r5;
    FIELD_AT(v42, s32, 20) = (FIELD_AT(v42, s32, 12)) + r5;
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v42, 0);
  }
  Call1((void (*)())Func_02002518, 10);
  Call1((void (*)())Func_02002518, 12);
  r1 = 0x10000;
  r3 = ((FIELD_AT(r9, u16, 0)) - 9) << 16;
  r0 = r9;
  if ((u32)(((FIELD_AT(r9, u16, 0)) - 9) << 16) <= (u32)65536)
    goto L_0200aae8;
  goto L_0200ac20;
L_0200aae8:
  r0 = 0x300000;
  goto L_0200ac1c;
L_0200aaee:
  Func_02000668((s32)gData_02003dee);
  v43 = GameFlag_Test(0x109);
  if (v43 == 0) {
    v44 = Object_GetById(13);
    FIELD_AT(v44, s32, 8) = (FIELD_AT(v44, s32, 8)) + 65536;
    FIELD_AT(v44, u8, 85) = r7;
    FIELD_AT((v44 + 85), u8, 4) = r7;
    FIELD_AT(v44, s32, 12) = (FIELD_AT(v44, s32, 12)) + 1572864;
    FIELD_AT(v44, s32, 20) = (FIELD_AT(v44, s32, 12)) + 1572864;
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v44, 0);
  }
  Call1((void (*)())Func_02002518, 13);
  v45 = GameFlag_Test(0x8da);
  if (v45 != 0) {
    Call3((void (*)())Func_02003b0c, 8, 0x2880000, 0x2580000);
  }
  v46 = GameFlag_Test(0x8db);
  if (v46 != 0) {
    Call3((void (*)())Func_02003b0c, 9, 0x2380000, 0x2680000);
  }
  Func_0200079c((s32)gData_02003de8);
  v47 = GameFlag_Test(0x250);
  if (v47 != 0) {
    Call6((void (*)())Func_02003aa4, 121, 48, 1, 3, 123, 48);
  }
  v48 = GameFlag_Test(0x251);
  if (v48 != 0) {
    Call6((void (*)())Func_02003aa4, 121, 48, 1, 3, 125, 48);
  }
  v49 = GameFlag_Test(0x252);
  if (v49 != 0) {
    Call6((void (*)())Func_02003aa4, 108, 56, 1, 3, 109, 56);
  }
  r0 = 0;
  r2 = FIELD_AT(r9, u16, 0);
  r3 = FIELD_AT(r9, s16, 0);
  if ((FIELD_AT(r9, s16, 0)) != 14)
    goto L_0200abea;
  v50 = Func_0200244c(0x300000);
  r0 = v50;
  goto L_0200ac20;
L_0200abea:
  r1 = 0x10000;
  r3 = (r2 - 16) << 16;
  if ((u32)((r2 - 16) << 16) > (u32)65536)
    goto L_0200ac20;
  v51 = GameFlag_Test(0x109);
  if (v51 == 0) {
    v52 = Object_GetById(FIELD_AT(r8, s32, 532));
    FIELD_AT(v52, s32, 16) = (FIELD_AT(v52, s32, 16)) + 1048576;
    r2 = 0x100000;
    r3 = (FIELD_AT(v52, s32, 16)) + 1048576;
  }
  r0 = -0x200000;
L_0200ac1c:
  v53 = Func_0200244c(r0);
  r0 = v53;
L_0200ac20:
  Func_020024c8(r0);
  goto L_0200acda;
L_0200ac26:
  r3 = 225;
  if (r2 != (225))
    goto L_0200acda;
  v54 = GameFlag_Test(0x8dc);
  if (v54 != 0) {
    Call3((void (*)())Func_02003b0c, 8, 0, 0);
    r5 = 42;
    Call6((void (*)())Func_02003aa4, 0, 32, 5, 8, r5, 17);
    Call6((void (*)())Func_02003aa4, 0, 96, 5, 8, r5, 81);
    r5 = 5;
    r6 = 8;
    Call6((void (*)())Func_02003a94, 0, 32, 42, 17, r5, r6);
    Call6((void (*)())Func_02003a94, 0, 96, 42, 81, r5, r6);
  }
  r5 = 133;
  r5 = r5 << 2;
  r5 = r5 + r8;
  v55 = Object_GetById(FIELD_AT(r5, s32, 0));
  FIELD_AT(v55, u8, 35) = 32 | (FIELD_AT(v55, u8, 35));
  Func_02000668((s32)gData_02003e02);
  r3 = FIELD_AT(r8, s16, 482);
  if ((FIELD_AT(r8, s16, 482)) > 2)
    goto L_0200acda;
  v56 = GameFlag_Test(0x109);
  if (v56 == 0) {
    v57 = Object_GetById(FIELD_AT(r5, s32, 0));
    FIELD_AT(v57, s32, 16) = (FIELD_AT(v57, s32, 16)) + 1048576;
    r1 = 0x100000;
    r3 = (FIELD_AT(v57, s32, 16)) + 1048576;
  }
  Call1((void (*)())Func_0200244c, -0x200000);
L_0200acda:
  /* add (s32)stack, #8 */
  return 0;
}
