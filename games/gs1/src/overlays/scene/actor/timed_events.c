/*
 * A run of unnamed leaf owners in resource_376: scene data getters, actor
 * dialogue beats, numbered scene steps, and the long presentation
 * sequence.
 */

#include "types.h"
#include "scene.h"

/*
 * Each Func_ symbol names the pre-relocation call word the image holds,
 * not a runtime address.
 */

#include "facing_object.h"

typedef struct {
    u8 pad_to_position[8];
    s32 x;
    s32 y;
    u8 pad_to_scale[8];
    s32 scale_x;
    s32 scale_y;
    u8 pad_to_velocity[0x44];
    s16 velocity_x;
    s16 velocity_y;
} ScaledMotion;

union SceneActor {
    s32 words[26];
    u16 halfwords[52];
};

/*
 * The fields at 100 and 102 are consecutive halfwords, so this byte and
 * halfword view needs two-byte layout rather than the default four.
 */
union SceneField {
    u16 value;
    u8 bytes[2];
} __attribute__((packed, aligned(2)));

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gCell[];
extern s16 gCell2[][1];
extern u8 gOv4[];
extern u8 gOv5[];
extern u32 gIw;
extern u8 gOv6[];

u8 *Actor_unk16_4();
u8 *Actor_unk17_4();

u8 *Actor_unk18_4(s32 actor_id);

union SceneActor *Actor_unk19_4(s32);
s32 *Actor_unk20_4(s32);

s32 OvObj_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 AdvancePositionScaleAndVelocity(ScaledMotion *motion)
{
    motion->x += motion->velocity_x << 8;
    motion->y += motion->velocity_y << 8;
    motion->scale_x += 0x666;
    motion->scale_y += 0x666;
    motion->velocity_x += 5;
    motion->velocity_y -= 1;
    return 0;
}

/* The eight-byte owner includes its one pool word. */
void *SceneData_GetTable9478(void)
{
    return (void *)0x02009478;
}

/* A four-byte leaf that returns zero. */
int SceneData_ReturnZero(void)
{
    return 0;
}

/* The 36-byte owner includes its three pool words. */
void *SceneData_SelectTable9568ByFlag(void)
{
    if (Actor_unk18(0x834) != 0)
        return (void *)0x02009590;
    return (void *)0x02009568;
}

void *SceneData_SelectFlaggedTable(void)
{
    void *tbl;

    if (Actor_unk19(0x87a)) {
        tbl = gOv;
    } else if (Actor_unk20(0x815)) {
        tbl = gOv2;
    } else {
        tbl = gOv3;
    }
    Actor_unk12_2(tbl);
    return tbl;
}

/* The 80-byte owner includes its seven pool words. */
void *SceneData_SelectTable9c00ByFlags(void)
{
    if (Actor_unk21_4(0x834) != 0)
        return (void *)0x02009ac8;
    if (Actor_unk22_4(0x87a) != 0)
        return (void *)0x02009ffc;
    if (Actor_unk21(0x815) != 0)
        return (void *)0x02009da4;
    return (void *)0x02009c00;
}

/* The 44-byte actor-15 scene owner includes its one pool word. */
void Dialogue_RunActor15Message0f6d(void)
{
    Actor_unk23_4();
    Actor_unk13_2(0xf6d);
    Actor_unk101_3(0, 15, 6);
    Actor_Apply(15, 0);
    Actor_unk24_4();
}

/* The 44-byte actor-19 scene owner includes its one pool word. */
void Dialogue_RunActor19Message0f73(void)
{
    Actor_unk25_4();
    Actor_unk14_2(0xf73);
    Actor_unk102_3(0, 19, 6);
    Actor_Apply2(19, 0);
    Actor_unk26_4();
}

void Scene_RunScene376(void)
{
    s32 callback;
    s32 base5_11a4;

    Actor_unk27_4();
    if (Actor_Check(0x81b) != 0) {
        Actor_Do(0x11a6);
        Actor_unk28_4(20, 0);
        callback = 0x20092fc;
        Actor_Place(20, 0x10000, callback);
    } else {
        base5_11a4 = (s32)gVal;
        Actor_unk29_4(base5_11a4);
        Actor_unk30_4(20, 0, 20);
        Actor_unk31_4((base5_11a4 + 1), 1);
        Actor_unk32_4(180, 0);
        Actor_unk2_2(0x81b);
    }
    Actor_unk33_4();
}

