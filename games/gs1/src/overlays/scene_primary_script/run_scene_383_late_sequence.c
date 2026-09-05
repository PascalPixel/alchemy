#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020072d6();
void Func_0200731c();
void Func_02007324();
void Func_02007356();
void Func_02007378();
void Func_0200738a();
void Func_0200739a();
void Func_020073b8();
void Func_02007938();
void Func_020079b0();
void Func_020079bc();
void Func_020079c8();
void Func_020079ca();
void Func_020079ce();
void Func_020079d0();
void Func_020079da();
void Func_020079dc();
void Func_020079ea();
s32 Func_02007a1a();
s32 Func_02007a26();
s32 Func_02007a30();
s32 Func_02007a3a();
void Func_02007a3e();
s32 Func_02007a46();
void Func_02007a4c();
s32 Func_02007a52();
void Func_02007a56();
void Func_02007a5c();
void Func_02007a62();
void Func_02007a64();
void Func_02007a6c();
void Func_02007a6e();
void Func_02007aae();
void Func_02007ae6();
void Func_02007aea();
void Func_02007af8();
void Func_02007b14();
void Func_02007b16();
void Func_02007b26();
void Func_02007b34();
void Func_02007b4a();
void Func_02007b5e();
void Func_02007b88();
void Func_02007b98();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunLateSequence(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Func_02007938();
    Call3(Func_020079ce, 10, 0x3180000, 0x1a00000);
    Call3(Func_020079dc, 11, 0x3200000, 0x1900000);
    Call3(Func_020079ea, 12, 0x3080000, 0x1a00000);
    Call3(Func_02007a56, 10, 0x3000, 0);
    Call3(Func_02007a62, 11, 0x3000, 0);
    Call3(Func_02007a6e, 12, 0x3000, 0);
    Call3(Func_020079d0, 11, 0xcccc, 0x6666);
    Call3(Func_020079da, 12, 0xcccc, 0x6666);
    Call3(Func_02007a30, 0, 0x3100000, 0x1c00000);
    Call3(Func_02007a3e, 1, 0x3280000, 0x1b00000);
    Call3(Func_02007a4c, 2, 0x3080000, 0x1b00000);
    Func_02007a5c(0, 19);
    Func_02007a64(1, 19);
    Func_02007a6c(2, 19);
    v5 = 2;
    *(u8 *)(Func_02007a1a(0) + 35) = v5;
    *(u8 *)(Func_02007a26(1) + 35) = v5;
    *(u8 *)(Func_02007a30(2) + 35) = v5;
    record = Func_02007a3a(0);
    Func_020079b0(record, 0);
    record = Func_02007a46(2);
    Func_020079bc(record, 0);
    record = Func_02007a52(1);
    Func_020079c8(record, 0);
    Call3(Func_02007b14, 8, 0xb000, 0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    Func_02007b4a(0, 0);
    Func_02007b5e();
    Func_020079ca();
    Func_020072d6();
    Func_02007a4c(60);
    Func_02007356(10, 3, 20);
    Call1(Func_02007b34, 0x12dd);
    Func_0200731c(10, 30);
    Func_02007324(8, 30);
    Call3(Func_02007aea, 11, 0x328, 0x1c8);
    Call3(Func_02007af8, 12, 0x318, 0x1c8);
    Func_02007b16(12);
    Func_02007b88(12, 0, 0);
    Func_02007b26(11);
    Func_02007b98(11, 0, 0);
    Func_02007aae(30);
    Func_020073b8(11, 3, 20);
    Func_02007378(11, 20);
    Func_0200739a(12, 0, 30);
    Func_0200738a(12, 60);
    Func_02007ae6();
}
