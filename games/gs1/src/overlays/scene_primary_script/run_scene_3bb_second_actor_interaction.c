#include "types.h"

#define FieldScene_RunSecondActorInteraction Func_02001424

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02001d44();
void Func_02001dd8();
void Func_02001e3c();
void Func_0200322e();
s32 Func_02003444();
void Func_020035b8();
s32 Func_02003632();
void Func_02004046();
void Func_02004274();
s32 Func_020045a4();
s32 Func_020045b0();
void Func_02005378();
void Func_020053d4();
void Func_02005412();
void Func_02005424();
s32 Func_02005462();
void Func_02005470();
void Func_0200547c();
void Func_02005496();
void Func_020054a2();
void Func_020054ca();
void Func_020054dc();
void Func_020054f4();
void Func_02005502();
void Func_02005508();
void Func_02005520();

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

void FieldScene_RunSecondActorInteraction(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200322e();
    } else {
        Func_02005378();
        rec = Value2(Func_02003444, a0, 2);
        if (rec == 0) {
            Call1(Func_02005424, 0x20a2);
            Func_02001d44();
            Call2(Func_0200547c, 0x30000, 0x6000);
            Call4(Func_02005496, 0x3d80000, -1, 0xe80000, 1);
            Func_020054a2();
            Value2(Func_02005462, a0, 0);
            Func_02001dd8();
            Func_02005470(a0, 0);
            Func_02004046(0, 0x438, 0x108);
            Func_020053d4(15);
            Call3(Func_02005412, 0, 0x18000, 0xc000);
            Value3(Func_020045a4, 0, 0x438, 216);
            Value3(Func_020045b0, 0, 0x428, 216);
            Func_02001e3c();
            Func_02005520();
            Call4(Func_02005502, -1, -1, -1, 0);
            Func_020054ca(a0, 0);
            Func_02004274(0);
            Func_02005508(0, 0);
            Func_020035b8(a0, 2);
        } else {
            if (rec == 1) {
                Call1(Func_020054dc, 0x20a1);
                Func_020054f4(a0, 0);
            }
        }
        Value3(Func_02003632, rec, a0, 2);
        ((void (*)())Func_02005462)();
    }
}
