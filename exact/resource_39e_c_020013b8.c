#include "types.h"

/*
 * Resource 39e scene dispatcher at 0x020013b8 (196 bytes, 20 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020013b8 every branch
 * (0x020013d4, 0x020013da, 0x020013e6, 0x020013ec, 0x020013fa, 0x02001400,
 * 0x02001424, 0x0200142e, 0x02001438, 0x02001440, 0x0200144a, 0x02001454,
 * 0x0200145c, 0x02001466, 0x0200146c) lands inside 0x020013b8-0x0200147b, and
 * the walk stops at the interworking return `pop {r5} / pop {r0} / bx r0` at
 * 0x02001476-0x0200147a.  So the executable extent is exactly
 * 0x020013b8-0x0200147b (196 bytes).  The popped register is r0, so the popped
 * value is the return address and the owner is `void`.
 *
 * Pool map — six words the walk never reaches as instructions, four of which
 * objdump renders as `b.n`, `subs` or `adr`:
 *   0x0200147c  0xffffe000   -0x2000
 *   0x02001480  0x3fff0000   the quadrant bound, pre-shifted
 *   0x02001484  0x00003fff   the same bound, unshifted
 *   0x02001488  0xffffa000   -0x6000
 *   0x0200148c  0x0000089a   story flag id
 *   0x02001490  0x0000089b   story flag id
 * None is an address: all six are below the 0x02008000 link base and all six
 * are used as arithmetic operands or as flag ids.  The next prologue
 * (`push {r5, lr}`) begins at 0x02001494 and is itself one of this owner's
 * callees.
 *
 * All 20 call sites were resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 13b8 147c`, and the
 * per-target histogram is Func_080770c0 x4, Func_0808a020 x2, Func_0808a018
 * x1, Func_0808a080 x1, Func_0808a208 x1, Func_0808a218 x1, Func_0808a220 x1,
 * plus nine intra-overlay calls to eight distinct owners:
 *   0x020013bc -> 0x04404 Func_0808a080   0x0200143c -> 0x0102c Func_0200102c
 *   0x020013c2 -> 0x043dc Func_0808a018   0x02001442 -> 0x01160 Func_02001160
 *   0x020013d6 -> 0x00e94 Func_02000e94   0x02001446 -> 0x043e4 Func_0808a020
 *   0x020013e8 -> 0x00bf4 Func_02000bf4   0x0200144e -> 0x043bc Func_080770c0
 *   0x020013fc -> 0x00db4 Func_02000db4   0x02001458 -> 0x0102c Func_0200102c
 *   0x02001402 -> 0x00cd4 Func_02000cd4   0x02001460 -> 0x043bc Func_080770c0
 *   0x0200140e -> 0x044e4 Func_0808a208   0x02001468 -> 0x01494 Func_02001494
 *   0x02001416 -> 0x044fc Func_0808a220   0x0200146e -> 0x01dbc Func_02001dbc
 *   0x0200141a -> 0x044f4 Func_0808a218   0x02001472 -> 0x043e4 Func_0808a020
 *   0x02001428 0x02001432 -> 0x043bc Func_080770c0
 * THIS IS WHY THE SWEEP FOUND SO MANY UNRECORDED OWNERS IN THIS OVERLAY: the
 * dispatcher and six of its eight callees are all absent from
 * out/decomp/overlays.json, so nothing that starts from the inventory can reach
 * any of them.  Func_02000cd4 and Func_0200102c are converted alongside this
 * file in the same pass; Func_02000e94, Func_02000bf4, Func_02000db4,
 * Func_02001160, Func_02001494 and Func_02001dbc remain.
 *
 * THE TWO Func_0808a020 SITES ARE REAL, one per outer arm (0x02001446 and
 * 0x02001472), so both are written out.  This is the mirror of the usual
 * bracket-close defect: here folding the two closes into one shared `goto`
 * would DEFLATE the multiset from two to one.  The opening Func_0808a018 has
 * one site and is written once.
 *
 * The quadrant test is the same idiom as the neighbouring Func_02000f80 and
 * shares its four pool words: the u16 at record+6 is biased by -0x2000,
 * -0x6000 and +0x6000 in turn and compared unsigned against 0x3fff, so the four
 * arms are the four 90-degree sectors in the order [0x2000,0x6000),
 * [0x6000,0xa000), [0xa000,0xe000), rest.  The first comparison is done on the
 * value shifted left by 16 against the pre-shifted 0x3fff0000 and the later two
 * on the value shifted back down against 0x3fff — same test, two spellings,
 * and reading either as a signed magnitude test loses the wrap.  The +0x6000
 * bias is materialised as `movs #192 / lsls #7`, a value and not a
 * displacement.
 *
 * After the sector arm the camera pair (1.0, 0.125) is set and committed, then
 * the s16 at record+18 selects between two flag-driven pairs.  Note the two
 * halves ask the SAME two flags in the OPPOSITE order, which is what makes the
 * four outcomes distinct rather than symmetric:
 *   +18 <= 209:  flag 0x89a clear -> Func_0200102c(0)
 *                flag 0x89a set and 0x89b clear -> Func_02001160()
 *                both set -> Func_0200102c(0)
 *   +18 >  209:  flag 0x89b set -> Func_0200102c(2)
 *                flag 0x89b clear and 0x89a clear -> Func_02001494()
 *                flag 0x89b clear and 0x89a set -> Func_02001dbc()
 * The first of those three is reached by two different edges (fall-through from
 * the 0x89b test and the `beq` from the 0x89a test), so it is spelled once with
 * a label; writing it into both edges would put two Func_0200102c(0) calls in
 * the C against the assembly's one at 0x0200143c.
 */

           /* record fetch, returns the record */
          /* scripted-section open  */
          /* scripted-section close */
          /* camera distance pair (16.16) */
      /* commit the camera move */
          /* two-argument camera selector */
           /* read a story flag */

      /* local: sector arm 1 */
      /* local: sector arm 2 */
      /* local: sector arm 3 */
      /* local: sector arm 4, retreat-along-arc */
          /* local: three-variant scripted scene */
      /* local */
      /* local */
      /* local */

