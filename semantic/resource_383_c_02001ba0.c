#include "types.h"

/*
 * resource_383 owner at 0x02001ba0, 708 bytes (0x02001ba0-0x02001e63):
 * code 0x02001ba0-0x02001e3d, two alignment bytes at 0x02001e3e and a
 * nine-word literal pool at 0x02001e40.
 *
 * The scene's closing beat.  It picks each actor's outfit from the animation
 * state halfword at +100 of its record, re-installs the follow-up handler
 * Func_02002ba0, replays the camera and dialogue sequence (lines 0x1299,
 * 0x129d and 0x129f), bumps the workspace counter at +472 when Func_0808a070
 * reports the optional branch was taken, and finishes by posting the
 * six-argument message and parking both actors.
 *
 * All 64 `bl` sites are placed and reach 26 distinct callees - the inventory
 * row's `calls=62` predates the corrected decoding.  Targets come from
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  Fifty-one reach the import veneer table at 0x02004cxx-
 * 0x02004f2b and are named by the main-image address in the veneer's trailing
 * word; thirteen reach this overlay's own Func_020045f4, Func_0200460c,
 * Func_02004624 and Func_0200463c, all byte-exact in assets/code.
 *
 * Link-base witness: the pool word 0x0200aba1 is file offset 0x2ba0 plus the
 * Thumb bit - this overlay's Func_02002ba0, the same handler Func_02001a4c
 * installs - and 0x0200d678 / 0x0200d650 / 0x0200d768 are in-image outfit
 * descriptors at file offsets 0x5678, 0x5650 and 0x5768.
 *
 * Note that r5 is reused: it holds the actor-24 state pointer for the two
 * outfit tests and is then reloaded with the literal 0x1299, from which the
 * second line id is formed as 0x1299 + 4.
 *
 * Epilogue is `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the
 * popped return address and the owner is void.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_02006924();
u8 *Func_0200692c();
void Func_020068fa();
void Func_02006838();
void Func_020068ea();
void Func_02006962();
void Func_0200696c();
void Func_02006980();
void Func_0200698a();
void Func_0200698c();
void Func_02006996();
void Func_020069a0();
void Func_020069dc();
void Func_02006a02();
void Func_02006a10();
void Func_020069fe();
void Func_02006a12();
void Func_02006a28();
void Func_02006a9c();
void Func_02006a7e();
void Func_02006290(s32, s32, s32);
void Func_02006a88();
void Func_02006aa8();
void Func_02006ac0();
void Func_02006aaa();
void Func_020062bc(s32, s32, s32);
void Func_02006a8c();
void Func_020062fe(s32, s32, s32);
void Func_020062f0(s32, s32, s32);
void Func_02006af0();
s32 Func_02006a50();
void Func_020062ec(s32, s32);
void Func_02006b12();
void Func_02006b0c();
void Func_0200631e(s32, s32, s32);
void Func_02006b5a();
void Func_02006a60();
void Func_02006352(s32, s32, s32);
void Func_02006b3c();
void Func_0200634e(s32, s32, s32);
void Func_02006388(s32, s32, s32);
void Func_02006348(s32, s32);
void Func_02006b9c();
void Func_02006ba8();
void Func_02006aae();
void Func_020063b8(s32, s32, s32);
void Func_02006378(s32, s32);
void Func_02006b9e();
void Func_02006bb6();
void Func_02006bca();
void Func_02006ae0();
void Func_02006b80();
void Func_02006b88();
void Func_020063fa(s32, s32, s32);
void Func_02006b76();
void Func_02006b8a();
void Func_02006bac();
void Func_02006bc4();
void Func_02006bd2();
void Func_02006c3c();
void Func_02006c48();
void Func_02006b14();

                     

                    
                                /* returns the actor record */

                     

                     

                     

                     

/* Four register arguments plus two stack words. */

/* This overlay's own routines; byte-exact sources in assets/code. */

                                  

/* The handler installed below, with its Thumb bit set. */
void Func_02002ba0(void);

