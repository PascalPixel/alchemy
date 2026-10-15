/* Near miss of Func_02000e30 (overlay 690, 96 bytes), differ 2 of 35: above
   the split line the native loads the pooled zero of the blend control between
   the two halves of its address (movs r2, #128; ldr r3; lsls r2, r2, #19), this
   form after both. Measured: the store in a one-pass block, the same; without
   the value local the scroll offset is held in r3 and its address in r2
   (differ 40); four minutes of permuter found nothing better. Overlay 690's
   Func_02000e90, which sets the three values this reads, is a draft too. */
#include "../../../../../games/tla/src/scene/settlements/local.h"
#include "io_reg.h"

void Func_02000e30(void)
{
    u32 value;

    if (REG_VCOUNT < Data_02002ce0) {
        value = Data_02002ce4;
        REG_BG1HOFS = value;
        REG_BLDCNT = 0;
    } else {
        value = Data_02002ce6;
        REG_BG1HOFS = value;
        REG_BLDALPHA = 0x100c;
        REG_BLDCNT = 0x3f42;
    }
}
