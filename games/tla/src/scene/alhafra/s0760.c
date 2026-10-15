#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02001cf4();

void Func_02000760(void) { Call3(Func_02001cf4, 0x40, 0xb80000, 0x2280000); }
