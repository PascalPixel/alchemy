#include "types.h"
extern u8 Value_000000a9;
extern u8 Value_000000aa;

/*
 * Resource 3c3, table selector at 0x0200008c (88 bytes, 1 call site).
 *
 * Complete owner: `push {lr}` at 0x0200008c, `pop {r1} / bx r1` at 0x020000be.
 * The popped branch register is r1, so r0 survives and IS the result — this
 * owner returns a pointer to an in-image script table.  Bytes 0x020000c4 to
 * 0x020000e3 are its literal pool (seven words), reached by no control path.
 *
 * This is the third of three sibling selectors in this overlay; 0x02000040 and
 * 0x020000e4 are already byte-exact in `assets/code/` and have exactly this
 * shape.  The difference here is the extra story-flag test in the 0xa9 arm,
 * which picks between two tables instead of one.
 *
 * The overlay is linked at 0x02008000, so a pool word 0x0200_8xxx names the
 * in-image offset `word - 0x8000`: 0x02008ba8 -> 0xba8, 0x02008c98 -> 0xc98,
 * 0x02008c50 -> 0xc50, 0x02008b90 -> 0xb90.  All four are even, so all four
 * are data (an odd word would be a Thumb entry point).  Confirmed against the
 * byte-exact sibling 0x02000040, whose 0x02008a40 / 0x02008ad0 / 0x02008998
 * words are the same kind of table.
 *
 * `Data_02000240` is the cross-overlay RAM global block; its signed halfword
 * at element 224 (byte offset 448) is the scene id this overlay keys on.  The
 * byte-exact siblings spell the compared constants `(s32)&Value_000000aa` —
 * that is the exact reconstruction's pooling device for a small integer, not a symbol,
 * so they are written here as the integers 0xaa and 0xa9.
 *
 * The single call site resolves through `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` to the
 * import veneer at 0x0200087c -> Func_080770c0.  Per-target multiset:
 *   Func_080770c0 x1.
 */

/* Old-style declarations: overlay import arities vary per call site. */
s32 Func_0200092a();    /* raw encoded call destination */

extern s16 Data_02000240[];
extern u8 Data_02008ba8[];
extern u8 Data_02008c98[];
extern u8 Data_02008c50[];
extern u8 Data_02008b90[];

s32 Func_0200008c(void)
{
    s16 scene = Data_02000240[224];

    if (scene == (s32)&Value_000000aa) {
        return (s32)Data_02008ba8;
    }
    if (scene == (s32)&Value_000000a9) {
        if (Func_0200092a(0x96f) != 0) {
            return (s32)Data_02008c98;
        }
        return (s32)Data_02008c50;
    }
    return (s32)Data_02008b90;
}
