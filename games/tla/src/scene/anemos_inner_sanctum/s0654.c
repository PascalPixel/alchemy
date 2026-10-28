#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_SetSingleChildFlags();

s32 Func_02000654(s32 a0) {
  Call2(Actor_SetSingleChildFlags, a0, 0);
  return 0;
}
