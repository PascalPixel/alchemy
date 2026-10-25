// Draft, differ 94 of 165: overlay 69e's copy of the function drafted in
// resource_698_c_0200091c.c (overlay 698's Func_0200091c), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_GetById();
s32 Func_020028f8();
s32 GameFlag_Test();
s32 Animation_ApplyChildValues();

void Func_020013a8(void) {
  s32 t7;
  s32 t0;
  s32 t1;
  s32 t2;
  s32 t8;
  s32 t13;
  s32 t18;
  s32 t19;

  t0 = Value1(Object_GetById, 9);
  t1 = Value1(Object_GetById, 0xa);
  t2 = Value1(Object_GetById, 0x17);
  Call3(Func_020028f8, 0x18, 0x2000000, 0xf80000);
  Call3(Func_020028f8, 0x17, 0x2000000, 0xf80000);
  Call3(Func_020028f8, 9, 0x1d80000, 0x1000000);
  Call3(Func_020028f8, 0xa, 0x2280000, 0x1000000);
  t7 = Value1(Object_GetById, 0x18);
  FIELD_AT(t7, s32, 24) = 0;
  FIELD_AT((0x55 + t2), u8, 0) = 0;
  FIELD_AT(t2, s32, 12) = FIELD_AT(t2, s32, 20);
  FIELD_AT((0x55 + t0), u8, 0) = 0;
  FIELD_AT(t0, s32, 12) = FIELD_AT(t0, s32, 20);
  FIELD_AT((0x55 + t1), u8, 0) = 0;
  FIELD_AT(t1, s32, 12) = FIELD_AT(t1, s32, 20);
  t8 = Value1(GameFlag_Test, 0xa0a);
  if (t8 != 0) {
    FIELD_AT(t0, s32, 12) = (FIELD_AT(t0, s32, 12) + 0x66640);
    FIELD_AT(t2, s32, 12) = (FIELD_AT(t2, s32, 12) + 0x1eb80);
    FIELD_AT(t2, s32, 8) = (FIELD_AT(t2, s32, 8) + -167744);
    FIELD_AT(FIELD_AT(t2, s32, 80), u16, 18) =
        (FIELD_AT(FIELD_AT(t2, s32, 80), u16, 18) + 0x800);
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 9), 4);
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 0xb), 4);
  }
  t13 = Value1(GameFlag_Test, 0xa0b);
  if (t13 != 0) {
    FIELD_AT(t1, s32, 12) = (FIELD_AT(t1, s32, 12) + 0x66640);
    FIELD_AT(t2, s32, 12) = (FIELD_AT(t2, s32, 12) + 0x1eb80);
    FIELD_AT(t2, s32, 8) = (FIELD_AT(t2, s32, 8) + 0x28f40);
    FIELD_AT(FIELD_AT(t2, s32, 80), u16, 18) =
        (FIELD_AT(FIELD_AT(t2, s32, 80), u16, 18) + -2048);
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 0xa), 4);
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 0xc), 4);
  }
  t18 = Value1(GameFlag_Test, 0xa0a);
  if (t18 != 0) {
    t19 = Value1(GameFlag_Test, 0xa0b);
    if (t19 != 0) {
      FIELD_AT(t0, s32, 12) = (FIELD_AT(t0, s32, 12) + 0x199900);
      FIELD_AT(t1, s32, 12) = (FIELD_AT(t1, s32, 12) + 0x199900);
      FIELD_AT(t2, s32, 12) = (FIELD_AT(t2, s32, 12) + 0x1b8480);
      FIELD_AT((0x55 + t0), u8, 0) = 4;
      FIELD_AT((0x55 + t1), u8, 0) = 4;
      FIELD_AT((0x55 + t2), u8, 0) = 4;
    }
  }
}
