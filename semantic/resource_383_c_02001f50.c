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
void Func_02006c9e();
void Func_02006bdc();
void Func_02006c8a();
void Func_02006c92();
void Func_02006d4a();
void Func_02006d52();
void Func_02006d5a();
void Func_02006d62();
void Func_02006d6a();
void Func_02006da4();
void Func_02006db6();
void Func_02006db8();
void Func_02006dc2();
void Func_02006d00();
void Func_02006df6();
void Func_02006e30();
void Func_02006df0();
void Func_020065f0(s32, s32);
void Func_02006e20();
void Func_02006e5a();
void Func_02006e1a();
void Func_0200661c(s32, s32);
void Func_02006e04();
void Func_02006e3e();
void Func_02006e48();
void Func_02006d8e();
void Func_02006e6c();
void Func_02006654(s32, s32);
void Func_02006e64();
void Func_02006daa();
void Func_0200666a(s32, s32);
void Func_020066d6(s32, s32);
void Func_02006f7e();
void Func_02006dc4();
void Func_02006e8c();
void Func_0200668c(s32, s32);
void Func_02006e9c();
void Func_0200669c(s32, s32);
void Func_02006ef0();
void Func_0200673c(void);
void Func_02006eb4();
void Func_02006ec4();
void Func_02006e0a();
void Func_020066ca(s32, s32);
void Func_02006ee4();
void Func_02006e22();
void Func_02006eca();
u8 *Func_02006e76();
u8 *Func_02006e8e();
u8 *Func_02006ea0();
void Func_02006fec();
void Func_02006ff8();
void Func_02004318(void);
void Func_02006e86();
u8 *Func_02006eec();
u8 *Func_02006ef8();
void Func_02006e00();
void Func_02006ee2();

                     

                     
                     
                                /* returns the participant record */

                     

                     

                     

                     
/* Four register arguments plus two stack words. */

/* This overlay's own routines; byte-exact sources in assets/code for 0x45f4,
 * 0x4658 and 0x4684. */

                         

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

    Func_02006c9e();
    Func_02006bdc((void (*)(void))((s32)&Func_020048c8 | 1));
    Func_02006c8a(0x107);
    Func_02006c92(592);                 /* 148 << 2 */

    Func_02006d4a(24, 1);
    Func_02006d52(25, 1);
    Func_02006d5a(2, 1);
    Func_02006d62(0, 1);
    Func_02006d6a(1, 1);

    Func_02006da4(1, 2, 0);
    Func_02006db6(0, 2, 0);
    Func_02006db8(24, 2, 0);
    Func_02006dc2(25, 2, 0);
    Func_02006d00(10);

    /* 193 << 1. */
    beat = (s16 *)(workspace + 386);
    speaker = 24;

    /* Beat 201: actor 24 speaks.  Beat 202: actor 25 speaks.  Beat 203: both,
     * 25 first.  Any other value: nobody speaks and the speaker stays 24.
     * The halfword is re-read between the two speeches, as the assembly does. */
    if (*beat >= 201 && *beat <= 203) {
        if (*beat != 201) {
            Func_02006df6(0x12a4);
            Func_02006e30(25, 258);     /* 129 << 1 */
            Func_02006df0(25, 2);
            Func_020065f0(25, 20);
            speaker = 25;
        }
        if (*beat != 202) {
            Func_02006e20(0x12a3);
            Func_02006e5a(24, 258);
            Func_02006e1a(24, 2);
            speaker = 24;
            Func_0200661c(24, 20);
        }
    }

    Func_02006e04(2, 1);
    Func_02006e3e(2, speaker, 0);
    Func_02006e48(1, 2, 0);
    Func_02006e48(1, 2);
    Func_02006d8e(20);
    Func_02006e6c(0x12a5);
    Func_02006654(1, 20);
    Func_02006e64(2, 2);
    Func_02006daa(20);
    Func_0200666a(2, 20);
    Func_020066d6(2, speaker);
    Func_02006f7e();

    Func_02006dc4(60);
    Func_02006e8c(24, 2);
    Func_0200668c(24, 20);
    Func_02006e9c(25, 2);
    Func_0200669c(25, 20);
    Func_02006ef0(2, 256, 60);          /* 128 << 1 */
    Func_0200673c();

    Func_02006eb4(1, 2);
    Func_02006ec4(0, 2);
    Func_02006e0a(20);
    Func_020066ca(1, 20);
    Func_02006ee4(2, 1, 0);
    Func_02006e22(20);
    Func_02006eca(2, 3);
    Func_02006e30(40);

    /* Clear bits 2 and 3 of the +9 flag byte in each participant's +80
     * sub-record and set bit 2. */
    record = Func_02006e76(0);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    flags = *(u8 **)(record + 80);

    record = Func_02006e8e(1);
    flags = *(u8 **)(record + 80);
    flags[9]  = permuted_71;
    permuted_71 = (u8)((flags[9] & ~12) | 4);

    record = Func_02006ea0(2);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    flags = *(u8 **)(record + 80);

    /* 228 << 1 and 224 << 1.  The workspace pointer is re-read here, exactly
     * as the assembly does. */
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 456) = 24;
    *(s32 *)(workspace + 448) = 0x201;

    Func_02006fec();
    Func_02006ff8();
    Func_02004318();

    /* The last two arguments go out on the stack. */
    Func_02006e48(14, 45, 3, 1, 14, 44);
    Func_02006e86(0x853);

    record = Func_02006eec(24);
    *(s16 *)(record + 100) = 5;
    *(s16 *)(record + 100) = 4;
    record = Func_02006ef8(25);

    Func_02006e00((void (*)(void))((s32)&Func_02002ba0 | 1), 3200);  /* 200 << 4 */

    *(s32 *)(workspace + 448) = 0x209;
    workspace = *(u8 **)0x03001ebc;

    Func_02006ee2();
}
