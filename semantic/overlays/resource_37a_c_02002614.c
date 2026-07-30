#include "types.h"

/*
 * Several overlay services intentionally accept different setup forms at
 * different call sites.  Keep their declarations unprototyped until the
 * shared overlay ABI headers describe those mode-dependent argument lists.
 */
void Func_02004e54();
void Func_02004fa6();
void Func_02004fc0();
void Func_02004fc2();
void Func_02004ff2();
void Func_02005008();
void Func_02005032();
void Func_0200505e();
void Func_0200506e();
void Func_02005074();
void Func_02005080();
void Func_020050ac();
void Func_020050cc();
void Func_020050e4();
void Func_02005106();
void Func_0200510a();
void Func_02005110();
void Func_0200511c();
void Func_02005136();
void Func_0200514c();
void Func_02005154();
void Func_02005158();
void Func_0200515e();
void Func_02005160();
void Func_02005162();
void Func_0200516c();
void Func_02005176();
void Func_02005178();
void Func_0200517e();
void Func_02005184();
void Func_0200518c();
void Func_02005190();
void Func_0200519c();
void *Func_020051a8(s32);
void Func_020051ae();
void Func_020051b4();
void *Func_020051ba(s32);
void Func_020051be();
void Func_020051ca();
void Func_020051cc();
void Func_020051da();
void Func_020051dc();
void Func_020051ee();
void Func_020051f2();
void Func_020051fe();
void *Func_02005200(s32);
void *Func_02005210(s32);
void Func_02005214();
void Func_0200521c();
void Func_0200521e();
void *Func_02005264(s32);
void Func_0200526a();
void Func_02005284();
void Func_02005286();
void Func_0200528c();
void Func_02005290();
void Func_02005294();
void Func_020052a4();
void Func_020052b6();
void Func_020052c8();
void Func_0200531e();

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

    Func_02004ff2();
    Func_020050cc(0x011e0000, -1, 0x860000, 0);
    Func_02004fc0();
    Func_02004fa6(1);
    Func_02005106(0x7fff, 0);
    Func_02005106(0x7fff, 0);
    Func_0200511c(1);
    Func_02004fc2(1);
    Func_02005080(1, 0x01180000, 0x860000);
    Func_020050e4(1, 0xa000, 0);

    runtime = runtime_globals[0];
    *(s32 *)(runtime + 0x1c8) = 1;

    Func_02005158();
    Func_0200516c();
    Func_0200514c(0x2051cc, 1);
    Func_02005162(0x78);
    Func_02005008(0x78);
    Func_0200505e(0x201);
    Func_0200506e(0x200);
    Func_02005074(0x202);
    Func_02005176(0x10000, 2);
    Func_0200518c(0x3c);
    Func_02005032(0x64);
    Func_02005154(0, 0x102);
    Func_0200515e(1, 0x102);
    Func_020050ac(0x3c);
    Func_02005160(0, 0x2000, 0x14);
    Func_0200516c(1, 0xe000, 0x28);
    Func_02005178(0, 0x9000, 0x28);
    Func_02005184(1, 0x5000, 0x50);
    Func_02005190(0, 0x8000, 0x0a);
    Func_0200519c(1, 0x1000, 0x3c);
    Func_020050e4(0x30000, 0x30000, 0x10000);
    Func_0200510a(0x14);
    Func_020051be(0, 0x6000, 0);
    Func_020051ca(1, 0xe000, 0x14);
    Func_02005110(-1, -1, 0xe666);
    Func_02005136(0x14);
    Func_020051ae(1, 2);
    Func_020051cc(0x1119);
    Func_020051dc(1, 0);
    Func_0200516c(0, 0);
    Func_020051b4(0, 1);
    Func_02005162(0x28);
    Func_02005290(0x6b);
    Func_02005158(0x40000, 0x40000, 0x10000);
    Func_0200517e(0x28);
    Func_0200526a();

    object = Func_020051a8(0);
    object[0x5a] &= 0xfe;
    object = Func_020051ba(1);
    object[0x5a] &= 0xfe;

    Func_02005214(0, 4, 0);
    Func_0200521e(1, 4, 0);
    Func_020051f2(0, 0x12c, 0x82);
    Func_020051fe(1, 0x10a, 0x90);
    Func_0200521c(1);
    Func_020051da(0x28);

    object = Func_02005200(0);
    object[0x5a] |= 1;
    object = Func_02005210(1);
    object[0x5a] |= 1;

    Func_0200531e(0x8d);
    Func_020051ee(0x10000, 0x10000, 0x10000);
    Func_02005214(0x28);
    Func_0200528c(1, 2);
    Func_02004e54(1, 0x0a);
    Func_02005284(0, 3);
    Func_02005294(1, 3);
    Func_02005294(1, 2);

    probe = Func_02005264(0);
    if (probe != 0)
        Func_02005286(
            1,
            *(s16 *)(probe + 0x0a),
            *(s16 *)(probe + 0x12));

    Func_020052a4(1);
    Func_020052b6(1, 0, 0);

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
    Func_020052c8();
}
