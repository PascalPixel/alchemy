#include "types.h"
#include "io_reg.h"

/// BG2 back to one texel per pixel across.
void Graphics_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

/// BG2 at half a texel per pixel across: twice as wide.
void Graphics_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }
