#include "types.h"

/*
 * resource_3a4 owner at 0x02001678, 248 bytes: map the scene id at
 * Data_02000240[224] onto one of twelve in-image script blobs, with a
 * story flag choosing between two of them on the first scene and a
 * default blob for every id the chain does not name.  A PUBLISHED
 * pointer (sweep B) -- its address is handed out as a plain word, so
 * the call-graph walk never reached it.
 *
 * It is a chain of `cmp`/`bne` against pool constants rather than a
 * jump table, which is what the compiler emits for a sparse but
 * contiguous set (0x4d..0x57) when each arm is a single load.  Every
 * arm branches to the ONE shared return at 0x02001704, so the arms are
 * not independent blocks here either -- they are twelve loads into r0
 * feeding a common epilogue.
 *
 * Complete owner: `push {lr}` at 0x02001678 through the interworking
 * return `pop {r1} / bx r1` at 0x02001704-0x02001706, then a
 * TWENTY-SIX-WORD literal pool at 0x02001708-0x0200176f; the next owner
 * begins at 0x02001770 and is itself unowned residue on this sweep.
 * measureSpan reports 144 because it stops at the code -- the pool is
 * 104 bytes, so a table-blind reading undersizes this row by more than
 * two fifths.  The pool trap at its most expensive on a small row.
 * No arguments, returns a pointer.
 *
 * The single call resolves to Func_080770c0 (the story-flag test) by
 * targetOffset.
 *
 * The twelve targets are in-image data: 0x0200cd18 through 0x0200d150,
 * i.e. image offsets 0x4d18..0x5150, spelled by linked address per the
 * convention the existing resource_3a4 files already set for
 * Data_0200cd6c and its neighbours.  They sit in one ascending run,
 * which is consistent with a table of scripts laid out in scene order.
 *
 * Uncertainty: the blobs are called script blobs on the strength of the
 * existing files' naming and their placement, not on anything decoded
 * here; nothing in this row reads their contents.  Scene ids 0x4d..0x57
 * and flag 0x8fd are literal.  The return type is spelled as a byte
 * pointer to match the existing declarations rather than because this
 * row dereferences it -- it does not.
 */

extern s16 Data_02000240[];

extern u8 Data_0200cd18[];   /* image offset 0x4d18: script blob (default) */
extern u8 Data_0200cd24[];   /* image offset 0x4d24: script blob */
extern u8 Data_0200cd6c[];   /* image offset 0x4d6c: script blob */
extern u8 Data_0200cd9c[];   /* image offset 0x4d9c: script blob */
extern u8 Data_0200cdc0[];   /* image offset 0x4dc0: script blob */
extern u8 Data_0200ce5c[];   /* image offset 0x4e5c: script blob */
extern u8 Data_0200cebc[];   /* image offset 0x4ebc: script blob */
extern u8 Data_0200cf34[];   /* image offset 0x4f34: script blob */
extern u8 Data_0200cfb8[];   /* image offset 0x4fb8: script blob */
extern u8 Data_0200d06c[];   /* image offset 0x506c: script blob */
extern u8 Data_0200d0cc[];   /* image offset 0x50cc: script blob */
extern u8 Data_0200d12c[];   /* image offset 0x512c: script blob */
extern u8 Data_0200d150[];   /* image offset 0x5150: script blob */

extern s32 Func_080770c0(s32 flagId);

u8 *Func_02001678(void)
{
    s16 scene = Data_02000240[224];

    if (scene == 0x4d) {
        if (Func_080770c0(0x8fd) != 0) {
            return Data_0200cd6c;
        }
        return Data_0200cd24;
    }
    if (scene == 0x4e) {
        return Data_0200cd9c;
    }
    if (scene == 0x4f) {
        return Data_0200cdc0;
    }
    if (scene == 0x50) {
        return Data_0200ce5c;
    }
    if (scene == 0x51) {
        return Data_0200cebc;
    }
    if (scene == 0x52) {
        return Data_0200cf34;
    }
    if (scene == 0x53) {
        return Data_0200cfb8;
    }
    if (scene == 0x54) {
        return Data_0200d06c;
    }
    if (scene == 0x55) {
        return Data_0200d0cc;
    }
    if (scene == 0x56) {
        return Data_0200d12c;
    }
    if (scene == 0x57) {
        return Data_0200d150;
    }
    return Data_0200cd18;
}
