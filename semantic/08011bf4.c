#include "types.h"

struct Dma_08011bf4 {
    const void *source;
    void *destination;
    u32 control;
};

struct AnimatedTransfer_08011bf4 {
    void *destination;
    s16 phase;
    u16 countdown;
    u16 interval;
    s16 length;
    u16 values[16];
};

/*
 * Advance the active cyclic halfword transfers. When a record's countdown
 * expires, rotate its value table by the current phase into a stack buffer,
 * DMA that buffer to the destination, advance the phase, and reload the
 * countdown.
 */
void Func_08011bf4(void) {
    u8 *state;
    struct AnimatedTransfer_08011bf4 *transfer;
    struct Dma_08011bf4 *dma3;
    u16 rotated[16];
    u32 active_count;
    u32 transfer_index;

    state = *(u8 **)0x03001EC0;
    active_count = *(u16 *)(state + 0xB0) & 3;
    transfer = (struct AnimatedTransfer_08011bf4 *)state;
    dma3 = (struct Dma_08011bf4 *)0x040000D4;

    for (transfer_index = 0;
         transfer_index < active_count;
         transfer_index++, transfer++) {
        u16 length;
        u16 source_index;
        u16 destination_index;

        if (transfer->countdown != 0) {
            transfer->countdown--;
            continue;
        }

        length = (u16)transfer->length;
        source_index = 0;
        destination_index = (u8)(length - (u16)transfer->phase);
        while (destination_index < length) {
            rotated[destination_index++] = transfer->values[source_index++];
        }
        destination_index = 0;
        while (destination_index < length - (u16)transfer->phase) {
            rotated[destination_index++] = transfer->values[source_index++];
        }

        dma3->source = rotated;
        dma3->destination = transfer->destination;
        dma3->control = 0x80000000 | length;

        transfer->phase++;
        if ((u16)transfer->phase >= length) {
            transfer->phase = 0;
        }
        transfer->countdown = transfer->interval;
    }
}
