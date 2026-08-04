typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_383 owner at 0x02002564, 384 bytes (0x02002564-0x020026e3):
 * code 0x02002564-0x02002583, an eight-word jump table at 0x02002584, code
 * 0x020025a4-0x02002693, and the complete 20-word literal pool at
 * 0x02002694-0x020026e3.
 *
 * Cutscene step table for the two staged actors (24 and 25).  It reads the
 * beat counter - the signed halfword at +364 of the overlay workspace block
 * at *(void **)0x03001ebc - and, for beats 11..18, re-points each actor's
 * animation descriptor by way of this overlay's Func_020026e4.
 *
 * The dispatch is `mov pc, r3` through the in-image table at 0x02002584.
 * Under the overlay's proven 0x02008000 link base its eight words
 * (0x0200a5a4, 0x0200a5b8, 0x0200a5d8, 0x0200a5fe, 0x0200a618, 0x0200a62c,
 * 0x0200a652, 0x0200a678) are file offsets 0x25a4..0x2678, every one inside
 * this owner; that is the witness for the base, and it is why the 0x0200d5xx
 * ..0x0200d8xx pool words are in-image descriptors rather than RAM globals.
 * `mov pc, rN` does not interwork, so the entries carry no Thumb bit.
 *
 * All 17 `bl` sites are placed and every one reaches the same callee, this
 * overlay's own prologue at file offset 0x26e4 - resolved with
 * tools/overlay_call_targets.ts (target offset = stored displacement + 2).
 * The inventory row reports `calls=0`, which is an artefact of the old
 * pc-relative decoding, not a property of the code.  Note that the r3 loaded
 * before each `bl` is that callee's fourth argument, not a `call_via` target:
 * the thunk-bank idiom needs an 0x030001xx IWRAM word, and these are 0x0200d
 * in-image data addresses.
 *
 * Func_020026e4(actor, expected, next, descriptor) fetches the actor record,
 * and only when the state halfword at +100 already equals `expected` does it
 * install `descriptor` and write `next` back - so each case below is a guarded
 * state transition, not an unconditional assignment.
 *
 * Epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.
 */

/* This overlay's guarded state-transition helper at file offset 0x26e4. */
void Func_020026e4(s32 actor, s32 expected, s32 next, const u8 *descriptor);

/* In-image animation descriptors; file offset = address - 0x8000. */
extern u8 Data_0200d538[];
extern u8 Data_0200d560[];
extern u8 Data_0200d5b0[];
extern u8 Data_0200d5d8[];
extern u8 Data_0200d600[];
extern u8 Data_0200d650[];
extern u8 Data_0200d678[];
extern u8 Data_0200d6c8[];
extern u8 Data_0200d6f0[];
extern u8 Data_0200d718[];
extern u8 Data_0200d768[];
extern u8 Data_0200d7a4[];
extern u8 Data_0200d7cc[];
extern u8 Data_0200d808[];
extern u8 Data_0200d830[];
extern u8 Data_0200d858[];
extern u8 Data_0200d894[];
extern u8 Data_0200d8bc[];

void Func_02002564(void)
{
    u8 *workspace;
    s32 beat;
    s32 expected;
    u8 *descriptor;

    workspace = *(u8 **)0x03001ebc;
    /* 182 << 1. */
    beat = *(s16 *)(workspace + 364) - 11;

    /* Unsigned compare, so beats below 11 join the beats above 18 in doing
     * nothing at all. */
    if ((unsigned int)beat > 7) {
        return;
    }

    /* THREE call sites are SHARED between cases: the `b.n` at 0x020025b6
     * takes case 0 into case 2's third call (0x020025f8), the one at
     * 0x020025d6 takes case 1 into case 3's second call (0x02002612), and the
     * one at 0x0200262a takes case 4 into case 6's third call (0x02002672).
     * Seventeen sites, not twenty. */
    switch (beat) {
    case 0:
        Func_020026e4(24, 1, 2, Data_0200d5b0);
        expected = 3;
        descriptor = Data_0200d8bc;
        goto next4;

    case 1:
        Func_020026e4(24, 1, 4, Data_0200d678);
        Func_020026e4(24, 2, 3, Data_0200d5d8);
        expected = 1;
        descriptor = Data_0200d830;
        goto next3;

    case 2:
        Func_020026e4(24, 2, 1, Data_0200d538);
        Func_020026e4(24, 3, 6, Data_0200d718);
        expected = 2;
        descriptor = Data_0200d894;
    next4:
        Func_020026e4(25, expected, 4, descriptor);
        break;

    case 3:
        Func_020026e4(24, 3, 2, Data_0200d5b0);
        expected = 4;
        descriptor = Data_0200d858;
    next3:
        Func_020026e4(25, expected, 3, descriptor);
        break;

    case 4:
        Func_020026e4(24, 4, 5, Data_0200d6c8);
        expected = 1;
        descriptor = Data_0200d7cc;
        goto next2;

    case 5:
        Func_020026e4(24, 4, 1, Data_0200d560);
        Func_020026e4(24, 5, 6, Data_0200d6f0);
        Func_020026e4(25, 3, 1, Data_0200d7a4);
        break;

    case 6:
        Func_020026e4(24, 5, 4, Data_0200d650);
        Func_020026e4(24, 6, 3, Data_0200d600);
        expected = 4;
        descriptor = Data_0200d808;
    next2:
        Func_020026e4(25, expected, 2, descriptor);
        break;

    case 7:
        Func_020026e4(24, 6, 5, Data_0200d6c8);
        Func_020026e4(25, 2, 1, Data_0200d768);
        break;
    }
}
