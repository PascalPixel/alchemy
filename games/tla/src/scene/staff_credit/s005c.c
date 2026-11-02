#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"

extern u32 gFrameCount;
extern u16 gData_02003dbc;
extern u16 gData_02003dbe;

/// Sets the layer blend, switching its alpha every two frames unless
/// gData_02003dbe is set.
void Func_0200005c(void) {
  u32 alphaReg = (u32)&REG_BLDALPHA;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if ((gFrameCount & 2) && gData_02003dbe == 0) {
    QueueIoWriteDelay2(alphaReg, gData_02003dbc | 15);
  } else {
    QueueIoWriteDelay2(alphaReg, gData_02003dbc | 16);
  }
}
