#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"

extern u32 gFrameCount;
extern u16 gData_02003fc0;

/// Sets the layer blend, switching its alpha every two frames.
void Func_0200130c(void) {
  u16 *blend;
  u16 blink;
  u16 value;
  u32 reg;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  reg = (u32)&REG_BLDALPHA;
  blink = gFrameCount & 2;
  blend = &gData_02003fc0;
  if (blink) {
    value = *blend | 12;
    QueueIoWriteDelay2(reg, value);
  } else {
    value = *blend | 16;
    QueueIoWriteDelay2(reg, value);
  }
}
