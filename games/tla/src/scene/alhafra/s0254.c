#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 ObjectMotion_SetActionVariant();

void Func_02000254(void) { Call2(ObjectMotion_SetActionVariant, 0x19, 2); }
