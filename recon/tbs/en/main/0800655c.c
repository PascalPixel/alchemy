/* Draft, not exact: 548 of 572 bytes with the canonical TBS flags.
 * 2026-10-03: fresh baseline score 2375 (59 register-only, 8 operand,
 * 10 reordered, 1 inserted, 12 deleted). The earlier 572-byte result in
 * this header is not reproduced by the current source and toolchain.
 * Trial: separate receive/send cursor scopes emit the identical 548-byte
 * object and score; retained to make the two transfer stages explicit.
 * Remaining: the peer packet stays in r7 instead of ip, cursor addresses
 * are reloaded, and branch/store topology also differs. Cursor scoping
 * alone does not resolve the mismatch; this experiment is closed.
 */
#include "DMA.H"
#include "SERIAL_RUNTIME.H"

/* VBlank receives first, then sends, using one shared sequence and reply
   byte. Each block carries 20 bytes; the high sequence bit requests a
   resend, and a final-block exchange releases the active cursors. */

extern volatile u16 gLinkStatus;

void SerialRuntime_StepBlockTransfer(void)
{
    struct SerialTransferState *peer;
    struct SerialTransferState *own;
    u32 gap;
    u32 id;
    /* FAKEMATCH: the halfword record retains the measured pooled-one
       construction; it is not protocol state. */
    struct { u16 v; } one;

    one.v = 1;
    id = ~((REG_SIOCNT << 26) >> 30) & 1;
    peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
    own = &gSerialTransfer;
    if ((gLinkStatus & 3) != 3)
        return;

    /* flags: 1 requests blocks; 2 acknowledges the final block. */
    {
        volatile s32 *receive = &gSerialReceiveDest;
        s32 dest = *receive;

        if (dest != 0) {
            if (own->flags == 1 && (u8)(peer->peer_flags - 1) <= 1) {
                if (peer->active == (gSerialBlockSequence & 0x7f)) {
                    own->active = 0;
                    switch (peer->peer_flags) {
                    case 1:
                        Dma_Set(peer->payload, (void *)dest, 0x84000005, (volatile u32 *)0x040000d4);
                        *receive += 20;
                        gSerialReceivedSize += 20;
                        own->status = (own->status + 1) | 0x80;
                        break;
                    case 2:
                        Dma_Set(peer->payload, (void *)dest, 0x84000005, (volatile u32 *)0x040000d4);
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

    /* peer_flags: 1 carries more data; 2 carries the final block. */
    {
        volatile s32 *send = &gSerialSendSource;
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
                    Dma_Set((void *)*send, own->payload, 0x84000005, (volatile u32 *)0x040000d4);
                    gSerialSendSize += (u16)-20;
                    if (gSerialSendSize != 0)
                        own->peer_flags = 1;
                    else
                        own->peer_flags = 2;
                    own->active = gSerialBlockSequence & 0x7f;
                    *send += 20;
                    gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
                }
            }
            if (own->peer_flags == 2 && peer->flags == 2) {
                gSerialSendSource = 0;
                own->peer_flags = 0;
                own->active = 1;
            }
        }
    }

    if (own->flags == 2) {
        if (peer->peer_flags != 2) {
            gSerialReceiveDest = 0;
            own->flags = 0;
        }
    } else {
        own->flags = 0;
        if (gSerialReceiveDest != 0)
            own->flags = 1;
    }
}
