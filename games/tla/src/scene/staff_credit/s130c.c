#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"

extern u32 gFrameCount;
extern u16 gData_02003fc0;

/// Sets the layer blend, switching its alpha every two frames.
void Func_0200130c(void) {
  u32 alphaRegister = (u32)&REG_BLDALPHA;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2) {
    QueueIoWriteDelay2(alphaRegister, gData_02003fc0 | 12);
  } else {
    QueueIoWriteDelay2(alphaRegister, gData_02003fc0 | 16);
  }
}
