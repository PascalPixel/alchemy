#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000e94();
void Func_02000ea0();
void Func_02000eac();
void Func_02000eb8();
void Func_02000ec4();
u8 *Func_02000ec6();
void Func_02000ed0();
u8 *Func_02000ed2();
void Func_02000edc();
u8 *Func_02000ede();
void Func_02000ee8();
u8 *Func_02000eea();
void Func_02000ef4();
u8 *Func_02000ef6();
u8 *Func_02000f02();
u8 *Func_02000f0e();
u8 *Func_02000f1a();
u8 *Func_02000f26();
void Func_02000f36();
void Func_02000f3c();
void Func_02000f42();
void Func_02000f44();
void Func_02000f48();
void Func_02000f4c();
void Func_02000f4e();
void Func_02000f54();
void Func_02000f5a();
void Func_02000f5c();
void Func_02000f60();
void Func_02000f64();
void Func_02000f66();
void Func_02000f6c();
void Func_02000f72();
void Func_02000f74();
void Func_02000f78();
void Func_02000f7c();
void Func_02000f7e();
void Func_02000f84();
void Func_02000f8a();
void Func_02000f8c();
void Func_02000f94();
void Func_02000f9c();
void Func_02000fa4();
void Func_02000fac();
void Func_02000fb4();
s32 Func_02000fd4();
void Func_02001014();
void Func_0200101a();
s32 Func_02001020();
void Func_0200102a();
u8 *Func_0200102c();
u8 *Func_02001036();
void Func_0200103a();
s32 Func_02001058();
void Func_02001060();
u8 *Func_0200106c();
void Func_02001070();
void Func_02001072();
void Func_02001076();
void Func_0200108a();
void Func_02001090();
void Func_02001092();
void Func_020010a2();
void Func_020010a4();
s32 Func_020010c0();
void Func_020010d4();
s32 Func_020010d8();
void Func_02001108();
void Func_02001120();

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

void FieldScene_RunScene398SequenceC(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    record = Func_02000ec6(18);
    Func_02000e94((s32)record, 0);
    record = Func_02000ed2(19);
    Func_02000ea0((s32)record, 0);
    record = Func_02000ede(20);
    Func_02000eac((s32)record, 0);
    record = Func_02000eea(21);
    Func_02000eb8((s32)record, 0);
    record = Func_02000ef6(22);
    Func_02000ec4((s32)record, 0);
    record = Func_02000f02(23);
    Func_02000ed0((s32)record, 0);
    record = Func_02000f0e(24);
    Func_02000edc((s32)record, 0);
    record = Func_02000f1a(25);
    Func_02000ee8((s32)record, 0);
    record = Func_02000f26(26);
    Func_02000ef4((s32)record, 0);
    Func_02000f44(18, 5);
    Func_02000f4c(19, 5);
    Func_02000f54(20, 5);
    Func_02000f5c(21, 5);
    Func_02000f64(22, 5);
    Func_02000f6c(23, 3);
    Func_02000f74(24, 3);
    Func_02000f7c(25, 3);
    Func_02000f84(26, 3);
    Func_02000f8c(9, 2);
    Func_02000f94(10, 2);
    Func_02000f9c(11, 2);
    Func_02000fa4(12, 2);
    Func_02000fac(13, 2);
    Func_02000fb4(14, 2);
    Func_02000f36(18);
    Func_02000f3c(19);
    Func_02000f42(20);
    Func_02000f48(21);
    Func_02000f4e(22);
    Func_02000f54(23);
    Func_02000f5a(24);
    Func_02000f60(25);
    Func_02000f66(26);
    Func_02000f6c(9);
    Func_02000f72(10);
    Func_02000f78(11);
    Func_02000f7e(12);
    Func_02000f84(13);
    Func_02000f8a(14);
    if (Value1(Func_02000fd4, 0x883) != 0) {
        Func_0200101a(8, 0, 0);
        Func_0200102a(15, 5);
        *(u8 *)(Func_02001020(15) + 85) = 0;
        record = Func_0200102c(15);
        *(volatile s32 *)((s32)record + 12) = -0x40000;
        {
            u8 *record = Func_02001036(15);
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 2);
        }
        Func_02001070(15, 2);
        Call6(Func_02001014, 0, 0, 1, 1, 18, 14);
    } else {
        Func_02001076(8, 2);
        record = Func_0200106c(8);
        Func_0200103a((s32)record, 0);
        Func_0200108a(15, 1);
    }
    Func_02001092(16, 1);
    if (Value1(Func_02001058, 0x302) != 0) {
        v5 = 36;
        Func_020010a4(17, 1);
        Call6(Func_02001060, 0, 1, 1, 1, v5, 22);
        Call6(Func_02001072, 0, 2, 1, 1, v5, 24);
    } else {
        v5 = 36;
        Func_020010d4(17, 5);
        Call6(Func_02001090, 1, 1, 1, 1, v5, 22);
        Call6(Func_020010a2, 1, 2, 1, 1, v5, 24);
    }
    if (Value1(Func_020010c0, 0x303) != 0) {
        Call3(Func_02001108, 11, 0x23a0000, 0x1780000);
    }
    if (Value1(Func_020010d8, 0x304) != 0) {
        Call3(Func_02001120, 12, 0x23a0000, 0x1780000);
    }
}
