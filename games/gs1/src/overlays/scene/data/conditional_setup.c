#include "types.h"
#include "scene.h"

extern s16 gCell[];
extern u8 gOv[];
extern u8 gWork[];

u8 *ConditionalSceneSetup_GetScriptData(void)
{
    return (u8 *)0x02008cf0;
}

s32 ConditionalSceneSetup_GetInitialState(void)
{
    return 0;
}

u8 *ConditionalSceneSetup_GetMessageData(void)
{
    return (u8 *)0x02008d38;
}

u8 *ConditionalSceneSetup_GetActorData(void)
{
    return (u8 *)0x02008d44;
}

u8 *ConditionalSceneSetup_GetEffectData(void)
{
    return (u8 *)0x02008e94;
}

s32 ConditionalSceneSetup_InitForScene15(void)
{
    if (gCell[225] == 15) {
        SceneData_unk5();
    }
    return 0;
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_2008cb4;

    SceneData_unk14_3();
    SceneData_unk15_3(14, 0);
    SceneData_unk16_3(15, 0);
    SceneData_unk17_3(16, 0);
    SceneData_unk18_3(17, 0);
    SceneData_unk19_3(18, 0);
    SceneData_unk20_3(19, 0);
    SceneData_Place(11, 0x109, 0x1e7);
    SceneData_unk2_3(11, 0xa000, 0);
    SceneData_unk3_3(12, 0x100, 0x1f4);
    SceneData_unk4_3(12, 0xa000, 0);
    SceneData_Run(0x10003, 0x10006);
    SceneData_unk21_3();
    SceneData_unk22_3(60);
    SceneData_unk2_4(0x1000000, -1, 0x2640000, 0);
    SceneData_unk23_3();
    SceneData_unk24_3();
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 32;
    SceneData_unk25_3();
    SceneData_unk3_4(0xcccc, 0x1999);
    SceneData_unk4_4(0x1000000, -1, 0x1f40000, 1);
    SceneData_unk26_2(20);
    SceneData_unk5_3(0x10000, 0x20000, 0x10000);
    SceneData_unk27_2();
    SceneData_unk28_2(145);
    SceneData_unk29_2(30);
    SceneData_unk30_2();
    SceneData_unk31_2(145);
    SceneData_unk32_2();
    SceneData_unk6_2(0x20000, 0x30000, 0x10000);
    SceneData_unk33_2();
    SceneData_unk34_2(145);
    SceneData_unk7_2(-1, -1, 0xe666);
    SceneData_unk35_2();
    SceneData_unk36_2(60);
    SceneData_Do(0x1122);
    SceneData_unk8_2(8, 0x102, 0);
    SceneData_unk37_2(60);
    SceneData_unk38_2(8, 0);
    SceneData_unk9_2(9, 0x5000, 0);
    SceneData_unk39_2(30);
    SceneData_unk40_2(9, 0);
    SceneData_unk41_2(30);
    SceneData_unk42_2(11, 4);
    SceneData_unk43_2(11, 0);
    SceneData_unk10_2(9, 0x3000, 0);
    SceneData_unk44_2(12, 11, 0);
    SceneData_unk45_2(30);
    SceneData_unk46_2(12, 4);
    SceneData_unk47_2(12, 0);
    SceneData_unk48_2(13, 1);
    SceneData_unk49_2(13, 0);
    SceneData_unk50_2(10, 13, 0);
    SceneData_unk51_2(30);
    SceneData_unk52_2(10, 1);
    SceneData_unk53_2(10, 0);
    SceneData_unk54_2(9, 10, 0);
    SceneData_unk55_2(30);
    SceneData_unk56_2(9, 1);
    SceneData_unk57_2(9, 0);
    SceneData_unk58_2(10, 9, 0);
    SceneData_unk59_2(30);
    SceneData_unk60_2(10, 4);
    SceneData_unk61_2(10, 0);
    SceneData_unk62_2(60);
    SceneData_unk11_2(0x20000, 0x30000, 0x10000);
    SceneData_unk63();
    SceneData_unk64(145);
    SceneData_unk65(60);
    SceneData_unk66(8, 9, 0);
    SceneData_unk67(10, 11, 0);
    SceneData_unk68(12, 13, 0);
    SceneData_unk69(8, 2);
    SceneData_unk70(9, 2);
    SceneData_unk71(10, 2);
    SceneData_unk72(11, 2);
    SceneData_unk73(12, 2);
    SceneData_unk74(13, 2);
    SceneData_unk12_2(-1, -1, 0xe666);
    SceneData_unk75();
    SceneData_unk13_2(0, 0x8000, 0x4000);
    SceneData_unk14_2(1, 0x8000, 0x4000);
    SceneData_unk15_2(0, 0x780000, 0x1020000);
    SceneData_unk5_4(0x18000, 0x3000);
    SceneData_unk6_3(0x700000, -1, 0x1400000, 1);
    SceneData_unk76(40);
    SceneData_unk77(0, 2);
    SceneData_unk78(1, 2);
    SceneData_unk16_2(0, 120, 0x140);
    SceneData_unk17_2(1, 104, 0x140);
    SceneData_unk79(0, 1);
    SceneData_unk80(1, 1);
    SceneData_unk81();
    SceneData_unk82(30);
    SceneData_unk18_2(1, 0x3000, 0);
    SceneData_unk19_2(1, 0x100, 0);
    SceneData_unk83(50);
    SceneData_unk20_2(1, 0x18000, 0xc000);
    SceneData_unk84(1, 2);
    SceneData_unk21_2(1, 105, 0x156);
    SceneData_unk85(1, 1);
    SceneData_unk86(1, 2);
    SceneData_unk87(1, 0);
    SceneData_unk88(10);
    SceneData_unk89(0, 1);
    SceneData_unk90(0, 1, 0);
    SceneData_unk91(20);
    SceneData_Check(1, 0);
    if (SceneData_unk2(0, 0) == 0) {
        SceneData_unk92(60);
        SceneData_unk93(0, 3);
        SceneData_unk94(1, 3);
        SceneData_unk95(50);
        SceneData_unk22_2(0, 0xc000, 0);
        SceneData_unk96(1, 2);
        SceneData_unk23_2(1, 0x10000, 0x8000);
        SceneData_unk24_2(1, 103, 0x140);
        SceneData_unk97(1, 1);
    } else {
        SceneData_unk98(60);
        SceneData_unk99(0, 3);
        SceneData_unk100(1, 3);
        SceneData_unk101(50);
        SceneData_unk25_2(1, 0x3000, 0);
        SceneData_unk102(0, 2);
        SceneData_unk26(0, 120, 0x154);
        SceneData_unk103(0, 1);
    }
    SceneData_unk104(12, 0);
    SceneData_unk7_3(1, 0x102);
    SceneData_unk105(1, 2);
    SceneData_unk106(40);
    SceneData_unk27(9, 0xa000, 0);
    SceneData_unk28(11, 0xa000, 0);
    SceneData_unk29(10, 0xa000, 0);
    SceneData_unk30(12, 0xa000, 0);
    SceneData_unk31(13, 0xa000, 0);
    SceneData_unk8_3(0x30000, 0x6000);
    SceneData_unk107(10, 1);
    SceneData_unk108();
    SceneData_unk109(50);
    SceneData_unk110(10, 2);
    SceneData_unk111(10, 0);
    SceneData_unk112(30);
    SceneData_unk113(8, 1);
    SceneData_unk114(8, 0);
    SceneData_unk115(40);
    SceneData_unk116(9, 1);
    SceneData_unk117(9, 0);
    SceneData_unk118(40);
    SceneData_unk32(0, 0x3000, 0);
    SceneData_unk33(1, 0x3000, 0);
    SceneData_unk9_3(0x700000, -1, 0x1400000, 1);
    SceneData_unk119();
    SceneData_unk120(0, 2);
    SceneData_unk121(1, 2);
    SceneData_unk122(1);
    SceneData_unk123(50);
    SceneData_unk124(0, 3);
    SceneData_unk125(1, 3);
    SceneData_unk126(1);
    SceneData_unk127(60);
    SceneData_unk10_3(0x10000, 0x2000);
    SceneData_unk11_3(0xd60000, -1, 0x1d80000, 1);
    SceneData_unk34(0, 0x10000, 0x8000);
    SceneData_unk35(1, 0x10000, 0x8000);
    SceneData_unk12_3(0, 0x2008c00);
    SceneData_unk128(30);
    SceneData_unk13_3(1, 0x2008c64);
    SceneData_unk129(1);
    SceneData_unk130(0, 0, 0);
    SceneData_unk131(1, 0, 0);
    SceneData_unk132();
    SceneData_unk36(9, 0x8000, 0);
    SceneData_unk37(8, 0xcccc, 0x6666);
    SceneData_unk133(8, 2);
    SceneData_unk38(8, 0x109, 0x1c7);
    SceneData_unk39(8, 246, 0x1c7);
    SceneData_unk134(8, 1);
    SceneData_unk135(30);
    SceneData_unk136(9, 1);
    SceneData_unk137(9, 0);
    SceneData_unk40(0, 0x102, 0);
    SceneData_unk138(50);
    SceneData_unk41(1, 0x102, 0);
    SceneData_unk139(50);
    SceneData_unk140(8, 1);
    SceneData_unk141(8, 0);
    SceneData_unk142(40);
    SceneData_unk143(0, 1, 0);
    SceneData_unk144(50);
    SceneData_unk145(0, 0, 0);
    SceneData_unk146(1, 0, 0);
    SceneData_unk147(20);
    SceneData_unk148(0, 4);
    SceneData_unk149(1, 4);
    SceneData_unk150(40);
    SceneData_unk42(10, 0x102, 0);
    SceneData_unk151(50);
    SceneData_unk43(1, 0x3000, 0);
    SceneData_unk152(10);
    SceneData_unk3(10, 0);
    if (SceneData_unk4(0, 0) == 0) {
        SceneData_unk153(40);
        SceneData_unk154(8, 9, 0);
        SceneData_unk155(50);
        SceneData_unk44(8, 0x8000, 0);
        SceneData_unk45(9, 0x8000, 0);
        SceneData_unk156(40);
        SceneData_unk157(9, 1);
        SceneData_unk158(9, 0);
        bump_step(1);
    } else {
        SceneData_unk159(40);
        SceneData_unk160(8, 9, 0);
        SceneData_unk161(50);
        SceneData_unk46(8, 0x8000, 0);
        SceneData_unk47(9, 0x8000, 0);
        SceneData_unk162(40);
        SceneData_unk163(9, 1);
        bump_step(1);
        SceneData_unk164(9, 0);
    }
    SceneData_unk165(30);
    SceneData_unk166(1, 0, 0);
    SceneData_unk167(30);
    SceneData_unk168(0, 2);
    SceneData_unk169(1, 2);
    SceneData_unk170(40);
    SceneData_unk171(0, 4);
    SceneData_unk172(1, 4);
    SceneData_unk173(60);
    SceneData_unk174(8, 1);
    SceneData_unk175(8, 0);
    SceneData_unk176(8, 9, 0);
    SceneData_unk177(30);
    SceneData_unk178(8, 1);
    SceneData_unk179(30);
    SceneData_unk180(8, 3);
    SceneData_unk181(30);
    SceneData_unk182(8, 0);
    SceneData_unk183(20);
    SceneData_unk184(9, 1);
    SceneData_unk48(9, 0xb000, 0);
    SceneData_unk185(30);
    SceneData_unk186(9, 3);
    SceneData_unk187(50);
    SceneData_unk49(8, 0x8000, 0);
    SceneData_unk188(20);
    SceneData_unk189(8, 1);
    SceneData_unk190(8, 0);
    SceneData_unk191(40);
    SceneData_unk192(8, 9, 0);
    SceneData_unk193(40);
    SceneData_unk194(8, 3);
    SceneData_unk195(9, 3);
    SceneData_unk196(30);
    SceneData_unk50(8, 255, 0x1bd);
    SceneData_unk197(40);
    SceneData_unk51(0x2008ea0, 45, 11);
    SceneData_unk198(188);
    SceneData_unk199(30);
    SceneData_unk52(8, 255, 0x186);
    SceneData_unk200(20);
    SceneData_unk53(9, 0xcccc, 0x6666);
    SceneData_unk54(10, 0xcccc, 0x6666);
    SceneData_unk55(9, 255, 0x186);
    SceneData_unk56(10, 255, 0x1cc);
    SceneData_unk57(10, 0x8000, 0);
    SceneData_unk201(40);
    SceneData_unk202(10, 3);
    SceneData_unk203(30);
    SceneData_unk204(0, 1);
    SceneData_unk205(1, 1);
    SceneData_unk206(40);
    SceneData_unk58(10, 255, 0x186);
    base5_2008cb4 = (s32)gOv;
    SceneData_unk207(0, base5_2008cb4);
    SceneData_unk208(40);
    SceneData_unk209(1, base5_2008cb4);
    SceneData_unk210(1);
    SceneData_unk59(11, 0x102, 0);
    SceneData_unk60(12, 0x102, 0);
    SceneData_unk211(40);
    SceneData_unk61(0x20000, 0x30000, 0x10000);
    SceneData_unk212();
    SceneData_unk213(145);
    SceneData_unk214(30);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 64;
    SceneData_unk215();
    SceneData_unk62(-1, -1, 0xe666);
    SceneData_unk216();
    SceneData_unk2_2(0x12f);
    SceneData_unk3_2(0x879);
    SceneData_unk217(1);
    SceneData_unk218();
}
