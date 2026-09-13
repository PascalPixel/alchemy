#include "types.h"

struct AudioTrackSlotWork {
    u8 unknown0000[0x3404];
    s32 bucket_by_slot[0x400];
    u8 unknown4404[0x34];
    u32 input_cursor;
    s32 unknown443c;
    u32 input_limit;
};

extern struct AudioTrackSlotWork *Data_02004c00;

void AudioTrack_ResetSlotBuckets(void)
{
    s32 index;
    s32 limit;
    s32 zero;
    u8 *record;
    s32 *slot;

    limit = 0x3FF;
    index = 0;
    zero = 0;
    record = *(u8 **)0x02004C00 + 4;
    do {
        *(s32 *)(record + 4) = index;
        index++;
        *(s32 *)record = zero;
        record += 12;
    } while (index <= limit);
    slot = (s32 *)(*(u8 **)0x02004C00 + 0x3000);
    {
        s32 zero2 = 0;
        for (index = 0xFF; index >= 0; index--) {
            *slot++ = zero2;
        }
    }
}

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))


void AudioTrack_InsertSlotNode(s32 index)
{
    s32 base;
    s32 node_off;
    s32 tbl_off;
    s32 bucket;
    s32 bucket_off;
    s32 link_off;
    void **node;
    void *next;

    base = (s32)Data_02004c00;
    node_off = index * 12;
    tbl_off = index * 4 + 0x3404;
    bucket = *(s32 *)(base + tbl_off) * 4;
    link_off = node_off + 4;
    *(s32 *)(base + link_off) = base + bucket + 0x3000;
    bucket_off = bucket + 0x3000;
    *(s32 *)(base + node_off) = *(s32 *)(base + bucket_off);
    node = (void **)(base + node_off);
    *(void **)(base + bucket_off) = node;
    next = *node;
    if (next != 0)
        FIELD(next, void *, 4) = node;
}

void AudioTrack_RemoveSlotNode(s32 slot)
{
    s32 next_node;
    s32 track_table;
    s32 slot_offset;
    s32 next_link_offset;
    void *previous_node;

    track_table = *(s32 *)0x02004C00;
    slot_offset = slot * 12;
    next_link_offset = slot_offset + 4;
    next_node = *(s32 *)(track_table + next_link_offset);
    if (next_node != 0) {
        previous_node = *(void **)(track_table + slot_offset);
        if (previous_node != 0) {
            *(s32 *)((u8 *)previous_node + 4) = next_node;
        }
        **(s32 **)(track_table + next_link_offset) =
            *(s32 *)(track_table + slot_offset);
    }
}




void AudioTrack_InsertSlotNode(s32 index);
void AudioTrack_RemoveSlotNode(s32 index);

void AudioTrack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input)
{
    s32 limit = count;
    s32 current = 0;
    u32 base = (u32)start;

    if (current < limit) {
        u32 mask = 0x3ff;
        u32 removal = base + 0x124;

        do {
            u32 slot = base + (u32)current;
            struct AudioTrackSlotWork *state;
            u32 read_offset;
            u32 next_offset;
            u8 value;

            AudioTrack_RemoveSlotNode(removal & mask);
            state = Data_02004c00;
            read_offset = state->input_cursor;
            value = input[read_offset];
            next_offset = read_offset + 1;
            state->input_cursor = next_offset;
            if (next_offset == state->input_limit) {
                state->bucket_by_slot[slot & mask] = -1;
                break;
            }

            state->bucket_by_slot[slot & mask] = value;
            current++;
            AudioTrack_InsertSlotNode(slot & mask);
            removal++;
        } while (current < limit);
    }

    current++;
    if (current < limit) {
        u32 mask = 0x3ff;
        struct AudioTrackSlotWork **root = &Data_02004c00;
        s32 empty = -1;

        do {
            u32 slot = (base + (u32)current) & mask;
            struct AudioTrackSlotWork *state;

            AudioTrack_RemoveSlotNode(slot);
            state = *root;
            current++;
            state->bucket_by_slot[slot] = empty;
        } while (current < limit);
    }
}

extern u8 Data_0000443c[];

void AudioTrack_CopyBufferedBytes(u8 *destination)
{
    u32 cnt_off = 0x4404;
    u8 **base_p = (u8 **)0x02004c00;
    u8 *base = *base_p;
    u32 index = 0;
    u32 *cnt_p = (u32 *)(base + cnt_off);
    u32 saved_off = cnt_off;
    u32 count = *cnt_p;

    if (count != 0) {
        u32 displacement = (u32)Data_0000443c;
        u32 *offset = (u32 *)(base + displacement);
        u32 *cnt_p2;
        u8 *source;
        displacement -= 52;
        cnt_p = (u32 *)displacement;
        cnt_p2 = (u32 *)(base + saved_off);
        source = base + (u32)cnt_p;
        do {
            destination[*offset] = *source;
            (*offset)++;
            index = index + 1;
            source = source + 1;
        } while (index != *cnt_p2);
    }
}
