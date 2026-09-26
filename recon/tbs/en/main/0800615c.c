/* Not-yet-C, complete 228-byte receive collector and pool.
 * Dma_Set recovers the stmia block; mask update follows that transfer.
 * Walking the checksum pointer and reloading the packet from pp for its
 * inversion avoids retaining another saved register. An s32 inline IME
 * setter selects movs 1 rather than a short-range halfword pool.
 * Candidate 224 bytes, equal topology, 59 aligned halfword edits. Remaining
 * table-base reuse, register lifetimes and first pool differ. Three structural
 * hypotheses stopped, no adoption or byte credit. */
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
    s32 channel;
    u16 **ready_ptr;
    u16 **pp;
    u8 *dst;

    channel_state[1] = 0;
    Serial_SetIme(0);
    state = SERIAL_RUNTIME;
    ready_ptr = state->ready_buffer;
    channel = 3;
    do {
        u16 *swap = ready_ptr[4];

        ready_ptr[4] = *ready_ptr;
        *ready_ptr++ = swap;
    } while (--channel >= 0);
    channel_state[0] = *(u32 *)SERIAL_RUNTIME->channel_flags;
    *(u32 *)SERIAL_RUNTIME->channel_flags = 0;
    Serial_SetIme(1);
    SERIAL_RUNTIME->current_mask = 0;

    pp = state->pending_buffer;
    dst = (u8 *)payload;
    for (channel = 0; channel <= 1; channel++) {
        u16 *packet;
        u32 index;
        s32 checksum;

        packet = *pp;
        checksum = 0;
        for (index = 0; index <= 13; index++)
            checksum += *packet++;
        if (((u8 *)&channel_state[0])[channel] == 1 && (s16)checksum == -1) {
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
