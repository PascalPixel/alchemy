#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x02000bbc (1224 bytes, 114 call sites).
 *
 * Complete owner: prologue `push {r5, r6, lr}` at 0x02000bbc and the single
 * interworking return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x02001054.  r0
 * holds the popped return address, so the owner is `void`.  Both early guards
 * branch to that same epilogue.
 *
 * Two literal pools sit inside the span and are branched over, not executed:
 * 0x02000dc0-0x02000deb (0x00000834, 0x00019999, 0x00003333, 0x00000eb6,
 * 0x00004013, 0x00000315, 0x00004011, 0x0000cccc, 0x0200aef0, 0x0200af50,
 * 0x0200af78) and 0x0200105a-0x02001083 (alignment, 0x03001ebc, 0x00004013,
 * 0x00013333, 0x00002666, 0x02f60000, 0x0000cccc, 0x00001999, 0x030a0000,
 * 0x0200a874, 0x0200ac00).  The first is jumped over by the `b.n 0x02000dec`
 * at 0x02000dbe; the second follows the epilogue.  The next prologue at
 * 0x02001084 is a separate owner in this reconstruction.
 *
 * Link base 0x02008000: the five in-image pool words 0x0200aef0, 0x0200af50,
 * 0x0200af78, 0x0200a874 and 0x0200ac00 are file offsets 0x2ef0, 0x2f50,
 * 0x2f78, 0x2874 and 0x2c00.  All are even, so they are data (animation
 * scripts and the table also used by 0x02000940 and 0x02001274), not Thumb
 * entry points.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`, never from the
 * disassembler's `bl` annotations: 98 import veneers and 16 prologues in this
 * overlay, the latter all the byte-exact 0x020022c8 and 0x020022e0.
 *
 * The workspace pointer 0x03001ebc and the halfword counter at +472 are the
 * ones the byte-exact `assets/code/resource_374_c_020003b4.c` uses.  Note that
 * the counter is bumped on BOTH arms of the Func_0808a070 test, but at
 * different points: the failing arm bumps it before Func_0808a180 and the
 * succeeding arm after, which is why the r6 flag exists at all.  That ordering
 * is preserved.
 *
 * Uncertainties:
 *  - 0x00004011 and 0x00004013 are handles appearing in the same argument
 *    position as the small integers 0, 17-19, 22, 25 and 26.  0x4013 is
 *    additionally built by arithmetic at 0x02000f1a (`adds r5, #19` on the
 *    0x4000 already in r5), which is evidence that the low bits are a
 *    sub-index, but the namespace is not established here;
 *  - Func_0808a080(0)'s record is read at +8/+16 as words once and at +10/+18
 *    as signed halfwords three times; those are the only fields touched, so
 *    byte offsets are used rather than a speculative struct.
 */

/* Workspace pointer global. */
extern u8 *Data_03001ebc;

/* In-image data under the 0x02008000 link base. */
extern u8 Data_02002874[];
extern u8 Data_02002c00[];
extern u8 Data_02002ef0[];
extern u8 Data_02002f50[];
extern u8 Data_02002f78[];

/* Overlay-local callees (byte-exact siblings in assets/code). */
void Func_020022c8(s32, s32);
void Func_020022e0(s32, s32, s32);

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: one name may carry more than one interface. */
s32 Func_080770c0();           /* story-flag query */
void Func_080770c8();          /* story-flag set   */
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0b0();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a150();
void Func_0808a168();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();