/* The 32-byte actor-16 dialogue owner includes its one pool word. */
void Dialogue_RunActor16Message11be(void)
{
    Actor_unk34_4();
    Actor_unk15_2(0x11be);
    Actor_Apply3(16, 0);
    Actor_unk35_4();
}

/* The 32-byte actor-10 dialogue owner includes its one pool word. */
void Dialogue_RunActor10Message1c3d(void)
{
    Actor_unk36_4();
    Actor_unk16_2(0x1c3d);
    Actor_Apply4(10, 0);
    Actor_unk37_4();
}

void Scene_RunScene376(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_unk38_4();
    Actor_unk39_4((s32)gVal2);
    Actor_Run(0x800b, 0);
    Actor_unk40_4();
}

/* The 76-byte shared numbered-scene owner includes its two pool words. */
void State_SetRuntimeWord448To521AndRun(s32 value)
{
    extern u8 *gWork;
    void Actor_unk41_4(s32 value);
    void Actor_unk42_4(void);

    if (Actor_unk22(0x834) != 0)
        Actor_unk43_4();
    Actor_unk17_2(123);
    *(s32 *)(gWork + 448) = 521;
    *(s32 *)(gWork + 456) = 16;
    Actor_unk44_4();
    Actor_unk42_4();
    Actor_unk41_4(value);
}

/* Eight numbered-scene wrappers follow, each a twelve-byte owner. */
void Scene_RunIndexedStep1(void)
{
    Actor_unk18_2(1);
}

void Scene_RunIndexedStep2(void)
{
    Actor_unk19_2(2);
}

void Scene_RunIndexedStep3(void)
{
    Actor_unk20_2(3);
}

void Scene_RunIndexedStep4(void)
{
    Actor_unk21_2(4);
}

void Scene_RunIndexedStep5(void)
{
    Actor_unk22_2(5);
}

void Scene_RunIndexedStep6(void)
{
    Actor_unk23_2(6);
}

void Scene_RunIndexedStep7(void)
{
    Actor_unk24_2(7);
}

void Scene_RunIndexedStep8(void)
{
    Actor_unk25_2(8);
}

s32 Scene_RunSupplementalSequenceOne(void)
{
    extern u8 gWork[];
    void Actor_unk46_4();
    void Actor_unk47_4();

    u32 i;
    u8 *record;
    u8 *volatile *scene = (u8 *volatile *)gWork;

    *(s32 *)(scene[0] + 0x1c0) = 0x209;
    if (Actor_unk2(0x834) != 0) {
        Actor_unk48_4(8, 0, 0);
        Actor_unk49_4(9, 0, 0);
        Actor_unk46_4(10, 0, 0);
        Actor_unk50_4(11, 0, 0);
        Actor_unk51_4(12, 0, 0);
        Actor_unk52_4(13, 0, 0);
        Actor_unk53_4(14, 0, 0);
        Actor_unk47_4(15, 0, 0);
        ((void (*)())Actor_unk23)(16, 0, 0);
        Actor_unk54_4(17, 0, 0);
        Actor_unk55_4(18, 0, 0);
        Actor_unk56_4(19, 0, 0);
        Actor_unk57_4(20, 0, 0);
        Actor_unk58_4(21, 0, 0);
        Actor_unk59_4(22, 0, 0);
        Actor_unk60_4();
        {
            u16 *target = (u16 *)(scene[3] + 0x1f84);
            s32 shown = 1;

            *target = shown;
        }
        Actor_unk61_4();
        Actor_unk62_4(30);
        Actor_unk63_4();
        Actor_unk64_4();
        Actor_unk65_4();
    }
    if (Actor_unk3(0x87a) != 0) {
        if (gCell2[225][0] == 6) {
            if (Actor_unk4(0x81d) == 0) {
                Actor_unk66_4();
            }
        }
        {
            u8 *record = Actor_unk16_4(10);
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 128);
        }
    }
    if (gCell2[225][0] == 2) {
        if (Actor_unk5(0x815) != 0) {
            Actor_unk2_3(13, 0x1c60000, 0x960000);
            record = Actor_unk17_4(13);
            Actor_unk67_4((s32)record, 0);
            Actor_unk68_4(13, 5);
            Actor_unk69_4(4);
        }
    }
    return 0;
}

