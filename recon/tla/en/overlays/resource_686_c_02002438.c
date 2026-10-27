// Draft, differ 139 of 989: translated from its listing, with the rooms
// compared as their linker ids (SceneId_Ov686Room1 to 8, loaded from the pool
// as the native does); names and structure still to do. Left: the native saves
// r8 and r10 only, and branch offsets follow.
#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Func_020010f4[];
extern u8 Data_02004030[];
extern u8 Func_020023d0[];
s32 Func_02002ed4();
s32 GameFlag_Test();
s32 Func_02002e6c();
s32 Func_02002dfc();
s32 Object_GetById();
s32 Func_02002df4();
s32 Func_02002eec();
void Object_SetModeById();
s32 Func_02000418();
s32 Func_02002e44();
s32 Func_02002edc();
void Camera_MoveWithinMapBounds();
void Event_StartScreenTransition();
void FieldEvent_WaitTicks();
void WaitTicks();
void GameFlag_SetBit();
s32 Func_02002ebc();
s32 Scheduler_AddOrUpdateCallback();
s32 Func_02001420();
s32 Func_02002e04();
s32 Func_02002dec();

s32 Func_02002438(s32 a0) {
  s32 r7, r1, r2, r3, r0, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12,
      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, r5,
      v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41,
      v42, v43, v44, v45, v46, v47, v48, v49, v50, r6, v51, v52, r10, v53, v54,
      v55, v56, r8, v57, v58, v59;
  s32 stack[2];

  r7 = (s32)&gPartyState;
  /* sub (s32)stack, #8 */
  r1 = 0;
  r2 = FIELD_AT(r7, s16, 480);
  r3 = (s32)&SceneId_Ov686Room1;
  r0 = a0;
  if ((FIELD_AT(r7, s16, 480)) == ((s32)&SceneId_Ov686Room1))
    goto L_0200a456;
  goto L_0200a686;
L_0200a456:
  Call1((void (*)())Func_02002ed4, 0);
  v0 = GameFlag_Test(0x9d2);
  if (v0 == 0)
    goto L_0200a474;
  Call3((void (*)())Func_02002e6c, 10, 0, 0);
L_0200a474:
  v1 = GameFlag_Test(0x9d3);
  if (v1 == 0)
    goto L_0200a48c;
  Call3((void (*)())Func_02002e6c, 11, 0, 0);
L_0200a48c:
  v2 = GameFlag_Test(0x9d4);
  if (v2 == 0)
    goto L_0200a4a4;
  Call3((void (*)())Func_02002e6c, 12, 0, 0);
L_0200a4a4:
  v3 = GameFlag_Test(0x9d5);
  if (v3 == 0)
    goto L_0200a4bc;
  Call3((void (*)())Func_02002e6c, 13, 0, 0);
L_0200a4bc:
  v4 = GameFlag_Test(0x9d6);
  if (v4 == 0)
    goto L_0200a4d4;
  Call3((void (*)())Func_02002e6c, 14, 0, 0);
L_0200a4d4:
  v5 = GameFlag_Test(0x9d7);
  if (v5 == 0)
    goto L_0200a4ec;
  Call3((void (*)())Func_02002e6c, 15, 0, 0);
L_0200a4ec:
  v6 = GameFlag_Test(0x9d8);
  if (v6 == 0)
    goto L_0200a504;
  Call3((void (*)())Func_02002e6c, 16, 0, 0);
L_0200a504:
  v7 = GameFlag_Test(0x990);
  if (v7 == 0)
    goto L_0200a51a;
  Call3((void (*)())Func_02002e6c, 17, 0, 0);
L_0200a51a:
  v8 = GameFlag_Test(0x991);
  if (v8 == 0)
    goto L_0200a532;
  Call3((void (*)())Func_02002e6c, 18, 0, 0);
L_0200a532:
  v9 = GameFlag_Test(0x992);
  if (v9 == 0)
    goto L_0200a54a;
  Call3((void (*)())Func_02002e6c, 19, 0, 0);
L_0200a54a:
  v10 = GameFlag_Test(0x993);
  if (v10 == 0)
    goto L_0200a562;
  Call3((void (*)())Func_02002e6c, 20, 0, 0);
L_0200a562:
  v11 = GameFlag_Test(0x994);
  if (v11 == 0)
    goto L_0200a57a;
  Call3((void (*)())Func_02002e6c, 21, 0, 0);
L_0200a57a:
  v12 = GameFlag_Test(0x995);
  if (v12 == 0)
    goto L_0200a592;
  Call3((void (*)())Func_02002e6c, 22, 0, 0);
L_0200a592:
  v13 = GameFlag_Test(0x996);
  if (v13 == 0)
    goto L_0200a5aa;
  Call3((void (*)())Func_02002e6c, 23, 0, 0);
L_0200a5aa:
  v14 = GameFlag_Test(0x997);
  if (v14 == 0)
    goto L_0200a5c2;
  Call3((void (*)())Func_02002e6c, 24, 0, 0);
L_0200a5c2:
  v15 = GameFlag_Test(0x998);
  if (v15 == 0)
    goto L_0200a5da;
  Call3((void (*)())Func_02002e6c, 25, 0, 0);
L_0200a5da:
  v16 = GameFlag_Test(0x999);
  if (v16 == 0)
    goto L_0200a5f2;
  Call3((void (*)())Func_02002e6c, 26, 0, 0);
L_0200a5f2:
  v17 = GameFlag_Test(0x99a);
  if (v17 == 0)
    goto L_0200a60a;
  Call3((void (*)())Func_02002e6c, 27, 0, 0);
L_0200a60a:
  v18 = GameFlag_Test(0x99b);
  if (v18 == 0)
    goto L_0200a622;
  Call3((void (*)())Func_02002e6c, 28, 0, 0);
L_0200a622:
  v19 = GameFlag_Test(0x99c);
  if (v19 == 0)
    goto L_0200a63a;
  Call3((void (*)())Func_02002e6c, 29, 0, 0);
L_0200a63a:
  v20 = GameFlag_Test(0x99d);
  if (v20 == 0)
    goto L_0200a652;
  Call3((void (*)())Func_02002e6c, 30, 0, 0);
L_0200a652:
  v21 = GameFlag_Test(0x9c6);
  if (v21 != 0)
    goto L_0200a662;
  goto L_0200ad2a;
L_0200a662:
  Call3((void (*)())Func_02002e6c, 8, 0x480000, 0x580000);
  Call6((void (*)())Func_02002dfc, 4, 68, 1, 1, 2, 69);
  goto L_0200ad2a;
L_0200a686:
  if (r2 != ((s32)&SceneId_Ov686Room2))
    goto L_0200a758;
  v22 = GameFlag_Test(0x99e);
  if (v22 == 0)
    goto L_0200a6a4;
  Call3((void (*)())Func_02002e6c, 9, 0, 0);
L_0200a6a4:
  v23 = GameFlag_Test(0x99f);
  if (v23 == 0)
    goto L_0200a6bc;
  Call3((void (*)())Func_02002e6c, 10, 0, 0);
L_0200a6bc:
  v24 = GameFlag_Test(0x9a0);
  if (v24 == 0)
    goto L_0200a6d2;
  Call3((void (*)())Func_02002e6c, 11, 0, 0);
L_0200a6d2:
  v25 = GameFlag_Test(0x9dc);
  if (v25 != 0)
    goto L_0200a6e2;
  goto L_0200ad2a;
L_0200a6e2:
  v26 = Object_GetById(8);
  r5 = v26;
  FIELD_AT(r5, u8, 85) = 3;
  Call3((void (*)())Func_02002e6c, 8, 0xc80000, 0x580000);
  stack[1] = 7;
  r5 = 10;
  Call6((void (*)())Func_02002df4, 35, 6, 7, 16, r5, 7);
  Call6((void (*)())Func_02002df4, 35, 70, 7, 16, r5, 71);
  r1 = 0;
  r2 = 0x1e2;
  r3 = FIELD_AT(r7, s16, 482);
  if ((FIELD_AT(r7, s16, 482)) == 6)
    goto L_0200a73e;
  if (r3 == 17)
    goto L_0200a73e;
  if (r3 == 15)
    goto L_0200a73e;
  goto L_0200ad2a;
L_0200a73e:
  Call1((void (*)())Func_02002eec, 0x23f);
  goto L_0200ad2a;
L_0200a758:
  if (r2 != ((s32)&SceneId_Ov686Room3))
    goto L_0200a7fa;
  v27 = GameFlag_Test(0x9c7);
  if (v27 == 0)
    goto L_0200a78e;
  Call3((void (*)())Func_02002e6c, 8, 0x2080000, 0x680000);
  Call6((void (*)())Func_02002dfc, 32, 69, 1, 1, 33, 69);
L_0200a78e:
  v28 = GameFlag_Test(0x9dd);
  if (v28 == 0)
    goto L_0200a7ae;
  Call3((void (*)())Func_02002e6c, 10, 0, 0);
  Call2((void (*)())Object_SetModeById, 9, 4);
L_0200a7ae:
  v29 = GameFlag_Test(0x201);
  if (v29 != 0)
    goto L_0200a7be;
  goto L_0200ad2a;
L_0200a7be:
  stack[1] = 24;
  r5 = 13;
  Call6((void (*)())Func_02002dfc, 13, 30, 3, 2, r5, 24);
  Call6((void (*)())Func_02002dfc, 14, 87, 1, 1, 14, 88);
  Call6((void (*)())Func_02002dfc, 41, 48, 3, 2, r5, 41);
  goto L_0200ad2a;
L_0200a7fa:
  if (r2 == ((s32)&SceneId_Ov686Room4))
    goto L_0200a802;
  goto L_0200aae8;
L_0200a802:
  v30 = GameFlag_Test(0x9a1);
  if (v30 == 0)
    goto L_0200a81a;
  Call3((void (*)())Func_02002e6c, 11, 0, 0);
L_0200a81a:
  v31 = GameFlag_Test(0x9a2);
  if (v31 == 0)
    goto L_0200a832;
  Call3((void (*)())Func_02002e6c, 12, 0, 0);
L_0200a832:
  v32 = GameFlag_Test(0x9a3);
  if (v32 == 0)
    goto L_0200a84a;
  Call3((void (*)())Func_02002e6c, 13, 0, 0);
L_0200a84a:
  v33 = GameFlag_Test(0x9a4);
  if (v33 == 0)
    goto L_0200a862;
  Call3((void (*)())Func_02002e6c, 14, 0, 0);
L_0200a862:
  v34 = GameFlag_Test(0x9a5);
  if (v34 == 0)
    goto L_0200a87a;
  Call3((void (*)())Func_02002e6c, 15, 0, 0);
L_0200a87a:
  v35 = GameFlag_Test(0x9a6);
  if (v35 == 0)
    goto L_0200a892;
  Call3((void (*)())Func_02002e6c, 16, 0, 0);
L_0200a892:
  v36 = GameFlag_Test(0x9a7);
  if (v36 == 0)
    goto L_0200a8aa;
  Call3((void (*)())Func_02002e6c, 17, 0, 0);
L_0200a8aa:
  v37 = GameFlag_Test(0x9c8);
  if (v37 == 0)
    goto L_0200a8c6;
  Call3((void (*)())Func_02002e6c, 8, 0x280000, 0x3080000);
L_0200a8c6:
  v38 = GameFlag_Test(0x9c9);
  if (v38 == 0)
    goto L_0200a8e2;
  Call3((void (*)())Func_02002e6c, 18, 0x2780000, 0x1c80000);
L_0200a8e2:
  v39 = GameFlag_Test(0x9ca);
  if (v39 == 0)
    goto L_0200a912;
  Call3((void (*)())Func_02002e6c, 19, 0x2680000, 0x880000);
  Call6((void (*)())Func_02002df4, 37, 70, 1, 1, 37, 71);
L_0200a912:
  v40 = GameFlag_Test(0x9d9);
  if (v40 == 0)
    goto L_0200a93e;
  v41 = Object_GetById(9);
  r5 = v41;
  FIELD_AT(r5, u8, 85) = 3;
  Call3((void (*)())Func_02002e6c, 9, 0xc80000, 0x2d80000);
L_0200a93e:
  v42 = GameFlag_Test(0x9da);
  if (v42 == 0)
    goto L_0200a96c;
  v43 = Object_GetById(10);
  r5 = v43;
  Call3((void (*)())Func_02002e6c, 10, 0xe80000, 0x2d80000);
  FIELD_AT(r5, u8, 85) = 3;
  goto L_0200a994;
L_0200a96c:
  v44 = GameFlag_Test(0x9db);
  if (v44 == 0)
    goto L_0200a994;
  v45 = GameFlag_Test(0x109);
  if (v45 != 0)
    goto L_0200a994;
  Call3((void (*)())Func_02002e6c, 10, 0x1480000, 0x2f80000);
L_0200a994:
  v46 = GameFlag_Test(0x9da);
  if (v46 == 0)
    goto L_0200a9f6;
  v47 = GameFlag_Test(0x9d9);
  if (v47 == 0)
    goto L_0200a9f6;
  r1 = 0x40000;
  r2 = 0x1e2;
  r3 = ((FIELD_AT((s32)(s32)&gPartyState, u16, 482)) - 13) << 16;
  if ((u32)(((FIELD_AT((s32)(s32)&gPartyState, u16, 482)) - 13) << 16) >
      (u32)262144)
    goto L_0200a9d0;
  Call1((void (*)())Func_02002eec, 0x23f);
L_0200a9d0:
  r5 = 12;
  Call6((void (*)())Func_02002df4, 16, 27, 3, 13, r5, 47);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 13, r5, 111);
