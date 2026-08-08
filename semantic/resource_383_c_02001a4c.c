#include "types.h"

/*
 * resource_383 owner at 0x02001a4c, 340 bytes (0x02001a4c-0x02001b9f):
 * code 0x02001a4c-0x02001b81, two alignment bytes at 0x02001b82 and a
 * seven-word literal pool at 0x02001b84.
 *
 * A cutscene staging step of the same family as the byte-exact sources in
 * this overlay and the semantic Func_02001e80: the two actor records (24 and
 * 25) are fetched up front, the three background layers are reprogrammed,
 * both actors are dressed, placed and scripted, a six-argument message is
 * posted, two sound effects are started, the next scene handler
 * Func_02002ba0 is installed, and finally each actor's animation-state
 * halfword at +100 is seeded (24 -> 1, 25 -> 3).
 *
 * All 32 `bl` sites are placed and reach 17 distinct callees.  Targets come
 * from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  Twenty-eight reach the import veneer table at 0x02004cxx-
 * 0x02004f2b and are named by the main-image address in the veneer's trailing
 * word; four reach this overlay's own Func_020045f4 (three sites) and
 * Func_0200460c, both of which have byte-exact sources in assets/code.
 *
 * Link-base witness: the pool word 0x0200aba1 handed to Func_080000d0 is file
 * offset 0x2ba0 plus the Thumb bit - a function pointer into this overlay -
 * and 0x0200d830 / 0x0200d560 are the in-image actor descriptors at file
 * offsets 0x5830 / 0x5560 that Func_02001e80 already uses.  Both confirm the
 * 0x02008000 link base.
 *
 * Epilogue is `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the
 * popped return address and the owner is void.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_020067d0();
u8 *Func_020067d8();
void Func_020067a6();
void Func_020067f0();
void Func_020067fa();
void Func_02006804();
void Func_02006840();
void Func_0200684c();
void Func_020067da();
void Func_020068e6();
void Func_020068f2();
void Func_020067f8();
void Func_020060d2(s32, s32, s32);
void Func_020068ca();
void Func_02006810();
void Func_020068ee();
void Func_020060d6(s32, s32);
void Func_02006930();
void Func_0200682e();
void Func_020060ee(s32, s32);
void Func_020068fe();
void Func_020060fe(s32, s32);
void Func_0200689c();
void Func_020068aa();
void Func_020068ba();
void Func_020068c2();
void Func_020068d0();
void Func_02006834();
void Func_02006872();
void Func_0200687a();
void Func_020067dc();
void Func_020068c4();

                     

                                /* returns the actor record written below */

                     

                     

/* Four register arguments plus two stack words. */

/* This overlay's own routines; byte-exact sources in assets/code. */

/* The scene handler installed at the end, at file offset 0x2ba0. */
void Func_02002ba0(void);

/* In-image actor descriptors at file offsets 0x5830 and 0x5560. */
extern u8 Data_0200d830[];
extern u8 Data_0200d560[];

void Func_02001a4c(void)
{
    u8 *actor24;
    u8 *actor25;

    actor24 = Func_020067d0(24);
    actor25 = Func_020067d8(25);
    Func_020067a6(actor25);

    Func_020067f0(0, 0xcccc, 0x6666);
    Func_020067fa(1, 0xcccc, 0x6666);
    Func_02006804(2, 0xcccc, 0x6666);

    Func_02006840(0, 232, 696);         /* 174 << 2 */
    Func_0200684c(0, 200, 696);

    Func_020067da(10);
    Func_020068e6(25, 256, 0);          /* 128 << 1 */
    Func_020068f2(24, 256, 0);
    Func_020067f8(60);

    Func_020060d2(25, 0, 10);
    Func_020068ca(24, 2);
    Func_02006810(20);
    Func_020068ee(0x1296);
    Func_020060d6(24, 20);

    Func_02006930(25, 258);             /* 129 << 1 */
    Func_0200682e(60);
    Func_020060ee(25, 20);

    Func_020068fe(24, 1);
    Func_020060fe(24, 30);

    /* 128 << 11 and 128 << 10; 224 << 10 and 224 << 9: 16.16 coordinates. */
    Func_0200689c(24, 0x40000, 0x20000);
    Func_020068aa(25, 0x38000, 0x1c000);

    Func_020068ba(25, Data_0200d830);
    Func_020068c2(24, Data_0200d560);
    Func_020068d0(24);

    /* The last two arguments go out on the stack. */
    Func_02006834(14, 45, 3, 1, 14, 44);

    Func_02006872(0x852);
    Func_0200687a(768);                 /* 192 << 2 */

    Func_020067dc((void (*)(void))((s32)&Func_02002ba0 | 1), 3200);  /* 200 << 4 */

    /* Halfword animation state at +100 of each actor record. */
    *(s16 *)(actor24 + 100) = 1;
    *(s16 *)(actor25 + 100) = 3;

    Func_020068c4();
}