/*
 * Choose what follows from actor zero's directional halfword at +6 and two
 * story flags. The halfword is reduced by 0xa001 modulo 2^32 and compared
 * unsigned with 0x3ffe, so the first arm covers 0xa001..0xdfff; what that
 * range means is not established. The record is fetched before either path
 * is chosen. The 128-byte owner includes its seven pool words.
 */
void Scene_RunByActorDirectionAndFlags(void)
{
    void Actor_unk70_4(s32 arg0, s32 actor_id);

    u8 *p;
    u32 dir;

    p = Actor_unk18_4(0);
    dir = *(u16 *)(p + 6);
    dir += 0xffff5fff;

    if (dir <= 0x3ffe) {
        Actor_unk70_4(1, 21);
        return;
    }

    Actor_unk71_4();
    if (Actor_unk72_4(0x87a) != 0) {
        Actor_unk73_4(0x1c06);
        Actor_Apply5(21, 0);
    } else {
        if (Actor_unk24(0x815) != 0) {
            Actor_unk74_4(0x11a2);
        } else {
            Actor_unk26_2(0x0f53);
        }
        Actor_Apply6(21, 0);
    }
    Actor_unk75_4();
}

void Scene_RunScene376(void)
{
    extern u8 gWork[];
    void Actor_unk76_4();

    u32 i;
    s32 record;

    record = Actor_unk25(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0xa001) <= 0x3ffe) {
        Actor_unk77_4(2, 22);
    } else {
        ((void (*)())Actor_unk26)();
        if (Actor_unk6(0x87a) != 0) {
            Actor_unk3_2(0x1c09);
        } else {
            if (Actor_unk7(0x815) != 0) {
                Actor_unk4_2(0x11a3);
            } else {
                Actor_unk5_2(0xf54);
            }
        }
        Actor_unk78_4(22, 0);
        Actor_unk79_4();
    }
}

void Scene_RunScene376(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    record = Actor_unk27(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0xa001) <= 0x3ffe) {
        Actor_unk80_4(3, 20);
    } else {
        if (Actor_unk8(0x87a) != 0) {
            Actor_unk81_4();
            Actor_unk6_2(0x1c0a);
            Actor_unk82_4(20, 0);
            Actor_unk83_4();
        } else {
            if (Actor_unk9(0x815) != 0) {
                Actor_unk84_4();
            } else {
                Actor_unk85_4();
                Actor_unk7_2(0xf55);
                Actor_unk86_4(20, 0);
                Actor_unk87_4();
            }
        }
    }
}

