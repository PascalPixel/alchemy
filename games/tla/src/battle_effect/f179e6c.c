#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

void Graphics_LoadGreyRamp(void);

void Func_08179e6c(void) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 y;
  s32 x;

  Graphics_LoadGreyRamp();
  REG_BG2CNT = 0x784;
  PLTT_BACKDROP = 0;
  REG_BG2PA = 0x4e;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  for (y = 0; y != 128; y++) {
    for (x = 0; x != 128; x++) {
      s32 level;

      if (y < 32)
        level = y * 2;
      else if (y > 95)
        level = 255 - y * 2;
      else
        level = 63;
      canvas[(((x / 8) * 16 + y / 8) * 8 + (x & 7)) * 8 + (y & 7)] = level;
    }
  }
}
