#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x02001274 (832 bytes, 79 call sites).
 *
 * Complete owner and entirely straight-line: prologue
 * `push {r5, r6, lr} ; mov r6, r8 ; push {r6}` at 0x02001274 and the
 * interworking return `pop {r3} ; mov r8, r3 ; pop {r5, r6} ; pop {r0} ;
 * bx r0` at 0x02001550.  r0 holds the popped return address, so the owner is
 * `void`.  Bytes 0x0200155a-0x020015b3 are the literal pool (22 words,
 * listed by `overlay_show.ts`), reached only by `ldr rN, [pc, #imm]`.  The
 * next prologue at 0x020015b4 is a separate owner in this reconstruction.
 *
 * This owner is the continuation of 0x02001084, which calls it as its last
 * real action.
 *
 * Link base 0x02008000.  Five pool words fall in the in-image band and are
 * all even, so they are data rather than Thumb entry points: 0x0200a8e8,
 * 0x0200a940, 0x0200a998, 0x0200a9f0 (file offsets 0x28e8/0x2940/0x2998/
 * 0x29f0, each passed as the script argument of Func_0808a098 or
 * Func_0808a0b0) and 0x0200ac00 (0x2c00, the same table 0x02000940 passes to
 * Func_0808a168).  0x02000240 is below the base: it is the EWRAM progress
 * table, and this owner writes the single byte at its offset 0x22b.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`, never from the
 * disassembler's `bl` annotations: 67 import veneers and 12 prologues in this
 * overlay, the latter all reaching the byte-exact 0x020022c8 and 0x020022e0.
 *
 * Numeric arguments are the shift chains the assembly builds, e.g.
 * `movs r0,#178 ; lsls r0,#15` is 0x00590000 and `movs r1,#160 ; lsls r1,#7`
 * is 0x00005000; they read as 16.16 fixed-point coordinates and frame counts.
 *
 * Uncertainties:
 *  - 0x00001019 and 0x00002018 are handles used in the same argument position
 *    as the small integers 0, 23, 24 and 25; their namespace is not
 *    established here, so they are passed through as constants;
 *  - Func_0808a010's argument is 40, 10, 20, 80, 6 and 6 at its eight sites,
 *    which is consistent with a frame count but is not asserted;
 *  - the byte written at Data_02000240 + 0x22b is at an odd offset in a table
 *    that is read as signed halfwords elsewhere, so it is reached through a
 *    byte pointer rather than an element index.
 */

/* EWRAM progress table. */
extern s16 Data_02000240[];

/* In-image data under the 0x02008000 link base. */
extern u8 Data_020028e8[];
extern u8 Data_02002940[];
extern u8 Data_02002998[];
extern u8 Data_020029f0[];
extern u8 Data_02002c00[];

/* Overlay-local callees (byte-exact siblings in assets/code). */
void Func_020022c8(s32, s32);
void Func_020022e0(s32, s32, s32);

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: one name may carry more than one interface. */
void Func_080000c0();          /* frame wait */
void Func_080091e0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a8();
void Func_0808a0b0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a158();
void Func_0808a168();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a250();
void Func_0808a260();
void Func_0808a268();

void Func_02001274(void)
{
    s32 captionA;
    s32 captionB;

    captionA = 0x1019;
    captionB = 0x2018;

    Func_0808a158(25, 15);
    Func_080091e0(Func_0808a080(25), 0);
    Func_0808a0f0(25, 0, 0x014b0000);
    Func_080000c0(1);
    Func_0808a180(captionA, 0);

    Func_0808a1e8(23, 256, 0);
    Func_0808a1e8(24, 256, 40);
    Func_0808a0f0(25, 0, 0);
    Func_0808a1b8(23, 0x5000, 0);
    Func_020022e0(24, 0x5000, 40);

    Func_0808a208(0x18000, 0x3000);
    Func_0808a210(0x00590000, 0x00b00000, 0x01390000, 1);
    Func_0808a218();
    Func_0808a010(40);

    Func_0808a1b8(23, 0xe000, 0);
    Func_020022e0(24, 0x7000, 40);
    Func_0808a208(0x0000cccc, 0x00001999);
    Func_0808a210(0x00640000, 0x00900000, 0x014d0000, 1);

    Func_0808a090(23, 0x10000, 0x8000);
    Func_0808a090(24, 0x10000, 0x8000);
    Func_0808a0c8(23, 105, 0x149);
    Func_0808a010(10);
    Func_0808a0c8(24, 124, 0x149);

    Func_0808a0e8(23);
    Func_0808a100(23, 1);
    Func_0808a1b8(23, 0x5000, 0);
    Func_0808a0e8(24);
    Func_0808a100(24, 1);
    Func_0808a1b8(24, 0x5000, 0);

    Func_0808a158(25, 0);
    Func_080091e0(Func_0808a080(25), 1);
    Func_0808a0f0(25, 0, 0x014b0000);
    Func_0808a090(25, 0x00013333, 0x00009999);
    Func_0808a0d0(25, 37, 0x153);
    Func_0808a010(20);
    Func_0808a110(23, 3);
    Func_0808a178(23, 0);

    Func_0808a1e8(25, 0x101, 0);
    Func_020022e0(0, 0xd000, 10);
    Func_0808a070(0, 0);
    Func_0808a010(40);

    Func_0808a1b8(23, 0, 0);
    Func_020022e0(24, 0x8000, 20);
    Func_0808a138(25, 2);
    Func_020022c8(captionA, 10);
    Func_0808a168(0, 0x00010019, Data_02002c00);
    Func_0808a0d0(25, 93, 0x169);
    Func_020022e0(25, 0xd000, 40);
    Func_020022c8(25, 20);
    Func_0808a0a8(0);

    Func_0808a1b8(23, 0, 0);
    Func_020022e0(24, 0x8000, 15);
    Func_0808a100(23, 3);
    Func_0808a110(24, 3);
    Func_0808a1b8(23, 0x5000, 0);
    Func_020022e0(24, 0x5000, 30);
    Func_0808a110(24, 4);
    Func_0808a180(captionB, 0);

    Func_020022e0(0, 0xd000, 30);
    Func_020022e0(0, 0x4000, 40);
    Func_0808a138(23, 2);
    Func_0808a110(23, 3);
    Func_020022c8(23, 20);

    Func_0808a1f0(0, 258);
    Func_0808a1f0(25, 258);
    Func_0808a010(80);

    Func_0808a098(24, Data_020028e8);
    Func_0808a010(6);
    Func_0808a098(23, Data_02002940);
    Func_0808a010(20);
    Func_0808a098(0, Data_02002998);
    Func_0808a010(6);
    Func_0808a0b0(25, Data_020029f0);

    *((u8 *)Data_02000240 + 0x22b) = 2;

    Func_0808a260(5, 19);
    Func_0808a268(5, 19);
    Func_0808a250(12, 4);
    Func_080770c8(282);
}
