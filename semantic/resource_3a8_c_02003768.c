typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3a8 owner at 0x02003768, 252 bytes: the per-frame step for the two
 * scripted followers 8 and 9, run while the hero walks past them.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02003768 through the single
 * interworking epilogue `pop {r5, r6} / pop {r0} / bx r0` at 0x0200383c,
 * followed by an alignment halfword and its eight-word literal pool at
 * 0x02003844-0x02003863.  The pool is data, not code, and is excluded.
 * Nothing is live past the return.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  r0 is written with 8
 * before any read and r1-r3 are never read before being written, so it takes
 * no arguments.
 *
 * Call accounting: 15 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3a8 3768` (6 distinct targets:
 * 5 import veneers and 1 intra-overlay prologue).  The disassembler's own
 * annotations are wrong in the usual overlay way - an overlay `bl` stores
 * `target_offset - 2` - and were not used.  In body order:
 *   0x0200376c, 0x02003774, 0x02003792 -> 0x02003c1c -> Func_0808a080
 *   0x020037b0, 0x020037ce, 0x020037f6, 0x02003816 -> 0x02003bd4
 *                                                  -> Func_080770c0
 *   0x020037da, 0x02003822 -> 0x02003d64 -> Func_0808a5e0
 *   0x020037e0, 0x02003828 -> 0x02003d7c -> Func_080f9010
 *   0x020037e6, 0x0200382e -> 0x02003bdc -> Func_080770c8
 *   0x020037f0, 0x02003838 -> 0x02003864, this overlay's next prologue
 * The 3/4/2/2/2 split matches the tool's per-target counts exactly.
 *
 * No pool word here is an in-image address, so this owner adds no link-base
 * witness of its own; 0x03001e40 is an ordinary IWRAM global that the wider
 * semantic tree already reads as a bitfield word, and the rest are plain
 * constants (0xfffffe83, 0x309, 0x302, 0x202, 0x303, 0x2c5, 0x203).
 *
 * Structure: two near-identical arms, one per follower.  Each arm is gated on
 * a story flag, on the follower's integer x, and on bit 0 of 0x03001e40 - a
 * frame-alternation bit, so each follower is stepped on every other frame -
 * and then hands the follower's current 16.16 position to Func_02003864.  The
 * one-shot inside each arm plays cue 0xe6 and latches its own flag so it fires
 * once.  Follower 8 additionally has a lead-in case: when it is standing in a
 * narrow x window past a z threshold it merely copies the hero's facing bits
 * and does nothing else that frame.
 *
 * UNCERTAINTIES.
 *  - The prologue saves only r5, r6 and lr, yet r4 is used as a scratch
 *    pointer at 0x02003798-0x020037aa.  That is what the bytes say; it is an
 *    ABI irregularity in the original code, not a reading error, and it has no
 *    expression in C.
 *  - Func_0808a5e0(-1) has no other use anywhere in the tree, so its interface
 *    is unknown; -1 is passed as a plain s32.  It is spelled old-style.
 *  - Bits 2-3 of handle->flags09 are the facing bits, taken from the byte-exact
 *    and semantic siblings that mask that field with ~0x0c; the copy here is
 *    the same idiom, just sourced from another entity's handle rather than a
 *    constant.
 */

struct SceneHandle {
    u8 unknown_00[9];
    u8 flags09;                     /* 0x09, bits 2-3 select facing */
};

struct SceneEntity {
    u8 unknown_00[8];
    s32 x;                          /* 0x08, 16.16 */
    s32 y;                          /* 0x0c, 16.16 */
    s32 z;                          /* 0x10, 16.16 */
    u8 unknown_14[0x3c];
    struct SceneHandle *handle;     /* 0x50 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneEntity *Func_0808a080();    /* scene entity by selector */
s32 Func_080770c0();                    /* read a story flag */
void Func_080770c8();                   /* set a story flag */
void Func_0808a5e0();                   /* interface unknown, see above */
void Func_080f9010();                   /* play a sound cue */

/* Intra-overlay callee: takes a 16.16 position. */
void Func_02003864();

#define FRAME_BITS (*(volatile u32 *)0x03001e40)

void Func_02003768(void)
{
    struct SceneEntity *follower8;
    struct SceneEntity *follower9;
    struct SceneEntity *hero;
    s32 tileX;

    follower8 = Func_0808a080(8);
    follower9 = Func_0808a080(9);

    tileX = (s16)(follower8->x >> 16);

    /* `adds r3,#-381 / cmp #12 / bhi` - an unsigned window test, so an x below
     * 381 wraps and falls through to the else arm. */
    if ((u32)(tileX - 381) <= 12 && (s16)(follower8->z >> 16) > 0x309) {
        hero = Func_0808a080(0);
        follower8->handle->flags09 =
            (u8)((follower8->handle->flags09 & ~0x0c) |
                 (hero->handle->flags09 & 0x0c));
    } else if (Func_080770c0(0x302) == 0 &&
               (s16)(follower8->x >> 16) <= 245 &&
               (FRAME_BITS & 1) == 0) {
        if (Func_080770c0(0x202) == 0) {
            Func_0808a5e0(-1);
            Func_080f9010(0xe6);
            Func_080770c8(0x202);
        }
        Func_02003864(follower8->x, follower8->y, follower8->z);
    }

    if (Func_080770c0(0x303) == 0 &&
        (s16)(follower9->x >> 16) <= 0x2c5 &&
        (FRAME_BITS & 1) == 0) {
        if (Func_080770c0(0x203) == 0) {
            Func_0808a5e0(-1);
            Func_080f9010(0xe6);
            Func_080770c8(0x203);
        }
        Func_02003864(follower9->x, follower9->y, follower9->z);
    }
}
