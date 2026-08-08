#include "types.h"

/*
 * resource_383 owner at 0x02000690, 288 bytes (0x02000690-0x020007af):
 * code 0x02000690-0x020006e3, a five-word jump table at 0x020006e4, code
 * 0x020006f8-0x020007a5, two alignment bytes at 0x020007a6, and a literal
 * pool at 0x020007a8-0x020007cb (only the first two words fall inside the
 * inventory row; the rest of the pool is shared with the following row).
 *
 * Actor 25's walk-cycle stepper.  It fetches the actor record, reads the
 * orientation nibble from the halfword at +6 and the animation state from the
 * signed halfword at +100, re-dresses the actor, then dispatches on the state
 * to pick the next animation descriptor and the next state value.
 *
 * The dispatch is `mov pc, r3` through the in-image table at 0x020006e4.
 * Under this overlay's proven 0x02008000 link base its five words
 * (0x0200871a, 0x02008742, 0x0200871a, 0x02008742, 0x020086f8) are file
 * offsets 0x71a, 0x742, 0x71a, 0x742, 0x6f8 - all inside this owner - which
 * is the witness that the base applies here too, and is why the 0x0200dxxx /
 * 0x0200exxx pool words below are in-image data rather than RAM globals.
 * `mov pc, rN` does not interwork, so the entries carry no Thumb bit.
 *
 * All 13 `bl` sites are placed and reach 9 distinct import veneers; targets
 * come from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset = stored
 * displacement + 2), never from the disassembler's `bl` annotations, which
 * are wrong for every overlay branch.
 *
 * Epilogue is `pop {r5, r6, r7} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner is void.
 *
 * Uncertainty: the two range tests compare `(orientation - 0x2001)` and
 * `(orientation - 0x6001)` unsigned against 0x7ffe, i.e. they accept the
 * orientation windows 0x2001..0x9fff and 0x6001..0xdfff.  They are written
 * here exactly as the code computes them rather than as a named facing test,
 * because the meaning of the nibble is not established elsewhere in the tree.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_02005412();
void Func_020053f0();
void Func_020054b0();
void Func_020054ce();
void Func_020054e6();
void Func_02005454();
void Func_02005494();
void Func_020054a0();
void Func_020054c2();
void Func_020054ea();
void Func_0200550c();
void Func_0200552e();

                                /* returns the actor record read below */

                     

                     

/* In-image animation descriptors at file offsets 0x58bc and 0x5858, and the
 * descriptor pointer table at file offset 0x64d8. */
extern u8 Data_0200d8bc[];
extern u8 Data_0200d858[];
extern u8 *Data_0200e4d8[];

void Func_02000690(void)
{
    u8 *actor;
    s16 *state;
    u16 orientation;
    s32 half;
    s32 next;

    s32 permuted_7;
    permuted_7 = (u16)(*(u16 *)(actor + 6) & 0xf000);
    actor = Func_02005412(25);
    state = (s16 *)(actor + 100);
    half = *state >> 1;
    orientation  = permuted_7;

    /* Re-establish the actor's appearance for this beat. */
    Func_020053f0(actor);
    Func_020054b0(25, 2);
    Func_020054ce(0x12ad);
    Func_020054e6(25, 0);
    /* 224 << 10 and 224 << 9: 16.16 coordinates, not addresses. */
    Func_02005454(25, 0x38000, 0x1c000);

    next = *state;
    switch ((s32)*state) {
    case 2:
        if ((u32)(orientation - 0x2001) <= 0x7ffe) {
            Func_02005494(25, Data_0200e4d8[(half << 2) + *state]);
            next = (u16)*state - (half << 1) + 1;
            break;
        }
    case 0:
        goto mirrored;

    case 1:
    case 3:
        if ((u32)(orientation - 0x6001) <= 0x7ffe) {
            Func_020054a0(25, Data_0200e4d8[(half << 2) + *state]);
            next = (u16)*state - (half << 1) + 1;
            break;
        }
        goto mirrored;

    case 4:
        if ((u32)(orientation - 0x2001) <= 0x7ffe) {
            Func_020054c2(25, Data_0200d8bc);
            next = 2;
        } else {
            Func_020054ea(25, Data_0200d858);
            next = 3;
        }
        break;

    default:
        /* States above 4 (and, since the compare is unsigned, negative ones)
         * fall straight through to the mask and the hand-off below. */
        goto done;

    mirrored:
        /* Orientation outside the window: take the descriptor from the other
         * half of the table and step the state backwards instead. */
        Func_0200550c(25, Data_0200e4d8[((1 ^ half) << 2) + *state]);
        next = (u16)*state - (half << 1) + 0xffff;
        break;
    }

    *state = (s16)next;

    *state = (s16)(*(u16 *)state & 3);
done:
    Func_0200552e(25);
    Func_020054ea();
}
