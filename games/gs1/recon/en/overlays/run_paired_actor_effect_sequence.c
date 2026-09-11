#include "configured_effect_spawn.h"

#define Scene_RunPairedActorEffectSequence Func_02002360

extern u8 Data_02000240[];
extern const s32 Data_0200e074[];
extern const s32 Data_0200e088[];
extern const s32 Data_0200e0ac[];
extern const s32 Data_0200e22c[];
void Func_0808a018();
void Func_080091c0();
u8 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a098();
void Func_080091e0();
s32 Func_0808a228();
void Func_0808a210();
void Func_080000c0();
void Func_08009128();
void Func_0808a360();
void Func_0808a370();
void Func_0808a010();
void Func_0808a128();
void Func_020008a8();
void Func_0808a170();
void Func_02000894();
void Func_0808a1b8();
void Func_0808a138();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a1e8();
void Func_0808a110();
void Func_0808a130();
s32 Func_0808a178();
s32 Func_0808a070();
void Func_0808a180();
void Func_080f9010();
void Func_0808a0a8();
void Func_0808a188();
u8 *Func_080090c8();
u8 *Func_08000140();
void Func_08015250();
s32 Func_080001c8();
void Func_08000150();
void Func_0808a1e0();
void Func_08009150();
void Func_0808a0c0();
void Func_08009158();
void Func_02005688();
void Func_0808a158();
void Func_080091f0();
void Func_0808a330();
void Func_0808a348();
s32 Func_080000d0();
void Func_020035c4();
s32 Func_08000120();
s32 Func_08000118();
void Func_0808a0b8();
s32 Func_080770c8();
void Func_0808a260();
void Func_0808a268();
void Func_0808a250();
void Func_08077268();
void Func_020003a0();
void Func_0200013c(s32, s32, s32, s32, s32, s32, u32, const struct ConfiguredEffectOptions *);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 * Pointer1(u8 * (*f)(), s32 a0)
{
    return f(a0);
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

static __inline__ u8 * Pointer2(u8 * (*f)(), s32 a0, s32 a1)
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

static __inline__ u8 * Pointer4(u8 * (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Scene_RunPairedActorEffectSequence(void)
{
    u32 i;
    u8 *rec;
    u8 *rec7;
    u8 *record;
    u8 *p6;
    s32 value;
    s32 v7;
    s32 v6;
    s32 none;
    s32 p9;
    s32 p9b;
    s32 slot40;
    u8 *rec4;
    const s32 *script;
    s32 velocity[3];
    struct ConfiguredEffectOptions options_a;
    struct ConfiguredEffectOptions options_b;

    Func_0808a018();
    Call6(Func_080091c0, 17, 10, 4, 2, 17, 8);
    record = Func_0808a080(1);
    p9 = 0;
    {
        s32 shown = 0x8000;

        *(u16 *)((s32)record + 6) = shown;
    }
    Call3(Func_0808a0f0, 1, 0x1480000, 0xa80000);
    record = Func_0808a080(2);
    {
        s32 shown = 0x8000;

        *(u16 *)((s32)record + 6) = shown;
    }
    Call3(Func_0808a0f0, 2, 0x1540000, 0xc40000);
    record = Func_0808a080(3);
    {
        s32 shown = 0x8000;

        *(u16 *)((s32)record + 6) = shown;
    }
    Call3(Func_0808a0f0, 3, 0x1460000, 0xcc0000);
    record = Func_0808a080(6);
    {
        s32 shown = 0x3000;

        *(u16 *)((s32)record + 6) = shown;
    }
    Call3(Func_0808a0f0, 6, 0x10c0000, 0x9a0000);
    record = Func_0808a080(21);
    {
        s32 shown = 0x3000;

        *(u16 *)((s32)record + 6) = shown;
    }
    Call3(Func_0808a0f0, 21, 0x10c0000, 0xa40000);
    rec = Pointer1(Func_0808a080, 20);
    {
        s32 shown = 10;

        *(u16 *)((s32)rec + 100) = shown;
    }
    {
        s32 shown = 0xd000;

        *(u16 *)((s32)rec + 6) = shown;
    }
    Call3(Func_0808a0f0, 20, 0x1260000, 0xd40000);
    Func_0808a100(20, 9);
    Func_0808a098(20, (s32)Data_0200e074);
    record = Func_0808a080(20);
    Func_080091e0((s32)record, 0);
    rec = Pointer1(Func_0808a080, 19);
    {
        s32 shown = 10;

        *(u16 *)((s32)rec + 100) = shown;
    }
    {
        s32 shown = 0;

        *(u16 *)((s32)rec + 6) = shown;
    }
    Call3(Func_0808a0f0, 19, 0x11e0000, 0xc00000);
    Func_0808a100(19, 7);
    Func_0808a098(19, (s32)Data_0200e074);
    record = Func_0808a080(19);
    Func_080091e0((s32)record, 0);
    *(u8 *)(Func_0808a228() + 85) = p9;
    Call4(Func_0808a210, 0x1300000, 0x200000, 0xb40000, 0);
    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(80);
    Func_0808a128(1, 2, 20);
    Func_020008a8(1, 0x2000);
    Call1(Func_0808a170, 0x27cf);
    Call1(Func_02000894, 0x1001);
    Func_020008a8(3, 0xa000);
    Func_02000894(3);
    Func_0808a1b8(1, 0x8000, 0);
    Call2(Func_020008a8, 2, 0xa000);
    Func_0808a138(21, 2);
    Call3(Func_0808a090, 21, 0xcccc, 0x6666);
    *(u8 *)(Func_0808a080(21) + 90) &= 254;
    Call3(Func_0808a0d0, 21, 0x118, 164);
    Func_0808a010(1);
    {
        u8 *record = Func_0808a080(21);
        u8 value = *(u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Func_0808a010(20);
    Call3(Func_0808a1e8, 2, 0x102, 40);
    Func_02000894(2);
    Func_0808a110(21, 4);
    Func_02000894(21);
    Call3(Func_0808a1e8, 1, 0x103, 20);
    Func_0808a138(1, 1);
    Func_02000894(1);
    Call2(Func_0808a138, 21, 1);
    Func_020008a8(21, 0);
    Func_02000894(21);
    Func_0808a130(3, 2);
    Func_02000894(3);
    Func_0808a100(3, 4);
    Func_02000894(3);
    Call3(Func_0808a1e8, 21, 0x105, 40);
    Func_020008a8(21, 0x3000);
    Func_02000894(21);
    Func_0808a128(2, 2, 20);
    Func_02000894(2);
    Func_0808a110(21, 3);
    Func_02000894(21);
    Func_0808a138(1, 1);
    Func_020008a8(1, 0x2000);
    Value2(Func_0808a178, 1, 0);
    Call3(Func_0808a1b8, 2, 0xe000, 0);
    Call3(Func_0808a1b8, 3, 0xe000, 0);
    v7 = 1;
    if (Func_0808a070(0, 0) != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        v7 = 0;
    }
    Func_080000c0(20);
    Call3(Func_0808a1b8, 1, 0x8000, 0);
    Call3(Func_0808a1b8, 2, 0xa000, 0);
    Func_020008a8(3, 0xa000);
    Func_020008a8(21, 0);
    Func_0808a180(21, 0);
    Func_080f9010(17);
    Func_0808a010(40);
    if (v7 != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_0808a0a8(20);
    Func_0808a0a8(19);
    Func_080000c0(1);
    rec = Func_0808a080(20);
    *(s32 *)((s32)rec + 24) = 0x10000;
    *(s32 *)((s32)rec + 28) = 0x10000;
    rec = Func_0808a080(19);
    *(s32 *)((s32)rec + 24) = 0x10000;
    *(s32 *)((s32)rec + 28) = 0x10000;
    Func_080000c0(1);
    Func_02000894(20);
    Call3(Func_0808a1b8, 21, 0x3000, 0);
    Call3(Func_0808a1b8, 0, 0x6000, 0);
    Call3(Func_0808a1b8, 1, 0x6000, 0);
    Call3(Func_0808a1b8, 2, 0x8000, 0);
    Call3(Func_0808a1b8, 3, 0x8000, 40);
    Call2(Func_0808a100, 20, 1);
    record = Func_0808a080(20);
    Func_080091e0((s32)record, 1);
    Func_0808a010(20);
    Call3(Func_0808a090, 20, 0x3333, 0x1999);
    Call3(Func_0808a0d0, 20, 0x12c, 206);
    Func_0808a010(20);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    record = Func_0808a080(20);
    *(s32 *)((s32)record + 24) = -0x10000;
    Func_080f9010(161);
    Func_0808a100(20, 8);
    Func_0808a010(20);
    Func_0808a100(19, 1);
    record = Func_0808a080(19);
    Func_080091e0((s32)record, 1);
    Func_0808a128(19, 4, 40);
    Call3(Func_0808a090, 19, 0x3333, 0x1999);
    *(u8 *)(Func_0808a080(19) + 90) &= 254;
    Call3(Func_0808a0d0, 19, 0x128, 186);
    Call3(Func_0808a090, 19, 0x1999, 0xccc);
    Call3(Func_0808a0d0, 19, 0x124, 186);
    *(u8 *)(Func_0808a080(19) + 90) |= 1;
    record = Pointer1(Func_0808a080, 19);
    *(s32 *)((s32)record + 24) = -0x10000;
    Func_080f9010(161);
    Func_0808a100(19, 5);
    Func_0808a010(20);
    Func_0808a138(19, 2);
    Func_0808a010(20);
    Func_0808a138(20, 1);
    Func_0808a010(80);
    Func_0808a110(3, 4);
    Func_0808a188(3, 0, 20);
    Func_0808a138(19, 2);
    Func_0808a010(40);
    Func_02000894(19);
    Func_0808a1b8(0, 0xa000, 0);
    Call3(Func_0808a1b8, 1, 0x2000, 0);
    Call3(Func_0808a1b8, 2, 0x6000, 0);
    Call3(Func_0808a1b8, 3, 0xe000, 40);
    Call3(Func_0808a1b8, 0, 0x6000, 0);
    Call3(Func_0808a1b8, 1, 0x6000, 0);
    Call3(Func_0808a1b8, 2, 0x8000, 0);
    Call3(Func_0808a1b8, 3, 0x8000, 20);
    rec = Func_0808a080(20);
    {
        s32 shown = 0x3000;

        *(u16 *)((s32)rec + 6) = shown;
    }
    *(s32 *)((s32)rec + 24) = 0x10000;
    p9b = 0;
    Func_0808a100(20, 1);
    Func_0808a010(10);
    Func_020008a8(20, 0xd000);
    Func_0808a128(20, 6, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xa000, 0);
    Func_0808a1b8(1, 0xa000, 0);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a1b8(3, 0xa000, 0);
    Func_0808a1b8(21, 0xd000, 0);
    Func_0808a1b8(6, 0, 0);
    rec7 = Pointer4(Func_080090c8, 22, *(s32 *)((s32)rec + 8), (*(s32 *)((s32)rec + 12) + 0x80000), *(s32 *)((s32)rec + 16));
    if ((s32)rec7 != 0) {
        p6 = *(u8 **)(rec7 + 80);
        p6[39] = p9b;
        p6[5] &= -33;
        p6[9] = ((s32)(15 & p6[9]) & -13);
        rec7[35] &= 254;
        rec7[85] = p9b;
        {
            s32 shown = 1;

            rec7[92] = shown;
        }
        *(s32 *)((s32)rec7 + 48) = 0x19999;
        *(s32 *)((s32)rec7 + 52) = 0xcccc;
        record = Pointer2(Func_08000140, 17, 0x608);
        slot40 = (s32)record;
        Func_08015250(220);
        Func_080001c8(p6[28], 128, (slot40 + 0x400));
        Func_08000150(17);
    }
    Call2(Func_0808a1e0, 22, 1);
    v6 = 128;
    record = Func_0808a080(22);
    *(s32 *)((s32)record + 8) = *(s32 *)((s32)rec + 8);
    *(s32 *)((s32)record + 12) = (v6 << 14);
    *(s32 *)((s32)record + 16) = *(s32 *)((s32)rec + 16);
    record[85] = 3;
    *(s32 *)((s32)record + 48) = 0x19999;
    *(s32 *)((s32)record + 52) = 0xcccc;
    *(s32 *)((s32)record + 24) = 0xc000;
    *(s32 *)((s32)record + 28) = 0xc000;
    if ((s32)rec7 != 0) {
        rec7[85] = 3;
        *(s32 *)((s32)rec7 + 72) = 0x9999;
        *(s32 *)((s32)rec7 + 68) = 0xcccc;
        *(s32 *)((s32)rec7 + 40) = 0x80000;
        Call4(Func_08009150, (s32)rec7, 0x1340000, (v6 << 14), 0xa40000);
    }
    Call3(Func_0808a0c0, 22, 0x134, 164);
    Func_0808a0f0(22, 0, 0);
    Func_0808a1e0(21, 0);
    if ((s32)rec7 != 0) {
        Call1(Func_080f9010, 0x135);
        Func_080091e0((s32)rec7, 0);
        *(s32 *)((s32)rec7 + 40) = 0x40000;
        Call4(Func_08009150, (s32)rec7, 0x13a0000, (v6 << 14), 0x890000);
        Func_08009158((s32)rec7);
        Call1(Func_080f9010, 0x135);
        *(u8 *)(*(s32 *)((s32)rec7 + 80) + 9) = ((-13 & *(u8 *)(*(s32 *)((s32)rec7 + 80) + 9)) | 4);
        *(s32 *)((s32)rec7 + 40) = 0x60000;
        Call4(Func_08009150, (s32)rec7, 0x11d0000, (v6 << 14), 0x920000);
        Func_08009158((s32)rec7);
        Call1(Func_080f9010, 0x135);
        *(s32 *)((s32)rec7 + 40) = 0x50000;
        Call4(Func_08009150, (s32)rec7, 0x12c0000, (v6 << 14), 0x9a0000);
        Func_08009158((s32)rec7);
        Func_080000c0(6);
        *(s32 *)((s32)rec7 + 8) = 0;
        *(s32 *)((s32)rec7 + 12) = 0;
        *(s32 *)((s32)rec7 + 16) = 0;
        Func_02005688((s32)rec7);
    }
    Call3(Func_0808a1e8, 21, 0x100, 0);
    Call3(Func_0808a1e8, 6, 0x100, 0);
    Call3(Func_0808a1e8, 0, 0x100, 0);
    Call3(Func_0808a1e8, 1, 0x100, 0);
    Call3(Func_0808a1e8, 2, 0x100, 0);
    Call3(Func_0808a1e8, 3, 0x100, 30);
    Func_0808a1e0(21, 1);
    *(u8 *)(Func_0808a080(21) + 35) |= 1;
    Func_0808a130(2, 2);
    Func_02000894(2);
    Func_0808a138(3, 1);
    Func_0808a188(3, 0, 40);
    Func_0808a138(20, 1);
    Func_0808a010(20);
    Func_02000894(20);
    Call3(Func_0808a1b8, 0, 0x5000, 0);
    Call3(Func_0808a1b8, 1, 0x5000, 0);
    Call3(Func_0808a1b8, 2, 0x8000, 0);
    Call3(Func_0808a1b8, 3, 0x8000, 0);
    Func_0808a1b8(6, 0x3000, 0);
    Func_020008a8(21, 0x3000);
    Call3(Func_0808a1e8, 21, 0x101, 0);
    Call3(Func_0808a1e8, 6, 0x101, 0);
    Call3(Func_0808a1e8, 0, 0x101, 0);
    Call3(Func_0808a1e8, 1, 0x101, 0);
    Call3(Func_0808a1e8, 2, 0x101, 0);
    Call3(Func_0808a1e8, 3, 0x101, 60);
    Call3(Func_0808a1e8, 20, 0x108, 40);
    Func_02000894(20);
    Func_0808a1b8(0, 0xa000, 0);
    Call3(Func_0808a1b8, 1, 0x2000, 40);
    Value2(Func_0808a178, 1, 0);
    if (Value2(Func_0808a070, 0, 0) == 0) {
        Func_0808a100(1, 3);
        v7 = 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Func_0808a100(1, 4);
        v7 = 0;
    }
    Func_02000894(1);
    if (v7 != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_0808a010(20);
    record = Func_0808a080(24);
    Func_080091e0((s32)record, 0);
    Func_0808a158(24, 7);
    rec = Func_0808a080(24);
    none = 0;
    *(s32 *)((s32)rec + 28) = -0x10000;
    *(s32 *)((s32)rec + 24) = 0x1999;
    rec[85] = none;
    *(s32 *)((s32)rec + 12) = 0x400000;
    *(s32 *)((s32)rec + 16) = 0x9e0000;
    *(s32 *)((s32)rec + 8) = 0x1300000;
    record = Func_0808a080(25);
    Func_080091e0((s32)record, 0);
    Func_0808a158(25, 7);
    rec = Func_0808a080(25);
    *(s32 *)((s32)rec + 28) = -0x10000;
    *(s32 *)((s32)rec + 24) = 0x1999;
    rec[85] = none;
    *(s32 *)((s32)rec + 12) = 0x600000;
    *(s32 *)((s32)rec + 8) = 0x1300000;
    *(s32 *)((s32)rec + 16) = 0x9e0000;
    Call3(Func_0808a1e8, 21, 0x100, 0);
    Call3(Func_0808a1e8, 6, 0x100, 0);
    Call3(Func_0808a1e8, 0, 0x100, 0);
    Call3(Func_0808a1e8, 1, 0x100, 0);
    Call3(Func_0808a1e8, 2, 0x100, 0);
    Call3(Func_0808a1e8, 3, 0x100, 0);
    Call3(Func_0808a1b8, 1, 0xa000, 0);
    Call3(Func_0808a1b8, 2, 0xa000, 0);
    Call3(Func_0808a1b8, 3, 0xa000, 0);
    Call3(Func_0808a1b8, 21, 0xd000, 0);
    Func_0808a1b8(6, 0, 0);
    script = Data_0200e088;
    Func_0808a098(24, (s32)script);
    Func_0808a098(25, (s32)script);
    Func_080f9010(145);
    Call3(Func_080091f0, 0x60000, 0x60000, 0x10000);
    Call2(Func_0808a330, 0x4063ff, 0);
    Func_0808a348(16);
    Func_080000c0(20);
    Call2(Func_0808a330, 0x7fff, 0);
    Func_0808a348(24);
    Func_080000c0(60);
    Value2(Func_080000d0, 0x200b6d1, 0xc80);
    Func_080f9010(141);
    Call3(Func_080091f0, 0x40000, 0x40000, 0x10000);
    Call2(Func_0808a330, 0x4063ff, 0);
    Func_0808a348(120);
    script = Data_0200e0ac;
    Func_0808a098(24, (s32)script);
    Func_0808a098(25, (s32)script);
    Func_080000c0(120);
    Call3(Func_080091f0, 0x30000, 0x30000, 0x10000);
    Call2(Func_0808a330, 0x203210, 0);
    Func_0808a348(120);
    Func_080000c0(120);
    Func_080f9010(63);
    Call3(Func_080091f0, 0x20000, 0x20000, 0x10000);
    Call2(Func_0808a330, 0x10000, 0);
    Func_0808a348(120);
    Func_080000c0(120);
    Call3(Func_080091f0, 0x10000, 0x10000, 0x10000);
    Func_0808a100(19, 1);
    Func_0808a1b8(19, 0, 0);
    record = Func_0808a080(19);
    *(s32 *)((s32)record + 24) = 0x10000;
    Func_0808a128(19, 4, 40);
    Func_0808a138(19, 1);
    Func_0808a188(19, 0, 20);
    Call3(Func_0808a1b8, 0, 0x6000, 0);
    Call3(Func_0808a1b8, 1, 0x6000, 0);
    Call3(Func_0808a1b8, 2, 0x8000, 0);
    Call3(Func_0808a1b8, 3, 0x8000, 0);
    Call3(Func_0808a1b8, 21, 0x3000, 0);
    Call3(Func_0808a1b8, 6, 0x3000, 20);
    v6 = 128;
    Func_0808a138(20, 1);
    Func_02000894(20);
    Func_0808a138(3, 1);
    Func_020008a8(3, (v6 << 8));
    Func_02000894(3);
    Func_020008a8(20, 0);
    Func_0808a110(20, 4);
    Func_02000894(20);
    Call3(Func_0808a1e8, 19, 0x103, 20);
    Func_0808a130(19, 2);
    Func_02000894(19);
    Call3(Func_0808a1e8, 1, 0x100, 20);
    Func_020008a8(1, 0x6000);
    Func_02000894(1);
    Call2(Func_020008a8, 20, 0xd000);
    Func_0808a110(20, 3);
    Func_0808a188(20, 0, 20);
    Func_0808a138(1, 1);
    Func_02000894(1);
    Call3(Func_0808a1b8, 19, 0xb000, 20);
    Func_0808a138(19, 1);
    Func_02000894(0x2013);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a130(2, 1);
    Func_0808a138(3, 1);
    Func_0808a1b8(0, (v6 << 8), 0);
    Func_0808a1b8(1, (v6 << 8), 0);
    Call3(Func_0808a1b8, 2, 0xa000, 0);
    Call2(Func_020008a8, 3, 0xa000);
    Call3(Func_0808a1e8, 21, 0x101, 80);
    Func_02000894(21);
    Func_0808a1b8(0, 0xa000, 0);
    Call3(Func_0808a1b8, 1, 0x2000, 0);
    Func_0808a1b8(2, 0x6000, 0);
    Call3(Func_0808a1b8, 3, 0xe000, 40);
    Func_0808a138(20, 1);
    Func_02000894(20);
    Func_0808a1b8(0, (v6 << 8), 0);
    Func_0808a1b8(1, (v6 << 8), 0);
    Func_0808a1b8(2, (v6 << 8), 0);
    Call3(Func_0808a1b8, 3, (v6 << 8), 20);
    Func_020008a8(20, 0xb000);
    Func_02000894(0x2014);
    Func_0808a110(21, 3);
    Func_0808a1b8(21, 0xb000, 60);
    Call3(Func_0808a1b8, 21, 0x3000, 40);
    Func_0808a138(19, 1);
    Func_02000894(0x2013);
    Func_0808a1b8(21, 0, 40);
    Func_02000894(21);
    Call3(Func_0808a1b8, 20, 0xd000, 40);
    Func_020008a8(20, 0xb000);
    Func_02000894(0x2014);
    Call3(Func_0808a1b8, 21, 0x3000, 40);
    Call3(Func_0808a1b8, 21, 0x3000, 20);
    Func_0808a110(21, 3);
    Func_0808a1b8(19, 0, 40);
    Func_0808a1b8(19, 0xb000, 20);
    Call3(Func_0808a1e8, 19, 0x101, 40);
    Func_02000894(0x2013);
    Call2(Func_020008a8, 21, 0x3000);
    Call3(Func_0808a1e8, 21, 0x101, 20);
    Func_02000894(21);
    Call3(Func_0808a1b8, 20, 0xb000, 20);
    Func_0808a100(19, 4);
    Func_02000894(0x2013);
    Call3(Func_0808a1e8, 21, 0x103, 80);
    Func_02000894(21);
    Func_0808a110(19, 3);
    Func_02000894(0x2013);
    Call3(Func_0808a1e8, 21, 0x103, 20);
    Func_0808a130(21, 2);
    Func_02000894(21);
    Func_0808a100(19, 4);
    Func_02000894(0x2013);
    Call3(Func_0808a1e8, 21, 0x102, 60);
    Func_0808a138(21, 1);
    Func_02000894(21);
    Call3(Func_0808a1e8, 20, 0x108, 40);
    Func_0808a100(20, 4);
    Func_0808a188(0x2014, 0, 40);
    Func_0808a138(21, 1);
    Func_0808a010(20);
    Func_02000894(21);
    Call3(Func_0808a1e8, 0, 0x100, 0);
    Call3(Func_0808a1e8, 1, 0x100, 0);
    Call3(Func_0808a1e8, 2, 0x100, 0);
    Call3(Func_0808a1e8, 3, 0x100, 0);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a1b8(3, 0xa000, 20);
    Call3(Func_0808a1b8, 19, 0x3000, 20);
    Func_0808a100(19, 3);
    Func_0808a110(20, 3);
    Func_0808a1b8(21, 0xb000, 20);
    Call1(Func_02000894, 0xa015);
    Call3(Func_0808a090, 6, 0x10000, (v6 << 8));
    Call3(Func_0808a090, 21, 0x10000, (v6 << 8));
    script = Data_0200e22c;
    Func_0808a098(21, (s32)script);
    Func_0808a010(20);
    Call2(Func_0808a098, 6, (s32)script);
    Func_020008a8(1, 0x6000);
    Func_0808a130(1, 2);
    Func_02000894(1);
    Func_080f9010(17);
    Call2(Func_0808a330, 0x40250d, 1);
    Func_0808a348(40);
    Call2(Func_020008a8, 20, 0xd000);
    Func_02000894(20);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(2, (v6 << 8), 0);
    Call3(Func_0808a1b8, 3, (v6 << 8), 0);
    Func_020035c4();
    Func_0808a158(20, 7);
    Func_0808a158(19, 7);
    Func_0808a010(20);
    Call2(Func_0808a158, 20, 0x100);
    Call2(Func_0808a158, 19, 0x100);
    Func_0808a010(20);
    Func_020035c4();
    Func_0808a128(3, 2, 20);
    Func_02000894(3);
    Func_020008a8(19, 0);
    Func_02000894(19);
    Func_020035c4();
    rec4 = Func_0808a080(19);
    options_a.mode = 7;
    options_a.callback = (u32)Func_020003a0;
    options_a.accum18 = 0x10000;
    options_a.accum1c = 0x10000;
    i = 0;
    do {
        velocity[0] = Func_08000120(i << 12);
        velocity[1] = 0;
        velocity[2] = Func_08000118(i << 12) * 2;
        velocity[0] *= 3;
        Func_0200013c(*(s32 *)(rec4 + 8), *(s32 *)(rec4 + 12),
                     *(s32 *)(rec4 + 16), velocity[0], velocity[1],
                     velocity[2], 0x1090000, &options_a);
        i++;
    } while (i <= 16);
    Func_080f9010(212);
    Func_080000c0(6);
    Func_020035c4();
    rec4 = Func_0808a080(20);
    options_b.mode = 7;
    options_b.callback = (u32)Func_020003a0;
    options_b.accum18 = 0x10000;
    options_b.accum1c = 0x10000;
    i = 0;
    do {
        velocity[0] = Func_08000120(i << 12);
        velocity[1] = 0;
        velocity[2] = Func_08000118(i << 12) * 2;
        velocity[0] *= 3;
        Func_0200013c(*(s32 *)(rec4 + 8), *(s32 *)(rec4 + 12),
                     *(s32 *)(rec4 + 16), velocity[0], velocity[1],
                     velocity[2], 0x1090000, &options_b);
        i++;
    } while (i <= 16);
    Func_080f9010(212);
    Func_0808a128(2, 6, 20);
    Func_080f9010(54);
    Func_02000894(2);
    Func_0808a100(20, 4);
    Func_02000894(20);
    Func_020035c4();
    Call3(Func_0808a090, 20, 0x3333, 0x1999);
    Call3(Func_0808a090, 19, 0x3333, 0x1999);
    *(u8 *)(Func_0808a080(20) + 90) &= 254;
    *(u8 *)(Func_0808a080(19) + 90) &= 254;
    Call3(Func_0808a0b8, 20, 0x126, 196);
    Call3(Func_0808a0b8, 19, 0x126, 196);
    Value2(Func_080000d0, 0x200b7c5, 0xc80);
    Func_0808a130(1, 2);
    Func_0808a180(1, 0);
    Value1(Func_080770c8, 0x234);
    Func_0808a180(2, 0);
    Value1(Func_080770c8, 0x235);
    Func_020035c4();
    Func_0808a010(20);
    Func_020035c4();
    Func_0808a010(20);
    Data_02000240[0x22b] = 3;
    Call2(Func_0808a260, 187, 3);
    Func_0808a268(187, 9);
    Call2(Func_0808a250, 98, 0);
    Func_08077268();
    Value1(Func_080770c8, 0x351);
}
