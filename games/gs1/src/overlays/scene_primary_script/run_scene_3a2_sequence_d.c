#include "types.h"

#define FieldScene_RunScene3a2SequenceD Func_02000c30

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002124();
void Func_02002134();
void Func_0200215e();
void Func_02002176();
void Func_0200219a();
void Func_020021b6();
void Func_020021d2();
void Func_020021d6();
void Func_020021d8();
void Func_020021e4();
void Func_020021ee();
void Func_020021f4();
void Func_020021fe();
void Func_02002200();
void Func_02002202();
void Func_0200220c();
s32 Func_02002214();
void Func_02002218();
s32 Func_02002226();
void Func_02002230();
void Func_02002238();
void Func_02002240();
void Func_0200224a();
void Func_0200224c();
void Func_02002268();
void Func_0200226e();
void Func_0200227e();
void Func_02002284();
void Func_02002290();
void Func_0200229a();
void Func_0200229e();
void Func_020022a0();
void Func_020022a2();
void Func_020022aa();
void Func_020022b4();
void Func_020022c2();
void Func_020022c4();
void Func_020022d4();
void Func_020022e0();
void Func_020022e6();
void Func_020022e8();
void Func_020022ec();
void Func_020022ee();
void Func_020022f0();
void Func_020022fa();
void Func_02002308();
void Func_02002314();
void Func_02002326();
void Func_0200232e();
void Func_02002344();
void Func_0200234e();
void Func_0200235a();
void Func_0200235e();
void Func_0200236e();
void Func_02002376();
void Func_0200237c();
void Func_0200237e();
void Func_02002386();
void Func_0200238a();
void Func_0200238e();
void Func_02002390();
void Func_0200239e();
void Func_020023aa();
s32 Func_020023ac();
void Func_020023ae();
void Func_020023b4();
void Func_020023bc();
void Func_020023c2();
void Func_020023c4();
void Func_020023cc();
void Func_020023ce();
void Func_020023d6();
void Func_020023e4();
void Func_02002402();
u8 *Func_02002408();
void Func_0200240a();
void Func_02002424();
void Func_0200242a();
void Func_02002440();
void Func_02002442();
void Func_0200244a();
void Func_02002458();
void Func_0200245a();
void Func_0200246c();
void Func_02002470();
void Func_02002472();
void Func_02002476();
void Func_02002486();
void Func_02002488();
void Func_0200248c();
void Func_02002490();
void Func_02002492();
void Func_0200249c();
void Func_020024b4();
void Func_020024cc();
void Func_020024d4();
void Func_020024e4();
void Func_020024fc();
void Func_02002514();
void Func_02002588();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a2SequenceD(void)
{
    u32 i;
    u8 *record;

    Func_02002124();
    Call2(Func_020021fe, 8, 0x102);
    Func_020021b6(8, 2);
    Func_02002134(60);
    Call1(Func_020021d2, 0x19da);
    Func_020021f4(8, 0, 20);
    Call2(Func_02002226, 10, 0x102);
    Func_020021d8(10, 4, 0);
    Func_0200215e(60);
    Func_02002218(10, 0, 20);
    Func_02002200(8, 1);
    Func_02002176(20);
    Func_02002230(8, 0, 20);
    Call3(Func_0200224c, 10, 0xd000, 20);
    Func_0200220c(10, 3);
    Func_0200219a(20);
    Call3(Func_020021ee, 8, 178, 0x114);
    Call3(Func_02002202, 10, 172, 0x11c);
    Func_02002218(8);
    Call3(Func_02002284, 8, 0x5000, 0);
    Call3(Func_02002290, 10, 0xb000, 0);
    Func_020021d6(20);
    Func_0200226e(8, 2);
    Func_020021e4(20);
    Func_0200229e(8, 0, 20);
    *(u8 *)(Func_02002214(8) + 90) &= 254;
    *(u8 *)(Func_02002226(10) + 90) &= 254;
    Call3(Func_02002240, 8, 0x3333, 0x1999);
    Call3(Func_0200224a, 10, 0x3333, 0x1999);
    Func_0200229a(8, 5);
    Func_020022a2(10, 6);
    Func_02002238(20);
    Func_0200236e(125);
    Func_020022a0(8, 2, 0);
    Func_020022aa(9, 2, 0);
    Func_020022b4(10, 2, 0);
    Func_020022c2(10);
    Func_02002268(30);
    Func_020022e0(8, 5);
    Func_020022e8(10, 6);
    Func_0200227e(20);
    Func_020023b4(125);
    Func_020022e6(8, 4, 0);
    Func_020022f0(9, 4, 0);
    Func_020022fa(10, 4, 0);
    Func_02002308(10);
    Func_020022ee(9);
    Func_02002326(8, 1);
    Func_0200232e(10, 1);
    Func_020022c4(50);
    Func_0200234e(10, 2, 0);
    Func_020022d4(20);
    Func_0200238e(10, 0, 20);
    Func_0200235e(8, 3);
    Func_020022ec(20);
    Value2(Func_020023ac, 8, 0);
    Func_020023ae(8, 0, 20);
    Func_02002376(8, 5);
    Func_0200237e(10, 6);
    Func_02002314(20);
    Func_0200244a(125);
    Func_0200237c(8, 2, 0);
    Func_02002386(9, 2, 0);
    Func_02002390(10, 2, 0);
    Func_0200239e(10);
    Func_02002344(30);
    Func_020023bc(8, 5);
    Func_020023c4(10, 6);
    Func_0200235a(20);
    Func_02002490(125);
    Func_020023c2(8, 4, 0);
    Func_020023cc(9, 4, 0);
    Func_020023d6(10, 4, 0);
    Func_020023e4(10);
    Func_0200238a(40);
    Func_02002402(8, 1);
    Func_0200240a(10, 1);
    Func_02002424(10, 2, 0);
    Func_020023aa(20);
    Call3(Func_02002476, 10, 0xd000, 20);
    Func_02002470(10, 0, 20);
    Func_02002440(8, 3);
    Func_020023ce(20);
    Func_02002488(8, 0, 30);
    Func_02002492(8, 0, 20);
    {
        u8 *record = Func_02002408(10);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Call3(Func_02002424, 10, 0xcccc, 0x6666);
    Call3(Func_02002458, 10, 168, 0x128);
    Call3(Func_020024d4, 10, 0xd000, 20);
    Func_0200248c(10, 5);
    Func_0200249c(8, 3);
    Func_0200242a(20);
    Func_020024e4(8, 0, 20);
    Func_020024b4(10, 3);
    Func_02002442(20);
    Func_020024fc(10, 0, 20);
    Func_020024cc(8, 3);
    Func_0200245a(20);
    Func_02002514(8, 0, 20);
    Func_020024e4(0, 3);
    Func_02002472(20);
    Func_02002486();
    Call1(Func_0200246c, 0x8b2);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x201;
    Func_02002588(6);
}
