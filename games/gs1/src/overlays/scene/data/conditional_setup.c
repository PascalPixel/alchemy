#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/data/conditional_setup.h"

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
        SceneData_Check5();
    }
    return 0;
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_2008cb4;

    SceneData_Run14();
    SceneData_Run15(14, 0);
    SceneData_Run16(15, 0);
    SceneData_Run17(16, 0);
    SceneData_Run18(17, 0);
    SceneData_Run19(18, 0);
    SceneData_Run20(19, 0);
    SceneData_Place(11, 0x109, 0x1e7);
    SceneData_Place2(11, 0xa000, 0);
    SceneData_Place3(12, 0x100, 0x1f4);
    SceneData_Place4(12, 0xa000, 0);
    SceneData_Run(0x10003, 0x10006);
    SceneData_Run21();
    SceneData_Run22(60);
    SceneData_Run2(0x1000000, -1, 0x2640000, 0);
    SceneData_Run23();
    SceneData_Run24();
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 32;
    SceneData_Run25();
    SceneData_Run3(0xcccc, 0x1999);
    SceneData_Run4(0x1000000, -1, 0x1f40000, 1);
    SceneData_Run26(20);
    SceneData_Place5(0x10000, 0x20000, 0x10000);
    SceneData_Run27();
    SceneData_Run28(145);
    SceneData_Run29(30);
    SceneData_Run30();
    SceneData_Run31(145);
    SceneData_Run32();
    SceneData_Place6(0x20000, 0x30000, 0x10000);
    SceneData_Run33();
    SceneData_Run34(145);
    SceneData_Place7(-1, -1, 0xe666);
    SceneData_Run35();
    SceneData_Run36(60);
    SceneData_Do(0x1122);
    SceneData_Place8(8, 0x102, 0);
    SceneData_Run37(60);
    SceneData_Run38(8, 0);
    SceneData_Place9(9, 0x5000, 0);
    SceneData_Run39(30);
    SceneData_Run40(9, 0);
    SceneData_Run41(30);
    SceneData_Run42(11, 4);
    SceneData_Run43(11, 0);
    SceneData_Place10(9, 0x3000, 0);
    SceneData_Run44(12, 11, 0);
    SceneData_Run45(30);
    SceneData_Run46(12, 4);
    SceneData_Run47(12, 0);
    SceneData_Run48(13, 1);
    SceneData_Run49(13, 0);
    SceneData_Run50(10, 13, 0);
    SceneData_Run51(30);
    SceneData_Run52(10, 1);
    SceneData_Run53(10, 0);
    SceneData_Run54(9, 10, 0);
    SceneData_Run55(30);
    SceneData_Run56(9, 1);
    SceneData_Run57(9, 0);
    SceneData_Run58(10, 9, 0);
    SceneData_Run59(30);
    SceneData_Run60(10, 4);
    SceneData_Run61(10, 0);
    SceneData_Run62(60);
    SceneData_Place11(0x20000, 0x30000, 0x10000);
    SceneData_Run63();
    SceneData_Run64(145);
    SceneData_Run65(60);
    SceneData_Run66(8, 9, 0);
    SceneData_Run67(10, 11, 0);
    SceneData_Run68(12, 13, 0);
    SceneData_Run69(8, 2);
    SceneData_Run70(9, 2);
    SceneData_Run71(10, 2);
    SceneData_Run72(11, 2);
    SceneData_Run73(12, 2);
    SceneData_Run74(13, 2);
    SceneData_Place12(-1, -1, 0xe666);
    SceneData_Run75();
    SceneData_Place13(0, 0x8000, 0x4000);
    SceneData_Place14(1, 0x8000, 0x4000);
    SceneData_Place15(0, 0x780000, 0x1020000);
    SceneData_Run5(0x18000, 0x3000);
    SceneData_Run6(0x700000, -1, 0x1400000, 1);
    SceneData_Run76(40);
    SceneData_Run77(0, 2);
    SceneData_Run78(1, 2);
    SceneData_Place16(0, 120, 0x140);
    SceneData_Place17(1, 104, 0x140);
    SceneData_Run79(0, 1);
    SceneData_Run80(1, 1);
    SceneData_Run81();
    SceneData_Run82(30);
    SceneData_Place18(1, 0x3000, 0);
    SceneData_Place19(1, 0x100, 0);
    SceneData_Run83(50);
    SceneData_Place20(1, 0x18000, 0xc000);
    SceneData_Run84(1, 2);
    SceneData_Place21(1, 105, 0x156);
    SceneData_Run85(1, 1);
    SceneData_Run86(1, 2);
    SceneData_Run87(1, 0);
    SceneData_Run88(10);
    SceneData_Run89(0, 1);
    SceneData_Run90(0, 1, 0);
    SceneData_Run91(20);
    SceneData_Check(1, 0);
    if (SceneData_Check2(0, 0) == 0) {
        SceneData_Run92(60);
        SceneData_Run93(0, 3);
        SceneData_Run94(1, 3);
        SceneData_Run95(50);
        SceneData_Place22(0, 0xc000, 0);
        SceneData_Run96(1, 2);
        SceneData_Place23(1, 0x10000, 0x8000);
        SceneData_Place24(1, 103, 0x140);
        SceneData_Run97(1, 1);
    } else {
        SceneData_Run98(60);
        SceneData_Run99(0, 3);
        SceneData_Run100(1, 3);
        SceneData_Run101(50);
        SceneData_Place25(1, 0x3000, 0);
        SceneData_Run102(0, 2);
        SceneData_Place26(0, 120, 0x154);
        SceneData_Run103(0, 1);
    }
    SceneData_Run104(12, 0);
    SceneData_Run7(1, 0x102);
    SceneData_Run105(1, 2);
    SceneData_Run106(40);
    SceneData_Place27(9, 0xa000, 0);
    SceneData_Place28(11, 0xa000, 0);
    SceneData_Place29(10, 0xa000, 0);
    SceneData_Place30(12, 0xa000, 0);
    SceneData_Place31(13, 0xa000, 0);
    SceneData_Run8(0x30000, 0x6000);
    SceneData_Run107(10, 1);
    SceneData_Run108();
    SceneData_Run109(50);
    SceneData_Run110(10, 2);
    SceneData_Run111(10, 0);
    SceneData_Run112(30);
    SceneData_Run113(8, 1);
    SceneData_Run114(8, 0);
    SceneData_Run115(40);
    SceneData_Run116(9, 1);
    SceneData_Run117(9, 0);
    SceneData_Run118(40);
    SceneData_Place32(0, 0x3000, 0);
    SceneData_Place33(1, 0x3000, 0);
    SceneData_Run9(0x700000, -1, 0x1400000, 1);
    SceneData_Run119();
    SceneData_Run120(0, 2);
    SceneData_Run121(1, 2);
    SceneData_Run122(1);
    SceneData_Run123(50);
    SceneData_Run124(0, 3);
    SceneData_Run125(1, 3);
    SceneData_Run126(1);
    SceneData_Run127(60);
    SceneData_Run10(0x10000, 0x2000);
    SceneData_Run11(0xd60000, -1, 0x1d80000, 1);
    SceneData_Place34(0, 0x10000, 0x8000);
    SceneData_Place35(1, 0x10000, 0x8000);
    SceneData_Run12(0, 0x2008c00);
    SceneData_Run128(30);
    SceneData_Run13(1, 0x2008c64);
    SceneData_Run129(1);
    SceneData_Run130(0, 0, 0);
    SceneData_Run131(1, 0, 0);
    SceneData_Run132();
    SceneData_Place36(9, 0x8000, 0);
    SceneData_Place37(8, 0xcccc, 0x6666);
    SceneData_Run133(8, 2);
    SceneData_Place38(8, 0x109, 0x1c7);
    SceneData_Place39(8, 246, 0x1c7);
    SceneData_Run134(8, 1);
    SceneData_Run135(30);
    SceneData_Run136(9, 1);
    SceneData_Run137(9, 0);
    SceneData_Place40(0, 0x102, 0);
    SceneData_Run138(50);
    SceneData_Place41(1, 0x102, 0);
    SceneData_Run139(50);
    SceneData_Run140(8, 1);
    SceneData_Run141(8, 0);
    SceneData_Run142(40);
    SceneData_Run143(0, 1, 0);
    SceneData_Run144(50);
    SceneData_Run145(0, 0, 0);
    SceneData_Run146(1, 0, 0);
    SceneData_Run147(20);
    SceneData_Run148(0, 4);
    SceneData_Run149(1, 4);
    SceneData_Run150(40);
    SceneData_Place42(10, 0x102, 0);
    SceneData_Run151(50);
    SceneData_Place43(1, 0x3000, 0);
    SceneData_Run152(10);
    SceneData_Check3(10, 0);
    if (SceneData_Check4(0, 0) == 0) {
        SceneData_Run153(40);
        SceneData_Run154(8, 9, 0);
        SceneData_Run155(50);
        SceneData_Place44(8, 0x8000, 0);
        SceneData_Place45(9, 0x8000, 0);
        SceneData_Run156(40);
        SceneData_Run157(9, 1);
        SceneData_Run158(9, 0);
        bump_step(1);
    } else {
        SceneData_Run159(40);
        SceneData_Run160(8, 9, 0);
        SceneData_Run161(50);
        SceneData_Place46(8, 0x8000, 0);
        SceneData_Place47(9, 0x8000, 0);
        SceneData_Run162(40);
        SceneData_Run163(9, 1);
        bump_step(1);
        SceneData_Run164(9, 0);
    }
    SceneData_Run165(30);
    SceneData_Run166(1, 0, 0);
    SceneData_Run167(30);
    SceneData_Run168(0, 2);
    SceneData_Run169(1, 2);
    SceneData_Run170(40);
    SceneData_Run171(0, 4);
    SceneData_Run172(1, 4);
    SceneData_Run173(60);
    SceneData_Run174(8, 1);
    SceneData_Run175(8, 0);
    SceneData_Run176(8, 9, 0);
    SceneData_Run177(30);
    SceneData_Run178(8, 1);
    SceneData_Run179(30);
    SceneData_Run180(8, 3);
    SceneData_Run181(30);
    SceneData_Run182(8, 0);
    SceneData_Run183(20);
    SceneData_Run184(9, 1);
    SceneData_Place48(9, 0xb000, 0);
    SceneData_Run185(30);
    SceneData_Run186(9, 3);
    SceneData_Run187(50);
    SceneData_Place49(8, 0x8000, 0);
    SceneData_Run188(20);
    SceneData_Run189(8, 1);
    SceneData_Run190(8, 0);
    SceneData_Run191(40);
    SceneData_Run192(8, 9, 0);
    SceneData_Run193(40);
    SceneData_Run194(8, 3);
    SceneData_Run195(9, 3);
    SceneData_Run196(30);
    SceneData_Place50(8, 255, 0x1bd);
    SceneData_Run197(40);
    SceneData_Place51(0x2008ea0, 45, 11);
    SceneData_Run198(188);
    SceneData_Run199(30);
    SceneData_Place52(8, 255, 0x186);
    SceneData_Run200(20);
    SceneData_Place53(9, 0xcccc, 0x6666);
    SceneData_Place54(10, 0xcccc, 0x6666);
    SceneData_Place55(9, 255, 0x186);
    SceneData_Place56(10, 255, 0x1cc);
    SceneData_Place57(10, 0x8000, 0);
    SceneData_Run201(40);
    SceneData_Run202(10, 3);
    SceneData_Run203(30);
    SceneData_Run204(0, 1);
    SceneData_Run205(1, 1);
    SceneData_Run206(40);
    SceneData_Place58(10, 255, 0x186);
    base5_2008cb4 = (s32)gOv;
    SceneData_Run207(0, base5_2008cb4);
    SceneData_Run208(40);
    SceneData_Run209(1, base5_2008cb4);
    SceneData_Run210(1);
    SceneData_Place59(11, 0x102, 0);
    SceneData_Place60(12, 0x102, 0);
    SceneData_Run211(40);
    SceneData_Place61(0x20000, 0x30000, 0x10000);
    SceneData_Run212();
    SceneData_Run213(145);
    SceneData_Run214(30);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 64;
    SceneData_Run215();
    SceneData_Place62(-1, -1, 0xe666);
    SceneData_Run216();
    SceneData_Do2(0x12f);
    SceneData_Do3(0x879);
    SceneData_Run217(1);
    SceneData_Run218();
}
