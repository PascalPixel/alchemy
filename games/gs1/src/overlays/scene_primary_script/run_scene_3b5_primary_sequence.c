#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000980

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001fb6[];
extern u8 Data_03001ebc[];
void Func_020016e0();
void Func_0200170e();
void Func_02001728();
void Func_02001736();
s32 Func_02001760();
void Func_0200177c();
void Func_0200178a();
void Func_02001796();
void Func_020017a0();
void Func_020017ac();
void Func_020017ae();
void Func_020017b0();
void Func_020017b4();
void Func_020017c2();
void Func_020017ca();
void Func_020017d6();
void Func_020017da();
s32 Func_020017de();
void Func_020017e2();
void Func_020017e8();
void Func_020017ee();
void Func_020017f6();
void Func_020017fe();
void Func_02001822();
void Func_02001828();
void Func_02001830();
void Func_02001832();
s32 Func_02001838();
void Func_02001840();
void Func_02001844();
void Func_02001846();
void Func_02001854();
void Func_02001864();
void Func_02001872();
void Func_0200188a();
void Func_02001890();
void Func_02001894();
void Func_020018a6();
void Func_020018aa();
void Func_020018be();
void Func_020018ca();
void Func_020018e0();
void Func_020018ea();
void Func_020018f6();
void Func_02001900();
void Func_02001902();
void Func_02001904();
void Func_02001910();
void Func_02001914();
void Func_0200191a();
void Func_0200191c();
void Func_02001924();
void Func_02001926();
void Func_02001928();
void Func_02001932();
void Func_02001936();
void Func_02001938();
void Func_02001946();
void Func_02001952();
void Func_02001954();
void Func_02001972();
void Func_0200197e();
void Func_02001988();
void Func_02001992();
void Func_020019c2();
void Func_020019cc();
void Func_020019ce();
void Func_020019d6();
void Func_020019e2();
void Func_020019e6();
void Func_020019ea();
void Func_020019f2();
void Func_020019f8();
void Func_020019fa();
void Func_020019fe();
void Func_02001a1a();
void Func_02001a24();
void Func_02001a28();
void Func_02001a2e();
void Func_02001a34();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    s32 base5_1fb6;

    Func_020016e0();
    Call4(Func_020017c2, -1, -1, -1, 0);
    Call3(Func_02001728, 29, 0x10000, 0x8000);
    Call3(Func_02001736, 30, 0x10000, 0x8000);
    base5_1fb6 = (s32)Data_00001fb6;
    Func_020017ae(base5_1fb6);
    Call3(Func_0200177c, 29, 0x480000, 0xd00000);
    Call3(Func_0200178a, 30, 0x380000, 0xd00000);
    Func_020017ca(32, 15);
    record = Func_02001760(32);
    Func_0200170e(record, 0);
    Call3(Func_020017ac, 32, 0x5f0000, 0x280000);
    Func_02001796(29, 72, 248);
    Func_020017a0(30, 56, 248);
    Call3(Func_020017b4, 0, 64, 0x108);
    Call3(Func_02001840, 0, 0xc000, 0);
    Func_020017d6(29);
    Func_020017ee(29, 1);
    Func_020017f6(30, 1);
    Func_020017fe(0, 1);
    Func_02001828(29, 0, 0);
    Func_02001832(30, 0, 0);
    Func_020017b0(20);
    Call2(Func_0200188a, 29, 0x102);
    Call2(Func_02001894, 30, 0x102);
    Func_02001844(29, 2);
    Func_02001854(30, 2);
    Func_020017da(20);
    Func_0200188a(29, 0);
    Func_020017e8(25);
    Value4(Func_020017de, 52, 0, 12, 7);
    Func_020017e2((base5_1fb6 + 3), 11, 12, 2);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1f4)) = 32;
    if (Value2(Func_02001838, 0, 0) == 0) {
        Func_02001822(20);
        Func_020018aa(30, 2);
        Func_02001830(30);
        Func_02001902(30, 0, 0);
        Func_02001840(30);
        Func_02001846(10);
        Func_020018be(29, 3);
        Func_02001854(20);
        Func_02001926(29, 0, 0);
        Func_02001864(30);
        Func_0200191c(29, 0);
        Func_02001872(20);
        Call3(Func_02001946, 29, 0x4000, 0);
        Call3(Func_02001952, 30, 0x4000, 0);
        Func_02001890(30);
        Func_02001900(29, 3);
        Func_02001910(30, 3);
        Func_020018a6(20);
        Call3(Func_020018e0, 29, 0x1cccc, 0xe666);
        Call3(Func_020018ea, 30, 0x1cccc, 0xe666);
        Func_02001904(29, 232, 248);
        Func_020018ca(2);
        Func_02001914(30, 232, 248);
        Func_02001932(29);
        Func_02001924(29, 248, 248);
        Func_02001936(30, 248, 248);
    } else {
        Func_020018f6(20);
        Func_0200197e(30, 2);
        Func_02001904(30);
        Func_020019d6(30, 0, 0);
        Func_02001914(30);
        Func_0200191a(10);
        Func_02001992(29, 4);
        Func_02001928(20);
        Func_020019fa(29, 0, 0);
        Func_02001938(30);
        bump_step(1);
        Func_020019fe(29, 0);
        Func_02001954(20);
        Call3(Func_02001a28, 29, 0x4000, 0);
        Call3(Func_02001a34, 30, 0x4000, 0);
        Func_02001972(30);
        Func_020019e2(29, 3);
        Func_020019f2(30, 3);
        Func_02001988(20);
        Call3(Func_020019c2, 29, 0x19999, 0xcccc);
        Call3(Func_020019cc, 30, 0x19999, 0xcccc);
        Func_020019e6(29, 72, 184);
        Func_020019f8(30, 56, 184);
    }
    Func_02001a1a(29, 0, 0);
    Func_02001a24(30, 0, 0);
    Func_02001a2e(32, 0, 0);
    Call1(Func_020019ce, 0x8c0);
    Func_020019ea();
}
