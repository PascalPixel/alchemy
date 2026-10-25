// Draft of overlay 6ae's Func_02000314, translated from its listing: differ
// 25 of 121. Left: its shape: 120 instructions against the listing's 121.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_WaitFrames();
s32 Func_020001d8();
s32 Func_02001f10();
s32 Func_020029ac();
s32 Func_02003658();
s32 Func_02003ff4();
s32 Func_0200558c();
s32 Func_0200560c();
s32 Func_02005614();
s32 Func_0200567c();
s32 Func_0200573c();
s32 Func_02005784();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Object_SetModeById();
s32 WaitFrames();
extern u8 gPartyState[];

s32 Func_02000314(s32 p0) {
  s32 v_r3, v_r0, v_r5, t0, t2, t3, t12, t16, t28;

  t0 = FIELD_AT(0x3000000, s32, 108);
  *(s32 *)(t0 + 0x1ac) = 0x204;
  Call1((void (*)())Func_020001d8, p0);
  t2 = Value1((s32 (*)())GameFlag_SetBit, 0x112);
  t3 = *(s16 *)((s32)gPartyState + 0x1e2);
  v_r5 = (s32)gPartyState;
  v_r0 = t2;
  v_r3 = t3;
  if (v_r3 != 5)
    goto L_02008364;
  *(u16 *)0x4000052 = 0xc08;
  *(u16 *)0x4000050 = 0x3f10;
  Func_02003ff4();
  goto L_02008446;
L_02008364:
  if (v_r3 != 4)
    goto L_0200840e;
  Func_0200560c();
  Call4((void (*)())Func_02005784, 0, 0, 0, 0);
  Call2((void (*)())Object_SetModeById, 20, 8);
  Call2((void (*)())Object_SetModeById, 21, 8);
  Call2((void (*)())Object_SetModeById, 22, 7);
  Call3((void (*)())Func_0200567c, 19, 0x1f20000, 0x1400000);
  Call3((void (*)())Func_0200567c, 1, 0x21a0000, 0x1400000);
  t12 = *(s32 *)(v_r5 + 0x214);
  Call3((void (*)())Func_0200567c, t12, 0x1fa0000, 0x1180000);
  Call1((void (*)())WaitFrames, 1);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  t16 = Value0((s32 (*)())Func_0200573c);
  *(u8 *)(t16 + 85) = 0;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2000000, 0x600000, 0x1380000,
        1);
  Func_0200558c();
  Call1((void (*)())WaitFrames, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Func_02003658();
  Func_02005614();
  goto L_02008446;
L_0200840e:
  if (v_r3 != 3)
    goto L_0200843e;
  Call2((void (*)())Object_SetModeById, 20, 8);
  Call2((void (*)())Object_SetModeById, 21, 8);
  Call2((void (*)())Object_SetModeById, 22, 7);
  t28 = Value1((s32 (*)())GameFlag_Test, 0xa3d);
  if (t28 != 0)
    goto L_02008446;
  Func_020029ac();
  goto L_02008446;
L_0200843e:
  if (v_r3 != 2)
    goto L_02008446;
  Call1((void (*)())Func_02001f10, v_r0);
L_02008446:
  return 0;
}
