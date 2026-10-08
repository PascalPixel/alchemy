#include "TRACKBUF.H"

void AudioTrack_ResetSlotBuckets(void)
{
    /* FAKEMATCH: retain the original signed word stores at the node back-link and slot lanes. Direct node fields change register allocation at the same 60-byte TBS extent and shorten the TLA body from 60 to 56. */
    s32 index;
    s32 limit;
    s32 zero;
    u8 *record;
    s32 *slot;

    limit = 0x3FF;
    index = 0;
    zero = 0;
    record = (u8 *)&Flash_Handler3->nodes[0].back;
    do {
        *(s32 *)(record + sizeof(struct AudioSlotNode *)) = index;
        index++;
        *(s32 *)record = zero;
        record += sizeof(struct AudioSlotNode);
    } while (index <= limit);
    slot = (s32 *)Flash_Handler3->bucket;
    {
        s32 zero2 = 0;
        for (index = 0xFF; index >= 0; index--) {
            *slot++ = zero2;
        }
    }
}

void AudioTrack_InsertSlotNode(s32 index)
{
    /* FAKEMATCH: address-word link stores preserve back-link store order and 68 bytes; typed insertion yields 60. */
    s32 base;
    s32 node_off;
    s32 tbl_off;
    s32 bucket;
    s32 bucket_off;
    s32 link_off;
    void **node;
    void *next;

    base = (s32)Flash_Handler3;
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
    /* FAKEMATCH: signed address-word reads retain the back-link reload; typed pointers remove four bytes in each game. */
    s32 next_node;
    s32 track_table;
    s32 slot_offset;
    s32 next_link_offset;
    void *previous_node;

    track_table = (s32)Flash_Handler3;
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
    u32 base = start;

    if (current < limit) {
        u32 mask = 0x3ff;
        u32 removal = base + 0x124;

        do {
            u32 slot = base + current;
            struct AudioTrackSlotWork *state;
            u32 read_offset;
            u32 next_offset;
            u8 value;

            AudioTrack_RemoveSlotNode(removal & mask);
            state = Flash_Handler3;
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
        struct AudioTrackSlotWork **root = &Flash_Handler3;
        s32 empty = -1;

        do {
            u32 slot = (base + current) & mask;
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
    /* FAKEMATCH: unsigned word views and the byte cursor preserve lane-base registers; direct accesses swap them and remove four bytes. */
    u32 cnt_off = (u32)&((struct AudioTrackSlotWork *)0)->out_cnt;
    u8 **base_p = (u8 **)&Flash_Handler3;
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

#define WINDOW_MASK 0x3ff
#define MATCH_MAX 272
#define DIST_MAX 63
#define PRIME_COUNT 672

static __inline__ void AudioTrack_FindWindowMatch(s32 start)
{
    struct AudioSlotNode *node;
    s32 len;

    Flash_Handler3->match_len = 1;
    if (Flash_Handler3->bucket_by_slot[start] == -1) {
        return;
    }
    node = Flash_Handler3->bucket[Flash_Handler3->bucket_by_slot[start]];
    if (start + MATCH_MAX > WINDOW_MASK) {
        while (node != NULL) {
            s32 dist = (start - node->slot) & WINDOW_MASK;

            if (dist > 0 && dist <= DIST_MAX) {
                for (len = 1; len < MATCH_MAX; len++) {
                    if (Flash_Handler3->bucket_by_slot[(start + len) & WINDOW_MASK]
                        != Flash_Handler3->bucket_by_slot[(node->slot + len) & WINDOW_MASK]) {
                        break;
                    }
                }
                if (Flash_Handler3->match_len < len) {
                    Flash_Handler3->match_ofs = dist;
                    Flash_Handler3->match_len = len;
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
                    if (Flash_Handler3->bucket_by_slot[start + len]
                        != Flash_Handler3->bucket_by_slot[(node->slot + len) & WINDOW_MASK]) {
                        break;
                    }
                }
                if (Flash_Handler3->match_len < len) {
                    Flash_Handler3->match_ofs = dist;
                    Flash_Handler3->match_len = len;
                    if (len == MATCH_MAX) {
                        return;
                    }
                }
            }
            node = node->prev;
        }
    }
}

s32 AudioTrack_PackStream(const u8 *input, u8 *dst, s32 size)
{
    s32 defer;
    s32 len;
    s32 save_ofs;
    s32 save_len;
    s32 reach[2];
    s16 code;

    defer = 0;
    Flash_Handler3 = (struct AudioTrackSlotWork *)gMapCellBuffer;
    AudioTrack_ResetSlotBuckets();
    Flash_Handler3->input_limit = size;
    Flash_Handler3->pos = 0;
    Flash_Handler3->input_cursor = 0;
    Flash_Handler3->out_total = 0;
    Flash_Handler3->flag_mask = 0x80;
    Flash_Handler3->out_buf[0] = 0;
    Flash_Handler3->out_cnt = 1;
    AudioTrack_ConsumeSlotBytes(0, PRIME_COUNT, input);

    while (Flash_Handler3->bucket_by_slot[Flash_Handler3->pos] != -1) {
        AudioTrack_FindWindowMatch(Flash_Handler3->pos);
        if (defer == 0) {
            len = Flash_Handler3->match_len;
            if (Flash_Handler3->match_len > 1) {
                save_ofs = Flash_Handler3->match_ofs;
                save_len = len;
                AudioTrack_FindWindowMatch((Flash_Handler3->pos + 1) & WINDOW_MASK);
                if (Flash_Handler3->match_len > 2) {
                    reach[0] = Flash_Handler3->match_len + 1;
                    AudioTrack_FindWindowMatch((Flash_Handler3->pos + len) & WINDOW_MASK);
                    reach[1] = Flash_Handler3->match_len + len;
                    if (reach[0] >= reach[1]) {
                        save_len = 1;
                        defer = 1;
                    }
                }
                Flash_Handler3->match_ofs = save_ofs;
                Flash_Handler3->match_len = save_len;
            }
        }

        if (Flash_Handler3->match_len > 1) {
            defer = 0;
            Flash_Handler3->out_buf[0] |= Flash_Handler3->flag_mask;
            if (Flash_Handler3->match_len <= 16) {
                code = ((Flash_Handler3->match_ofs << 4) & ~0xfff)
                       | (Flash_Handler3->match_ofs & 0xff)
                       | (((Flash_Handler3->match_len - 1) << 8) & 0xf00);
                Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = (u16)code >> 8;
                Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = code;
            } else {
                code = ((Flash_Handler3->match_ofs << 4) & ~0xfff)
                       | (Flash_Handler3->match_ofs & 0xff);
                Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = (u16)code >> 8;
                Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = code;
                Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] =
                    Flash_Handler3->match_len - 17;
            }
        } else {
            Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] =
                Flash_Handler3->bucket_by_slot[Flash_Handler3->pos];
            Flash_Handler3->match_len = 1;
        }

        AudioTrack_ConsumeSlotBytes(Flash_Handler3->pos + PRIME_COUNT,
                                    Flash_Handler3->match_len, input);
        Flash_Handler3->pos =
            (Flash_Handler3->pos + Flash_Handler3->match_len) & WINDOW_MASK;
        Flash_Handler3->flag_mask >>= 1;
        if (Flash_Handler3->flag_mask == 0) {
            AudioTrack_CopyBufferedBytes(dst);
            Flash_Handler3->flag_mask = 0x80;
            Flash_Handler3->out_buf[0] = 0;
            Flash_Handler3->out_cnt = 1;
        }
    }

    Flash_Handler3->out_buf[0] |= Flash_Handler3->flag_mask;
    Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = 0;
    Flash_Handler3->out_buf[Flash_Handler3->out_cnt++] = 0;
    AudioTrack_CopyBufferedBytes(dst);
    return Flash_Handler3->out_total;
}
