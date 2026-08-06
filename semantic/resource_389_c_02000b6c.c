#include "types.h"

/*
 * resource_389 owner at 0x02000b6c, 100 bytes: a scene-script callback that
 * repaints actor 8 one of two ways depending on the tile row it stands on, and
 * records which way in event flag 0x302.
 *
 * Named before it was opened.  It appears as the middle word of a three-word
 * scene-script record in the overlay's data band:
 *   0xffff0008, 0x02008b6d, 0x00008c15
 * Under the proven 0x02008000 link base 0x02008b6d is file offset 0x0b6c plus
 * the Thumb bit, and the record's low halfword 8 is exactly the actor selector
 * this owner passes to Func_0808a080.  Its neighbours in the same table are
 * 0x02008ce1 with selector 9 and 0x02009089 with selector 0x0a, and both of
 * those owners use their record's selector too, so the field is corroborated
 * three times.
 *
 * Complete owner: `push {r5, lr}` at 0x02000b6c through the single epilogue at
 * 0x02000bc0, ending `pop {r0} / bx r0` - void.
 *
 * Pool map from a control-flow walk: 0x02000b6c-0x02000bc7 is reached as
 * instructions, 0x02000bc8-0x02000bcf is not (0x000fffff and 0x00000302).
 * 92 code + 8 pool = 100, exactly the inventory row's figures.
 *
 * Calls: 7 sites against an advertised 6 - a beat, the safe direction.
 * Per-target multiset from `bun tools/overlay_call_targets.ts resource_389
 * b6c`:
 *   Func_080091c0  x2   0xb98 0xbb2
 *   Func_0808a080  x1   0xb72
 *   Func_0808a018  x1   0xb82
 *   Func_0808a020  x1   0xbbc
 *   Func_080770c8  x1   0xbb8
 *   Func_080770d0  x1   0xb9e
 *
 * Func_0808a020 at 0x02000bbc is the shared tail of both arms: written once
 * after the `if`/`else` so its site count stays at one.  The two Func_080091c0
 * calls are NOT folded - they differ only in r0 (18 against 24) and are two
 * genuine sites.
 *
 * Cross-check with the entry-0 root: semantic/overlays/resource_389_c_0200121c.c
 * reads the same flag 0x302 and chooses between exactly the same two
 * Func_080091c0 argument sets, with 24 on the flag-set side.  Here the arm that
 * SETS the flag is the one that passes 24, and the arm that CLEARS it passes
 * 18.  The two owners agree, which is an independent check on both readings.
 *
 * `x + 0x000fffff when negative, then asr #20` is the tile-grid idiom.
 *
 * UNCERTAINTY: at 0x02000b82 r0 still holds the actor returned at 0x02000b72,
 * so Func_0808a018 is spelled with it; the same shape and the same uncertainty
 * as 0x02000ce0's site.
 */

struct SceneActor {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneActor *Func_0808a080();  /* scene actor by selector */
void Func_0808a018();                /* scene bracket helper */
void Func_0808a020();                /* scene bracket close */
void Func_080091c0();                /* six-argument scripted-actor call */
void Func_080770c8();                /* set an event flag */
void Func_080770d0();                /* clear an event flag */

void Func_02000b6c(void)
{
    struct SceneActor *actor = Func_0808a080(8);
    s32 tile = actor->x;

    if (tile < 0) tile += 0x000fffff;
    tile >>= 20;

    Func_0808a018(actor);

    if (tile == 20) {
        { s32 f1 = 18; s32 g1 = 6; Func_080091c0(18, 40, 6, 3,  f1, g1); }
        Func_080770d0(0x302);
    } else {
        { s32 f2 = 18; s32 g2 = 6; Func_080091c0(24, 40, 6, 3,  f2, g2); }
        Func_080770c8(0x302);
    }

    Func_0808a020();
}
