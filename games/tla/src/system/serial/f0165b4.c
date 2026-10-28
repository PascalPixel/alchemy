#include "serial.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write.h"

u8 Serial_CollectReceivedPayloads(void *payload) {
  u8 flags[8];
  s32 i;
  u32 k;
  u16 *packet;
  u16 *swap;
  s32 sum;

  *(u32 *)&flags[4] = 0;
  Io_Write16(0, &REG_IME);
  for (i = 0; i <= 3; i++) {
    swap = gSerialWork.pendingBuffer[i];
    gSerialWork.pendingBuffer[i] = gSerialWork.readyBuffer[i];
    gSerialWork.readyBuffer[i] = swap;
  }
  *(u32 *)flags = *(u32 *)gSerialWork.channelFlags;
  *(u32 *)gSerialWork.channelFlags = 0;
  Io_Write16(1, &REG_IME);
  gSerialWork.currentMask = 0;
  /* FAKEMATCH(barrier): an empty asm holds the currentMask store ahead of the
   * loop setup; as plain statements, 2 of 106 instructions differ in tla-en. */
  asm volatile("");
  for (i = 0; i <= 1; i++) {
    packet = gSerialWork.pendingBuffer[i];
    sum = 0;
    for (k = 0; k <= 13; k++)
      sum += packet[k];
    if (flags[i] == 1 && (s16)sum == -1) {
      Dma_Set(gSerialWork.pendingBuffer[i] + 2, (u8 *)payload + i * 24,
              DMA_COPY_WORDS(6), REG_DMA3);
      gSerialWork.currentMask |= 1 << i;
    }
    if ((s16)sum == -1)
      gSerialWork.pendingBuffer[i][1] = ~gSerialWork.pendingBuffer[i][1];
  }
  gSerialWork.receivedMask |= gSerialWork.currentMask;
  return gSerialWork.currentMask;
}
