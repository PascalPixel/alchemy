#include "types.h"

#define FieldScene_RunScene3a5_020014b0 Func_020014b0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002512();
void Func_02003160();
s32 Func_02003208();
s32 Func_02003216();
void Func_0200322e();
s32 Func_02003270();
s32 Func_02003278();
s32 Func_02003280();
s32 Func_0200328c();
void Func_0200328e();
void Func_020032a6();
s32 Func_020032c4();
void Func_020032ce();
s32 Func_020032dc();
void Func_020032f6();
void Func_020032f8();
void Func_020032fc();
void Func_0200330e();
void Func_02003324();
s32 Func_0200332c();
void Func_02003334();
void Func_02003338();
void Func_0200333a();
void Func_02003348();
void Func_02003366();
void Func_0200337e();
void Func_02003380();
void Func_02003398();
void Func_0200339a();
void Func_02003412();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3a5_020014b0(void)
{
    s32 rec8;
    s32 record;
    s32 rect[3];
    s32 shown;
    u16 *shown_addr;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02002512();
    if (Value1(Func_02003208, 0x90a) != 0) {
    } else {
        rec8 = Value1(Func_02003216, 0x200);
        if (rec8 != 0) {
        } else {
            Call1(Func_0200322e, 0x200);
            Func_02003160(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Value1(Func_02003270, 0);
            *(s32 *)(record + 36) = rec8;
            record = Value1(Func_02003278, 0);
            *(s32 *)(record + 44) = rec8;
            record = Func_02003280(0);
            *(s32 *)(record + 56) = -0x80000000;
            record = Func_0200328c(0);
            *(s32 *)(record + 64) = -0x80000000;
            Func_020032ce(0, 1);
            Func_020032f8(0, 8, 0);
            Func_0200328e(40);
            Func_020032f6(0, 2);
            Call2(Func_02003338, 0, 0x102);
            Func_020032a6(40);
            *(u8 *)(Func_020032c4(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Func_020032dc(0);
            Call3(Func_02003216, -0x100000, *(u16 *)(record + 6), (s32)rect);
            Call3(Func_020032fc, 0, 0x20000, 0x10000);
            Func_02003334(0, 2);
            Func_0200333a(0, rect[0] / 0x10000, rect[2] / 0x10000);
            Func_02003348(0);
            Func_0200330e(2);
            *(u8 *)(Func_0200332c(0) + 90) |= 1;
            Func_02003324(30);
            Func_02003412(148);
            Func_0200339a(8, 2);
            Func_02003338(20);
            Call3(Func_02003366, 8, 0x28000, 0x14000);
            Func_02003380(8, 168, 104);
            Call3(Func_0200337e, 8, 0x8000, 0x4000);
            Func_02003398(8, 168, 92);
            *shown_addr = shown;
            Func_0200328e(0);
        }
    }
    /* unlifted: 0x02001590..0x02001594 (2), 0x0200159c..0x020015a0 (2) */
}
