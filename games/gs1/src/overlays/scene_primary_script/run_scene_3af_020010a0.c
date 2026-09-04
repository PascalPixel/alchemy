#include "types.h"

#define FieldScene_RunScene3af_020010a0 Func_020010a0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020023b0();
void Func_02004afe();
s32 Func_02005372();
s32 Func_0200537c();
void Func_020053a4();
void Func_020053d2();
s32 Func_020053dc();
void Func_020053e6();
void Func_020053e8();
s32 Func_02005400();
void Func_02005420();
s32 Func_0200542e();
void Func_02005434();
void Func_02005438();
void Func_02005444();
u8 *Func_0200544e();
void Func_02005476();
void Func_0200548a();
void Func_02005492();
void Func_02005498();
void Func_020054c0();
void Func_020054cc();
void Func_020054ea();
void Func_02005510();
void Func_0200555c();
void Func_0200557e();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3af_020010a0(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_02005372, 0x911) != 0) {
        if (Value1(Func_0200537c, 0x922) == 0) {
            Func_020053a4();
            Func_02005510();
            Func_020023b0();
            Call3(Func_020053e6, 20, 0x6666, 0x3333);
            *(u8 *)(Func_020053dc(20) + 90) &= 254;
            Call3(Func_02005444, 20, 232, 0x330);
            Func_020053d2(1);
            *(u8 *)(Func_02005400(20) + 90) |= 1;
            Func_020053e8(20);
            Func_02005498(20, 2);
            Func_02004afe(20);
            Call3(Func_02005438, 20, 0x13333, 0x9999);
            *(u8 *)(Func_0200542e(20) + 90) &= 254;
            Call3(Func_02005492, 20, 244, 0x324);
            Func_02005420(1);
            {
                u8 *record = Func_0200544e(20);
                u8 value = *(volatile u8 *)&record[90];
            
                record[90] = (u8)(value | 1);
            }
            Func_02005434(20);
            Call3(Func_02005476, 20, 0x33333, 0x19999);
            Call3(Func_020054c0, 20, 248, 0x30a);
            Call3(Func_020054cc, 20, 248, 0x2bc);
            Call3(Func_020054ea, 20, 0xf60000, 0x2000000);
            Func_0200555c(20, 0, 0);
            Call3(Func_0200557e, 0, 0x101, 60);
            Func_0200548a();
        }
    }
}