L_0200a9f6:
  v48 = GameFlag_Test(0x109);
  r0 = v48;
  if (v48 != 0)
    goto L_0200aa16;
  r1 = 0;
  r2 = 0x1e2;
  r3 = FIELD_AT((s32)(s32)&gPartyState, s16, 482);
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 482)) != 17)
    goto L_0200aa16;
  v49 = Func_02000418();
  r0 = v49;
L_0200aa16:
  r5 = (s32)&gPartyState;
  r1 = 0;
  r2 = 0x1e2;
  r3 = FIELD_AT(r5, s16, 482);
  if ((FIELD_AT(r5, s16, 482)) != 20)
    goto L_0200aac2;
  Func_02002e44();
  Call1((void (*)())Func_02002edc, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call3((void (*)())Func_02002e6c, FIELD_AT(r5, s32, 532), 0, 0);
  Call3((void (*)())Func_02002e6c, 10, 0, 0);
  Event_StartScreenTransition();
  Call1((void (*)())FieldEvent_WaitTicks, 15);
  v50 = Object_GetById(10);
  r5 = v50;
  Call3((void (*)())Func_02002e6c, 10, 0x1480000, 0x2f80000);
  FIELD_AT(r5, s32, 12) = (FIELD_AT(r5, s32, 12)) + 8388608;
  r6 = 60;
  FIELD_AT(r5, u8, 85) = 3;
  r1 = 0x800000;
  r2 = r5 + 85;
  r3 = 3;
  goto L_0200aa94;
L_0200aa92:
  r6 = r6 - 1;
L_0200aa94:
  if (r6 == 0)
    goto L_0200aaa8;
  Call1((void (*)())WaitTicks, 1);
  r2 = -1;
  r3 = FIELD_AT(r5, s32, 40);
  if ((FIELD_AT(r5, s32, 40)) <= -1)
    goto L_0200aa92;
L_0200aaa8:
  Call1((void (*)())Func_02002eec, 188);
  Call1((void (*)())FieldEvent_WaitTicks, 60);
  Call1((void (*)())GameFlag_SetBit, 0x109);
  Call1((void (*)())Func_02002ebc, 20);
L_0200aac2:
  r3 = ((FIELD_AT((s32)(s32)&gPartyState, u16, 482)) - 13) << 16;
  if ((u32)(((FIELD_AT((s32)(s32)&gPartyState, u16, 482)) - 13) << 16) <=
      (u32)196608)
    goto L_0200aada;
  goto L_0200ad2a;
L_0200aada:
  goto L_0200ab54;
L_0200aae8:
  if (r2 != ((s32)&SceneId_Ov686Room5))
    goto L_0200ab60;
  v51 = GameFlag_Test(0x9db);
  if (v51 == 0)
    goto L_0200ab06;
  Call3((void (*)())Func_02002e6c, 9, 0, 0);
L_0200ab06:
  v52 = GameFlag_Test(0x9cb);
  if (v52 == 0)
    goto L_0200ab36;
  Call3((void (*)())Func_02002e6c, 8, 0x1180000, 0x1380000);
  Call6((void (*)())Func_02002dfc, 16, 84, 1, 1, 18, 84);
L_0200ab36:
  r1 = 0x1e2;
  r2 = 0;
  r3 = FIELD_AT(r7, s16, 482);
  if ((FIELD_AT(r7, s16, 482)) == 1)
    goto L_0200ab4a;
  if (r3 == 12)
    goto L_0200ab4a;
  goto L_0200ad2a;
L_0200ab4a:
  Call1((void (*)())Func_02002eec, 0x23f);
L_0200ab54:
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020010f4, 0x480);
  goto L_0200ad2a;
