#include "TYPES.H"
extern u8 Flash_Handler3[];

/* One window slot's link in the chain of slots holding the same byte value. */
struct AudioSlotNode {
    struct AudioSlotNode *prev;
    struct AudioSlotNode **back;
    s32 slot;
};

/* The packer's work block: a 1024-slot ring of input bytes, each slot chained
   by byte value, and the token group being assembled. */
struct AudioTrackSlotWork {
    struct AudioSlotNode nodes[0x400];
    struct AudioSlotNode *bucket[0x100];
    s32 flag_mask;
    s32 bucket_by_slot[0x400];
    s32 out_cnt;
    u8 out_buf[0x24];
    s32 match_ofs;
    s32 match_len;
    s32 pos;
    u32 input_cursor;
    s32 out_total;
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
    record = *(u8 **)Flash_Handler3 + 4;
    do {
        *(s32 *)(record + 4) = index;
        index++;
        *(s32 *)record = zero;
        record += 12;
    } while (index <= limit);
    slot = (s32 *)(*(u8 **)Flash_Handler3 + 0x3000);
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

    track_table = *(s32 *)Flash_Handler3;
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

void AudioTrack_CopyBufferedBytes(u8 *destination)
{
    u32 cnt_off = 0x4404;
    u8 **base_p = (u8 **)Flash_Handler3;
    u8 *base = *base_p;
    u32 index = 0;
    u32 *cnt_p = (u32 *)(base + cnt_off);
    u32 saved_off = cnt_off;
    u32 count = *cnt_p;

    if (count != 0) {
        u32 displacement = 0x443c;
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

extern u8 gMapCellBuffer[];

/*
 * Sliding-window packer over the slot work block above.
 *
 * Output: a control byte whose set bits mark coded pairs, then per token one
 * literal byte or a big-endian 16-bit code (distance bits 8..11 in the top
 * nibble, length - 1 in the next, distance bits 0..7 below), with a third
 * byte holding length - 17 when the length nibble is 0. A zero code ends it.
 */

#define WINDOW_MASK 0x3ff
#define MATCH_MAX 272
#define DIST_MAX 63
#define PRIME_COUNT 672

/* Longest run at `start` that repeats an earlier slot on the same byte-value
   chain, left in match_len and match_ofs; a length of 1 means no match. */
static __inline__ void AudioTrack_FindWindowMatch(s32 start)
{
    struct AudioSlotNode *node;
    s32 len;

    Data_02004c00->match_len = 1;
    if (Data_02004c00->bucket_by_slot[start] == -1) {
        return;
    }
    node = Data_02004c00->bucket[Data_02004c00->bucket_by_slot[start]];
    if (start + MATCH_MAX > WINDOW_MASK) {
        /* The run can wrap the ring, so both sides are masked. */
        while (node != NULL) {
            s32 dist = (start - node->slot) & WINDOW_MASK;

            if (dist > 0 && dist <= DIST_MAX) {
                for (len = 1; len < MATCH_MAX; len++) {
                    if (Data_02004c00->bucket_by_slot[(start + len) & WINDOW_MASK]
                        != Data_02004c00->bucket_by_slot[(node->slot + len) & WINDOW_MASK]) {
                        break;
                    }
                }
                if (Data_02004c00->match_len < len) {
                    Data_02004c00->match_ofs = dist;
                    Data_02004c00->match_len = len;
                    if (len == MATCH_MAX) {
                        return;
                    }
                }
            }
            node = node->prev;
        }
    } else {
        while (node != NULL) {
            s32 dist = (start - node->slot) & WINDOW_MASK;

            if (dist > 0 && dist <= DIST_MAX) {
                for (len = 1; len < MATCH_MAX; len++) {
                    if (Data_02004c00->bucket_by_slot[start + len]
                        != Data_02004c00->bucket_by_slot[(node->slot + len) & WINDOW_MASK]) {
                        break;
                    }
                }
                if (Data_02004c00->match_len < len) {
                    Data_02004c00->match_ofs = dist;
                    Data_02004c00->match_len = len;
                    if (len == MATCH_MAX) {
                        return;
                    }
                }
            }
            node = node->prev;
        }
    }
}

/* Pack `size` bytes of `input` into `dst`; returns the packed byte count. */
s32 AudioTrack_PackStream(const u8 *input, u8 *dst, s32 size)
{
    s32 defer;
    s32 len;
    s32 save_ofs;
    s32 save_len;
    s32 reach[2];
    s16 code;

    defer = 0;
    Data_02004c00 = (struct AudioTrackSlotWork *)gMapCellBuffer;
    AudioTrack_ResetSlotBuckets();
    Data_02004c00->input_limit = size;
    Data_02004c00->pos = 0;
    Data_02004c00->input_cursor = 0;
    Data_02004c00->out_total = 0;
    Data_02004c00->flag_mask = 0x80;
    Data_02004c00->out_buf[0] = 0;
    Data_02004c00->out_cnt = 1;
    AudioTrack_ConsumeSlotBytes(0, PRIME_COUNT, input);

    while (Data_02004c00->bucket_by_slot[Data_02004c00->pos] != -1) {
        AudioTrack_FindWindowMatch(Data_02004c00->pos);
        if (defer == 0) {
            len = Data_02004c00->match_len;
            if (Data_02004c00->match_len > 1) {
                /* Lazy evaluation: take one literal now when the match one
                   slot later reaches at least as far as this match followed
                   by the best one after it. */
                save_ofs = Data_02004c00->match_ofs;
                save_len = len;
                AudioTrack_FindWindowMatch((Data_02004c00->pos + 1) & WINDOW_MASK);
                if (Data_02004c00->match_len > 2) {
                    reach[0] = Data_02004c00->match_len + 1;
                    AudioTrack_FindWindowMatch((Data_02004c00->pos + len) & WINDOW_MASK);
                    reach[1] = Data_02004c00->match_len + len;
                    if (reach[0] >= reach[1]) {
                        save_len = 1;
                        defer = 1;
                    }
                }
                Data_02004c00->match_ofs = save_ofs;
                Data_02004c00->match_len = save_len;
            }
        }

        if (Data_02004c00->match_len > 1) {
            defer = 0;
            Data_02004c00->out_buf[0] |= Data_02004c00->flag_mask;
            if (Data_02004c00->match_len <= 16) {
                code = ((Data_02004c00->match_ofs << 4) & ~0xfff)
                       | (Data_02004c00->match_ofs & 0xff)
                       | (((Data_02004c00->match_len - 1) << 8) & 0xf00);
                Data_02004c00->out_buf[Data_02004c00->out_cnt++] = (u16)code >> 8;
                Data_02004c00->out_buf[Data_02004c00->out_cnt++] = code;
            } else {
                code = ((Data_02004c00->match_ofs << 4) & ~0xfff)
                       | (Data_02004c00->match_ofs & 0xff);
                Data_02004c00->out_buf[Data_02004c00->out_cnt++] = (u16)code >> 8;
                Data_02004c00->out_buf[Data_02004c00->out_cnt++] = code;
                Data_02004c00->out_buf[Data_02004c00->out_cnt++] =
                    Data_02004c00->match_len - 17;
            }
        } else {
            Data_02004c00->out_buf[Data_02004c00->out_cnt++] =
                Data_02004c00->bucket_by_slot[Data_02004c00->pos];
            Data_02004c00->match_len = 1;
        }

        AudioTrack_ConsumeSlotBytes(Data_02004c00->pos + PRIME_COUNT,
                                    Data_02004c00->match_len, input);
        Data_02004c00->pos =
            (Data_02004c00->pos + Data_02004c00->match_len) & WINDOW_MASK;
        Data_02004c00->flag_mask >>= 1;
        if (Data_02004c00->flag_mask == 0) {
            AudioTrack_CopyBufferedBytes(dst);
            Data_02004c00->flag_mask = 0x80;
            Data_02004c00->out_buf[0] = 0;
            Data_02004c00->out_cnt = 1;
        }
    }

    Data_02004c00->out_buf[0] |= Data_02004c00->flag_mask;
    Data_02004c00->out_buf[Data_02004c00->out_cnt++] = 0;
    Data_02004c00->out_buf[Data_02004c00->out_cnt++] = 0;
    AudioTrack_CopyBufferedBytes(dst);
    return Data_02004c00->out_total;
}
