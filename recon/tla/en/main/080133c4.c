// Matches: differ 0 (tla-en, 2026-11-08)
// ☀️'s System_InstallIwramAndIrqs with ⚓️'s 1024-word runtime copy and the
// gamepak interrupt enabled. One form serves all six editions: tla-ja and
// tla-en at 080133c4, tla-de, tla-es, tla-fr and tla-it at 080133f0, each
// listing 0x74 bytes, this function alone; every edition measures differ 0,
// and each edition's ELF holds the same nine pool words in the draft's order.
// The native pool holds 0x03000100 twice, the copy's destination and the
// vector's value, so one of them is a constant (agscc's pool merges a symbol
// named twice): the destination is Camelot's fixed address of IwramIrqMain,
// the value the symbol. The vector itself is the BIOS's user interrupt
// pointer at 0x03007ffc (GBATEK; pret's INTR_VECTOR), written through that
// fixed address with gIrqVector as its linked name. As a constant its
// address is an output dependence on the DMA asm, so sched2 takes the
// value's pool load (a true dependence on the asm's memory clobber) first,
// and local-alloc gives the shorter-lived value r3: the native str r3, [r2]
// and pool order. Spelled as the symbol gIrqVector, the address was a pool
// load too, and every arrangement either swapped the store's registers or
// loaded the address first (differ 2 to 12 in earlier rounds, with permutes).
// Before adoption both fixed addresses belong in irq.h, each listed once
// (O2), and 0xc3ff is ☀️'s KEYCNT_DISARMED, not yet in ⚓️'s io_reg.h.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write.h"
#include "irq.h"

void System_InstallIwramAndIrqs(void) {
  Io_Write16(0, &REG_IME);
  Dma_Set(IwramResident_Rom, FIXED_ADDRESS((u8 *), 0x03000100, IwramIrqMain),
          DMA_COPY_WORDS(1024), REG_DMA3);
  *FIXED_ADDRESS((void **), 0x03007ffc, gIrqVector) = IwramIrqMain;
  Dma_Set(gDefaultIrqHandlers, gIrqHandlers, DMA_COPY_WORDS(IRQ_COUNT),
          REG_DMA3);
  Io_Write16(0, &REG_DISPSTAT);
  Io_Write16(0xc3ff, &REG_KEYCNT);
  Io_Write16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK, &REG_IE);
  Io_Write16(1, &REG_IME);
}
