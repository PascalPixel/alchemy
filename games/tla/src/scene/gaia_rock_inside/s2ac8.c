#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_GetById();
s32 ObjectMotion_SetAngleToward();
s32 ResourceMetadata_Register();
s32 Func_02004d9c();

void Func_02002ac8(s32 a0, s32 a1) {
  s32 t2;
  s32 t0;

  t0 = Object_GetById();
  Call3(ObjectMotion_SetAngleToward, a0, a1, 0);
  if (t0 != 0) {
    t2 = Value2(ResourceMetadata_Register, FIELD_AT(t0, s32, 80), 0x185);
    FIELD_AT(t2, u8, 5) = 0;
    FIELD_AT(t2, u8, 6) = 0;
    Call2(Func_02004d9c, t0, 0);
    Call2(Func_02004d9c, t0, 4);
  }
}
