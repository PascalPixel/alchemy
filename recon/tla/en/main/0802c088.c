// Near miss, differ 2 of 113 (tla-en, 2026-10-31): one pair in sched2's
// order. The listing stores the cleared DMA 0 enable bit, then moves 128
// into r4 to build affineRegisters (0x04000020); the draft builds it first.
// In sched2 both are ready with priority 3 and equal dependants, so RTL order
// picks the address (its constant load sits at the declaration). Assigning
// affineRegisters after the DMA stores, or inside the if, moves the global
// allocation instead (58 or 83 differ); declaring it first changes nothing.
// The perspective map's HBlank setup each frame, ☀️'s WorldMap_ApplyAffinePage
// (games/tbs/src/field/field/world_map.c) written over ⚓️'s heap slots:
// stops HBlank DMA 0, loads the first scanline's BG2 and BG3 affine
// registers from this frame's page and starts the HBlank DMA for the rest,
// then sets the window bits of DISPCNT.
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
  /* FAKEMATCH(one-pass): mode cleared in a one-pass block before shownTop is
   * read back, which sets the zero before that load; as plain statements, 4
   * of 113 instructions differ in tla-en. */
  do {
    mode = 0;
  } while (0);
  windowTop = perspective->shownTop;
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
