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
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`, never from the
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
void Func_020039d4();
u8 *Func_0200394a();
void Func_020038e0();
void Func_020039aa();
void Func_02003888();
void Func_02003a18();
void Func_02003a4c();
void Func_02003a58();
void Func_020039da();
void Func_02003a5e();
void Func_020035c2(s32, s32, s32);
void Func_02003a9a();
void Func_02003ab2();
void Func_02003abe();
void Func_0200399c();
void Func_02003aa0();
void Func_02003600(s32, s32, s32);
void Func_02003ad4();
void Func_02003aec();
void Func_02003a12();
void Func_02003a20();
void Func_02003a5a();
void Func_020039f8();
void Func_02003a6a();
void Func_02003a80();
void Func_02003a98();
void Func_02003b12();
void Func_02003ab0();
void Func_02003b2a();
void Func_02003afa();
u8 *Func_02003a70();
void Func_02003a06();
void Func_02003ad0();
void Func_02003a92();
void Func_02003b02();
void Func_02003b54();
void Func_02003b90();
void Func_020036de(s32, s32, s32);
void Func_02003ac2();
void Func_02003aa2();
void Func_02003ba6();
void Func_02003704(s32, s32, s32);
void Func_02003b68();
void Func_020036fc(s32, s32);
void Func_02003b9a();
void Func_02003b4c();
void Func_02003732(s32, s32, s32);
void Func_02003722(s32, s32);
void Func_02003b44();
void Func_02003bf6();
void Func_02003754(s32, s32, s32);
void Func_02003b98();
void Func_02003ba8();
void Func_02003c1a();
void Func_02003778(s32, s32, s32);
void Func_02003bc4();
void Func_02003c1c();
void Func_02003792(s32, s32, s32);
void Func_0200379e(s32, s32, s32);
void Func_02003c02();
void Func_02003bf2();
void Func_0200379e_b(s32, s32);
void Func_02003c84();
void Func_02003c8e();
void Func_02003b84();
void Func_02003bcc();
void Func_02003b92();
void Func_02003bda();
void Func_02003ba0();
void Func_02003be8();
void Func_02003bae();
void Func_02003c0e();
void Func_02003d12();
void Func_02003d22();
void Func_02003d1a();
void Func_02003bca();

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: one name may carry more than one interface. */
                               /* frame wait */

                     

                     

                     

                     

                     

                     

                     

                     

                     

void Func_02001274(void)
{
    s32 captionA;
    s32 captionB;

    captionA = 0x1019;
    captionB = 0x2018;

    Func_020039d4(25, 15);
    Func_020038e0(Func_0200394a(25), 0);
    Func_020039aa(25, 0, 0x014b0000);
    Func_02003888(1);
    Func_02003a18(captionA, 0);

    Func_02003a4c(23, 256, 0);
    Func_02003a58(24, 256, 40);
    Func_020039da(25, 0, 0);
    Func_02003a5e(23, 0x5000, 0);
    Func_020035c2(24, 0x5000, 40);

    Func_02003a9a(0x18000, 0x3000);
    Func_02003ab2(0x00590000, 0x00b00000, 0x01390000, 1);
    Func_02003abe();
    Func_0200399c(40);

    Func_02003aa0(23, 0xe000, 0);
    Func_02003600(24, 0x7000, 40);
    Func_02003ad4(0x0000cccc, 0x00001999);
    Func_02003aec(0x00640000, 0x00900000, 0x014d0000, 1);

    Func_02003a12(23, 0x10000, 0x8000);
    Func_02003a20(24, 0x10000, 0x8000);
    Func_02003a5a(23, 105, 0x149);
    Func_020039f8(10);
    Func_02003a6a(24, 124, 0x149);

    Func_02003a80(23);
    Func_02003a98(23, 1);
    Func_02003b12(23, 0x5000, 0);
    Func_02003a98(24);
    Func_02003ab0(24, 1);
    Func_02003b2a(24, 0x5000, 0);

    Func_02003afa(25, 0);
    Func_02003a06(Func_02003a70(25), 1);
    Func_02003ad0(25, 0, 0x014b0000);
    Func_02003a92(25, 0x00013333, 0x00009999);
    Func_02003ad4(25, 37, 0x153);
    Func_02003a6a(20);
    Func_02003b02(23, 3);
    Func_02003b54(23, 0);

    Func_02003b90(25, 0x101, 0);
    Func_020036de(0, 0xd000, 10);
    Func_02003ac2(0, 0);
    Func_02003aa2(40);

    Func_02003ba6(23, 0, 0);
    Func_02003704(24, 0x8000, 20);
    Func_02003b68(25, 2);
    Func_020036fc(captionA, 10);
    Func_02003b9a(0, 0x00010019, Data_02002c00);
    Func_02003b4c(25, 93, 0x169);
    Func_02003732(25, 0xd000, 40);
    Func_02003722(25, 20);
    Func_02003b44(0);

    Func_02003bf6(23, 0, 0);
    Func_02003754(24, 0x8000, 15);
    Func_02003b98(23, 3);
    Func_02003ba8(24, 3);
    Func_02003c1a(23, 0x5000, 0);
    Func_02003778(24, 0x5000, 30);
    Func_02003bc4(24, 4);
    Func_02003c1c(captionB, 0);

    Func_02003792(0, 0xd000, 30);
    Func_0200379e(0, 0x4000, 40);
    Func_02003c02(23, 2);
    Func_02003bf2(23, 3);
    Func_0200379e_b(23, 20);

    Func_02003c84(0, 258);
    Func_02003c8e(25, 258);
    Func_02003b84(80);

    Func_02003bcc(24, Data_020028e8);
    Func_02003b92(6);
    Func_02003bda(23, Data_02002940);
    Func_02003ba0(20);
    Func_02003be8(0, Data_02002998);
    Func_02003bae(6);
    Func_02003c0e(25, Data_020029f0);

    *((u8 *)Data_02000240 + 0x22b) = 2;

    Func_02003d12(5, 19);
    Func_02003d22(5, 19);
    Func_02003d1a(12, 4);
    Func_02003bca(282);
}
