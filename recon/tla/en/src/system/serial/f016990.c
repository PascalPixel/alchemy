#include "types.h"
#include "io_reg.h"
#include "serial.h"

struct SerialHalfword {
  u16 v;
};

void Serial_StepBlockTransfer(void) {
  struct SerialTransferState *peer;
  u32 id;
  struct SerialTransferState *own;
  u32 gap;
  /* FAKEMATCH(temporary): the 1 kept in a halfword record, loaded from the
   * pool into r8 at entry; as the constant 1 at both stores, 86 of 268
   * instructions differ in tla-en. */
  struct SerialHalfword one;

  id = ~((REG_SIOCNT << 26) >> 30) & 1;
  one.v = 1;
  own = &gSerialTransfer;
  peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
  if ((gLinkStatus & 3) != 3)
    return;
  {
    s32 *receive = &gSerialReceiveDest;
    s32 dest = *receive;
    if (dest != 0) {
      if (own->flags == 1 && (u8)(peer->peerFlags - 1) <= 2 - 1) {
        if (peer->active == (gSerialBlockNumber & 0x7f)) {
          own->active = 0;
          switch (peer->peerFlags) {
          case 1:
            Dma_Set(peer->payload, (void *)dest, DMA_COPY_WORDS(5), REG_DMA3);
            *receive += 20;
            gSerialReceivedSize += 20;
            own->status = (own->status + 1) | 0x80;
            break;
          case 2:
            Dma_Set(peer->payload, (void *)dest, DMA_COPY_WORDS(5), REG_DMA3);
            gSerialReceivedSize += 20;
            own->flags = 2;
            own->status = 0;
            own->active = one.v;
            break;
          }
          gSerialBlockNumber = (gSerialBlockNumber + 1) & 0x7f;
        } else if (gSerialBlockNumber & 0x80) {
          if (own->active & 0x80) {
            own->active = one.v;
          } else if (own->active == 1) {
            own->active = 0;
            gSerialBlockNumber &= 0x7f;
          }
        } else {
          own->active = gSerialBlockNumber | 0x80;
          gSerialBlockNumber |= 0x80;
        }
      } else {
        own->active = 0;
      }
    }
  }
  {
    s32 *send = &gSerialSendSource;
    s32 src = *send;
    if (src != 0) {
      if (peer->flags == 1) {
        if (peer->active & 0x80) {
          gap = (gSerialBlockNumber - peer->active) & 0x7f;
          *send = src - gap * 20;
          gSerialSendSize += gap * 20;
          gSerialBlockNumber -= gap;
          gSerialBlockNumber &= 0x7f;
        }
        if (gSerialSendSize != 0) {
          Dma_Set((void *)*send, own->payload, DMA_COPY_WORDS(5), REG_DMA3);
          gSerialSendSize += -20;
          if (gSerialSendSize != 0) {
            own->peerFlags = 1;
            own->active = gSerialBlockNumber & 0x7f;
            *send += 20;
          } else {
            own->peerFlags = 2;
            own->active = gSerialBlockNumber & 0x7f;
            *send += 20;
          }
          gSerialBlockNumber = (gSerialBlockNumber + 1) & 0x7f;
        }
      }
      if (own->peerFlags == 2 && peer->flags == 2) {
        gSerialSendSource = 0;
        own->peerFlags = 0;
        own->active = 1;
      }
    }
  }
  if (own->flags == 2) {
    if (peer->peerFlags != 2) {
      gSerialReceiveDest = 0;
      own->flags = 0;
    }
  } else {
    own->flags = 0;
    if (gSerialReceiveDest != 0)
      own->flags = 1;
  }
}
