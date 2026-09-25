/* Draft, not exact (2026-09-25): 560 of 572 bytes, 268 differing halfwords.
   SerialRuntime_StepBlockTransfer (called from the VBlank handler after the
   packet exchange): moves one 20-byte block of a bulk link transfer per frame.
   Receive side B copies the peer's block into the destination when the peer's
   sequence matches ours (kind 1 more, kind 2 last); send side A copies the next
   source block into our packet and rewinds by the gap the peer reports.
   What lined up: statement order, the switch on the peer's kind, the pooled 1
   (Value_00000001, held in r8 from the top) for the sequence resets.
   Remaining: the ROM keeps the peer pointer's idx*24 folded (ip + 4 for the
   DMA source) where GCC here keeps idx*24 live and pools 0x02002024; it keeps
   &SERIAL_ACTIVE_B/A in r7 and 0x7f in lr (here swapped); and its first literal
   pool sits after the own->flags test (0x48), which lengthens the early
   branches into beq/b pairs. A reassigned address local and pointer-plus-index
   spellings did not stop the fold. */
#include "DMA.H"
#include "SERIAL_RUNTIME.H"

/* Moves one 20-byte block of a bulk transfer per frame through the link
   packets: block B receives into the destination pointer when the peer's
   sequence matches ours, block A sends from the source pointer and rewinds
   when the peer reports a gap. */

struct SerialBlock {
    u8 sequence;
    u8 status;
    u8 flags;
    u8 peer_flags;
    u32 data[5];
};

#define SERIAL_BLOCKS ((struct SerialBlock *)0x02002020)
#define SERIAL_OWN_BLOCK ((struct SerialBlock *)0x02002220)

extern u8 Value_00000001;
extern volatile u16 gLinkStatus;

void SerialRuntime_StepBlockTransfer(void)
{
    struct SerialBlock *peer;
    struct SerialBlock *own;
    s32 dest;
    s32 src;
    u32 gap;
    u8 kind;
    u32 one;

    peer = &SERIAL_BLOCKS[~((REG_SIOCNT << 26) >> 30) & 1];
    one = (u32)&Value_00000001;
    own = SERIAL_OWN_BLOCK;
    if ((gLinkStatus & 3) != 3)
        return;

    dest = SERIAL_ACTIVE_B;
    if (dest != 0) {
        if (own->flags == 1 && (u8)(peer->peer_flags - 1) <= 1) {
            if (peer->sequence == (SERIAL_RESULT & 0x7f)) {
                own->sequence = 0;
                kind = peer->peer_flags;
                switch (kind) {
                case 1:
                    Dma_Set(peer->data, (void *)dest, 0x84000005, (volatile u32 *)0x040000d4);
                    SERIAL_ACTIVE_B += 20;
                    SERIAL_VALUE_B += 20;
                    own->status = (own->status + 1) | 0x80;
                    break;
                case 2:
                    Dma_Set(peer->data, (void *)dest, 0x84000005, (volatile u32 *)0x040000d4);
                    SERIAL_VALUE_B += 20;
                    own->flags = 2;
                    own->status = 0;
                    own->sequence = one;
                    break;
                }
                SERIAL_RESULT = (SERIAL_RESULT + 1) & 0x7f;
            } else if (SERIAL_RESULT & 0x80) {
                if (own->sequence & 0x80) {
                    own->sequence = one;
                } else if (own->sequence == 1) {
                    own->sequence = 0;
                    SERIAL_RESULT &= 0x7f;
                }
            } else {
                own->sequence = SERIAL_RESULT | 0x80;
                SERIAL_RESULT |= 0x80;
            }
        } else {
            own->sequence = 0;
        }
    }

    src = SERIAL_ACTIVE_A;
    if (src != 0) {
        kind = peer->flags;
        if (kind == 1) {
            if (peer->sequence & 0x80) {
                gap = (SERIAL_RESULT - peer->sequence) & 0x7f;
                SERIAL_ACTIVE_A = src - gap * 20;
                SERIAL_VALUE_A += gap * 20;
                SERIAL_RESULT -= gap;
                SERIAL_RESULT &= 0x7f;
            }
            if (SERIAL_VALUE_A != 0) {
                Dma_Set((void *)SERIAL_ACTIVE_A, own->data, 0x84000005, (volatile u32 *)0x040000d4);
                SERIAL_VALUE_A += (u16)-20;
                if (SERIAL_VALUE_A != 0)
                    own->peer_flags = 1;
                else
                    own->peer_flags = 2;
                own->sequence = SERIAL_RESULT & 0x7f;
                SERIAL_ACTIVE_A += 20;
                SERIAL_RESULT = (SERIAL_RESULT + 1) & 0x7f;
            }
        }
        if (own->peer_flags == 2 && peer->flags == 2) {
            SERIAL_ACTIVE_A = 0;
            own->peer_flags = 0;
            own->sequence = 1;
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
