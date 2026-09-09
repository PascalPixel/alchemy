#include "types.h"

#define FieldScene_RunFirstActorInteraction Func_020012f0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_020030fa();
s32 Func_02003310();
void Func_020034a6();
s32 Func_02003520();
s32 Func_02003efe();
void Func_02004162();
s32 Func_0200445a();
s32 Func_02004466();
s32 Func_02004470();
s32 Func_02004482();
void Func_0200448e();
void Func_02005244();
void Func_020052c8();
void Func_020052dc();
void Func_020052f0();
void Func_02005300();
s32 Func_02005302();
void Func_0200532a();
void Func_02005344();
void Func_02005350();
void Func_02005358();
void Func_0200535e();
void Func_0200536a();
void Func_02005370();
void Func_020053b8();
void Func_020053ca();
void Func_020053d2();
void Func_020053e2();
s32 Func_020053f6();

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

void FieldScene_RunFirstActorInteraction(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_020030fa();
    } else {
        Func_02005244();
        rec = Value2(Func_02003310, a0, 1);
        if (rec == 0) {
            Call1(Func_020052f0, 0x209e);
            Call2(Func_02005344, 0x30000, 0x6000);
            Call4(Func_0200535e, 0x4c80000, -1, 0xb80000, 1);
            Func_0200536a();
            Func_0200532a(a0, 0);
            Value3(Func_02003efe, 0, 0x4f8, 168);
            Call3(Func_020052c8, 0, 0x18000, 0xc000);
            Func_0200445a(0, 0x508, 184);
            Value3(Func_02004466, 0, 0x508, 216);
            Func_02004470(0, 0x4c8, 216);
            Func_02005370(a0, 0);
            Value3(Func_02004482, 0, 0x4c8, 248);
            Call3(Func_0200448e, 0, 0x4a8, 248);
            Func_020052dc(3);
            record = Func_02005302(0);
            *(volatile s32 *)(record + 40) = 0x40000;
            Func_02005358(0, 28);
            Call2(Func_020053d2, 0, 0x102);
            Func_02005300(30);
            Func_020053b8(a0, 0);
            Func_02004162(0);
            Call2(Func_020053f6, 0, 0);
            Func_020034a6(a0, 1);
        } else {
            if (rec == 1) {
                Call1(Func_020053ca, 0x209d);
                Func_020053e2(a0, 0);
            }
        }
        Value3(Func_02003520, rec, a0, 1);
        Func_02005350();
    }
}
