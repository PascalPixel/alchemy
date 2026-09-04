#include "types.h"

#define FieldScene_RunScene3bf_02004794 Func_02004794

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000244f[];
extern u8 Data_00002455[];
extern u8 Data_03001ebc[];
s32 Func_02009dae();
void Func_02009dc8();
s32 Func_02009dcc();
void Func_02009dec();
void Func_02009e0a();
void Func_02009e3e();
void Func_02009e4c();
void Func_02009e82();
void Func_02009e90();
void Func_02009ea8();
void Func_02009eae();
void Func_02009eba();
s32 Func_02009ebe();
s32 Func_02009ece();
void Func_02009ed4();
void Func_02009ed6();
void Func_02009eda();
void Func_02009ef2();
void Func_02009ef8();
void Func_02009f14();
void Func_02009f1c();
void Func_02009f34();
void Func_02009f38();
void Func_02009f40();
void Func_02009f46();
void Func_02009f4c();
void Func_02009f50();
void Func_02009f54();
void Func_02009f5e();
void Func_02009f66();
s32 Func_02009f6e();
void Func_02009f78();
void Func_02009f7e();
void Func_02009f88();
void Func_02009f98();
void Func_02009fa8();
s32 Func_02009fb8();
void Func_02009fbe();
s32 Func_02009fce();
void Func_02009fd0();
void Func_02009fd6();
void Func_02009fe6();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_02004794(void)
{
    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Func_02009dc8();
    if (Value1(Func_02009dae, 0x941) != 0) {
        Call1(Func_02009e90, 0x2566);
        Func_02009ea8(18, 0);
        Func_02009dec();
    } else {
        if (Value1(Func_02009dcc, 0x313) != 0) {
            Call1(Func_02009eae, 0x2457);
            Value2(Func_02009ebe, 25, 0);
            Func_02009e0a();
        } else {
            Call3(Func_02009ef8, 25, 0x102, 30);
            Func_02009eba(25, 0, 0);
            base5_244f = (s32)Data_0000244f;
            Func_02009eda(base5_244f);
            Func_02009ef2(25, 0);
            Func_02009ed4(25, 24, 0);
            Func_02009f54(24, 1);
            Func_02009f50();
            Func_02009e3e(60);
            Func_02009f66(0, 1);
            Func_02009e4c(20);
            Call3(Func_02009f46, 25, 0x105, 60);
            Func_02009f1c((base5_244f + 1));
            Func_02009f34(25, 0);
            Call3(Func_02009f5e, 25, 0x107, 60);
            Func_02009f34((base5_244f + 2));
            Func_02009f4c(25, 0);
            Func_02009e82(70);
            Call3(Func_02009f7e, 25, 0x100, 60);
            Func_02009f40(25, 0, 0);
            Func_02009f5e((base5_244f + 3));
            Value2(Func_02009f6e, 25, 0);
            if (Value2(Func_02009ece, 0, 0) == 0) {
                Func_02009f78((base5_244f + 4));
                Func_02009f88(25, 0);
            } else {
                Func_02009f88((base5_244f + 5));
                Func_02009f98(25, 0);
            }
            Func_02009ed6(60);
            Call3(Func_02009fd0, 25, 0x105, 60);
            base5_2455 = (s32)Data_00002455;
            Func_02009fa8(base5_2455);
            Value2(Func_02009fb8, 25, 0);
            Func_02009f98(25, 1);
            Func_02009fbe((base5_2455 + 1));
            Value2(Func_02009fce, 25, 0);
            Func_02009f98(25, 3);
            Func_02009fd6((base5_2455 + 2));
            Value2(Func_02009fe6, 25, 0);
            Call1(Func_02009f14, 0x313);
            Func_02009f38();
        }
    }
}
