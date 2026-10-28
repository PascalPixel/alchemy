#include "types.h"
#include "dma.h"

void Func_0803a8fc(const void *source, u8 *destination);

void Func_0803c170(const void *source, void *destination) {
  u8 buffer[128];

  Func_0803a8fc(source, buffer);
  Dma_Set(buffer, destination, DMA_COPY_WORDS(32), REG_DMA3);
}
