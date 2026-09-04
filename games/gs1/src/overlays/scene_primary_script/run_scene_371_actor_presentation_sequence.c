#include "types.h"

#define FieldScene_RunActorPresentationSequence Func_02002cb4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000000[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
void Func_02006e8e();
void Func_02006f44();
void Func_02006fa8();
void Func_0200701e();
void Func_0200702c();
void Func_0200704a();
void Func_02007056();
void Func_0200705e();
void Func_02007060();
void Func_020070d6();
void Func_020070dc();
void Func_020070de();
void Func_020070e2();
void Func_020070ec();
void Func_020070f4();
void Func_02007102();
void Func_02007124();
void Func_02007134();
void Func_0200713c();
void Func_02007150();
void Func_0200715a();
void Func_0200715e();
void Func_02007164();
void Func_0200716c();
void Func_02007172();
void Func_02007176();
void Func_02007178();
void Func_02007192();
void Func_02007194();
void Func_02007198();
void Func_0200719e();
void Func_020071a0();
void Func_020071a2();
void Func_020071b2();
void Func_020071b4();
void Func_020071b8();
void Func_020071c2();
void Func_020071c4();
void Func_020071c6();
void Func_020071ce();
void Func_020071d4();
void Func_020071da();
void Func_020071de();
void Func_020071e2();
void Func_020071f0();
void Func_020071f4();
void Func_020071fe();
void Func_02007202();
void Func_02007212();
void Func_02007218();
void Func_0200721e();
void Func_02007220();
void Func_02007228();
void Func_02007238();
void Func_02007248();
void Func_0200724a();
void Func_0200724c();
void Func_02007252();
void Func_0200725c();
s32 Func_02007260();
void Func_02007264();
void Func_02007268();
void Func_0200726e();
void Func_02007278();
void Func_0200729e();
void Func_020072a8();
void Func_020072b4();
void Func_020072bc();
void Func_020072c0();
void Func_020072cc();
void Func_02007302();
void Func_0200730a();
void Func_02007314();
void Func_0200731e();
void Func_02007334();
void Func_02007344();
void Func_0200734e();
void Func_02007360();
void Func_02007366();
void Func_02007370();
void Func_02007388();
void Func_02007390();
void Func_020073b8();
void Func_020073ba();
void Func_020073c6();
void Func_020073cc();
void Func_020073dc();
void Func_020073e2();
void Func_020073fa();
void Func_020073fc();
void Func_0200740e();
void Func_02007422();
void Func_0200742c();
void Func_02007484();
void Func_0200749a();
void Func_020074a6();
void Func_020074bc();
void Func_0200751e();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunActorPresentationSequence(void)
{
    u32 i;
    s32 record;
    s32 base2_2000240;
    s32 base3_2000240;

    base2_2000240 = (s32)Data_02000240;
    base3_2000240 = (s32)Data_02000240;
    Func_02006f44(*(u8 *)((base2_2000240 + 0x205)), *(u8 *)((base3_2000240 + 0x206)));
    Func_02006fa8();
    Call2(Func_02007164, 0x10000, 0x12c);
    Call4(Func_020070de, -1, -1, -1, 0);
    Func_02007056(5, 19);
    Func_0200705e(8, 5);
    Func_02007060(0, 0, 0);
    Func_02006e8e(1);
    Call2(Func_020071a0, 0x18000, 16);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call2(Func_02007150, 0x10003, 1);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Func_02007178();
    Func_020071d4();
    Func_020071a0();
    Func_0200701e(40);
    Func_020070d6(5, 1);
    Func_0200702c(20);
    Call1(Func_02007102, 0x2913);
    Func_02007124(5, 0, 20);
    Func_020070f4(8, 2);
    Func_0200704a(20);
    Func_0200713c(8, 0, 20);
    Call3(Func_0200715e, 5, 0x107, 20);
    Func_02007150(5, 0, 20);
    Call3(Func_02007172, 8, 0x105, 80);
    Func_02007164(8, 0, 10);
    Func_02007134(5, 2);
    Func_02007176(5, 0, 20);
    Call3(Func_02007198, 8, 0x105, 100);
    Call3(Func_020071a2, 5, 0x105, 40);
    Func_02007194(5, 0, 20);
    Func_0200719e(8, 0, 10);
    Call3(Func_020071c2, 5, 0x102, 20);
    Func_020071b4(5, 0, 10);
    Call2(Func_020071de, 8, 0x102);
    Func_020070dc(80);
    Func_020071ce(8, 0, 20);
    Call3(Func_020071f0, 5, 0x105, 80);
    Func_020071e2(8, 0, 120);
    Func_020071b2(5, 1);
    Func_020071f4(5, 0, 40);
    Func_020071fe(8, 0, 20);
    Call3(Func_02007220, 5, 0x105, 40);
    Func_02007212(5, 0, 120);
    Call3(Func_0200716c, 9, 0x6666, 0x3333);
    Call3(Func_020071c6, 9, 0x1ddc0000, 0xd840000);
    Call3(Func_020071b8, 9, 0x1d94, 0xd8c);
    Call3(Func_020071c4, 9, 0x1d88, 0xda0);
    Func_0200715a(20);
    Call3(Func_0200724c, 0x6009, 0, 20);
    Call3(Func_0200726e, 8, 0x101, 0);
    Call3(Func_02007278, 5, 0x101, 60);
    Func_02007218(9, 3);
    Call2(Func_02007268, 0x6009, 0);
    Func_020070ec();
    Func_02007192(20);
    Call2(Func_020071da, 9, 0x200cf7c);
    Func_020071a0(80);
    Func_02007238(8, 1);
    Func_02007252(8, 4, 40);
    Func_0200724a(5, 1);
    Func_02007264(5, 4, 60);
    Call3(Func_020072c0, 8, 0x3000, 0);
    Call3(Func_020072cc, 5, 0xb000, 40);
    Call3(Func_0200721e, 8, 0x9999, 0x4ccc);
    Call3(Func_02007228, 5, 0x9999, 0x4ccc);
    Call2(Func_02007238, 8, 0x200d01c);
    Func_020071fe(20);
    Call2(Func_0200731e, 0xb333, 0x1666);
    Call4(Func_02007334, 0x1e380000, -1, 0xdc80000, 1);
    Func_0200725c(5, 0x200d0a8);
    do {
        Func_020072b4(10, 6);
        Func_020072bc(6, 8);
        Func_020070e2(1);
    } while (*(s16 *)(Func_02007260(5) + 100) == 0);
    Func_02007248(20);
    Call3(Func_02007344, 9, 0x8000, 20);
    Call2(Func_02007366, 8, 0x102);
    Call2(Func_02007370, 5, 0x102);
    Func_0200726e(40);
    Func_020073fa();
    Call11(Func_02007390, 5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
    Func_0200729e(20);
    Func_02007202();
    Call2(Func_020073c6, 0x10000, 0x2000);
    Call4(Func_020073dc, 0x1e580000, -1, 0xdc80000, 1);
    Call3(Func_020073b8, 9, 0x3000, 0);
    Call3(Func_0200730a, 8, 0x19999, 0xcccc);
    Call3(Func_02007314, 5, 0x19999, 0xcccc);
    Call3(Func_0200734e, 8, 0x1e7c, 0xdb8);
    Call3(Func_02007360, 5, 0x1e6c, 0xdd8);
    Func_02007388(8, 1);
    Func_02007484();
    Func_02007302(80);
    Func_020073ba(8, 1);
    Func_020073fc(8, 0, 20);
    Func_020073cc(5, 2);
    Call3(Func_0200740e, 0x1005, 0, 40);
    Call3(Func_02007422, 8, 0x8000, 20);
    Func_020073e2(8, 2);
    Func_0200742c(8, 0, 60);
    Func_020072a8();
    Func_0200751e(17);
    Func_020074a6(0, 0);
    Func_020074bc(120);
    Func_02007212(120);
    Func_0200749a((s32)Data_00000000, 10);
}
