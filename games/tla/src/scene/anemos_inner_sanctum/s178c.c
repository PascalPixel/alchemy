#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_020030e0();
s32 Func_02003450();

void Func_0200178c(void) {
  s32 t0;

  t0 = Func_020030e0();
  if (t0 != 0) {
    Call1(Func_02003450, 0xb);
  }
}
