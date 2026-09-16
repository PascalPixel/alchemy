#include "TYPES.H"

#define FieldScene_RunElementalStarDemand Func_02000f8c

extern u8 Data_03001ebc[];
void Func_020051f8();
void Func_02005208();
void Func_02005230();
void Func_0200524c();
void Func_02005268();
void Func_0200529e();
void Func_020052b6();
void Func_0200530c();
void Func_0200544a();
void Func_020054ae();
void Func_020054d4();
void Func_020054f6();
void Func_02005570();
void Func_02005636();
void Func_0200565e();
void Func_0200566e();
void Func_020059ae();
void Func_020059c4();
void Func_020059f2();
void Func_020059fc_a();
void Func_020059fc_b();
void Func_02005a1c();
struct ObjectRuntime;
struct ObjectRuntime *Func_02005a30(u32);
void Func_02005a38_a();
struct ObjectRuntime *Func_02005a38_b(u32);
void Func_02005a50();
void Func_02005a58();
void Func_02005a72();
void Func_02005a78();
void Func_02005a8a();
void Func_02005a94();
void Func_02005aac();
void Func_02005aae();
void Func_02005ab2();
void Func_02005ab8();
void Func_02005abe();
void Func_02005ada();
void Func_02005ae0();
void Func_02005b04();
void Func_02005b14();
void Func_02005b26();
void Func_02005b36();
void Func_02005b3a();
void Func_02005b5c();
void Func_02005b64();
void Func_02005b68();
void Func_02005b70();
void Func_02005b76();
void Func_02005b7c_a();
struct ObjectRuntime *Func_02005b7c_b(u32);
void Func_02005b86();
void Func_02005b88();
void Func_02005ba4();
void Func_02005ba6();
void Func_02005bb6();
void Func_02005bbc_a();
void Func_02005bbc_b();
void Func_02005bc8();
void Func_02005bd2_a();
void Func_02005bd2_b();
void Func_02005bde();
void Func_02005be0_a();
void Func_02005be0_b();
void Func_02005bf2();
void Func_02005c0a();
void Func_02005c16();
void Func_02005c18();
void Func_02005c26();
void Func_02005c3a();
void Func_02005c3e_a();
void Func_02005c3e_b();
void Func_02005c48();
void Func_02005c4e();
void Func_02005c50_a();
void Func_02005c50_b();
void Func_02005c54();
void Func_02005c5a();
void Func_02005c5c();
void Func_02005c64();
void Func_02005c82();
void Func_02005caa();
void Func_02005cb4_a();
void Func_02005cb4_b();
void Func_02005cca();
void Func_02005cce();
void Func_02005cd2();
void Func_02005cda();
void Func_02005cfc();
s32 Func_02005d02();
void Func_02005d04();
void Func_02005d14();
void Func_02005d44();
void Func_02005d50();
void Func_02005d54();
void Func_02005d5c();
void Func_02005d60();
void Func_02005d64();
s32 Func_02005da2();
void Func_02005db4();
void Func_02005dd8();
void Func_02005dde();
void Func_02005dea();
void Func_02005dfa();
void Func_02005e00();
void Func_02005e0e();
void Func_02005e1a();
void Func_02005e30();
void Func_02005e3a();
void Func_02005e4a();
void Func_02005e52();
void Func_02005e5c_a();
void Func_02005e5c_b();
void Func_02005e62();
void Func_02005e68();
void Func_02005e76();
void Func_02005e82();
void Func_02005ea6();
void Func_02005eba();

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