/* In-image outfit descriptors at file offsets 0x5678, 0x5650 and 0x5768. */
extern u8 Data_0200d678[];
extern u8 Data_0200d650[];
extern u8 Data_0200d768[];

void Func_02001ba0(void)
{
    u8 *actor24;
    u8 *actor25;
    u8 *workspace;
    s32 line;

    actor24 = Func_02006924(24);
    actor25 = Func_0200692c(25);
    Func_020068fa(actor25);

    Func_02006838((void (*)(void))((s32)&Func_02002ba0 | 1));
    Func_020068ea(768);                 /* 192 << 2 */

    if (*(s16 *)(actor24 + 100) <= 3) {
        Func_02006962(24, Data_0200d678);
    } else {
        Func_0200696c(24, Data_0200d650);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        Func_02006980(25, Data_0200d768);
    } else {
        Func_0200698a(25, Data_0200d650);
    }

    Func_0200698c(0, 0xcccc, 0x6666);
    Func_02006996(1, 0xcccc, 0x6666);
    Func_020069a0(2, 0xcccc, 0x6666);

    Func_020069dc(0, 248, 728);         /* 182 << 2 */
    Func_02006a02(2, 0xf80000, 0x2d80000);      /* 248 << 16, 182 << 18 */
    Func_02006a10(1, 0xf80000, 0x2d80000);
    Func_020069fe(2, 264, 744);         /* 132 << 1, 186 << 2 */
    Func_02006a12(1, 232, 744);
    Func_02006a28(2);

    Func_02006a9c(0, 0x4000, 0);        /* 128 << 7 */
    Func_02006a7e(1, 0, 0);
    Func_02006290(2, 0, 30);
    Func_02006a88(2, 1);

    line = 0x1299;
    Func_02006aa8(line);
    Func_02006ac0(2, 0);
    Func_02006aaa(0, 2, 0);
    Func_020062bc(1, 2, 20);
    Func_02006a8c(0, 3);
    Func_020062fe(1, 3, 20);
    Func_020062f0(0, 1, 10);
    Func_02006af0(1, 0);

    if (Func_02006a50(0, 0) != 0) {
        /* 236 << 1: the same workspace block whose +456 word the other
         * cutscene steps in this overlay publish. */
        workspace = *(u8 **)0x03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
    }

    Func_020062ec(1, 30);
    Func_02006b12(line + 4);
    Func_02006b0c(0, 2, 0);
    Func_0200631e(1, 2, 50);
    Func_02006b5a(2, 256, 0);           /* 128 << 1 */
    Func_02006a60(60);

    Func_02006352(0, 1, 50);
    Func_02006b3c(0, 2, 0);
    Func_0200634e(1, 2, 30);
    Func_02006388(2, 3, 10);
    Func_02006348(2, 20);
    Func_02006b9c(0, 258, 0);           /* 129 << 1 */
    Func_02006ba8(1, 258, 0);
    Func_02006aae(60);

    Func_020063b8(2, 3, 20);
    Func_02006378(2, 30);
    Func_02006b9e(0x129f);
    Func_02006bb6(1, 0);
    Func_02006bca(0, 0x4000, 0);
    Func_02006ae0(40);

    Func_02006b80(0, 3);
    Func_02006b88(1, 3);
    Func_020063fa(2, 3, 50);
    Func_02006b76(2, 248, 728);
    Func_02006b8a(1, 248, 728);
    Func_02006bac(1, 0, 0);
    Func_02006bb6(2, 0, 0);

    /* 208 << 15 and 240 << 15 against 174 << 18: 16.16 coordinates. */
    Func_02006bc4(24, 0x680000, 0x2b80000);
    Func_02006bd2(25, 0x780000, 0x2b80000);
    Func_02006c3c(24, 0, 0);
    Func_02006c48(25, 0x8000, 0);       /* 128 << 8 */

    /* The last two arguments go out on the stack. */
    Func_02006b14(14, 50, 3, 1, 14, 44);
    Func_02006b80();
}
