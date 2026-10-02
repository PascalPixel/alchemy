#include "TYPES.H"

/*
 * Sliding-window packer over the AudioTrack slot work block that
 * TRACK_SLOT_BUFFER.C maintains (reset, insert, remove, consume, copy).
 * The block is the map cell buffer, published through Flash_Handler3.
 *
 * Output: a control byte whose set bits mark coded pairs, then per token one
 * literal byte or a big-endian 16-bit code (distance bits 8..11 in the top
 * nibble, length - 1 in the next, distance bits 0..7 below), with a third
 * byte holding length - 17 when the length nibble is 0. A zero code ends it.
 *
 * What the listing shows about the source, each confirmed by the compiler:
 *   - the search is one inlined routine expanded three times; `len` is its
 *     own local and `dist` is declared inside each chain walk (the two walks
 *     hold it in different registers, the three expansions in the same ones);
 *   - the run comparison is a for loop with a break, and the chain record's
 *     slot is read again inside it (the wrapping walk's exit test is too long
 *     to duplicate before the first loop pass, the other walk's is not);
 *   - match_ofs is signed, so the code word is built in a signed short;
 *   - the high byte is taken from the code as an unsigned short;
 *   - the lazy test reads match_len again after copying it to `len`.
 *
 * Remaining difference against recon/tbs/raw/080f7f78.s: 3 instructions.
 * The reference keeps `len3` in a stack slot of its own (one more word of
 * frame, one store before the compare, every stack offset above it shifted),
 * and loads the constant offset of `pos` before it adds 1 to `len2` where
 * this source adds first. Everything else is identical.
 */

struct AudioSlotNode {
    struct AudioSlotNode *prev;
    struct AudioSlotNode **back;
    s32 slot;
};

struct AudioPackWork {
    struct AudioSlotNode nodes[0x400];
    struct AudioSlotNode *bucket[0x100];
    s32 flag_mask;
    s32 val[0x400];
    s32 out_cnt;
    u8 out_buf[0x24];
    s32 match_ofs;
    s32 match_len;
    s32 pos;
    s32 input_cursor;
    s32 out_total;
    s32 input_limit;
};

extern struct AudioPackWork *Flash_Handler3;
extern u8 gMapCellBuffer[];

void AudioTrack_ResetSlotBuckets(void);
void AudioTrack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input);
void AudioTrack_CopyBufferedBytes(u8 *dst);

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

    Flash_Handler3->match_len = 1;
    if (Flash_Handler3->val[start] == -1) {
        return;
    }
    node = Flash_Handler3->bucket[Flash_Handler3->val[start]];
    if (start + MATCH_MAX > WINDOW_MASK) {
        /* The run can wrap the ring, so both sides are masked. */
        while (node != NULL) {
            s32 dist = (start - node->slot) & WINDOW_MASK;

            if (dist > 0 && dist <= DIST_MAX) {
                for (len = 1; len < MATCH_MAX; len++) {
                    if (Flash_Handler3->val[(start + len) & WINDOW_MASK]
                        != Flash_Handler3->val[(node->slot + len) & WINDOW_MASK]) {
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
                    if (Flash_Handler3->val[start + len]
                        != Flash_Handler3->val[(node->slot + len) & WINDOW_MASK]) {
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

/* Pack `size` bytes of `input` into `dst`; returns the packed byte count. */
s32 AudioTrack_PackStream(const u8 *input, u8 *dst, s32 size)
{
    s32 defer;
    s32 len;
    s32 save_ofs;
    s32 save_len;
    s32 len3;
    s32 len2;
    s16 code;

    defer = 0;
    Flash_Handler3 = (struct AudioPackWork *)gMapCellBuffer;
    AudioTrack_ResetSlotBuckets();
    Flash_Handler3->input_limit = size;
    Flash_Handler3->pos = 0;
    Flash_Handler3->input_cursor = 0;
    Flash_Handler3->out_total = 0;
    Flash_Handler3->flag_mask = 0x80;
    Flash_Handler3->out_buf[0] = 0;
    Flash_Handler3->out_cnt = 1;
    AudioTrack_ConsumeSlotBytes(0, PRIME_COUNT, input);

    while (Flash_Handler3->val[Flash_Handler3->pos] != -1) {
        AudioTrack_FindWindowMatch(Flash_Handler3->pos);
        if (defer == 0) {
            len = Flash_Handler3->match_len;
            if (Flash_Handler3->match_len > 1) {
                /* Lazy evaluation: take one literal now when the match one
                   slot later reaches at least as far as this match followed
                   by the best one after it. */
                save_ofs = Flash_Handler3->match_ofs;
                save_len = len;
                AudioTrack_FindWindowMatch((Flash_Handler3->pos + 1) & WINDOW_MASK);
                if (Flash_Handler3->match_len > 2) {
                    len2 = Flash_Handler3->match_len + 1;
                    AudioTrack_FindWindowMatch((Flash_Handler3->pos + len) & WINDOW_MASK);
                    len3 = Flash_Handler3->match_len + len;
                    if (len2 >= len3) {
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
                Flash_Handler3->val[Flash_Handler3->pos];
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
