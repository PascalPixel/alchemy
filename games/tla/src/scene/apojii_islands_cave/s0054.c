#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();

void Func_02000054(s32 a0, s32 a1) { Call1(GameFlag_SetBit, (a1 + 0x8e1)); }
