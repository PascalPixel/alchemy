#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_argument.h"

// The Broken Seal's BattleFx_ArmPaletteHBlankDma: stops DMA 0, then arms it
// to copy one palette line from the battle effect work into the BG palette
// at each horizontal blank.
void BattleFx_ArmPaletteHBlankDma(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u32 palette;

  /* FAKEMATCH: the one-pass block reads the control word back before its mask
   * is started (one reordered row, differ 2 without it). */
  do {
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
  } while (0);
  palette = 0xa0;
  /* FAKEMATCH: the empty asm holds the palette address's first half, so it is
   * started before the control word is read and shifted after, as the native
   * does; as a plain constant both halves follow the source offset (one
   * reordered row, differ 2 without it). */
  __asm__("" : "+l"(palette));
  (void)channel[5];
  Dma_Set(work + 0x7640, (void *)(palette << 19), 0xa2600001,
          (volatile u32 *)channel);
}

// The particle streams in mode 0.
DEFINE_EFFECT_KIND(BattleFx_RunMeteor, BattleFx_RunParticleStreams, 0)

// The particle streams in mode 1.
DEFINE_EFFECT_KIND(BattleFx_RunOuterSpace, BattleFx_RunParticleStreams, 1)
