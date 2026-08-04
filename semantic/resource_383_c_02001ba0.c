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
 * tools/overlay_call_targets.ts (target offset = stored displacement + 2),
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
void Func_080000d8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();            /* returns the actor record */
void Func_0808a090();
void Func_0808a098();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a138();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
/* Four register arguments plus two stack words. */
void Func_080091c0();

/* This overlay's own routines; byte-exact sources in assets/code. */
void Func_020045f4(s32, s32);
void Func_0200460c(s32, s32, s32);
void Func_02004624(s32, s32, s32);
void Func_0200463c(s32, s32, s32);

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

    actor24 = Func_0808a080(24);
    actor25 = Func_0808a080(25);
    Func_0808a018(actor25);

    Func_080000d8((void (*)(void))((s32)&Func_02002ba0 | 1));
    Func_080770d0(768);                 /* 192 << 2 */

    if (*(s16 *)(actor24 + 100) <= 3) {
        Func_0808a098(24, Data_0200d678);
    } else {
        Func_0808a098(24, Data_0200d650);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        Func_0808a098(25, Data_0200d768);
    } else {
        Func_0808a098(25, Data_0200d650);
    }

    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);

    Func_0808a0d0(0, 248, 728);         /* 182 << 2 */
    Func_0808a0f0(2, 0xf80000, 0x2d80000);      /* 248 << 16, 182 << 18 */
    Func_0808a0f0(1, 0xf80000, 0x2d80000);
    Func_0808a0c8(2, 264, 744);         /* 132 << 1, 186 << 2 */
    Func_0808a0d0(1, 232, 744);
    Func_0808a0e8(2);

    Func_0808a1b8(0, 0x4000, 0);        /* 128 << 7 */
    Func_0808a148(1, 0, 0);
    Func_0200460c(2, 0, 30);
    Func_0808a138(2, 1);

    line = 0x1299;
    Func_0808a170(line);
    Func_0808a180(2, 0);
    Func_0808a148(0, 2, 0);
    Func_0200460c(1, 2, 20);
    Func_0808a100(0, 3);
    Func_0200463c(1, 3, 20);
    Func_02004624(0, 1, 10);
    Func_0808a178(1, 0);

    if (Func_0808a070(0, 0) != 0) {
        /* 236 << 1: the same workspace block whose +456 word the other
         * cutscene steps in this overlay publish. */
        workspace = *(u8 **)0x03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
    }

    Func_020045f4(1, 30);
    Func_0808a170(line + 4);
    Func_0808a148(0, 2, 0);
    Func_0200460c(1, 2, 50);
    Func_0808a1e8(2, 256, 0);           /* 128 << 1 */
    Func_0808a010(60);

    Func_02004624(0, 1, 50);
    Func_0808a148(0, 2, 0);
    Func_0200460c(1, 2, 30);
    Func_0200463c(2, 3, 10);
    Func_020045f4(2, 20);
    Func_0808a1e8(0, 258, 0);           /* 129 << 1 */
    Func_0808a1e8(1, 258, 0);
    Func_0808a010(60);

    Func_0200463c(2, 3, 20);
    Func_020045f4(2, 30);
    Func_0808a170(0x129f);
    Func_0808a180(1, 0);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a010(40);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0200463c(2, 3, 50);
    Func_0808a0c8(2, 248, 728);
    Func_0808a0d0(1, 248, 728);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);

    /* 208 << 15 and 240 << 15 against 174 << 18: 16.16 coordinates. */
    Func_0808a0f0(24, 0x680000, 0x2b80000);
    Func_0808a0f0(25, 0x780000, 0x2b80000);
    Func_0808a1b8(24, 0, 0);
    Func_0808a1b8(25, 0x8000, 0);       /* 128 << 8 */

    /* The last two arguments go out on the stack. */
    Func_080091c0(14, 50, 3, 1, 14, 44);
    Func_0808a020();
}
