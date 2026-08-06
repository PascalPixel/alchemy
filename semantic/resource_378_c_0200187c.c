#include "types.h"

/*
 * resource_378 scripted scene owner at 0x0200187c (3,400 bytes).
 *
 * Boundary evidence: `push {r5,r6,lr}` at 0x0200187c; the owner has a
 * single epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020025b6-0x020025ba.  The two trailing pool words at
 * 0x020025bc-0x020025c3 belong to this owner, and the next prologue is
 * 0x020025c4.  The control-flow walk also accounts for the branch-skipped
 * pools at 0x02001bb0-0x02001bcf and 0x02002224-0x0200222b.
 *
 * This is one coherent straight-line cutscene script.  Calls retain the
 * observed literal/register arguments where the byte sequence establishes
 * them.  Old-style declarations are intentional: these shared scene
 * helpers have several historical signatures across the overlays.  The
 * only back-edge is the 32-iteration actor-record update loop.
 *
 * Call accounting: 400 resolved BL sites, 29 distinct targets.  The loop
 * is represented once per machine BL site, as required by the ordered and
 * per-target multiset checks.
 */

extern u8 *Data_03001ebc;































extern void Func_02004f5e();
extern void Func_02004f16();
extern void Func_02004e7c();
extern void Func_02004eb6();
extern void Func_02004ec0();
extern void Func_02004eca();
extern void Func_02004ed4();
extern void Func_02004ede();
extern void Func_02004ee8();
extern void Func_02004ef2();
extern void Func_02004fd4();
extern void Func_02004fe0();
extern void Func_02004f8a();
extern void Func_02004f94();
extern void Func_02004f9e();
extern void Func_02004fa8();
extern void Func_02004fba();
extern void Func_02004f10();
extern void Func_02004f90();
extern void Func_02004fa0();
extern void Func_02004f26();
extern void Func_02004fa6();
extern void Func_02004fae();
extern void Func_02004fb6();
extern void Func_02004fc6();
extern void Func_02004f4c();
extern void Func_02004ffe();
extern void Func_02005008();
extern void Func_02005012();
extern void Func_0200501c();
extern void Func_02005026();
extern void Func_02004f84();
extern void Func_0200500c();
extern void Func_02004f92();
extern void Func_02005058();
extern void Func_02005070();
extern void Func_02005050();
extern void Func_020050aa();
extern void Func_02004fc8();
extern void Func_020050a0();
extern void Func_02004fd6();
extern void Func_0200507e();
extern void Func_02004fe4();
extern void Func_02005096();
extern void Func_02004ff4();
extern void Func_0200507c();
extern void Func_02005002();
extern void Func_020050b4();
extern void Func_020050be();
extern void Func_020050a4();
extern void Func_0200502a();
extern void Func_0200510e();
extern void Func_0200503c();
extern void Func_020050c4();
extern void Func_0200504a();
extern void Func_02005122();
extern void Func_020051a6();
extern void Func_02005186();
extern void Func_02005194();
extern void Func_02005072();
extern void Func_02005172();
extern void Func_0200518c();
extern void Func_02005092();
extern void Func_020051e0();
extern void Func_020051e8();
extern void Func_0200511e();
extern void Func_020050e8();
extern void Func_02005112();
extern void Func_02005212();
extern void Func_020050d0();
extern void Func_02005178();
extern void Func_02005158();
extern void Func_020040c0();
extern void Func_02005196();
extern void Func_020051a0();
extern void Func_020051aa();
extern void Func_020051b4();
extern void Func_020051be();
extern void Func_020051c8();
extern void Func_020051c0();
extern void Func_020051d0();
extern void Func_020051d8();
extern void Func_02005156();
extern void Func_0200522e();
extern void Func_02005164();
extern void Func_0200516a();
extern void Func_02005242();
extern void Func_0200523a();
extern void Func_020052d0();
extern void Func_020052d8();
extern void Func_02004d8c();
extern void Func_020051c2();
extern u8 * Func_020051f0();
extern u8 * Func_020051fc();
extern void Func_02005330();
extern void Func_020052a8();
extern void Func_0200526a();
extern void Func_02005200();
extern void Func_02005312();
extern void Func_0200531e();
extern void Func_02005340();
extern void Func_0200534e();
extern void Func_0200522c();
extern void Func_020042a8();
extern void Func_020052b0();
extern void Func_02005386();
extern void Func_02005244();
extern void Func_020052ec();
extern void Func_02005252();
extern void Func_020052f2();
extern void Func_020052fa();
extern void Func_02005302();
extern void Func_0200530a();
extern void Func_02005322();
extern void Func_02005288();
extern void Func_02005360();
extern void Func_02005296();
extern void Func_02005348();
extern void Func_020052a6();
extern void Func_02005358();
extern void Func_020052b6();
extern void Func_02005368();
extern void Func_02005372();
extern void Func_0200537c();
extern void Func_02005390();
extern void Func_020052ee();
extern void Func_020053c6();
extern void Func_020052fc();
extern void Func_020053ae();
extern void Func_0200530c();
extern void Func_02005394();
extern void Func_0200531a();
extern void Func_0200539a();
extern void Func_020053aa();
extern void Func_020053e2();
extern void Func_020053c8();
extern void Func_02005426();
extern void Func_0200535c();
extern void Func_02005450();
extern void Func_0200545c();
extern void Func_02005468();
extern void Func_02005474();
extern void Func_02005480();
extern void Func_0200548c();
extern void Func_0200549e();
extern void Func_020053bc();
extern void Func_02005494();
extern void Func_020053ca();
extern void Func_0200544a();
extern void Func_02005452();
extern void Func_0200545a();
extern void Func_02005462();
extern void Func_0200546a();
extern void Func_0200547a();
extern void Func_02005400();
extern void Func_02005488();
extern void Func_0200540e();
extern void Func_02005500();
extern void Func_0200541e();
extern void Func_020054d8();
extern void Func_020054e2();
extern void Func_02005438();
extern void Func_020054ea();
extern void Func_020054f4();
extern void Func_020054fe();
extern void Func_02005508();
extern void Func_02005466();
extern void Func_0200550e();
extern void Func_0200554c();
extern void Func_02005482();
extern void Func_02005574();
extern void Func_02005492();
extern void Func_02005532();
extern void Func_0200553a();
extern void Func_02005542();
extern void Func_0200554a();
extern void Func_02005552();
extern void Func_02005562();
extern void Func_020054c8();
extern void Func_020055a0();
extern void Func_020054d6();
extern void Func_020055c8();
extern void Func_020054e6();
extern void Func_020055da();
extern void Func_020054f8();
extern void Func_020055d0();
extern void Func_02005506();
extern void Func_020055f8();
extern void Func_02005516();
extern void Func_020055be();
extern void Func_02005612();
extern void Func_02005530();
extern void Func_020055d8();
extern void Func_020055e0();
extern void Func_020055e8();
extern void Func_020055f0();
extern void Func_02005600();
extern void Func_02005566();
extern void Func_0200563e();
extern void Func_020055f4();
extern void Func_020055fc();
extern void Func_02005604();
extern void Func_0200560c();
extern void Func_02005614();
extern void Func_02005624();
extern void Func_020055aa();
extern void Func_02005682();
extern void Func_020055b8();
extern void Func_02005638();
extern void Func_02005640();
extern void Func_02005648();
extern void Func_02005650();
extern void Func_02005658();
extern void Func_02005668();
extern void Func_020055ee();
extern void Func_02005696();
extern void Func_020056d4();
extern void Func_0200560a();
extern void Func_02005692();
extern void Func_02005618();
extern void Func_020056f0();
extern void Func_02005626();
extern void Func_02005718();
extern void Func_02005722();
extern void Func_0200572c();
extern void Func_02005738();
extern void Func_02005742();
extern void Func_0200574c();
extern void Func_0200566a();
extern void Func_02005712();
extern void Func_0200574a();
extern void Func_02005680();
extern void Func_02005732();
extern void Func_02005690();
extern void Func_0200569e();
extern void Func_02005776();
extern void Func_020056ac();
extern void Func_020057a0();
extern void Func_020057ac();
extern void Func_020057b8();
extern void Func_020057c4();
extern void Func_020057d0();
extern void Func_020057dc();
extern void Func_020056fa();
extern void Func_0200570a();
extern void Func_02005792();
extern void Func_020057ea();
extern void Func_02005720();
extern void Func_0200578a();
extern void Func_020057e6();
extern void Func_020057f0();
extern void Func_020057fa();
extern void Func_02005804();
extern void Func_0200580e();
extern void Func_02005822();
extern void Func_0200582c();
extern void Func_02005836();
extern void Func_02005870();
extern void Func_0200587c();
extern void Func_02005888();
extern void Func_02005814();
extern void Func_020057c2();
extern void Func_020058a6();
extern void Func_020058b2();
extern void Func_020058be();
extern void Func_02005854();
extern void Func_02005866();
extern void Func_020057fc();
extern void Func_020058f0();
extern void Func_02005906();
extern void Func_02005912();
extern void Func_0200591e();
extern void Func_0200592a();
extern void Func_02005936();
extern void Func_0200585a();
extern void Func_0200590c();
extern void Func_02005916();
extern void Func_02005920();
extern void Func_0200597a();
extern void Func_02005898();
extern void Func_0200594a();
extern void Func_020058a8();
extern void Func_02005950();
extern void Func_020058b6();
extern void Func_0200599a();
extern void Func_020058c8();
extern void Func_020059bc();
extern void Func_020058da();
extern void Func_0200598c();
extern void Func_020058f2();
extern void Func_020059ca();
extern void Func_02005900();
extern void Func_020059e4();
extern void Func_02005a04();
extern void Func_02005922();
extern void Func_02005930();
extern void Func_020059e2();
extern void Func_02005940();
extern void Func_020059e8();
extern void Func_0200594e();
extern void Func_02005a32();
extern void Func_02005960();
extern void Func_02005a38();
extern void Func_0200596e();
extern void Func_020059ee();
extern void Func_020059fe();
extern void Func_02005984();
extern void Func_020059fc();
extern void Func_02005a16();
extern void Func_02005a20();
extern void Func_02005aa2();
extern void Func_020059c0();
extern void Func_02005a2a();
extern void Func_02005a84();
extern void Func_02005a86();
extern void Func_02005a90();
extern void Func_02005a76();
extern void Func_02005ad4();
extern void Func_02005a0a();
extern void Func_02005a8a();
extern void Func_02005a9a();
extern void Func_02005af8();
extern void Func_02005a2e();
extern void Func_02005a98();
extern void Func_02005aea();
extern void Func_02005a48();
extern void Func_02005afa();
extern void Func_02005b04();
extern void Func_02005a62();
extern void Func_02005a70();
extern void Func_02005b48();
extern void Func_02005a7e();
extern void Func_02005b72();
extern void Func_02005b7e();
extern void Func_02005a9c();
extern void Func_02005b24();
extern void Func_02005aaa();
extern void Func_02005b5c();
extern void Func_02005b66();
extern void Func_02005ac4();
extern void Func_02005b9c();
extern void Func_02005ad2();
extern void Func_02005b7a();
extern void Func_02005ae0();
extern void Func_02005bb8();
extern void Func_02005aee();
extern void Func_02005be2();
extern void Func_02005bee();
extern void Func_02005b0c();
extern void Func_02005c00();
extern void Func_02005b1e();
extern void Func_02005bf6();
extern void Func_02005b2c();
extern void Func_02005bcc();
extern void Func_02005bd4();
extern void Func_02005c30();
extern void Func_02005c3c();
extern void Func_02005b5a();
extern void Func_02005b68();
extern void Func_02005c40();
extern void Func_02005b76();
extern void Func_02005c28();
extern void Func_02005b86();
extern void Func_02005c06();
extern void Func_02005b94();
extern void Func_02005c14();
void Func_0200187c(void)
{
    u8 *actor;
    s32 delta = -36355;
    s32 i;

    Func_02004f5e(1, 0x102); /* 0x1884 */
    Func_02004f16(1, 2); /* 0x188c */
    Func_02004e7c(0x14); /* 0x1892 */
    Func_02004eb6(0, 0xcccc, 0x6666); /* 0x189c */
    Func_02004ec0(1, 0xcccc, 0x6666); /* 0x18a6 */
    Func_02004eca(0xb, 0xcccc, 0x6666); /* 0x18b0 */
    Func_02004ed4(0xc, 0xcccc, 0x6666); /* 0x18ba */
    Func_02004ede(9, 0xcccc, 0x6666); /* 0x18c4 */
    Func_02004ee8(0xa, 0xcccc, 0x6666); /* 0x18ce */
    Func_02004ef2(8, 0xcccc, 0x6666); /* 0x18d8 */
    Func_02004fd4(0xc00000, -1, 0xa00000, 1); /* 0x18ea */
    Func_02004fe0(); /* 0x18ee */
    Func_02004f8a(1, 0, 0); /* 0x18f8 */
    Func_02004f94(0xc, 0, 0); /* 0x1902 */
    Func_02004f9e(9, 0, 0); /* 0x190c */
    Func_02004fa8(0xa, 0, 0); /* 0x1916 */
    Func_02004fba(0, 0xb, 0); /* 0x1920 */
    Func_02004f10(0xa); /* 0x1926 */
    Func_02004f90(0, 3); /* 0x192e */
    Func_02004fa0(0xb, 3); /* 0x1936 */
    Func_02004f26(0x3c); /* 0x193c */
    Func_02004fa6(8, 3); /* 0x1944 */
    Func_02004fae(0xc, 3); /* 0x194c */
    Func_02004fb6(9, 3); /* 0x1954 */
    Func_02004fc6(0xa, 3); /* 0x195c */
    Func_02004f4c(0x32); /* 0x1962 */
    Func_02004ffe(0, 8, 0); /* 0x196c */
    Func_02005008(0xb, 8, 0); /* 0x1976 */
    Func_02005012(0xc, 8, 0); /* 0x1980 */
    Func_0200501c(9, 8, 0); /* 0x198a */
    Func_02005026(0xa, 8, 0); /* 0x1994 */
    Func_02004f84(0x1e); /* 0x199a */
    Func_0200500c(1, 4); /* 0x19a2 */
    Func_02004f92(0x14); /* 0x19a8 */
    Func_02005058(0x1171); /* 0x19ae */
    Func_02005070(1, 0); /* 0x19b6 */
    Func_02004fa6(0x14); /* 0x19bc */
    Func_02005058(0xc, 1, 0); /* 0x19c6 */
    Func_02005050(0xc, 2); /* 0x19ce */
    Func_020050aa(0xc, 0x103, 0); /* 0x19d8 */
    Func_02004fc8(0x3c); /* 0x19de */
    Func_020050a0(0xc, 0); /* 0x19e6 */
    Func_02004fd6(0x14); /* 0x19ec */
    Func_0200507e(1, 2); /* 0x19f4 */
    Func_02004fe4(0x14); /* 0x19fa */
    Func_02005096(1, 0xc, 0); /* 0x1a04 */
    Func_02004ff4(0x14); /* 0x1a0a */
    Func_0200507c(1, 3); /* 0x1a12 */
    Func_02005002(0x28); /* 0x1a18 */
    Func_020050b4(1, 8, 0); /* 0x1a22 */
    Func_020050be(0xc, 8, 0); /* 0x1a2c */
    Func_0200501c(0x14); /* 0x1a32 */
    Func_020050a4(8, 3); /* 0x1a3a */
    Func_0200502a(0x14); /* 0x1a40 */
    Func_0200510e(8, 0xd000, 0); /* 0x1a4c */
    Func_0200503c(0x1e); /* 0x1a52 */
    Func_020050c4(8, 3); /* 0x1a5a */
    Func_0200504a(0x1e); /* 0x1a60 */
    Func_02005122(8, 0); /* 0x1a68 */
    Func_02005058(0x50); /* 0x1a6e */
    Func_020051a6(0x11); /* 0x1a74 */
    Func_02005186(0x10005, 1); /* 0x1a7c */
    Func_02005194(0x3c); /* 0x1a82 */
    Func_02005072(0x28); /* 0x1a88 */
    Func_02005172(0x6666, 0xccc); /* 0x1a90 */
    Func_0200518c(0xc00000, -1, 0x680000, 1); /* 0x1aa2 */
    Func_02005092(0x78); /* 0x1aa8 */
    Func_020051e0(0x15); /* 0x1aae */
    Func_020051e8(0x134); /* 0x1ab6 */
    Func_0200511e(0xd, 0xc80000, 0x80000); /* 0x1ac4 */
    Func_020050e8(0xd, 0x6666, 0x3333); /* 0x1ace */
    Func_02005112(0xd, 0xc8, 0x48); /* 0x1ad8 */
    Func_02005212(0x120); /* 0x1ae0 */
    Func_020050d0(0x1e); /* 0x1ae6 */
    Func_02005178(8, 2); /* 0x1aee */
    Func_02005158(8, 0); /* 0x1af6 */
    Func_020040c0(); /* 0x1afa */
    Func_02005196(0, 0xd, 0); /* 0x1b04 */
    Func_020051a0(1, 0xd, 0); /* 0x1b0e */
    Func_020051aa(0xb, 0xd, 0); /* 0x1b18 */
    Func_020051b4(0xc, 0xd, 0); /* 0x1b22 */
    Func_020051be(9, 0xd, 0); /* 0x1b2c */
    Func_020051c8(0xa, 0xd, 0); /* 0x1b36 */
    Func_020051c0(0, 2); /* 0x1b3e */
    Func_020051c8(1, 2); /* 0x1b46 */
    Func_020051d0(0xb, 2); /* 0x1b4e */
    Func_020051d8(0xc, 2); /* 0x1b56 */
    Func_020051e0(9, 2); /* 0x1b5e */
    Func_020051e8(0xa, 2); /* 0x1b66 */
    Func_02005156(0x28); /* 0x1b6c */
    Func_0200522e(0xd, 0); /* 0x1b74 */
    Func_02005164(0x14); /* 0x1b7a */
    Func_0200516a(0x28); /* 0x1b80 */
    Func_02005242(0xd, 0); /* 0x1b88 */
    Func_02005178(0x3c); /* 0x1b8e */
    Func_0200523a(0xd, 0x100); /* 0x1b98 */
    Func_020052d0(0x11); /* 0x1b9e */
    Func_020052d8(0x134); /* 0x1ba6 */

    /* The four call sites in this loop update actor 13 by one fixed-point delta. */
    for (i = 0; i < 32; i++) {
        Func_02004d8c(0xd); /* 0x1bd2 */
        Func_020051c2(4); /* 0x1bd8 */
        actor = Func_020051f0(13); /* 0x1bde */
        *(s32 *)(actor + 0x18) += delta;
        actor = Func_020051fc(13); /* 0x1bea */
        *(s32 *)(actor + 0x1c) += delta;
    }

    Func_02005330(0x120); /* 0x1bfe */
    Func_020052a8(0xd, 0); /* 0x1c06 */
    Func_0200526a(0xd, 0, 0); /* 0x1c10 */
    Func_02005200(0x28); /* 0x1c16 */
    Func_02005312(0xc00000, -1, 0xa00000, 1); /* 0x1c28 */
    Func_0200531e(); /* 0x1c2c */
    Func_02005340(0x10000, 0); /* 0x1c36 */
    Func_0200534e(0x3c); /* 0x1c3c */
    Func_0200522c(0x78); /* 0x1c42 */
    Func_020042a8(); /* 0x1c46 */
    Func_020052b0(8, 1); /* 0x1c4e */
    Func_02005386(2); /* 0x1c54 */
    Func_02005244(0x3c); /* 0x1c5a */
    Func_020052ec(8, 2); /* 0x1c62 */
    Func_02005252(0x14); /* 0x1c68 */
    Func_020052f2(0, 1); /* 0x1c70 */
    Func_020052fa(1, 1); /* 0x1c78 */
    Func_02005302(0xb, 1); /* 0x1c80 */
    Func_0200530a(0xc, 1); /* 0x1c88 */
    Func_02005312(9, 1); /* 0x1c90 */
    Func_02005322(0xa, 1); /* 0x1c98 */
    Func_02005288(0x1e); /* 0x1c9e */
    Func_02005360(0xb, 0); /* 0x1ca6 */
    Func_02005296(0x1e); /* 0x1cac */
    Func_02005348(8, 0, 0); /* 0x1cb6 */
    Func_020052a6(0x1e); /* 0x1cbc */
    Func_02005358(0xc, 8, 0); /* 0x1cc6 */
    Func_020052b6(0x14); /* 0x1ccc */
    Func_02005368(0, 8, 0); /* 0x1cd6 */
    Func_02005372(1, 8, 0); /* 0x1ce0 */
    Func_0200537c(0xb, 8, 0); /* 0x1cea */
    Func_02005386(9, 8, 0); /* 0x1cf4 */
    Func_02005390(0xa, 8, 0); /* 0x1cfe */
    Func_020052ee(0x14); /* 0x1d04 */
    Func_020053c6(0xc, 0); /* 0x1d0c */
    Func_020052fc(0x14); /* 0x1d12 */
    Func_020053ae(8, 0xc, 0); /* 0x1d1c */
    Func_0200530c(0x14); /* 0x1d22 */
    Func_02005394(8, 3); /* 0x1d2a */
    Func_0200531a(0x14); /* 0x1d30 */
    Func_0200539a(0, 3); /* 0x1d38 */
    Func_020053aa(1, 3); /* 0x1d40 */
    Func_02005330(0x14); /* 0x1d46 */
    Func_020053e2(8, 0, 0); /* 0x1d50 */
    Func_02005340(0x14); /* 0x1d56 */
    Func_020053c8(8, 3); /* 0x1d5e */
    Func_0200534e(0x1e); /* 0x1d64 */
    Func_02005426(8, 0); /* 0x1d6c */
    Func_0200535c(0x14); /* 0x1d72 */
    Func_02005450(0, 0x102, 0); /* 0x1d7e */
    Func_0200545c(1, 0x102, 0); /* 0x1d8a */
    Func_02005468(0xb, 0x102, 0); /* 0x1d96 */
    Func_02005474(0xc, 0x102, 0); /* 0x1da2 */
    Func_02005480(9, 0x102, 0); /* 0x1dae */
    Func_0200548c(0xa, 0x102, 0); /* 0x1dba */
    Func_020053aa(0x1e); /* 0x1dc0 */
    Func_0200549e(8, 0x102, 0); /* 0x1dcc */
    Func_020053bc(0x1e); /* 0x1dd2 */
    Func_02005494(8, 0); /* 0x1dda */
    Func_020053ca(0x14); /* 0x1de0 */
    Func_0200544a(0, 3); /* 0x1de8 */
    Func_02005452(1, 3); /* 0x1df0 */
    Func_0200545a(0xb, 3); /* 0x1df8 */
    Func_02005462(0xc, 3); /* 0x1e00 */
    Func_0200546a(9, 3); /* 0x1e08 */
    Func_0200547a(0xa, 3); /* 0x1e10 */
    Func_02005400(0x14); /* 0x1e16 */
    Func_02005488(8, 3); /* 0x1e1e */
    Func_0200540e(0xb4); /* 0x1e24 */
    Func_02005500(0xc, 0x105, 0); /* 0x1e2e */
    Func_0200541e(0x3c); /* 0x1e34 */
    Func_020054d8(0, 1, 0); /* 0x1e3e */
    Func_020054e2(9, 0xa, 0); /* 0x1e48 */
    Func_02005438(0x28); /* 0x1e4e */
    Func_020054ea(0, 8, 0); /* 0x1e58 */
    Func_020054f4(1, 8, 0); /* 0x1e62 */
    Func_020054fe(9, 8, 0); /* 0x1e6c */
    Func_02005508(0xa, 8, 0); /* 0x1e76 */
    Func_02005466(0x28); /* 0x1e7c */
    Func_0200550e(1, 2); /* 0x1e84 */
    Func_02005474(0x14); /* 0x1e8a */
    Func_0200554c(1, 0); /* 0x1e92 */
    Func_02005482(0x1e); /* 0x1e98 */
    Func_02005574(8, 0x101, 0); /* 0x1ea2 */
    Func_02005492(0x3c); /* 0x1ea8 */
    Func_02005532(0, 1); /* 0x1eb0 */
    Func_0200553a(1, 1); /* 0x1eb8 */
    Func_02005542(0xb, 1); /* 0x1ec0 */
    Func_0200554a(0xc, 1); /* 0x1ec8 */
    Func_02005552(9, 1); /* 0x1ed0 */
    Func_02005562(0xa, 1); /* 0x1ed8 */
    Func_020054c8(0x1e); /* 0x1ede */
    Func_020055a0(0xb, 0); /* 0x1ee6 */
    Func_020054d6(0x14); /* 0x1eec */
    Func_020055c8(8, 0x101, 0); /* 0x1ef6 */
    Func_020054e6(0x50); /* 0x1efc */
    Func_020055da(0xc, 0x102, 0); /* 0x1f08 */
    Func_020054f8(0x3c); /* 0x1f0e */
    Func_020055d0(0xc, 0); /* 0x1f16 */
    Func_02005506(0x1e); /* 0x1f1c */
    Func_020055f8(8, 0x105, 0); /* 0x1f26 */
    Func_02005516(0x3c); /* 0x1f2c */
    Func_020055be(8, 1); /* 0x1f34 */
    Func_02005612(8, 0x106, 0); /* 0x1f40 */
    Func_02005530(0x14); /* 0x1f46 */
    Func_020055d0(0, 1); /* 0x1f4e */
    Func_020055d8(1, 1); /* 0x1f56 */
    Func_020055e0(0xb, 1); /* 0x1f5e */
    Func_020055e8(0xc, 1); /* 0x1f66 */
    Func_020055f0(9, 1); /* 0x1f6e */
    Func_02005600(0xa, 1); /* 0x1f76 */
    Func_02005566(0x28); /* 0x1f7c */
    Func_0200563e(8, 0); /* 0x1f84 */
    Func_02005574(0x14); /* 0x1f8a */
    Func_020055f4(0, 3); /* 0x1f92 */
    Func_020055fc(1, 3); /* 0x1f9a */
    Func_02005604(0xb, 3); /* 0x1fa2 */
    Func_0200560c(0xc, 3); /* 0x1faa */
    Func_02005614(9, 3); /* 0x1fb2 */
    Func_02005624(0xa, 3); /* 0x1fba */
    Func_020055aa(0x1e); /* 0x1fc0 */
    Func_02005682(8, 0); /* 0x1fc8 */
    Func_020055b8(0x1e); /* 0x1fce */
    Func_02005638(0, 3); /* 0x1fd6 */
    Func_02005640(1, 3); /* 0x1fde */
    Func_02005648(0xb, 3); /* 0x1fe6 */
    Func_02005650(0xc, 3); /* 0x1fee */
    Func_02005658(9, 3); /* 0x1ff6 */
    Func_02005668(0xa, 3); /* 0x1ffe */
    Func_020055ee(0x78); /* 0x2004 */
    Func_02005696(1, 1); /* 0x200c */
    Func_020055fc(0x14); /* 0x2012 */
    Func_020056d4(1, 0); /* 0x201a */
    Func_0200560a(0x1e); /* 0x2020 */
    Func_02005692(8, 3); /* 0x2028 */
    Func_02005618(0x14); /* 0x202e */
    Func_020056f0(8, 0); /* 0x2036 */
    Func_02005626(0x14); /* 0x203c */
    Func_02005718(0, 0x105, 0); /* 0x2046 */
    Func_02005722(1, 0x105, 0); /* 0x2050 */
    Func_0200572c(0xb, 0x105, 0); /* 0x205a */
    Func_02005738(0xc, 0x100, 0); /* 0x2066 */
    Func_02005742(9, 0x105, 0); /* 0x2070 */
    Func_0200574c(0xa, 0x105, 0); /* 0x207a */
    Func_0200566a(0x3c); /* 0x2080 */
    Func_02005712(0xc, 1); /* 0x2088 */
    Func_0200574a(0xc, 0); /* 0x2090 */
    Func_02005680(0x14); /* 0x2096 */
    Func_02005732(8, 0xc, 0); /* 0x20a0 */
    Func_02005690(0x14); /* 0x20a6 */
    Func_02005718(8, 4); /* 0x20ae */
    Func_0200569e(0x14); /* 0x20b4 */
    Func_02005776(8, 0); /* 0x20bc */
    Func_020056ac(0x14); /* 0x20c2 */
    Func_020057a0(0, 0x100, 0); /* 0x20ce */
    Func_020057ac(1, 0x100, 0); /* 0x20da */
    Func_020057b8(0xb, 0x100, 0); /* 0x20e6 */
    Func_020057c4(0xc, 0x100, 0); /* 0x20f2 */
    Func_020057d0(9, 0x100, 0); /* 0x20fe */
    Func_020057dc(0xa, 0x100, 0); /* 0x210a */
    Func_020056fa(0x3c); /* 0x2110 */
    Func_020057ac(8, 0, 0); /* 0x211a */
    Func_0200570a(0x14); /* 0x2120 */
    Func_02005792(8, 3); /* 0x2128 */
    Func_020057ea(8, 0); /* 0x2130 */
    Func_02005720(0x28); /* 0x2136 */
    Func_0200578a(8, 0xa8, 0xb0); /* 0x2140 */
    Func_020057dc(0, 8, 0); /* 0x214a */
    Func_020057e6(1, 8, 0); /* 0x2154 */
    Func_020057f0(0xb, 8, 0); /* 0x215e */
    Func_020057fa(0xc, 8, 0); /* 0x2168 */
    Func_02005804(9, 8, 0); /* 0x2172 */
    Func_0200580e(0xa, 8, 0); /* 0x217c */
    Func_020057d0(8, 0xc8, 0xc8); /* 0x2186 */
    Func_02005822(0, 8, 0); /* 0x2190 */
    Func_0200582c(1, 8, 0); /* 0x219a */
    Func_02005836(0xc, 8, 0); /* 0x21a4 */
    Func_02005870(0xb, 0, 0); /* 0x21ae */
    Func_0200587c(9, 0x8000, 0); /* 0x21ba */
    Func_02005888(0xa, 0x8000, 0); /* 0x21c6 */
    Func_02005814(8, 0xc8, 0x110); /* 0x21d2 */
    Func_020057c2(0x28); /* 0x21d8 */
    Func_020058a6(0xb, 0x3000, 0); /* 0x21e4 */
    Func_020058b2(9, 0x5000, 0); /* 0x21f0 */
    Func_020058be(0xa, 0x5000, 0); /* 0x21fc */
    Func_02005854(8); /* 0x2202 */
    Func_02005866(8, 0, 0); /* 0x220c */
    Func_020057fc(0x3c); /* 0x2212 */
    Func_020058f0(0, 0x102, 0); /* 0x221e */
    Func_02005906(1, 0x102, 0); /* 0x2234 */
    Func_02005912(0xb, 0x102, 0); /* 0x2240 */
    Func_0200591e(0xc, 0x102, 0); /* 0x224c */
    Func_0200592a(9, 0x102, 0); /* 0x2258 */
    Func_02005936(0xa, 0x102, 0); /* 0x2264 */
    Func_02005854(0x3c); /* 0x226a */
    Func_0200585a(0x78); /* 0x2270 */
    Func_0200590c(0, 9, 0); /* 0x227a */
    Func_02005916(1, 0xa, 0); /* 0x2284 */
    Func_02005920(0xb, 9, 0); /* 0x228e */
    Func_0200592a(0xc, 9, 0); /* 0x2298 */
    Func_02005888(0x78); /* 0x229e */
    Func_0200597a(9, 0x105, 0); /* 0x22a8 */
    Func_02005898(0x3c); /* 0x22ae */
    Func_0200594a(9, 0, 0); /* 0x22b8 */
    Func_020058a8(0xa); /* 0x22be */
    Func_02005950(9, 2); /* 0x22c6 */
    Func_020058b6(0xa); /* 0x22cc */
    Func_0200599a(9, 0x5000, 0); /* 0x22d8 */
    Func_020058c8(0x14); /* 0x22de */
    Func_020059bc(9, 0x102, 0); /* 0x22ea */
    Func_020058da(0x50); /* 0x22f0 */
    Func_0200598c(9, 0xa, 0); /* 0x22fa */
    Func_0200598c(9, 1); /* 0x2302 */
    Func_020058f2(0x14); /* 0x2308 */
    Func_020059ca(9, 0); /* 0x2310 */
    Func_02005900(0x14); /* 0x2316 */
    Func_020059e4(0xa, 0x5000, 0); /* 0x2322 */
    Func_02005912(0x14); /* 0x2328 */
    Func_02005a04(0xa, 0x101, 0); /* 0x2332 */
    Func_02005922(0x3c); /* 0x2338 */
    Func_020059ca(9, 2); /* 0x2340 */
    Func_02005930(0x14); /* 0x2346 */
    Func_020059e2(0xa, 0, 0); /* 0x2350 */
    Func_02005940(0x1e); /* 0x2356 */
    Func_020059e8(0xa, 2); /* 0x235e */
    Func_0200594e(0x1e); /* 0x2364 */
    Func_02005a32(0xa, 0x5000, 0); /* 0x2370 */
    Func_02005960(0x14); /* 0x2376 */
    Func_02005a38(0xa, 0); /* 0x237e */
    Func_0200596e(0x14); /* 0x2384 */
    Func_020059ee(9, 3); /* 0x238c */
    Func_020059fe(0xa, 3); /* 0x2394 */
    Func_02005984(0x14); /* 0x239a */
    Func_020059e8(9, 0xc8, 0x110); /* 0x23a6 */
    Func_020059fc(0xa, 0xc8, 0x110); /* 0x23b2 */
    Func_02005a16(9, 0, 0); /* 0x23bc */
    Func_02005a20(0xa, 0, 0); /* 0x23c6 */
    Func_02005aa2(0xc, 0x105, 0); /* 0x23d0 */
    Func_020059c0(0x3c); /* 0x23d6 */
    Func_02005a2a(0xc, 0xc8, 0x88); /* 0x23e0 */
    Func_02005a84(0, 0xc, 0); /* 0x23ea */
    Func_02005a86(1, 0xc, 0); /* 0x23f4 */
    Func_02005a90(0xb, 0xc, 0); /* 0x23fe */
    Func_020059ee(0x1e); /* 0x2404 */
    Func_02005a76(0xc, 4); /* 0x240c */
    Func_020059fc(0x14); /* 0x2412 */
    Func_02005ad4(0xc, 0); /* 0x241a */
    Func_02005a0a(0x1e); /* 0x2420 */
    Func_02005a8a(0, 3); /* 0x2428 */
    Func_02005a9a(1, 3); /* 0x2430 */
    Func_02005a20(0x14); /* 0x2436 */
    Func_02005af8(0xb, 0); /* 0x243e */
    Func_02005a2e(0x14); /* 0x2444 */
    Func_02005a98(0xb, 0xa8, 0xa8); /* 0x244e */
    Func_02005aea(0xb, 0xc, 0); /* 0x2458 */
    Func_02005a48(0x14); /* 0x245e */
    Func_02005afa(0, 0xb, 0); /* 0x2468 */
    Func_02005b04(1, 0xb, 0); /* 0x2472 */
    Func_02005a62(0x14); /* 0x2478 */
    Func_02005aea(0xb, 4); /* 0x2480 */
    Func_02005a70(0x14); /* 0x2486 */
    Func_02005b48(0xb, 0); /* 0x248e */
    Func_02005a7e(0x14); /* 0x2494 */
    Func_02005b72(0, 0x102, 0); /* 0x24a0 */
    Func_02005b7e(1, 0x102, 0); /* 0x24ac */
    Func_02005a9c(0x3c); /* 0x24b2 */
    Func_02005b24(0xc, 4); /* 0x24ba */
    Func_02005aaa(0x14); /* 0x24c0 */
    Func_02005b5c(0, 0xc, 0); /* 0x24ca */
    Func_02005b66(1, 0xc, 0); /* 0x24d4 */
    Func_02005ac4(0x14); /* 0x24da */
    Func_02005b9c(0xc, 0); /* 0x24e2 */
    Func_02005ad2(0x14); /* 0x24e8 */
    Func_02005b7a(0xb, 2); /* 0x24f0 */
    Func_02005ae0(0x14); /* 0x24f6 */
    Func_02005bb8(0xb, 0); /* 0x24fe */
    Func_02005aee(0x14); /* 0x2504 */
    Func_02005be2(0, 0x102, 0); /* 0x2510 */
    Func_02005bee(1, 0x102, 0); /* 0x251c */
    Func_02005b0c(0x3c); /* 0x2522 */
    Func_02005c00(0xc, 0x102, 0); /* 0x252e */
    Func_02005b1e(0x3c); /* 0x2534 */
    Func_02005bf6(0xc, 0); /* 0x253c */
    Func_02005b2c(0x14); /* 0x2542 */
    Func_02005bcc(0, 2); /* 0x254a */
    Func_02005bd4(1, 2); /* 0x2552 */
    Func_02005c30(0, 0x100, 0); /* 0x255e */
    Func_02005c3c(1, 0x100, 0); /* 0x256a */
    Func_02005b5a(0x3c); /* 0x2570 */
    Func_02005be2(0xb, 3); /* 0x2578 */
    Func_02005b68(0x14); /* 0x257e */
    Func_02005c40(0xb, 0); /* 0x2586 */
    Func_02005b76(0x14); /* 0x258c */
    Func_02005c28(0, 0xb, 0); /* 0x2596 */
    Func_02005b86(0x1e); /* 0x259c */
    Func_02005c06(0xb, 3); /* 0x25a4 */
    Func_02005b94(0x1e); /* 0x25aa */
    Func_02005c14(0xc, 3); /* 0x25b2 */
}
