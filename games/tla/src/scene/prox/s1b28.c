#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_020019a4();

void Func_02001b28(void) { Call6(Func_020019a4, 0x34, 0x27, 0x32, 9, 2, 2); }
