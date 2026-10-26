// 26 October 2026, under -mthumb-slow-loads: differ 4 of 157: the native issues
// a load one slot later after a store, where alchemy-gcc issues it at once; its
// four empty asms are gone, since they no longer change anything; alchemy
// permute (5 to 6 minutes)
// and an empty asm or one-pass block after any one statement do not move it.
// Draft of overlay 653's Func_020019d4, translated from its listing: differ 4
// of 157. Left: instruction order around
//   adds r5, r0, #0
//   ldr r3, [r5, #8]
//   - movs r6, #0
//   asrs r3, r3, #20
//   mov r8, r3
// (- the listing, + this draft). Tried: Call, Value and direct forms of each
// call, empty asms and one-pass blocks between statements.
#include "types.h"
#include "party_state.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_02000778();
void Func_02005388();
void GameFlag_ClearBit();
void GameFlag_SetBit();
s32 GameFlag_Test();
s32 Object_GetById();
extern u8 gData_02006298[];
extern u8 gData_0200629c[];

void Func_020019d4(void) {
  s32 t1, t8, t10, t12, t13, t14, t17, t18, p_r6, v_r5_1, v_r7_1, v_r8_1;

  t1 = Value1((s32 (*)())Object_GetById, gPartyState.currentOwner);
  v_r5_1 = t1;
  v_r8_1 = (FIELD_AT(v_r5_1, s32, 8) >> 20);
  v_r7_1 = (FIELD_AT(v_r5_1, s32, 16) >> 20);
  if (v_r8_1 != 13)
    goto L_02009a0c;
  if (v_r7_1 != 13)
    goto L_02009a0c;
  Call1((void (*)())GameFlag_SetBit, 0x208);
  p_r6 = 1;
  goto L_02009a14;
L_02009a0c:
  Call1((void (*)())GameFlag_ClearBit, 0x208);
  p_r6 = 0;
L_02009a14:
  if (v_r8_1 != 23)
    goto L_02009a2c;
  if (v_r7_1 != 15)
    goto L_02009a2c;
  Call1((void (*)())GameFlag_SetBit, 0x209);
  p_r6 = 1;
  goto L_02009a36;
L_02009a2c:
  Call1((void (*)())GameFlag_ClearBit, 0x209);
L_02009a36:
  if (p_r6 == 0)
    goto L_02009a44;
  Call1((void (*)())GameFlag_SetBit, 0x17d);
  goto L_02009a4c;
L_02009a44:
  Call1((void (*)())GameFlag_ClearBit, 0x17d);
L_02009a4c:
  if (FIELD_AT(gData_02006298, s32, 0) == 0)
    goto L_02009a8c;
  if (FIELD_AT(v_r5_1, s32, 12) != 0)
    goto L_02009a8c;
  if (FIELD_AT(v_r5_1, s32, 8) <= 0x11c0000)
    goto L_02009a8c;
  if (FIELD_AT(v_r5_1, s32, 8) >= 0x1340000)
    goto L_02009a8c;
  if (FIELD_AT(v_r5_1, s32, 16) >= 0xc80000)
    goto L_02009a8c;
  t8 = Value1((s32 (*)())GameFlag_Test, 0x20b);
  if (t8 != 0)
    goto L_02009a8c;
  Func_02000778(v_r5_1, 0x26666);
L_02009a8c:
  if (FIELD_AT(gData_0200629c, s32, 0) == 0)
    goto L_02009aca;
  if (FIELD_AT(v_r5_1, s32, 12) != 0)
    goto L_02009aca;
  if (FIELD_AT(v_r5_1, s32, 8) <= 0x14a0000)
    goto L_02009aca;
  if (FIELD_AT(v_r5_1, s32, 8) > 0x165ffff)
    goto L_02009aca;
  if (FIELD_AT(v_r5_1, s32, 16) >= 0xd80000)
    goto L_02009aca;
  t10 = Value1((s32 (*)())GameFlag_Test, 0x20b);
  if (t10 != 0)
    goto L_02009aca;
  Func_02000778(v_r5_1, 0x39999);
L_02009aca:
  t12 = Value1((s32 (*)())GameFlag_Test, 0x20e);
  if (t12 == 0)
    goto L_02009b06;
  t13 = Value1((s32 (*)())GameFlag_Test, 0x20c);
  if (t13 != 0)
    goto L_02009b30;
  t14 = Value1((s32 (*)())GameFlag_Test, 0x20d);
  if (t14 != 0)
    goto L_02009b30;
  Call1((void (*)())Func_02005388, -1);
  Call1((void (*)())GameFlag_ClearBit, 0x20e);
  goto L_02009b30;
L_02009b06:
  t17 = Value1((s32 (*)())GameFlag_Test, 0x20c);
  if (t17 != 0)
    goto L_02009b20;
  t18 = Value1((s32 (*)())GameFlag_Test, 0x20d);
  if (t18 == 0)
    goto L_02009b30;
L_02009b20:
  Call1((void (*)())Func_02005388, 162);
  Call1((void (*)())GameFlag_SetBit, 0x20e);
L_02009b30:
}
