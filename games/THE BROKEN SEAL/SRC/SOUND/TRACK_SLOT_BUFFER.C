#include "TRACKBUF.H"

void AudioTrack_ResetSlotBuckets(void)
{
    /* FAKEMATCH: retain the original signed word stores at the node back-link
       and slot lanes. Direct node fields change register allocation at the
       same 60-byte TBS extent and shorten the TLA body from 60 to 56. */
    s32 index;
    s32 limit;
    s32 zero;
    u8 *record;
    s32 *slot;

    limit = 0x3FF;
    index = 0;
    zero = 0;
    record = (u8 *)&gScratchWord->nodes[0].back;
    do {
        *(s32 *)(record + sizeof(struct AudioSlotNode *)) = index;
        index++;
        *(s32 *)record = zero;
        record += sizeof(struct AudioSlotNode);
    } while (index <= limit);
    slot = (s32 *)gScratchWord->bucket;
    {
        s32 zero2 = 0;
        for (index = 0xFF; index >= 0; index--) {
            *slot++ = zero2;
        }
    }
}

void AudioTrack_InsertSlotNode(s32 index)
{
    /* FAKEMATCH: retain the existing address-word link stores and reloads.
       A direct typed-node insertion shrinks 68 to 60 bytes and changes
       the back-link store order. All offsets derive from the real owners. */
    s32 base;
    s32 node_off;
    s32 tbl_off;
    s32 bucket;
    s32 bucket_off;
    s32 link_off;
    void **node;
    void *next;

    base = (s32)gScratchWord;
    node_off = index * sizeof(struct AudioSlotNode);
    tbl_off = index * sizeof(s32) + (u32)&((struct AudioTrackSlotWork *)0)->bucket_by_slot;
    bucket = *(s32 *)(base + tbl_off) * 4;
    link_off = node_off + (u32)&((struct AudioSlotNode *)0)->back;
    *(s32 *)(base + link_off) = base + bucket + (u32)&((struct AudioTrackSlotWork *)0)->bucket;
    bucket_off = bucket + (u32)&((struct AudioTrackSlotWork *)0)->bucket;
    *(s32 *)(base + node_off) = *(s32 *)(base + bucket_off);
    node = (void **)(base + node_off);
    *(void **)(base + bucket_off) = node;
    next = *node;
    if (next != 0)
        ((struct AudioSlotNode *)next)->back = (struct AudioSlotNode **)node;
}

