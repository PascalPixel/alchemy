#include "types.h"

#define FieldScene_RunProgressDependentActorSequence Func_02001760

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000e85[];
extern u8 Data_02009ce0[];
extern u8 Data_03001ebc[];
s32 Func_020031ca();
s32 Func_020031d6();
void Func_020031fe();
void Func_0200321e();
void Func_02003220();
void Func_02003254();
s32 Func_0200328e();
void Func_020032a0();
void Func_020032b4();
void Func_020032c2();
void Func_020032c6();
s32 Func_020032c8();
void Func_020032cc();
void Func_020032dc();
void Func_020032de();
void Func_020032e4();
void Func_020032e6();
void Func_020032fe();
void Func_02003322();
void Func_0200332a();
void Func_0200333c();
void Func_02003340();
void Func_0200334c();
void Func_02003352();
s32 Func_02003360();
void Func_0200336a();
void Func_02003370();
void Func_02003378();
void Func_0200338a();
void Func_02003390();
void Func_02003392();
void Func_02003398();
void Func_0200339c();
void Func_020033a2();
void Func_020033a6();
void Func_020033b2();
void Func_020033b4();
void Func_020033b8();
void Func_020033bc();
void Func_020033be();
void Func_020033ce();
s32 Func_020033e0();
void Func_020033f6();
void Func_02003404();
void Func_0200340a();
void Func_02003414();
void Func_02003416();
void Func_0200341a();
void Func_0200342a();
void Func_02003438();
void Func_0200344a();
void Func_02003464();
void Func_02003470();
void Func_02003482();
void Func_0200348c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunProgressDependentActorSequence(void)
{
    u32 i;
    s32 record;
    s32 base5_e85;
    s32 base5_2009ce0;

    if (Value1(Func_020031ca, 0x839) != 0) {
    } else {
        if (Value1(Func_020031d6, 0x82f) != 0) {
            Func_020031fe();
            ((void (*)())Func_0200328e)(11, 2);
            Call1(Func_020032b4, 0xe8b);
            Func_020032cc(11, 0);
            Func_02003220();
        } else {
            Func_0200321e();
            Func_02003254(11);
            Func_020032b4(11, 1);
            base5_e85 = (s32)Data_00000e85;
            Func_020032dc(base5_e85);
            Func_020032fe(11, 0, 20);
            Call3(Func_02003322, 0, 0x100, 30);
            Call4(Func_02003352, 0x620000, -1, 0x11b0000, 1);
            Call3(Func_020032b4, 0, 94, 0x125);
            Call3(Func_02003340, 0, 0xa000, 0);
            record = Value1(Func_0200328e, 0);
            if (record != 0) {
                Func_020032e4(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
            Call3(Func_020032de, 1, 110, 0x117);
            Call3(Func_0200336a, 1, 0xa000, 40);
            Func_0200332a(11, 2);
            Func_020032a0(40);
            Value2(Func_02003360, 11, 0);
            if (Value2(Func_020032c8, 0, 0) == 0) {
                Func_0200334c(11, 2);
                Func_020032c2(20);
                Func_02003378((base5_e85 + 2));
                Func_02003390(11, 0);
                Call1(Func_020032c6, 0x82f);
            } else {
                Func_02003370(11, 2);
                Func_020032e6(20);
                Func_0200339c((base5_e85 + 3));
                Func_020033be(11, 0, 40);
                Func_02003398(11, 0, 0);
                Func_02003378(11, 1);
                Func_02003392(11, 4, 40);
                Func_0200338a(11, 6);
                Call3(Func_02003404, 11, 0x101, 40);
                Func_020033f6(11, 0, 10);
                Func_020033a6(11, 1);
                Func_0200333c(10);
                Func_020033bc(11, 3);
                Func_02003416(11, 0, 10);
                Func_020033ce(11, 3);
                base5_2009ce0 = (s32)Data_02009ce0;
                Call3(Func_0200340a, 0, 0x1000b, base5_2009ce0);
                Call3(Func_02003414, 1, 0x1000b, base5_2009ce0);
                Call2(Func_020033b4, 11, 0x2009bdc);
                Func_020033b2(0);
                Func_020033b8(1);
                Call3(Func_02003464, 0, 0x4000, 0);
                Call3(Func_02003470, 1, 0x4000, 60);
                Call3(Func_02003482, 0, 0x105, 0);
                Call3(Func_0200348c, 1, 0x105, 120);
                Call1(Func_020033a2, 0x839);
            }
            Func_0200342a(1, 2);
            record = Value1(Func_020033e0, 0);
            if (record != 0) {
                Func_0200341a(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_02003438(1);
            Func_0200344a(1, 0, 0);
            Func_020033f6();
        }
    }
}
