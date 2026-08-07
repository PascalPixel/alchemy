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
void Func_080000c0();          /* frame wait */
void Func_08009128();
void Func_080091e0();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
u8 *Func_0808a080();           /* fetch the object record for a handle */
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a1b8();
void Func_0808a1e0();
void Func_0808a210();
void Func_0808a248();
void Func_0808a2d0();
void Func_0808a2d8();
void Func_0808a2e0();
void Func_0808a360();
void Func_0808a368();
void Func_0808a370();
void Func_080f9010();

void Func_020015b4(void)
{
    u8 *workspace;
    u8 *record;

    Func_080f9010(17);
    Func_0808a018();

    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a210(0x400000, 0x900000, 0x15e00000, 0);
    Func_08009128();
    Func_080000c0(1);

    Func_0808a0f0(0, 0x300000, 0x15a00000);
    Func_0808a0f0(25, 0x4e0000, 0x16600000);
    Func_0808a0f0(23, 0x670000, 0x15600000);
    Func_0808a0f0(24, 0x700000, 0x16800000);

    Func_0808a1b8(23, 0x8000, 0);
    Func_0808a1b8(24, 0x8000, 0);

    Func_0808a100(0, 16);
    record = Func_0808a080(0);
    *(s32 *)(record + 0x18) = (s32)0xffff0000;
    Func_080091e0(Func_0808a080(0), 0);

    Func_0808a100(25, 7);
    record = Func_0808a080(25);
    *(u16 *)(*(u8 **)(record + 0x50) + 0x1e) = 0x1555;
    Func_080091e0(Func_0808a080(25), 0);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 256;

    Func_0808a360();
    Func_0808a2e0();
    Func_0808a370();
    Func_0808a010(80);

    Func_0808a1b8(23, 0x3000, 0);
    Func_0808a1b8(24, 0xc000, 40);
    Func_0808a110(23, 3);
    Func_0808a010(20);
    Func_0808a110(24, 3);
    Func_0808a1b8(23, 0x8000, 10);
    Func_0808a1b8(24, 0x8000, 10);
    Func_0808a1e0(0, 3);
    Func_0808a1e0(25, 3);

    Func_0808a090(23, 0x00013333, 0x00026666);
    record = Func_0808a080(23);
    *(s32 *)(record + 0x44) = 0x28f;
    *(s32 *)(record + 0x48) = 0x8000;
    Func_0808a098(23, Data_02002a48);
    Func_0808a010(24);

    Func_0808a090(24, 0x00013333, 0x00026666);
    record = Func_0808a080(24);
    *(s32 *)(record + 0x44) = 0x28f;
    *(s32 *)(record + 0x48) = 0x8000;
    Func_0808a0b0(24, Data_02002b2c);
    Func_0808a010(40);

    Func_0808a2d0();
    Func_0808a2d8();
    Func_080000c0(20);
    Func_0808a2d8();
    Func_080000c0(60);
    Func_0808a2d8();
    Func_080000c0(20);
    Func_0808a2d0();
    Func_0808a010(40);

    *(s32 *)(workspace + 456) = 120;
    workspace = Data_03001ebc;

    Func_0808a368();
    Func_0808a370();
    Func_080770d0(0x834);
    Func_0808a248(9);
    Func_0808a020();
}
