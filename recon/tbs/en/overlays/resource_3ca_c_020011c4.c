/* NONMATCHING 2026-09-27: direct global counter reads and decrement,
 * followed by a row-loop pointer, produce 228/232 bytes, 109 differing
 * halfwords and 20 aligned edits. The pool order improves, but the tile
 * claims lr and the counter moves to ip, deleting four required copies.
 * Restore the canonical pointer lifetime; this split-owner axis is closed.
 * Astra load/shift boundary (2026-09-27): load the raw tile offset,
 * initialize count, then shift tile. This restores the first three pool
 * values' order, but tile becomes r6 and the candidate shrinks to 228/232
 * (106 halfwords / 42 aligned edits). An explicit unsigned shift fixes
 * asrs to lsrs only; the allocation and score stay unchanged. A distinct
 * offset input restores the row bodies (228/109/20), but exchanges lr/ip
 * ownership and omits the reference's count copy. Full differences read.
 * Reject all three: correct pool order alone does not admit the entry.
 * Restore the 232-byte canonical; stop this load/shift lifetime axis.
 * NONMATCHING Astra ship-row pass (2026-09-27): 232/232 bytes, 24
 * differing halfwords / 17 aligned edits. Complete normalized differences
 * read after every step. No exact bytes or alignment credit.
 * Transferred TITLE.C's goto loop and advancing word writes. The counter
 * now has word-sized 16-bit fields: this prevents HImode add-of-0xffff and
 * reproduces the word subtract. Explicit signed/unsigned snapshots precede
 * the test. Only the first row's read is volatile; the goto loop prevents
 * hoisting its mask/zero/shape. A separate off temporary puts division before
 * the zero store. Three post-increment writes in the later rows retain the
 * original cursor alongside the compiler's induction, matching both updates.
 * Submission advances p after the call. These are the admitted invariants.
 * Remaining: count initialization/pool precedes tile setup instead of
 * interleaving it; two reload scratches and the last mask/counter order.
 * Bounded witnesses (candidate bytes / differing halfwords / aligned edits):
 * full-width s16/u16 fields: 236/82/51, identical to the older union;
 * s32/u32 fields: 232/78/50; pretest snapshots: 232/79/51;
 * volatile row read in for: 228/111/66, unwanted hoisted constants and r8;
 * absolute pointers plus union word/counter stores: 232/79/51, no alias fix;
 * goto first row: 224/93/47; advancing all words: 228/107/25;
 * division staged before stores: 228/109/20;
 * count initialized before tile lookup: 232/24/17, retained below.
 * Moving count between slot lookup and tile load restores the old 228/109/20;
 * between id and slot lookup: 232/26/21, wrong pool order and scratches;
 * using count again in the last row gives lr/ip correctly but retains the
 * tail address in lr instead of reloading it: 228/109/17;
 * one-pass count publication after slot lookup: 228/108/21, still swapped;
 * all counter accesses volatile: 232/97/20, extra decrement read and wrong
 * signed test. These producer/qualifier alternatives are rejected. Keep the
 * admitted row/cursor behavior and obtain new evidence before another trial.
 */
