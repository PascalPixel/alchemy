#include "types.h"
#include "scene.h"

struct EventActor {
    u8 reserved_00[0x23];
    u8 flags;
    u8 reserved_24[0x2c];
    u8 *render_state;
};

struct Slot020008e0 {
    u8 head[6];
    u16 heading;
};

struct Actor02001060 {
    u8 head[12];
    s32 rank;
    u8 body[19];
    u8 flags;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern s16 gCell[];
extern u8 Value_0000004a;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];

void *Actor_unk8_4(s32);

u8 *Actor_unk9_4();

u8 *Actor_unk10_4();

u8 *Actor_unk11_4();

u8 *Actor_unk12_4();

struct Actor02001060 *Actor_unk13_4(s32);
struct Actor *Actor_unk14_4(s32);
struct Actor *Actor_unk15_4(s32);

u8 *Actor_unk16_4();
u8 *Actor_unk17_4();
u8 *Actor_unk18_4();
u8 *Actor_unk19_4();

u8 *Actor_unk20_4();

/* Scene event steps and table getters for resource_3a2. */

/*
 * Table getter at 0x020000b0. The eight-byte owner includes its one pool word
 * at 0x020000b4, which holds 0x020098ec; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* Value-returning: the reference sets r1 before r0 at this site. */

/*
 * Prepare actor 14 at 0x020010b8: clear bit 1 of the bytes at +35 and +89,
 * clear the byte at +85, and install the callback at 0x02009061 in the record
 * at +0x6c -- that pool word is odd, so it is a Thumb entry and not data. The
 * zero stored at +85 is held in a local because a register carries it. The
 * callback drives the same bit-1 flag the clears here touch.
 */

/* Two sites reach this one symbol with different arities; old-style so both
 * calls are legal. */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Actor_unk21_4(s32, s32, s32);

    Actor_unk21_4(actor, x, y);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 EventScript_PrepareActorRenderFlags(struct EventActor *actor)
{
    actor->flags &= ~1;
    actor->render_state[9] |= 0xc;
    actor->render_state[21] |= 0xc;
    return 0;
}

s32 OvObj_SetFacingTowardObject10(void *self)
{
    void *obj;

    obj = Actor_unk8_4(0xA);
    FIELD_AT_OFFSET(self, s16 *, 6) = Actor_Apply(FIELD_AT_OFFSET(obj, s32 *, 0x10) - FIELD_AT_OFFSET(self, s32 *, 0x10), FIELD_AT_OFFSET(obj, s32 *, 8) - FIELD_AT_OFFSET(self, s32 *, 8));
    return 0;
}

