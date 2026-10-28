#include "types.h"
#include "adventure.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_02004a4c();
void Func_02004b04();
void Func_02004c7c();
void Func_02004c84();
s32 GameFlag_Test();
s32 Object_GetById();
void Object_SetModeById();

void Func_02000fa4(void) {
  s32 t1, t2, t8, t10, t12, t13, t18, t19, t24, v_r5;

  t1 = Value1((s32 (*)())GameFlag_Test, 0x8a6);
  if (t1 == 0)
    goto L_02009022;
  t2 = Value1((s32 (*)())Object_GetById, 15);
  v_r5 = t2;
  Call3((void (*)())Func_02004b04, 15, 0x1500000, 0x2180000);
  Call2((void (*)())Object_SetModeById, 15, 4);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  FIELD_AT(v_r5, u8, 85) = 2;
  FIELD_AT(v_r5, s32, 40) = 0;
  FIELD_AT(v_r5, s32, 12) = -0x40000;
  FIELD_AT(v_r5, s32, 20) = -0x40000;
  Value4((s32 (*)())Func_02004c7c, 0x1500000, 0x2100000, 2, 0);
  Value4((s32 (*)())Func_02004c7c, 0x1500000, 0x2100000, 0, 0);
  Call4((void (*)())Func_02004c7c, 0x1600000, 0x2100000, 0, 0);
L_02009022:
  t8 = Value1((s32 (*)())GameFlag_Test, 0x8a4);
  if (t8 == 0)
    goto L_0200903a;
  Call3((void (*)())Func_02004b04, 16, 0, 0);
L_0200903a:
  t10 = Value1((s32 (*)())GameFlag_Test, 0x8a5);
  if (t10 == 0)
    goto L_02009052;
  Call3((void (*)())Func_02004b04, 17, 0, 0);
L_02009052:
  t12 = Value1((s32 (*)())GameFlag_Test, 0x8a0);
  if (t12 == 0)
    goto L_020090a8;
  t13 = Value1((s32 (*)())Object_GetById, 10);
  v_r5 = t13;
  Call3((void (*)())Func_02004b04, 10, 0x1f80000, 0x2080000);
  Call2((void (*)())Object_SetModeById, 10, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  asm("");
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  Value4((s32 (*)())Func_02004c7c, FIELD_AT(v_r5, s32, 8),
         FIELD_AT(v_r5, s32, 16), 0, 0);
  Call4((void (*)())Func_02004c84, FIELD_AT(v_r5, s32, 8),
        FIELD_AT(v_r5, s32, 16), 0, 128);
L_020090a8:
  t18 = Value1((s32 (*)())GameFlag_Test, 0x8a1);
  if (t18 == 0)
    goto L_02009100;
  t19 = Value1((s32 (*)())Object_GetById, 11);
  v_r5 = t19;
  Call3((void (*)())Func_02004b04, 11, 0x1980000, 0x2180000);
  Call2((void (*)())Object_SetModeById, 11, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  asm("");
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  Value4((s32 (*)())Func_02004c7c, FIELD_AT(v_r5, s32, 8),
         FIELD_AT(v_r5, s32, 16), 0, 0);
  Call4((void (*)())Func_02004c84, FIELD_AT(v_r5, s32, 8),
        FIELD_AT(v_r5, s32, 16), 0, 128);
L_02009100:
  t24 = Value1((s32 (*)())GameFlag_Test, 0x8a7);
  if (t24 == 0)
    goto L_02009122;
  Call6((void (*)())Func_02004a4c, 32, 24, 2, 2, 30, 24);
L_02009122:
}
