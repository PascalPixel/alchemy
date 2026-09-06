#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02009808();
void Func_02009852();
void Func_0200987a();
void Func_02009894();
void Func_020098b0();
void Func_020098c8();
void Func_020098dc();
s32 Func_020098e6();
s32 Func_020098ee();
void Func_02009920();
void Func_0200992c();
void Func_0200994e();
void Func_020099ac();
void Func_020099de();
void Func_020099ec();
void Func_020099f6();
void Func_02009a0a();
void Func_02009a1c();
void Func_02009a48();
void Func_02009a56();
void Func_02009a5c();
void Func_02009a8a();
void Func_02009ac6();
void Func_02009ad6();
void Func_02009ade();
void Func_02009b5c();
void Func_02009b7c();
void Func_02009b84();

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

void FieldScene_RunScene3c8_02004a2c(void)
{
    u32 i;
    u8 *p8;
    s32 rec;
    u8 *rec8;
    s32 record;
    s32 none;
    s32 v2;
    s32 slot0;

    rec = Value1(Func_020098e6, 0);
    rec8 = Value1(Func_020098ee, 20);
    Func_020098dc();
    Call4(Func_020099de, -1, -1, -1, 0);
    Func_02009852();
    Func_02009808(1);
    *(s32 *)(rec + 12) = 0x820000;
    *(s32 *)(rec + 72) = 0x8000;
    none = 0;
    *(s32 *)(rec + 68) = none;
    p8 = rec + 85;
    *p8 = none;
    Func_02009a48();
    Func_02009a5c();
    Func_02009a8a(204);
    Func_02009920(30);
    *p8 = 3;
    Func_0200992c(24);
    Call2(Func_02009a1c, 0, 0x101);
    Func_020099ac(0, 22);
    *p8 &= 254;
    *(s32 *)((s32)rec8 + 12) += -0x30000;
    *(s32 *)(rec + 12) += -0x30000;
    *(s32 *)(rec + 20) += -0x30000;
    Func_0200987a(2);
    *(s32 *)((s32)rec8 + 12) += -0x20000;
    *(s32 *)(rec + 12) += -0x20000;
    *(s32 *)(rec + 20) += -0x20000;
    Func_02009894(10);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Func_020098b0(4);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Func_020098c8(4);
    *(s32 *)((s32)rec8 + 12) += 0x10000;
    *(s32 *)(rec + 12) += 0x10000;
    *(s32 *)(rec + 20) += 0x10000;
    *p8 = none;
    rec8[85] = none;
    Call2(Func_02009ac6, 0, 0x100);
    Func_02009a56(0, 1);
    Func_020099ec(40);
    *(s32 *)(rec + 108) = 0x200c969;
    Func_020099f6(60);
    Func_02009ad6(0, 1);
    Func_02009ade(20, 1);
    Func_02009b7c(17);
    Call1(Func_02009b84, 0x134);
    Call1(Func_02009a0a, 0x101);
    v2 = 0;
    do {
        *(s32 *)(rec + 12) += 0x10000;
        *(s32 *)(rec + 20) += 0x10000;
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        slot0 = v2;
        Func_0200994e(1);
        v2 = slot0;
        v2 = (v2 + 1);
    } while ((u32)v2 <= 127);
    Func_02009b5c(21);
}
