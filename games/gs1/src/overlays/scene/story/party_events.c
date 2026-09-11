#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/party_events.h"

extern s16 gCell[];
extern u8 Value_00000067;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];

u8 *Story_Run32();

u8 *Story_Run33();

/* Signed halfword table in RAM; index 225 selects the scene. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; imports are named by the main-image address in the
 * trailing word of the overlay veneer. Old-style declarations are required
 * here, because the arity varies from site to site.
 */

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Story_Place170(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Story_Place171(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Story_Place172(channel, value, zero);
}

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Story_Place173(channel, value, zero);
}

static __inline__ void ConfigureUniformSecond(s32 channel, s32 value, s32 zero)
{
    Story_Place174(channel, value, zero);
}

static __inline__ void ConfigureUniformThird(s32 channel, s32 value, s32 zero)
{
    Story_Place175(channel, value, zero);
}

static __inline__ void ConfigureUniformFourth(s32 channel, s32 value, s32 zero)
{
    Story_Place176(channel, value, zero);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 gWork[];

    *(u16 *)(*(u8 **)gWork + 0x1d8) += amount;
}

s32 SceneData_SelectTable9bd4ByState(void)
{
    if (gCell[224] == (s32)&Value_00000067) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x02009d9c, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetTable9d9c(void)
{
    return (u8 *)0x02009d9c;
}

s32 SceneData_SelectTable9ddcByStateWithInit(void)
{
    if (gCell[224] == (s32)&Value_00000067) {
        Story_Run34(gOv3);
        return (s32)gOv3;
    }
    return (s32)gOv4;
}

s32 SceneData_SelectTable9f2cByState(void)
{
    if (gCell[224] == (s32)&Value_00000067) {
        return (s32)gOv5;
    }
    return (s32)gOv6;
}

void Dialogue_RunActor13Message1b83(void)
{
    Story_Run35();
    Story_Do7(0x1B83);
    Story_Apply(13, 0);
    Story_Run36();
}

void Dialogue_RunActor16Message1b88(void)
{
    Story_Run37();
    Story_Do8(0x1B88);
    Story_Apply2(16, 0);
    Story_Run38();
}

void Scene_RunActorEightTurnDialogue(void)
{
    void Story_Run39(void);

    Story_Run40();
    Story_Place177(8, 0x100, 0x3C);
    Story_Do9(0x1B91);
    Story_Place178(8, 0, 0xA);
    Story_Apply3(8, 2);
    Story_Place179(8, 0, 0xA);
    Story_Apply4(8, 4);
    Story_Place180(8, 0, 0xA);
    Story_Apply5(8, 3);
    Story_Place181(8, 0, 0xA);
    Story_Do10(0x913);
    Story_Run39();
}

void Scene_RunScene3aa(void)
{
    extern u8 gWork[];
    void Story_Run41();

    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Story_Run42();
    Story_Run43(10);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Story_Run44(188);
    } else {
        Story_Run45(158);
    }
    Story_Run41(1);
    Story_Run46(2);
    Story_Run47(10);
    Story_Place(0, 0x8000, 0x4000);
    Story_Run48(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Story_Place2(0, 0, -16);
    } else {
        Story_Place3(0, 3, -16);
    }
    Story_Run49(16);
    Story_Run50(*(s16 *)(((s32)p5 + 0x16c)));
    Story_Run51(1);
    Story_Run52(2);
    Story_Run53();
}

s32 State_SetWord448To209AndRun(void)
{
    extern u8 *gWork;

    *(s32 *)(gWork + 448) = 0x209;
    if (gCell[224] == (s32)(u32)&Value_00000067) {
        Story_Run54();
    }
    return 0;
}

/*
 * The overlay's scene dispatcher, switching on the scene selector
 * gCell[225]: 10 and 11 share a body and any other value does
 * nothing. The epilogue pops the return address into r0, so this is void
 * and takes no arguments; the 252-byte owner includes its seven pool words.
 * The locals holding the coordinates, the record's +6 halfword (named by
 * position only) and the fifth and sixth arguments force those to be built.
 */
