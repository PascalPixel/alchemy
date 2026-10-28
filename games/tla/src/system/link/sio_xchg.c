/// @file
/// Link: exchange per-frame data and assemble outgoing packets.

#include "serial_runtime.h"
#include "dma.h"
#include "io_reg.h"

/// In phase 1, collects incoming data in receive and builds the next packet
/// from send. Returns the link status: bits 0-7 currentMask (with bit 7 also
/// set in mode 8), bits 8-15 receivedMask, 0x1000 for the parent and 0x2000
/// when the SIOCNT multiplayer ID is at least 2.
s32 Serial_ExchangePayloads(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 cnt;
  s32 st;
  s32 ret;

  siocnt = &REG_SIOCNT;
  cnt = *siocnt;
  if (gSerialWork.phase == 1) {
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    gSerialWork.packetNumber++;
  }
  st = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == 8)
    st |= 0x80;
  ret = Serial_AddErrorFlag(st);
  if (((cnt << 26) >> 30) > 1) // Bits 4 and 5.
    ret |= 0x2000;
  return ret;
}

/// Builds the outgoing packet in sendBuffer[0], in the same format as above.
void Serial_PrepareSendPacket(void *payload) {
  volatile struct SerialWork *sr;
  u32 i;
  u32 sum;

  sr = SERIAL_RUNTIME;
  {
    u8 *pkt;
    u8 cur;
    u8 recv;

    pkt = (u8 *)sr->sendBuffer[0];
    pkt[0] = sr->packetNumber;
    cur = sr->currentMask;
    recv = sr->receivedMask;
    sum = 0;
    /* FAKEMATCH: ⚓️ clears the checksum only after reading both masks */
    *(volatile u16 *)(pkt + 2) = sum;
    pkt[1] = recv ^ cur;
    Dma_Set(payload, pkt + 4, (DMA_ENABLE | DMA_32BIT) << 16 | 6, REG_DMA3);
  }
  {
    u16 *pkt;
    u32 v;

    pkt = sr->sendBuffer[0];
    for (i = 0; i <= 13; i++) {
      v = *pkt;
      pkt++;
      sum += v;
    }
  }
  {
    u32 inv;
    struct SerialPacket *pkt;

    inv = ~sum;
    pkt = (struct SerialPacket *)sr->sendBuffer[0];
    pkt->checksum = inv;
  }
  if (sr->mode != 0)
    REG_TM3CNT_H = 0;
  sr->sendIndex = -1;
  if (sr->mode != 0 && sr->transferEnabled != 0)
    REG_TM3CNT_H = TIMER_INTR_ENABLE | TIMER_ENABLE;
}
