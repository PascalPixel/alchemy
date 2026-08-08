#include "types.h"

/*
 * resource_39e owner at 0x02001334, 132 bytes: a FOUR-WAY QUADRANT
 * SELECTOR. It reads slot 0's facing angle once and dispatches to one of
 * four already-drafted owners depending on which quarter-turn the angle
 * falls in.
 *
 * Complete owner: `push {r5, lr}` at 0x02001334 through
 * `pop {r5} / pop {r0} / bx r0` at 0x020013a2-0x020013a6, then the
 * four-word literal pool 0x020013a8-0x020013b7. The next owner's
 * prologue is exactly at 0x020013b8. **132 bytes**, measured to the
 * epilogue.
 *
 * Published population (sweep B). All 11 `bl` sites resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 1334 13b8`; sites=11
 * and eleven bl lines were transcribed. Like its neighbour 0x020012e0
 * this row is a `bl` caller of drafted owners -- 0x02000e94, 0x02000bf4,
 * 0x02000db4, 0x02000cd4 -- so it corroborates four drafts at once, and
 * together the two dispatchers name six.
 *
 * THE QUADRANT ARITHMETIC. The angle is fetched ONCE here (unlike
 * 0x020012e0, which re-fetches) into r2 as the u16 at record[6], and each
 * test rotates it by a different bias before comparing against a quarter
 * turn:
 *   angle - 0x2000 <= 0x3fff  -> Func_02000e94
 *   angle - 0x6000 <= 0x3fff  -> Func_02000bf4
 *   angle + 0x6000 <= 0x3fff  -> Func_02000db4
 *   otherwise                 -> Func_02000cd4
 * i.e. the circle split at 0x2000, 0x6000, 0xa000 and 0xe000 -- four
 * quadrants rotated 45 degrees off the axes, which is what you want when
 * the four callees are the four diagonal-facing variants.
 *
 * The compiler spells the FIRST comparison in the high half -- it shifts
 * the biased angle left 16 and compares against 0x3fff0000 -- and the
 * other two in the low half, with the `lsls #16 / lsrs #16` zero-extend
 * pair. Both spellings compute the same predicate. The first is written
 * here in its shifted form so the asm is recoverable from the source; a
 * reader who normalises it silently loses the fact that GCC picked two
 * different encodings for one idiom inside one function.
 *
 * All three biases arrive as pool words (0xffffe000, 0xffffa000) or a
 * shift chain (192 << 7 = 0x6000), and the bound 0x3fff is pooled twice,
 * once as 0x3fff0000 and once as 0x3fff. Five pool words for what is
 * arithmetically two constants.
 *
 * Uncertainty: the reading of record[6] as a facing angle is inference
 * from the quarter-turn constants -- see the same note in
 * resource_39e_c_020012e0.c. Func_0808a100(0, 8) before the dispatch and
 * Func_0808a100(0, 1) after it are transcribed, not identified.
 */

extern void Func_02005722(s32 frames);
extern void Func_0200571c(void);
extern void Func_02005784(void);
extern u8 *Func_0200573e(s32 slot);
extern void Func_0200579c(s32 slot, s32 arg1);
extern void Func_020057ea(s32 slot, s32 arg1);
extern void Func_02001f68(void);
extern void Func_0200213c(void);
extern void Func_02002062(void);
extern void Func_020021f6(void);
extern void Func_020023c8(s32 arg0);

void Func_02001334(void)
{
    u8 *record = Func_0200573e(0);
    u16 angle;

    Func_0200571c();
    Func_0200579c(0, 8);
    Func_02005722(20);

    angle = *(u16 *)(record + 6);

    if ((u32)((angle + 0xffffe000) << 16) <= 0x3fff0000u) {
        Func_020021f6();
    } else if ((u16)(angle + 0xffffa000) <= 0x3fffu) {
        Func_02001f68();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        Func_0200213c();
    } else {
        Func_02002062();
    }

    Func_020057ea(0, 1);
    Func_020023c8(1);
    Func_02005784();
}
