// Draft, differ 4 of 268 (2026-10-24): The Broken Seal's
// Serial_StepBlockTransfer (games/tbs/src/runtime/runtime/serial.c) on this
// game's names, the same source but for one one-pass block. As ☀️ wrote it,
// differ 6; the one-pass block around the send size's -20 loads the size
// before it builds 0xffec (permuter). Left: twice the 127 mask, held in lr,
// is copied there one slot off: the native reads the sequence byte before
// mov lr, r2 in the receive test, and copies lr before reading it again in
// the rewind clear. No single empty asm, one-pass block or statement swap
// improves it.
#include "types.h"
#include "io_reg.h"
#include "serial_runtime.h"

struct SerialHalfword {
  u16 v;
};

void SerialRuntime_StepBlockTransfer(void) {
  struct SerialTransferState *peer;
  u32 id;
  struct SerialTransferState *own;
  u32 gap;
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
        if (peer->active == (gSerialBlockSequence & 0x7f)) {
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
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
        } else if (gSerialBlockSequence & 0x80) {
          if (own->active & 0x80) {
            own->active = one.v;
          } else if (own->active == 1) {
            own->active = 0;
            gSerialBlockSequence &= 0x7f;
          }
        } else {
          own->active = gSerialBlockSequence | 0x80;
          gSerialBlockSequence |= 0x80;
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
          gap = (gSerialBlockSequence - peer->active) & 0x7f;
          *send = src - gap * 20;
          gSerialSendSize += gap * 20;
          gSerialBlockSequence -= gap;
          gSerialBlockSequence &= 0x7f;
        }
        if (gSerialSendSize != 0) {
          Dma_Set((void *)*send, own->payload, DMA_COPY_WORDS(5), REG_DMA3);
          do {
            gSerialSendSize += -20;
          } while (0);
          if (gSerialSendSize != 0) {
            own->peerFlags = 1;
            own->active = gSerialBlockSequence & 0x7f;
            *send += 20;
          } else {
            own->peerFlags = 2;
            own->active = gSerialBlockSequence & 0x7f;
            *send += 20;
          }
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
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
