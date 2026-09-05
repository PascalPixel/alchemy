#include "types.h"

#define FieldScene_RunScene3adSequenceB Func_02001760

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002558[];
void Func_020032c6();
void Func_020032f0();
void Func_02003300();
void Func_0200330c();
void Func_02003318();
void Func_02003320();
void Func_02003328();
void Func_0200332e();
void Func_02003330();
void Func_02003332();
void Func_02003346();
void Func_02003348();
void Func_02003352();
void Func_02003354();
void Func_02003366();
void Func_0200336c();
void Func_02003374();
void Func_0200337c();
void Func_0200337e();
void Func_02003380();
void Func_02003384();
void Func_0200338c();
void Func_02003396();
void Func_0200339e();
void Func_020033a4();
void Func_020033ba();
void Func_020033cc();
void Func_020033d0();
void Func_020033d4();
void Func_020033dc();
void Func_020033e2();
void Func_020033e4();
void Func_020033f4();
void Func_020033f8();
void Func_02003404();
void Func_02003406();
void Func_0200340a();
void Func_0200343a();
void Func_0200347a();
s32 Func_0200347c();
void Func_02003486();
void Func_02003492();
void Func_02003494();
void Func_0200349e();
s32 Func_020034ac();
void Func_020034ae();
void Func_020034b0();
void Func_020034bc();
void Func_020034c8();
void Func_020034ce();
void Func_020034d4();
s32 Func_020034dc();
void Func_020034de();
void Func_020034e4();
void Func_020034ec();
void Func_020034fe();
void Func_0200350e();
void Func_02003510();
void Func_0200351c();
void Func_0200352e();
void Func_02003530();
void Func_0200353e();
void Func_02003542();
void Func_020035de();
void Func_02003602();
void Func_0200360c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3adSequenceB(void)
{
    u32 i;
    s32 record;
    s32 base5_2558;

    Call3(Func_02003328, 1, 0x5000, 0);
    base5_2558 = (s32)Data_00002558;
    Func_02003318(base5_2558);
    Func_02003330(1, 0);
    Func_02003300(2, 3);
    Func_0200332e((base5_2558 + 1));
    Func_02003346(2, 0);
    Func_02003330(13, 2, 0);
    Func_02003320(13, 3);
    Func_020032c6(20);
    Func_02003354((base5_2558 + 2));
    Func_0200336c(13, 0);
    Call3(Func_02003380, 12, 0x3000, 0);
    Func_02003348(12, 3);
    Func_020032f0(30);
    Func_0200337e((base5_2558 + 3));
    Func_02003396(12, 0);
    Func_02003366(13, 3);
    Func_0200330c(20);
    Func_0200336c(0, 3);
    Func_02003374(1, 3);
    Func_0200337c(2, 3);
    Func_02003384(3, 3);
    Func_02003332(80);
    Call3(Func_0200337e, 13, -16, 0);
    Func_0200338c(13);
    Func_020033a4(13, 1);
    Func_02003352(40);
    Func_020033ba(13, 3);
    Call3(Func_02003406, 13, 0x5000, 0);
    Func_0200336c(30);
    Func_020033cc(0, 3);
    Func_020033d4(1, 3);
    Func_020033dc(2, 3);
    Func_020033e4(3, 3);
    Call3(Func_020033d0, 12, 152, 0x210);
    Func_0200339e(20);
    Call3(Func_020033e2, 13, 160, 0x210);
    Func_020033f8(12);
    Call3(Func_020033f4, 12, 168, 0x280);
    Func_0200340a(13);
    Call3(Func_02003406, 13, 168, 0x280);
    Call3(Func_0200347a, 0, 0x5000, 0);
    Call3(Func_02003486, 2, 0x5000, 0);
    Call3(Func_02003492, 3, 0x5000, 0);
    Call3(Func_0200349e, 1, 0x5000, 0);
    Func_02003404(20);
    Call3(Func_020034b0, 0, 0x4000, 0);
    Call3(Func_020034bc, 2, 0x4000, 0);
    Call3(Func_020034c8, 3, 0x4000, 0);
    Call3(Func_020034d4, 1, 0x4000, 0);
    Func_0200343a(200);
    Func_02003494(13, 0, 0);
    Func_0200349e(12, 0, 0);
    Func_020034ae(1, 2);
    record = Value1(Func_0200347c, 0);
    if (record != 0) {
        Func_0200349e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020034bc(1);
    Func_020034ce(1, 0, 0);
    Func_020034de(2, 2);
    record = Value1(Func_020034ac, 0);
    if (record != 0) {
        Func_020034ce(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020034ec(2);
    Func_020034fe(2, 0, 0);
    Func_0200350e(3, 2);
    record = Value1(Func_020034dc, 0);
    if (record != 0) {
        Func_020034fe(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200351c(3);
    Func_0200352e(3, 0, 0);
    Func_020034e4(30);
    Call3(Func_02003530, 0, -16, 0);
    Func_0200353e(0);
    Func_020035de(0, 1);
    Call3(Func_02003542, 0, 168, 0x280);
    Func_02003510(60);
    Func_0200360c();
    Func_02003602(3);
}