/* Earlier NONMATCHING: canonical body was 236/232 bytes, 82 differing
 * halfwords, 51 aligned edits; superseded by the Astra row/cursor model.
 * H5 ship 2026-09-27, absolute RAM pointers with the
 * sprite word/halfword union give 244/232 bytes, 87 differing halfwords,
 * 56 aligned edits. Approved alias.c treats differing symbol bases as
 * nonaliasing, which admitted this distinct base-identity test; replacing
 * those bases still does not restore the word subtract or first loop
 * reload. It also duplicates both address pool words because the tail
 * still uses named globals. Complete normalized diff read; admission fails.
 * Restore independent globals and plain word stores. No alias/constant
 * spelling follow-up is supported by this counterexample.
 * H4 ship 2026-09-27, sprite word/halfword union writes
 * produce exactly the baseline binary (236/232 bytes, 82 differing halfwords,
 * 51 aligned edits). This does not give the separate-global counter the
 * aggregate state's reload/decrement shape. Restore the u32 sprite words;
 * close this alias-view transfer after the complete normalized comparison.
 * H3 ship 2026-09-27, capturing signed test and unsigned decrement inputs
 * before the branch gives 236/232 bytes, 103 differing halfwords, 54 aligned
 * edits. CSE introduces another unsigned load; the decrement still uses
 * pooled 0xffff and forwards the first iteration. Reject pre-branch input
 * capture: it does not satisfy the reference's two-load/word-subtract gate.
 * The pre-branch reference invariant remains ldrsh, ldrh, compare, word
 * subtract, strh, then an independent loop reload. No further view or
 * temporary spelling is justified without a new ownership fact.
 * H2 ship 2026-09-27, 236 of 232 bytes, 82 differing
 * halfwords, 51 aligned edits. Signed union destination with unsigned source
 * produces the original baseline bytes: no independent word decrement and
 * still a forwarded first iteration. Full normalized diff read. Close the
 * volatile/view axis after two trials without the required subtract shape.
 * H1 ship 2026-09-27, 236 of 232 bytes, 104 differing
 * halfwords, 69 aligned edits. Volatile counter pointer restores the loop
 * reload but adds r8 saves, a second decrement load and a 0xffff pool word;
 * it does not emit the required unconditional ldrsh/ldrh then word subtract.
 * Full normalized diff read. Rejected volatile-pointer ownership model.
 * Previous baseline: 236 of 232 bytes, 82 differing halfwords, 51 aligned edits.
 * 2026-09-27: the complete pool proves the 24 twelve-byte records end at
 * count +0x120, followed by the VRAM id +0x122. Modelling these as one
 * state restores the word decrement but folds the three independent pool
 * addresses into one base and offsets. It still forwards the decremented
 * value into the first iteration instead of reloading. Rejected ownership
 * model preserved in 6fd230b5d. Separate signed/unsigned union views of the
 * counter emit exactly the separate-global baseline bytes (cmp confirmed):
 * the unsigned decrement still uses the pooled 0xffff and forwards its
 * value into the first iteration. Keep separate globals and these views;
 * stop the state-layout/counter-view axis after both structural trials.
 * Earlier baseline: 236/232 bytes, 82 halfwords, 51 aligned edits. Same loops
 * as MENU/TITLE/SPRITE_ROW.C. Remaining: the reference loads the counter
 * twice before the test (ldrsh for the test, ldrh for an SImode decrement)
 * where ours decrements in HImode through a pooled 0xffff; ours then threads
 * the first loop iteration past its reload (b into the loop), and the
 * tile/255 registers differ. A signed decrement shrank to 228 bytes but did
 * not recover the unsigned second load. An explicit word cast and goto loop
 * compiled like the original draft; retain its ordinary for loop. */
#include "TYPES.H"

struct VramBlock {
    u16 base;
    u16 offset;
};

struct Sprite {
    u32 words[3];
};

union RowCounter {
    /* FAKEMATCH: word-sized fields retain the unsigned decrement. */
    s32 signed_value : 16;
    u32 value : 16;
};

extern struct VramBlock Data_03001b10[];
extern s16 BabiFune_FadeSlot;
extern union RowCounter BabiFune_FadeStep;
extern u32 Data_02009af8[];

void Main_080001e8(struct Sprite *sprite, s32 value);

void Local_020011c4(void)
{
    u32 *w;
    struct Sprite *p;
    union RowCounter *count;
    s32 tile;
    u32 i;
    s32 v;
    s32 y;
    s32 active;
    u32 remaining;

    count = &BabiFune_FadeStep;
    tile = Data_03001b10[BabiFune_FadeSlot].offset >> 5;
    w = Data_02009af8;
    active = count->signed_value;
    remaining = count->value;
    if (active != 0) {
        count->signed_value = remaining - 1;
    }
    /* FAKEMATCH: a row-local volatile read and the title's goto loop
     * preserve the independent counter read without hoisting constants. */
    i = 0;
first_row:
    {
        s32 off;

        v = (s16)*(volatile u16 *)count;
        off = -v / 2;
        *w++ = 0;
        *w++ = (off & 0xff) | (i << 21) | 0x80004000;
        *w++ = tile;
    }
    if (++i < 8)
        goto first_row;
    y = (v / 2 + 0x88) & 0xff;
    for (i = 0; i < 8; i++) {
        *w++ = 0;
        *w++ = (i << 21) | y | 0x80004000;
        *w++ = tile;
    }
    y = (BabiFune_FadeStep.signed_value / 2 + 0x98) & 0xff;
    for (i = 0; i < 8; i++) {
        *w++ = 0;
        *w++ = (i << 21) | y | 0x80004000;
        *w++ = tile;
    }
    p = (struct Sprite *)Data_02009af8;
    for (i = 0; i < 24; i++) {
        Main_080001e8(p, 255);
        p++;
    }
}
