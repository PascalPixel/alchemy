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
