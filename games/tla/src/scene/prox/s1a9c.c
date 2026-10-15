#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02001a9c(void) { Func_020019a4(0x30, 0x27, 0x39, 0x1a, 1, 2); }
