#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02004dac();
s32 ObjectDispatch_SetAnimationSpeed();
s32 Func_02004f34();
extern u8 gData_020055c0[];

s32 Func_02000718(void) {
  s32 t0;

  t0 = Value1(Object_GetById, 9);
  Call2(Func_02004dac, t0, (s32)gData_020055c0);
  FIELD_AT(t0, s32, 8) = 0x1d80000;
  Call2(ObjectDispatch_SetAnimationSpeed, t0, 0x10);
  Call2(Func_02004f34, 0xa, 0x102);
  return 0;
}
