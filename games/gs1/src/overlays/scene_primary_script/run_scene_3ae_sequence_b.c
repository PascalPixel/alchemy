#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020023a4();
void Func_020023d0();
u8 *Func_02002400();
void Func_0200243c();
void Func_0200244a();
void Func_02002456();
void Func_02002472();
void Func_02002476();
void Func_0200248c();
void Func_0200249a();
void Func_020024a8();
void Func_020024b6();
void Func_020024c4();
void Func_020024dc();
void Func_020024ec();
void Func_020024f4();
void Func_020024f6();
void Func_020024fa();
void Func_02002500();
void Func_0200250c();
void Func_0200251c();
void Func_02002520();
void Func_02002526();
s32 Func_02002528();
void Func_02002532();
void Func_02002536();
void Func_02002538();
void Func_02002552();
void Func_02002558();
void Func_02002568();
s32 Func_0200256e();
void Func_0200257a();
void Func_02002586();
void Func_0200258c();
void Func_020025a6();
void Func_020025a8();
void Func_020025b8();
void Func_020025c6();
void Func_020025e2();
void Func_020025fe();
void Func_0200260c();
void Func_02002610();
void Func_02002616();
void Func_02002618();
void Func_0200261a();
void Func_0200262a();
void Func_0200262c();
void Func_0200263a();
void Func_02002640();
void Func_02002648();
void Func_02002656();
void Func_0200265a();
void Func_02002662();
void Func_0200266c();
void Func_02002670();
void Func_02002676();
void Func_0200267a();
void Func_0200268c();
void Func_020026a0();
void Func_020026a6();
void Func_020026aa();
void Func_020026ac();
void Func_020026b4();
void Func_020026b8();
void Func_020026bc();
void Func_020026c0();
void Func_020026c2();
void Func_020026c4();
void Func_020026d4();
void Func_020026de();
void Func_020026e2();
void Func_020026e6();
void Func_020026ee();
void Func_020026f2();
void Func_020026f4();
void Func_0200270e();
void Func_0200271c();
void Func_02002724();
void Func_0200272e();
void Func_02002736();
void Func_0200274e();
void Func_02002750();
void Func_0200275e();
void Func_0200276c();
void Func_02002770();
void Func_02002776();
void Func_02002796();
void Func_0200279e();
s32 Func_020027a6();
void Func_020027a8();
void Func_020027b2();
void Func_020027c2();
void Func_020027c6();
s32 Func_020027cc();
void Func_020027d0();
void Func_020027d4();
void Func_020027ec();
void Func_020027fa();
void Func_02002808();
void Func_02002816();
void Func_02002826();
s32 Func_02002844();
void Func_02002850();
void Func_0200285a();
void Func_02002860();
void Func_02002868();
s32 Func_0200286a();
void Func_0200286e();
void Func_02002872();
void Func_02002882();
void Func_020028a2();
void Func_020028a4();
void Func_020028c0();
void Func_020028ce();
void Func_020028d2();
s32 Func_020028d6();
void Func_020028e0();
s32 Func_020028fa();
void Func_020028fe();
void Func_02002918();
void Func_0200291c();
void Func_02002930();
void Func_02002938();
u8 *Func_02002944();
void Func_02002962();
void Func_0200296c();
void Func_02002970();
void Func_02002978();
void Func_0200297e();
void Func_0200298c();
void Func_020029a8();
void Func_020029ac();
void Func_020029b6();
void Func_020029c0();
void Func_020029c4();
void Func_020029c6();
void Func_020029ca();
void Func_020029ce();
void Func_020029da();
void Func_020029de();
void Func_020029e8();
void Func_020029f6();
void Func_020029fc();
void Func_02002a14();
void Func_02002a1a();
void Func_02002a24();
void Func_02002a32();
void Func_02002a36();
void Func_02002a3a();
void Func_02002a46();
void Func_02002a52();
void Func_02002a60();
void Func_02002a62();
void Func_02002a66();
void Func_02002a70();
void Func_02002a82();
void Func_02002a9c();
void Func_02002ad2();
void Func_02002af4();
void Func_02002b46();
void Func_02002b50();
void Func_02002b7c();

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

