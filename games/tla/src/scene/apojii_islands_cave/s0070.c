#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"

s32 Func_02000070(void) {
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x201;
  return 0;
}
