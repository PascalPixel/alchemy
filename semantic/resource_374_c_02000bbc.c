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
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`, never from the
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
s32 Func_02003238();
s32 Func_02003246();
void Func_02003278();
void Func_02003390();
void Func_020033aa();
void Func_020033b6();
void Func_02003364();
void Func_02003344();
void Func_0200338e();
void Func_020032ec();
void Func_020032fa();
void Func_0200333c();
u8 *Func_02003302();
void Func_02003360();
void Func_0200335c();
void Func_020033ae();
void Func_020033f8();
void Func_02003402();
void Func_0200339a();
void Func_020033dc();
void Func_0200332a();
void Func_020033d2();
void Func_02003424();
void Func_020033cc();
void Func_02003436();
void Func_02003450();
void Func_0200345c();
void Func_020033fc();
void Func_0200345e();
void Func_020033b8();
void Func_020033c2();
void Func_020033d4();
void Func_020033e2();
void Func_020034a6();
void Func_020034b2();
void Func_02003422();
void Func_020033d0();
void Func_020034d2();
void Func_020034dc();
void Func_020034cc();
void Func_020034f0();
void Func_02003050(s32, s32, s32);
void Func_020034b4();
void Func_02003048(s32, s32);
void Func_020034a4();
void Func_0200342a();
void Func_020034da();
void Func_02003512();
s32 Func_0200346a();
void Func_020034d8();
void Func_0200350e();
void Func_0200357e();
void Func_0200310c(s32, s32, s32);
void Func_020035a8();
void Func_020035dc();
void Func_020035e8();
void Func_020035f4();
void Func_02003600();
void Func_020035fc();
void Func_02003608();
void Func_02003618();
void Func_02003176(s32, s32, s32);
void Func_0200364a();
void Func_02003662();
void Func_0200366e();
void Func_02003666();
void Func_0200367e();
void Func_020035a6();
void Func_020035b4();
void Func_020031c4(s32, s32, s32);
void Func_02003628();
void Func_020031bc(s32, s32);
void Func_02003620();
void Func_020031cc(s32, s32);
void Func_02003630();
void Func_020031fe(s32, s32, s32);
void Func_020031ee(s32, s32);
void Func_02003212(s32, s32, s32);
void Func_02003660();
void Func_02003226(s32, s32, s32);
void Func_0200368a();
void Func_0200321e(s32, s32);
void Func_020036ac();
void Func_020036f6();
void Func_02003254(s32, s32, s32);
void Func_0200325e(s32, s32, s32);
void Func_020036aa();
void Func_02003256(s32, s32);
void Func_0200327a(s32, s32, s32);
void Func_020036c6();
void Func_020036ce();
u8 *Func_0200367c();
void Func_020036be();
void Func_020036dc();
void Func_020036ee();
void Func_020036fe();
u8 *Func_020036ac_b();
void Func_0200370c();
void Func_0200371e();
void Func_0200372e();
u8 *Func_020036dc_b();
void Func_0200373c();
void Func_0200374e();
void Func_020037a2();
void Func_020036ca();

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: one name may carry more than one interface. */
                               /* story-flag query */
                               /* story-flag set   */

                     

                     

                     

                     

                     

                     

                     

                     

                     

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

    if (Func_02003238(0x834) == 0) return;
    if (Func_02003246(0x840) != 0) return;

    Func_02003278();
    Func_02003390(0x00019999, 0x00003333);
    Func_020033aa(0x00c50000, -1, 0x03000000, 1);
    Func_020033b6();
    Func_02003364(0x0eb6);
    Func_02003344(19, 2);
    Func_0200338e(handleA, 0, 10);

    Func_020032ec(0, 0x10000, 0x8000);
    Func_020032fa(25, 0x10000, 0x8000);
    Func_0200333c(0, 179, 0x315);

    record = Func_02003302(0);
    if (record != NULL) {
        Func_02003360(25, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_0200335c(25, 179, 804);
    Func_020033ae(0, 25, 40);
    Func_020033f8(0, 0, 0);
    Func_02003402(25, 0, 0);

    Func_0200339a(17, 3);
    Func_020033aa(18, 3);
    Func_020033dc(17, 18, 0);
    Func_0200332a(20);
    Func_020033d2(17, 1);
    Func_02003424(handleB, 0, 10);
    Func_020033cc(18, 3);
    Func_02003436(18, 0, 10);
    Func_02003450(17, 0, 0);
    Func_0200345c(18, 0xf000, 10);
    Func_020033fc(19, 3);
    Func_0200345e(handleA, 0, 10);

    Func_020033b8(17, 0x00019999, 0x0000cccc);
    Func_020033c2(18, 0x00019999, 0x0000cccc);
    Func_020033d4(17, Data_02002ef0);
    Func_0200339a(20);
    Func_020033e2(18, Data_02002ef0);

    Func_020034a6(0, 0xc000, 0);
    Func_020034b2(25, 0xc000, 60);
    Func_02003402(0, Data_02002f50);
    Func_02003422(25, Data_02002f78);
    Func_020033d0(20);
    Func_020034d2(0, 0, 0);
    Func_020034dc(25, 0, 10);
    Func_020034cc(25, 0);
    Func_020034f0(19, 0x8000, 0);

    Func_02003050(26, 0x6000, 20);
    Func_020034b4(26, 2);
    Func_02003048(26, 10);
    Func_020034a4(0, 3);
    Func_020034b4(25, 3);
    Func_0200342a(20);
    Func_020034da(19, 2);
    Func_02003512(handleA, 0);

    if (Func_0200346a(0, 0) == 1) {
        Func_020034d8(19, 4);
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        Func_0200350e(19, 3);
        bumped = 1;
    }

    Func_0200357e(handleA, 0);

    if (bumped != 0) {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
    }

    Func_0200310c(22, 0x4000, 30);
    Func_020035a8(22, 0);
    Func_020035dc(19, 256, 0);
    Func_020035e8(26, 256, 0);
    Func_020035f4(0, 256, 0);
    Func_02003600(25, 256, 40);
    Func_020035fc(19, 0xa000, 0);
    Func_02003608(26, 0xa000, 0);
    Func_02003618(0, 0xe000, 0);
    Func_02003176(25, 0xe000, 10);

    Func_0200364a(0x00013333, 0x00002666);
    Func_02003662(0x00d70000, -1, 0x02f60000, 1);
    Func_0200366e();
    Func_02003666(0x0000cccc, 0x00001999);
    Func_0200367e(0x00cd0000, -1, 0x030a0000, 1);

    Func_020035a6(22, Data_02002874);
    Func_020035b4(22);
    Func_020031c4(22, 0x2000, 60);
    Func_02003628(19, 2);
    Func_020031bc(19, 10);
    Func_02003620(22, 3);
    Func_020031cc(22, 20);
    Func_02003630(19, 3);
    Func_020035a6(10);

    Func_020031fe(19, 0x4000, 30);
    Func_020031ee(handleA, 10);
    Func_02003212(26, 0xe000, 30);
    Func_02003660(26, 3);
    Func_02003226(19, 0x8000, 30);
    Func_0200368a(19, 2);
    Func_0200321e(handleA, 10);

    Func_020036ac(0, 25, 40);
    Func_020036f6(0, 0, 0);
    Func_02003254(25, 0, 20);
    Func_0200325e(26, 0x8000, 30);
    Func_020036aa(26, 3);
    Func_02003256(26, 30);
    Func_0200327a(26, 0xc000, 30);
    Func_020036c6(26, 3);
    Func_020036ce(22, 3);

    Func_020036ce(25, 2);
    record = Func_0200367c(0);
    if (record != NULL) {
        Func_020036be(25, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020036dc(25);
    Func_020036ee(25, 0, 0);

    Func_020036fe(26, 2);
    record = Func_020036ac_b(0);
    if (record != NULL) {
        Func_020036ee(26, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200370c(26);
    Func_0200371e(26, 0, 0);

    Func_0200372e(22, 2);
    record = Func_020036dc_b(0);
    if (record != NULL) {
        Func_0200371e(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200373c(22);
    Func_0200374e(22, 0, 0);

    Func_020037a2(19, 0x10000, Data_02002c00);
    Func_020036ca(0x840);
    Func_020036f6();
}
