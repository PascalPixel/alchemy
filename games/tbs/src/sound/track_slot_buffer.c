#include "trackbuf.h"

void AudioTrack_ResetSlotBuckets(void)
{
    s32 index;
    struct AudioSlotNode **bucket;

    for (index = 0; index < 0x400; index++) {
        Flash_Handler3->nodes[index].slot = index;
        Flash_Handler3->nodes[index].prev = NULL;
    }
    bucket = Flash_Handler3->bucket;
    for (index = 0xFF; index >= 0; index--) {
        *bucket++ = NULL;
    }
}

void AudioTrack_InsertSlotNode(s32 index)
{
    struct AudioTrackSlotWork *work = Flash_Handler3;

    work->nodes[index].prev = (struct AudioSlotNode *)&work->bucket[work->bucket_by_slot[index]];
    work->nodes[index].next = work->bucket[work->bucket_by_slot[index]];
    work->bucket[work->bucket_by_slot[index]] = &work->nodes[index];
    if (work->nodes[index].next != NULL)
        work->nodes[index].next->prev = &work->nodes[index];
}

void AudioTrack_RemoveSlotNode(s32 slot)
{
    if (Flash_Handler3->nodes[slot].prev != NULL) {
        if (Flash_Handler3->nodes[slot].next != NULL)
            Flash_Handler3->nodes[slot].next->prev = Flash_Handler3->nodes[slot].prev;
        Flash_Handler3->nodes[slot].prev->next = Flash_Handler3->nodes[slot].next;
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
    struct AudioTrackSlotWork *work = Flash_Handler3;
    s32 index;

    for (index = 0; index != work->out_cnt; index++) {
        destination[work->out_total] = work->out_buf[index];
        work->out_total++;
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
            node = node->next;
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
            node = node->next;
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
