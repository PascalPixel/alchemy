// Draft of overlay Func_020002f4, translated from its listing: differ 18 of 255
// in tla-en (was 33). The scene compared first is SceneId_Ov6abRoom1, as the
// native's pool shows, and the second flag test reuses an earlier temporary,
// which gives the native's registers there. Left: the order and registers of
// the leader's position from the two saved cells.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02000714();
s32 Func_02000ae4();
s32 Func_02000d88();
s32 Func_02000db8();
s32 Func_02001d7c();
s32 Func_02001df0();
s32 Func_02003e04();
s32 Func_020041fc();
s32 Func_0200423c();
s32 Func_02004244();
s32 Func_0200432c();
s32 Func_0200443c();
s32 GameFlag_GetByte();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 ObjectMotion_SetActionVariant();
s32 Object_GetById();
s32 WaitFrames();
extern u8 Data_02004cec[];
extern u8 gPartyState[];
extern char SceneId_Ov6abRoom1;

s32 Func_020002f4(void) {
  s32 v_r7, v_r8, v_r10, v_r6, v_r5, t0, t1, t2, t7, t12;
  s32 t13, t14, t15, t16, t17, t18, t20, t22, t24, t27;
  s32 t28, t31, t34, t35, t40, t42, t45, t46;

  t0 = FIELD_AT((s32)gPartyState, s16, 480);
  t1 = FIELD_AT((s32)gPartyState, s16, 482);
  t2 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t2, s32, 428) = 0x204;
  *(s32 *)(s32)Data_02004cec = 0;
  Call1((void (*)())GameFlag_SetBit, 0x112);
  Func_02001d7c();
  Call4((void (*)())Func_02001df0, 0, 0, 0, 88);
  v_r10 = 0x3000000;
  v_r8 = (s32)gPartyState;
  v_r7 = t1;
  if (t0 == (s32)&SceneId_Ov6abRoom1)
    goto L_02008350;
  goto L_0200853a;
L_02008350:
  Call1((void (*)())Func_0200443c, 0);
  t7 = Value1((s32 (*)())GameFlag_Test, 0xa30);
  if (t7 == 0)
    goto L_020083ac;
  Call6((void (*)())Func_0200423c, 65, 65, 9, 21, 38, 24);
  Call6((void (*)())Func_02004244, 65, 65, 9, 21, 38, 24);
  Call6((void (*)())Func_02004244, 76, 64, 9, 22, 38, 87);
  Call6((void (*)())Func_02004244, 76, 64, 9, 22, 102, 23);
L_020083ac:
  if (v_r7 != 11)
    goto L_0200840a;
  t12 = FIELD_AT(v_r8, s32, 532);
  t13 = Value1((s32 (*)())Object_GetById, t12);
  t14 = FIELD_AT(v_r10, s32, 108);
  t15 = Value1((s32 (*)())GameFlag_GetByte, 0x340);
  t16 = Value1((s32 (*)())GameFlag_GetByte, 0x348);
  v_r6 = (t14 + 0x1cc);
  t17 = *(s32 *)v_r6;
  FIELD_AT(t13, s32, 16) = ((t16 << 20) - 0x1280000);
  v_r5 = ((t15 << 20) + 0x80000);
  FIELD_AT(t13, s32, 8) = v_r5;
  FIELD_AT(t17, s32, 8) = v_r5;
  t18 = FIELD_AT(t13, s32, 16);
  FIELD_AT(t17, s32, 16) = t18;
  Func_020041fc();
  t20 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t20 != 0)
    goto L_0200840a;
  Func_02003e04();
L_0200840a:
  if ((u32)(v_r7 - 9) > (u32)1)
    goto L_02008422;
  t16 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t16 != 0)
    goto L_02008422;
  Func_02003e04();
L_02008422:
  if (v_r7 == 1)
    goto L_0200842c;
  if (v_r7 == 20)
    goto L_0200842c;
  goto L_0200853a;
L_0200842c:
  t24 = Value1((s32 (*)())GameFlag_Test, 0xa30);
  if (t24 != 0)
    goto L_02008452;
  Call2((void (*)())ObjectMotion_SetActionVariant, 11, 3);
  Call2((void (*)())ObjectMotion_SetActionVariant, 12, 3);
  t27 = Value1((s32 (*)())Object_GetById, 12);
  FIELD_AT(t27, s32, 24) = 0xffff0000;
L_02008452:
  t28 = Value1((s32 (*)())GameFlag_Test, 0xa4b);
  if (t28 == 0)
    goto L_02008470;
  Func_02000d88();
  Call3((void (*)())Func_0200432c, 10, 0, 0);
  goto L_02008494;
L_02008470:
  t31 = Value1((s32 (*)())GameFlag_Test, 0xa4a);
  if (t31 == 0)
    goto L_0200848a;
  Call1((void (*)())GameFlag_SetBit, 0x233);
  goto L_02008494;
L_0200848a:
  Call3((void (*)())Func_0200432c, 10, 0, 0);
L_02008494:
  t34 = Value1((s32 (*)())GameFlag_Test, 0xa3b);
  if (t34 == 0)
    goto L_02008532;
  t35 = Value1((s32 (*)())GameFlag_Test, 0xa30);
  if (t35 != 0)
    goto L_020084c2;
  Call3((void (*)())Func_0200432c, 11, 0, 0);
  Call3((void (*)())Func_0200432c, 12, 0, 0);
L_020084c2:
  Func_02000d88();
  Call1((void (*)())WaitFrames, 10);
  t40 = Value1((s32 (*)())GameFlag_Test, 0xa3a);
  if (t40 != 0)
    goto L_020084f8;
  Call1((void (*)())Func_02000ae4, 0);
  t42 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t42 != 0)
    goto L_020084f8;
  Call1((void (*)())Func_02000db8, 13);
  Call1((void (*)())Func_02000db8, 14);
L_020084f8:
  t45 = Value1((s32 (*)())Object_GetById, 13);
  t46 = FIELD_AT(t45, s32, 8);
  if (t46 <= 0x1900000)
    goto L_0200853a;
  Call6((void (*)())Func_0200423c, 39, 22, 2, 1, 43, 13);
  Call6((void (*)())Func_0200423c, 31, 21, 2, 2, 40, 12);
  goto L_0200853a;
L_02008532:
  if (v_r7 != 20)
    goto L_0200853a;
  Func_02000714();
L_0200853a:
  return 0;
}
