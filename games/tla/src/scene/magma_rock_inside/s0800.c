#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02004c98();
void Func_02004ca0();
s32 GameFlag_Test();
void Actor_SetSingleChildFlags();
s32 Actor_GetById();
void Func_020007ac();

void Func_02000800(s32 a0, s32 a1, s32 a2) {
  s32 t1, t4, t5, v5, v6, v7;

  v5 = (a2 << 16);
  v7 = a1;
  t1 = Value0(Actor_GetById);
  v6 = t1;
  Call2(Func_02004c98, t1, 4);
  FIELD_AT(v6, u8, 90) = 0;
  FIELD_AT(v6, s32, 48) = 0x8000;
  FIELD_AT(v6, s32, 52) = 0x8000;
  FIELD_AT(v6, u8, 85) = 2;
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 4 of 53 instructions differ in
   * tla-en. */
  asm("");
  v5 = (v5 >> 16);
  v5 = (v5 << 16);
  FIELD_AT(v6, u8, 35) = (128 | FIELD_AT(v6, u8, 35));
  v5 = ((u32)v5 >> 16);
  Call2(Actor_SetSingleChildFlags, v6, 0);
  t4 = Value1(GameFlag_Test, v5);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 2 of 53 instructions differ in
   * tla-en. */
  asm("");
  if (t4 == 0)
    goto L_0200886a;
  FIELD_AT(v6, s32, 20) = 0x100000;
  FIELD_AT(v6, s32, 12) = 0x100000;
  t5 = Value1(GameFlag_Test, 0x109);
  if (t5 != 0)
    goto L_02008870;
  Call2(Func_02004ca0, v6, v7);
  goto L_02008870;
L_0200886a:
  FIELD_AT(v6, s32, 20) = -0xe0000;
  FIELD_AT(v6, s32, 12) = -0xe0000;
L_02008870:
  FIELD_AT(v6, s32, 108) = (s32)Func_020007ac;
}
