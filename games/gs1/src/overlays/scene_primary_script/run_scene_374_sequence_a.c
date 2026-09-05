#include "types.h"

#define FieldScene_RunScene374SequenceA Func_02000bbc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00004000[];
extern u8 Data_0200aef0[];
extern u8 Data_0200ac00[];
void Func_02003048();
s32 Func_02003050();
void Func_0200310c();
void Func_02003176();
void Func_020031bc();
s32 Func_020031c4();
void Func_020031cc();
void Func_020031ee();
void Func_020031fe();
void Func_02003212();
void Func_0200321e();
void Func_02003226();
s32 Func_02003238();
s32 Func_02003246();
void Func_02003254();
void Func_02003256();
void Func_0200325e();
void Func_02003278();
s32 Func_0200327a();
void Func_020032ec();
void Func_020032fa();
s32 Func_02003302();
void Func_0200332a();
void Func_0200333c();
void Func_02003344();
void Func_0200335c();
void Func_02003360();
void Func_02003364();
void Func_0200338e();
void Func_02003390();
void Func_0200339a();
void Func_020033aa();
void Func_020033ae();
void Func_020033b6();
void Func_020033b8();
void Func_020033c2();
void Func_020033cc();
void Func_020033d0();
void Func_020033d2();
void Func_020033d4();
void Func_020033dc();
void Func_020033e2();
void Func_020033f8();
void Func_020033fc();
s32 Func_02003402();
void Func_02003422();
void Func_02003424();
void Func_0200342a();
void Func_02003436();
void Func_02003450();
void Func_0200345c();
void Func_0200345e();
s32 Func_0200346a();
void Func_020034a4();
void Func_020034a6();
void Func_020034b2();
void Func_020034b4();
void Func_020034cc();
void Func_020034d2();
void Func_020034d8();
void Func_020034da();
void Func_020034dc();
void Func_020034f0();
void Func_0200350e();
s32 Func_02003512();
void Func_0200357e();
void Func_020035a6();
void Func_020035a8();
void Func_020035b4();
void Func_020035dc();
void Func_020035e8();
void Func_020035f4();
void Func_020035fc();
void Func_02003600();
void Func_02003608();
void Func_02003618();
void Func_02003620();
void Func_02003628();
void Func_02003630();
void Func_0200364a();
void Func_02003660();
void Func_02003662();
void Func_02003666();
void Func_0200366e();
s32 Func_0200367c();
void Func_0200367e();
void Func_0200368a();
void Func_020036aa();
s32 Func_020036ac();
void Func_020036be();
void Func_020036c6();
void Func_020036ca();
void Func_020036ce();
s32 Func_020036dc();
void Func_020036ee();
void Func_020036f6();
void Func_020036fe();
void Func_0200370c();
void Func_0200371e();
void Func_0200372e();
void Func_0200373c();
void Func_0200374e();
void Func_020037a2();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene374SequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_200aef0;
    s32 base5_4000;

    i = 0;
    if (Value1(Func_02003238, 0x834) == 0) {
    } else {
        if (Value1(Func_02003246, 0x840) != 0) {
        } else {
            Func_02003278();
            Call2(Func_02003390, 0x19999, 0x3333);
            Call4(Func_020033aa, 0xc50000, -1, 0x3000000, 1);
            Func_020033b6();
            Call1(Func_02003364, 0xeb6);
            Func_02003344(19, 2);
            Call3(Func_0200338e, 0x4013, 0, 10);
            Call3(Func_020032ec, 0, 0x10000, 0x8000);
            Call3(Func_020032fa, 25, 0x10000, 0x8000);
            Call3(Func_0200333c, 0, 179, 0x315);
            record = Value1(Func_02003302, 0);
            if (record != 0) {
                Func_02003360(25, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            Call3(Func_0200335c, 25, 179, 0x324);
            Func_020033ae(0, 25, 40);
            Func_020033f8(0, 0, 0);
            Call3((void (*)())Func_02003402, 25, 0, 0);
            Func_0200339a(17, 3);
            Func_020033aa(18, 3);
            Func_020033dc(17, 18, 0);
            Func_0200332a(20);
            Func_020033d2(17, 1);
            Call3(Func_02003424, 0x4011, 0, 10);
            Func_020033cc(18, 3);
            Func_02003436(18, 0, 10);
            Func_02003450(17, 0, 0);
            Call3(Func_0200345c, 18, 0xf000, 10);
            Func_020033fc(19, 3);
            Call3(Func_0200345e, 0x4013, 0, 10);
            Call3(Func_020033b8, 17, 0x19999, 0xcccc);
            Call3(Func_020033c2, 18, 0x19999, 0xcccc);
            base5_200aef0 = (s32)Data_0200aef0;
            Func_020033d4(17, base5_200aef0);
            Func_0200339a(20);
            Func_020033e2(18, base5_200aef0);
            Call3(Func_020034a6, 0, 0xc000, 0);
            Call3(Func_020034b2, 25, 0xc000, 60);
            Value2(Func_02003402, 0, 0x200af50);
            Call2(Func_02003422, 25, 0x200af78);
            Func_020033d0(20);
            Func_020034d2(0, 0, 0);
            Func_020034dc(25, 0, 10);
            Func_020034cc(25, 0);
            Call3(Func_020034f0, 19, 0x8000, 0);
            Value3(Func_02003050, 26, 0x6000, 20);
            Func_020034b4(26, 2);
            Func_02003048(26, 10);
            Func_020034a4(0, 3);
            Func_020034b4(25, 3);
            Func_0200342a(20);
            Func_020034da(19, 2);
            Value2(Func_02003512, 0x4013, 0);
            if (Value2(Func_0200346a, 0, 0) == 1) {
                i = 1;
                Func_020034d8(19, 4);
            } else {
                Func_0200350e(19, 3);
                bump_step(1);
            }
            Call2(Func_0200357e, 0x4013, 0);
            if (i != 0) {
                bump_step(1);
            }
            base5_4000 = 0x4000;
            Func_0200310c(22, base5_4000, 30);
            Func_020035a8(22, 0);
            Call3(Func_020035dc, 19, 0x100, 0);
            Call3(Func_020035e8, 26, 0x100, 0);
            Call3(Func_020035f4, 0, 0x100, 0);
            Call3(Func_02003600, 25, 0x100, 40);
            Call3(Func_020035fc, 19, 0xa000, 0);
            Call3(Func_02003608, 26, 0xa000, 0);
            Call3(Func_02003618, 0, 0xe000, 0);
            Func_02003176(25, 0xe000, 10);
            Call2(Func_0200364a, 0x13333, 0x2666);
            Call4(Func_02003662, 0xd70000, -1, 0x2f60000, 1);
            Func_0200366e();
            Call2(Func_02003666, 0xcccc, 0x1999);
            Call4(Func_0200367e, 0xcd0000, -1, 0x30a0000, 1);
            Call2(Func_020035a6, 22, 0x200a874);
            Func_020035b4(22);
            Value3(Func_020031c4, 22, 0x2000, 60);
            Func_02003628(19, 2);
            Func_020031bc(19, 10);
            Func_02003620(22, 3);
            Func_020031cc(22, 20);
            Func_02003630(19, 3);
            Func_020035a6(10);
            Func_020031fe(19, base5_4000, 30);
            Func_020031ee((base5_4000 + 19), 10);
            Func_02003212(26, 0xe000, 30);
            Func_02003660(26, 3);
            Func_02003226(19, 0x8000, 30);
            Func_0200368a(19, 2);
            Func_0200321e((base5_4000 + 19), 10);
            Call3((void (*)())Func_020036ac, 0, 25, 40);
            Func_020036f6(0, 0, 0);
            Func_02003254(25, 0, 20);
            Func_0200325e(26, 0x8000, 30);
            Func_020036aa(26, 3);
            Func_02003256(26, 30);
            Value3(Func_0200327a, 26, 0xc000, 30);
            Func_020036c6(26, 3);
            Func_020036ce(22, 3);
            Func_020036ce(25, 2);
            record = Value1(Func_0200367c, 0);
            if (record != 0) {
                Func_020036be(25, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Call1((void (*)())Func_020036dc, 25);
            Func_020036ee(25, 0, 0);
            Func_020036fe(26, 2);
            record = Value1(Func_020036ac, 0);
            if (record != 0) {
                Func_020036ee(26, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_0200370c(26);
            Func_0200371e(26, 0, 0);
            Func_0200372e(22, 2);
            record = Value1(Func_020036dc, 0);
            if (record != 0) {
                Func_0200371e(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_0200373c(22);
            Func_0200374e(22, 0, 0);
            Call3(Func_020037a2, 19, 0x10000, (s32)Data_0200ac00);
            Call1(Func_020036ca, 0x840);
            Func_020036f6();
        }
    }
}
