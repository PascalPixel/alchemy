#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "battle_effect.h"

/// Meteor Blow's horizontal-blank callback, the same routine as
/// particle_stream_modes.c's BattleFx_ArmPaletteHBlankDma: stops DMA 0, then
/// arms it to copy one BG palette word per scanline from the line table in the
/// effect work, so the backdrop's colour changes down the screen.
// The Broken Seal's BattleFx_ArmPaletteHBlankDma.
void BattleFx_ArmMeteorBlowPaletteDma(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u32 palette;

  /* FAKEMATCH: the one-pass block reads the control word back before its mask
   * is started (one reordered row, differ 2 without it). */
  do {
    channel[5] &= 0xc5ff; // clear the start timing and repeat
    channel[5] &= 0x7fff; // stop the channel
  } while (0);
  palette = 0xa0;
  /* FAKEMATCH: the empty asm starts the palette address before the control
   * word is read and shifts it after, as the native does; as a plain
   * constant, 2 instructions differ in tla-en. */
  __asm__("" : "+l"(palette));
  (void)channel[5];
  Dma_Set(work + 0x7640, (void *)(palette << 19), 0xa2600001,
          (volatile u32 *)channel);
}
