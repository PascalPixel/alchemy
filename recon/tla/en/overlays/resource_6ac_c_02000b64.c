// Draft of overlay 6ac's Func_02000b64, translated from its listing: differ
// 99 of 324. Left: its shape: 327 instructions against the listing's 324.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 AudioCommand_WaitForCompletion();
s32 Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitTicks();
s32 Func_02001324();
s32 Func_0200154c();
s32 Func_02002e6c();
s32 Func_02003170();
s32 Func_02003198();
s32 Func_020031c0();
s32 Func_020031e0();
s32 Func_020031e8();
s32 Func_02003218();
s32 Func_02003228();
s32 Func_02003248();
s32 Func_02003250();
s32 Func_02003258();
s32 Func_02003278();
s32 Func_02003280();
s32 Func_02003290();
s32 Func_020032b8();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Object_GetById();
s32 WaitTicks();
extern u8 MsgYouHaveProven[];
extern u8 gData_0200338c[];
extern u8 gData_0200339c[];
extern u8 gData_020033c8[];
extern u8 gData_020033f4[];
extern u8 gPartyState[];

s32 Func_02000b64(void) {
  s32 v_r6, v_r7, v_r5, v_r1, v_r2, v_r0, t0, t1, t2, t6;
  s32 t7, t8, t9, t10, t11, t34;

  t0 = FIELD_AT((s32)gPartyState, s16, 480);
  t1 = FIELD_AT((s32)gPartyState, s16, 482);
  t2 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t2, s32, 428) = 0x100;
  Call1((void (*)())GameFlag_SetBit, 0x112);
  Call4((void (*)())Func_0200154c, (s32)gData_0200338c, (s32)gData_0200339c,
        (s32)gData_020033c8, (s32)gData_020033f4);
  v_r5 = t1;
  v_r7 = 0x3000000;
  v_r6 = t0;
  if (v_r6 != 0x128)
    goto L_02008bb0;
  goto L_02008e76;
L_02008bb0:
  if (v_r6 == 0x11f)
    goto L_02008bb8;
  goto L_02008e76;
L_02008bb8:
  Call1((void (*)())Func_02003278, 0);
  t6 = FIELD_AT(v_r7, s32, 108);
  FIELD_AT(t6, s32, 428) = 0x204;
  if ((u32)(v_r5 - 1) <= (u32)1)
    goto L_02008be2;
  if (v_r5 == 8)
    goto L_02008be2;
  if (v_r5 == 10)
    goto L_02008be2;
  if (v_r5 == 12)
    goto L_02008be2;
  if (v_r5 == 14)
    goto L_02008be2;
  goto L_02008e76;
L_02008be2:
  t7 = Value1((s32 (*)())GameFlag_Test, 0xa31);
  v_r6 = t7;
  if (v_r6 == 0)
    goto L_02008bf4;
  goto L_02008e76;
L_02008bf4:
  t8 = Value1((s32 (*)())GameFlag_Test, 0xa32);
  if (t8 != 0)
    goto L_02008c04;
  goto L_02008e76;
L_02008c04:
  t9 = Value1((s32 (*)())GameFlag_Test, 0xa33);
  if (t9 != 0)
    goto L_02008c14;
  goto L_02008e76;
L_02008c14:
  t10 = Value1((s32 (*)())GameFlag_Test, 0xa34);
  if (t10 != 0)
    goto L_02008c24;
  goto L_02008e76;
L_02008c24:
  t11 = Value1((s32 (*)())GameFlag_Test, 0xa35);
  if (t11 != 0)
    goto L_02008c34;
  goto L_02008e76;
L_02008c34:
  Func_020031e0();
  Call1((void (*)())Func_02003280, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Func_020032b8, 78);
  Call2((void (*)())Func_02003218, 0x20000, 0x4000);
  Camera_MoveWithinMapBounds(0xe00000, 0x400000, 0xd80000, 1);
  Func_02003228();
  Call1((void (*)())FieldEvent_WaitTicks, 20);
  Call2((void (*)())Func_02003250, 0x10000, 0);
  Call2((void (*)())Func_02003248, 0x10003, 0);
  Call1((void (*)())Func_02003258, 20);
  Call1((void (*)())WaitTicks, 20);
  *(u16 *)0x500021e = 0x7fff;
  Call1((void (*)())WaitTicks, 1);
  Call3((void (*)())Func_020031c0, (s32)MsgYouHaveProven, 0, 4);
  Call2((void (*)())Func_02003248, 0x10000, 0);
  Call1((void (*)())Func_02003258, 20);
  Call1((void (*)())WaitTicks, 20);
  Call2((void (*)())Func_02003218, 0x10000, 0x2000);
  v_r0 = 224;
  v_r2 = 216;
  v_r1 = 128;
  goto L_02008d00;
L_02008d00:
  Call4((void (*)())Camera_MoveWithinMapBounds, (v_r0 << 16), (v_r1 << 13),
        (v_r2 << 16), 1);
  Func_02003228();
  Call1((void (*)())FieldEvent_WaitTicks, 40);
  t34 = Value1((s32 (*)())Object_GetById, 16);
  FIELD_AT(t34, u8, 85) = v_r6;
  FIELD_AT(t34, s32, 12) = 0x100000;
  FIELD_AT(t34, s32, 20) = 0x100000;
  FIELD_AT(t34, u8, 98) = 18;
  FIELD_AT(t34, u16, 102) = 0x4000;
  FIELD_AT(t34, s32, 104) = v_r6;
  Func_02002e6c();
  Call1((void (*)())WaitTicks, 40);
  Call3((void (*)())Func_02003198, 0x20000, 0x20000, 0x10000);
  Call6((void (*)())Func_02001324, 216, 216, 65, 65, 13, 13);
  Call6((void (*)())Func_02001324, 232, 216, 66, 65, 14, 13);
  Call6((void (*)())Func_02001324, 232, 232, 66, 66, 14, 14);
  Call6((void (*)())Func_02001324, 216, 200, 65, 64, 13, 12);
  Call6((void (*)())Func_02001324, 248, 200, 67, 64, 15, 12);
  Call6((void (*)())Func_02001324, 248, 232, 67, 66, 15, 14);
  Call6((void (*)())Func_02001324, 216, 232, 65, 66, 13, 14);
  Call6((void (*)())Func_02001324, 200, 216, 64, 65, 12, 13);
  Call6((void (*)())Func_02001324, 232, 200, 66, 64, 14, 12);
  Call6((void (*)())Func_02001324, 248, 216, 67, 65, 15, 13);
  Call6((void (*)())Func_02001324, 200, 232, 64, 66, 12, 14);
  Call6((void (*)())Func_02001324, 200, 200, 64, 64, 12, 12);
  Call3((void (*)())Func_02003198, -1, -1, 0xe666);
  Call6((void (*)())Func_02003170, 30, 12, 4, 3, 12, 12);
  Call1((void (*)())FieldEvent_WaitTicks, 40);
  Call1((void (*)())GameFlag_SetBit, 0xa31);
  Call1((void (*)())Func_020032b8, 80);
  AudioCommand_WaitForCompletion();
  Func_02003290();
  Func_020031e8();
L_02008e76:
  return 0;
}
