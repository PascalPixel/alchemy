#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_020030e0();

void Func_020017a0(void) {
  s32 t0;

  t0 = Func_020030e0();
  if (t0 != 0) {
    Import_EventSetExitRequest(0xc);
  }
}
