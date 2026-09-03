#include "types.h"

#define FieldScene_RunMultiActorPresentation Func_020004a8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000250d[];
extern u8 Data_00002512[];
extern u8 Data_03001ebc[];
void Func_02001026();
s32 Func_02001fb2();
void Func_02001fc6();
void Func_02001fe2();
void Func_0200201e();
void Func_0200202a();
void Func_02002038();
void Func_0200203a();
void Func_02002050();
void Func_02002068();
s32 Func_0200206e();
void Func_0200208e();
void Func_02002094();
s32 Func_020020aa();
void Func_020020ac();
void Func_020020bc();
void Func_020020c0();
void Func_020020ca();
s32 Func_020020e6();
void Func_020020e8();
void Func_020020fc();
void Func_020020fe();
void Func_02002106();
void Func_02002118();
s32 Func_02002122();
void Func_02002124();
void Func_02002138();
void Func_0200213e();
void Func_02002142();
void Func_0200215c();
void Func_0200215e();
void Func_02002160();
void Func_0200216a();
void Func_02002174();
void Func_0200219c();
void Func_020021b6();
void Func_020021c0();
void Func_020021c4();
void Func_020021d6();
void Func_020021d8();
s32 Func_020021e0();
void Func_020021ee();
void Func_020021f8();
void Func_02002206();
void Func_02002210();
void Func_02002226();
void Func_02002228();
void Func_02002232();
void Func_02002234();
s32 Func_02002236();
void Func_0200225a();
void Func_0200226a();
void Func_02002272();
void Func_02002276();
void Func_0200227e();
void Func_02002282();
void Func_02002286();
void Func_0200228e();
void Func_0200229e();
void Func_020022a6();
void Func_020022ae();
void Func_020022b6();
void Func_020022b8();
void Func_020022d0();
void Func_020022d2();
void Func_020022de();
void Func_020022e0();
void Func_020022ea();
void Func_020022f6();
void Func_02002304();
void Func_02002306();
void Func_02002308();
void Func_02002312();
void Func_0200231e();
void Func_02002320();
void Func_02002326();
void Func_02002336();
void Func_02002338();
void Func_02002342();
void Func_02002372();
void Func_02002376();
void Func_0200238a();
void Func_02002394();
void Func_020023cc();
void Func_02002400();
void Func_0200240c();

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

void FieldScene_RunMultiActorPresentation(void)
{
    u32 i;
    s32 record;
    s32 base5_250d;
    s32 base5_2512;

    if (Value1(Func_02001fb2, 0x941) == 0) {
    } else {
        Call1(Func_02001fc6, 0x94d);
        Func_02001fe2();
        Call3(Func_02002038, 12, 0x900000, 0x1900000);
        Call3(Func_0200201e, 12, 0x18000, 0xc000);
        Call3(Func_0200203a, 12, 184, 0x190);
        Func_02002050(12);
        Func_02002068(12, 1);
        Call3(Func_020020bc, 12, 0x3000, 0);
        Func_02002094(0, 1);
        Func_0200202a(30);
        Call2(Func_020020fe, 0x8000, 0x1000);
        Call4(Func_02002118, 0xc00000, -1, 0x1b00000, 1);
        record = Value1(Func_0200206e, 0);
        if (record != 0) {
            Func_020020ac(13, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Call3(Func_0200208e, 13, 0x14ccc, 0xa666);
        Call3(Func_020020aa, 13, 168, 0x1d0);
        Func_020020c0(13);
        Call3(Func_02002124, 13, 0xc000, 0);
        record = Value1(Func_020020aa, 0);
        if (record != 0) {
            Func_020020e8(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Call3(Func_020020ca, 2, 0x14ccc, 0xa666);
        Call3(Func_020020e6, 2, 152, 0x1e8);
        Func_020020fc(2);
        Call3(Func_02002160, 2, 0xc000, 0);
        record = Value1(Func_020020e6, 0);
        if (record != 0) {
            Func_02002124(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Call3(Func_02002106, 3, 0x14ccc, 0xa666);
        Call3(Func_02002122, 3, 168, 0x1e8);
        Func_02002138(3);
        Call3(Func_0200219c, 3, 0xc000, 0);
        record = Value1(Func_02002122, 0);
        if (record != 0) {
            Func_02002160(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Call3(Func_02002142, 1, 0x14ccc, 0xa666);
        Call3(Func_0200215e, 1, 184, 0x1e8);
        Func_02002174(1);
        Call3(Func_020021d8, 1, 0xc000, 0);
        Func_0200213e(30);
        Func_020021b6(1, 1);
        base5_250d = (s32)Data_0000250d;
        Func_020021d6(base5_250d);
        Func_020021ee(1, 0);
        Func_0200215c(30);
        Func_020021c4(3, 3);
        Func_0200216a(10);
        Func_020021f8((base5_250d + 1));
        Func_02002210(3, 0);
        Call3(Func_02002234, 2, 0x102, 70);
        Func_02002206(2, 0, 0);
        Func_02002210(0, 2, 0);
        Func_02002226((base5_250d + 2));
        Value2(Func_02002236, 2, 0);
        Func_02002228(3, 0, 0);
        Func_02002232(1, 0, 0);
        Func_020021c0(30);
        if (Value2(Func_020021e0, 0, 0) == 0) {
            Func_0200225a((base5_250d + 3));
            Func_02002272(1, 0);
        } else {
            Func_0200226a((base5_250d + 4));
            Func_02002282(1, 0);
        }
        Call3(Func_020022a6, 13, 0x100, 70);
        base5_2512 = (s32)Data_00002512;
        Func_02002286(base5_2512);
        Func_0200229e(13, 0);
        Func_02002276(0, 2);
        Func_0200227e(1, 2);
        Func_02002286(2, 2);
        Func_0200228e(3, 2);
        Call3(Func_020022d2, 0, 0xc000, 0);
        Call3(Func_020022de, 1, 0xc000, 0);
        Call3(Func_020022ea, 2, 0xc000, 0);
        Call3(Func_020022f6, 3, 0xc000, 0);
        Func_020022b6(13, 2);
        Call3(Func_02002372, 13, 0, -16);
        Func_020022b8(13);
        Func_020022d0(13, 1);
        Func_02002306((base5_2512 + 1));
        Func_0200231e(13, 0);
        Call3(Func_02002342, 12, 0x100, 65);
        Func_02002320((base5_2512 + 2));
        Func_02002338(12, 0);
        Func_02002308(13, 3);
        Func_020022ae(80);
        Call3(Func_020022e0, 12, 0x6666, 0x3333);
        Call3(Func_020023cc, 12, -13, 0);
        Func_02002312(12);
        Call3(Func_02002376, 12, 0x4000, 0);
        Call3(Func_02002394, 12, 0x102, 70);
        Func_02002372((base5_2512 + 3));
        Func_0200238a(12, 0);
        Call3(Func_02002336, 12, 168, 0x1b0);
        Func_02002304(40);
        Func_02002400();
        Func_0200240c();
        Func_02002312(20);
        Func_02002326();
        Func_02001026();
    }
}
