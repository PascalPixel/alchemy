// Draft of overlay 6b0's Func_02001398, translated from its listing: differ
// 134 of 159. Left: its shape: 155 instructions against the listing's 159.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Engine_ObjectCreate();
s32 FieldEvent_WaitFrames();
s32 Func_020032e0();
s32 Func_020032e8();
s32 Func_02003328();
s32 Func_02003398();
s32 Func_020033a0();
s32 Func_02003420();
s32 Func_02003430();
s32 Func_02003438();
s32 Func_02003488();
s32 Func_020034b8();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
s32 Object_SetModeById();
s32 Object_SetPartAttribute();
s32 Object_SetSpritePriority();
extern u8 gData_02003574[];
extern u8 gPartyState[];

void Func_02001398(void) {
  s32 v_r0, v_r1, v_r6, v_r7, v_r5, t0, t1, t2, t3, t4;
  s32 t5, t9, t10, t11, t12, t18, t20, t21, t26, t33;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s32 *)((s32)gPartyState + 0x214);
  t2 = Object_GetById(t1);
  t3 = *(s16 *)((s32)gPartyState + 0x1e0);
  t4 = FIELD_AT(t2, s32, 8);
  t5 = FIELD_AT(t2, s32, 16);
  v_r7 = t0;
  v_r6 = t2;
  v_r1 = t4;
  v_r0 = t5;
  if (t3 != 0x132)
    goto L_020093dc;
  Func_02003328(17, 71, (v_r1 >> 20), ((v_r0 >> 20) + 64), 71, 71);
  goto L_020093f0;
L_020093dc:
  Func_02003328(54, 87, (v_r1 >> 20), ((v_r0 >> 20) + 64), 87, 87);
L_020093f0:
  Func_020034b8(106);
  t9 = FIELD_AT(v_r6, s32, 8);
  t10 = FIELD_AT(v_r6, s32, 16);
  t11 = Engine_ObjectCreate(0x15d, (((t9 >> 20) << 20) + 0x80000), 0,
                            (((t10 >> 20) << 20) + 0x80000));
  t12 = *(u8 *)(t11 + 35);
  *(u8 *)(t11 + 35) = (2 | t12);
  *(u8 *)(t11 + 85) = 0;
  Object_SetPartAttribute(t11, 4);
  FIELD_AT(t11, s32, 28) = 0x16000;
  FIELD_AT(t11, s32, 24) = 0x16000;
  Object_SetSpritePriority(t11, 2);
  Func_020032e0(t11, 3);
  ObjectDispatch_SetSingleChildFlags(t11, 0);
  Func_020032e8(t11, (s32)gData_02003574);
  t18 = *(s16 *)(v_r7 + 0x154);
  GameFlag_SetBit((t18 + 0x1ce));
  t20 = GameFlag_Test(0x200);
  v_r5 = t20;
  v_r6 = 1;
L_02009476:
  t21 = GameFlag_Test((v_r6 + 0x200));
  v_r6 = (v_r6 + 1);
  v_r5 = (v_r5 & t21);
  if (v_r6 != 7)
    goto L_02009476;
  if (v_r5 == 0)
    goto L_02009506;
  Func_02003398();
  Func_02003488(0);
  FieldEvent_WaitFrames(10);
  Func_02003420(0xcccc, 0x1999, 0);
  t26 = *(s16 *)((s32)gPartyState + 0x1e0);
  v_r5 = 9;
  if (t26 != 0x132)
    goto L_020094c2;
  v_r5 = 8;
L_020094c2:
  Func_02003438(v_r5, 1);
  Func_02003430();
  FieldEvent_WaitFrames(10);
  Func_020034b8(0x246);
  Object_SetModeById(v_r5, 2);
  GameFlag_SetBit(0x207);
  t33 = Object_GetById(v_r5);
  *(u8 *)(t33 + 85) = 4;
  FieldEvent_WaitFrames(80);
  Func_020033a0();
L_02009506:
  return;
}
