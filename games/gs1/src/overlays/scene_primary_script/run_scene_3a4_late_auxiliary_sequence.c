#include "types.h"

#define FieldScene_RunLateAuxiliarySequence Func_02002f10

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 19 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

extern u8 Data_02000240[];
extern u8 Value_00000056;
void Func_020069d4();
void Func_02006a84();
void Func_02006a9c();
void Func_02006ad8();
void Func_02006b06();
void Func_02006b0e();
void Func_02006b18();
void Func_02006b1c();
void Func_02006b4c();
void Func_02006b58();
void Func_02006b70();
void Func_02006b94();
void Func_02006b98();
void Func_02006bae();
void Func_02006bba();
void Func_02006c0a();
void Func_02006c6a();
void Func_02006c72();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */


static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Func_02002f10(void)
{
    Value0(Func_02006a84);
    Call2(Func_02006b94, 39321, 4915);
    Call4(Func_02006bae, 21495808, -1, 5701632, 1);
    Call3(Func_02006ad8, 0, 39321, 19660);
    Value3(Func_02006b1c, 0, 328, 116);
    Value1(Func_02006c6a, 148);
    Value2(Func_020069d4, 33599213, 3200);
    Call3(Func_02006a9c, 65536, 65536, 65536);
    Call3(Func_02006b0e, 8, 6553, 3276);
    Call3(Func_02006b18, 9, 6553, 3276);
    Call2(Func_02006b70, 8, 2);
    Call3(Func_02006b4c, 8, 328, 104);
    Value3(Func_02006b58, 9, 328, 108);
    Value1(Func_02006b06, 60);
    Call3(Func_02006c0a, 0, 256, 0);
    Value2(Func_02006bba, 0, 2);
    Value1(Func_02006b98, 8);
    do {
        Data_02000240[0x22B] = 3;
    } while (0);
    Call2(Func_02006c72, (s32)&Value_00000056, 99);
    Call2(Func_02006c6a, 53, 3);
}
