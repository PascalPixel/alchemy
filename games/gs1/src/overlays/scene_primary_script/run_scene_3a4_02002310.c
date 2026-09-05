#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02004f5c();
void Func_02005172();
void Func_02005d7c();
s32 Func_02005e68();
s32 Func_02005e8e();
void Func_02005e94();
void Func_02005ea8();
s32 Func_02005ed6();
void Func_02005ee6();
void Func_02005ef2();
s32 Func_02005f02();
void Func_02005f10();
void Func_02005f1a();
void Func_02005f1c();
void Func_02005f8a();
void Func_02005faa();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

void FieldScene_RunScene3a4_02002310(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02005e68, 0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Func_02005f1a(9, 0, 0);
    } else {
        Func_02005d7c();
        if (Value1(Func_02005e8e, 0x109) == 0) {
            if (Data_02000240_t[225][0] != 99) {
                goto L_0200235a;
            }
            Func_02005172();
        } else {
            L_0200235a:;
            Call6(Func_02005e94, 38, 24, 1, 2, 37, 24);
            Call6(Func_02005ea8, 44, 23, 1, 2, 45, 23);
            if (Value1(Func_02005ed6, 0x8fe) == 0) {
                Func_02005faa(9, 2);
                Func_02005f8a(9, 3);
                Func_02004f5c(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (Value1(Func_02005f02, 0x323) != 0) {
        Call6(Func_02005ef2, 0, 0, 1, 1, 24, 80);
        Call6(Func_02005ee6, 0, 1, 24, 11, 1, 2);
    } else {
        Call6(Func_02005f1c, 2, 0, 1, 1, 24, 80);
        Call6(Func_02005f10, 2, 1, 24, 11, 1, 2);
    }
}
