#include "types.h"
#include "scene.h"

/* audio/track/reset_slot_buckets.c */
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

/* audio/track/insert_slot_node.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern void *gOv;

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

    base = (s32)gOv;
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

/* audio/track/remove_slot_node.c */
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

/* audio/track/consume_slot_bytes.c */
struct State_080f7e60 {
    u8 padding0000[0x3404];
    s32 bucket_by_slot[0x400];
    u8 padding4404[0x34];
    u32 input_cursor;
    s32 padding443c;
    u32 input_limit;
};

extern struct State_080f7e60 *gOv;

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
            struct State_080f7e60 *state;
            u32 read_offset;
            u32 next_offset;
            u8 value;

            AudioTrack_RemoveSlotNode(removal & mask);
            state = gOv;
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
        struct State_080f7e60 **root = &gOv;
        s32 empty = -1;

        do {
            u32 slot = (base + (u32)current) & mask;
            struct State_080f7e60 *state;

            AudioTrack_RemoveSlotNode(slot);
            state = *root;
            current++;
            state->bucket_by_slot[slot] = empty;
        } while (current < limit);
    }
}

/* audio/track/copy_buffered_bytes.c */
extern u8 gVal[];

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
        u32 displacement = (u32)gVal;
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

/* audio/music/player/step_volume_and_pitch_toward_targets.c */
extern u8 gOv;
extern u16 gOv2;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;
extern u16 gOv6;
extern u16 gOv7;
extern u8 gOv8[];
extern u8 gOv9[];

void Audio_Run(void)
{
    s32 delta;

    if (gOv != 0) {
        if (gOv == 1) {
            if (gOv8[4] == 0) {
                gOv = 0;
                gOv6 = 0x100;
            }
        } else {
            gOv -= 1;
        }
    }
    if ((s16)gOv6 != (s16)gOv2) {
        delta = (s16)gOv6 - (s16)gOv2;
        if (delta > 0) {
            gOv2 = gOv2 + gOv4;
        } else {
            gOv2 = gOv2 - gOv4;
        }
        if ((((s16)gOv6 - (s16)gOv2) ^ delta) < 0) {
            gOv2 = gOv6;
        }
        Audio_Place(gOv9, 255, gOv2);
    }
    if ((s16)gOv5 != (s16)gOv7) {
        delta = (s16)gOv5 - (s16)gOv7;
        if (delta > 0) {
            gOv7 = gOv7 + gOv3;
        } else {
            gOv7 = gOv7 - gOv3;
        }
        if ((((s16)gOv5 - (s16)gOv7) ^ delta) < 0) {
            gOv7 = gOv5;
        }
        Audio_Apply(gOv9, gOv7);
        Audio_unk2(gOv9, 255, (s16)((s16)gOv7 * 12 - 3072));
    }
    Audio_unk2_2();
}
