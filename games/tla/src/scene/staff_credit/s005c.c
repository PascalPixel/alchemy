#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"

extern u32 gFrameCount;
extern u16 gData_02003dbc;
extern u16 gData_02003dbe;

/// Sets the layer blend, switching its alpha every two frames unless
/// gData_02003dbe is set.
void Func_0200005c(void) {
  u16 *blend;
  u16 blink;
  u16 value;
  u32 reg;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  reg = (u32)&REG_BLDALPHA;
  blink = gFrameCount & 2;
  blend = &gData_02003dbc;
  if (blink && gData_02003dbe == 0) {
    value = *blend | 15;
    QueueIoWriteDelay2(reg, value);
  } else {
    value = *blend | 16;
    QueueIoWriteDelay2(reg, value);
  }
}
