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
 * from tools/overlay_call_targets.ts (target offset = stored displacement + 2),
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
void Func_080000d0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
u8 *Func_0808a080();            /* returns the actor record written below */
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0d0();
void Func_0808a138();
void Func_0808a170();
void Func_0808a1e8();
void Func_0808a1f0();
/* Four register arguments plus two stack words. */
void Func_080091c0();

/* This overlay's own routines; byte-exact sources in assets/code. */
void Func_020045f4(s32, s32);
void Func_0200460c(s32, s32, s32);

/* The scene handler installed at the end, at file offset 0x2ba0. */
void Func_02002ba0(void);

/* In-image actor descriptors at file offsets 0x5830 and 0x5560. */
extern u8 Data_0200d830[];
extern u8 Data_0200d560[];

void Func_02001a4c(void)
{
    u8 *actor24;
    u8 *actor25;

    actor24 = Func_0808a080(24);
    actor25 = Func_0808a080(25);
    Func_0808a018(actor25);

    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);

    Func_0808a0d0(0, 232, 696);         /* 174 << 2 */
    Func_0808a0d0(0, 200, 696);

    Func_0808a010(10);
    Func_0808a1e8(25, 256, 0);          /* 128 << 1 */
    Func_0808a1e8(24, 256, 0);
    Func_0808a010(60);

    Func_0200460c(25, 0, 10);
    Func_0808a138(24, 2);
    Func_0808a010(20);
    Func_0808a170(0x1296);
    Func_020045f4(24, 20);

    Func_0808a1f0(25, 258);             /* 129 << 1 */
    Func_0808a010(60);
    Func_020045f4(25, 20);

    Func_0808a138(24, 1);
    Func_020045f4(24, 30);

    /* 128 << 11 and 128 << 10; 224 << 10 and 224 << 9: 16.16 coordinates. */
    Func_0808a090(24, 0x40000, 0x20000);
    Func_0808a090(25, 0x38000, 0x1c000);

    Func_0808a098(25, Data_0200d830);
    Func_0808a098(24, Data_0200d560);
    Func_0808a0a0(24);

    /* The last two arguments go out on the stack. */
    Func_080091c0(14, 45, 3, 1, 14, 44);

    Func_080770c8(0x852);
    Func_080770c8(768);                 /* 192 << 2 */

    Func_080000d0((void (*)(void))((s32)&Func_02002ba0 | 1), 3200);  /* 200 << 4 */

    /* Halfword animation state at +100 of each actor record. */
    *(s16 *)(actor24 + 100) = 1;
    *(s16 *)(actor25 + 100) = 3;

    Func_0808a020();
}
