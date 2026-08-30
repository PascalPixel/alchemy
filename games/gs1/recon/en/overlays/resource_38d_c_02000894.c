#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunMultiStageActorSequence:
 * 232 calls, one polling loop, and all explicit actor and workspace effects.
 * Recovered from the complete decoded band and checked against the original assembly. */

#define FieldScene_RunMultiStageActorSequence Func_02000894

void Func_02002596();
void Func_02002ce4();
void Func_02002d02();
void *Func_02002d34();
void Func_02002d50();
s32 Func_02002d5a();
void Func_02002d62();
s32 Func_02002dbc();
void *Func_02002de0();
void Func_02002dfc();
void Func_02002e1c();
void Func_02002e20();
void Func_02002e36();
void Func_02002e42();
void Func_02002e56();
void *Func_02002e68();
void Func_02002e70();
void Func_02002e74();
void Func_02002e7a();
void Func_02002e7e();
void Func_02002e94();
void Func_02002e9c();
void Func_02002eae();
void Func_02002eba();
void Func_02002ec0();
void Func_02002ed2();
void Func_02002ef4();
void Func_02002efa();
void Func_02002f02();
void Func_02002f0a();
void Func_02002f0e();
void Func_02002f18();
void Func_02002f3e();
void Func_02002f44();
void Func_02002f4c();
void Func_02002f4e();
void Func_02002f54();
void Func_02002f5a();
void Func_02002f60();
void Func_02002f66();
void Func_02002f76();
void Func_02002f88();
s32 Func_02002f8e();
void Func_02002f92();
void Func_02002f9a();
void Func_02002fa8();
void *Func_02002fae();
void Func_02002fb2();
void Func_02002fb4();
void *Func_02002fc2();
void Func_02002fca();
void Func_02002fcc();
void *Func_02002fe0();
void Func_02002fe6();
void Func_02002fe8();
void Func_02002ff6();
void Func_02002ff8();
s32 Func_02002ffc();
void Func_02003000();
void Func_02003004();
void Func_0200300a();
void Func_0200300c();
void Func_02003018();
void Func_0200301c();
void Func_02003030();
void Func_02003032();
void Func_0200303c();
void Func_02003046();
void Func_0200304e();
void Func_02003080();
void Func_02003084();
void Func_02003088();
void Func_0200308e();
void Func_02003090();
void Func_020030a2();
void Func_020030b0();
void Func_020030c8();
void Func_020030d0();
void Func_020030d8();
void Func_0200311c();
s32 Func_0200313e();
void Func_0200314a();
void Func_02003156();
void Func_0200315e();
void Func_02003162();
void Func_02003172();
void Func_02003178();
void Func_0200318e();
void Func_020031a2();
void Func_020031ae();
void Func_020031b0();
void Func_020031ce();
void Func_020031da();
void Func_020031dc();
s32 Func_020031e6();
void Func_020031ee();
void Func_020031f2();
void *Func_020031f6();
void Func_020031fa();
void Func_02003206();
void Func_02003214();
void *Func_02003216();
void Func_0200322e();
void Func_02003230();
void Func_0200323c();
void *Func_02003240();
void Func_02003250();
void Func_02003258();
void Func_02003278();
void Func_0200327a();
void Func_0200327e();
void Func_02003280();
void Func_02003292();
void Func_0200329c();
void Func_020032a2();
void Func_020032a8();
void Func_020032ae();
void Func_020032b0();
void Func_020032c2();
void Func_020032cc();
void Func_020032d4();
void Func_020032d6();
s32 Func_020032ea();
void *Func_0200330a();
void Func_02003310();
void *Func_0200331e();
void *Func_0200333c();
void Func_02003354();
s32 Func_02003358();
void Func_0200335c();
void Func_02003366();
void Func_0200336c();
void Func_02003374();
void Func_02003378();
void Func_0200338c();
void Func_020033a2();
void Func_020033aa();
void Func_020033b0();
void Func_020033bc();
void Func_020033c6();
void Func_020033dc();
void Func_020033de();
void Func_020033e0();
void Func_020033e4();
void Func_020033ea();
void Func_020033ec();
void Func_020033fe();
void Func_0200340c();
void Func_0200340e();
void Func_02003424();
void Func_02003428();
void Func_0200342c();
void Func_02003434();
void Func_02003438();
void Func_0200348a();
void *Func_02003496();
void *Func_020034a4();
void Func_020034a6();
void Func_020034b2();
void Func_020034b6();
void Func_020034bc();
void Func_020034be();
void Func_020034c4();
void Func_020034cc();
void Func_020034d0();
void Func_020034d8();
void Func_020034dc();
void Func_020034f0();
void Func_020034fc();
void Func_02003508();
void Func_0200350c();
void Func_02003512();
void Func_02003514();
void Func_0200351c();
void Func_02003534();
void Func_0200353a();
void Func_02003546();
s32 Func_02003568();
void Func_0200358c();
s32 Func_02003592();
void Func_020035a4();
void Func_020035c6();
void Func_020035cc();
void Func_020035d0();
void Func_020035da();
void Func_020035de();
void Func_020035e4();
void Func_020035ec();
void Func_020035fc();
void Func_02003600();
void Func_02003604();
void Func_0200360c();
void Func_02003616();
void Func_02003618();
void Func_02003624();
void Func_0200362a();
void Func_02003630();
void Func_0200363a();
void Func_0200364c();
void Func_02003656();
void Func_02003666();
void Func_0200366e();
void Func_0200368a();
void Func_02003696();
void Func_020036a2();
void Func_020036ae();
void Func_020036c8();
void Func_020036d6();
void Func_020036ec();
void Func_020036f0();
void Func_02003702();
void Func_0200370c();
void Func_0200370e();
void Func_02003710();
void Func_02003722();
void Func_0200372c();
void Func_02003738();
void Func_0200374a();
void Func_0200374c();
void Func_02003750();

