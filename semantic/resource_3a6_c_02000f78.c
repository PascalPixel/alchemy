#include "types.h"

/*
 * Resource 3a6 tile-27 rain sequence at 0x02000f78 (320 bytes, 24 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000f78, the guard's long branch
 * (0x02000f94 -> 0x020010a6) and the loop back edge (0x02001056 ->
 * 0x02001000) both land inside the body, and the walk stops at the
 * high-register unwind `add sp,#56 / pop {r3,r5} / mov r8,r3 / mov sl,r5 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` at 0x020010aa-0x020010b6.  So the
 * executable extent is exactly 0x02000f78-0x020010b7 (320 bytes).  The popped
 * register is r0, so the popped value is the return address and the owner is
 * `void`.
 *
 * Pool map — four words the walk never reaches as instructions:
 *   0x020010b8  0xfff80000   -8.0 in 16.16, referenced twice
 *   0x020010bc  0x00009999   0.6 in 16.16
 *   0x020010c0  0xffff8000   -0.5 in 16.16
 *   0x020010c4  0x00000201   a story cue id
 * None is an address: all four are below the 0x02008000 link base and all four
 * are arithmetic operands or call arguments.  The next prologue (`push {lr}`)
 * begins at 0x020010c8.
 *
 * All 24 call sites were resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3a6 0f78 10b8`, and the
 * per-target histogram is Func_0808a080 x9, Func_02000048 x2, Func_080000f8
 * x2, Func_080090d0 x2, Func_0808a010 x2, Func_0200013c x1, Func_0808a018 x1,
 * Func_0808a020 x1, Func_0808a100 x1, Func_080091c0 x1, Func_080770c8 x1,
 * Func_080f9010 x1:
 *   0x02000f82 -> 0x01cac Func_0808a018   0x02001040 -> 0x01cbc Func_0808a080
 *   0x02000f88 -> 0x01cbc Func_0808a080   0x02001050 -> 0x01ca4 Func_0808a010
 *   0x02000f98 -> 0x01cbc Func_0808a080   0x02001068 -> 0x01c6c Func_080091c0
 *   0x02000fa4 -> 0x01cbc Func_0808a080   0x0200106e -> 0x01cbc Func_0808a080
 *   0x02000fb6 -> 0x00048 Func_02000048   0x0200107e -> 0x01c94 Func_080770c8
 *   0x02000fbe -> 0x01cbc Func_0808a080   0x02001084 -> 0x01cbc Func_0808a080
 *   0x02000fcc -> 0x01cbc Func_0808a080   0x02001090 -> 0x01cfc Func_0808a100
 *   0x02000fda -> 0x00048 Func_02000048   0x02001096 -> 0x01c4c Func_080090d0
 *   0x02000fe2 -> 0x01cbc Func_0808a080   0x0200109c -> 0x01c4c Func_080090d0
 *   0x02000ffa -> 0x01d5c Func_080f9010   0x020010a2 -> 0x01ca4 Func_0808a010
 *   0x02001000 -> 0x01c24 Func_080000f8   0x020010a6 -> 0x01cb4 Func_0808a020
 *   0x02001012 -> 0x01c24 Func_080000f8   0x0200103a -> 0x0013c Func_0200013c
 *
 * NINE Func_0808a080 SITES FOR ONE RECORD.  The assembly re-fetches record 10
 * before nearly every field access instead of caching the pointer, so the C
 * does the same; caching it would drop the count from nine to one, which is the
 * largest single deflation available in this file.
 *
 * THE WHOLE BODY IS UNDER ONE GUARD and the scripted close is OUTSIDE it.  The
 * guard `asrs #20 / cmp #27` on record 10's +8 is the tile-index view of the
 * 16.16 coordinate (a 16.16 value shifted right by 20 is the coordinate divided
 * by 16, i.e. its tile), so the sequence only runs when that actor stands on
 * tile 27.  Func_0808a018 opens before the test and the single Func_0808a020
 * closes after the join, so the early exit is a `goto` to that close rather
 * than a second close.
 *
 * The two Func_02000048 calls create the two objects that are torn down by the
 * two Func_080090d0 calls at the end — handles parked in sl and r8 across the
 * whole 68-iteration loop, which is why the owner needs the high-register
 * frame.  Both are placed at record 10's +16 plus 52.0 and at its +8 minus and
 * plus 8.0, i.e. symmetrically either side of the actor.
 *
 * The loop runs 68 iterations (`adds r7,#1 / cmp #67 / bls`), one frame each,
 * and each iteration drops one particle at a random point of a rectangle:
 * `lsls #4 / adds` is a multiply by 17 and `lsls #3 / subs / lsls #1` a
 * multiply by 14, and the `lsrs #16 / lsls #16` after each is a FLOOR, not a
 * shift pair — it clears the fraction so the product is a whole number carried
 * in 16.16.  The origins 432.0 (`movs #216 / lsls #17`) and 656.0
 * (`movs #164 / lsls #18`) are materialised as values.  Meanwhile record 10's
 * +12 falls by 0.5 per frame, so the actor sinks 34.0 over the run.
 *
 * The 40-byte parameter block at sp+16 is filled once before the loop with only
 * three fields written (+4 = 7, +8 = +12 = 0.6) and handed to every particle,
 * so it is spelled as bytes rather than as an invented struct that would imply
 * the untouched words are zero.
 */

