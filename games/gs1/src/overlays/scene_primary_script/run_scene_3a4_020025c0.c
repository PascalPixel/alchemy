#include "types.h"

#define FieldScene_RunScene3a4_020025c0 Func_020025c0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02005a50();
s32 Func_02006128();
s32 Func_02006148();
void Func_0200614a();
void Func_0200617a();
s32 Func_02006180();
void Func_02006186();
s32 Func_02006196();
u8 *Func_0200619a();
void Func_020061a4();
void Func_020061b0();
void Func_020061d2();
void Func_020061fc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

void FieldScene_RunScene3a4_020025c0(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        if (Value1(Func_02006128, 0x109) == 0) {
            Call3(Func_020061d2, 8, 0x1660000, 0x680000);
        }
    }
    Func_02005a50(9);
    if (Value1(Func_02006148, 0x200) != 0) {
        rec7 = Func_0200619a(9);
        Func_020061fc(9, 5);
        Call6(Func_0200614a, 45, 41, 1, 1, 43, 41);
        {
            u8 value = *(volatile u8 *)&rec7[35];

            rec7[35] = (u8)(value | 2);
        }
    }
    if (Value1(Func_02006180, 0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    }
    if (Value1(Func_02006196, 0x326) != 0) {
        Call6(Func_02006186, 17, 93, 1, 1, 16, 92);
        Call6(Func_0200617a, 46, 29, 16, 28, 1, 2);
    } else {
        Call6(Func_020061b0, 15, 93, 1, 1, 16, 92);
        Call6(Func_020061a4, 47, 29, 16, 28, 1, 2);
    }
}
