#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02002330

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_02000432[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_0200469e();
void Func_020046b8();
s32 Func_02004710();
void Func_02004bd6();
s32 Func_02004dec();
void Func_02004fcc();
s32 Func_02005048();
s32 Func_020059ea();
void Func_02005c88();
void Func_02005cd0();
void Func_02005d36();
void Func_02006c72();
void Func_02006d50();
void Func_02006d86();
void Func_02006dd8();
void Func_02006de0();
void Func_02006de6();
void Func_02006df6();
void Func_02006e16();
s32 Func_02006e2e();
void Func_02006e56();
void Func_02006e5e();
void Func_02006e6a();
void Func_02006e6c();
void Func_02006e84();
void Func_02006e90();
void Func_02006ea8();
void Func_02006ed0();
void Func_02006eee();
void Func_02006ef0();
void Func_02006f14();
void Func_02006f20();
void Func_02006f3a();
void Func_02006f52();
void Func_02006f64();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 i;
    s32 rec2;
    s32 rec7;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02004bd6();
    } else {
        Func_02006d50();
        rec2 = Value2(Func_02004dec, a0, 5);
        if (rec2 != 0) {
        } else {
            Call1(Func_02006e16, 0x20c3);
            Call2(Func_02006e6a, 0x30000, 0x6000);
            Call4(Func_02006e84, 0x4380000, -1, 0xa80000, 1);
            Func_02006e90();
            Func_02006d86(30);
            Func_02006e56(a0, 0);
            Func_02006e5e(a0, 0);
            Value3(Func_020059ea, 0, 0x3d8, 184);
            Call3(Func_02006de0, 0, 0x18000, 0xc000);
            Func_020046b8(0, 0x3e0, 184);
            Call3(Func_02006df6, 0, 0x4ccc, 0x2666);
            Call3(Func_0200469e, 0, 0x460, 184);
            Func_02006dd8(120);
            Call2(Func_02006ed0, 0, 0x101);
            Func_02006de6(120);
            Func_02005cd0(0);
            Func_02006e6c(0, 1);
            Call2(Func_02006eee, 0, 0x100);
            Call3(Func_02006ef0, 0, 0x105, 0);
            rec7 = Func_02006e2e(0);
            for (i = 119; i >= 0; i--) {
                if (*(s32 *)(rec7 + 8) > 0x3e00000) {
                    *(s32 *)(rec7 + 8) += -0x13333;
                }
                Func_02006c72(1);
            }
            Call3(Func_02006f20, 0, 0x103, 60);
            Value3(Func_02004710, 0, 0x460, 184);
            Func_02006f14(a0, 0);
            Func_02005d36(0);
            {
                u8 *flag = (u8 *)Data_02000240_t;

                flag[498] = 1;
            }
            Func_02005c88(0);
            Func_02006f64(0, 0);
            Func_02004fcc(a0, 5);
            goto L_02002494;
        }
        if (rec2 == 1) {
            Call1(Func_02006f3a, 0x20c2);
            Func_02006f52(a0, 0);
        }
        L_02002494:;
        Value3(Func_02005048, rec2, a0, 5);
        Func_02006ea8();
    }
}