extern u8 * Func_020057c2();
extern void Func_020057a0();
extern void Func_0200226c(void);
extern void Func_02001fde(void);
extern void Func_020021b2(void);
extern void Func_020020d8(void);
extern void Func_020058f4();
extern void Func_02005914();
extern void Func_02005910(void);
extern s32 Func_020057e6();
extern s32 Func_020057f0();
extern void Func_0200246a();
extern void Func_020025a4(void);
extern void Func_0200582c();
extern s32 Func_0200580c();
extern void Func_02002486();
extern s32 Func_0200581e();
extern void Func_020028fe(void);
extern void Func_0200322c(void);
extern void Func_02005858();
void Func_020013b8(void)
{
    u8 *record = Func_020057c2(0);
    u16 facing;

    Func_020057a0();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Func_0200226c();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        Func_02001fde();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {   /* 192 << 7 */
        Func_020021b2();
    } else {
        Func_020020d8();
    }

    Func_020058f4(0x10000, 0x2000);        /* 128 << 9, 128 << 6 */
    Func_02005914(20, 1);
    Func_02005910();

    if (*(s16 *)(record + 18) <= 209) {
        if (Func_020057e6(0x89a) == 0) goto scene0;
        if (Func_020057f0(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        Func_0200246a(0);
        goto firstSceneComplete;
scene1:
        Func_020025a4();
firstSceneComplete:
        Func_0200582c();
        return;
    }

    if (Func_0200580c(0x89b) != 0) {
        Func_02002486(2);
    } else if (Func_0200581e(0x89a) == 0) {
        Func_020028fe();
    } else {
        Func_0200322c();
    }
    Func_02005858();
}