void Scene_RunLongPresentationSequence(void)
{
    extern u8 gWork[];
    void Actor_unk88_4();

    u32 i;
    s32 record;
    s32 v6;
    s32 base7_20090c1;
    s32 base5_20092fc;
    s32 base5_2009400;

    Actor_unk89_4();
    Actor_unk2_4(-1, -1, -1, 0);
    Actor_unk90_4(1);
    Actor_unk91_4(3, 1);
    Actor_unk3_3(0, 0x6666, 0x3333);
    Actor_unk4_3(1, 0x6666, 0x3333);
    Actor_unk5_3(2, 0x6666, 0x3333);
    Actor_unk6_3(3, 0x6666, 0x3333);
    Actor_unk92_4(8, 5);
    Actor_unk7_3(0, 0x328, 0x1fc);
    record = Actor_unk28(23);
    Actor_unk93_4(record, 0);
    record = Actor_unk29(24);
    Actor_unk94_4(record, 0);
    record = Actor_unk30(25);
    Actor_unk95_4(record, 0);
    v6 = 0;
    *(u8 *)(Actor_unk31(23) + 85) = v6;
    *(u8 *)(Actor_unk32(24) + 85) = v6;
    *(u8 *)(Actor_unk33(25) + 85) = v6;
    base7_20090c1 = (s32)Actor_unk96_4;
    Actor_unk3_4(base7_20090c1, 0xc80);
    Actor_unk97_4(1);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 32;
    Actor_unk98_4();
    Actor_unk99_4();
    Actor_unk100_4(0);
    Actor_unk101_4(0, 1);
    record = Actor_unk10(0);
    if (record != 0) {
        Actor_unk102_4(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Actor_unk11(0);
    if (record != 0) {
        Actor_unk103_4(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Actor_unk12(0);
    if (record != 0) {
        Actor_unk104_4(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Actor_unk8_3(1, 0x318, 0x200);
    Actor_unk9_3(2, 0x338, 0x1f8);
    Actor_unk10_3(3, 0x332, 0x20c);
    Actor_unk105_4(1, 1);
    Actor_unk106_4(2, 1);
    Actor_unk107_4(10);
    base5_20092fc = (s32)gOv4;
    Actor_unk11_3(0, 0x1000a, base5_20092fc);
    Actor_unk12_3(1, 0x1000a, base5_20092fc);
    Actor_unk13_3(2, 0x1000a, base5_20092fc);
    Actor_unk14_3(3, 0x1000a, base5_20092fc);
    Actor_unk8_2(0x12c);
    *(u8 *)(Actor_unk34() + 85) = v6;
    Actor_unk4_4(0x1999, 0x333);
    Actor_unk5_4(0x3120000, 0, 0x1ae0000, 1);
    Actor_unk108_4(240);
    Actor_unk109_4(10);
    Actor_unk15_3(10, 0x102, 80);
    Actor_unk16_3(10, 0x333, 0x195);
    Actor_unk110_4(40);
    Actor_unk111_4(10, 4);
    Actor_unk112_4(40);
    Actor_unk17_3(10, 0xd000, 20);
    Actor_unk9_2(0x1c1e);
    Actor_unk18_3(0x900a, 0, 20);
    Actor_unk113_4(0);
    Actor_unk114_4(1);
    Actor_unk115_4(2);
    Actor_unk116_4(3);
    Actor_unk19_3(11, 0x100, 40);
    Actor_unk20_3(0x200b, 0, 20);
    Actor_unk117_4(10, 2);
    Actor_unk118_4(40);
    Actor_unk21_3(0x900a, 0, 10);
    Actor_unk22_3(11, 0x5000, 10);
    Actor_unk23_3(0x200b, 0, 40);
    Actor_unk119_4(10, 2);
    Actor_unk120_4(20);
    Actor_unk24_3(0x900a, 0, 20);
    Actor_unk25_3(0, 0x102, 80);
    Actor_unk26_3(11, 0x106, 40);
    Actor_unk27_3(0x200b, 0, 40);
    Actor_unk121_4(10, 2);
    Actor_unk28_3(10, 0x102, 20);
    Actor_unk122_4(10, 4);
    Actor_unk29_3(0x900a, 0, 10);
    Actor_unk123_4(11, 1);
    Actor_unk124_4(11, 3);
    Actor_unk125_4(20);
    Actor_unk126_4(10, 1);
    Actor_unk127_4(10, 4);
    Actor_unk128_4(11, 1);
    Actor_unk129_4(11, 3);
    Actor_unk130_4(10, 1);
    Actor_unk131_3(10, 4);
    Actor_unk30_3(9, 0x105, 0);
    Actor_unk132_3(9, 1);
    Actor_unk133_3(20);
    Actor_unk31_3(9, 0x1000, 40);
    Actor_unk134_3(9, 2);
    Actor_unk135_3(60);
    Actor_unk136_3(9, 3);
    Actor_unk137_3(40);
    Actor_unk32_3(0x4009, 0, 40);
    Actor_unk138_3(11, 0);
    Actor_unk139_3(11, 2);
    Actor_unk33_3(0x200b, 0, 10);
    Actor_unk140_3(9, 4);
    Actor_unk141_3(9, 2);
    Actor_unk34_3(0x4009, 0, 10);
    Actor_unk35_3(10, 0x100, 20);
    Actor_unk36_3(10, 0x5000, 40);
    Actor_unk142_3(10, 3);
    Actor_unk37_3(0x400a, 0, 10);
    Actor_unk143_3(9, 4);
    Actor_unk38_3(9, 0xd000, 10);
    Actor_unk144_3(9, 2, 0);
    Actor_unk145_3(9, 4);
    Actor_unk39_3(0x4009, 0, 10);
    Actor_unk40_3(11, 0x101, 0);
    Actor_unk41_3(10, 0x101, 40);
    Actor_unk42_3(10, 0xd000, 80);
    Actor_unk43_3(10, 0x5000, 60);
    Actor_unk146_3(10, 2);
    Actor_unk147_3(11, 2);
    Actor_SetRect(10, 11, 6, 6, 6, 11, 12, 1, 7, 1, v6);
    Actor_unk148_3(20);
    Actor_unk6_4(0x19999, 0x3333);
    Actor_unk7_4(0x3090000, 0, 0x1d40000, 1);
    Actor_unk149_3();
    Actor_unk150_3(40);
    Actor_unk151_3(1, 3);
    Actor_unk44_3(0x1001, 0, 20);
    Actor_unk152_3(8, 2);
    Actor_unk153_3(base7_20090c1);
    Actor_unk154_3(40);
    Actor_unk155_2(8, 6);
    Actor_unk156_2(20);
    Actor_unk45_3(0x4008, 0, 20);
    Actor_unk8_4(0x2ee0000, 0, 0x1c30000, 1);
    Actor_unk157_2(20);
    Actor_unk46_3(11, 0x5000, 0);
    Actor_unk47_3(10, 0x5000, 10);
    Actor_unk48_3(8, 0x1000, 40);
    Actor_unk49_3(8, 0x100, 40);
    Actor_unk50_3(8, 0x3000, 20);
    Actor_unk51_3(8, 0x1000, 20);
    Actor_unk52_3(8, 0x3000, 40);
    Actor_unk158_2(8, 6);
    Actor_unk159_2(60);
    Actor_unk160_2(8, 6, 0);
    Actor_unk53_3(0x4008, 0, 20);
    Actor_unk54_3(1, 0x19999, 0xcccc);
    Actor_unk55_3(1, 0x315, 0x1d9);
    Actor_unk56_3(1, 0x7000, 20);
    Actor_unk161_2(1, 3);
    Actor_unk57_3(0x4001, 0, 10);
    Actor_unk58_3(8, 0x1000, 20);
    Actor_unk162_2(8, 3);
    Actor_unk13(0x4008, 0);
    Actor_unk59_3(10, 0x5000, 0);
    Actor_unk60_3(9, 0x1000, 0);
    Actor_unk61_3(1, 0x3000, 0);
    Actor_unk62_3(2, 0x7000, 0);
    Actor_unk63_3(3, 0xb000, 0);
    if (Actor_unk14(0, 0) == 1) {
        bump_step(1);
    }
    Actor_unk9_4(0x3090000, 0, 0x1ac0000, 1);
    ((void (*)())Actor_unk35)(20);
    Actor_unk163_2(10, 2);
    Actor_unk164_2(10, 0);
    Actor_unk165_2(11, 4);
    Actor_unk166_2(20);
    Actor_unk10_2(0x1c33);
    Actor_unk10_4(0x200b, 0);
    Actor_unk11_4(0x3090000, 0, 0x1d40000, 1);
    Actor_unk167_2(20);
    Actor_unk64_3(1, 0xd000, 20);
    Actor_unk168_2(1, 3);
    Actor_unk169_2(20);
    Actor_unk170_2(9, 4);
    Actor_unk65_3(9, 0xd000, 10);
    Actor_unk12_4(0x4009, 0);
    Actor_unk171_2(8, 3);
    Actor_unk13_4(0x4008, 0);
    Actor_unk66_3(1, 0x7000, 10);
    Actor_unk172_2(1, 3);
    Actor_unk67_3(9, 0x1000, 10);
    Actor_unk173_2(11, 3);
    Actor_unk174_2(10, 3);
    Actor_unk175_2(9, 3);
    Actor_unk176_2(8, 3);
    Actor_unk177_2(20);
    Actor_unk68_3(1, 0x3000, 20);
    Actor_unk69_3(1, 0x102, 80);
    Actor_unk70_3(1, 0x7000, 20);
    Actor_unk71_3(0x4001, 0, 20);
    Actor_unk72_3(1, 0x3000, 10);
    Actor_unk178_2(0, 0, 40);
    Actor_unk179_2(0, 3);
    Actor_unk180_2(1, 3);
    Actor_unk181_2(20);
    Actor_unk73_3(0, 0x4000, 20);
    Actor_unk182_2(3, 3);
    Actor_unk183_2(20);
    Actor_unk74_3(1, 0x1000, 0);
    Actor_unk75_3(0, 0xe000, 0);
    Actor_unk76_3(2, 0x10000, 0x8000);
    Actor_unk77_3(2, 0x333, 0x1e9);
    Actor_unk78_3(2, 0xb000, 40);
    Actor_unk184_2(2, 2);
    Actor_unk185_2(2, 0, 20);
    Actor_unk186_2(2, 3);
    Actor_unk187_2(8, 3);
    Actor_unk188_2(9, 3);
    Actor_unk189_2(10, 3);
    Actor_unk190_2(9, 3);
    *(u8 *)(Actor_unk36(3) + 35) &= 254;
    Actor_unk191_2(3, 1);
    Actor_unk79_3(3, 0x10000, 0x8000);
    Actor_unk80_3(3, 0x31a, 0x208);
    Actor_unk81_3(1, 0x5000, 0);
    Actor_unk82_3(0, 0xa000, 0);
    Actor_unk83_3(3, 0x310, 0x1f0);
    Actor_unk84_3(3, 0x9000, 10);
    *(u8 *)(Actor_unk37(3) + 35) |= 1;
    Actor_unk192_2(3, 0, 20);
    Actor_unk193_2(8, 3);
    Actor_unk194_2(9, 3);
    Actor_unk195_2(10, 3);
    Actor_unk196_2(9, 3);
    Actor_unk197_2(20);
    Actor_unk14_4(0x3090000, 0, 0x1ac0000, 1);
    Actor_unk198_2(20);
    Actor_unk85_3(11, 0x6666, 0x3333);
    Actor_unk86_3(11, 0x343, 0x184);
    Actor_unk87_3(11, 0x5000, 0);
    Actor_unk88_3(11, 0x108, 40);
    Actor_unk89_3(0x200b, 0, 20);
    Actor_unk15_4(0x3090000, 0, 0x1d40000, 1);
    Actor_unk199_2(40);
    Actor_unk90_3(2, 0x7000, 0);
    Actor_unk91_3(3, 0xf000, 40);
    Actor_unk92_3(2, 0x9000, 0);
    Actor_unk93_3(3, 0xd000, 20);
    Actor_unk200_2(2, 3);
    Actor_unk201_2(3, 3);
    Actor_unk202_2(20);
    Actor_unk203_2(10, 1);
    Actor_unk204_2(20);
    Actor_unk205_2(10, 3);
    Actor_unk206_2(10, 0, 20);
    Actor_unk94_3(0, 0xc000, 0);
    Actor_unk95_3(1, 0xd000, 0);
    Actor_unk96_3(2, 0xb000, 0);
    Actor_unk97_3(3, 0xd000, 40);
    Actor_unk207_2(0, 3);
    Actor_unk208_2(1, 3);
    Actor_unk209_2(2, 3);
    Actor_unk210_2(3, 3);
    Actor_unk211_2(20);
    Actor_unk98_3(2, 0x10000, 0x8000);
    base5_2009400 = (s32)gOv5;
    Actor_unk212_2(1, base5_2009400);
    Actor_unk15(2, base5_2009400);
    Actor_unk16(3, base5_2009400);
    Actor_unk17(10, 0x2009310);
    Actor_unk99_3(11, 0x345, 0x178);
    Actor_unk100_3(11, 0xd000, 20);
    Actor_unk11_2(0x81d);
    Actor_unk213_2();
}

void Scene_UpdateTimedActor(void)
{
    u32 Actor_unk214_2(u32, u32);

    s32 no;
    u32 phase;
    union SceneActor *actor;
    s32 *other;

    phase = Actor_unk214_2(gIw, 180);
    no = 23;
    switch (phase) {
    case 10:
        break;
    case 20:
        no = 24;
        break;
    case 30:
        no = 25;
        break;
    default:
        return;
    }
    actor = Actor_unk19_4(no);
    if (actor == NULL) {
        return;
    }
    other = Actor_unk20_4(8);
    if (other != NULL) {
        Actor_unk103_3(no, other[2], other[4]);
    }
    actor->words[6] = 0x6666;
    actor->words[7] = 0x6666;
    {
        s32 y = actor->words[3] + 0x180000;
        union SceneField *dst = (union SceneField *)(actor->halfwords + 50);
        s32 value;
        actor->words[3] = y;
        actor->words[15] = y;
        value = 25;
        dst->value = value;
        dst++;
        value = 128;
        dst->value = value;
    }
    Actor_Apply7(no, gOv6);
}