void FieldScene_RunScene3aeSequenceB(void)
{
    u32 i;
    u8 *record;
    s32 none;
    s32 v5;
    s32 v6;

    Call1(Func_020023a4, 0x8ab);
    Func_020023d0();
    Func_020024dc();
    Call1(Func_02002472, 0x23eb);
    record = Func_02002400(11);
    none = 0;
    record[35] = none;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 21) |= 12;
    Call4(Func_020024f4, 0xe80000, -1, 0x1300000, 1);
    Call3(Func_0200244a, 0, 0x10000, 0x8000);
    Call3(Func_02002476, 0, 216, 0x110);
    Call3(Func_020024fa, 0, 0x2000, 0);
    Func_02002526();
    Func_0200243c(20);
    Call3(Func_02002520, 13, 0x102, 50);
    Func_02002500(13, 0);
    Func_02002456(10);
    Call3(Func_02002538, 10, 0x107, 50);
    Func_02002532(10, 0, 0);
    Func_020024ec(10, 4, 13);
    Func_020024f6(10, 4, 30);
    Func_02002536(10, 0);
    Func_0200248c(10);
    Func_0200251c(11, 2);
    Func_0200249a(20);
    Func_02002552(11, 0);
    Func_020024a8(10);
    Func_02002520(13, 4);
    Func_020024b6(20);
    Call2((void (*)())Func_0200256e, 13, 0);
    Func_020024c4(10);
    Call3(Func_020025a6, 10, 0x103, 55);
    Call3(Func_0200250c, 10, 0x20000, 0x10000);
    Func_02002616(10, 16, 0);
    Func_02002568(10, 7, 0);
    v5 = 254;
    Func_0200262a(10, 24, 0);
    *(u8 *)(Func_02002528(10) + 90) &= v5;
    Call3(Func_02002640, 10, -16, 0);
    Func_02002656(153);
    Call3(Func_02002558, 13, 0x26666, 0x13333);
    Func_02002662(13, 16, 0);
    Func_02002538(10);
    v6 = 1;
    Func_020025a8(10, 1);
    *(u8 *)(Func_0200256e(10) + 90) |= v6;
    Call2(Func_0200263a, 13, 0x102);
    Func_020025e2(13, 2);
    Func_020026a0(155);
    Call1((void (*)())Func_0200256e, 10);
    Func_020026ac(155);
    Func_0200257a(10);
    Func_020026b8(155);
    Func_02002586(10);
    Func_0200258c(20);
    Call3(Func_020025c6, 13, 0x6666, 0x3333);
    Func_02002618(13, 6, 0);
    Func_020026de(159);
    Call3(Func_020026e2, 13, -8, 0);
    Func_020025b8(20);
    Func_02002670(10, 0);
    Func_020025c6(10);
    Call3(Func_020026aa, 13, 0x102, 70);
    Call3(Func_02002610, 16, 0x10000, 0x8000);
    Call3(Func_0200271c, 16, -8, 0);
    Call3(Func_020026c0, 16, 0x5000, 0);
    Func_020025fe(30);
    Func_02002676(16, 4);
    Func_0200260c(20);
    Func_020026c4(16, 0);
    Func_0200261a(10);
    Call3(Func_020026ee, 10, 0xe000, 0);
    Func_0200262c(35);
    Func_020026bc(10, 2);
    Func_0200263a(20);
    Func_020026f2(10, 0);
    Func_02002648(10);
    Call3(Func_0200271c, 16, 0x2000, 0);
    Func_0200265a(55);
    Call3(Func_0200272e, 16, 0x5000, 0);
    Func_0200266c(30);
    Func_02002724(16, 0);
    Func_0200267a(10);
    Call3(Func_0200274e, 11, 0xe000, 0);
    Func_0200268c(20);
    Call3(Func_02002770, 11, 0x102, 50);
    Func_02002750(11, 0);
    Func_020026a6(10);
    Func_02002736(13, 2);
    Func_020026b4(20);
    Func_0200276c(13, 0);
    Func_020026c2(10);
    Call3(Func_02002796, 13, 0xa000, 0);
    Func_020026d4(60);
    Call3(Func_020027a8, 13, 0x8000, 0);
    Func_020026e6(30);
    Func_0200279e(13, 0);
    Func_020026f4(10);
    Func_020027c6(10, 0, 0);
    Func_020027d0(11, 0, 0);
    Func_0200270e(20);
    Func_0200279e(10, 2);
    Func_0200271c(20);
    Func_020027d4(10, 0);
    Call3(Func_0200275e, 10, 0x13333, 0x9999);
    Func_02002868(10, 8, 0);
    Call3(Func_02002776, 16, 0x20000, 0x10000);
    Call3(Func_02002882, 16, -8, 16);
    Call3(Func_02002826, 16, 0x8000, 0);
    Func_02002816(16, 0);
    Func_0200276c(10);
    Call3(Func_02002850, 10, 0x102, 50);
    *(u8 *)(Func_020027a6(10) + 90) &= v5;
    Call3(Func_020027c2, 10, 0xcccc, 0x6666);
    Call3(Func_020028ce, 10, -8, 0);
    *(u8 *)(Func_020027cc(10) + 90) |= v6;
    Func_020027b2(20);
    Func_0200286e(10, 2);
    Func_020027ec(20);
    Func_020028a4(10, 0);
    Func_020027fa(10);
    Func_02002872(16, 4);
    Func_02002808(20);
    Func_020028c0(16, 0);
    Func_02002816(10);
    *(u8 *)(Func_02002844(10) + 90) &= v5;
    Call3(Func_02002860, 10, 0xcccc, 0x6666);
    Call3(Func_0200296c, 10, -16, 0);
    *(u8 *)(Func_0200286a(10) + 90) |= v6;
    Func_0200291c(10, 0, 0);
    Func_0200285a(20);
    Func_020028d2(10, 4);
    Func_02002868(20);
    Call3(Func_020028a2, 10, 0x1cccc, 0xe666);
    Func_020029ac(10, 8, 0);
    Func_020028fe(10, 6, 0);
    Func_020029c0(10, 24, 0);
    Func_020029ce(133);
    Func_02002918(16, 6, 0);
    Call2(Func_020028e0, 16, 0x20096e4);
    *(u8 *)(Func_020028d6(10) + 90) &= v5;
    Func_02002938(10, 6, 0);
    Call3(Func_020029fc, 10, -12, 4);
    record = Value1(Func_020028fa, 10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(volatile s32 *)((s32)record + 80);
        s32 shown = 0xc000;
    
        *(volatile u16 *)(target + 30) = shown;
    }
    Call3(Func_02002a3a, 10, -12, 4);
    Call3(Func_020029de, 10, 0x4000, 0);
    {
        u8 *record = Func_02002944(10);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | v6);
    }
    Func_02002a62(159);
    Func_02002930(20);
    Call3(Func_02002a14, 11, 0x102, 50);
    Call3(Func_02002978, 11, 0x18000, 0xc000);
    Func_02002a82(11, 24, 0);
    Func_02002a24(11, 0xc000, 0);
    Func_02002962(10);
    Func_02002a1a(11, 0);
    Func_02002970(10);
    Func_020029e8(16, 4);
    Func_0200297e(20);
    Func_02002a36(16, 0);
    Func_0200298c(20);
    Call3(Func_020029c6, 16, 0xcccc, 0x6666);
    Call3(Func_02002ad2, 16, -8, 0);
    Func_020029a8(20);
    Func_02002a60(16, 0);
    Func_020029b6(10);
    Func_02002a46(10, 2);
    Func_020029c4(20);
    Func_020029ca(10);
    Func_02002a9c(16, 0, 0);
    Func_020029da(40);
    Func_02002a52(13, 3);
    Func_020029e8(10);
    Func_02002a60(13, 3);
    Func_020029f6(20);
    Func_020029fc(10);
    Call3(Func_02002a3a, 16, 0x10000, 0x8000);
    Call3(Func_02002b46, 16, 24, -24);
    Func_02002b50(16, 8, 0);
    Call3(Func_02002af4, 16, 0xe000, 0);
    Func_02002a32(20);
    Call3(Func_02002a70, 13, 0x10000, 0x8000);
    Call3(Func_02002b7c, 13, 0, -8);
    Func_02002a52(10);
    Func_02002a66();
}
