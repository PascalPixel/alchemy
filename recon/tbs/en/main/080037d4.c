/* Draft, not exact: 22 differing halfwords, 48-byte candidate for the
   54-byte owner (2026-09-23). Residual: the reference multiplies a copy of
   the preset index (adds r2, r0; muls r2, r3) and pads before its pool;
   every spelling tried multiplies r0 in place, six bytes shorter. */

#include "DMA.H"

/* Copies one of five 152-byte parameter presets from ROM into IWRAM. */
s32 Sound_LoadPresetParameters(u32 preset)
{
    if (preset > 4)
        preset = 0;
    Dma_Set((const void *)(0x08000404 + preset * 152), (void *)0x03000bd8, 0x84000026,
        (volatile u32 *)0x040000d4);
    return 0;
}
