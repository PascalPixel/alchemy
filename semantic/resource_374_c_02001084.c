#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x02001084 (496 bytes, 50 call sites).
 *
 * Complete owner and entirely straight-line: prologue
 * `push {r5, r6, lr} ; mov r6, r8 ; push {r6}` at 0x02001084 and the
 * interworking return `pop {r3} ; mov r8, r3 ; pop {r5, r6} ; pop {r0} ;
 * bx r0` at 0x02001240.  r0 holds the popped return address, so the owner is
 * `void`.  Bytes 0x0200124a-0x02001273 are the literal pool (alignment,
 * 0x010b0000, 0x00013333, 0x00009999, 0x00000157, 0x00000ed6, 0x01190000,
 * 0x0000cccc, 0x00006666, 0x00008017, 0x00002018); they are reached only by
 * `ldr rN, [pc, #imm]`.  The next prologue at 0x02001274 is a separate owner,
 * and it is this owner's own last real call.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`, not from the
 * disassembler's `bl` annotations (an overlay `bl` stores target_offset - 2):
 * 41 import veneers plus 9 prologues in this overlay.  Of the prologues,
 * 0x020022c8 and 0x020022e0 are already byte-exact in `assets/code`, and
 * 0x02001274 is the neighbouring owner.
 *
 * Everything the owner does is a fixed sequence of scripted camera, actor and
 * caption commands; the numeric arguments are the shift chains the assembly
 * builds (e.g. `movs r1,#210 ; lsls r1,#15` is 0x00690000) and read as 16.16
 * fixed-point coordinates.
 *
 * Uncertainties:
 *  - the two pool words 0x00008017 and 0x00002018 are handles used both as
 *    Func_0808a180's and Func_020022c8's first argument, the same position in
 *    which the small integers 23 and 24 appear elsewhere in this owner.  Their
 *    namespace is not established here, so they are passed through as
 *    constants;
 *  - Func_0808a210 is reached three times with four arguments and
 *    Func_0808a180 five times with two; one main-image name may still carry
 *    more than one interface, so old-style declarations are used throughout.
 */

/* Overlay-local callees (byte-exact siblings in assets/code). */
void Func_020022c8(s32, s32);
void Func_020022e0(s32, s32, s32);

/* Neighbouring owner in this overlay. */
void Func_02001274(void);

/* Main-image imports reached through this overlay's veneer table. */
void Func_080000c0();          /* frame wait */
void Func_08009128();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a048();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
u8 *Func_0808a228();
void Func_0808a2e0();
void Func_0808a360();
void Func_080f9010();

void Func_02001084(void)
{
    u8 *record;
    s32 captionA;
    s32 captionB;

    record = Func_0808a228();
    Func_0808a018();

    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a210(0x400000, 0x900000, 0x15e00000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a048(1, 0);
    Func_0808a360();
    Func_080f9010(17);
    Func_0808a2e0();
    Func_0808a0f0(23, 0x690000, 0x010b0000);
    Func_080000c0(1);
    Func_0808a090(0, 0x00013333, 0x00009999);
    Func_0808a0d0(0, 93, 0x157);
    Func_0808a170(0x0ed6);
    Func_0808a180(23, 0);
    Func_080f9010(61);

    record[0x55] = 0;

    Func_0808a208(0x30000, 0x6000);
    Func_0808a210(0x6d0000, 0xb00000, 0x01190000, 1);
    Func_0808a218();
    Func_0808a010(40);
    Func_0808a0f0(24, 0x870000, 0xb10000);
    Func_0808a090(24, 0x0000cccc, 0x00006666);
    Func_0808a0c8(24, 126, 258);
    Func_0808a010(40);
    Func_0808a1b8(23, 0xd000, 0);
    Func_0808a0e8(24);
    Func_0808a100(24, 1);
    Func_020022e0(24, 0x7000, 10);
    Func_0808a110(23, 3);
    Func_0808a110(24, 4);

    captionA = 0x8017;
    captionB = 0x2018;

    Func_0808a180(captionB, 0);
    Func_0808a138(23, 2);
    Func_020022c8(captionA, 30);
    Func_020022e0(24, 0xb000, 20);
    Func_020022c8(captionB, 10);
    Func_020022e0(23, 0xb000, 40);
    Func_0808a180(captionA, 0);
    Func_0808a110(24, 4);
    Func_0808a180(captionB, 0);
    Func_020022e0(23, 0xf000, 10);
    Func_0808a138(23, 2);
    Func_0808a180(captionA, 0);
    Func_020022e0(24, 0x6000, 20);
    Func_0808a110(24, 3);
    Func_0808a010(20);
    Func_020022c8(captionB, 20);

    Func_02001274();
    Func_0808a020();
}
