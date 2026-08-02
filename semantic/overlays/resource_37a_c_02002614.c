#include "types.h"

/*
 * Several overlay imports intentionally accept different setup forms at
 * different call sites. Keep them unprototyped until shared ABI headers
 * describe those mode-dependent argument lists.
 */
void Func_020025fc(s32, s32);
void Func_080000c0();
void Func_08009128();
void Func_080091f0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a070();
u8 *Func_0808a080();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a1b8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a2f8();
void Func_0808a330();
void Func_0808a338();
void Func_0808a348();
void Func_0808a360();
void Func_0808a370();
void Func_0808a5e0();
void Func_080f9010();

/*
 * Configure the overlay's complete opening scene: display layers, two actors,
 * camera/effect tracks, timing, object visibility, and the shared runtime
 * defaults consumed by the frame loop that follows.
 */
void Func_02002614(void)
{
    u8 **runtime_globals = (u8 **)0x03001ebc;
    u8 *runtime;
    u8 *scene;
    u8 *object;
    u8 *probe;

    Func_0808a018();
    Func_0808a210(0x011e0000, -1, 0x860000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a338(0x7fff, 0);
    Func_0808a330(0x7fff, 0);
    Func_0808a348(1);
    Func_080000c0(1);
    Func_0808a0f0(1, 0x01180000, 0x860000);
    Func_0808a1b8(1, 0xa000, 0);

    runtime = runtime_globals[0];
    *(s32 *)(runtime + 0x1c8) = 1;

    Func_0808a360();
    Func_0808a370();
    Func_0808a330(0x2051cc, 1);
    Func_0808a348(0x78);
    Func_080000c0(0x78);
    Func_080770c8(0x201);
    Func_080770d0(0x200);
    Func_080770d0(0x202);
    Func_0808a330(0x10000, 2);
    Func_0808a348(0x3c);
    Func_080000c0(0x64);
    Func_0808a1f0(0, 0x102);
    Func_0808a1f0(1, 0x102);
    Func_0808a010(0x3c);
    Func_0808a1b8(0, 0x2000, 0x14);
    Func_0808a1b8(1, 0xe000, 0x28);
    Func_0808a1b8(0, 0x9000, 0x28);
    Func_0808a1b8(1, 0x5000, 0x50);
    Func_0808a1b8(0, 0x8000, 0x0a);
    Func_0808a1b8(1, 0x1000, 0x3c);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_0808a010(0x14);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0xe000, 0x14);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(0x14);
    Func_0808a138(1, 2);
    Func_0808a170(0x1119);
    Func_0808a178(1, 0);
    Func_0808a070(0, 0);
    Func_0808a100(0, 1);
    Func_0808a010(0x28);
    Func_080f9010(0x6b);
    Func_080091f0(0x40000, 0x40000, 0x10000);
    Func_0808a010(0x28);
    Func_0808a2f8();

    object = Func_0808a080(0);
    object[0x5a] &= 0xfe;
    object = Func_0808a080(1);
    object[0x5a] &= 0xfe;

    Func_0808a128(0, 4, 0);
    Func_0808a128(1, 4, 0);
    Func_0808a0b8(0, 0x12c, 0x82);
    Func_0808a0b8(1, 0x10a, 0x90);
    Func_0808a0e8(1);
    Func_0808a010(0x28);

    object = Func_0808a080(0);
    object[0x5a] |= 1;
    object = Func_0808a080(1);
    object[0x5a] |= 1;

    Func_0808a5e0(0x8d);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(0x28);
    Func_0808a138(1, 2);
    Func_020025fc(1, 0x0a);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a100(1, 2);

    probe = Func_0808a080(0);
    if (probe != 0)
        Func_0808a0b8(
            1,
            *(s16 *)(probe + 0x0a),
            *(s16 *)(probe + 0x12));

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    scene = runtime_globals[5];
    *(s16 *)(scene + 0x0e00) = 0;
    *(s16 *)(scene + 0x0e02) = 0;
    *(s16 *)(scene + 0x0e04) = 0;
    scene[0x2a00] = 0;
    scene[0x2a01] = 1;
    scene[0x2a02] = 1;
    scene[0x2a03] = 1;
    *(volatile u16 *)0x05000000 = 0;

    runtime = runtime_globals[0];
    *(s32 *)(runtime + 0x1c0) = 0x204;
    *(s32 *)(runtime + 0x1c8) = 0x10;
    Func_0808a020();
}
