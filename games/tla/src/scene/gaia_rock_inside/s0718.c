#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02004dac();
s32 Actor_SetAnimationSpeed();
extern u8 gData_020055c0[];

s32 Func_02000718(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  Func_02004dac(t0, (s32)gData_020055c0);
  FIELD_AT(t0, s32, 8) = 0x1d80000;
  Actor_SetAnimationSpeed(t0, 0x10);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}
