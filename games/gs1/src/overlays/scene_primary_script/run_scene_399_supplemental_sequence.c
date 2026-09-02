#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000384

/* Audited retained supplemental scene body.
 * The complete production span preserves 41 calls, 0 loop(s), and 3 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

s32 Func_0200259a();
extern u8 Value_0200b1d6;
struct SceneRecord {
    u8 pad[100];
    u16 value100;
};
void Func_020025b6();
void Func_020025c2();
void Func_020025ca();
s32 Func_020025cc();
void Func_020025d6();
void Func_020025ec();
void Func_020025f4();
void Func_02002630();
void Func_02002632();
void *Func_0200264e();
void Func_0200266c();
void *Func_0200267c();
void Func_02002684();
void Func_02002690();
void Func_0200269a();
void Func_020026a4();
void Func_020026a8();
void Func_020026b2();
void Func_020026b4();
void Func_020026bc();
void Func_020026c8();
void Func_020026dc();
void Func_020026e8();
void Func_020026ec();
void *Func_02002708();
void Func_0200270c();
void Func_02002718();
void Func_0200271a();
void Func_0200272a();
void Func_02002744();
void Func_02002754();
void Func_02002760();
void Func_02002762();
void Func_02002764();
void Func_02002782();
void Func_020027ae();
void Func_020027c2();
void Func_020027c6();
void Func_020027ca();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */


static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void Func_02000384(void)
{
    void *p18;
    void *p3;
    void *p39;

    if (Value1(Func_0200259a, 2177) != 0) {
        Value0(Func_020025c2);
        p3 = Value3(Func_0200267c, 9, 0, 0);
        Value1(Func_020025ca, 10);
        Value1(Func_020026a8, 5700);
        Value2(Func_020026c8, 9, 0);
        Value0(Func_020025ec);
    } else {
        if (Value1(Func_020025cc, 2091) != 0) {
            Value0(Func_020025f4);
            Call2(Func_02002684, 9, 7);
            Value3(Func_020025b6, 33599936, 10, 69);
            Value1(Func_020026dc, 5484);
            Call2(Func_020026ec, 9, 0);
            Call2(Func_020026a4, 9, 8);
            Call3(Func_020025d6, (s32)&Value_0200b1d6, 10, 69);
            Call0(Func_02002632);
        } else {
            Value0(Func_02002630);
            p18 = Value1(Func_0200264e, 9);
            ((struct SceneRecord *)p18)->value100 = 10;
            Value2(Func_0200266c, 9, 33596660);
            Value1(Func_0200271a, 5428);
            Value2(Func_0200272a, 9, 0);
            Value1(Func_02002690, 8);
            Call3(Func_02002764, 8, 256, 40);
            Call3(Func_02002760, 8, 53248, 10);
            Call2(Func_02002718, 8, 2);
            Call3(Func_02002762, 8, 0, 20);
            Value2(Func_020026b2, 0, 33596772);
            Call3(Func_020026b4, 8, 104857, 52428);
            Value2(Func_020026dc, 8, 33596680);
            Value1(Func_0200269a, 40);
            Call3(Func_02002744, 8, 2, 0);
            Call2(Func_02002754, 8, 2);
            Value2(Func_020027c6, 8, 258);
            Value1(Func_020026bc, 60);
            Call3(Func_020027ae, 8, 0, 10);
            Call3(Func_020027ca, 8, 12288, 20);
            Call2(Func_02002782, 8, 2);
            Value2(Func_020027c2, 8, 0);
            p39 = Value1(Func_02002708, 8);
            *(u8 *)((u8 *)(p39) + 89) ^= 0x2;
            Value1(Func_020026e8, 0x82c);
            Value0(Func_0200270c);
        }
    }
}
