#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_02000420

/* Audited retained supplemental scene body.
 * The complete production span preserves 31 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

extern u8 Value_00001032;
void Func_02001fa2();
s32 Func_02001fb4();
void Func_02001fb6();
s32 Func_02001fc0();
void Func_02001fe8();
void Func_02001fea();
void Func_02001ff2();
void Func_02002004();
void Func_02002008();
void Func_02002018();
void Func_02002030();
void Func_02002052();
void Func_0200205e();
void Func_02002080();
void Func_020020a8();
void Func_020020c0();
void Func_020020d6();
void Func_020020d8();
void Func_020020dc();
void Func_020020fe();
void Func_02002100();
void Func_02002114();
void Func_02002128();
void Func_02002132();
void Func_02002144();
void Func_02002146();
void Func_02002150();
void Func_0200215a();
void Func_02002166();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
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

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

void Func_02000420(void)
{
    s32 base;

    if (Value1(Func_02001fb4, 3842) == 0) {
    } else {
        if (Value1(Func_02001fc0, 2081) != 0) {
        } else {
            Call0(Func_02001fea);
            Value0(Func_020020fe);
            Value1(Func_02002114, 182);
            Value6(Func_02001fb6, 0, 71, 100, 71, 1, 1);
            Value0(Func_02001fa2);
            Value1(Func_02002008, 40);
            base = (s32)&Value_00001032;
            Value2(Func_02001ff2, base, 1);
            Call1(Func_02002018, 20);
            Value1(Func_02002146, 183);
            Func_02001fe8(122, 20, 120, 30, 1, 2);
            Call6(Func_02002004, 122, 20, 1, 2, 120, 30);
            Call0(Func_02001fe8);
            Value3(Func_02002030, 65536, 65536, 65536);
            Value1(Func_0200205e, 20);
            Call3(Func_02002132, 0, 256, 0);
            Value3(Func_02002052, 131072, 131072, 65536);
            Value1(Func_02002080, 20);
            Call3(Func_02002144, 0, 16384, 40);
            Call3(Func_02002150, 0, 32768, 20);
            Call3(Func_0200215a, 0, 0, 20);
            Call3(Func_02002166, 0, 16384, 10);
            Call3(Func_02002128, 0, 4, 20);
            Call3(Func_02002132, 0, 6, 40);
            Value3(Func_020020a8, -1, -1, 58982);
            Value1(Func_020020d8, 40);
            Value2(Func_020020c0, base + 1, 1);
            Value1(Func_020020d6, 0x143);
            Call1(Func_020020dc, 2081);
            Call0(Func_02002100);
        }
    }
}
