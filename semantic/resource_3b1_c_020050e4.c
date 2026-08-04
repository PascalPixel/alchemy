typedef unsigned char u8;
typedef signed int s32;

void Func_0200486c();
void Func_02004880();
void Func_020048ac();
void Func_020048e8();
void Func_080000c0();
void Func_08015210();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b0();
void Func_0808a0c0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a138();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a1d8();
void Func_0808a208();
void Func_0808a360();

/*
 * resource_3b1 owner at 0x020050e4, 528 bytes including its ten-word trailing
 * pool. It initializes a four-actor tableau, copies three nullable positions,
 * then runs the staged transition and flag teardown. The sole return is at
 * 0x020052ca. All 48 BL sites across 24 targets appear in machine order.
 */
void Func_020050e4(void)
{
    u8 *record;

    Func_020048ac(0x01b80000, -1, 0x00b00000, 0x01000001);
    Func_0808a0f0(0, 0x01b80000, 0x00860000);
    Func_0808a360();
    Func_0808a090(0, 0x19999, 0xcccc);
    Func_0808a100(0, 5);
    Func_0808a0c0(0, 0x198, 0x86);
    Func_0808a0c0(0, 0x198, 0x98);
    Func_0808a0c0(0, 0x1b0, 0xa6);
    Func_0808a100(0, 1);
    Func_0808a1b8(0, 0x4000, 0);

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(1);
    if (record != 0)
        Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));

    Func_080000c0(1);
    Func_0808a090(2, 0x19999, 0xcccc);
    Func_0808a0c8(2, 0x1a8, 0x98);
    Func_0808a090(1, 0x19999, 0xcccc);
    Func_0808a0c8(1, 0x1c0, 0xa8);
    Func_0808a090(3, 0x20000, 0x10000);
    Func_0808a0d0(3, 0x1ca, 0x98);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a1b8(2, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(3, 0x4000, 40);
    Func_0808a1d8(10);
    Func_08015210(0x1e46, 1, 10);
    Func_0808a010(10);
    Func_020048e8(0, 0, 40);
    Func_020048e8(1, 0x4000, 20);
    Func_0808a208(0x39999, 0x7333);
    Func_020048ac(0x01b80000, -1, 0x01400000, 0x10000014);
    Func_0808a138(8, 2);
    Func_02004880(8, 0xd000);
    Func_0808a170(0x1e47);
    Func_0200486c(8);
    Func_0808a1b8(8, 0, 20);
    Func_020048ac(0x01b80000, -1, 0x00860000, 0x10000000);
    Func_0808a098(1, 0x0200e7c8);
    Func_0808a098(2, 0x0200e7c8);
    Func_0808a0b0(3, 0x0200e7c8);
    Func_0808a010(40);
    Func_080770c8(0x301);
    Func_020048e8(23, 0, 0);
    Func_080770d0(0x12f);
}
