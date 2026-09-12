#include "types.h"
#include "scene.h"

extern s16 gCell[];
extern u8 Value_00000067;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];

u8 *Story_unk32_4();

u8 *Story_unk33_4();

/* Signed halfword table in RAM; index 225 selects the scene. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; imports are named by the main-image address in the
 * trailing word of the overlay veneer. Old-style declarations are required
 * here, because the arity varies from site to site.
 */

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Story_unk170_2(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Story_unk171_2(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Story_unk172_2(channel, value, zero);
}

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Story_unk173_2(channel, value, zero);
}

static __inline__ void ConfigureUniformSecond(s32 channel, s32 value, s32 zero)
{
    Story_unk174_2(channel, value, zero);
}

static __inline__ void ConfigureUniformThird(s32 channel, s32 value, s32 zero)
{
    Story_unk175_2(channel, value, zero);
}

static __inline__ void ConfigureUniformFourth(s32 channel, s32 value, s32 zero)
{
    Story_unk176_2(channel, value, zero);
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
        Story_unk34_4(gOv3);
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
    Story_unk35_4();
    Story_unk7_2(0x1B83);
    Story_Apply(13, 0);
    Story_unk36_4();
}

void Dialogue_RunActor16Message1b88(void)
{
    Story_unk37_4();
    Story_unk8_2(0x1B88);
    Story_Apply2(16, 0);
    Story_unk38_4();
}

void Scene_RunActorEightTurnDialogue(void)
{
    void Story_unk39_4(void);

    Story_unk40_4();
    Story_unk177_2(8, 0x100, 0x3C);
    Story_unk9_2(0x1B91);
    Story_unk178_2(8, 0, 0xA);
    Story_Apply3(8, 2);
    Story_unk179_2(8, 0, 0xA);
    Story_Apply4(8, 4);
    Story_unk180_2(8, 0, 0xA);
    Story_Apply5(8, 3);
    Story_unk181_2(8, 0, 0xA);
    Story_unk10_2(0x913);
    Story_unk39_4();
}

void Scene_RunScene3aa(void)
{
    extern u8 gWork[];
    void Story_unk41_4();

    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Story_unk42_4();
    Story_unk43_4(10);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Story_unk44_4(188);
    } else {
        Story_unk45_4(158);
    }
    Story_unk41_4(1);
    Story_unk46_4(2);
    Story_unk47_4(10);
    Story_Place(0, 0x8000, 0x4000);
    Story_unk48_4(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Story_unk2_3(0, 0, -16);
    } else {
        Story_unk3_3(0, 3, -16);
    }
    Story_unk49_4(16);
    Story_unk50_4(*(s16 *)(((s32)p5 + 0x16c)));
    Story_unk51_4(1);
    Story_unk52_3(2);
    Story_unk53_3();
}

