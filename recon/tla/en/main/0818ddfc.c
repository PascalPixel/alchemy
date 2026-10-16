// Draft, differ 0 of 29 with two devices, 2 without: summon 29's palette
// HBlank DMA arm. The Broken Seal's twin is BattleFx_ArmPaletteHBlankDma;
// The Lost Age has the same routine once more as Func_08164bb4 (adopted in
// particle_stream_modes.c with these same two devices).
// Plain (no device): differ 4. The native reads the control word back before
// it starts the first mask (0xc5ff); here the mask is started first. A
// one-pass block around the two masks closes that (differ 2).
// Left without the barrier: the native starts the palette address
// (movs r1, #160) before the read-back of the control word and shifts it
// after; agscc starts it after the work offset 0x7640 (one reordered row).
// Tried without a barrier, each differ 2: the address as BG_PLTT, as a
// pointer local set before the masks, inside the masks' block, in its own
// one-pass block, with the read-back in a one-pass block, as a for (;;) with
// break, as &work[0x7640], and the control word read into a local; the work
// offset in a block with the read-back gave 4. The empty asm that holds
// palette's first half closes it, as on Func_08164bb4.
// Not adopted: the barrier would raise The Lost Age's count of empty asm
// barriers, which S2 says may only fall; whether that covers new adoptions
// is Pascal's open decision D2.
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "battle_effect.h"

// Summon 29's horizontal-blank callback: stops DMA 0, then arms it to copy
// one BG palette word per scanline from the line table in the effect work,
// so the backdrop's colour changes down the screen.
void Func_0818ddfc(void) {
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
  /* FAKEMATCH: the empty asm holds the palette address's first half, so it is
   * started before the control word is read and shifted after, as the native
   * does; as a plain constant both halves follow the work offset (one
   * reordered row, differ 2 without it). */
  __asm__("" : "+l"(palette));
  (void)channel[5];
  Dma_Set(work + 0x7640, (void *)(palette << 19), 0xa2600001,
          (volatile u32 *)channel);
}
