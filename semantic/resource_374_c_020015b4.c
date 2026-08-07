#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x020015b4 (532 bytes, 56 call sites).
 *
 * Complete owner and entirely straight-line: prologue
 * `push {r5, r6, lr} ; mov r6, r8 ; push {r6}` at 0x020015b4 and the
 * interworking return `pop {r3} ; mov r8, r3 ; pop {r5, r6} ; pop {r0} ;
 * bx r0` at 0x0200179a.  r0 holds the popped return address, so the owner is
 * `void`.  Bytes 0x020017a4-0x020017c7 are the literal pool (0xffff0000,
 * 0x00001555, 0x03001ebc, 0x00013333, 0x00026666, 0x0000028f, 0x0200aa48,
 * 0x0200ab2c, 0x00000834), reached only by `ldr rN, [pc, #imm]`.  The next
 * prologue at 0x020017c8 is a separate owner.
 *
 * Link base 0x02008000: the pool words 0x0200aa48 and 0x0200ab2c are file
 * offsets 0x2a48 and 0x2b2c.  Both are even, so they are in-image *data*
 * addresses rather than Thumb entry points, and they are passed as the second
 * argument of the two script/table setters Func_0808a098 and Func_0808a0b0.
 * They are named Data_02002a48 / Data_02002b2c accordingly.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
 * stores target_offset - 2, so the disassembler's annotations are unusable).
 * All 56 sites are import veneers; 25 distinct main-image imports.
 *
 * The workspace pointer 0x03001ebc is the one used by the byte-exact
 * `assets/code/resource_374_c_020003b4.c`; this owner writes two of its words,
 * at +448 and +456.
 *
 * Uncertainties:
 *  - Func_0808a010's argument is 80, 20, 24 and 40 at the five sites here.
 *    The small integers 0, 23, 24 and 25 are used elsewhere in this owner as
 *    object handles, so the 24 at 0x02001720 is ambiguous between a handle and
 *    a frame count; it is passed through unchanged;
 *  - the two records fetched by Func_0808a080 are only partially typed: the
 *    fields touched are +0x18, +0x44, +0x48 (words), +0x50 (a pointer whose
 *    halfword at +0x1e is written) and nothing else, so byte offsets are used
 *    rather than a speculative struct.
 */

/* Workspace pointer global. */
extern u8 *Data_03001ebc;

/* In-image data reached under the 0x02008000 link base. */
extern u8 Data_02002a48[];
extern u8 Data_02002b2c[];

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: one name may carry more than one interface. */
void Func_02003e0a();
void Func_02003c5e();
void Func_02003d88();
void Func_02003bc6();
void Func_02003da0();
void Func_02003c24();
void Func_02003be2();
void Func_02003d18();
void Func_02003d26();
void Func_02003d34();
void Func_02003d42();
void Func_02003dc6();
void Func_02003dd2();
void Func_02003d6a();
u8 *Func_02003d18_b();
u8 *Func_02003d22();
void Func_02003cb8();
u8 *Func_02003d36();
u8 *Func_02003d42_b();
void Func_02003cd8();
void Func_02003eac();
void Func_02003ea8();
void Func_02003ec4();
void Func_02003d3a();
void Func_02003e3e();
void Func_02003e4a();
void Func_02003dea();
void Func_02003d60();
void Func_02003df8();
void Func_02003e6c();
void Func_02003e78();
void Func_02003e88();
void Func_02003e90();
u8 *Func_02003dd0();
void Func_02003df2();
void Func_02003db8();
void Func_02003dfa();
u8 *Func_02003df8_b();
void Func_02003e2c();
void Func_02003dda();
void Func_02003f46();
void Func_02003f52();
void Func_02003d40();
void Func_02003f5c();
void Func_02003d4a();
void Func_02003f66();
void Func_02003d54();
void Func_02003f68();
void Func_02003e06();
void Func_02003fa0();
void Func_02003fac();
void Func_02003e12();
void Func_02003f60();
void Func_02003e3c();
                               /* frame wait */

                     

                     
                               /* fetch the object record for a handle */

                     

                     

                     

                     

                     

void Func_020015b4(void)
{
    u8 *workspace;
    u8 *record;

    Func_02003e0a(17);
    Func_02003c5e();

    Func_02003d88(-1, -1, -1, 0);
    Func_02003bc6(1);
    Func_02003da0(0x400000, 0x900000, 0x15e00000, 0);
    Func_02003c24();
    Func_02003be2(1);

    Func_02003d18(0, 0x300000, 0x15a00000);
    Func_02003d26(25, 0x4e0000, 0x16600000);
    Func_02003d34(23, 0x670000, 0x15600000);
    Func_02003d42(24, 0x700000, 0x16800000);

    Func_02003dc6(23, 0x8000, 0);
    Func_02003dd2(24, 0x8000, 0);

    Func_02003d6a(0, 16);
    record = Func_02003d18_b(0);
    *(s32 *)(record + 0x18) = (s32)0xffff0000;
    Func_02003cb8(Func_02003d22(0), 0);

    Func_02003d88(25, 7);
    record = Func_02003d36(25);
    *(u16 *)(*(u8 **)(record + 0x50) + 0x1e) = 0x1555;
    Func_02003cd8(Func_02003d42_b(25), 0);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 256;

    Func_02003eac();
    Func_02003ea8();
    Func_02003ec4();
    Func_02003d3a(80);

    Func_02003e3e(23, 0x3000, 0);
    Func_02003e4a(24, 0xc000, 40);
    Func_02003dea(23, 3);
    Func_02003d60(20);
    Func_02003df8(24, 3);
    Func_02003e6c(23, 0x8000, 10);
    Func_02003e78(24, 0x8000, 10);
    Func_02003e88(0, 3);
    Func_02003e90(25, 3);

    Func_02003dd2(23, 0x00013333, 0x00026666);
    record = Func_02003dd0(23);
    *(s32 *)(record + 0x44) = 0x28f;
    *(s32 *)(record + 0x48) = 0x8000;
    Func_02003df2(23, Data_02002a48);
    Func_02003db8(24);

    Func_02003dfa(24, 0x00013333, 0x00026666);
    record = Func_02003df8_b(24);
    *(s32 *)(record + 0x44) = 0x28f;
    *(s32 *)(record + 0x48) = 0x8000;
    Func_02003e2c(24, Data_02002b2c);
    Func_02003dda(40);

    Func_02003f46();
    Func_02003f52();
    Func_02003d40(20);
    Func_02003f5c();
    Func_02003d4a(60);
    Func_02003f66();
    Func_02003d54(20);
    Func_02003f68();
    Func_02003e06(40);

    *(s32 *)(workspace + 456) = 120;
    workspace = Data_03001ebc;

    Func_02003fa0();
    Func_02003fac();
    Func_02003e12(0x834);
    Func_02003f60(9);
    Func_02003e3c();
}
