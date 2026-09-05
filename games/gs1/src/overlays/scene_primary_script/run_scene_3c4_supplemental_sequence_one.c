#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02001744

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002172();
void Func_020022dc();
void Func_020047ca();
s32 Func_020047ec();
s32 Func_020047fe();
s32 Func_02004838();
void Func_02004842();
void Func_0200485c();
s32 Func_02004860();
void Func_0200489c();
s32 Func_0200489e();
void Func_020048a8();
u8 *Func_020048b8();
void Func_020048de();
void Func_020048f6();
void Func_020048fe();
u8 *Func_02004924();
void Func_0200492e();
void Func_02004934();
void Func_02004942();
void Func_0200494a();
void Func_0200496c();
void Func_0200496e();
void Func_02004978();
void Func_0200497a();
void Func_02004986();
void Func_0200498c();
void Func_020049aa();
void Func_020049b4();
void Func_020049ce();
void Func_02004a02();
void Func_02004a3e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

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

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    u8 *record;
    s32 value;
    s32 arg0;
    s32 arg2;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *slot;

    Func_02004842(a0);
    record = Value1(Func_02004860, 19);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) != 48) {
    } else {
        if (Value1(Func_02004838, 0x202) == 0) {
        } else {
            Func_0200485c(30);
            rec2 = Func_02002172(0x3020000, 0, 0x1120000, 223);
            slot = slot16;
            *(s32 *)(slot + 8) = 0x9999;
            *(s32 *)(slot + 12) = 0x9999;
            *(s32 *)(slot + 4) = 7;
            *(u8 *)(Func_0200489e(19) + 85) = 0;
            Func_0200494a(185);
            for (i = 0; i < 16; i++) {
                Func_020047ca(3);
                record = Func_020048b8(19);
                *(volatile s32 *)((s32)record + 12) += -0x10000;
                rec7 = Value0(Func_020047ec);
                arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
                value = Value0(Func_020047fe);
                arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
                Func_020022dc(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
            }
            Call6(Func_020048a8, 51, 8, 1, 1, 45, 14);
            Func_020048fe(30);
            {
                u8 *record = Func_02004924(19);
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 2);
            }
            Func_0200496e(19, 3);
            Func_0200489c(rec2);
            Call6(Func_020048de, 45, 4, 1, 1, 48, 14);
            Call3(Func_0200498c, 21, 0x3080000, 0xe80000);
            v5 = 1;
            v6 = 3;
            Func_02004a02(188);
            Call6(Func_020048f6, 58, 8, 45, 14, v5, v6);
            Call3(Func_02004934, 0, 0x50000, 0x10000);
            Call3(Func_02004942, -1, -1, 0xe666);
            Func_02004978(20);
            Func_02004a3e(188);
            Call6(Func_0200492e, 59, 8, 45, 14, v5, v6);
            Call3(Func_0200496c, 0, 0x50000, 0x10000);
            Call3(Func_0200497a, -1, -1, 0xe666);
            Func_02004986();
            Func_020049b4(10);
            Call1(Func_020049aa, 0x972);
        }
    }
    Func_020049ce();
}
