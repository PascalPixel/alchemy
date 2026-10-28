#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct BattleFxTransferWork {
  u8 unk0000[0x7780];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[0x78e4 - 0x7788];
  s32 vblanksSinceTransfer;
  s32 transferPending;
};

void ColorBuffer_HalveNonzero(u8 *source, u8 *destination, s32 size);
void ColorBuffer_ScaleNonzeroThreeQuarters(u8 *source, u8 *destination,
                                           s32 size);

static __inline__ s32 BattlePres_FillWords(void *destination, s32 size,
                                           u32 value) {
  return Iwram_FillWords(destination, size, value);
}

/// Sends the 256x128 8bpp canvas to VRAM 0x06003500 on the frames its transfer
/// is pending, then clears or fades it; a task, as The Broken
/// Seal's.
void BattlePres_ProcessPendingTileTransfer(void) {
  struct BattleFxTransferWork *work;
  u8 *buf;

  /* FAKEMATCH: the one-pass block loads the effect work before the pending
   * offset is started; assigned directly the offset is started first (differ
   * 2). */
  do {
    work = (struct BattleFxTransferWork *)Ram_HeapSlots->battleFxWork[0];
  } while (0);
  if (work->transferPending == 1) {
    buf = Ram_HeapSlots->battleFxWork[1];
    switch (work->transferMode) {
    case 1: // Send it, then fill it with transferValue.
      Dma_Set(buf, (void *)0x06003500, (DMA_ENABLE | DMA_32BIT) << 16 | 0x2000,
              REG_DMA3);
      /* FAKEMATCH: the fill goes through the inline wrapper
       * BattlePres_FillWords, as ☀️'s IWRAM call layer; called directly, 4
       * instructions differ in tla-en. */
      BattlePres_FillWords(buf, 0x8000, work->transferValue);
      break;
    case 2: // Afterimage: nonzero colour numbers drop to 1/2 or 3/4.
      if (work->transferValue == 50)
        ColorBuffer_HalveNonzero(buf, (u8 *)0x06003500, 0x8000);
      else
        ColorBuffer_ScaleNonzeroThreeQuarters(buf, (u8 *)0x06003500, 0x8000);
      break;
    }
    work->transferPending = 0;
    work->vblanksSinceTransfer = 1;
  } else {
    work->vblanksSinceTransfer++;
  }
}

void Palette_DarkenSceneStep(void) {
  u16 *color;
  s32 i;

  color = (u16 *)0x05000140;
  for (i = 0; i != 16; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    blue--;
    green--;
    red--;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }
  color = (u16 *)0x05000202;
  for (i = 0; i != 239; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    blue--;
    green--;
    red--;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = RGB(red, green, blue);
  }
}
