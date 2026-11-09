#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02003bc8();
s32 Func_02003ca8();
s32 Func_02003bd0();
extern u8 gData_02003e4c[];

void Func_02001da8(void) {
  Func_02003bc8();
  Event_ResolvePendingActions(0);
  Func_02003ca8((s32)gData_02003e4c);
  Func_02003bd0();
}
