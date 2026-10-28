// Draft, differ 6 of 113 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Draft, differ 16 of 113, instruction order only: the perspective map's
// HBlank setup each frame, ☀️'s WorldMap_ApplyAffinePage
// (games/tbs/src/field/field/world_map.c) written over ⚓️'s heap slots:
// stops HBlank DMA 0, loads the first scanline's BG2 and BG3 affine
// registers from this frame's page and starts the HBlank DMA for the rest,
// then sets the window bits of DISPCNT. Left: eight single rows placed one or
// two slots apart (the heap slot load, the DISPCNT mask, the second DMA
// control read, a word copy and the window bottom read).
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "frame.h"

struct MapScanline {
  u8 unk00[20];
};

struct MapAffinePair {
  u32 words[8];
};

struct MapAffineWork {
  struct MapScanline scanlines[160];
  struct MapAffinePair pages[2][160];
};

struct PerspectiveWindow {
  u8 unk000[0x104];
  u16 windowTop;
  u16 windowBottom;
  u16 shownTop;
  u16 shownBottom;
  u16 windowPending;
};

void Func_0802c088(void) {
  struct MapAffinePair(*affinePages)[160] =
      ((struct MapAffineWork *)Ram_HeapSlots->unk1c[0])->pages;
  s32 displayControl = (s16)(REG_DISPCNT & 0xfff8);
  u32 *affineRegisters = (u32 *)0x04000020;
  struct PerspectiveWindow *perspective =
      (struct PerspectiveWindow *)Ram_HeapSlots->mapWork;
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

  dma0[5] &= 0xc5ff;
  dma0[5] &= 0x7fff;
  (void)dma0[5];
  if (affinePages != NULL) {
    affineSource = (u32 *)affinePages[gFrameCount & 1];
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters = *affineSource++;
    Dma_Set(affineSource, (void *)0x04000020, 0xa6600008, (volatile u32 *)dma0);
  }
  perspective->shownTop = perspective->windowTop;
  perspective->shownBottom = perspective->windowBottom;
  windowTop = perspective->shownTop;
  mode = 0;
  if (windowTop < 200) {
    u16 bottom = perspective->shownBottom;

    mode = (bottom != 0) << 1;
    if (windowTop <= bottom) {
      mode = 0;
      if (windowTop == 0)
        mode = 2;
    }
  }
  displayControl |= mode;
  nextDisplayControl = (u16)displayControl;
  REG_DISPCNT = nextDisplayControl;
  perspective->windowPending = 0;
}