void Func_02000bbc(void)
{
    u8 *workspace;
    u8 *record;
    s32 bumped;
    s32 handleA;
    s32 handleB;

    bumped = 0;
    handleA = 0x4013;
    handleB = 0x4011;

    if (Func_080770c0(0x834) == 0) return;
    if (Func_080770c0(0x840) != 0) return;

    Func_0808a018();
    Func_0808a208(0x00019999, 0x00003333);
    Func_0808a210(0x00c50000, -1, 0x03000000, 1);
    Func_0808a218();
    Func_0808a170(0x0eb6);
    Func_0808a138(19, 2);
    Func_0808a188(handleA, 0, 10);

    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a090(25, 0x10000, 0x8000);
    Func_0808a0d0(0, 179, 0x315);

    record = Func_0808a080(0);
    if (record != NULL) {
        Func_0808a0f0(25, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_0808a0d0(25, 179, 804);
    Func_0808a150(0, 25, 40);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(25, 0, 0);

    Func_0808a100(17, 3);
    Func_0808a110(18, 3);
    Func_0808a150(17, 18, 0);
    Func_0808a010(20);
    Func_0808a130(17, 1);
    Func_0808a188(handleB, 0, 10);
    Func_0808a100(18, 3);
    Func_0808a188(18, 0, 10);
    Func_0808a1b8(17, 0, 0);
    Func_0808a1b8(18, 0xf000, 10);
    Func_0808a110(19, 3);
    Func_0808a188(handleA, 0, 10);

    Func_0808a090(17, 0x00019999, 0x0000cccc);
    Func_0808a090(18, 0x00019999, 0x0000cccc);
    Func_0808a098(17, Data_02002ef0);
    Func_0808a010(20);
    Func_0808a098(18, Data_02002ef0);

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(25, 0xc000, 60);
    Func_0808a098(0, Data_02002f50);
    Func_0808a0b0(25, Data_02002f78);
    Func_0808a010(20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(25, 0, 10);
    Func_0808a180(25, 0);
    Func_0808a1b8(19, 0x8000, 0);

    Func_020022e0(26, 0x6000, 20);
    Func_0808a138(26, 2);
    Func_020022c8(26, 10);
    Func_0808a100(0, 3);
    Func_0808a110(25, 3);
    Func_0808a010(20);
    Func_0808a138(19, 2);
    Func_0808a178(handleA, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_0808a100(19, 4);
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        Func_0808a100(19, 3);
        bumped = 1;
    }

    Func_0808a180(handleA, 0);

    if (bumped != 0) {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
    }

    Func_020022e0(22, 0x4000, 30);
    Func_0808a180(22, 0);
    Func_0808a1e8(19, 256, 0);
    Func_0808a1e8(26, 256, 0);
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(25, 256, 40);
    Func_0808a1b8(19, 0xa000, 0);
    Func_0808a1b8(26, 0xa000, 0);
    Func_0808a1b8(0, 0xe000, 0);
    Func_020022e0(25, 0xe000, 10);

    Func_0808a208(0x00013333, 0x00002666);
    Func_0808a210(0x00d70000, -1, 0x02f60000, 1);
    Func_0808a218();
    Func_0808a208(0x0000cccc, 0x00001999);
    Func_0808a210(0x00cd0000, -1, 0x030a0000, 1);

    Func_0808a098(22, Data_02002874);
    Func_0808a0a0(22);
    Func_020022e0(22, 0x2000, 60);
    Func_0808a138(19, 2);
    Func_020022c8(19, 10);
    Func_0808a110(22, 3);
    Func_020022c8(22, 20);
    Func_0808a110(19, 3);
    Func_0808a010(10);

    Func_020022e0(19, 0x4000, 30);
    Func_020022c8(handleA, 10);
    Func_020022e0(26, 0xe000, 30);
    Func_0808a110(26, 3);
    Func_020022e0(19, 0x8000, 30);
    Func_0808a138(19, 2);
    Func_020022c8(handleA, 10);

    Func_0808a150(0, 25, 40);
    Func_0808a1b8(0, 0, 0);
    Func_020022e0(25, 0, 20);
    Func_020022e0(26, 0x8000, 30);
    Func_0808a110(26, 3);
    Func_020022c8(26, 30);
    Func_020022e0(26, 0xc000, 30);
    Func_0808a110(26, 3);
    Func_0808a110(22, 3);

    Func_0808a100(25, 2);
    record = Func_0808a080(0);
    if (record != NULL) {
        Func_0808a0b8(25, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0808a0e8(25);
    Func_0808a0f0(25, 0, 0);

    Func_0808a100(26, 2);
    record = Func_0808a080(0);
    if (record != NULL) {
        Func_0808a0b8(26, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0808a0e8(26);
    Func_0808a0f0(26, 0, 0);

    Func_0808a100(22, 2);
    record = Func_0808a080(0);
    if (record != NULL) {
        Func_0808a0b8(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0808a0e8(22);
    Func_0808a0f0(22, 0, 0);

    Func_0808a168(19, 0x10000, Data_02002c00);
    Func_080770c8(0x840);
    Func_0808a020();
}
