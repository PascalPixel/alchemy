#include "types.h"

#define FieldScene_RunTwoActorStagingSequence Func_02000be0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000f85[];
extern u8 Data_00000f91[];
extern u8 Data_02009ce0[];
extern u8 Data_03001ebc[];
s32 Func_0200264a();
void Func_02002668();
void Func_02002674();
void Func_020026bc();
u8 *Func_020026e0();
void Func_02002706();
void Func_02002712();
void Func_0200271c();
void Func_02002730();
void Func_0200273e();
void Func_02002748();
void Func_02002752();
void Func_02002752_a();
void Func_02002768();
void Func_02002780();
void Func_02002788();
void Func_02002790();
void Func_02002792();
void Func_02002798();
void Func_020027a6();
void Func_020027ca();
void Func_0200280c();
void Func_02002816();
void Func_02002828();
void Func_0200282c();
void Func_0200282c_a();
void Func_02002832();
void Func_0200283c();
void Func_02002840();
void Func_02002848();
void Func_0200284c();
void Func_02002858();
void Func_0200285a();
void Func_02002884();
void Func_0200288a();
void Func_0200288e();
void Func_02002890();
void Func_0200289c();
void Func_0200289c_a();
void Func_020028a0();
void Func_020028a2();
void Func_020028a8();
void Func_020028a8_a();
void Func_020028b4();
void Func_020028d0();
void Func_020028d2();
void Func_020028d2_a();
void Func_020028e0();
void Func_020028e4();
void Func_020028f6();
void Func_020028fe();
void Func_02002904();
void Func_02002912();
void Func_02002912_a();
void Func_02002918();
void Func_0200291a();
void Func_0200292a();
void Func_0200293c();
void Func_0200293e();
void Func_0200294a();
void Func_02002954();
void Func_02002958();
void Func_0200296a();
void Func_0200296e();
void Func_0200298e();
void Func_02002996();
s32 Func_0200299e();
void Func_020029ac();
void Func_020029b8();
void Func_020029c4();
void Func_020029d6();
void Func_020029ee();
void Func_020029f4();
void Func_020029fa();
void Func_02002a0a();
void Func_02002a16();
void Func_02002a18();
void Func_02002a20();
s32 Func_02002a36();
s32 Func_02002a36_a();
void Func_02002a3c();
void Func_02002a42();
void Func_02002a64();
void Func_02002a72();
s32 Func_02002a88();
void Func_02002a8c();
void Func_02002a90();
void Func_02002a94();
void Func_02002aa0();
void Func_02002aaa();
void Func_02002aae();
void Func_02002aba();
void Func_02002aca();
void Func_02002ad4();
void Func_02002ada();
void Func_02002ae0();
void Func_02002ae4();
void Func_02002afa();
void Func_02002b14();
s32 Func_02002b20();
void Func_02002b22();
void Func_02002b2a();
void Func_02002b30();
void Func_02002b32();
void Func_02002b38();
void Func_02002b48();
void Func_02002b4c();
void Func_02002b56();
void Func_02002b64();
void Func_02002b70();
void Func_02002b72();
void Func_02002b86();
void Func_02002b94();
void Func_02002b94_a();
void Func_02002b9c();
void Func_02002ba8();
void Func_02002ba8_a();
void Func_02002bac();
void Func_02002bba();
void Func_02002bcc();
void Func_02002bd8();
void Func_02002be2();
void Func_02002be4();
void Func_02002be4_a();
s32 Func_02002bee();
void Func_02002bf0();
void Func_02002bf4();
s32 Func_02002c02();
void Func_02002c0e();
void Func_02002c0e_a();
void Func_02002c14();
void Func_02002c1a();
void Func_02002c1e();
void Func_02002c30();
s32 Func_02002c34();
void Func_02002c36();
s32 Func_02002c46();
s32 Func_02002c46_a();
void Func_02002c4a();
void Func_02002c4c();
void Func_02002c4e();
void Func_02002c60();
void Func_02002c78();
void Func_02002c84();
s32 Func_02002ca6();
void Func_02002ca8();
void Func_02002cb0();
s32 Func_02002cd6();
void Func_02002cda();
void Func_02002cde();
void Func_02002ce0();
void Func_02002cea();
void Func_02002cf0();
void Func_02002cfe();
void Func_02002d0a();
void Func_02002d10();
void Func_02002d10_a();
void Func_02002d20();
void Func_02002d20_a();
void Func_02002d2e();
void Func_02002d3a();
void Func_02002d40();
void Func_02002d8e();
void Func_02002d9a();
void Func_02002dd0();
void Func_02002e46();

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