void Func_02000894(void)
{
    void *p1;
    void *p18;
    void *p181;
    void *p4;
    void *target;
    u8 *workspace;

    workspace = *(u8 **)0x03001ebc;

    p1 = Func_02002d34();
    Func_02002e56(-1, -1, -1, 0);
    Func_02002ce4(1);
    p4 = Func_02002e68();
    *(u8 *)((u8 *)(p4) + 85) = 0;
    Func_02002e74(58589184, -1, 43515904, 0);
    Func_02002d02(1);
    Func_02002dfc(0, 0, 0);
    if (Func_02002d5a(2143) != 0) {
        Func_02002e9c(58589184, -1, 45744128, 0);
        Func_02002e20(19, 57409536, 41549824);
        Func_02002e94(19, 12288, 0);
        Func_02002e36(0, 58589184, 52297728);
    }
    Func_02002d62();
    Func_02002d50(1);
    *(u32 *)(workspace + 448) = 256;
    *(u32 *)(workspace + 456) = 40;
    Func_02002efa();
    Func_02002f0e();
    if (Func_02002dbc(2143) != 0) {
    } else {
        p18 = Func_02002de0(80);
        Func_02002e7a(19, 58589184, 52297728);
        Func_02002f02(39321, 4915);
        Func_02002f18(58589184, -1, 45744128, 1);
        Func_02002e42(19, 52428, 26214);
        Func_02002e7e(19, 894, 696);
        Func_02002e1c(80);
        Func_02002f44(58589184, -1, 43515904, 1);
        Func_02002eba(19);
        Func_02002eae(19, 842, 696);
        Func_02002eba(19, 842, 636);
        Func_02002f4e(18, 28672, 20);
        Func_02002ed2(19, 876, 634);
        Func_02002f0a(19, 3);
        Func_02002e70(20);
        Func_02002f18(18, 3);
        Func_02002e7e(10);
        Func_02002f54(5175);
        Func_02002f76(8210, 0, 10);
        Func_02002f4e(19, 2);
        Func_02002f88(19, 0, 20);
        Func_02002f60(18, 1);
        Func_02002f9a(8210, 0, 10);
        Func_02002f5a(19, 3);
        Func_02002ec0(40);
        Func_02002fca(18, 261, 60);
        Func_02002fb2(8210, 0);
        Func_02002f92(18, 1);
        Func_02002fcc(8210, 0, 10);
        Func_02002ff6(19, 258);
        Func_02002ef4(60);
        Func_02002ff8(19, 12288, 10);
        Func_02003004(18, 20480, 10);
        Func_02003032(58589184, -1, 45744128, 1);
        Func_02002fb4(0, 58589184, 52297728);
        Func_02002f66(0, 39321, 19660);
        Func_02002fa8(0, 894, 726);
        Func_02002f3e(20);
        Func_02002fe6(0, 3);
        Func_02002f4c(10);
        Func_0200300c(18, 1);
        Func_0200303c(8210, 0);
        Func_02003084(58589184, -1, 43515904, 1);
        Func_02002fe8(0, 894, 684);
        target = Func_02002fae(0);
        if (target != 0) {
            Func_0200301c(1, *(u32 *)((u8 *)target + 8), *(u32 *)((u8 *)target + 16));
        }
        target = Func_02002fc2(0);
        if (target != 0) {
            Func_02003030(2, *(u32 *)((u8 *)target + 8), *(u32 *)((u8 *)target + 16));
        }
        if (Func_02002f8e(3) != 0) {
            target = Func_02002fe0(0);
            if (target != 0) {
                Func_0200304e(3, *(u32 *)((u8 *)target + 8), *(u32 *)((u8 *)target + 16));
            }
        }
        Func_02003000(1, 39321, 19660);
        Func_0200300a(2, 39321, 19660);
        Func_02003018(3, 65536, 32768);
        Func_02003080(1, 2);
        Func_02003088(2, 2);
        Func_02003090(3, 2);
        Func_02003084(1, -16, 16);
        Func_0200308e(2, 16, 16);
        if (Func_02002ffc(3) != 0) {
            Func_020030a2(3, 32, 16);
        }
        Func_020030b0(2);
        Func_020030c8(1, 1);
        Func_020030d0(2, 1);
        Func_020030d8(3, 1);
        Func_02003046(10);
        Func_0200314a(3, 49152, 0);
        Func_02003156(1, 49152, 0);
        Func_02003162(2, 49152, 20);
        Func_0200311c(18, 2, 20);
        Func_02003178(18, 28672, 10);
        Func_02003172(8210, 0, 10);
        Func_0200318e(19, 4096, 10);
        Func_0200313e(19, 3);
        Func_020031a2(18, 20480, 40);
        Func_020031ae(18, 28672, 10);
        Func_0200315e(18, 4);
        Func_020031b0(8210, 0, 10);
        Func_020031da(19, 258);
        Func_020030d8(40);
        Func_020031dc(18, 20480, 20);
        Func_020031ee(18, 261, 40);
        Func_020031ce(8210, 0);
        Func_020031fa(1, 57344, 0);
        Func_02003206(2, 40960, 0);
        if (Func_0200313e(0, 0) == 0) {
        } else {
            Func_020031f2(5184);
            Func_02003214(8210, 0, 10);
            Func_0200327e(19, 12288, 0);
            Func_0200322e(18, 4);
            Func_02003280(8210, 0, 10);
            Func_0200329c(1, 49152, 0);
            Func_020032a8(2, 49152, 0);
            Func_02003258(0, 3);
            Func_02003258(1, 2);
            target = Func_020031f6(0);
            if (target != 0) {
                Func_02003230(1, *(s16 *)((u8 *)target + 10), *(s16 *)((u8 *)target + 18));
            }
            Func_02003278(2, 2);
            target = Func_02003216(0);
            if (target != 0) {
                Func_02003250(2, *(s16 *)((u8 *)target + 10), *(s16 *)((u8 *)target + 18));
            }
            if (Func_020031e6(3) != 0) {
                Func_020032a2(3, 2);
                target = Func_02003240(0);
                if (target != 0) {
                    Func_0200327a(3, *(s16 *)((u8 *)target + 10), *(s16 *)((u8 *)target + 18));
                }
            }
            Func_020032b0(2);
            Func_020032c2(1, 0, 0);
            Func_020032cc(2, 0, 0);
            Func_020032d6(3, 0, 0);
            Func_0200323c(2143);
            Func_02003292(0, 65536, 32768);
            Func_020032d6(0, 894, 752);
            *(u32 *)(workspace + 456) = 16;
            Func_020033b0();
            Func_020033bc();
            Func_02002596();
        }
        Func_020032d4(0);
        Func_02003310(0);
        Func_020032ae(80);
        Func_020033c6();
        Func_020033de();
        Func_02003354(0);
        Func_0200336c(0, 1);
        target = Func_0200330a(0);
        if (target != 0) {
            Func_02003378(1, *(u32 *)((u8 *)target + 8), *(u32 *)((u8 *)target + 16));
        }
        target = Func_0200331e(0);
        if (target != 0) {
            Func_0200338c(2, *(u32 *)((u8 *)target + 8), *(u32 *)((u8 *)target + 16));
        }
        if (Func_020032ea(3) != 0) {
            target = Func_0200333c(0);
            if (target != 0) {
                Func_020033aa(3, *(u32 *)((u8 *)target + 8), *(u32 *)((u8 *)target + 16));
            }
        }
        Func_0200335c(1);
        Func_02003366(2);
        Func_02003374(3, 65536, 32768);
        Func_020033dc(1, 2);
        Func_020033e4(2, 2);
        Func_020033ec(3, 2);
        Func_020033e0(1, -16, 16);
        Func_020033ea(2, 16, 16);
        if (Func_02003358(3) != 0) {
            Func_020033fe(3, 32, 16);
        }
        Func_0200340c(2);
        Func_02003424(1, 1);
        Func_0200342c(2, 1);
        Func_02003434(3, 1);
        Func_020033a2(10);
        Func_020034a6(3, 49152, 0);
        Func_020034b2(1, 49152, 0);
        Func_020034be(2, 49152, 20);
        Func_020034d0(18);
        Func_020034a6();
        Func_020034b6();
        Func_0200340e(0, 0);
    }
    Func_020034f0(3, 49152, 0);
    Func_020034fc(0, 49152, 0);
    Func_02003508(1, 49152, 0);
    Func_02003514(2, 49152, 20);
    Func_020034bc(3, 3);
    Func_020034c4(0, 3);
    Func_020034cc(1, 3);
    Func_020034dc(2, 3);
    Func_02003546(18);
    Func_0200351c();
    Func_02003534();
    Func_0200348a(20);
    Func_02003438();
    p181 = Func_02003496(20);
    *(s32 *)(p181 + 24) = 32768;
    *(s32 *)(p181 + 28) = 32768;
    target = Func_020034a4(18);
    if (target != 0) {
        Func_02003512(20, *(u32 *)((u8 *)target + 8), *(u32 *)((u8 *)target + 16));
    }
    Func_02003428(1);
    Func_0200353a(20, 6, 0);
    Func_020034d8(20, 131072, 65536);
    Func_0200350c(20);
    Func_020034b2();
    Func_020035a4();
    Func_020035c6(3);
    Func_020035d0(0);
    Func_020035da(1);
    Func_020035e4(2);
    Func_0200358c(18, 4);
    Func_020035de();
    Func_02003600(1);
    Func_02003604(1, 57344, 10);
    Func_020035ec();
    Func_02003618(3, 40960, 0);
    Func_02003624(2, 40960, 0);
    Func_02003630(0, 24576, 0);
    if (Func_02003568(0, 0) != 1) {
        do {
            Func_020035fc(1, 2);
            Func_0200360c(2, 2);
            Func_0200362a();
            Func_0200363a();
        } while (Func_02003592(0, 0) != 1);
    }
    Func_02003616(1, 3);
    Func_0200364c();
    Func_0200366e();
    Func_0200368a(3, 49152, 0);
    Func_02003696(0, 49152, 0);
    Func_020036a2(1, 49152, 0);
    Func_020036ae(2, 49152, 10);
    Func_02003656(0, 3);
    Func_02003666(0, 3);
    Func_020035cc(20);
    Func_020036d6(18);
    Func_020036c8();
    Func_020036ec(2, 258, 60);
    Func_020036f0(18, 12288, 10);
    Func_02003702(18);
    Func_0200370c(1);
    Func_0200370e(1, 0, 20);
    Func_020036d6(1, 1);
    Func_02003710();
    Func_0200372c(3, 32768, 0);
    Func_02003738(0, 8192, 20);
    Func_0200374c(2, 258, 60);
    Func_02003750(2, 32768, 10);
    Func_0200374a();
    Func_02003722(1, 1);
}