void Scene_DispatchSceneByIndex(void)
{
    extern u8 *gWork;

    u8 *rec;
    s32 h;
    s32 x1 = 0x038a0000;
    s32 z1 = 0x01a60000;

    Story_Run55(1);
    Story_Run56(2);

    switch (gCell[225]) {
    case 9:
        if (Story_Check21(0x941) != 0) {
            rec = Story_Run32(8);
            h = 0x1000;
            *(u16 *)(rec + 6) = h;

            if (Story_Check22(0x914) == 0) {
                Story_Run57();
            }
        } else {
            Story_Run58(9, 0, 0);
            if (Story_Check23(0x321) != 0) {
                Story_Place182(8, x1, z1);
                rec = Story_Run33(8);
                h = 0xd000;
                *(u16 *)(rec + 6) = h;
            }
        }
        break;

    case 10:
    case 11:
        if (Story_Check24(0x915) != 0) {
            s32 a5 = 4;
            s32 a6 = 3;
            Story_SetRect(58, 70, 54, 70, a5, a6);
            {
                s32 b5 = 55;
                s32 b6 = 8;
                Story_SetRect2(55, 9, 2, 1, b5, b6);
            }
            Story_Run59();
            Story_Run60(1);
        }
        break;

    case 20:
        Story_Run61(9, 0, 0);
        if (Story_Check25(0x109) == 0) {
            Story_Run62();
        }
        break;

    default:
        break;
    }
}