void AudioTrack_RemoveSlotNode(s32 slot)
{
    /* FAKEMATCH: retain the existing signed address-word back-link reads
       and reload. Direct node pointers shorten TBS 44 to 40 and TLA 40
       to 36 bytes; these views add no record or storage. */
    s32 next_node;
    s32 track_table;
    s32 slot_offset;
    s32 next_link_offset;
    void *previous_node;

    track_table = (s32)gScratchWord;
    slot_offset = slot * sizeof(struct AudioSlotNode);
    next_link_offset = slot_offset + (u32)&((struct AudioSlotNode *)0)->back;
    next_node = *(s32 *)(track_table + next_link_offset);
    if (next_node != 0) {
        previous_node = *(void **)(track_table + slot_offset);
        if (previous_node != 0) {
            *(s32 *)&((struct AudioSlotNode *)previous_node)->back = next_node;
        }
        **(s32 **)(track_table + next_link_offset) =
            *(s32 *)(track_table + slot_offset);
    }
}

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
            state = gScratchWord;
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
        struct AudioTrackSlotWork **root = &gScratchWord;
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
    /* FAKEMATCH: retain the existing unsigned word views and byte cursor
       at the output lanes. Typed direct accesses shrink 72 to 68 bytes
       and reverse the two lane-base registers. */
    u32 cnt_off = (u32)&((struct AudioTrackSlotWork *)0)->out_cnt;
    u8 **base_p = (u8 **)&gScratchWord;
    u8 *base = *base_p;
    u32 index = 0;
    u32 *cnt_p = (u32 *)(base + cnt_off);
    u32 saved_off = cnt_off;
    u32 count = *cnt_p;

    if (count != 0) {
        u32 displacement = (u32)&((struct AudioTrackSlotWork *)0)->out_total;
        u32 *offset = (u32 *)(base + displacement);
        u32 *cnt_p2;
        u8 *source;
        displacement -= (u32)&((struct AudioTrackSlotWork *)0)->out_total
            - (u32)&((struct AudioTrackSlotWork *)0)->out_buf;
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

    gScratchWord->match_len = 1;
    if (gScratchWord->bucket_by_slot[start] == -1) {
        return;
    }
    node = gScratchWord->bucket[gScratchWord->bucket_by_slot[start]];
    if (start + MATCH_MAX > WINDOW_MASK) {
        /* The run can wrap the ring, so both sides are masked. */
        while (node != NULL) {
            s32 dist = (start - node->slot) & WINDOW_MASK;

            if (dist > 0 && dist <= DIST_MAX) {
                for (len = 1; len < MATCH_MAX; len++) {
                    if (gScratchWord->bucket_by_slot[(start + len) & WINDOW_MASK]
                        != gScratchWord->bucket_by_slot[(node->slot + len) & WINDOW_MASK]) {
                        break;
                    }
                }
                if (gScratchWord->match_len < len) {
                    gScratchWord->match_ofs = dist;
                    gScratchWord->match_len = len;
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
                    if (gScratchWord->bucket_by_slot[start + len]
                        != gScratchWord->bucket_by_slot[(node->slot + len) & WINDOW_MASK]) {
                        break;
                    }
                }
                if (gScratchWord->match_len < len) {
                    gScratchWord->match_ofs = dist;
                    gScratchWord->match_len = len;
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
    gScratchWord = (struct AudioTrackSlotWork *)gMapCellBuffer;
    AudioTrack_ResetSlotBuckets();
    gScratchWord->input_limit = size;
    gScratchWord->pos = 0;
    gScratchWord->input_cursor = 0;
    gScratchWord->out_total = 0;
    gScratchWord->flag_mask = 0x80;
    gScratchWord->out_buf[0] = 0;
    gScratchWord->out_cnt = 1;
    AudioTrack_ConsumeSlotBytes(0, PRIME_COUNT, input);

    while (gScratchWord->bucket_by_slot[gScratchWord->pos] != -1) {
        AudioTrack_FindWindowMatch(gScratchWord->pos);
        if (defer == 0) {
            len = gScratchWord->match_len;
            if (gScratchWord->match_len > 1) {
                /* Lazy evaluation: take one literal now when the match one
                   slot later reaches at least as far as this match followed
                   by the best one after it. */
                save_ofs = gScratchWord->match_ofs;
                save_len = len;
                AudioTrack_FindWindowMatch((gScratchWord->pos + 1) & WINDOW_MASK);
                if (gScratchWord->match_len > 2) {
                    reach[0] = gScratchWord->match_len + 1;
                    AudioTrack_FindWindowMatch((gScratchWord->pos + len) & WINDOW_MASK);
                    reach[1] = gScratchWord->match_len + len;
                    if (reach[0] >= reach[1]) {
                        save_len = 1;
                        defer = 1;
                    }
                }
                gScratchWord->match_ofs = save_ofs;
                gScratchWord->match_len = save_len;
            }
        }

        if (gScratchWord->match_len > 1) {
            defer = 0;
            gScratchWord->out_buf[0] |= gScratchWord->flag_mask;
            if (gScratchWord->match_len <= 16) {
                code = ((gScratchWord->match_ofs << 4) & ~0xfff)
                       | (gScratchWord->match_ofs & 0xff)
                       | (((gScratchWord->match_len - 1) << 8) & 0xf00);
                gScratchWord->out_buf[gScratchWord->out_cnt++] = (u16)code >> 8;
                gScratchWord->out_buf[gScratchWord->out_cnt++] = code;
            } else {
                code = ((gScratchWord->match_ofs << 4) & ~0xfff)
                       | (gScratchWord->match_ofs & 0xff);
                gScratchWord->out_buf[gScratchWord->out_cnt++] = (u16)code >> 8;
                gScratchWord->out_buf[gScratchWord->out_cnt++] = code;
                gScratchWord->out_buf[gScratchWord->out_cnt++] =
                    gScratchWord->match_len - 17;
            }
        } else {
            gScratchWord->out_buf[gScratchWord->out_cnt++] =
                gScratchWord->bucket_by_slot[gScratchWord->pos];
            gScratchWord->match_len = 1;
        }

        AudioTrack_ConsumeSlotBytes(gScratchWord->pos + PRIME_COUNT,
                                    gScratchWord->match_len, input);
        gScratchWord->pos =
            (gScratchWord->pos + gScratchWord->match_len) & WINDOW_MASK;
        gScratchWord->flag_mask >>= 1;
        if (gScratchWord->flag_mask == 0) {
            AudioTrack_CopyBufferedBytes(dst);
            gScratchWord->flag_mask = 0x80;
            gScratchWord->out_buf[0] = 0;
            gScratchWord->out_cnt = 1;
        }
    }

    gScratchWord->out_buf[0] |= gScratchWord->flag_mask;
    gScratchWord->out_buf[gScratchWord->out_cnt++] = 0;
    gScratchWord->out_buf[gScratchWord->out_cnt++] = 0;
    AudioTrack_CopyBufferedBytes(dst);
    return gScratchWord->out_total;
}
