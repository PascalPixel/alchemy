#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02002b5c();

void Func_02000178(void) { Call3(Func_02002b5c, 0x40, 0, 0); }
