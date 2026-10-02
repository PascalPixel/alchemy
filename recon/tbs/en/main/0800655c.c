/* Draft, not exact: 572 of 572 bytes. Moves one 20-byte block of a bulk link
   transfer per frame. The pooled 1 in r8 and the early literal pool come from
   a one kept in a one-halfword record (a HImode value); the 0x7f, the zero and
   the peer flag in lr come from writing the tests without a kind variable.
   Remaining: the ROM keeps the peer payload pointer in ip and the transfer
   cursor address in r7 in both halves; here the two are swapped (the peer
   pointer has 11 uses against the cursor pointer 10 over the same length). */
#include "DMA.H"
#include "SERIAL_RUNTIME.H"

/* Moves one 20-byte block of a bulk transfer per frame through the link
   packets: block B receives into the destination pointer when the peer's
   sequence matches ours, block A sends from the source pointer and rewinds
   when the peer reports a gap. */


extern volatile u16 gLinkStatus;

void SerialRuntime_StepBlockTransfer(void)
{
    struct SerialTransferState *peer;
    struct SerialTransferState *own;
    volatile s32 *slot;
    s32 dest;
    s32 src;
    u32 gap;
    u32 id;
    struct { u16 v; } one;

    one.v = 1;
    id = ~((REG_SIOCNT << 26) >> 30) & 1;
    peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
    own = &gSerialTransfer;
    if ((gLinkStatus & 3) != 3)
        return;

    slot = &SERIAL_ACTIVE_B;
    dest = *slot;
    if (dest != 0) {
        if (own->flags == 1 && (u8)(peer->peer_flags - 1) <= 1) {
            if (peer->active == (gSerialBlockSequence & 0x7f)) {
                own->active = 0;
                switch (peer->peer_flags) {
                case 1:
                    Dma_Set(peer->reserved, (void *)dest, 0x84000005, (volatile u32 *)0x040000d4);
                    *slot += 20;
                    SERIAL_VALUE_B += 20;
                    own->status = (own->status + 1) | 0x80;
                    break;
                case 2:
                    Dma_Set(peer->reserved, (void *)dest, 0x84000005, (volatile u32 *)0x040000d4);
                    SERIAL_VALUE_B += 20;
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

    slot = &SERIAL_ACTIVE_A;
    src = *slot;
    if (src != 0) {
        if (peer->flags == 1) {
            if (peer->active & 0x80) {
                gap = (gSerialBlockSequence - peer->active) & 0x7f;
                *slot = src - gap * 20;
                SERIAL_VALUE_A += gap * 20;
                gSerialBlockSequence -= gap;
                gSerialBlockSequence &= 0x7f;
            }
            if (SERIAL_VALUE_A != 0) {
                Dma_Set((void *)*slot, own->reserved, 0x84000005, (volatile u32 *)0x040000d4);
                SERIAL_VALUE_A += (u16)-20;
                if (SERIAL_VALUE_A != 0)
                    own->peer_flags = 1;
                else
                    own->peer_flags = 2;
                own->active = gSerialBlockSequence & 0x7f;
                *slot += 20;
                gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
            }
        }
        if (own->peer_flags == 2 && peer->flags == 2) {
            SERIAL_ACTIVE_A = 0;
            own->peer_flags = 0;
            own->active = 1;
        }
    }

    if (own->flags == 2) {
        if (peer->peer_flags != 2) {
            SERIAL_ACTIVE_B = 0;
            own->flags = 0;
        }
    } else {
        own->flags = 0;
        if (SERIAL_ACTIVE_B != 0)
            own->flags = 1;
    }
}
