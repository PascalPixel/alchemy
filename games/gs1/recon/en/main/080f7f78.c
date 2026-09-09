#include "types.h"

/*
 * Sliding-window packer that drives the AudioTrack slot machinery at
 * 0x080f7db4..0x080f7f78 (reset_slot_buckets.c, insert_slot_node.c,
 * remove_slot_node.c, consume_slot_bytes.c, copy_buffered_bytes.c).
 *
 * The work block lives at 0x02010000 and is published through the pointer
 * cell Data_02004c00, exactly the block the four siblings already index.
 * Offsets recovered from the literal pools of games/gs1/asm/080f7f78.s and
 * cross-checked against consume_slot_bytes.c:
 *
 *   0x0000  nodes[0x400]      12-byte {prev, back, slot} chain records
 *   0x3000  bucket[0x100]     chain head per byte value
 *   0x3400  flag_mask         walking control-byte bit, 0x80 -> 0
 *   0x3404  val[0x400]        ring of decoded window bytes, -1 = end of input
 *   0x4404  out_cnt           bytes staged in out_buf
 *   0x4408  out_buf[0x24]     one control byte plus up to eight codes
 *   0x442c  match_ofs         best distance found by the last search
 *   0x4430  match_len         best length found by the last search
 *   0x4434  pos               current window slot
 *   0x4438  input_cursor      consume_slot_bytes.c: input_cursor
 *   0x443c  out_total         consume/copy: destination write cursor
 *   0x4440  input_limit       consume_slot_bytes.c: input_limit
 *
 * Output format, read off the two encode blocks at 0x080f8528 and 0x080f8594:
 * a control byte whose set bits mark coded pairs, then per token either one
 * literal byte or a big-endian 16-bit code
 *   bits 12..15 = distance bits 8..11
 *   bits  8..11 = length - 1, or 0 for the extended form
 *   bits  0..7  = distance bits 0..7
 * with the extended form adding a third byte holding length - 17. Because the
 * search only ever accepts distances 1..63 the two distance fields overlap and
 * the high nibble is always zero; the reference builds it anyway, as a shift
 * left by four masked with ~0xfff. A zero code (distance 0, length nibble 0)
 * is written as the terminator.
 *
 * Uncertain, recorded rather than guessed:
 *   - 672 (0xa8 << 2) is the priming byte count handed to ConsumeSlotBytes and
 *     is also added to `pos` on every refill; its relation to the 0x124 skew
 *     inside consume_slot_bytes.c is not established here.
 *   - The three search sites are spelled as one inlined helper. The reference
 *     has three literal copies, each carrying both the wrapping and the
 *     non-wrapping index form; whether the original used a macro, an inline
 *     function or three hand-written copies is not recoverable.
 *   - out_buf is sized to the 0x24 bytes between 0x4408 and 0x442c; only the
 *     control byte plus eight three-byte codes (25 bytes) are ever staged.
 *   - match_ofs is typed u32 because the reference shifts it logically; the
 *     value it ever holds (1..63) does not distinguish the two signednesses.
 *
 * Residual against games/gs1/asm/080f7f78.s: 1836 candidate bytes against
 * 1920, differing_halfwords=916. Both sides carry the same five calls in the
 * same order and the same six chain walks, and every reference branch, loop
 * and store is represented. What is left is allocation and placement, not
 * structure: the reference spills five more locals (44 bytes of frame against
 * 24), recomputes the loop-invariant &val[start + 1] once per chain node where
 * this source lets GCC hoist it above the walk, duplicates the outer loop's
 * exit test instead of sharing one copy, builds 0xf00 from the literal pool
 * rather than as a shifted immediate, and advances the staged count by
 * repeated +1 rather than by a constant. Spelling those byte writes with a
 * `cnt++` local was tried and rejected: it shortens the candidate to 1820
 * bytes and raises both residual counts.
 */

/* One record per window slot. insert_slot_node.c shows the chain is threaded
   through field 0 toward the previously inserted slot holding the same byte
   value, while field 4 holds the address of the pointer cell that refers to
   this record. Only field 0 and the slot index are read here. */
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
    u32 match_ofs;
    s32 match_len;
    s32 pos;
    s32 input_cursor;
    s32 out_total;
    s32 input_limit;
};

/* The owner register carries no name for this address yet, so the readable
   name is bound to the Func_<address> compatibility alias here. */
#define AudioTrack_PackStream Func_080f7f78

extern struct AudioPackWork *Data_02004c00;

void AudioTrack_ResetSlotBuckets(void);
void AudioTrack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input);
void AudioTrack_CopyBufferedBytes(u8 *dst);

#define WINDOW_MASK 0x3ff
#define MATCH_MAX 271
#define DIST_MAX 63
#define PRIME_COUNT 672

/* Longest run starting at `start` that repeats an earlier window slot on the
   same byte-value chain. Leaves the result in match_len / match_ofs; a length
   of 1 means "no usable match". Distances are limited to DIST_MAX. */
