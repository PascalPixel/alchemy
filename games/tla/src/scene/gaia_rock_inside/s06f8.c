#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02004dac();
extern u8 gData_0200559c[];

s32 Func_020006f8(void) {
  Func_02004dac(((s32)Actor_GetById(9)), (s32)gData_0200559c);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}
