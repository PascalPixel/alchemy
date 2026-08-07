#include "types.h"

/*
 * resource_383 owner at 0x02001e80, 208 bytes (0x02001e80-0x02001f4f):
 * code 0x02001e80-0x02001f43 and a three-word literal pool at 0x02001f44
 * (0x000012a0, 0x0200d830, 0x0200d560).
 *
 * Cutscene staging step: two actors (24 and 25) are created, dressed, placed
 * and given their scripts, then a six-argument message is posted.
 *
 * All 20 `bl` sites are placed; they reach 12 distinct callees.  Targets are
 * resolved with tools/lib/overlay_call_targets.ts (target offset = stored
 * displacement + 2), not from the disassembler's annotations, which are wrong
 * for every overlay `bl`.  Seventeen sites reach the import veneer table at
 * 0x02004cxx-0x02004f2b and are named by the main-image address in the
 * veneer's trailing word; the other three reach this overlay's own
 * Func_020045f4, whose byte-exact source is in assets/code.
 *
 * The two 0x0200dxxx pool words are in-image data under the overlay's
 * 0x02008000 link base (file offsets 0x5830 and 0x5560) - the same base that
 * 0x02004754's Data_0200cf2c witnesses and that the byte-exact
 * resource_383_c_02004704.c already relies on.  They are the two actors'
 * script/animation descriptors.
 *
 * Epilogue is `add sp, #8 / pop {r0} / bx r0`, so the owner is void.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_02006bc4();
void Func_02006c8c();
void Func_02006bd2();
void Func_02006cb0();
void Func_02006498(s32, s32);
void Func_02006cda();
void Func_02006cfc();
void Func_02006cbc();
void Func_020064bc(s32, s32);
void Func_02006cac();
void Func_02006c12();
void Func_020064d2(s32, s32);
void Func_02006c70();
void Func_02006c7e();
void Func_02006c8e();
void Func_02006c96();
void Func_02006ca4();
u8 *Func_02006c92();
u8 *Func_02006c9e();
void Func_02006c20();
                     
                                /* returns the actor record written below */

                     

                     

/* Four register arguments plus two stack words. */

/* This overlay's own routine at file offset 0x45f4 (byte-exact source in
 * assets/code/resource_383_c_020045f4.c). */

/* In-image descriptors at file offsets 0x5830 and 0x5560. */
extern u8 Data_0200d830[];
extern u8 Data_0200d560[];

void Func_02001e80(void)
{
    u8 *actor;

    Func_02006bc4(30);
    Func_02006c8c(24, 1);
    Func_02006bd2(20);
    Func_02006cb0(0x12a0);
    Func_02006498(24, 20);

    Func_02006cda(25, 0, 20);
    Func_02006cfc(25, 258);         /* 129 << 1 */
    Func_02006cbc(25, 2);
    Func_020064bc(25, 20);

    Func_02006cac(24, 4);
    Func_02006c12(20);
    Func_020064d2(24, 20);

    /* 128 << 11 and 128 << 10; 224 << 10 and 224 << 9.  These are 16.16
     * coordinates (0x40000 = 4.0, 0x20000 = 2.0, and so on), not addresses. */
    Func_02006c70(24, 0x40000, 0x20000);
    Func_02006c7e(25, 0x38000, 0x1c000);

    Func_02006c8e(25, Data_0200d830);
    Func_02006c96(24, Data_0200d560);
    Func_02006ca4(24);

    /* Halfword field at +100 of the actor record. */
    actor = Func_02006c92(24);
    actor = Func_02006c9e(25);
    *(s16 *)(actor + 100) = 3;
    *(s16 *)(actor + 100) = 1;

    /* The last two arguments go out on the stack. */
    Func_02006c20(14, 48, 4, 1, 14, 44);
}
