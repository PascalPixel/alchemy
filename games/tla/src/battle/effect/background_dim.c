#include "types.h"
#include "ram_buffer.h"

struct BattleFxFadeWork {
  s32 unk00;
  s32 halvePending;
  s32 framesLeft;
  s32 step;
};

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);

// Each frame while frames are left, darkens the battle background's 128
// colours by one more step of 1092 into BG palette 0x050000c0.
void BattleFx_DimBackgroundStep(void) {
  struct BattleFxFadeWork *work =
      (struct BattleFxFadeWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *battle = (u8 *)Ram_HeapSlots->battleWork;

  if (work->framesLeft > 0) {
    work->step++;
    ColorBuffer_ScaleFar(battle + 0x56c, (void *)0x050000c0,
                         0x10000 - work->step * 1092, 128);
    work->framesLeft--;
  }
}
