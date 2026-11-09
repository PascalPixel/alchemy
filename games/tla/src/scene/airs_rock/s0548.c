#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
void Func_02003bc8(void);
void Func_02003bd0(void);
extern u8 MsgFieldWindStoneWarning[];

void Func_02000548(void) {
  Func_02003bc8();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgFieldWindStoneWarning, 1);
  Func_02003bd0();
}
