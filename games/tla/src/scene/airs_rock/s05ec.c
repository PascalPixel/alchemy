#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02003bc8();
s32 Func_02003bd0();

void Func_020005ec(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(10));
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  if ((FIELD_AT(t0, s32, 8) >> 20) == 0x1f) {
    Event_SetFlag(0x310);
  } else {
    Event_ClearFlag(0x310);
  }
  Func_02003bd0();
}
