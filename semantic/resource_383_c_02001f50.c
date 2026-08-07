#include "types.h"

/*
 * resource_383 owner at 0x02001f50, 632 bytes (0x02001f50-0x020021c7):
 * code 0x02001f50-0x0200219f and a ten-word literal pool at 0x020021a0.
 *
 * The scene's second act.  It re-installs the per-frame follower handler
 * Func_020048c8, restarts the two sound streams, wakes the five participants
 * (0, 1, 2, 24 and 25), then plays the beat selected by the workspace
 * halfword at +386 - the same halfword Func_020048c8 writes when the
 * followers stay blocked, and whose values 201..203 pick which of the two
 * actors speaks.  Afterwards it clears bits 2-3 of the +9 flag byte inside
 * each participant's +80 sub-record (leaving bit 2 set), publishes 24 and
 * 0x201 into the workspace, posts the six-argument message, seeds the two
 * actors' animation states, installs the follow-up handler Func_02002ba0 and
 * finally publishes 0x209.
 *
 * All 61 `bl` sites are placed and reach 25 distinct callees - the inventory
 * row's `calls=58` predates the corrected decoding.  Targets come from
 * tools/lib/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  Fifty-one reach the import veneer table at 0x02004cxx-
 * 0x02004f2b and are named by the main-image address in the veneer's trailing
 * word; ten reach this overlay's own Func_020045f4 (seven sites),
 * Func_02004658, Func_02004684 and Func_020021c8.
 *
 * Link-base witness: the pool words 0x0200c8c9 and 0x0200aba1 are file
 * offsets 0x48c8 and 0x2ba0 plus the Thumb bit, i.e. this overlay's own
 * Func_020048c8 and Func_02002ba0 - both installed as handlers here.
 *
 * Epilogue is `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the
 * popped return address and the owner is void.
 *
 * Uncertainty: Func_0808a480, Func_0808a368, Func_0808a370 and
 * Func_02004684 are all reached with no argument register set up by this
 * owner, so they are treated as taking none.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_080000d0();
void Func_080000d8();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
u8 *Func_0808a080();            /* returns the participant record */
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a368();
void Func_0808a370();
void Func_0808a480();
/* Four register arguments plus two stack words. */
void Func_080091c0();

/* This overlay's own routines; byte-exact sources in assets/code for 0x45f4,
 * 0x4658 and 0x4684. */
void Func_020045f4(s32, s32);
void Func_02004658(s32, s32);
void Func_02004684(void);
void Func_020021c8(void);

/* Handlers installed below, with their Thumb bits set. */
void Func_020048c8(void);
void Func_02002ba0(void);

void Func_02001f50(void)
{
    u8 *workspace;
    s16 *beat;
    u8 *record;
    u8 *flags;
    s32 speaker;

    s32 permuted_71;
    workspace = *(u8 **)0x03001ebc;

    Func_0808a018();
    Func_080000d8((void (*)(void))((s32)&Func_020048c8 | 1));
    Func_080770d0(0x107);
    Func_080770d0(592);                 /* 148 << 2 */

    Func_0808a100(24, 1);
    Func_0808a100(25, 1);
    Func_0808a100(2, 1);
    Func_0808a100(0, 1);
    Func_0808a100(1, 1);

    Func_0808a148(1, 2, 0);
    Func_0808a150(0, 2, 0);
    Func_0808a148(24, 2, 0);
    Func_0808a148(25, 2, 0);
    Func_0808a010(10);

    /* 193 << 1. */
    beat = (s16 *)(workspace + 386);
    speaker = 24;

    /* Beat 201: actor 24 speaks.  Beat 202: actor 25 speaks.  Beat 203: both,
     * 25 first.  Any other value: nobody speaks and the speaker stays 24.
     * The halfword is re-read between the two speeches, as the assembly does. */
    if (*beat >= 201 && *beat <= 203) {
        if (*beat != 201) {
            Func_0808a170(0x12a4);
            Func_0808a1f0(25, 258);     /* 129 << 1 */
            Func_0808a138(25, 2);
            Func_020045f4(25, 20);
            speaker = 25;
        }
        if (*beat != 202) {
            Func_0808a170(0x12a3);
            Func_0808a1f0(24, 258);
            Func_0808a138(24, 2);
            speaker = 24;
            Func_020045f4(24, 20);
        }
    }

    Func_0808a100(2, 1);
    Func_0808a148(2, speaker, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a170(0x12a5);
    Func_020045f4(1, 20);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_020045f4(2, 20);
    Func_02004658(2, speaker);
    Func_0808a480();

    Func_0808a010(60);
    Func_0808a138(24, 2);
    Func_020045f4(24, 20);
    Func_0808a138(25, 2);
    Func_020045f4(25, 20);
    Func_0808a1e8(2, 256, 60);          /* 128 << 1 */
    Func_02004684();

    Func_0808a130(1, 2);
    Func_0808a138(0, 2);
    Func_0808a010(20);
    Func_020045f4(1, 20);
    Func_0808a148(2, 1, 0);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(40);

    /* Clear bits 2 and 3 of the +9 flag byte in each participant's +80
     * sub-record and set bit 2. */
    record = Func_0808a080(0);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    flags = *(u8 **)(record + 80);

    record = Func_0808a080(1);
    flags = *(u8 **)(record + 80);
    flags[9]  = permuted_71;
    permuted_71 = (u8)((flags[9] & ~12) | 4);

    record = Func_0808a080(2);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    flags = *(u8 **)(record + 80);

    /* 228 << 1 and 224 << 1.  The workspace pointer is re-read here, exactly
     * as the assembly does. */
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 456) = 24;
    *(s32 *)(workspace + 448) = 0x201;

    Func_0808a368();
    Func_0808a370();
    Func_020021c8();

    /* The last two arguments go out on the stack. */
    Func_080091c0(14, 45, 3, 1, 14, 44);
    Func_080770c8(0x853);

    record = Func_0808a080(24);
    *(s16 *)(record + 100) = 5;
    *(s16 *)(record + 100) = 4;
    record = Func_0808a080(25);

    Func_080000d0((void (*)(void))((s32)&Func_02002ba0 | 1), 3200);  /* 200 << 4 */

    *(s32 *)(workspace + 448) = 0x209;
    workspace = *(u8 **)0x03001ebc;

    Func_0808a020();
}