/* Moves the event's current message on by amount. */
static __inline__ void SkipMessage(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* The demand for the Elemental Stars. Its dialogue starts at message 0x107d
 * while the actors move into place; a nonzero answer to the prompt skips one
 * reply, and the dialogue then continues from message 0x108d. */
void FieldScene_RunElementalStarDemand(void)
{
    u8 *field_85;
    u8 *field_80_38;
    u8 *record12;
    u8 *record8;
    s32 cnt;
    s32 zero;

    Func_02005aae(61);
    Func_020059ae(10, 4);
    Call1(Func_020059fc_a, 0x107d);
    Func_020051f8(10, 10);
    Func_020059c4(11, 4);
    Func_02005208(11, 30);
    Call3(Func_02005a58, 9, 0x102, 60);
    Func_020059f2(9, 4, 10);
    Func_020059fc_b(9, 6, 30);
    Func_02005230(9, 10);
    Func_02005a1c(10, 1);
    Call3(Func_02005a78, 10, 0xb000, 10);
    Func_0200524c(10, 20);
    Call2(Func_02005a38_a, 11, 1);
    Call3(Func_02005a94, 11, 0xd000, 20);
    Func_02005268(11, 30);
    Call3(Func_02005ab8, 9, 0x102, 60);
    Func_02005ab2(5, 0, 0);
    Call3(Func_02005abe, 9, 0x7000, 80);
    Call3(Func_02005ada, 5, 0x102, 40);
    Func_0200529e(5, 20);
    Func_02005a8a(9, 2);
    Func_02005a72(9, 4);
    Func_020052b6(9, 10);
    record12 = (u8 *)Func_02005a30(12);
    record8 = (u8 *)Func_02005a38_b(8);
    field_80_38 = *(u8 **)(record12 + 80) + 38;
    zero = 0;
    *field_80_38 = zero;
    *(s32 *)(record12 + 24) = 0x1999;
    *(s32 *)(record12 + 28) = 0x1999;
    *(s32 *)(record8 + 24) = 0x1999;
    *(s32 *)(record8 + 28) = 0x1999;
    Call2(Func_02005ae0, 12, 0x100);
    Call3(Func_02005aac, 12, 0x1d70000, 0x1220000);
    field_85 = record12 + 85;
    *field_85 = zero;
    *(s32 *)(record12 + 12) = 0x280000;
    Func_02005a50(1);
    Func_0200530c(12, 10);
    Call3(Func_02005b5c, 5, 0x100, 0);
    Call3(Func_02005b68, 9, 0x100, 30);
    Call3(Func_02005b64, 5, 0xc000, 0);
    Call3(Func_02005b70, 9, 0xb000, 10);
    Call3(Func_02005b7c_a, 11, 0xd000, 0);
    Call3(Func_02005b88, 10, 0xb000, 0);
    Call2(Func_02005bbc_a, 0x20000, 0x4000);
    Call4(Func_02005bd2_a, 0x1d70000, -1, 0x1350000, 1);
    Func_02005bde();
    Call3(Func_02005b3a, 8, 0x1d70000, 0x1220000);
    Func_02005c50_a(190);
    Func_02005bc8(12, 2);
    for (cnt = 0; cnt != 90; cnt++) {
        *(s32 *)(record12 + 12) += -0x1999;
        *(s32 *)(record12 + 24) += 0x28f;
        *(s32 *)(record12 + 28) += 0x28f;
        *(s32 *)(record8 + 24) += 0x28f;
        *(s32 *)(record8 + 28) += 0x28f;
        Func_02005b04(1);
    }
    *field_85 = 5;
    Func_02005b14(80);
    for (cnt = 0; cnt != 60; cnt++) {
        *(s32 *)(record12 + 12) += -0x8000;
        Func_02005b26(1);
    }
    *field_85 = 3;
    Func_02005b36(30);
    *field_80_38 = 1;
    Func_02005bb6(8, 0, 0);
    Func_02005c3e_a(12, 1);
    {
        u8 *flags = (u8 *)Func_02005b7c_b(12) + 35;
        cnt = 1;
        cnt |= *flags;
        *flags = cnt;
    }
    Func_02005c16(12, 0);
    Call3(Func_02005ba4, 12, 0x8000, 0x4000);
    Call3(Func_02005be0_a, 12, 0x1d7, 0x132);
    Func_02005b86(40);
    Func_02005c26(12, 2);
    Call2(Func_0200544a, 0x400c, 20);
    Func_02005c48(5, 9, 0);
    Func_02005ba6(20);
    Func_02005c3e_b(5, 2);
    Func_02005c4e(9, 2);
    Func_02005bbc_b(40);
    Func_02005c54(10, 1);
    Func_02005c64(11, 1);
    Func_02005bd2_b(20);
    Func_02005c5a(10, 4);
    Call3(Func_02005cce, 10, 0x5000, 0);
    Call3(Func_02005cda, 11, 0x5000, 10);
    Func_020054ae(10, 30);
    Call2(Func_02005d04, 12, 0x102);
    Func_02005c0a(60);
    Func_02005caa(11, 1);
    Func_02005c18(10);
    Func_020054d4(11, 30);
    Call3(Func_02005d14, 11, 0xd000, 30);
    Func_02005cb4_a(11, 3);
    Func_02005c3a(20);
    Func_020054f6(11, 30);
    Func_02005cca(12, 3);
    Func_02005c50_b(20);
    Call3(Func_02005d44, 11, 0x5000, 40);
    Call3(Func_02005d50, 9, 0x5000, 0);
    Call3(Func_02005d5c, 5, 0x6000, 20);
    Func_02005cfc(10, 3);
    Func_02005c82(20);
    Func_02005dde();
    Func_02005dea();
    Call4(Func_02005db4, 0x1080000, -1, 0x1cc0000, 0);
    Func_02005be0_b();
    Func_02005b76(1);
    Func_02005dfa();
    Func_02005e0e();
    Func_02005cb4_b(40);
    Func_02005570(10, 40);
    Func_02005d54(0, 3);
    Func_02005d64(1, 3);
    Func_02005cd2(80);
    Value2(Func_02005da2, 11, 0);
    if (Value2(Func_02005d02, 0, 0) != 0) {
        SkipMessage(1);
    }
    Func_02005dd8(9, 0, 20);
    Func_02005e5c_a();
    Func_02005e68();
    Call4(Func_02005e30, 0x1dd0000, -1, 0x14e0000, 0);
    Func_02005c5c();
    Func_02005bf2(1);
    Func_02005ea6();
    Func_02005eba();
    Func_02005d60(20);
    Func_02005e00(10, 2);
    Call3(Func_02005e5c_b, 10, 0xb000, 10);
    Call1(Func_02005e3a, 0x108d);
    Func_02005636(10, 20);
    Call3(Func_02005e76, 5, 0x2000, 0);
    Call3(Func_02005e82, 9, 0x3000, 10);
    Func_02005e1a(9, 4);
    Call2(Func_0200565e, 0x5009, 40);
    Func_02005e4a(11, 1);
    Func_0200566e(11, 10);
    Func_02005e52(5, 2);
    Func_02005e62(9, 2);
}
