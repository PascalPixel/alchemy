#include "types.h"

#define FieldScene_RunScene3a4_02002490 Func_02002490

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_020050b4();
void Func_02005356();
void Func_0200596a();
void Func_02005f1a();
s32 Func_02005fe8();
s32 Func_0200600a();
s32 Func_0200602c();
void Func_0200605a();
s32 Func_02006062();
void Func_0200607c();
void Func_02006088();
s32 Func_02006098();
void Func_0200609a();
void Func_020060a6();
void Func_020060b2();
u8 *Func_020060d0();
void Func_020060e0();
void Func_02006100();
void Func_0200610e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
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

void FieldScene_RunScene3a4_02002490(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_02005fe8, 0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Func_0200609a(10, 0, 0);
    } else {
        if (Value1(Func_0200600a, 0x109) == 0) {
            if (Data_02000240_t[225][0] == 99) {
                Func_02005356();
            }
        }
        Func_02005f1a();
        if (Value1(Func_0200602c, 0x907) == 0) {
            Func_02006100(10, 2);
            Func_020060e0(10, 3);
            Call4(Func_020050b4, 0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    Func_0200596a(9);
    if (Value1(Func_02006062, 0x200) != 0) {
        Func_0200610e(9, 5);
        Call6(Func_0200605a, 23, 13, 1, 1, 25, 13);
        {
            u8 *record = Func_020060d0(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    if (Value1(Func_02006098, 0x325) != 0) {
        Call6(Func_02006088, 10, 72, 1, 1, 11, 73);
        Call6(Func_0200607c, 49, 32, 11, 4, 1, 2);
    } else {
        Call6(Func_020060b2, 12, 72, 1, 1, 11, 73);
        Call6(Func_020060a6, 48, 32, 11, 4, 1, 2);
    }
}
