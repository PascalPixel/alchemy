#include "types.h"

#define FieldScene_RunScene3ba_020015e0 Func_020015e0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02001748();
void Func_0200179a();
void Func_020017e2();
void Func_02003152();
s32 Func_02003368();
void Func_020034d2();
s32 Func_0200354c();
s32 Func_02003f6c();
void Func_0200418e();
void Func_0200529c();
void Func_020052e0();
void Func_02005322();
void Func_02005340();
void Func_0200536c();
void Func_0200537c();
void Func_0200537e();
void Func_02005390();
void Func_020053aa();
s32 Func_020053b6();
void Func_020053dc();
void Func_020053ee();
void Func_020053f4();
void Func_02005406();
void Func_02005412();

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

void FieldScene_RunScene3ba_020015e0(s32 a0)
{
    u32 i;
    s32 rec8;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02003152();
    } else {
        Func_0200529c();
        rec8 = Value2(Func_02003368, a0, 3);
        if (rec8 == 0) {
            Call1(Func_02005340, 0x2095);
            Func_02001748();
            Call2(Func_02005390, 0x30000, 0x6000);
            Call4(Func_020053aa, 0x3480000, -1, 0xd80000, 1);
            ((void (*)())Func_020053b6)();
            Func_0200537e(a0, 0);
            Func_0200179a();
            Func_020052e0(60);
            Func_02005390(a0, 0);
            Value3(Func_02003f6c, 0, 0x2e0, 200);
            Value3(Func_020053b6, 0, 0, 0);
            Func_020017e2();
            Call3(Func_02005340, 0, 0x10000, 0x8000);
            Call3(Func_0200536c, 0, 0x330, 200);
            Func_02005322(30);
            Call3(Func_020053f4, 0, 0x105, 60);
            Func_020053dc(a0, 0);
            Func_0200418e(0);
            Func_02005412(0, 0);
            Func_020034d2(a0, 3);
        } else {
            if (rec8 == 1) {
                Call1(Func_020053ee, 0x2094);
                Func_02005406(a0, 0);
            }
        }
        Value3(Func_0200354c, rec8, a0, 3);
        Func_0200537c();
    }
}