s32 SceneData_SelectTable97b4ByState(void)
{
    if (gCell[224] == (s32)&Value_0000004a) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable98ec(void)
{
    return (u8 *)0x020098ec;
}

s32 SceneData_SelectTable9918ByState(void)
{
    if (gCell[224] == (s32)&Value_0000004a) {
        return (s32)gOv3;
    }
    return (s32)gOv4;
}

void Dialogue_RunMessage1958Step(void)
{
    extern u8 *gWork;

    u8 *work;

    Actor_unk22_4();
    Actor_unk9_2(0x1958);
    Actor_Apply2(10, 0);

    if (Actor_Apply3(0, 0) == 1) {
        Actor_unk10_2(20);
        Actor_Apply4(10, 0);
    } else {
        work = gWork;
        *(u16 *)(work + 472) += 1;
        Actor_Apply5(10, 0);
    }

    Actor_unk23_4();
}

void Dialogue_RunActor11Message195d(void)
{
    Actor_unk24_4();
    Actor_unk11_2(0x195D);
    Actor_Apply6(11, 0);
    Actor_unk25_4();
}

void Dialogue_RunActor13Message1961(void)
{
    Actor_unk26_4();
    Actor_unk12_2(0x1961);
    Actor_Apply7(13, 0);
    Actor_unk27_4();
}

void Scene_RunPrimaryScript(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_unk28_4(188);
    Actor_Place(0x2009788, 67, 6);
    *(u8 *)(Actor_unk9(0) + 85) = 0;
    Actor_unk2_3(0, 0xcccc, 0x6666);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Actor_unk29_4(0, 2);
    Actor_unk3_3(0, 0, -16);
    Actor_unk30_4(16);
    Actor_unk31_4(2);
}

void Scene_RunScene3a2SequenceA(void)
{
    extern u8 gWork[];
    void Actor_unk32_4();

    u32 i;
    s32 record;

    Actor_unk33_4();
    Actor_unk4_3(8, 0x880000, 0xa80000);
    Actor_unk5_3(8, 0x5000, 0);
    Actor_unk6_3(0, 0x900000, 0xc80000);
    Actor_unk7_3(1, 0xa00000, 0xc00000);
    Actor_unk8_3(2, 0x800000, 0xc80000);
    Actor_unk9_3(3, 0x700000, 0xc00000);
    Actor_unk10_3(0, 0xc000, 0);
    Actor_unk11_3(1, 0xa000, 0);
    Actor_unk12_3(2, 0xc000, 0);
    Actor_unk13_3(3, 0xe000, 0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x201;
    Actor_unk34_4();
    Actor_unk35_4();
    Actor_unk36_4(60);
    Actor_unk37_4(8, 3);
    Actor_unk38_4(20);
    Actor_Do(0x19e9);
    Actor_unk39_4(8, 0, 20);
    Actor_unk40_4(0, 3);
    Actor_unk41_4(1, 3);
    Actor_unk42_4(2, 3);
    Actor_unk43_4(3, 3);
    Actor_unk44_4(20);
    Actor_unk45_4(8, 4);
    Actor_unk46_4(20);
    Actor_unk47_4(8, 0, 20);
    Actor_unk48_4(3, 4);
    Actor_unk49_4(20);
    Actor_unk50_4(3, 0, 20);
    Actor_unk51_4(2, 2);
    Actor_unk52_4(20);
    Actor_unk53_4(2, 0, 20);
    Actor_unk54_4(8, 2);
    Actor_unk14_3(8, 0x5000, 20);
    Actor_unk55_4(8, 0, 20);
    Actor_unk15_3(2, 0x102, 60);
    Actor_unk56_4(120);
    Actor_unk57_4(1, 2);
    Actor_unk58_4(20);
    Actor_unk16_3(1, 0x6000, 20);
    Actor_Check(1, 0);
    if (Actor_unk2(0, 0) == 0) {
        Actor_unk59_4(20);
        Actor_unk60_4(3, 0, 20);
        Actor_unk17_3(3, 0x101, 60);
        Actor_unk61_4(3, 0, 20);
        Actor_unk18_3(0, 0xe000, 0);
        Actor_unk19_3(2, 0xe000, 0);
        Actor_unk62_4(1, 2);
        Actor_Run(1, 0x102);
        Actor_unk63_4(60);
        Actor_unk20_3(1, 0x8000, 20);
        Actor_unk64_4(1, 0, 20);
        bump_step(2);
    } else {
        bump_step(2);
        Actor_unk65_4(20);
        Actor_unk66_4(3, 0, 20);
        Actor_unk67_4(3, 3);
        Actor_unk68_4(20);
        Actor_unk69_4(3, 0, 20);
        Actor_unk21_3(1, 0x102, 60);
        Actor_unk22_3(0, 0xe000, 0);
        Actor_unk23_3(2, 0xe000, 0);
        Actor_unk70_4(1, 0, 20);
    }
    Actor_unk71_4(8, 2);
    Actor_unk72_4(20);
    Actor_unk24_3(0, 0xc000, 0);
    Actor_unk25_3(1, 0xa000, 0);
    Actor_unk26_3(2, 0xc000, 0);
    Actor_unk27_3(3, 0xe000, 0);
    Actor_unk73_4(20);
    Actor_unk28_3(8, 0x3000, 20);
    Actor_unk74_4(8, 0, 20);
    Actor_unk75_4(1, 2);
    Actor_unk76_4(20);
    Actor_unk77_4(1, 3);
    Actor_unk78_4(30);
    Actor_unk79_4(8, 3);
    Actor_unk80_4(20);
    Actor_unk81_4(8, 0, 20);
    Actor_unk82_4(8, 2);
    Actor_unk83_4(20);
    Actor_unk84_4(8, 0, 20);
    Actor_unk85_4(0, 3);
    Actor_unk86_4(1, 3);
    Actor_unk87_4(2, 3);
    Actor_unk88_4(3, 3);
    Actor_unk89_4(20);
    Actor_unk90_4(8, 3);
    Actor_unk91_4(20);
    Actor_unk29_3(8, 0xc000, 30);
    Actor_unk92_4(188);
    Actor_unk30_3(0x2009788, 67, 6);
    Actor_unk31_3(8, 0xcccc, 0x6666);
    Actor_unk93_4(8, 136, 136);
    Actor_unk94_4(8, 0, 0);
    Actor_unk95_4(188);
    Actor_unk32_3(0x200979e, 67, 6);
    Actor_unk96_4(60);
    Actor_unk97_4();
    Actor_unk98_4(1, 2);
    Actor_unk99_4(20);
    Actor_unk33_3(1, 0x6000, 20);
    Actor_unk100_4(1, 0, 20);
    Actor_unk34_3(0, 0x8000, 20);
    Actor_unk101_4(2, 2);
    Actor_unk102_4(20);
    Actor_unk35_3(3, 0x2000, 20);
    Actor_unk103_4(3, 0, 20);
    Actor_unk104_4(1, 1);
    Actor_unk36_3(0, 0xe000, 20);
    Actor_unk105_4(1, 0);
    Actor_unk106_4(20);
    Actor_unk107_4(2, 2);
    Actor_unk108_4(20);
    Actor_unk37_3(0, 0x8000, 20);
    Actor_unk109_4(2, 0, 20);
    Actor_unk110_4(0, 1);
    Actor_unk111_4(1, 1);
    Actor_unk112_4(3, 1);
    Actor_unk113_4(20);
    Actor_unk114_4(2, 4);
    Actor_unk115_4(20);
    Actor_unk116_4(2, 0, 20);
    Actor_unk117_4(3, 2);
    Actor_unk118_4(20);
    Actor_unk119_4(3, 0, 20);
    Actor_unk38_3(2, 0xb000, 20);
    Actor_unk120_4(2, 3);
    Actor_unk121_4(20);
    Actor_unk122_4(2, 3);
    Actor_unk123_4(20);
    Actor_unk124_4(2, 0, 20);
    Actor_unk39_3(1, 0x102, 60);
    Actor_unk125_4(1, 0, 20);
    Actor_unk40_3(2, 0xe000, 20);
    Actor_unk126_4(2, 4);
    Actor_unk127_4(20);
    Actor_unk128_4(2, 0, 20);
    Actor_unk41_3(0, 0x102, 0);
    Actor_unk42_3(3, 0x102, 60);
    Actor_unk129_4(3, 0, 20);
    Actor_unk43_3(2, 0xa000, 20);
    Actor_unk130_4(2, 3);
    Actor_unk131_3(20);
    Actor_unk132_3(2, 0, 20);
    Actor_unk133_3(0, 1);
    Actor_unk134_3(1, 1);
    Actor_unk135_3(3, 1);
    Actor_unk136_3(20);
    Actor_unk44_3(2, 0x8000, 0x4000);
    Actor_unk137_3(2, 128, 184);
    Actor_unk45_3(2, 0x4000, 20);
    Actor_unk46_3(0, 0xc000, 0);
    Actor_unk47_3(1, 0xa000, 0);
    Actor_unk48_3(3, 0xe000, 20);
    Actor_unk138_3(2, 3);
    Actor_unk139_3(20);
    Actor_unk32_4(2, 0, 20);
    Actor_unk140_3(0, 3);
    Actor_unk141_3(1, 3);
    Actor_unk142_3(3, 3);
    Actor_unk143_3(20);
    Actor_unk49_3(1, 0x8000, 0x4000);
    Actor_unk50_3(3, 0x8000, 0x4000);
    Actor_unk144_3(1, 144, 200);
    Actor_unk145_3(2, 144, 200);
    Actor_unk146_3(3, 144, 200);
    Actor_unk147_3(1);
    Actor_unk148_3(1, 0, 0);
    Actor_unk149_3(2);
    Actor_unk150_3(2, 0, 0);
    Actor_unk151_3(3);
    Actor_unk152_3(3, 0, 0);
    Actor_unk153_3();
}

void Dialogue_RunLine1956(void)
{
    void Actor_unk154_3(void);

    Actor_unk154_3();
    Actor_Apply8(0, 1);
    Actor_Apply9(0x1956, 1);
    Actor_unk155_2();
}

void ConfigureAndPlaceActorFourteen(void)
{
    void Actor_unk21_4(s32, s32, s32);

    s32 a = 21, b = 9;
    Actor_unk156_2(85, 9, 1, 1, a, b);
    Actor_unk157_2(100, 0, 0);
    PlaceActor(14, 0x01580000, 0x00980000);
}

void Scene_RunScene3a2(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_SetRect(21, 73, 1, 1, 21, 9);
    Actor_unk51_3(100, -1, -1);
    Actor_unk158_2(14, 0, 0);
}

void Dialogue_RunActorFifteenByLeaderHeading(void)
{
    struct Slot020008e0 *Actor_unk159_2(s32);

    u32 heading = Actor_unk159_2(0)->heading;

    Actor_unk160_2();
    if (heading - 0xA001 <= 0x3FFE) {
        Actor_unk13_2(15);
    } else {
        Actor_unk14_2(0x1A1E);
        Actor_Apply10(15, 0);
    }
    Actor_unk161_2();
}

void Scene_RunEventTransition(void)
{
    u8 *record;
    s32 none;

    if (Actor_unk3(0x89a) == 0) {
    } else {
        Actor_unk162_2();
        Actor_unk52_3(10, 0x2180000, 0xd80000);
        Actor_unk2_2(0x18b5);
        Actor_unk163_2(10, 0, 20);
        Actor_unk164_2(0, 2);
        Actor_unk165_2(20);
        record = Actor_unk9_4(0);
        *(s32 *)((s32)record + 108) = 0x2008055;
        record = Actor_unk4(0);
        if ((*(s32 *)((s32)record + 16) >> 20) == 13) {
            Actor_unk53_3(0, 0x1b8, 200);
        }
        Actor_unk54_3(10, 0x20000, 0x10000);
        Actor_unk166_2(10, 2);
        Actor_unk55_3(10, 0x198, 216);
        {
            u8 *record = Actor_unk10_4(10);
            u32 flag = 1;

            flag = flag | record[35];
            record[35] = (u8)flag;
        }
        Actor_unk167_2(10);
        Actor_unk56_3(10, 0x8000, 20);
        Actor_unk168_2(10, 0, 20);
        Actor_unk169_2(10, 2);
        Actor_unk2_4(10, 0x102);
        Actor_unk170_2(60);
        Actor_unk171_2(10, 0, 20);
        Actor_unk5(10, 0x200962c);
        Actor_unk3_4(0x1280000, -1, 0x1580000, 1);
        Actor_unk3_2(0x8b0);
        Actor_unk172_2(10);
        Actor_unk173_2();
        Actor_unk57_3(0, 0x10000, 0x8000);
        Actor_unk4_4(0, 0x20096b8);
        Actor_unk174_2(0);
        Actor_unk175_2(10);
        none = 0;
        record = Actor_unk11_4(0);
        *(s32 *)((s32)record + 108) = none;
        Actor_unk176_2(30);
        Actor_unk177_2(10, 2);
        Actor_unk178_2(20);
        Actor_unk58_3(10, 0x5000, 120);
        Actor_unk59_3(10, 0x105, 60);
        Actor_unk60_3(0, 0x101, 60);
        Actor_unk179_2(10, 4);
        Actor_unk180_2(20);
        Actor_unk181_2(10, 0, 20);
        Actor_unk182_2();
    }
}

void Scene_RunActorCue(void)
{
    extern u8 gWork[];

    Actor_unk183_2();
    Actor_unk4_2(0x18b9);
    Actor_unk61_3(10, 0x105, 60);
    Actor_unk6(10, 0);
    if (Actor_unk7(0, 0) == 1) {
        Scene_AdvanceStep(1);
    }
    Actor_unk184_2(20);
    Actor_unk185_2(10, 4);
    Actor_unk186_2(20);
    Actor_unk187_2(10, 0, 20);
    Actor_unk188_2();
}

void Scene_RunActorExchange(void)
{
    Actor_unk189_2();
    Actor_unk190_2();
    Actor_unk191_2();
    Actor_unk192_2(30);
    Actor_unk62_3(0, 0x6000, 0);
    Actor_unk193_2(9, 1);
    Actor_unk194_2();
    Actor_unk195_2(8, 2);
    Actor_unk196_2(20);
    Actor_unk5_2(0x19cf);
    Actor_unk197_2(8, 0, 20);
    Actor_unk198_2(9, 1);
    Actor_unk199_2(20);
    Actor_unk200_2(9, 0, 20);
    Actor_unk201_2(8, 3);
    Actor_unk202_2(20);
    Actor_unk203_2(8, 0, 20);
    Actor_unk63_3(10, 0xd000, 20);
    Actor_unk204_2(10, 0, 20);
    Actor_unk205_2(8, 3);
    Actor_unk206_2(60);
    Actor_unk64_3(8, 0x102, 60);
    Actor_unk207_2(8, 4);
    Actor_unk208_2(20);
    Actor_unk209_2(8, 0, 20);
    Actor_unk210_2(10, 2);
    Actor_unk5_4(10, 0x102);
    Actor_unk211_2(60);
    Actor_unk65_3(10, 0xb000, 20);
    Actor_unk212_2(9, 5);
    Actor_unk213_2();
    Actor_unk6_2(0x8b1);
}

void Scene_RunActorSequence(void)
{
    extern u8 gWork[];
    void Actor_unk214_2();

    s32 mask;

    Actor_unk214_2();
    Actor_unk6_4(8, 0x102);
    Actor_unk215_2(8, 2);
    Actor_unk216_2(60);
    Actor_unk7_2(0x19da);
    Actor_unk217_2(8, 0, 20);
    Actor_unk7_4(10, 0x102);
    Actor_unk218_2(10, 4, 0);
    Actor_unk219_2(60);
    Actor_unk220_2(10, 0, 20);
    Actor_unk221_2(8, 1);
    Actor_unk222_2(20);
    Actor_unk223_2(8, 0, 20);
    Actor_unk66_3(10, 0xd000, 20);
    Actor_unk224_2(10, 3);
    Actor_unk225_2(20);
    Actor_unk67_3(8, 178, 0x114);
    Actor_unk68_3(10, 172, 0x11c);
    Actor_unk226_2(8);
    Actor_unk69_3(8, 0x5000, 0);
    Actor_unk70_3(10, 0xb000, 0);
    Actor_unk227_2(20);
    Actor_unk228_2(8, 2);
    Actor_unk229_2(20);
    mask = 254;
    Actor_unk230_2(8, 0, 20);
    *(u8 *)(Actor_unk10(8) + 90) &= mask;
    *(u8 *)(Actor_unk11(10) + 90) &= mask;
    Actor_unk71_3(8, 0x3333, 0x1999);
    Actor_unk72_3(10, 0x3333, 0x1999);
    Actor_unk231_2(8, 5);
    Actor_unk232_2(10, 6);
    Actor_unk233_2(20);
    Actor_unk234_2(125);
    Actor_unk235_2(8, 2, 0);
    Actor_unk236_2(9, 2, 0);
    Actor_unk237_2(10, 2, 0);
    Actor_unk238_2(10);
    Actor_unk239_2(30);
    Actor_unk240_2(8, 5);
    Actor_unk241_2(10, 6);
    Actor_unk242_2(20);
    Actor_unk243_2(125);
    Actor_unk244_2(8, 4, 0);
    Actor_unk245_2(9, 4, 0);
    Actor_unk246_2(10, 4, 0);
    Actor_unk247_2(10);
    Actor_unk248_2(9);
    Actor_unk249_2(8, 1);
    Actor_unk250_2(10, 1);
    Actor_unk251_2(50);
    Actor_unk252_2(10, 2, 0);
    Actor_unk253_2(20);
    Actor_unk254_2(10, 0, 20);
    Actor_unk255_2(8, 3);
    Actor_unk256_2(20);
    Actor_unk8(8, 0);
    Actor_unk257_2(8, 0, 20);
    Actor_unk258_2(8, 5);
    Actor_unk259_2(10, 6);
    Actor_unk260_2(20);
    Actor_unk261_2(125);
    Actor_unk262_2(8, 2, 0);
    Actor_unk263_2(9, 2, 0);
    Actor_unk264_2(10, 2, 0);
    Actor_unk265_2(10);
    Actor_unk266_2(30);
    Actor_unk267_2(8, 5);
    Actor_unk268_2(10, 6);
    Actor_unk269_2(20);
    Actor_unk270_2(125);
    Actor_unk271_2(8, 4, 0);
    Actor_unk272_2(9, 4, 0);
    Actor_unk273(10, 4, 0);
    Actor_unk274(10);
    Actor_unk275(40);
    Actor_unk276(8, 1);
    Actor_unk277(10, 1);
    Actor_unk278(10, 2, 0);
    Actor_unk279(20);
    Actor_unk73_3(10, 0xd000, 20);
    Actor_unk280(10, 0, 20);
    Actor_unk281(8, 3);
    Actor_unk282(20);
    Actor_unk283(8, 0, 30);
    Actor_unk284(8, 0, 20);
    {
        u8 *record = Actor_unk12_4(10);
        u32 flag = 1;

        flag = flag | record[90];
        record[90] = (u8)flag;
    }
    Actor_unk74_3(10, 0xcccc, 0x6666);
    Actor_unk75_3(10, 168, 0x128);
    Actor_unk76_3(10, 0xd000, 20);
    Actor_unk285(10, 5);
    Actor_unk286(8, 3);
    Actor_unk287(20);
    Actor_unk288(8, 0, 20);
    Actor_unk289(10, 3);
    Actor_unk290(20);
    Actor_unk291(10, 0, 20);
    Actor_unk292(8, 3);
    Actor_unk293(20);
    Actor_unk294(8, 0, 20);
    Actor_unk295(0, 3);
    Actor_unk296(20);
    Actor_unk297();
    Actor_unk8_2(0x8b2);
    *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x201;
    Actor_unk298(6);
}

void Scene_RunScriptedSteps0And1A12(void)
{
    Actor_unk299();
    Actor_Apply11(0, 1);
    Actor_Apply12(0x1A12, 1);
    Actor_unk300();
}

void Scene_RunPairedLayoutStepsThenSetOne(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Actor_unk2_5(5, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Actor_unk3_5(5, 28, 1, 2, fifth, sixth);
    }
    Actor_unk15_2(1);
}

void State_RunRect6x28Step(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Actor_unk4_5(6, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Actor_unk5_5(6, 28, 1, 2, fifth, sixth);
    }
    Actor_unk16_2(1);
}

s32 Actor_SetFlagBitByRankAgainstActorZero(struct Actor02001060 *actor)
{
    if (Actor_unk13_4(0)->rank > actor->rank) {
        actor->flags |= 2;
    } else {
        actor->flags &= 0xFD;
    }
}

void Actor_UpdateActorFourteenByDepth(void)
{
    struct Actor *current = Actor_unk14_4(0);
    struct Actor *other = Actor_unk15_4(14);

    if (current->f10 <= other->f10) {
        Actor_Apply13(14, 1);
    }
}

void ActorDraw_PrepareActorFourteenWithCallback(void)
{
    u8 zero;

    zero = 0;
    Actor_unk301();

    Actor_unk16_4(14)[35] &= 0xfd;
    Actor_unk17_4(14)[89] &= 0xfd;
    Actor_unk18_4(14)[85] = zero;
    *(void **)(Actor_unk19_4(14) + 108) = (void *)0x02009061;

    Actor_unk302(55, 16, 1, 1, 56, 18);
    Actor_unk303(55, 16, 1, 1, 20, 18);

    Actor_unk304(1);
    Actor_unk305(512);
    Actor_unk306(14, 2);
    Actor_unk307();
}

void Scene_SetSlot15Byte89AndRunStep(void)
{
    u8 *slot;

    Actor_unk308();
    {
        s32 fifth = 21;
        s32 sixth = 11;

        Actor_unk6_5(14, 6, 1, 2, fifth, sixth);
    }
    slot = Actor_unk20_4(15) + 89;
    *slot = 254;
    Actor_unk17_2(0x201);
    Actor_unk309();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gCell[224] == (s32)&Value_0000004a) {
        return (s32)gOv5;
    }
    return (s32)gOv6;
}