void FieldScene_RunTwoActorStagingSequence(void)
{
    u8 *record;
    s32 x, y;
    s32 base7_f85;
    s32 base5_f91;
    s32 base5_2009ce0;

    if (Value1(Func_0200264a, 0x808) != 0) {
    } else {
        Func_02002674();
        Func_020027ca(17);
        Call1(Func_02002668, 0x808);
        base7_f85 = (s32)Data_00000f85;
        Func_02002730(base7_f85);
        Func_02002752(14, 0, 10);
        Call3(Func_020026bc, 0, 0x13333, 0x9999);
        Call3(Func_02002780, 0, 0x100, 30);
        Call3(Func_02002706, 0, 0x188, 0x148);
        Call3(Func_02002792, 0, 0x4000, 10);
        record = Func_020026e0(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Func_0200273e(5, x << 16, y << 16);
        Func_02002748(1, x << 16, y << 16);
        Call3(Func_02002712, 5, 0x13333, 0x9999);
        Call3(Func_0200271c, 1, 0x13333, 0x9999);
        Call3(Func_02002752_a, 5, 0x178, 0x148);
        Call3(Func_02002768, 1, 0x198, 0x148);
        Func_02002788(0, 0);
        Func_02002790(5, 0);
        Func_02002798(1, 0);
        Call3(Func_0200280c, 1, 0x8000, 0);
        Func_02002816(5, 0, 20);
        Call3(Func_02002828, 0, 0x101, 0);
        Call3(Func_02002832, 1, 0x101, 0);
        Call3(Func_0200283c, 5, 0x101, 30);
        Call3(Func_02002840, 0, 0x4000, 0);
        Call3(Func_0200284c, 1, 0x4000, 0);
        Call3(Func_02002858, 5, 0x4000, 0);
        Call2(Func_02002884, 0x60000, 0xc000);
        Call4(Func_0200289c, 0xd70000, -1, 0x1590000, 1);
        Func_020028a8();
        Func_020027a6(20);
        Func_02002904(61);
        Func_0200282c(14, 4);
        Func_0200282c_a(14, 4);
        Func_0200288e(14, 0, 20);
        Func_020028a8_a(15, 0, 10);
        Func_020028a2(15, 0, 10);
        Func_0200285a(14, 3);
        Func_020028b4(14, 0, 10);
        Call3(Func_020028d0, 15, 0x4000, 60);
        Func_02002890(15, 1);
        Func_020028d2(15, 0, 20);
        Func_0200288a(14, 3);
        Func_020028e4(14, 0, 10);
        Func_0200289c_a(15, 4);
        Func_020028f6(15, 0, 6);
        Call3(Func_0200291a, 14, 0x100, 0);
        Func_020028d2_a(14, 2);
        Func_02002848(20);
        Func_0200292a(15, 0, 10);
        Call3(Func_0200293c, 15, 0x101, 40);
        Func_0200293e(14, 0, 60);
        Call3(Func_0200294a, 14, 0x8000, 40);
        Func_02002954(14, 0, 40);
        Call3(Func_020028e0, 14, 232, 0x168);
        Func_0200296a(14, 0, 10);
        Func_02002912(15, 3);
        Func_020028a0(10);
        Call3(Func_020028fe, 14, 0x188, 0x168);
        Call3(Func_02002912_a, 15, 216, 0x168);
        Call3(Func_02002918, 15, 0x178, 0x168);
        Call3(Func_020029ac, 0, 0x4000, 0);
        Call3(Func_020029b8, 1, 0x4000, 0);
        Call3(Func_020029c4, 5, 0x4000, 0);
        Call4(Func_020029fa, 0x1890000, -1, 0x1530000, 1);
        Call3(Func_02002958, 14, 0x188, 0x168);
        Call3(Func_0200296e, 15, 0x178, 0x168);
        Func_0200298e(14, 0);
        Func_02002996(15, 0);
        Call3(Func_02002a0a, 14, 0xd000, 0);
        Call3(Func_02002a16, 15, 0xd000, 30);
        Func_020029d6(14, 2);
        Func_02002a18(14, 0, 10);
        Call3(Func_02002a3c, 1, 0x102, 60);
        Func_020029f4(1, 1);
        ((void (*)())Func_02002a36)(1, 0, 10);
        Func_020029ee(15, 4);
        Value2(Func_02002a36_a, 0x100f, 0);
        if (Value2(Func_0200299e, 0, 0) == 0) {
            Func_02002a42((base7_f85 + 10));
        } else {
            Func_02002a72((base7_f85 + 11));
        }
        Call3(Func_02002a94, 0x100f, 0, 10);
        Func_02002a64(1, 2);
        base5_f91 = (s32)Data_00000f91;
        Func_02002a8c(base5_f91);
        Func_02002aae(1, 0, 20);
        Func_02002a90(14, 15, 40);
        Call3(Func_02002ad4, 14, 0xd000, 0);
        Call3(Func_02002ae0, 15, 0xd000, 20);
        Func_02002ada(14, 0, 60);
        Func_02002aaa(15, 1);
        Func_02002a20(10);
        Func_02002aa0(15, 3);
        Call3(Func_02002afa, 0x100f, 0, 10);
        Func_02002aca(5, 2);
        Func_02002aba(5, 3);
        Call3(Func_02002b14, 0x1005, 0, 20);
        Func_02002ae4(14, 2);
        Call3(Func_02002b38, 14, 0xa000, 20);
        Value2(Func_02002b20, 14, 0);
        if (Value2(Func_02002a88, 0, 0) == 0) {
            Func_02002b2a((base5_f91 + 5));
        } else {
            Func_02002b32((base5_f91 + 6));
        }
        Func_02002b64(5, 0, 0);
        Call3(Func_02002b70, 1, 0x8000, 20);
        Func_02002b30(14, 2);
        Func_02002b72(14, 0, 10);
        Func_02002b4c(14, 1, 30);
        Func_02002b56(14, 5, 30);
        Call3(Func_02002ba8, 14, 0x105, 80);
        Func_02002b48(14, 4);
        Call1(Func_02002b86, 0xf98);
        Func_02002ba8_a(14, 0, 6);
        Call3(Func_02002bcc, 0, 0x100, 0);
        Call3(Func_02002bd8, 1, 0x100, 0);
        Call3(Func_02002be4, 5, 0x100, 0);
        Func_02002b94(1, 1);
        Func_02002b9c(5, 1);
        Func_02002bac(0, 1);
        Func_02002b22(40);
        Func_02002bba(5, 2);
        Call3(Func_02002c0e, 1, 0x4000, 0);
        Call3(Func_02002c1a, 5, 0x4000, 10);
        Call3(Func_02002c14, 0x1005, 0, 10);
        Func_02002be4_a(15, 2);
        Func_02002c36(15, 0, 10);
        Call3(Func_02002c30, 0x100f, 0, 10);
        Call3(Func_02002c4c, 14, 0x8000, 20);
        Func_02002bf4(14, 4);
        Func_02002c4e(14, 0, 10);
        Func_02002c1e(15, 2);
        Func_02002b94_a(10);
        Call3(Func_02002c78, 14, 0xb000, 0);
        Call3(Func_02002c84, 15, 0xd000, 20);
        Call3(Func_02002be2, 15, 0x8000, 0x4000);
        Call3(Func_02002bf0, 14, 0x8000, 0x4000);
        record = Value1(Func_02002bee, 14);
        record[90] &= 254;
        record = Value1(Func_02002c02, 15);
        record[90] &= 254;
        Call3(Func_02002c4a, 14, 0x188, 0x178);
        Call3(Func_02002c60, 15, 0x178, 0x178);
        Func_02002c0e_a(6);
        record = Value1(Func_02002c34, 14);
        record[90] |= 1;
        record = Value1(Func_02002c46, 15);
        {
            u8 value = *(volatile u8 *)&record[90];

            record[90] = (u8)(value | 1);
        }
        Func_02002ca8(14, 0);
        Func_02002cb0(15, 0);
        ((void (*)())Func_02002c46_a)(20);
        Func_02002cde(1, 2);
        Func_02002d20(1, 0, 10);
        Func_02002d3a(0, 1, 20);
        Func_02002cda(0, 3);
        Func_02002cea(1, 3);
        Func_02002dd0(17);
        Func_02002cf0(1, 2);
        record = Value1(Func_02002ca6, 0);
        if ((s32)record != 0) {
            Func_02002ce0(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02002cfe(1);
        Func_02002d10(1, 0, 0);
        Func_02002d20_a(5, 2);
        record = Value1(Func_02002cd6, 0);
        if ((s32)record != 0) {
            Func_02002d10_a(5, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02002d2e(5);
        Func_02002d40(5, 0, 0);
        base5_2009ce0 = (s32)Data_02009ce0;
        Call3(Func_02002d8e, 14, 0x10000, base5_2009ce0);
        Call3(Func_02002d9a, 15, 0x10000, base5_2009ce0);
        Func_02002e46();
        Func_02002d0a();
    }
}
