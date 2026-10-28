// Draft, differ 12 of 40 (was 28): ☀️'s System_InstallIwramAndIrqs with
// ⚓️'s 0x400-word runtime copy and the gamepak interrupt enabled. The native
// pool holds IwramIrqMain twice: the copy's destination and the vector's
// value are two entries, which agscc's pool merges whenever both are the
// same symbol, so every later pool offset is 4 short. The empty asm on the
// destination keeps CSE from carrying it to the vector store (the vector's
// value is reloaded after the copy, as in the native); without it the
// address stays in r4 and the registers shift (differ 30). A second
// entry would need a second name for 0x03000100, which O2 forbids unless
// evidence shows the source had one (the IWRAM runtime block it copies is
// a candidate).
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"

void System_InstallIwramAndIrqs(void) {
  u8 *iwram;

  Io_Write16(0, &REG_IME);
  iwram = IwramIrqMain;
  asm("" : "+r"(iwram));
  Dma_Set(IwramRuntime_Rom, iwram, (DMA_ENABLE | DMA_32BIT) << 16 | 0x400,
          REG_DMA3);
  gIrqVector = IwramIrqMain;
  Dma_Set(gDefaultIrqHandlers, gIrqHandlers,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0xe, REG_DMA3);
  Io_Write16(0, &REG_DISPSTAT);
  Io_Write16(0xc3ff, &REG_KEYCNT);
  Io_Write16(0x3001, &REG_IE);
  Io_Write16(1, &REG_IME);
}
