#include "types.h"

struct TileFadeState_08090658 {
    u8 unknown_000[0x508];
    u8 packed_tiles[0x22];
    u16 fade_position;
    u8 unknown_52c[0x0d];
    u8 buffer_index;
    s8 fade_start;
    s8 fade_end;
    s8 fade_frames;
    s8 fade_progress;
};

struct QueuedTransfer_08090658 {
    const void *source;
    void *destination;
    u32 control;
};

struct TransferQueue_08090658 {
    u16 count;
    u16 padding;
    struct QueuedTransfer_08090658 entries[32];
};

void Func_08004278(void (*callback)(void));
s32 Func_080072f0(s32 numerator, s32 denominator, s32 range, s32 workspace);

/*
 * Advance the two-frame tile fade, update the packed four-bit tile entries,
 * and queue their transfer to the first character block.
 */
void Func_08090658(void)
{
    struct TileFadeState_08090658 *state =
        *(struct TileFadeState_08090658 **)0x03001ecc;
    const u8 *tile_order = (const u8 *)0x0809e8ee;
    volatile struct TransferQueue_08090658 *queue =
        (volatile struct TransferQueue_08090658 *)0x02002090;
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    s32 phase;
    u8 nibble;
    s32 index;

    if (state->fade_frames != 0) {
        if (state->fade_progress >= state->fade_frames) {
            volatile u16 *dma_control = (volatile u16 *)0x040000ba;

            state->fade_frames = 0;
            Func_08004278(Func_08090658);
            *dma_control &= 0xc5ff;
            *dma_control &= 0x7fff;
            return;
        }

        {
            s32 range = state->fade_end - state->fade_start;

            state->fade_progress++;
            state->fade_position = state->fade_start +
                Func_080072f0(state->fade_progress * range,
                              state->fade_frames, range, 0x03000380);
        }
    }

    phase = state->fade_position - 1;
    state->buffer_index ^= 1;
    nibble = (phase & 0x20) != 0 ? 0x0f : 0;

    for (index = 0; index < 2; index++) {
        u8 packed_index = tile_order[((phase & 0x1f) * 2 + index) & 0x3f];
        u8 *entry = &state->packed_tiles[packed_index >> 1];

        if ((packed_index & 1) != 0) {
            *entry = (*entry & 0x0f) | (nibble << 4);
        } else {
            *entry = (*entry & 0xf0) | nibble;
        }
    }

    {
        u16 saved_interrupt_master = *interrupt_master;
        u16 queue_index;

        *interrupt_master = (u16)(u32)interrupt_master;
        queue_index = queue->count;
        if (queue_index <= 31) {
            volatile struct QueuedTransfer_08090658 *entry =
                &queue->entries[queue_index];

            queue->count = queue_index + 1;
            entry->source = state->packed_tiles;
            entry->destination = (void *)0x06000000;
            entry->control = 0x84000008;
        }
        *interrupt_master = saved_interrupt_master;
    }
}
