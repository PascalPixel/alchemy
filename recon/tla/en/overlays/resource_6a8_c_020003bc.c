// Draft of overlay 6a8's Func_020003bc, translated from its listing: differ
// 30 of 85. Left: instruction order and register choice.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020002d8();
s32 Func_020043c4();
s32 Object_GetById();
s32 SceneActor_CalculateFixedPointDistance();
extern u8 gPartyState[];

s32 Func_020003bc(s32 p0) {
  s32 v_r5, v_r8, v_r7, v_r10, v_r6, v_r3, v_r0, v_r1, t0, t1;
  s32 t2, t3, t4, t6, t7, t8, t9, t10, t11, t12;
  s32 t13;

  t0 = FIELD_AT(0x3000000, s32, 60);
  t1 = FIELD_AT(0x3000000, s32, 108);
  t2 = FIELD_AT((s32)gPartyState, s32, 532);
  t3 = Object_GetById(t2);
  t4 = SceneActor_CalculateFixedPointDistance((t3 + 8), (p0 + 8));
  v_r10 = t0;
  v_r7 = 0;
  v_r8 = t1;
  v_r5 = p0;
  if (t4 > 11)
    goto L_02008400;
  FIELD_AT(v_r5, u8, 91) = v_r7;
  Func_020043c4(v_r5, 2);
  goto L_0200846a;
L_02008400:
  v_r6 = (v_r5 + 100);
  t6 = *(s16 *)v_r6;
  v_r3 = t6;
  if (v_r3 != 0)
    goto L_02008410;
  v_r0 = 18;
  goto L_0200841a;
L_02008410:
  if (v_r3 != 1)
    goto L_02008418;
  v_r0 = 16;
  goto L_0200841a;
L_02008418:
  v_r0 = 17;
L_0200841a:
  t7 = Object_GetById(v_r0);
  t8 = Func_020002d8(v_r5, t7, 24, 0);
  if (t8 != 0)
    goto L_0200846a;
  t9 = FIELD_AT((s32)gPartyState, s32, 532);
  t10 = Object_GetById(t9);
  t11 = FIELD_AT(v_r8, s16, 352);
  v_r1 = t10;
  if (t11 != 0)
    goto L_02008454;
  t12 = FIELD_AT(v_r10, u8, 4);
  if (t12 == 0)
    goto L_02008460;
L_02008454:
  t13 = *(u16 *)v_r6;
  if ((2 & t13) == 0)
    goto L_02008460;
  v_r7 = 1;
L_02008460:
  Func_020002d8(v_r5, v_r1, 32, v_r7);
L_0200846a:
  return 0;
}
