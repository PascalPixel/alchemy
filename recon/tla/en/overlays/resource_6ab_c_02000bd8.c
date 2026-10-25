// Draft of overlay 6ab's Func_02000bd8, translated from its listing: differ
// 85 of 155. Left: its shape: 154 instructions against the listing's 155.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Func_020041ec();
s32 Func_02004204();
s32 Func_0200420c();
s32 Func_020042a4();
s32 Func_020042c4();
s32 Func_020042dc();
s32 Func_020042e4();
s32 Func_0200437c();
s32 Func_02004394();
s32 Func_020043a4();
s32 Func_020043c4();
s32 Func_020043d4();
s32 Func_02004404();
s32 Func_0200440c();
s32 Func_02004414();
s32 Func_0200444c();
s32 Func_0200446c();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Inventory_AddItem();
s32 Object_GetById();
s32 Object_SetModeById();
s32 Object_SetPositionAndResetMotion();
s32 PartyInventory_Remove();
s32 UiText_ShowPositionedMessageAndWait();
s32 Unit_Get();
s32 WaitFrames();
extern u8 Data_02004cec[];
extern u8 MsgMarsStar02[];
extern u8 MsgMarsStar06[];
extern u8 MsgMarsStar08[];
extern u8 gPartyState[];

void Func_02000bd8(void) {
  s32 v_r7, v_r5, v_r6, v_r0, v_r1, v_r2, v_r3, t6, t7, t8;
  s32 t9, t11, t13, t14, t15, t16, t18, t24, t25, t27;
  s32 t30, t33;

  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  Call2((void (*)())Object_SetModeById, 13, 5);
  Call1((void (*)())Func_0200437c, (s32)MsgMarsStar02);
  Call2((void (*)())Func_02004394, 13, 0);
  Call2((void (*)())Object_SetModeById, 13, 10);
  t6 = Value1((s32 (*)())GameFlag_Test, 0xa3a);
  v_r7 = t6;
  if (v_r7 == 0)
    goto L_02008c14;
  goto L_02008d80;
L_02008c14:
  v_r5 = ((s32)gPartyState + 0x214);
  t7 = *(s32 *)v_r5;
  t8 = Value1((s32 (*)())Object_GetById, t7);
  t9 = *(s32 *)v_r5;
  Call2((void (*)())Func_020043a4, t9, 0x4000);
  t11 = *(s32 *)v_r5;
  Call2((void (*)())Object_SetModeById, t11, 28);
  t13 = *(s32 *)(s32)Data_02004cec;
  FIELD_AT(t13, u8, 85) = v_r7;
  FIELD_AT(t13, s32, 48) = 0x40000;
  FIELD_AT(t13, s32, 52) = 0x20000;
  t14 = FIELD_AT(t8, s32, 12);
  t15 = FIELD_AT(t8, s32, 8);
  t16 = FIELD_AT(t8, s32, 16);
  Call4((void (*)())Func_02004204, t13, t15, (t14 + 0x200000), t16);
  t18 = *(s32 *)(s32)Data_02004cec;
  Call1((void (*)())Func_0200420c, t18);
  Call1((void (*)())Func_0200446c, 83);
  Call1((void (*)())PartyInventory_Remove, 224);
  Inventory_AddItem(4, 222);
  Call2((void (*)())UiText_ShowPositionedMessageAndWait, (s32)MsgMarsStar08, 1);
  v_r6 = 216;
  v_r5 = 0;
  goto L_02008c98;
L_02008c94:
  v_r6 = (v_r6 + 2);
  v_r5 = (v_r5 + 1);
L_02008c98:
  if (v_r5 > 14)
    goto L_02008cb4;
  t24 = Value1((s32 (*)())Unit_Get, 4);
  t25 = *(u16 *)(t24 + v_r6);
  if ((0x1ff & t25) != 222)
    goto L_02008c94;
  Call2((void (*)())Func_020042c4, 4, v_r5);
L_02008cb4:
  t27 = FIELD_AT((s32)gPartyState, s32, 532);
  Call2((void (*)())Object_SetModeById, t27, 1);
  Call1((void (*)())GameFlag_SetBit, 0xa3a);
  t30 = *(s32 *)(s32)Data_02004cec;
  v_r5 = (s32)Data_02004cec;
  v_r3 = 0;
  v_r2 = 0;
  v_r1 = 0;
  v_r0 = t30;
  goto L_02008ce8;
L_02008ce8:
  Call4((void (*)())Object_SetPositionAndResetMotion, v_r0, v_r1, v_r2, v_r3);
  Call1((void (*)())WaitFrames, 1);
  t33 = *(s32 *)v_r5;
  Call1((void (*)())Func_020041ec, t33);
  Call1((void (*)())WaitFrames, 1);
  Call2((void (*)())Func_020043c4, 0x4cccc, 0x9999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2a80000, -1, 0x920000, 1);
  Func_020043d4();
  Call2((void (*)())Func_0200440c, 0x10000, 0);
  Call2((void (*)())Func_02004404, 0x10003, 0);
  Call1((void (*)())Func_02004414, 20);
  Call1((void (*)())WaitFrames, 20);
  *(u16 *)0x500021e = 0x7fff;
  Call1((void (*)())WaitFrames, 1);
  Call3((void (*)())Func_020042a4, (s32)MsgMarsStar06, 0, 0);
  Call2((void (*)())Func_02004404, 0x10000, 0);
  Call1((void (*)())Func_02004414, 20);
  Call1((void (*)())WaitFrames, 20);
  goto L_02008d80;
L_02008d80:
  Func_020042e4();
  return;
}
