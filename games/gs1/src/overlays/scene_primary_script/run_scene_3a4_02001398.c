#include "types.h"

#define FieldScene_RunScene3a4_02001398 Func_02001398

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001953[];
extern u8 Data_03001ebc[];
void Func_020024fe();
s32 Func_02004eee();
s32 Func_02004efc();
s32 Func_02004f08();
void Func_02004f0c();
void Func_02004f1c();
void Func_02004f6e();
s32 Func_02004f8e();
void Func_02004fa2();
void Func_02004fae();
void Func_02004fb0();
void Func_02004fbc();
void Func_02004fda();
void Func_02004fe4();
void Func_02004fec();
void Func_02004ff2();
s32 Func_02004ff6();
void Func_02004ff8();
void Func_02005016();
s32 Func_0200501a();
void Func_02005024();
void Func_0200502a();
void Func_02005032();
s32 Func_02005040();
void Func_0200504c();
void Func_0200504e();
void Func_0200505c();
u8 *Func_02005060();
void Func_02005068();
void Func_02005076();
void Func_0200508c();
void Func_02005094();
void Func_02005146();
void Func_0200516a();
void Func_02005170();
void Func_02005174();
void Func_0200517a();
void Func_02005182();
void Func_02005184();
void Func_02005190();
void Func_02005196();
void Func_02005198();
void Func_0200519c();
void Func_0200519e();
void Func_020051a0();
void Func_020051a6();
void Func_020051b0();
void Func_020051b2();
void Func_020051ba();
void Func_020051bc();
void Func_020051be();
void Func_020051c8();
void Func_020051d2();
void Func_020051d6();
void Func_020051da();
void Func_020051e0();
void Func_020051e8();
void Func_0200520a();
void Func_02005214();
void Func_02005218();
void Func_0200521a();
void Func_0200522c();
void Func_02005276();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene3a4_02001398(void)
{
    u32 i;
    u8 *record;
    s32 base5_1953;

    Func_02004f0c();
    base5_1953 = (s32)Data_00001953;
    Value2(Func_02004eee, base5_1953, 1);
    if (Value1(Func_02004efc, 0x908) != 0) {
    } else {
        if (Value1(Func_02004f08, 0xf14) != 0) {
        } else {
            Call1(Func_02004f1c, 0x205);
            Call3(Func_02004f6e, 0, 0xcccc, 0x6666);
            Call3(Func_02004fb0, 0, 0x316, 140);
            Call3(Func_02004fbc, 0, 0x30c, 140);
            Call3(Func_02005040, 0, 0xc000, 0);
            record = Value1(Func_02004f8e, 0);
            if ((s32)record != 0) {
                Func_02004fec(1, *(volatile s32 *)((s32)record + 8), *(volatile s32 *)((s32)record + 16));
            }
            Call3(Func_02004fae, 1, 0xcccc, 0x6666);
            Call3(Func_02004ff2, 1, 0x320, 140);
            Call3(Func_02005076, 1, 0xc000, 20);
            Func_0200505c((base5_1953 + 1));
            Func_02005024(1, 4);
            Func_02004fa2(20);
            Func_0200508c(1, 0, 10);
            Func_0200504e(1, 6, 0);
            Call3(Func_02004ff8, 1, 0x19999, 0xcccc);
            *(u8 *)(Func_02004ff6(1) + 90) &= 254;
            Call3(Func_0200504e, 1, 0x318, 110);
            Func_02004fe4(1);
            *(u8 *)(Func_0200501a(1) + 90) |= 1;
            Func_020051b2(161);
            Call3(Func_02004fda, 0x20000, 0x10000, 0x10000);
            *(u8 *)(Func_02005040(1) + 90) &= 254;
            Call3(Func_02005094, 1, 0x318, 120);
            Func_0200502a(1);
            {
                u8 *record = Func_02005060(1);
                u8 value = *(volatile u8 *)&record[90];
            
                record[90] = (u8)(value | 1);
            }
            Call3(Func_02005016, -1, -1, 0xe666);
            Func_0200504c(80);
            Func_0200520a(141);
            Call3(Func_02005032, 0x10000, 0x10000, 0x10000);
            Func_02005068(40);
            Call3(Func_0200516a, 0, 0x101, 0);
            Call3(Func_02005174, 1, 0x101, 60);
            Call3(Func_02005170, 0, 0x8000, 0);
            Func_0200517a(1, 0, 20);
            Func_02005184(0, 0, 0);
            Call3(Func_02005190, 1, 0x8000, 40);
            Call3(Func_0200519c, 0, 0x8000, 0);
            Func_020051a6(1, 0, 40);
            Call3(Func_020051b2, 0, 0xc000, 0);
            Call3(Func_020051be, 1, 0xc000, 40);
            Call3(Func_020051da, 1, 0x102, 60);
            Call3(Func_020051d6, 1, 0x4000, 20);
            Func_02005196(1, 2);
            Func_020051e0(1, 0, 10);
            Call3(Func_02005146, 1, 0x28000, 0x14000);
            Func_0200519e(1, 5);
            Call3(Func_02005182, 1, 0x31c, 138);
            Func_02005214(0, 0, 0);
            Call3(Func_02005198, 1, 0x324, 140);
            Call3(Func_0200522c, 0, 0x4000, 0);
            Call3(Func_020051b0, 1, 0x324, 166);
            Call3(Func_020051bc, 1, 0x2fc, 166);
            Call3(Func_020051c8, 1, 0x2fc, 198);
            Call3(Func_020051d2, 1, 0x312, 198);
            Call3(Func_02005276, 0, 0x102, 0);
            Call3(Func_020051e8, 1, 0x312, 246);
            Func_02005218(1, 1);
            Func_0200521a(1, 0, 0);
            Func_020051a0(40);
            Func_020024fe(10);
        }
    }
    Func_020051ba();
}