u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_0808a018();          /* scripted-section open  */
void Func_0808a020();          /* scripted-section close */
void Func_0808a010();          /* wait this many frames */
void Func_0808a100();          /* set a slot's presentation mode */
void Func_080091c0();          /* six-argument presentation request */
void Func_080770c8();          /* one-argument story cue */
void Func_080f9010();          /* one-argument notify */
void Func_080090d0();          /* tear down the object */
s32 Func_080000f8(void);       /* 0..0xffff random source */
s32 Func_02000048();           /* local: create an object, returns its handle */
void Func_0200013c();          /* local: spawn, eight arguments */

void Func_02000f78(void)
{
    u8 params[40];
    s32 left;
    s32 right;
    u32 index;
    s32 x;
    s32 z;

    Func_0808a018();

    if ((*(s32 *)(Func_0808a080(10) + 8) >> 20) != 27) goto close;

                         0,
    left = Func_02000048(*(s32 *)(Func_0808a080(10) + 8) - 0x80000,
                         *(s32 *)(Func_0808a080(10) + 16) + 0x340000,
                         241);
                          0,
    right = Func_02000048(*(s32 *)(Func_0808a080(10) + 8) + 0x80000,
                          *(s32 *)(Func_0808a080(10) + 16) + 0x340000,
                          241);

    Func_0808a080(9)[0x55] = 0;

    *(s32 *)(params + 12) = 0x9999;
    *(s32 *)(params + 4) = 7;
    *(s32 *)(params + 8) = 0x9999;

    Func_080f9010(216);

    for (index = 0; index <= 67; index++) {
        x = ((Func_080000f8() * 17) & ~0xffff) + 0x1b00000;   /* 216 << 17 */
        z = ((Func_080000f8() * 14) & ~0xffff) + 0x2900000;   /* 164 << 18 */
        Func_0200013c(x, 0, z, 0, 0, 0, 0x90000, params);
        *(s32 *)(Func_0808a080(10) + 12) += -0x8000;          /* -0.5 */
        Func_0808a010(1);
    }

    Func_080091c0(31, 39, 2, 1, 27, 41);
    Func_0808a080(10)[0x23] |= 2;
    Func_080770c8(0x201);
    *(s32 *)(Func_0808a080(10) + 12) = -0x80000;              /* -8.0 */
    Func_0808a100(10, 2);
    Func_080090d0(left);
    Func_080090d0(right);
    Func_0808a010(30);

close:
    Func_0808a020();
}