static __inline__ void AudioTrack_FindWindowMatch(s32 start)
{
    struct AudioSlotNode *node;
    s32 cand;
    s32 dist;
    s32 len;

    Data_02004c00->match_len = 1;
    if (Data_02004c00->val[start] == -1) {
        return;
    }
    node = Data_02004c00->bucket[Data_02004c00->val[start]];
    if (start + (MATCH_MAX + 1) > WINDOW_MASK) {
        /* The forward run can wrap the ring, so both sides need masking. */
        while (node != NULL) {
            cand = node->slot;
            dist = (start - cand) & WINDOW_MASK;
            if ((u32)(dist - 1) <= (u32)(DIST_MAX - 1)) {
                len = 1;
                while (len <= MATCH_MAX
                       && Data_02004c00->val[(start + len) & WINDOW_MASK]
                          == Data_02004c00->val[(cand + len) & WINDOW_MASK]) {
                    len++;
                }
                if (Data_02004c00->match_len < len) {
                    Data_02004c00->match_ofs = dist;
                    Data_02004c00->match_len = len;
                    if (len == MATCH_MAX + 1) {
                        return;
                    }
                }
            }
            node = node->prev;
        }
    } else {
        /* start + MATCH_MAX still fits the ring, so only the older side wraps. */
        while (node != NULL) {
            cand = node->slot;
            dist = (start - cand) & WINDOW_MASK;
            if ((u32)(dist - 1) <= (u32)(DIST_MAX - 1)) {
                len = 1;
                while (len <= MATCH_MAX
                       && Data_02004c00->val[start + len]
                          == Data_02004c00->val[(cand + len) & WINDOW_MASK]) {
                    len++;
                }
                if (Data_02004c00->match_len < len) {
                    Data_02004c00->match_ofs = dist;
                    Data_02004c00->match_len = len;
                    if (len == MATCH_MAX + 1) {
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
    s32 ahead;
    s32 save_len;
    u32 save_ofs;
    s32 cnt;
    u32 ofs;
    u32 work;
    s16 code;

    defer = 0;
    Data_02004c00 = (struct AudioPackWork *)0x02010000;
    AudioTrack_ResetSlotBuckets();
    Data_02004c00->input_limit = size;
    Data_02004c00->pos = 0;
    Data_02004c00->input_cursor = 0;
    Data_02004c00->out_total = 0;
    Data_02004c00->flag_mask = 0x80;
    /* The reference reads the low byte of the `defer` slot here, so the
       cleared control byte and the cleared flag share one value. */
    Data_02004c00->out_buf[0] = (u8)defer;
    Data_02004c00->out_cnt = 1;
    AudioTrack_ConsumeSlotBytes(0, PRIME_COUNT, input);

    while (Data_02004c00->val[Data_02004c00->pos] != -1) {
        AudioTrack_FindWindowMatch(Data_02004c00->pos);
        if (defer == 0) {
            len = Data_02004c00->match_len;
            if (len > 1) {
                /* Lazy evaluation: prefer one literal plus the match one slot
                   later when that pair covers at least as much as this match
                   plus whatever follows it. */
                save_ofs = Data_02004c00->match_ofs;
                save_len = len;
                AudioTrack_FindWindowMatch((Data_02004c00->pos + 1) & WINDOW_MASK);
                ahead = Data_02004c00->match_len;
                if (ahead > 2) {
                    /* The reference forms ahead + 1 and spills it here, before
                       the third search clobbers match_len, so the increment is
                       written on the local rather than at the comparison. */
                    ahead++;
                    AudioTrack_FindWindowMatch((Data_02004c00->pos + len) & WINDOW_MASK);
                    if (ahead >= Data_02004c00->match_len + len) {
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
            Data_02004c00->out_buf[0] |= (u8)Data_02004c00->flag_mask;
            if (Data_02004c00->match_len > 16) {
                /* Extended form: empty length nibble, run length in a third
                   byte. Lengths 17..272 map onto 0..255. */
                ofs = Data_02004c00->match_ofs;
                work = ((ofs << 4) & ~0xfff) | (ofs & 0xff);
                code = (s16)work;
                cnt = Data_02004c00->out_cnt;
                Data_02004c00->out_buf[cnt] = (u8)(code >> 8);
                Data_02004c00->out_cnt = cnt + 1;
                Data_02004c00->out_buf[cnt + 1] = (u8)code;
                Data_02004c00->out_cnt = cnt + 2;
                Data_02004c00->out_buf[cnt + 2] =
                    (u8)(Data_02004c00->match_len - 17);
                Data_02004c00->out_cnt = cnt + 3;
            } else {
                /* Short form: lengths 2..16 fit the nibble as length - 1. */
                ofs = Data_02004c00->match_ofs;
                work = ((ofs << 4) & ~0xfff) | (ofs & 0xff)
                       | (((u32)(Data_02004c00->match_len - 1) << 8) & 0xf00);
                code = (s16)work;
                cnt = Data_02004c00->out_cnt;
                Data_02004c00->out_buf[cnt] = (u8)(code >> 8);
                Data_02004c00->out_cnt = cnt + 1;
                Data_02004c00->out_buf[cnt + 1] = (u8)code;
                Data_02004c00->out_cnt = cnt + 2;
            }
        } else {
            cnt = Data_02004c00->out_cnt;
            Data_02004c00->out_buf[cnt] =
                (u8)Data_02004c00->val[Data_02004c00->pos];
            Data_02004c00->out_cnt = cnt + 1;
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

    Data_02004c00->out_buf[0] |= (u8)Data_02004c00->flag_mask;
    cnt = Data_02004c00->out_cnt;
    Data_02004c00->out_buf[cnt] = 0;
    Data_02004c00->out_cnt = cnt + 1;
    Data_02004c00->out_buf[cnt + 1] = 0;
    Data_02004c00->out_cnt = cnt + 2;
    AudioTrack_CopyBufferedBytes(dst);
    return Data_02004c00->out_total;
}
