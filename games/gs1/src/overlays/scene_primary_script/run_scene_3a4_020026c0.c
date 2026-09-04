#include "types.h"

#define FieldScene_RunScene3a4_020026c0 Func_020026c0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002ade();
void Func_02005b38();
void Func_02005b74();
void Func_02005bb0();
void Func_020061fe();
void Func_02006212();
void Func_02006226();
s32 Func_02006230();
u8 *Func_02006260();
void Func_02006262();
s32 Func_0200626a();
u8 *Func_0200629c();
void Func_020062a0();
s32 Func_020062a8();
void Func_020062cc();
u8 *Func_020062d8();
void Func_020062dc();
s32 Func_020062e8();
void Func_020062f6();
void Func_02006302();
u8 *Func_02006316();
void Func_02006354();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a4_020026c0(void)
{
    u32 i;
    u8 *record;

    record = Func_02006260(9);
    Func_020061fe((s32)record, 0);
    Func_02002ade();
    Func_02005b38(9);
    if (Value1(Func_02006230, 0x200) != 0) {
        Func_020062dc(9, 5);
        Call6(Func_02006226, 0, 0, 1, 1, 26, 26);
        {
            u8 *record = Func_0200629c(9);
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 2);
        }
    }
    Func_02005b74(11);
    if (Value1(Func_0200626a, 0x201) != 0) {
        Call2((void (*)())Func_02006316, 11, 5);
        Call6(Func_02006262, 1, 0, 1, 1, 17, 10);
        {
            u8 *record = Func_020062d8(11);
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 2);
        }
    }
    Func_02005bb0(12);
    if (Value1(Func_020062a8, 0x204) != 0) {
        Func_02006354(12, 5);
        Call6(Func_020062a0, 1, 0, 1, 1, 26, 15);
        {
            u8 *record = Func_02006316(12);
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 2);
        }
    }
    Call2(Func_02006212, 0x200b429, 0xc80);
    if (Value1(Func_020062e8, 0x327) != 0) {
        Call6((void (*)())Func_020062d8, 30, 82, 1, 1, 29, 81);
        Call6(Func_020062cc, 46, 28, 29, 17, 1, 2);
    } else {
        Call6(Func_02006302, 28, 82, 1, 1, 29, 81);
        Call6(Func_020062f6, 47, 28, 29, 17, 1, 2);
    }
}
