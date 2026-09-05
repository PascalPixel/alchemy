#include "types.h"

#define FieldScene_RunScene399SequenceB Func_02000f90

extern u8 Data_0200a74c[];
extern u8 Data_0200a5ec[];
extern u8 Data_0200a760[];
extern u8 Data_03001ebc[];

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002800();
void Func_0200312a();
void Func_020031c4();
void Func_02003248();
void Func_02003252();
void Func_0200325c();
s32 Func_02003270();
s32 Func_02003284();
void Func_020032a6();
void Func_020032c2();
void Func_020032d2();
void Func_020032dc();
void Func_020032e6();
void Func_020032ec();
void Func_020032ee();
void Func_020032fa();
void Func_020032fc();
void Func_02003302();
void Func_0200331c();
void Func_0200331e();
s32 Func_02003322();
void Func_02003332();
void Func_02003342();
void Func_02003352();
void Func_0200336a();
void Func_0200336e();
void Func_02003374();
void Func_0200337e();
void Func_0200338a();
s32 Func_0200338c();
void Func_02003392();
void Func_020033a0();
void Func_020033b2();
void Func_020033b8();
void Func_020033c4();
void Func_020033c8();
void Func_020033d2();
void Func_020033da();
void Func_020033dc();
void Func_020033e0();
void Func_020033e4();
void Func_020033ee();
void Func_020033f8();
void Func_02003400();
void Func_0200340a();
void Func_0200340c();
void Func_02003420();
void Func_02003436();
void Func_0200343c();
void Func_02003440();
void Func_02003442();
void Func_02003448();
void Func_0200344e();
void Func_02003458();
void Func_0200345c();
void Func_02003460();
void Func_02003466();
void Func_0200346e();
void Func_02003472();
void Func_0200347a();
void Func_02003480();
u8 *Func_02003482();
void Func_02003484();
void Func_02003490();
void Func_02003498();
void Func_020034a0();
void Func_020034a2();
void Func_020034b8();
void Func_020034d2();
s32 Func_020034d6();
void Func_020034e0();
void Func_020034f0();
void Func_020034f2();
void Func_020034fa();
void Func_02003510();
void Func_02003518();
void Func_0200351a();
void Func_0200351c();
void Func_02003554();
void Func_02003558();
void Func_02003580();
void Func_02003598();
void Func_020035a4();
void Func_020035b2();
void Func_020035d4();
void Func_020035e2();
void Func_020035ea();
void Func_020035f2();
u8 *Func_02003616();
void Func_02003618();
void Func_0200361c();
void Func_02003622();
void Func_02003630();
s32 Func_02003632();
void Func_02003634();
void Func_02003638();
void Func_0200365a();
void Func_02003662();
void Func_02003670();
void Func_02003676();
void Func_0200367c();
void Func_02003684();
void Func_0200368a();
void Func_02003692();
s32 Func_02003696();
void Func_020036a4();
s32 Func_020036a8();
void Func_020036b0();
s32 Func_020036b8();
void Func_020036bc();
void Func_020036c8();
void Func_020036ca();
void Func_020036d0();
void Func_020036ec();
u8 *Func_020036f0();
void Func_020036fa();
void Func_020036fc();
u8 *Func_02003700();
void Func_02003704();
u8 *Func_0200370e();
void Func_02003720();
void Func_02003728();
void Func_0200372c();
void Func_02003732();
void Func_02003738();
void Func_0200373c();
void Func_0200374e();
void Func_0200375c();
void Func_0200376e();
void Func_02003830();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene399SequenceB(void)
{
    u32 i;
    u8 *record;
    s32 base5_200a74c;
    s32 base5_200a5ec;
    s32 base5_200a760;

    Func_020031c4();
    Call3(Func_02003252, 3, 0xb60000, 0x960000);
    Call4(Func_020032fc, 0x8d0000, -1, 0xdd0000, 0);
    Func_0200312a(1);
    Call2(Func_02003302, 0x4ccc, 0x999);
    Call4(Func_0200331c, 0x8c0000, -1, 0xa40000, 1);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 40;
    Func_0200336e();
    Call3(Func_02003248, 0, 0x6666, 0x3333);
    Call3(Func_02003252, 1, 0x6666, 0x3333);
    Call3(Func_0200325c, 2, 0x6666, 0x3333);
    Func_020032a6(0, 142, 221);
    Call3(Func_02003342, 0, 0xd000, 0);
    record = Value1(Func_02003270, 0);
    if (record != 0) {
        Func_020032e6(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003284, 0);
    if (record != 0) {
        Func_020032fa(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_020032dc(1, 150, 234);
    Func_020032ee(2, 134, 234);
    Func_0200331e(1, 1);
    base5_200a74c = (s32)Data_0200a74c;
    Call3(Func_0200336a, 0, 0x10003, base5_200a74c);
    Call3(Func_02003374, 1, 0x10003, base5_200a74c);
    Call3(Func_0200337e, 2, 0x10003, base5_200a74c);
    Func_020033da();
    base5_200a5ec = (s32)Data_0200a5ec;
    Func_020032ec(9, base5_200a5ec);
    Func_020032c2(40);
    Call2(Func_020033dc, 3, 0x102);
    Func_020032d2(40);
    Func_0200338a(3, 1);
    Call1(Func_020033b8, 0x155c);
    Func_020033d2(3, 0, 20);
    Value2(Func_02003322, 9, base5_200a5ec);
    Func_020033e4(9, 0, 20);
    Call3(Func_02003400, 3, 0x8000, 20);
    Func_0200340a(8, 0, 10);
    Func_020033b2(8, 4);
    Func_0200340c(8, 0, 40);
    Func_020033c4(3, 3);
    Func_02003332(10);
    Call3(Func_02003436, 3, 0x4000, 0);
    Call3(Func_02003442, 8, 0x3000, 20);
    Func_0200343c(3, 0, 10);
    Value2(Func_0200338c, 9, base5_200a5ec);
    Func_02002800();
    Call3(Func_02003472, 3, 0x101, 60);
    Func_0200345c(3, 0, 40);
    Func_02003466(9, 0, 20);
    Call3(Func_02003490, 8, 0x105, 60);
    Func_02003420(9, 7);
    Call3(Func_02003352, 0x200b1c0, 10, 69);
    Func_020033a0(10);
    Func_02003458(3, 2);
    Func_02003448(3, 4);
    Func_020034a2(3, 0, 20);
    Func_02003472(9, 1);
    Func_020033c8(40);
    Func_02003460(9, 8);
    Call3(Func_02003392, 0x200b1d6, 10, 69);
    Func_020033e0(40);
    Func_02003480(3, 3);
    Func_020033ee(20);
    Func_020034f0(8, 0, 20);
    Func_02003498(8, 3);
    Func_020034f2(8, 0, 10);
    Call3(Func_0200351c, 3, 0x101, 30);
    Call3(Func_02003518, 3, 0x8000, 10);
    Func_020034b8(3, 4);
    Func_0200351a(3, 0, 10);
    Func_020034d2(8, 3);
    Func_02003440(20);
    Func_020034e0(3, 3);
    Func_0200344e(40);
    Call3(Func_02003484, 3, 0x10000, 0x8000);
    {
        u8 *record = Func_02003482(3);
        s32 shown = 0;
    
        *(u16 *)(record + 100) = shown;
    }
    Call2(Func_020034a0, 3, 0x200a670);
    while (*(s16 *)(Func_020034d6(3) + 100) == 0) {
        Func_020033f8(1);
    }
    Call4(Func_020035f2, 0x8c0000, -1, 0xc60000, 1);
    Func_02003510(3);
    Call3(Func_020035ea, 3, 0x101, 80);
    Func_020035d4(3, 0, 40);
    Func_020035a4(3, 1);
    Func_020034fa(10);
    Func_020035e2(3, 0);
    Func_020036c8(131);
    Call2(Func_0200365a, 0x10000, 0);
    Call2(Func_0200365a, 0x207e9f, 0);
    Func_02003670(10);
    Func_0200346e(1);
    Func_020036ec(220);
    Func_0200347a(40);
    Call2(Func_0200367c, 0x10000, 0);
    Func_02003692(60);
    Func_02003490(60);
    Call2(Func_02003662, 3, 0x102);
    Func_02003558(20);
    Func_0200365a(3, 0, 10);
    Call3(Func_02003598, 3, 0x20000, 0x10000);
    Func_020035e2(3, 202, 198);
    Func_02003580(40);
    Func_02003638(3, 2);
    Func_02003670(3, 0);
    Func_02003630(3, 4);
    Func_0200368a(3, 0, 20);
    Call2(Func_020036bc, 3, 0x102);
    Func_020035b2(40);
    Func_020036a4(3, 0, 40);
    Call3(Func_020036d0, 3, 0x100, 40);
    Func_020036b0(3, 0);
    Call1((void (*)())Func_02003616, 0);
    Func_0200361c(1);
    Func_02003622(2);
    Call3(Func_02003618, 3, 0x30000, 0x18000);
    {
        u8 *record = Func_02003616(3);
        s32 shown = 0;
    
        *(u16 *)(record + 100) = shown;
    }
    Call2(Func_02003634, 3, 0x200a6e0);
    while (*(s16 *)(Func_02003632(3) + 100) == 0) {
        Func_02003554(1);
    }
    Call3(Func_02003720, 0, 0x4000, 0);
    Call3(Func_0200372c, 1, 0x4000, 0);
    Call3(Func_02003738, 2, 0x4000, 10);
    Call3(Func_02003676, 0, 0x40000, 0x20000);
    Call3(Func_02003684, 1, 0x40000, 0x20000);
    Call3(Func_02003692, 2, 0x40000, 0x20000);
    Func_02003830(152);
    *(u8 *)(Func_02003696(0) + 90) &= 254;
    *(u8 *)(Func_020036a8(1) + 90) &= 254;
    *(u8 *)(Func_020036b8(2) + 90) &= 254;
    Func_020036fa(0, 132, 206);
    Func_02003704(1, 136, 221);
    Call3((void (*)())Func_0200370e, 2, 122, 238);
    Func_020036fc(3);
    Func_020036ca(80);
    {
        u8 *record = Func_020036f0(0);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    {
        u8 *record = Func_02003700(1);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    {
        u8 *record = Func_0200370e(2);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Call3(Func_02003728, 0, 0xcccc, 0x6666);
    Call3(Func_02003732, 1, 0xcccc, 0x6666);
    Call3(Func_0200373c, 2, 0xcccc, 0x6666);
    base5_200a760 = (s32)Data_0200a760;
    Func_0200374e(1, base5_200a760);
    Func_0200376e(2, base5_200a760);
    Func_0200372c(20);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x209;
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 24;
    Call1(Func_02003738, 0x82b);
    Func_0200375c();
}
