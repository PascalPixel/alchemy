#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

void Func_020009a0(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(12));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x203);
}
