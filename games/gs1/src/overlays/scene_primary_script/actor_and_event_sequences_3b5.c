#include "types.h"

/* Shared scene work and address-ordered actor/event sequences for resource_3b5. */

extern u8 Data_03001ebc[];
/* Loader-relocated overlay calls retain their audited per-call bindings. */
void Func_02000f66();
void Func_02000f7e();
s32 Func_02000fa6_a();
s32 Func_02000fa6_b();
u8 *Func_02000fb8();
s32 Func_0200132a();
void Func_0200133c_a();
void Func_0200133c_b();
void Func_0200138a();
void Func_02001396();
void Func_020013d2();
void Func_020013ea();
void Func_02001402();
void Func_02001412();
void Func_0200141a();
void Func_02001432();
void Func_02001452();
void Func_020015f4();
void Func_02001618();
void Func_02001642();
s32 Func_0200164e();
void Func_0200167e();
void Func_02001688();
void Func_020016a4();
void Func_020016ae();
void Func_020016bc();
void Func_020016be();
s32 Func_020016ce_a();
s32 Func_020016ce_b();
void Func_020016da();
void Func_020016fa();
void Func_02001740();
void Func_02001748();

/* Shared scene-call helpers preserve the existing argument evaluation model. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void bump_step(s32 amount)
{
    /* The scene step counter is at 0x1d8 of the shared scene work record. */
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3b5_02000224(void)
{
    u32 i;
    u8 *record;

    record = Value1(Func_02000fa6_a, 8);
    if ((s32)record != 0) {
        record[89] = 0;
    }
    record = Func_02000fb8(8);
    Func_02000f66((s32)record, 0);
    Call4(Func_02000f7e, 0, 0x2200000, 0x1200000, 253);
    Call1(Func_02000fa6_b, 0x200);
}

void FieldScene_RunScene3b5_020005dc(void)
{
    u32 i;
    s32 record;

    Func_0200133c_a();
    if (Value1(Func_0200132a, 0x8bf) == 0) {
        Call1(Func_0200133c_b, 0x8bf);
        Call1(Func_020013ea, 0x2368);
        Func_02001402(19, 0);
        Func_02001452(233, 3);
        Func_02001412(19, 0);
        Func_020013d2(0, 1);
        Func_0200138a(233, 0);
    } else {
        Call1(Func_0200141a, 0x236a);
        Func_02001432(19, 0);
    }
    Func_02001396();
}

void FieldScene_RunScene3b5SequenceA(void)
{
    u32 i;
    s32 record;

    Func_020015f4();
    Call3(Func_02001642, 0, 0x130, 0x138);
    Call3(Func_020016ce_a, 0, 0xc000, 0);
    Call3(Func_020016da, 28, 0x4000, 0);
    Func_02001618(20);
    Call1(Func_020016be, 0xe3d);
    Value2(Func_020016ce_b, 28, 0);
    if (Value2(Func_0200164e, 0, 0) == 0) {
        bump_step(1);
        Func_020016fa(28, 0);
        Call3(Func_02001688, 28, 0x10000, 0x8000);
        Call3(Func_020016ae, 28, 0x140, 0x130);
        Call3(Func_020016bc, 28, 0x13c, 0x148);
        Call3(Func_02001748, 28, 0xa000, 0);
        Call1(Func_0200167e, 0x8c1);
    } else {
        Func_02001740(28, 0);
    }
    Func_020016a4();
}
