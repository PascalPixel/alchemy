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
s32 ObjectDispatch_ApplyValueToChildren();
s32 Func_02004f34();
extern u8 gData_02005610[];

s32 Func_02000748(void) {
  s32 t0;

  t0 = Value1(Object_GetById, 9);
  Call2(Func_02004dac, t0, (s32)gData_02005610);
  Call2(ObjectDispatch_ApplyValueToChildren, t0, 0);
  Call2(Func_02004f34, 0xa, 0x102);
  return 0;
}
