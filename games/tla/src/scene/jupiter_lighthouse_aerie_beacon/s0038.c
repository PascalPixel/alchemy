#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Actor_SetSingleChildFlags();

s32 Func_02000038(s32 a0) {
  Sound_Play(0x98);
  Actor_SetSingleChildFlags(a0, 0);
  return 0;
}
