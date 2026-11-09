#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02004dac();
extern u8 gData_02005544[];

s32 Func_020006e0(void) {
  Func_02004dac(((s32)Actor_GetById(9)), (s32)gData_02005544);
  return 0;
}
