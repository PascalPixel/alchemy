// Draft, differ 2 of 40 in every edition (was 12): ☀️'s
// System_InstallIwramAndIrqs with ⚓️'s 0x400-word runtime copy and the
// gamepak interrupt enabled. One form serves all six editions: tla-ja and
// tla-en at 080133c4, tla-de, tla-es, tla-fr and tla-it at 080133f0, each
// listing 0x74 bytes, this function alone, and every edition measures the
// same 2 instructions; no edition difference is known.
// The native pool holds 0x03000100 twice: the copy's destination and the
// vector's value are two entries, so one of them is a constant rather than
// the symbol, which agscc's pool would merge. Here the vector's value is
// Camelot's fixed address of IwramIrqMain (FIXED_ADDRESS, as iwram_call.h
// writes the other resident IWRAM routines); spelled as the symbol, the
// entries merge and every later pool offset is 4 short (differ 12). As the
// destination instead, the pool order is reversed (differ 4). Remaining: the
// vector store's two registers are swapped (str r2, [r3] for str r3, [r2]),
// register allocation only; a 4-minute permute found nothing better.
// Before adoption the fixed address belongs in irq.h, listed once (O2).
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write.h"
#include "irq.h"

void System_InstallIwramAndIrqs(void) {
  u8 *iwram;

  Io_Write16(0, &REG_IME);
  iwram = IwramIrqMain;
  Dma_Set(IwramResident_Rom, iwram, (DMA_ENABLE | DMA_32BIT) << 16 | 0x400,
          REG_DMA3);
  iwram = FIXED_ADDRESS((u8 *), 0x03000100, IwramIrqMain);
  gIrqVector = iwram;
  Dma_Set(gDefaultIrqHandlers, gIrqHandlers,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0xe, REG_DMA3);
  Io_Write16(0, &REG_DISPSTAT);
  Io_Write16(0xc3ff, &REG_KEYCNT);
  Io_Write16(0x3001, &REG_IE);
  Io_Write16(1, &REG_IME);
}
