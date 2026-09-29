/* 2026-09-29: eight minutes of permutation found 1640; the cleaned natural
 * form kept here scores 1660 (26 register-only, 8 operand, 9 reordered, 1
 * inserted, 7 deleted), from 2385 (alchemy permute --function
 * Func_0800615c). What moved it: the channel flags are read into a local
 * before channel_state[1] and the runtime flags are cleared, and stored
 * into channel_state[0] after; the destination is set before the current
 * mask is cleared, and the locals are declared in a different order. */
/* Not-yet-C, complete 228-byte receive collector and pool.
 * Dma_Set recovers the stmia block; mask update follows that transfer.
 * Walking the checksum pointer and reloading the packet from pp for its
 * inversion avoids retaining another saved register. An s32 inline IME
 * setter selects movs 1 rather than a short-range halfword pool.
 * Candidate 224 bytes, equal topology, 59 aligned halfword edits. Remaining
 * table-base reuse, register lifetimes and first pool differ. Three structural
 * hypotheses stopped, no adoption or byte credit.
 * 2026-09-27: reusing PREPARE_SEND_PACKET.C's volatile runtime pointer for
 * every field and volatile word channel-flag accesses gives 224/228 bytes,
 * 92 differing halfwords, 65 aligned edits (baseline 93/59). It adds reads
 * before byte member stores, keeps the runtime in ip throughout, and still
 * coalesces the initial packet-table reload. Whole-runtime volatility does
 * not describe this collector's access boundaries; baseline retained. */
#include "serial_runtime_family.h"
#include "DMA.H"

static __inline__ void Serial_SetIme(s32 value)
{
    REG_IME = value;
}

u8 Func_0800615c(void *payload)
{
    struct SerialRuntime *state;
    u32 channel_state[2];
    u16 **pp;
    u16 **ready_ptr;
    s32 channel;
    u8 *dst;
    u32 flags;

    Serial_SetIme(0);
    state = SERIAL_RUNTIME;
    ready_ptr = state->ready_buffer;
    channel = 3;
    do {
        u16 *swap = ready_ptr[4];

        ready_ptr[4] = *ready_ptr;
        *ready_ptr++ = swap;
    } while (--channel >= 0);
    flags = *(u32 *)SERIAL_RUNTIME->channel_flags;
    channel_state[1] = 0;
    *(u32 *)SERIAL_RUNTIME->channel_flags = 0;
    channel_state[0] = flags;
    Serial_SetIme(1);
    dst = (u8 *)payload;
    SERIAL_RUNTIME->current_mask = 0;
    pp = state->pending_buffer;
    for (channel = 0; channel <= 1; channel++) {
        s32 checksum;
        u16 *packet;
        u32 index;

        packet = *pp;
        checksum = 0;
        for (index = 0; index <= 13; index++)
            checksum += *packet++;
        if (((u8 *)channel_state)[channel] == 1 && (s16)checksum == -1) {
            Dma_Set(*pp + 2, dst, 0x84000006, (volatile u32 *)DMA3);
            SERIAL_RUNTIME->current_mask |= 1 << channel;
        }
        if ((s16)checksum == -1)
            (*pp)[1] = ~(*pp)[1];
        pp++;
        dst += 24;
    }
    state->received_mask |= state->current_mask;
    return state->current_mask;
}
