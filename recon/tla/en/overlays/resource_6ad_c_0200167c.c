// Draft of overlay 6ad's Func_0200167c, translated from its listing: differ
// 158 of 216. Left: its shape: 208 instructions against the listing's 216.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_FollowActor();
s32 Camera_MoveWithinMapBounds();
s32 Func_020054b8();
s32 Func_020054c0();
s32 Func_02005560();
s32 Func_02005568();
s32 Func_020055e8();
s32 Func_02005648();
s32 Func_020056c0();
s32 Map_GetTerrainHeight();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
s32 WaitFrames();
extern u8 gFrameCount[];
extern u8 gPartyState[];

void Func_0200167c(void) {
  s32 v_r9, v_r10, v_r7, v_r6, v_r5, v_r8, v_r0, v_r1, v_r2, v_r3;
  s32 t0, t1, t2, t3, t7, t8, t9, t10, t11, t12;
  s32 t13, t15, t16, t17, t18, t21, t22, t23, t24, t25;
  s32 t26, t28, t29, t31, t32, t34, t36, t38, t39, t40;
  s32 t41;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s32 *)((s32)gPartyState + 0x214);
  t2 = Object_GetById(t1);
  t3 = FIELD_AT(t2, s32, 104);
  Func_02005560();
  Func_02005648(0);
  *(u16 *)(0xd46 + t0) = 0;
  v_r9 = (85 + t2);
  *(u8 *)v_r9 = 4;
  ObjectDispatch_SetSingleChildFlags(t2, 0);
  t7 = *(u8 *)(99 + t3);
  v_r8 = (99 + t3);
  v_r5 = 0;
  v_r6 = t2;
  v_r7 = t3;
  v_r10 = t0;
  if (t7 == v_r5)
    goto L_0200971c;
L_020096d6:
  t8 = FIELD_AT(v_r7, s32, 8);
  FIELD_AT(v_r6, s32, 8) = t8;
  t9 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(v_r6, s32, 12) = (t9 + v_r5);
  t10 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(v_r6, s32, 16) = t10;
  if (v_r5 > 0x3ffff)
    goto L_020096f2;
  v_r5 = (v_r5 + 0x1999);
L_020096f2:
  t11 = *(s32 *)(s32)gFrameCount;
  t12 = *(u8 *)(s32)gFrameCount;
  FIELD_AT(v_r6, u16, 6) = (t12 << 12);
  t13 = *(u8 *)(v_r6 + 35);
  *(u8 *)(v_r6 + 35) = ((2 << (t11 & 1)) ^ t13);
  WaitFrames(1);
  t15 = *(u8 *)v_r8;
  if (t15 != 0)
    goto L_020096d6;
L_0200971c:
  *(u16 *)(0xd44 + v_r10) = 1;
  *(u16 *)(0xd46 + v_r10) = 1;
  t16 = FIELD_AT(v_r7, s32, 8);
  t17 = FIELD_AT(v_r7, u16, 6);
  FIELD_AT(v_r6, s32, 8) = (((((t16 >> 19) & -2) + (t17 >> 13)) - 1) << 19);
  t18 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(v_r6, s32, 16) =
      (((((t18 >> 19) & -2) - ((t17 >> 13) & 2)) + 1) << 19);
  FIELD_AT(v_r6, s32, 40) = 0x60000;
  *(u8 *)(v_r6 + 35) = 1;
  *(u8 *)v_r9 = 3;
  ObjectDispatch_SetSingleChildFlags(v_r6, 1);
  v_r3 = 0;
  v_r2 = -1;
  v_r1 = -1;
  v_r0 = -1;
  goto L_02009794;
L_02009794:
  Camera_MoveWithinMapBounds(v_r0, v_r1, v_r2, v_r3);
  t21 = Func_020055e8();
  FIELD_AT(t21, s32, 48) = 0x80000;
  FIELD_AT(t21, s32, 52) = 0x10000;
  *(u8 *)(t21 + 85) = 0;
  t22 = FIELD_AT(v_r6, s32, 8);
  t23 = FIELD_AT(v_r6, s32, 16);
  t24 = Map_GetTerrainHeight(0, t22, t23);
  t25 = FIELD_AT(v_r6, s32, 16);
  t26 = FIELD_AT(v_r6, s32, 8);
  Func_020054b8(t21, t26, t24, t25);
  t28 = FIELD_AT(v_r6, s32, 20);
  t29 = FIELD_AT(v_r6, s32, 12);
  v_r5 = 0;
  v_r7 = t21;
  if (t29 <= (t28 + 0x100000))
    goto L_020097fa;
  goto L_020097e0;
L_020097e0:
  WaitFrames(1);
  v_r5 = (v_r5 + 1);
  if (v_r5 > 59)
    goto L_020097fa;
  t31 = FIELD_AT(v_r6, s32, 20);
  t32 = FIELD_AT(v_r6, s32, 12);
  if (t32 > (t31 + 0x100000))
    goto L_020097e0;
L_020097fa:
  Func_020056c0(127);
  t34 = FIELD_AT(v_r6, s32, 40);
  v_r5 = 0;
  if (t34 == v_r5)
    goto L_0200981a;
L_02009808:
  WaitFrames(1);
  v_r5 = (v_r5 + 1);
  if (v_r5 > 59)
    goto L_0200981a;
  t36 = FIELD_AT(v_r6, s32, 40);
  if (t36 != 0)
    goto L_02009808;
L_0200981a:
  Func_020054c0(v_r7);
  v_r5 = ((s32)gPartyState + 0x214);
  t38 = *(s32 *)v_r5;
  t39 = Object_GetById(t38);
  t40 = *(u8 *)(t39 + 35);
  *(u8 *)(t39 + 35) = (1 | t40);
  t41 = *(s32 *)v_r5;
  Camera_FollowActor(t41, 1);
  *(u16 *)(0xd46 + v_r10) = 1;
  *(u16 *)(0x154 + v_r10) = 0;
  Func_02005568();
  return;
}
