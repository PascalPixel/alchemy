// Near miss, differ 2 of 268 (tla-en, 2026-10-30): one swap in the rewind
// clear (own->active = 0, then gSerialBlockNumber &= 0x7f). The native copies
// the 127 mask from lr before it reads the block number byte; this compiler
// reads the byte first. That read opens a read-modify-write, so
// -mthumb-slow-loads lets it issue straight after the own->active store, and
// its slower load rank puts it ahead of the copy; the ROM waits a cycle there.
// Scheduler cost model: the rewind clear as x = x & 0x7f, as 0x7f & x or
// through a u8 temporary stays at differ 2, its two statements swapped give
// differ 44, and permute (4 minutes) found nothing better.
// The Broken Seal's Serial_StepBlockTransfer
// (games/tbs/src/system/system/serial.c) on this game's names. The one-pass
// block around the send size's -20 that the 2026-10-24 draft needed changes
// nothing under -mthumb-slow-loads and is gone. With it, the 2026-10-26
// device search found no single empty asm, one-pass block, wrapping block or
// swap of neighbouring statements that brings it closer.
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
