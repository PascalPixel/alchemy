#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_argument.h"

/// Stops DMA 0, then arms it to copy one palette line from the battle effect
/// work into the BG palette at each horizontal blank.
// The Broken Seal's BattleEffect_ArmPaletteHBlankDma.
void BattleEffect_ArmPaletteHBlankDma(void) {
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
  /* FAKEMATCH: the empty asm starts the palette address before the control
   * word is read and shifts it after; as a plain constant, 2 instructions
   * differ in tla-en. */
  __asm__("" : "+l"(palette));
  (void)channel[5];
  Dma_Set(work + 0x7640, (void *)(palette << 19), DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// The particle streams in mode 0.
DEFINE_EFFECT_KIND(BattleEffect_RunMeteor, BattleEffect_RunParticleStreams, 0)

/// The particle streams in mode 1.
DEFINE_EFFECT_KIND(BattleEffect_RunOuterSpace, BattleEffect_RunParticleStreams, 1)
