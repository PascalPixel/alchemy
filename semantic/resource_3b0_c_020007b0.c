typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3b0 second scripted scene owner at 0x020007b0, complete 724-byte
 * span through alignment and its six-word pool before 0x02000a84.  The nine
 * local 0x02000a84 setup calls and all 63 imported calls remain explicit.
 */
extern u8 *Data_03001ebc;
extern void Func_02000a84(s32 actorId);
extern void Func_080000c0();
extern void Func_08009150();
extern void Func_08009158();
extern void Func_080091e0();
extern void Func_080770c8();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a030();
extern u8 *Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a098();
extern void Func_0808a0a8();
extern void Func_0808a0b8();
extern void Func_0808a0d0();
extern void Func_0808a0f0();
extern void Func_0808a128();
extern void Func_0808a138();
extern void Func_0808a158();
extern void Func_0808a1b8();
extern void Func_0808a1e8();
extern void Func_0808a248();
extern void Func_0808a360();
extern void Func_0808a368();
extern void Func_0808a370();
extern void Func_080f9010();

void Func_020007b0(void)
{
    u8 *actor;

    Func_0808a018();
    Func_0808a158(0, 15);
    actor = Func_0808a080(0);
    Func_080091e0(actor, 0);
    Func_080000c0(1);
    Func_0808a030((void *)0x0200976c);
    Func_080000c0(1);

    Func_02000a84(9);
    Func_02000a84(10);
    Func_02000a84(11);
    Func_02000a84(12);
    Func_02000a84(13);
    Func_02000a84(14);
    Func_02000a84(15);

    Func_0808a098(8, (void *)0x0200939c);
    *(s32 *)(Data_03001ebc + 448) = 515;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(120);

    actor = Func_0808a080(9);
    Func_0808a0a8(9);
    *(u32 *)(actor + 56) = 0x80000000u;
    *(u32 *)(actor + 60) = 0x80000000u;
    *(u32 *)(actor + 64) = 0x80000000u;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 40) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 76) = 0;
    Func_0808a010(20);
    Func_0808a090(9, 0x80000, 0x40000);
    Func_08009150(actor, 0x00a40000, 0x00900000, 0x01410000);
    Func_08009158(actor);
    Func_08009150(actor, 0x00a40000, 0x00680000, 0x01410000);
    Func_08009158(actor);
    Func_08009150(actor, 0x00cc0000, 0x007c0000, 0x01410000);
    Func_08009158(actor);
    Func_08009150(actor, 0x00900000, 0, 0x00a90000);

    Func_0808a0a8(8);
    Func_080000c0(1);
    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a1e8(8, 0x103, 60);
    Func_0808a090(9, 0x20000, 0x10000);
    Func_02000a84(9);
    Func_0808a010(20);
    Func_0808a098(8, (void *)0x0200939c);
    Func_0808a010(120);

    Func_0808a0a8(9);
    *(u32 *)(actor + 56) = 0x80000000u;
    *(u32 *)(actor + 60) = 0x80000000u;
    *(u32 *)(actor + 64) = 0x80000000u;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 40) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 76) = 0;
    Func_0808a010(20);
    Func_0808a090(9, 0x80000, 0x40000);
    Func_08009150(actor, 0x00a40000, 0x00900000, 0x01410000);
    Func_08009158(actor);
    Func_0808a090(9, 0x50000, 0x28000);
    Func_08009150(actor, 0x00a40000, 0x00680000, 0x01410000);
    Func_08009158(actor);
    Func_08009150(actor, 0x00a40000, 0x00720000, 0x01410000);
    Func_08009158(actor);
    Func_08009150(actor, 0x00a40000, 0x00680000, 0x01410000);
    Func_08009158(actor);
    Func_08009150(actor, 0x00cc0000, 0x007c0000, 0x01410000);
    Func_08009158(actor);
    Func_08009150(actor, 0x00900000, 0, 0x00a90000);

    Func_0808a0a8(8);
    Func_080000c0(1);
    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a1e8(8, 0x103, 60);
    Func_0808a090(9, 0x20000, 0x10000);
    Func_02000a84(9);
    Func_0808a128(8, 4, 20);
    Func_0808a128(8, 6, 40);
    Func_080f9010(29);
    Func_080770c8(0x8f0);
    Func_0808a170(0x1e49);
    Func_0808a188(16, 0, 20);
    Func_0808a1e8(8, 0x100, 0);
    Func_0808a0d0(8, 0xa4, 0x158);
    Func_0808a010(40);
    Func_0808a138(8, 2);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(12);
    Func_0808a020();
}
