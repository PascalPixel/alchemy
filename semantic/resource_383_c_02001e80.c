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
 * resolved with tools/overlay_call_targets.ts (target offset = stored
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
void Func_0808a010();
u8 *Func_0808a080();            /* returns the actor record written below */
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a110();
void Func_0808a138();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a1f0();
/* Four register arguments plus two stack words. */
void Func_080091c0();

/* This overlay's own routine at file offset 0x45f4 (byte-exact source in
 * assets/code/resource_383_c_020045f4.c). */
void Func_020045f4(s32, s32);

/* In-image descriptors at file offsets 0x5830 and 0x5560. */
extern u8 Data_0200d830[];
extern u8 Data_0200d560[];

void Func_02001e80(void)
{
    u8 *actor;

    Func_0808a010(30);
    Func_0808a138(24, 1);
    Func_0808a010(20);
    Func_0808a170(0x12a0);
    Func_020045f4(24, 20);

    Func_0808a1b8(25, 0, 20);
    Func_0808a1f0(25, 258);         /* 129 << 1 */
    Func_0808a138(25, 2);
    Func_020045f4(25, 20);

    Func_0808a110(24, 4);
    Func_0808a010(20);
    Func_020045f4(24, 20);

    /* 128 << 11 and 128 << 10; 224 << 10 and 224 << 9.  These are 16.16
     * coordinates (0x40000 = 4.0, 0x20000 = 2.0, and so on), not addresses. */
    Func_0808a090(24, 0x40000, 0x20000);
    Func_0808a090(25, 0x38000, 0x1c000);

    Func_0808a098(25, Data_0200d830);
    Func_0808a098(24, Data_0200d560);
    Func_0808a0a0(24);

    /* Halfword field at +100 of the actor record. */
    actor = Func_0808a080(24);
    actor = Func_0808a080(25);
    *(s16 *)(actor + 100) = 3;
    *(s16 *)(actor + 100) = 1;

    /* The last two arguments go out on the stack. */
    Func_080091c0(14, 48, 4, 1, 14, 44);
}