L_0200ab60:
  if (r2 == ((s32)&SceneId_Ov686Room6))
    goto L_0200ab68;
  goto L_0200ad04;
L_0200ab68:
  r10 = 482 + r7;
  r1 = 0;
  r3 = FIELD_AT(r7, s16, 482);
  if ((FIELD_AT(r7, s16, 482)) > 4)
    goto L_0200ab7c;
  Func_02001420(r0);
L_0200ab7c:
  v53 = GameFlag_Test(0x9e2);
  if (v53 == 0)
    goto L_0200abb0;
  r5 = 14;
  Call6((void (*)())Func_02002df4, 26, 67, 1, 3, r5, 65);
  Call6((void (*)())Func_02002dfc, 15, 5, 1, 1, r5, 5);
L_0200abb0:
  v54 = GameFlag_Test(0x9e3);
  if (v54 == 0)
    goto L_0200abe4;
  r5 = 2;
  Call6((void (*)())Func_02002df4, 26, 67, 1, 3, r5, 83);
  Call6((void (*)())Func_02002dfc, 1, 23, 1, 1, r5, 23);
L_0200abe4:
  v55 = GameFlag_Test(0x109);
  if (v55 != 0)
    goto L_0200abf2;
  goto L_0200ad2a;
L_0200abf2:
  v56 = GameFlag_Test(0x204);
  if (v56 != 0)
    goto L_0200ac00;
  goto L_0200ad2a;