void RunEventScript01(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Story_Run63();
    Story_Run(-1, -1, -1, 0);
    Story_Run64(1);
    Story_Run2(0x10002, 0);
    Story_Run3(0x10002, 0);
    Story_Run65(1);
    Story_Run66(1);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 24;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x201;
    Story_Place4(8, 0x3580000, 0x1b80000);
    Story_Place5(0, 0x3580000, 0x1e60000);
    Story_Place6(1, 0x3500000, 0x1f60000);
    Story_Place7(2, 0x3680000, 0x1e60000);
    Story_Place8(3, 0x3700000, 0x1f60000);
    Story_Place9(10, 0x3480000, 0x2060000);
    Story_Place10(11, 0x3780000, 0x2060000);
    Story_Run4(0x3600000, -1, 0x1d80000, 0);
    Story_Run67();
    Story_Run68(1);
    Story_Run69();
    Story_Run70();
    Story_Run71(40);
    Story_Run72(8, 1);
    Story_Run73(8, 3);
    Story_Run74(10);
    Story_Run75(2, 1);
    Story_Run76(2, 4);
    Story_Run77(20);
    Story_Place11(2, 0x8000, 20);
    Story_Run78(0, 2);
    Story_Run79(0, 0, 10);
    Story_Run80(0, 3);
    Story_Run81(1, 1);
    Story_Run82(3, 1);
    Story_Place12(1, 0xe000, 0);
    Story_Place13(3, 0xa000, 10);
    Story_Run83(1, 3);
    Story_Run84(3, 3);
    Story_Run85(20);
    Story_Run86(20);
    Story_Run5(8, 0x102);
    Story_Run87(8, 2);
    Story_Run88(60);
    Story_Place14(8, 0x5000, 10);
    Story_Run89(8, 3);
    Story_Place15(10, 0x100, 0);
    Story_Run90(10, 2);
    Story_Run91(60);
    Story_Place16(10, 0xf000, 20);
    Story_Run92(11, 2);
    Story_Place17(11, 0x9000, 40);
    Story_Place18(10, 0xd000, 0);
    Story_Place19(11, 0xb000, 20);
    Story_Run93(11, 3);
    Story_Run94(10);
    Story_Run95(8, 2);
    Story_Place20(8, 0x3000, 10);
    Story_Place21(8, 0x105, 60);
    Story_Run96(11, 2);
    Story_Run97(11, 4);
    Story_Run98(11, 4);
    Story_Run6(0x10000, 0);
    Story_Run99(40);
    Story_Run100(60);
    Story_Place22(8, 0x105, 60);
    Story_Do(0x1b21);
    Story_Run101(8, 0, 10);
    Story_Run102(10, 2);
    Story_Run103(10, 0, 10);
    Story_Run104(11, 4);
    Story_Run105(11, 0, 10);
    Story_Place23(2, 0x6000, 10);
    Story_Place24(0x6002, 0, 10);
    Story_Run106(11, 2);
    Story_Run107(11, 0, 10);
    Story_Run108(10, 4);
    Story_Run109(10, 0, 10);
    Story_Place25(2, 0xc000, 10);
    Story_Place26(0x2002, 0, 10);
    Story_Place27(8, 0x101, 60);
    Story_Run110(8, 0, 10);
    Story_Run111(11, 2);
    Story_Run112(11, 0, 10);
    Story_Place28(2, 0x2000, 10);
    Story_Run113(10, 3);
    Story_Run114(10, 0, 10);
    Story_Place29(2, 0x6000, 10);
    Story_Run115(2, 1);
    Story_Place30(0x6002, 0, 10);
    Story_Run116(10, 3);
    Story_Place31(2, 0x8000, 10);
    Story_Check(0x6002, 0);
    Story_Run7(8, 0x102);
    Story_Run117(60);
    Story_Run118(8, 0, 10);
    Story_Run119(2, 2);
    Story_Run120(10);
    Story_Place32(2, 0xc000, 20);
    Story_Run121(2, 3);
    Story_Run122(20);
    Story_Run123(1, 3);
    Story_Run124(1, 0, 10);
    Story_Place33(8, 0x105, 60);
    Story_Run125(3, 4);
    Story_Run126(3, 0, 40);
    Story_Place34(8, 0x1000, 40);
    Story_Place35(8, 0x6666, 0x3333);
    Story_Place36(8, 0x37c, 0x1b8);
    Story_Run127(40);
    Story_Place37(8, 0xd000, 20);
    Story_Place38(8, 0x105, 60);
    Story_Run128(60);
    Story_Run129(40);
    Story_Place39(8, 0x358, 0x1b8);
    Story_Run130(40);
    Story_Place40(8, 0x9000, 20);
    Story_Place41(8, 0x105, 60);
    Story_Place42(10, 0xf000, 0);
    Story_Place43(11, 0x9000, 40);
    Story_Place44(10, 0xd000, 0);
    Story_Place45(11, 0xb000, 10);
    Story_Run131(8, 1);
    Story_Run132(8, 0, 20);
    Story_Run133(8, 4);
    Story_Run134(8, 0, 20);
    Story_Run135(10, 2);
    Story_Run136(10, 0, 10);
    Story_Run137(8, 2);
    Story_Place46(8, 0x5000, 10);
    Story_Place47(0, 0x4000, 0);
    Story_Place48(1, 0x4000, 0);
    Story_Place49(2, 0x6000, 0);
    Story_Place50(3, 0x6000, 0);
    Story_Place51(11, 0x9000, 40);
    Story_Place52(10, 0xf000, 20);
    Story_Run138(10, 0, 20);
    Story_Run139(11, 2);
    Story_Run140(20);
    Story_Place53(11, 0xb000, 20);
    Story_Run141(11, 3);
    Story_Run142(11, 0, 10);
    Story_Place54(11, 0x9000, 40);
    Story_Run143(10, 3);
    Story_Run144(11, 3);
    Story_Place55(10, 0xd000, 0);
    Story_Place56(11, 0xb000, 10);
    Story_Run145(10, 2);
    Story_Run146(10, 0, 20);
    Story_Do11(20);
    Story_Run147(8, 2);
    Story_Run148(40);
    Story_Run149(8, 0, 10);
    Story_Run8(8, 0x102);
    Story_Run150(60);
    Story_Run151(8, 0, 20);
    Story_Run152(8, 4);
    Story_Run153(20);
    Story_Run154(8, 4);
    Story_Run155(8, 0, 20);
    Story_Run156(1, 2);
    Story_Place57(1, 0xe000, 20);
    Story_Check2(1, 0);
    Story_Place58(0, 0x6000, 0);
    v5 = 1;
    if (Story_Check3(0, 0) == 1) {
        bump_step(1);
        v5 = 0;
    }
    Story_Run157(1, 0, 10);
    if (v5 != 0) {
        bump_step(1);
    }
    Story_Place59(2, 0x8000, 10);
    Story_Run9(2, 0x102);
    Story_Run158(60);
    Story_Place60(8, 0x101, 0);
    Story_Place61(8, 0x3000, 40);
    Story_Place62(8, 0x5000, 20);
    Story_Place63(8, 0x3000, 20);
    Story_Run159(8, 0, 10);
    Story_Run160(2, 1);
    Story_Place64(2, 0xc000, 10);
    Story_Place65(0, 0xc000, 0);
    Story_Place66(1, 0xc000, 10);
    Story_Place67(0x2002, 0, 10);
    Story_Place68(8, 0x107, 60);
    ((void (*)())Story_Check26)(8, 0, 10);
    Story_Run161(8, 4);
    Story_Check4(8, 0);
    Story_Place69(2, 0x8000, 0);
    Story_Place70(1, 0xe000, 0);
    Story_Place71(3, 0xa000, 0);
    if (Story_Check5(0, 0) == 0) {
        Story_Run162(20);
        Story_Run163(8, 3);
        Story_Run164(8, 0, 10);
        bump_step(1);
    } else {
        Story_Run165(20);
        Story_Run166(8, 4);
        bump_step(1);
        Story_Run167(8, 0, 10);
    }
    Story_Run168(20);
    Story_Place72(2, 0x2000, 0);
    Story_Place73(3, 0xa000, 40);
    Story_Place74(2, 0xc000, 0);
    Story_Place75(3, 0xc000, 10);
    Story_Run169(3, 0, 20);
    Story_Place76(8, 0x105, 60);
    Story_Run170(8, 0, 40);
    Story_Run171(8, 2);
    Story_Run172(8, 0, 40);
    Story_Run173(2, 1);
    Story_Run174(20);
    Story_Place77(2, 0xcccc, 0x6666);
    Story_Place78(2, 0x364, 0x1d8);
    Story_Place79(0x2002, 0, 10);
    Story_Run175(8, 4);
    Story_Run176(8, 0, 10);
    Story_Run177(40);
    Story_Run178(20);
    Story_Place80(8, 0x105, 60);
    Story_Run179(8, 0, 10);
    Story_Run180(1, 2);
    Story_Run181(1, 0, 10);
    Story_Run182(8, 1);
    Story_Place81(8, 0x5000, 10);
    Story_Run183(8, 0, 10);
    Story_Run184(8, 3);
    Story_Place82(8, 0x3000, 10);
    Story_Run185(8, 0, 10);
    Story_Run10(0, 0x102);
    Story_Run11(1, 0x102);
    Story_Run12(2, 0x102);
    Story_Run13(3, 0x102);
    Story_Run186(40);
    Story_Place83(0x2002, 0, 10);
    Story_Run187(8, 3);
    Story_Check6(8, 0);
    Story_Run188(1, 1);
    Story_Run189(2, 1);
    Story_Run190(3, 1);
    Story_Place84(1, 0xe000, 0);
    Story_Place85(2, 0x6000, 0);
    Story_Place86(3, 0xa000, 0);
    if (Story_Check7(0, 0) == 0) {
        Story_Run191(20);
        Story_Run14(8, 0x102);
        Story_Run192(40);
        Story_Run193(8, 0, 10);
        bump_step(2);
    } else {
        Story_Run194(20);
        Story_Run195(1, 2);
        bump_step(1);
        Story_Run196(1, 0, 10);
        Story_Run15(8, 0x102);
        Story_Run197(40);
        Story_Run198(8, 0, 10);
    }
    Story_Place87(3, 0x101, 40);
    Story_Place88(3, 0xc000, 10);
    Story_Run199(3, 0);
    Story_Place89(1, 0xc000, 0);
    Story_Place90(2, 0xc000, 20);
    Story_Run200(8, 3);
    Story_Run201(8, 0, 10);
    Story_Run16(0, 0x102);
    Story_Run17(1, 0x102);
    Story_Run18(2, 0x102);
    Story_Run19(3, 0x102);
    Story_Run202(40);
    Story_Run203(8, 0, 10);
    Story_Place91(0, 0xe000, 0);
    Story_Place92(1, 0xe000, 0);
    Story_Place93(3, 0xa000, 10);
    Story_Run204(2, 4);
    Story_Place94(0x2002, 0, 10);
    Story_Run205(8, 3);
    Story_Run206(8, 0, 10);
    Story_Run207(8, 1);
    Story_Run208(8, 0, 10);
    Story_Run209(10);
    Story_Run210(8, 4);
    Story_Run211(8, 0, 20);
    Story_Run212(0, 4);
    Story_Run213(1, 4);
    Story_Run214(2, 4);
    Story_Run215(3, 4);
    Story_Run20(8, 0x102);
    Story_Run216(60);
    Story_Run217(8, 0, 10);
    Story_Run218(40);
    Story_Run219(8, 4);
    Story_Run220(8, 0, 10);
    Story_Run221(20);
    Story_Run222(2, 2);
    Story_Place95(0x2002, 0, 10);
    Story_Run223(8, 3);
    Story_Run224(8, 0, 10);
    Story_Place96(3, 0x101, 60);
    Story_Run225(3, 0, 10);
    Story_Place97(8, 0x108, 60);
    Story_Run226(8, 0, 10);
    Story_Run227(3, 3);
    Story_Run228(3, 0, 10);
    Story_Run229(8, 3);
    Story_Run230(8, 0, 10);
    Story_Place98(1, 0x101, 40);
    Story_Run231(1, 0, 10);
    Story_Place99(8, 0x5000, 10);
    Story_Run232(8, 3);
    Story_Run233(8, 0, 10);
    Story_Run234(10, 2);
    Story_Run235(10, 4);
    Story_Run236(10, 0, 10);
    Story_Place100(8, 0x105, 40);
    Story_Run237(8, 0, 10);
    Story_Place101(8, 0x3000, 10);
    Story_Run238(8, 4);
    Story_Run239(8, 0, 10);
    Story_Run240(8, 1);
    Story_Place102(8, 0x5000, 10);
    Story_Run241(8, 0, 10);
    Story_Run242(2, 1);
    Story_Place103(0x2002, 0, 10);
    Story_Place104(8, 0x100, 40);
    Story_Place105(8, 0x3000, 10);
    Story_Run243(8, 0, 10);
    Story_Run244(40);
    Story_Run245(20);
    Story_Run246(1, 1);
    Story_Run247(1, 0, 10);
    Story_Run248(8, 3);
    Story_Run249(8, 0, 10);
    Story_Run250(8, 4);
    Story_Run251(8, 0, 10);
    Story_Run252(0, 3);
    Story_Run253(1, 3);
    Story_Run254(2, 3);
    Story_Run255(3, 3);
    Story_Run256(8, 1);
    Story_Run257(8, 0, 10);
    Story_Run21(0, 0x102);
    Story_Run22(1, 0x102);
    Story_Run258(60);
    Story_Run259(8, 4);
    Story_Run260(8, 0, 10);
    Story_Run261(2, 1);
    Story_Run262(3, 1);
    Story_Run263(20);
    Story_Run264(8, 1);
    Story_Run265(8, 0, 10);
    Story_Run266(8, 3);
    Story_Run267(8, 0, 10);
    Story_Place106(2, 0x105, 60);
    Story_Place107(0x2002, 0, 10);
    Story_Place108(3, 0xa000, 10);
    Story_Run268(3, 0, 10);
    Story_Run269(2, 4);
    Story_Run270(2, 4);
    Story_Place109(2, 0x2000, 10);
    Story_Run271(2, 4);
    Story_Place110(0x2002, 0, 10);
    Story_Run272(8, 4);
    Story_Run273(8, 0, 10);
    Story_Place111(2, 0xc000, 0);
    Story_Run274(3, 2);
    Story_Place112(3, 0xc000, 10);
    Story_Run275(3, 0, 10);
    Story_Run276(8, 4);
    Story_Run277(8, 0, 10);
    Story_Run278(10, 2);
    Story_Run279(10, 0, 10);
    Story_Run280(11, 3);
    Story_Run281(11, 0, 20);
    Story_Run282(8, 3);
    Story_Run283(8, 0, 10);
    Story_Run284(2, 4);
    Story_Run285(2, 4);
    Story_Place113(0x2002, 0, 10);
    Story_Run286(1, 3);
    ((void (*)())Story_Check27)(1, 0, 10);
    Story_Run287(8, 1);
    Story_Check8(8, 0);
    Story_Place114(1, 0xe000, 0);
    Story_Place115(2, 0x6000, 0);
    Story_Place116(3, 0xa000, 0);
    if (Story_Check9(0, 0) == 0) {
        Story_Run288(20);
        Story_Run289(10);
        Story_Run290(1, 3);
        Story_Run291(2, 3);
        Story_Run292(3, 3);
        Story_Run293(10);
        Story_Run294(1, 0, 10);
        bump_step(2);
    } else {
        Story_Run295(10);
        Story_Run296(1, 2);
        bump_step(1);
        Story_Run297(1, 0, 10);
        Story_Run298(10);
        Story_Run299(1, 3);
        Story_Run300(2, 3);
        Story_Run301(3, 3);
        Story_Run302(10);
        Story_Run303(1, 0, 10);
    }
    Story_Run304(8, 3);
    Story_Run305(8, 0, 40);
    Story_Place117(8, 0x5000, 10);
    Story_Run306(8, 1);
    Story_Run307(10);
    Story_Run308(10, 1);
    Story_Run309(20);
    Story_Place118(8, 0x3000, 10);
    Story_Run310(8, 1);
    Story_Run311(10);
    Story_Run312(11, 1);
    Story_Run313(20);
    Story_Run314(8, 3);
    Story_Run315(8, 0, 10);
    Story_Run316(10, 3);
    Story_Run317(11, 3);
    Story_Place119(10, 0x10000, 0x8000);
    Story_Place120(11, 0x10000, 0x8000);
    Story_Place121(10, 0x350, 0x21c);
    Story_Place122(11, 0x370, 0x21c);
    Story_Run318(10, 0, 0);
    Story_Run319(11, 0, 0);
    Story_Run320(8, 3);
    Story_Run321(0, 3);
    Story_Run322(1, 3);
    Story_Run323(2, 3);
    Story_Run324(3, 3);
    Story_Run325(20);
    Story_Place123(1, 0x10000, 0x8000);
    Story_Place124(2, 0x10000, 0x8000);
    Story_Place125(3, 0x10000, 0x8000);
    tbl = (s32)gOv7;
    Story_Run326(1, tbl);
    Story_Check10(2, tbl);
    Story_Check11(3, tbl);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 16;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x209;
    Story_Do2(0x12f);
    Story_Do3(0x912);
    Story_Run327();
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Story_Place183(0, 0x6000, 0);
    ConfigureSecond(1, 0xe000, 0);
    ConfigureThird(2, 0x2000, 0);
    ConfigureFourth(3, 0xa000, 0);
    if (handoff != 0) {
        Story_Do12(handoff);
    }
}

    ConfigureUniformSecond(1, 0xc000, 0);
    ConfigureUniformThird(2, 0xc000, 0);
    ConfigureUniformFourth(3, 0xc000, 0);
    if (handoff != 0) {
        Story_Do13(handoff);
    }
}

