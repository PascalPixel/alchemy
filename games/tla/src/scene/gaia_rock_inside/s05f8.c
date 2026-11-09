#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

s32 Func_020005f8(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  FIELD_AT(FIELD_AT(t0, s32, 80), u16, 18) =
      (FIELD_AT(FIELD_AT(t0, s32, 80), u16, 18) + 0x300);
  return 0;
}