L_0200ac00:
  FIELD_AT((s32)Data_02004030, s32, 4) = 0x200000;
  r8 = 0x200000;
  if ((u32)(((FIELD_AT(r10, u16, 0)) - 1) << 16) > (u32)65536)
    goto L_0200ac76;
  r5 = 4;
  r6 = 5;
  Call6((void (*)())Func_02002dfc, 25, 22, 9, 2, r5, r6);
  Call6((void (*)())Func_02002dfc, 25, 86, 9, 2, r5, 69);
  r5 = 1;
  Call6((void (*)())Func_02002dfc, 22, 24, 15, 7, r5, 7);
  Call6((void (*)())Func_02002dfc, 22, 88, 15, 7, r5, 71);
  Call6((void (*)())Func_02002e04, 69, 6, 1, 2, 68, r6);
L_0200ac76:
  if ((u32)(((FIELD_AT(r10, u16, 0)) - 3) << 16) > (u32)65536)
    goto L_0200ace4;
  r5 = 4;
  r6 = 23;
  Call6((void (*)())Func_02002dfc, 25, 22, 9, 2, r5, r6);
  Call6((void (*)())Func_02002dfc, 25, 86, 9, 2, r5, 87);
  stack[1] = 25;
  r5 = 1;
  Call6((void (*)())Func_02002dfc, 22, 24, 15, 7, r5, 25);
  Call6((void (*)())Func_02002dfc, 22, 88, 15, 7, r5, 89);
  Call6((void (*)())Func_02002e04, 69, 24, 1, 4, 68, r6);
L_0200ace4:
  v57 = Object_GetById(FIELD_AT(r7, s32, 532));
  r5 = v57;
  FIELD_AT(r5, s32, 20) = r8;
  FIELD_AT(r5, s32, 12) = r8;
  Call1((void (*)())FieldEvent_WaitTicks, 1);
  Func_02002dec();
  goto L_0200ad2a;
L_0200ad04:
  r3 = (s32)&SceneId_Ov686Room8;
  if (r2 != ((s32)&SceneId_Ov686Room8))
    goto L_0200ad2a;
  v59 = Object_GetById(64);
  r5 = v59;
  if (r5 == 0)
    goto L_0200ad2a;
  FIELD_AT(r5, u8, 85) = 0;
  FIELD_AT(r5, s32, 20) = 0x480000;
  FIELD_AT(r5, s32, 12) = 0x480000;
  FIELD_AT(r5, s32, 108) = (s32)Func_020023d0;
  r3 = (s32)Func_020023d0;
L_0200ad2a:
  /* add (s32)stack, #8 */
  return 0;
}