void Scene_RunPartySequence(void)
{
    extern u8 gWork[];

    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Story_Run328();
    Story_Run23(-1, -1, -1, 0);
    Story_Run329(1);
    Story_Run24(0x3600000, -1, 0x2180000, 0);
    Story_Run330();
    Story_Run331(1);
    Story_Place126(0, 0x3600000, 0x2760000);
    *(s32 *)(*(u8 **)gWork + 0x1c0) = v5 = 0x100;
    *(s32 *)(*(u8 **)gWork + 0x1c8) = 40;
    Story_Run332();
    Story_Run25(0x6666, 0xccc);
    Story_Run26(0x3600000, -1, 0x1d80000, 1);
    Story_Place127(0, 0xcccc, 0x6666);
    Story_Place128(1, 0xcccc, 0x6666);
    Story_Place129(2, 0xcccc, 0x6666);
    Story_Place130(3, 0xcccc, 0x6666);
    Story_Place131(0, 0x360, 0x1f2);
    record = Story_Check12(0);
    if (record != 0) {
        Story_Run333(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Story_Check13(0);
    if (record != 0) {
        Story_Run334(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Story_Check14(0);
    if (record != 0) {
        Story_Run335(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Story_Place132(0, 0x358, 0x1e6);
    Story_Place133(1, 0x350, 0x1f6);
    Story_Place134(2, 0x368, 0x1e6);
    Story_Place135(3, 0x370, 0x1f6);
    Story_Run336(0, 1);
    Story_Run337(1, 1);
    Story_Run338(2, 1);
    Story_Run339(10);
    Story_Run340(10);
    Story_Run341(9, v5, 20);
    Story_Place136(9, 0x5000, 20);
    Story_Do4(0x2588);
    Story_Place137(0x2009, 0, 10);
    Story_Run342(8, v5, 20);
    Story_Place138(8, 0x3000, 20);
    Story_Run343(8, 0, 20);
    Story_Place139(8, 0x107, 60);
    Story_Run344(8, 0, 10);
    Story_Run27(0, 0x102);
    Story_Run28(1, 0x102);
    Story_Run29(2, 0x102);
    Story_Run30(3, 0x102);
    Story_Run345(60);
    Story_Place140(9, 0x102, 60);
    Story_Place141(9, 0x7000, 10);
    Story_Place142(0x2009, 0, 10);
    Story_Place143(8, 0x1000, 10);
    Story_Place144(8, 0x108, 20);
    Story_Run346(8, 0, 40);
    Story_Run347(8, 2);
    Story_Run348(8, 0, 20);
    Story_Place145(8, 0x3000, 10);
    Story_Check15(8, 0);
    v5 = 1;
    if (Story_Check16(0, 0) == 0) {
        Story_Run349(10);
        Story_Run350(8, 3);
    } else {
        Story_Run351(10);
        Scene_AdvanceStep(1);
        Story_Run352(8, 4);
        v5 = 0;
    }
    Story_Run353(8, 0, 10);
    if (v5 != 0) {
        Scene_AdvanceStep(1);
    }
    Story_Run354(9, 2);
    Story_Run31(9, 0x102);
    Story_Run355(80);
    Story_Place146(0x2009, 0, 10);
    Story_Place147(8, 0x1000, 10);
    Story_Place148(8, 0x107, 40);
    Story_Run356(2, 3);
    Story_Place149(0x2002, 0, 20);
    Story_Place150(8, 0x105, 60);
    Story_Place151(8, 0x3000, 60);
    Story_Run357(8, 0, 10);
    Story_Run358(1, 2);
    Story_Run359(1, 0, 10);
    Story_Place152(9, 0x5000, 0);
    Story_Place153(2, 0x6000, 10);
    Story_Run360(2, 2);
    Story_Place154(0x6002, 0, 10);
    Story_Place155(2, 0xc000, 10);
    Story_Run361(2, 2);
    Story_Place156(0x2002, 0, 20);
    Story_Run362(8, 3);
    Story_Run363(8, 0, 20);
    Story_Place157(2, 0x8000, 20);
    Story_Place158(0x6002, 0, 10);
    Story_Place159(3, 0xa000, 10);
    Story_Place160(3, 0x105, 40);
    Story_Run364(3, 0, 10);
    Story_Run365(2, 2);
    Story_Run366(80);
    Story_Run367(2, 3);
    Story_Run368(20);
    Story_Place161(1, 0xe000, 10);
    Story_Run369(1, 4);
    Story_Check17(1, 0);
    if (Story_Check18(0, 0) == 0) {
        Story_Run370(20);
        Scene_AdvanceStep(1);
    } else {
        Story_Run371(20);
        Story_Run372(1, 0, 10);
    }
    Story_Run373(2, 3);
    Story_Run374(3, 3);
    Story_Run375(20);
    Story_Run376(9, 2);
    Story_Place162(0x2009, 0, 10);
    Story_Run377(3, 2);
    Story_Run378(3, 0, 10);
    Story_Place163(9, 0x3000, 10);
    Story_Run379(9, 3);
    Story_Place164(0x2009, 0, 10);
    Story_Place165(2, 0x101, 60);
    Story_Place166(0x2002, 0, 10);
    Story_Run380(9, 1);
    Story_Place167(9, 0x5000, 10);
    Story_Place168(0x2009, 0, 10);
    Story_Run381(1, 2);
    Story_Run382(1, 0, 10);
    Story_Run383(9, 4);
    Story_Place169(0x2009, 0, 10);
    Story_Run384(3, 1);
    Story_Run385(3, 0, 10);
    Story_Run386(8, 1);
    Story_Run387(20);
    Story_Run388(8, 0, 10);
    Story_Run389(0, 3);
    Story_Run390(1, 3);
    Story_Run391(2, 3);
    Story_Run392(3, 3);
    Story_Run393(20);
    tbl = (s32)gOv7;
    Story_Run394(1, tbl);
    Story_Check19(2, tbl);
    Story_Check20(3, tbl);
    work = *(u8 **)gWork;
    *(s32 *)(((s32)work + 0x1c8)) = 16;
    *(s32 *)(((s32)work + 0x1c0)) = 0x209;
    Story_Do5(0x12f);
    Story_Do6(0x914);
    Story_Run395();
}