s32 State_SetWord448To209AndRun(void)
{
    extern u8 *gWork;

    *(s32 *)(gWork + 448) = 0x209;
    if (gCell[224] == (s32)(u32)&Value_00000067) {
        Story_unk54_3();
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

    Story_unk55_3(1);
    Story_unk56_3(2);

    switch (gCell[225]) {
    case 9:
        if (Story_unk21(0x941) != 0) {
            rec = Story_unk32_4(8);
            h = 0x1000;
            *(u16 *)(rec + 6) = h;

            if (Story_unk22(0x914) == 0) {
                Story_unk57_3();
            }
        } else {
            Story_unk58_3(9, 0, 0);
            if (Story_unk23(0x321) != 0) {
                Story_unk182_2(8, x1, z1);
                rec = Story_unk33_4(8);
                h = 0xd000;
                *(u16 *)(rec + 6) = h;
            }
        }
        break;

    case 10:
    case 11:
        if (Story_unk24(0x915) != 0) {
            s32 a5 = 4;
            s32 a6 = 3;
            Story_SetRect(58, 70, 54, 70, a5, a6);
            {
                s32 b5 = 55;
                s32 b6 = 8;
                Story_unk2_5(55, 9, 2, 1, b5, b6);
            }
            Story_unk59_3();
            Story_unk60_3(1);
        }
        break;

    case 20:
        Story_unk61_3(9, 0, 0);
        if (Story_unk25(0x109) == 0) {
            Story_unk62_3();
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

    Story_unk63_3();
    Story_Run(-1, -1, -1, 0);
    Story_unk64_3(1);
    Story_unk2_4(0x10002, 0);
    Story_unk3_4(0x10002, 0);
    Story_unk65_3(1);
    Story_unk66_3(1);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 24;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x201;
    Story_unk4_3(8, 0x3580000, 0x1b80000);
    Story_unk5_3(0, 0x3580000, 0x1e60000);
    Story_unk6_3(1, 0x3500000, 0x1f60000);
    Story_unk7_3(2, 0x3680000, 0x1e60000);
    Story_unk8_3(3, 0x3700000, 0x1f60000);
    Story_unk9_3(10, 0x3480000, 0x2060000);
    Story_unk10_3(11, 0x3780000, 0x2060000);
    Story_unk4_4(0x3600000, -1, 0x1d80000, 0);
    Story_unk67_3();
    Story_unk68_3(1);
    Story_unk69_3();
    Story_unk70_3();
    Story_unk71_3(40);
    Story_unk72_3(8, 1);
    Story_unk73_3(8, 3);
    Story_unk74_3(10);
    Story_unk75_3(2, 1);
    Story_unk76_3(2, 4);
    Story_unk77_3(20);
    Story_unk11_3(2, 0x8000, 20);
    Story_unk78_3(0, 2);
    Story_unk79_3(0, 0, 10);
    Story_unk80_3(0, 3);
    Story_unk81_3(1, 1);
    Story_unk82_3(3, 1);
    Story_unk12_3(1, 0xe000, 0);
    Story_unk13_3(3, 0xa000, 10);
    Story_unk83_3(1, 3);
    Story_unk84_3(3, 3);
    Story_unk85_3(20);
    Story_unk86_3(20);
    Story_unk5_4(8, 0x102);
    Story_unk87_3(8, 2);
    Story_unk88_3(60);
    Story_unk14_3(8, 0x5000, 10);
    Story_unk89_3(8, 3);
    Story_unk15_3(10, 0x100, 0);
    Story_unk90_3(10, 2);
    Story_unk91_3(60);
    Story_unk16_3(10, 0xf000, 20);
    Story_unk92_3(11, 2);
    Story_unk17_3(11, 0x9000, 40);
    Story_unk18_3(10, 0xd000, 0);
    Story_unk19_3(11, 0xb000, 20);
    Story_unk93_3(11, 3);
    Story_unk94_3(10);
    Story_unk95_3(8, 2);
    Story_unk20_3(8, 0x3000, 10);
    Story_unk21_3(8, 0x105, 60);
    Story_unk96_3(11, 2);
    Story_unk97_3(11, 4);
    Story_unk98_3(11, 4);
    Story_unk6_4(0x10000, 0);
    Story_unk99_3(40);
    Story_unk100_3(60);
    Story_unk22_3(8, 0x105, 60);
    Story_Do(0x1b21);
    Story_unk101_3(8, 0, 10);
    Story_unk102_3(10, 2);
    Story_unk103_3(10, 0, 10);
    Story_unk104_3(11, 4);
    Story_unk105_3(11, 0, 10);
    Story_unk23_3(2, 0x6000, 10);
    Story_unk24_3(0x6002, 0, 10);
    Story_unk106_3(11, 2);
    Story_unk107_3(11, 0, 10);
    Story_unk108_3(10, 4);
    Story_unk109_3(10, 0, 10);
    Story_unk25_3(2, 0xc000, 10);
    Story_unk26_3(0x2002, 0, 10);
    Story_unk27_3(8, 0x101, 60);
    Story_unk110_3(8, 0, 10);
    Story_unk111_3(11, 2);
    Story_unk112_3(11, 0, 10);
    Story_unk28_3(2, 0x2000, 10);
    Story_unk113_3(10, 3);
    Story_unk114_3(10, 0, 10);
    Story_unk29_3(2, 0x6000, 10);
    Story_unk115_3(2, 1);
    Story_unk30_3(0x6002, 0, 10);
    Story_unk116_3(10, 3);
    Story_unk31_3(2, 0x8000, 10);
    Story_Check(0x6002, 0);
    Story_unk7_4(8, 0x102);
    Story_unk117_3(60);
    Story_unk118_3(8, 0, 10);
    Story_unk119_3(2, 2);
    Story_unk120_3(10);
    Story_unk32_3(2, 0xc000, 20);
    Story_unk121_3(2, 3);
    Story_unk122_3(20);
    Story_unk123_3(1, 3);
    Story_unk124_3(1, 0, 10);
    Story_unk33_3(8, 0x105, 60);
    Story_unk125_3(3, 4);
    Story_unk126_3(3, 0, 40);
    Story_unk34_3(8, 0x1000, 40);
    Story_unk35_3(8, 0x6666, 0x3333);
    Story_unk36_3(8, 0x37c, 0x1b8);
    Story_unk127_3(40);
    Story_unk37_3(8, 0xd000, 20);
    Story_unk38_3(8, 0x105, 60);
    Story_unk128_3(60);
    Story_unk129_3(40);
    Story_unk39_3(8, 0x358, 0x1b8);
    Story_unk130_3(40);
    Story_unk40_3(8, 0x9000, 20);
    Story_unk41_3(8, 0x105, 60);
    Story_unk42_3(10, 0xf000, 0);
    Story_unk43_3(11, 0x9000, 40);
    Story_unk44_3(10, 0xd000, 0);
    Story_unk45_3(11, 0xb000, 10);
    Story_unk131_3(8, 1);
    Story_unk132_3(8, 0, 20);
    Story_unk133_3(8, 4);
    Story_unk134_3(8, 0, 20);
    Story_unk135_3(10, 2);
    Story_unk136_3(10, 0, 10);
    Story_unk137_3(8, 2);
    Story_unk46_3(8, 0x5000, 10);
    Story_unk47_3(0, 0x4000, 0);
    Story_unk48_3(1, 0x4000, 0);
    Story_unk49_3(2, 0x6000, 0);
    Story_unk50_3(3, 0x6000, 0);
    Story_unk51_3(11, 0x9000, 40);
    Story_unk52_2(10, 0xf000, 20);
    Story_unk138_3(10, 0, 20);
    Story_unk139_3(11, 2);
    Story_unk140_3(20);
    Story_unk53_2(11, 0xb000, 20);
    Story_unk141_3(11, 3);
    Story_unk142_3(11, 0, 10);
    Story_unk54_2(11, 0x9000, 40);
    Story_unk143_3(10, 3);
    Story_unk144_3(11, 3);
    Story_unk55_2(10, 0xd000, 0);
    Story_unk56_2(11, 0xb000, 10);
    Story_unk145_3(10, 2);
    Story_unk146_3(10, 0, 20);
    Story_unk11_2(20);
    Story_unk147_3(8, 2);
    Story_unk148_3(40);
    Story_unk149_3(8, 0, 10);
    Story_unk8_4(8, 0x102);
    Story_unk150_3(60);
    Story_unk151_3(8, 0, 20);
    Story_unk152_3(8, 4);
    Story_unk153_3(20);
    Story_unk154_3(8, 4);
    Story_unk155_3(8, 0, 20);
    Story_unk156_3(1, 2);
    Story_unk57_2(1, 0xe000, 20);
    Story_unk2(1, 0);
    Story_unk58_2(0, 0x6000, 0);
    v5 = 1;
    if (Story_unk3(0, 0) == 1) {
        bump_step(1);
        v5 = 0;
    }
    Story_unk157_3(1, 0, 10);
    if (v5 != 0) {
        bump_step(1);
    }
    Story_unk59_2(2, 0x8000, 10);
    Story_unk9_4(2, 0x102);
    Story_unk158_3(60);
    Story_unk60_2(8, 0x101, 0);
    Story_unk61_2(8, 0x3000, 40);
    Story_unk62_2(8, 0x5000, 20);
    Story_unk63_2(8, 0x3000, 20);
    Story_unk159_3(8, 0, 10);
    Story_unk160_3(2, 1);
    Story_unk64_2(2, 0xc000, 10);
    Story_unk65_2(0, 0xc000, 0);
    Story_unk66_2(1, 0xc000, 10);
    Story_unk67_2(0x2002, 0, 10);
    Story_unk68_2(8, 0x107, 60);
    ((void (*)())Story_unk26)(8, 0, 10);
    Story_unk161_3(8, 4);
    Story_unk4(8, 0);
    Story_unk69_2(2, 0x8000, 0);
    Story_unk70_2(1, 0xe000, 0);
    Story_unk71_2(3, 0xa000, 0);
    if (Story_unk5(0, 0) == 0) {
        Story_unk162_3(20);
        Story_unk163_3(8, 3);
        Story_unk164_3(8, 0, 10);
        bump_step(1);
    } else {
        Story_unk165_3(20);
        Story_unk166_3(8, 4);
        bump_step(1);
        Story_unk167_3(8, 0, 10);
    }
    Story_unk168_3(20);
    Story_unk72_2(2, 0x2000, 0);
    Story_unk73_2(3, 0xa000, 40);
    Story_unk74_2(2, 0xc000, 0);
    Story_unk75_2(3, 0xc000, 10);
    Story_unk169_3(3, 0, 20);
    Story_unk76_2(8, 0x105, 60);
    Story_unk170_3(8, 0, 40);
    Story_unk171_3(8, 2);
    Story_unk172_3(8, 0, 40);
    Story_unk173_3(2, 1);
    Story_unk174_3(20);
    Story_unk77_2(2, 0xcccc, 0x6666);
    Story_unk78_2(2, 0x364, 0x1d8);
    Story_unk79_2(0x2002, 0, 10);
    Story_unk175_3(8, 4);
    Story_unk176_3(8, 0, 10);
    Story_unk177_3(40);
    Story_unk178_3(20);
    Story_unk80_2(8, 0x105, 60);
    Story_unk179_3(8, 0, 10);
    Story_unk180_3(1, 2);
    Story_unk181_3(1, 0, 10);
    Story_unk182_3(8, 1);
    Story_unk81_2(8, 0x5000, 10);
    Story_unk183_3(8, 0, 10);
    Story_unk184_3(8, 3);
    Story_unk82_2(8, 0x3000, 10);
    Story_unk185_3(8, 0, 10);
    Story_unk10_4(0, 0x102);
    Story_unk11_4(1, 0x102);
    Story_unk12_4(2, 0x102);
    Story_unk13_4(3, 0x102);
    Story_unk186_3(40);
    Story_unk83_2(0x2002, 0, 10);
    Story_unk187_3(8, 3);
    Story_unk6(8, 0);
    Story_unk188_3(1, 1);
    Story_unk189_3(2, 1);
    Story_unk190_3(3, 1);
    Story_unk84_2(1, 0xe000, 0);
    Story_unk85_2(2, 0x6000, 0);
    Story_unk86_2(3, 0xa000, 0);
    if (Story_unk7(0, 0) == 0) {
        Story_unk191_2(20);
        Story_unk14_4(8, 0x102);
        Story_unk192_2(40);
        Story_unk193_2(8, 0, 10);
        bump_step(2);
    } else {
        Story_unk194_2(20);
        Story_unk195_2(1, 2);
        bump_step(1);
        Story_unk196_2(1, 0, 10);
        Story_unk15_4(8, 0x102);
        Story_unk197_2(40);
        Story_unk198_2(8, 0, 10);
    }
    Story_unk87_2(3, 0x101, 40);
    Story_unk88_2(3, 0xc000, 10);
    Story_unk199_2(3, 0);
    Story_unk89_2(1, 0xc000, 0);
    Story_unk90_2(2, 0xc000, 20);
    Story_unk200_2(8, 3);
    Story_unk201_2(8, 0, 10);
    Story_unk16_4(0, 0x102);
    Story_unk17_4(1, 0x102);
    Story_unk18_4(2, 0x102);
    Story_unk19_4(3, 0x102);
    Story_unk202_2(40);
    Story_unk203_2(8, 0, 10);
    Story_unk91_2(0, 0xe000, 0);
    Story_unk92_2(1, 0xe000, 0);
    Story_unk93_2(3, 0xa000, 10);
    Story_unk204_2(2, 4);
    Story_unk94_2(0x2002, 0, 10);
    Story_unk205_2(8, 3);
    Story_unk206_2(8, 0, 10);
    Story_unk207_2(8, 1);
    Story_unk208_2(8, 0, 10);
    Story_unk209_2(10);
    Story_unk210_2(8, 4);
    Story_unk211_2(8, 0, 20);
    Story_unk212_2(0, 4);
    Story_unk213_2(1, 4);
    Story_unk214_2(2, 4);
    Story_unk215_2(3, 4);
    Story_unk20_4(8, 0x102);
    Story_unk216_2(60);
    Story_unk217_2(8, 0, 10);
    Story_unk218_2(40);
    Story_unk219_2(8, 4);
    Story_unk220_2(8, 0, 10);
    Story_unk221_2(20);
    Story_unk222_2(2, 2);
    Story_unk95_2(0x2002, 0, 10);
    Story_unk223_2(8, 3);
    Story_unk224_2(8, 0, 10);
    Story_unk96_2(3, 0x101, 60);
    Story_unk225_2(3, 0, 10);
    Story_unk97_2(8, 0x108, 60);
    Story_unk226_2(8, 0, 10);
    Story_unk227_2(3, 3);
    Story_unk228_2(3, 0, 10);
    Story_unk229_2(8, 3);
    Story_unk230_2(8, 0, 10);
    Story_unk98_2(1, 0x101, 40);
    Story_unk231_2(1, 0, 10);
    Story_unk99_2(8, 0x5000, 10);
    Story_unk232_2(8, 3);
    Story_unk233_2(8, 0, 10);
    Story_unk234_2(10, 2);
    Story_unk235_2(10, 4);
    Story_unk236_2(10, 0, 10);
    Story_unk100_2(8, 0x105, 40);
    Story_unk237_2(8, 0, 10);
    Story_unk101_2(8, 0x3000, 10);
    Story_unk238_2(8, 4);
    Story_unk239_2(8, 0, 10);
    Story_unk240_2(8, 1);
    Story_unk102_2(8, 0x5000, 10);
    Story_unk241_2(8, 0, 10);
    Story_unk242_2(2, 1);
    Story_unk103_2(0x2002, 0, 10);
    Story_unk104_2(8, 0x100, 40);
    Story_unk105_2(8, 0x3000, 10);
    Story_unk243_2(8, 0, 10);
    Story_unk244_2(40);
    Story_unk245_2(20);
    Story_unk246_2(1, 1);
    Story_unk247_2(1, 0, 10);
    Story_unk248_2(8, 3);
    Story_unk249_2(8, 0, 10);
    Story_unk250_2(8, 4);
    Story_unk251_2(8, 0, 10);
    Story_unk252_2(0, 3);
    Story_unk253_2(1, 3);
    Story_unk254_2(2, 3);
    Story_unk255_2(3, 3);
    Story_unk256_2(8, 1);
    Story_unk257_2(8, 0, 10);
    Story_unk21_4(0, 0x102);
    Story_unk22_4(1, 0x102);
    Story_unk258_2(60);
    Story_unk259_2(8, 4);
    Story_unk260_2(8, 0, 10);
    Story_unk261_2(2, 1);
    Story_unk262_2(3, 1);
    Story_unk263_2(20);
    Story_unk264_2(8, 1);
    Story_unk265_2(8, 0, 10);
    Story_unk266_2(8, 3);
    Story_unk267_2(8, 0, 10);
    Story_unk106_2(2, 0x105, 60);
    Story_unk107_2(0x2002, 0, 10);
    Story_unk108_2(3, 0xa000, 10);
    Story_unk268_2(3, 0, 10);
    Story_unk269_2(2, 4);
    Story_unk270_2(2, 4);
    Story_unk109_2(2, 0x2000, 10);
    Story_unk271_2(2, 4);
    Story_unk110_2(0x2002, 0, 10);
    Story_unk272_2(8, 4);
    Story_unk273_2(8, 0, 10);
    Story_unk111_2(2, 0xc000, 0);
    Story_unk274_2(3, 2);
    Story_unk112_2(3, 0xc000, 10);
    Story_unk275_2(3, 0, 10);
    Story_unk276_2(8, 4);
    Story_unk277_2(8, 0, 10);
    Story_unk278_2(10, 2);
    Story_unk279_2(10, 0, 10);
    Story_unk280_2(11, 3);
    Story_unk281_2(11, 0, 20);
    Story_unk282_2(8, 3);
    Story_unk283_2(8, 0, 10);
    Story_unk284_2(2, 4);
    Story_unk285_2(2, 4);
    Story_unk113_2(0x2002, 0, 10);
    Story_unk286_2(1, 3);
    ((void (*)())Story_unk27)(1, 0, 10);
    Story_unk287_2(8, 1);
    Story_unk8(8, 0);
    Story_unk114_2(1, 0xe000, 0);
    Story_unk115_2(2, 0x6000, 0);
    Story_unk116_2(3, 0xa000, 0);
    if (Story_unk9(0, 0) == 0) {
        Story_unk288_2(20);
        Story_unk289_2(10);
        Story_unk290_2(1, 3);
        Story_unk291_2(2, 3);
        Story_unk292_2(3, 3);
        Story_unk293_2(10);
        Story_unk294_2(1, 0, 10);
        bump_step(2);
    } else {
        Story_unk295_2(10);
        Story_unk296_2(1, 2);
        bump_step(1);
        Story_unk297_2(1, 0, 10);
        Story_unk298_2(10);
        Story_unk299_2(1, 3);
        Story_unk300_2(2, 3);
        Story_unk301_2(3, 3);
        Story_unk302_2(10);
        Story_unk303_2(1, 0, 10);
    }
    Story_unk304_2(8, 3);
    Story_unk305_2(8, 0, 40);
    Story_unk117_2(8, 0x5000, 10);
    Story_unk306_2(8, 1);
    Story_unk307(10);
    Story_unk308(10, 1);
    Story_unk309(20);
    Story_unk118_2(8, 0x3000, 10);
    Story_unk310(8, 1);
    Story_unk311(10);
    Story_unk312(11, 1);
    Story_unk313(20);
    Story_unk314(8, 3);
    Story_unk315(8, 0, 10);
    Story_unk316(10, 3);
    Story_unk317(11, 3);
    Story_unk119_2(10, 0x10000, 0x8000);
    Story_unk120_2(11, 0x10000, 0x8000);
    Story_unk121_2(10, 0x350, 0x21c);
    Story_unk122_2(11, 0x370, 0x21c);
    Story_unk318(10, 0, 0);
    Story_unk319(11, 0, 0);
    Story_unk320(8, 3);
    Story_unk321(0, 3);
    Story_unk322(1, 3);
    Story_unk323(2, 3);
    Story_unk324(3, 3);
    Story_unk325(20);
    Story_unk123_2(1, 0x10000, 0x8000);
    Story_unk124_2(2, 0x10000, 0x8000);
    Story_unk125_2(3, 0x10000, 0x8000);
    tbl = (s32)gOv7;
    Story_unk326(1, tbl);
    Story_unk10(2, tbl);
    Story_unk11(3, tbl);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 16;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x209;
    Story_unk2_2(0x12f);
    Story_unk3_2(0x912);
    Story_unk327();
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Story_unk183_2(0, 0x6000, 0);
    ConfigureSecond(1, 0xe000, 0);
    ConfigureThird(2, 0x2000, 0);
    ConfigureFourth(3, 0xa000, 0);
    if (handoff != 0) {
        Story_unk12_2(handoff);
    }
}

    ConfigureUniformSecond(1, 0xc000, 0);
    ConfigureUniformThird(2, 0xc000, 0);
    ConfigureUniformFourth(3, 0xc000, 0);
    if (handoff != 0) {
        Story_unk13_2(handoff);
    }
}

void Scene_RunPartySequence(void)
{
    extern u8 gWork[];

    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Story_unk328();
    Story_unk23_4(-1, -1, -1, 0);
    Story_unk329(1);
    Story_unk24_4(0x3600000, -1, 0x2180000, 0);
    Story_unk330();
    Story_unk331(1);
    Story_unk126_2(0, 0x3600000, 0x2760000);
    *(s32 *)(*(u8 **)gWork + 0x1c0) = v5 = 0x100;
    *(s32 *)(*(u8 **)gWork + 0x1c8) = 40;
    Story_unk332();
    Story_unk25_4(0x6666, 0xccc);
    Story_unk26_4(0x3600000, -1, 0x1d80000, 1);
    Story_unk127_2(0, 0xcccc, 0x6666);
    Story_unk128_2(1, 0xcccc, 0x6666);
    Story_unk129_2(2, 0xcccc, 0x6666);
    Story_unk130_2(3, 0xcccc, 0x6666);
    Story_unk131_2(0, 0x360, 0x1f2);
    record = Story_unk12(0);
    if (record != 0) {
        Story_unk333(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Story_unk13(0);
    if (record != 0) {
        Story_unk334(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Story_unk14(0);
    if (record != 0) {
        Story_unk335(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Story_unk132_2(0, 0x358, 0x1e6);
    Story_unk133_2(1, 0x350, 0x1f6);
    Story_unk134_2(2, 0x368, 0x1e6);
    Story_unk135_2(3, 0x370, 0x1f6);
    Story_unk336(0, 1);
    Story_unk337(1, 1);
    Story_unk338(2, 1);
    Story_unk339(10);
    Story_unk340(10);
    Story_unk341(9, v5, 20);
    Story_unk136_2(9, 0x5000, 20);
    Story_unk4_2(0x2588);
    Story_unk137_2(0x2009, 0, 10);
    Story_unk342(8, v5, 20);
    Story_unk138_2(8, 0x3000, 20);
    Story_unk343(8, 0, 20);
    Story_unk139_2(8, 0x107, 60);
    Story_unk344(8, 0, 10);
    Story_unk27_4(0, 0x102);
    Story_unk28_4(1, 0x102);
    Story_unk29_4(2, 0x102);
    Story_unk30_4(3, 0x102);
    Story_unk345(60);
    Story_unk140_2(9, 0x102, 60);
    Story_unk141_2(9, 0x7000, 10);
    Story_unk142_2(0x2009, 0, 10);
    Story_unk143_2(8, 0x1000, 10);
    Story_unk144_2(8, 0x108, 20);
    Story_unk346(8, 0, 40);
    Story_unk347(8, 2);
    Story_unk348(8, 0, 20);
    Story_unk145_2(8, 0x3000, 10);
    Story_unk15(8, 0);
    v5 = 1;
    if (Story_unk16(0, 0) == 0) {
        Story_unk349(10);
        Story_unk350(8, 3);
    } else {
        Story_unk351(10);
        Scene_AdvanceStep(1);
        Story_unk352(8, 4);
        v5 = 0;
    }
    Story_unk353(8, 0, 10);
    if (v5 != 0) {
        Scene_AdvanceStep(1);
    }
    Story_unk354(9, 2);
    Story_unk31_4(9, 0x102);
    Story_unk355(80);
    Story_unk146_2(0x2009, 0, 10);
    Story_unk147_2(8, 0x1000, 10);
    Story_unk148_2(8, 0x107, 40);
    Story_unk356(2, 3);
    Story_unk149_2(0x2002, 0, 20);
    Story_unk150_2(8, 0x105, 60);
    Story_unk151_2(8, 0x3000, 60);
    Story_unk357(8, 0, 10);
    Story_unk358(1, 2);
    Story_unk359(1, 0, 10);
    Story_unk152_2(9, 0x5000, 0);
    Story_unk153_2(2, 0x6000, 10);
    Story_unk360(2, 2);
    Story_unk154_2(0x6002, 0, 10);
    Story_unk155_2(2, 0xc000, 10);
    Story_unk361(2, 2);
    Story_unk156_2(0x2002, 0, 20);
    Story_unk362(8, 3);
    Story_unk363(8, 0, 20);
    Story_unk157_2(2, 0x8000, 20);
    Story_unk158_2(0x6002, 0, 10);
    Story_unk159_2(3, 0xa000, 10);
    Story_unk160_2(3, 0x105, 40);
    Story_unk364(3, 0, 10);
    Story_unk365(2, 2);
    Story_unk366(80);
    Story_unk367(2, 3);
    Story_unk368(20);
    Story_unk161_2(1, 0xe000, 10);
    Story_unk369(1, 4);
    Story_unk17(1, 0);
    if (Story_unk18(0, 0) == 0) {
        Story_unk370(20);
        Scene_AdvanceStep(1);
    } else {
        Story_unk371(20);
        Story_unk372(1, 0, 10);
    }
    Story_unk373(2, 3);
    Story_unk374(3, 3);
    Story_unk375(20);
    Story_unk376(9, 2);
    Story_unk162_2(0x2009, 0, 10);
    Story_unk377(3, 2);
    Story_unk378(3, 0, 10);
    Story_unk163_2(9, 0x3000, 10);
    Story_unk379(9, 3);
    Story_unk164_2(0x2009, 0, 10);
    Story_unk165_2(2, 0x101, 60);
    Story_unk166_2(0x2002, 0, 10);
    Story_unk380(9, 1);
    Story_unk167_2(9, 0x5000, 10);
    Story_unk168_2(0x2009, 0, 10);
    Story_unk381(1, 2);
    Story_unk382(1, 0, 10);
    Story_unk383(9, 4);
    Story_unk169_2(0x2009, 0, 10);
    Story_unk384(3, 1);
    Story_unk385(3, 0, 10);
    Story_unk386(8, 1);
    Story_unk387(20);
    Story_unk388(8, 0, 10);
    Story_unk389(0, 3);
    Story_unk390(1, 3);
    Story_unk391(2, 3);
    Story_unk392(3, 3);
    Story_unk393(20);
    tbl = (s32)gOv7;
    Story_unk394(1, tbl);
    Story_unk19(2, tbl);
    Story_unk20(3, tbl);
    work = *(u8 **)gWork;
    *(s32 *)(((s32)work + 0x1c8)) = 16;
    *(s32 *)(((s32)work + 0x1c0)) = 0x209;
    Story_unk5_2(0x12f);
    Story_unk6_2(0x914);
    Story_unk395();
}
