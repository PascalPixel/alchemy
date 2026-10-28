#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();

void Func_020008f0(s32 a0, s32 a1) {
  s32 t0;

  t0 = Actor_GetById();
  if (t0 != 0) {
    FIELD_AT((t0 + 0x54), u8, 0) = a1;
  }
}
