typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern void Func_0200013c();
extern void Func_080000d0();
extern void Func_080000d8();
extern void Func_08000128();
extern s32 Func_080770c0();
extern void Func_080770c8();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern u8 *Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a098();
extern void Func_0808a0a0();
extern void Func_0808a0b8();
extern void Func_0808a0e0();
extern void Func_0808a0e8();
extern void Func_0808a100();
extern void Func_0808a138();
extern void Func_0808a148();
extern void Func_0808a170();
extern void Func_0808a180();
extern void Func_0808a188();
extern void Func_0808a1b8();
extern void Func_0808a5e8();
extern void Func_080f9010();

static s32 Fixed16TowardZero_020009dc(s32 value)
{
    if (value < 0)
        value += 0xffff;
    return value >> 16;
}

/* Complete 784-byte scene controller; all 57 call sites remain explicit. */
void Func_020009dc(void)
{
    s32 offset[3] = {0, 0, 0};
    s32 config[9];
    u8 *actor;
    u8 *owner;

    Func_0808a018();
    if (Func_080770c0(0x202) != 0) {
        Func_0808a148(14, 0, 0);
        Func_0808a010(10);
        Func_0808a170(0x17f4);
        Func_0808a180(14, 0);
        Func_0808a1b8(14, 0, 10);
        Func_0808a020();
        return;
    }

    Func_0808a170(0x17f2);
    Func_0808a180(14, 0);
    Func_0808a138(0, 2);
    Func_0808a080(0)[0x5a] &= (u8)~1;

    Func_08000128((s32)0xfff80000,
                  *(u16 *)(Func_0808a080(0) + 6), offset);
    Func_0808a100(0, 2);
    Func_0808a0e0(0, Fixed16TowardZero_020009dc(offset[0]),
                  Fixed16TowardZero_020009dc(offset[2]));
    Func_0808a0e8(0);
    Func_0808a080(0)[0x5a] |= 1;
    Func_0808a010(30);
    Func_0808a138(14, 2);

    Func_0200013c(0x00c00000, 0, 0x01380000,
                  0x1999, 0x3333, 0, 0x00020001, 0);
    Func_0200013c(0x00c00000, 0, 0x01380000,
                  0x3333, 0x1999, 0, 0x00020001, 0);
    Func_080f9010(132);
    Func_080000d0((u8 *)0x02008371, 3200);

    actor = Func_0808a080(14);
    *(s32 *)(actor + 40) = 0x60000;
    actor = Func_0808a080(14);
    *(s32 *)(actor + 72) = 0x10000;
    actor = Func_0808a080(14);
    *(s32 *)(actor + 68) = 0;
    Func_0808a090(14, 0x30000, 0x18000);
    Func_0808a0b8(14, 168, 312);
    Func_0808a0e8(14);
    Func_080f9010(134);
    Func_0808a098(19, (u8 *)0x020096a0);
    Func_080000d0((u8 *)0x02008359, 3200);

    config[0] = 1;
    *(u16 *)((u8 *)config + 24) = 0x011b;
    *(s32 *)((u8 *)config + 28) = 0x02009714;
    *(u16 *)((u8 *)config + 32) = 0x4000;

    Func_0200013c(0x00a80000, 0, 0x014c0000,
                  0, 0, 0, 0x00720000, config);
    Func_0808a0b8(14, 146, 312);
    Func_0808a0e8(14);
    Func_080000d8((u8 *)0x02008371);

    Func_0200013c(0x00900000, 0, 0x01380000,
                  0, 0, 0, 0x00020001, 0);
    Func_0200013c(0x00900000, 0, 0x01380000,
                  (s32)0xffffcccd, 0x1999, 0, 0x00020001, 0);
    Func_0200013c(0x00900000, 0, 0x01380000,
                  (s32)0xffff8000, 0, 0, 0x00020001, 0);
    Func_0808a0a0(19);
    Func_080f9010(124);

    Func_0200013c(0x00a80000, 0x00080000, 0x01380000,
                  0, 0, 0, 0x00020001, 0);
    Func_0200013c(0x00a80000, 0x00080000, 0x01380000,
                  0x3333, 0, 0, 0x00020001, 0);
    Func_0200013c(0x00a80000, 0x00080000, 0x01380000,
                  (s32)0xffffcccd, 0, 0, 0x00020001, 0);
    Func_080000d8((u8 *)0x02008359);

    actor = Func_0808a080(19);
    owner = *(u8 **)(actor + 80);
    *(u16 *)(owner + 30) = 0x8000;
    *(s32 *)(Func_0808a080(14) + 68) = 0x4000;
    *(s32 *)(Func_0808a080(14) + 72) = 0x10000;
    Func_0808a010(30);
    Func_0808a1b8(14, 0, 20);
    Func_0808a138(14, 2);
    Func_0808a010(20);
    Func_0808a188(14, 0, 20);
    Func_080770c8(0x202);
    Func_0808a5e8();
    Func_0808a020();
}
