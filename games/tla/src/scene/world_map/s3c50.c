#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"

extern u32 gFrameCount;
extern u16 gData_02006e08;

static inline void QueueBlend(u32 address, u32 value) {
  /* FAKEMATCH: forwarding through the inline sets the arguments up in their own
   * order: the native finishes the value's orr before the register address's
   * add (differ 12 as direct calls). */
  QueueIoWriteDelay2(address, value);
}

/// Every other two frames the world map's blend alternates between two
/// strengths.
void Func_02003c50(void) {
  s32 odd;
  u16 *alpha;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  odd = gFrameCount & 2;
  alpha = &gData_02006e08;
  if (odd)
    QueueBlend((u32)&REG_BLDALPHA, *alpha | 12);
  else
    QueueBlend((u32)&REG_BLDALPHA, *alpha | 16);
}
