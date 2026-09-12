#include "types.h"
#include "scene.h"
#include "configured_effect_spawn.h"
#include "configured_effect_spawn_body.inc"
#include "overlay_object.h"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"

/* map/locations/heidia/village_story/village_story_scene/actor_search.c */

typedef struct {
    u8 pad_to_kind[0x28];
    s16 *kind;
} ActorData;

typedef struct {
    u8 pad_to_data[0x50];
    ActorData *data;
} Actor;

Actor *GetActor(s32 actor_id);

s32 FindNearestF2Actor(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    Actor **actor_slot;
    Actor *origin;
    s32 nearest_actor = 0;
    s32 min_dist;
    u32 actor_id;

    min_dist = 640;
    origin = GetActor(0);
    actor_id = 8;
    actor_slot = (Actor **)(work + 0x34);
    do {
        Actor *actor = *actor_slot++;
        if (actor != 0) {
            if (*actor->data->kind == 0xf2) {
                s32 dist = MeasurePositionDistance(
                    (u8 *)origin + 8, (u8 *)actor + 8);
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_actor = actor_id;
                }
            }
        }
        actor_id++;
    } while (actor_id <= 65);
    return nearest_actor;
}

/* map/locations/heidia/village_story/village_story_scene/configurable_effect_spawn.c */

void Effect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

/* map/locations/heidia/village_story/village_story_scene/fixed_point_distance.c */
typedef s32(*IwramIntegerSquareRoot)(s32);

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot)0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

/* map/locations/heidia/village_story/village_story_scene/orbiting_effect.c */

void Effect_UpdateArcPosition(u8 *work)
{
    s32 parent;
    s32 phase;
    s32 next;
    s32 x;
    s32 y;

    parent = *(s32 *)(work + 104);
    phase = *(u16 *)(work + 100);
    x = Map_unk43(phase);
    *(volatile s32 *)(work + 8) = (*(s32 *)(parent + 8) + (x *(*(s32 *)(work + 48) + 28)));
    y = Map_unk44(phase);
    *(volatile s32 *)(work + 16) = ((y << 4) + 0x900000);
    *(volatile s32 *)(work + 56) = *(s32 *)(work + 8);
    *(volatile s32 *)(work + 64) = ((y << 4) + 0x900000);
    next = *(volatile u16 *)(work + 100);
    next = next + -0x200;
    *(volatile u16 *)(work + 100) = (u16)next;
}

/* map/locations/heidia/village_story/village_story_scene/overlay_object.c */
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct OverlayObjectRecordFieldView {
    u8 unknown_00[9];
    u8 unknown_00_0 : 2;
    u8 field_01 : 2;
    u8 unknown_04_0 : 4;
};

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

void *AcquireOverlayObject(s32, s32, s32, s32);

void SetOverlayObjectRecordField1(struct OverlayObject *object, s32 value)
{
    struct OverlayObjectRecordFieldView *record;
    record = (struct OverlayObjectRecordFieldView *)object->record;
    record->field_01 = value;
}

void *OvObj_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

void OvObj_ApplyVelocity(union Slot *object)
{
    u16 *record;

    object[2].w += object[17].w;
    object[3].w += object[18].w;
    object[4].w += object[19].w;
    object[6].w += object[12].w;
    object[7].w += object[13].w;
    record = (u16 *)object[20].p;
    record[15] += object[25].h[0];
}

/* map/locations/heidia/village_story/village_story_scene/run_scene_58_sequence.c */
#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE 0x1725
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_STEP_VALUE 0x157e
#else
#define SCENE_STEP_VALUE 0x159c
#endif
#if defined(GS1_EDITION_DE)
#endif

extern u8 *gWork;
extern u8 gCell[];
extern u8 Value_0000003a;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * resource_39d: scene-58 renderer setup.  The owner runs from 0x02000b24 to
 * its interworking epilogue at 0x02001ad4 and includes the 22-byte literal and
 * alignment tail that ends immediately before the next prologue, 4044 bytes in
 * all.  The declarations below are old-style because the image exposes no
 * prototype for these calls; call sites vary in arity.
 */

extern u8 *Scene_GetRecord();

extern u8 *Map_unk263_2();

/*
 * Loader-relocated ROM calls: each name spells the pre-relocation call word
 * the image holds, not a runtime address.
 */

void RunScene58Sequence(void)
{
    void *temp_r0;
    void *temp_r0_10;
    void *temp_r0_11;
    void *temp_r0_12;
    void *temp_r0_13;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r0_4;
    void *temp_r0_5;
    void *temp_r0_6;
    void *temp_r0_7;
    void *temp_r0_8;
    void *temp_r0_9;
    void *temp_r2;
    void *temp_r2_2;
    void *temp_r2_3;
    void *temp_r2_4;
    s32 flag;
    s32 flag2;
    s32 bits;

    Map_Run();
    temp_r0 = Map_Check(0x11);
    FIELD(temp_r0, u8 *, 0x55) = (u8)(0xFA & FIELD(temp_r0, u8 *, 0x55));
    Map_Place(0, 0x0000cccc, 0x00006666);
    Map_unk2_3(1, 0x0000cccc, 0x00006666);
    Map_unk3_3(2, 0x0000cccc, 0x00006666);
    Map_unk4_3(3, 0x0000cccc, 0x00006666);
    temp_r0_2 = Map_unk2(0);
    if (temp_r0_2 != 0) {
        Map_unk5_3(1, FIELD(temp_r0_2, s32 *, 8), FIELD(temp_r0_2, s32 *, 0x10));
    }
    temp_r0_3 = Map_unk3(0);
    if (temp_r0_3 != 0) {
        Map_unk6_3(2, FIELD(temp_r0_3, s32 *, 8), FIELD(temp_r0_3, s32 *, 0x10));
    }
    temp_r0_4 = Map_unk4(0);
    if (temp_r0_4 != 0) {
        Map_unk7_3(3, FIELD(temp_r0_4, s32 *, 8), FIELD(temp_r0_4, s32 *, 0x10));
    }
    Map_Do(1);
    Map_unk8_3(0, 0x158, 0xE8);
    Map_unk9_3(1, 0x148, 0xE8);
    Map_unk10_3(2, 0x158, 0xF8);
    Map_unk11_3(3, 0x148, 0xF8);
    Map_unk2_2(0);
    Map_unk12_3(0, 0xA000, 0);
    Map_unk3_2(1);
    Map_unk13_3(1, 0xC000, 0);
    Map_unk4_2(2);
    Map_unk14_3(2, 0xA000, 0);
    Map_unk5_2(3);
    Map_unk15_3(3, 0xC000, 0);
    Map_unk6_2(0x32);
    Map_unk2_4(1, 2);
    Map_unk7_2(0x14);
    Map_unk3_4(0x18000, 0x3000);
    Map_unk4_4(0x01480000, 0x280000, 0xB00000, 1);
    FIELD(Map_unk5_4(), s8 *, 0x55) = 0;
    Map_unk16_3(1, 0x18000, 0xC000);
    Map_unk17_3(1, 0x148, 0xD8);
    Map_unk6_4();
    Map_unk18_3(1, 0x0000cccc, 0x00006666);
    Map_unk8_2(0x3C);
    Map_unk7_4(0x01580000, 0x180000, 0xE80000, 1);
    Map_unk19_3(1, 0x2000, 0x14);
    Map_unk8_4();
    Map_unk9_4(1, 4);
    Map_unk9_2(SCENE_STEP_VALUE);
    Map_unk20_3(1, 0, 0x14);
    Map_unk10_4(3, 0x102);
    Map_unk10_2(0x3C);
    Map_unk21_3(3, 0x18000, 0xC000);
    Map_unk22_3(3, 0x148, 0xE8);
    Map_unk11_2(0xA);
    Map_unk23_3(3, 0, 0x14);
    Map_unk11_4(3, 4);
    Map_unk12_2(0x14);
    Map_unk24_3(3, 0, 0x1E);
    Map_unk25_3(2, 0, 0);
    Map_unk13_2(0x3C);
    Map_unk26_3(2, 0x150, 0xF8);
    Map_unk27_3(2, 0xA000, 0);
    Map_unk28_3(0, 0x8000, 0);
    Map_unk14_2(0x1E);
    Map_unk29_3(2, 0, 0x14);
    Map_unk30_3(3, 0, 0x1E);
    Map_unk5(3, 0);
    if (Map_unk6(0, 0) == 0) {
        Map_unk15_2(0x14);
        Map_unk31_3(3, 0x100, 0x3C);
        Map_unk32_3(1, 0x4000, 0xA);
        Map_unk12_4(1, 3);
        Map_unk33_3(1, 0, 0x14);
        Map_unk34_3(3, 0xC000, 0);
        Map_unk13_4(1, 4);
        Map_unk16_2(0x14);
        Map_unk35_3(1, 0, 0x14);
        temp_r2 = gWork;
        FIELD(temp_r2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2, u16 *, 0x1D8) + 2);
    } else {
        temp_r2_2 = gWork;
        FIELD(temp_r2_2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_2, u16 *, 0x1D8) + 2);
        Map_unk17_2(0x14);
        Map_unk36_3(3, 0x00000101, 0x3C);
        Map_unk37_3(1, 0x4000, 0xA);
        Map_unk14_4(1, 4);
        Map_unk38_3(1, 0, 0x14);
        Map_unk39_3(3, 0xC000, 0);
        Map_unk15_4(1, 2);
        Map_unk18_2(0x14);
        Map_unk40_3(1, 0, 0x14);
    }
    Map_unk41_3(3, 0x00000101, 0x3C);
    Map_unk42_3(3, 0, 0x14);
    Map_unk43_3(0, 1, 0);
    Map_unk19_2(0x3C);
    Map_unk44_3(0, 0x8000, 0);
    Map_unk45_3(1, 0x4000, 0x14);
    Map_unk16_4(0, 3);
    Map_unk17_4(1, 3);
    Map_unk20_2(0x14);
    Map_unk46_3(1, 0, 0x14);
    Map_unk18_4(3, 2);
    Map_unk47_3(3, 0, 0x14);
    Map_unk19_4(0, 3);
    Map_unk20_4(1, 3);
    Map_unk21_4(2, 3);
    Map_unk21_2(0x14);
    Map_unk22_4(3, 4);
    Map_unk22_2(0xA);
    Map_unk48_3(3, 0, 0xA);
    Map_unk23_4(3, 0x10);
    Map_unk49_3(3, 0, 0x3C);
    Map_unk23_2(0x11);
    Map_unk50_3(5, 0xD80000, 0xC80000);
    Map_unk51_3(5, 0, 0x14);
    Map_unk52_3(5, 0x780000, 0xA00000);
    Map_unk53_3(5, 0, 0);
    Map_unk24_4(3, 1);
    Map_unk25_4(0, 1);
    Map_unk26_4(1, 1);
    Map_unk54_3(0, 0, 0xA);
    Map_unk55_3(1, 0xE000, 0x14);
    Map_unk56_3(2, 0xA000, 0xA);
    Map_unk57_3(3, 0x8000, 0x14);
    Map_unk58_3(0, 0xA000, 5);
    Map_unk59_3(1, 0xC000, 0xA);
    Map_unk60_3(2, 0x8000, 5);
    Map_unk61_3(0, 0x8000, 5);
    Map_unk27_4(0, 2);
    Map_unk62_3(1, 0x8000, 0xA);
    Map_unk28_4(1, 2);
    Map_unk24_2(0x14);
    Map_unk29_4(0x30000, 0x6000);
    Map_unk30_4(0x780000, 0xffe80000, 0xA80000, 1);
    Map_unk31_4();
    Map_unk25_2(0x28);
    Map_unk32_4(0x15, 1);
    Map_unk26_2(0x14);
    Map_unk27_2(0x3D);
    Map_unk63_3(0x15, 0, 0x14);
    Map_unk33_4(0x17, 1);
    Map_unk28_2(0x14);
    Map_unk64_3(0x17, 0, 0x14);
    Map_unk65_3(0x17, 0xC000, 0x3C);
    Map_unk66_3(0x17, 0, 0x14);
    Map_unk67_3(0x17, 0, 0x14);
    Map_unk68_3(1, 0x01080000, 0x01200000);
    Map_unk69_3(1, 0, 0x14);
    Map_unk70_3(1, 0x01480000, 0xD80000);
    Map_unk34_4(0x17, 4);
    Map_unk29_2(0x14);
    Map_unk71_3(0x17, 0, 0x14);
    Map_unk35_4(0x17, 3);
    Map_unk30_2(0x14);
    Map_unk72_3(0x17, 0, 0x14);
    Map_unk73_3(0x17, 0xA000, 0xA);
    Map_unk74_3(0x15, 0x2000, 0xA);
    Map_unk36_4(0x15, 1);
    Map_unk31_2(0x14);
    Map_unk75_3(5, 0x8000, 0);
    Map_unk76_3(0x14, 0x8000, 0x1E);
    Map_unk77_3(0x15, 0, 0x1E);
    Map_unk37_4(5, 2);
    Map_unk38_4(0x15, 4);
    Map_unk32_2(0x14);
    Map_unk78_3(0x15, 0x2000, 0xA);
    Map_unk39_4(0x15, 3);
    Map_unk33_2(0x14);
    Map_unk40_4(0x17, 3);
    Map_unk79_3(0x15, 0x0000cccc, 0x00006666);
    Map_unk80_3(0x15, 0x68, 0xA8);
    Map_unk34_2(0x14);
    Map_unk81_3(0x17, 0, 0x14);
    Map_unk82_2(0x14, 0, 0xA);
    Map_unk83_2(5, 0x102, 0);
    Map_unk84_2(0x14, 0x102, 0x46);
    Map_unk85_2(0x16, 0x01080000, 0x01200000);
    Map_unk86_2(0x16, 0, 0x14);
    Map_unk87_2(0x16, 0x01280000, 0x780000);
    Map_unk88_2(0, 0xA000, 0);
    Map_unk89_2(1, 0xC000, 0);
    Map_unk90_2(2, 0xC000, 0);
    Map_unk91_2(3, 0xC000, 0);
    Map_unk92_2(5, 0, 0);
    Map_unk93_2(0x14, 0, 0);
    Map_unk41_4(0x18000, 0x3000);
    Map_unk42_4(0xE80000, 0x280000, 0x980000, 1);
    Map_unk94_2(0x16, 0x0000cccc, 0x00006666);
    Map_unk95_2(0x16, 0x110, 0x80);
    Map_unk96_2(0x16, 0x108, 0x98);
    Map_unk97_2(0x16, 0x118, 0xA8);
    Map_unk43_4();
    Map_unk98_2(0x16, 0x5000, 0x14);
    Map_unk35_2(0x14);
    Map_unk44_4(0x17, 0);
    Map_unk36_2(0x14);
    Map_unk45_4(0x16, 3);
    Map_unk37_2(0x14);
    Map_unk46_4(0x16, 0);
    Map_unk47_4(0x01280000, 0x280000, 0xD80000, 1);
    Map_unk99_2(0x16, 0x130, 0xB0);
    Map_unk100_2(0x16, 0x2000, 0);
    Map_unk48_4();
    Map_unk38_2(0x14);
    Map_unk49_4(0, 1);
    Map_unk50_4(1, 1);
    Map_unk51_4(2, 1);
    Map_unk52_4(3, 1);
    Map_unk39_2(0x14);
    Map_unk40_2(0x14);
    Map_unk53_4(0x16, 3);
    Map_unk41_2(0x14);
    Map_unk54_4(0x16, 0);
    Map_unk42_2(0xA);
    Map_unk101_2(0, 0x00000107, 0);
    Map_unk102_2(1, 0x00000107, 0);
    Map_unk103_2(3, 0x00000107, 0);
    Map_unk104_2(2, 0x00000107, 0x46);
    Map_unk55_4(0x16, 1);
    Map_unk43_2(0x1E);
    Map_unk56_4(0x16, 0);
    Map_unk44_2(0xA);
    Map_unk105_2(0, 0x00000105, 0);
    Map_unk106_2(1, 0x00000105, 0);
    Map_unk107_2(2, 0x00000105, 0);
    Map_unk108_2(3, 0x00000105, 0x46);
    Map_unk57_4(0x16, 3);
    Map_unk45_2(0x14);
    Map_unk58_4(0x16, 0);
    Map_unk46_2(0x14);
    Map_unk59_4(0x17, 0);
    Map_unk47_2(0xA);
    Map_unk109_2(0x16, 0x5000, 0);
    Map_unk48_2(0x28);
    Map_unk60_4(0x16, 0);
    Map_unk49_2(0x1E);
    Map_unk61_4(0x17, 0);
    Map_unk50_2(0xA);
    Map_unk110_2(0x16, 0x3000, 0);
    Map_unk51_2(0x1E);
    Map_unk62_4(0x16, 0);
    Map_unk52_2(0x14);
    temp_r0_5 = Map_unk7(1);
    FIELD(temp_r0_5, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_5, u8 *, 0x5A));
    Map_unk111_2(1, 0x148, 0xE0);
    Map_unk112_2(0, 0x158, 0xE0);
    Map_unk113_2(2, 0x158, 0xE8);
    Map_unk53_2(1);
    flag = 1;
    temp_r0_6 = Map_unk8(1);
    bits = FIELD(temp_r0_6, u8 *, 0x5A) | flag;
    FIELD(temp_r0_6, u8 *, 0x5A) = bits;
    Map_unk114_2(1, 0xC000, 0);
    Map_unk54_2(0);
    Map_unk115_2(0, 0xA000, 0);
    Map_unk55_2(2);
    Map_unk116_2(2, 0xC000, 0);
    Map_unk56_2(0x1E);
    Map_unk117_2(0x17, 0xA80000, 0xC80000);
    Map_unk118_2(0x17, 0, 0x14);
    Map_unk119_2(0x17, 0x680000, 0xC80000);
    Map_unk63_4(0x16, 1);
    Map_unk57_2(0x14);
    Map_unk120_2(0x16, 0, 0x14);
    Map_unk64_4(0x30000, 0x6000);
    Map_unk65_4(0x780000, 0xffe80000, 0xA80000, 1);
    Map_unk66_4();
    Map_unk67_4(0x17, 3);
    Map_unk58_2(0xA);
    Map_unk121_2(0x17, 0xD000, 0x14);
    Map_unk68_4(0x17, 3);
    Map_unk59_2(0x14);
    Map_unk122_2(0x17, 0, 0x14);
    Map_unk123_2(5, 0x5000, 0);
    Map_unk124_2(0x14, 0x3000, 0x46);
    Map_unk125_2(5, 0x14, 0);
    Map_unk60_2(0x32);
    Map_unk126_2(5, 0x5000, 0);
    Map_unk127_2(0x14, 0x3000, 0x14);
    Map_unk69_4(5, 4);
    Map_unk61_2(0x14);
    Map_unk128_2(0x17, 0x100, 0x3C);
    Map_unk129_2(0x17, 0, 0x14);
    Map_unk130_2(5, 0x00000105, 0x3C);
    Map_unk131_2(0x15, 0xD000, 0x14);
    Map_unk70_4(0x15, 1);
    Map_unk71_4(0x17, 3);
    Map_unk62_2(0x14);
    Map_unk132_2(0x17, 0, 0x14);
    Map_unk72_4(0x17, 4);
    Map_unk63_2(0x14);
    Map_unk133_2(0x17, 0, 0x14);
    Map_unk73_4(0x15, 0x102);
    Map_unk64_2(0x3C);
    Map_unk74_4(0x15, 4);
    Map_unk65_2(0x14);
    Map_unk134_2(0x15, 0, 0x14);
    Map_unk75_4(5, 0x102);
    Map_unk66_2(0x3C);
    Map_unk135_2(5, 0, 0x28);
    Map_unk76_4(0x15, 3);
    Map_unk67_2(0x3C);
    Map_unk136_2(5, 0xA000, 0xA);
    Map_unk77_4(0x14, 3);
    Map_unk68_2(0x14);
    Map_unk137_2(0x14, 0, 0x1E);
    Map_unk138_2(5, 0x0000b333, 0x00005999);
    Map_unk139_2(0x14, 0x0000b333, 0x00005999);
    Map_unk140_2(5, 0x80, 0x90);
    Map_unk141_2(0x14, 0x78, 0x88);
    Map_unk142_2(0x14, 0x5000, 0);
    Map_unk69_2(5);
    Map_unk143_2(5, 0, 0x14);
    Map_unk144_2(0x15, 0x3000, 0x14);
    temp_r0_7 = Map_unk45(0x15);
    FIELD(temp_r0_7, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_7, u8 *, 0x5A));
    Map_unk145_2(0x15, 0x58, 0x98);
    temp_r0_8 = Map_unk9(0x15);
    bits = FIELD(temp_r0_8, u8 *, 0x5A) | flag;
    FIELD(temp_r0_8, u8 *, 0x5A) = bits;
    Map_unk146_2(0x17, 0xB000, 0x14);
    Map_unk78_4(0x17, 3);
    Map_unk79_4(0x15, 3);
    Map_unk70_2(0x28);
    Map_unk147_2(0x17, 0x30000, 0x20000);
    FIELD(Map_unk10(0x17), s32 *, 0x28) = 0x40000;
    Map_unk71_2(0x98);
    temp_r0_9 = Map_unk11(0x17);
    FIELD(temp_r0_9, u8 *, 0x55) = (u8)(0x7E & FIELD(temp_r0_9, u8 *, 0x55));
    Map_unk80_4(Map_unk46(0x17), 0);
    FIELD(Map_unk47(0x11), s8 *, 0x55) = 4;
    Map_unk148_2(0x17, 0x68, 0xA8);
    Map_unk81_4(Map_unk48(0x17), 1);
    FIELD(Map_unk12(0x17), s8 *, 0x55) = 3;
    Map_unk149_2(0x17, 0, 0x1E);
    Map_unk150_2(0x15, 0, 0xA);
    Map_unk151_2(0x15, 0, 0x14);
    Map_unk82_3(5, 2);
    Map_unk83_3(5, 4);
    Map_unk72_2(0x14);
    Map_unk152_2(5, 0, 0x14);
    Map_unk153_2(0x17, 0, 0x14);
    Map_unk84_3(0x11, 0);
    Map_unk85_3(0x12, 0);
    Map_unk86_3();
    Map_unk87_3(0x11, 1);
    Map_unk88_3(0x12, 1);
    Map_unk154_2(0, 0x6000, 0);
    Map_unk155_2(1, 0x6000, 0);
    Map_unk156_2(2, 0x6000, 0);
    Map_unk157_2(3, 0x6000, 0);
    Map_unk89_3(0x01300000, 0x200000, 0xD80000, 1);
    Map_unk90_3();
    Map_unk158_2(0x14, 0x01100000, 0x01180000);
    Map_unk159_2(0x14, 0, 0x14);
    Map_unk160_2(0x14, 0, 0);
    Map_unk91_3(1, 1);
    Map_unk73_2(0x14);
    Map_unk161_2(1, 0, 0x14);
    Map_unk162_2(1, 0x18000, 0xC000);
    Map_unk163_2(1, 0x138, 0xD8);
    Map_unk164_2(1, 0x6000, 0xA);
    Map_unk165_2(0x16, 0, 0x14);
    Map_unk166_2(0, 0xA000, 0);
    Map_unk167_2(1, 0xC000, 0);
    Map_unk168_2(2, 0xC000, 0);
    Map_unk169_2(3, 0xC000, 0);
    Map_unk170_2(0x16, 0x138, 0xB8);
    Map_unk171_2(0x16, 0x3000, 0x14);
    Map_unk172_2(3, 0, 0x14);
    Map_unk173_2(1, 0x100, 0x3C);
    Map_unk174_2(1, 0x20000, 0x10000);
    temp_r0_10 = Map_unk13(1);
    FIELD(temp_r0_10, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_10, u8 *, 0x5A));
    Map_unk175_2(1, 0x148, 0xE0);
    Map_unk74_2(1);
    temp_r0_11 = Map_unk14(1);
    flag |= FIELD(temp_r0_11, u8 *, 0x5A);
    FIELD(temp_r0_11, u8 *, 0x5A) = flag;
    Map_unk92_3(0, 1);
    Map_unk93_3(1, 1);
    Map_unk94_3(2, 1);
    Map_unk95_3(3, 1);
    Map_unk75_2(0x14);
    Map_unk96_3(0x16, 1);
    Map_unk76_2(0xA);
    Map_unk176_2(0x16, 0, 0x14);
    Map_unk97_3(1, 2);
    Map_unk77_2(0xA);
    Map_unk15(1, 4);
    Map_unk78_2(0xA);
    Map_unk177_2(1, 0, 0x14);
    Map_unk98_3(2, 1);
    Map_unk79_2(0xA);
    Map_unk99_3(2, 3);
    Map_unk80_2(0x14);
    Map_unk178_2(2, 0, 0x14);
    Map_unk179_2(0x16, 0x00000101, 0x3C);
    Map_unk16(0x16, 0);
    if (Map_unk17(0, 0) == 0) {
        Map_unk81_2(0x14);
        Map_unk100_3(0x16, 4);
        Map_unk18(0x14);
        Map_unk180_2(0x16, 0, 0x14);
        temp_r2_3 = gWork;
        FIELD(temp_r2_3, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_3, u16 *, 0x1D8) + 1);
    } else {
        Map_unk82(0x14);
        Map_unk101_3(0x16, 4);
        Map_unk83(0x14);
        temp_r2_4 = gWork;
        FIELD(temp_r2_4, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_4, u16 *, 0x1D8) + 1);
        Map_unk181_2(0x16, 0, 0x14);
    }
    Map_unk102_3(0x16, 2);
    Map_unk84(0x14);
    Map_unk182_2(0x16, 0, 0x14);
    Map_unk183_2(0x16, 0x148, 0xC8);
    Map_unk103_3(0x16, 2);
    Map_unk184_2(0x16, 0xB000, 0x14);
    temp_r0_12 = Map_unk19(0x16);
    FIELD(temp_r0_12, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_12, u8 *, 0x5A));
    Map_unk185_2(0x16, 0x150, 0xD0);
    Map_unk85(1);
    temp_r0_13 = Map_unk49(0x16);
    flag2 = 1;
    flag2 |= FIELD(temp_r0_13, u8 *, 0x5A);
    FIELD(temp_r0_13, u8 *, 0x5A) = flag2;
    Map_unk186_2(0x16, 0x102, 0x3C);
    Map_unk187_2(3, 0x00000101, 0x3C);
    Map_unk188_2(3, 0, 0x14);
    Map_unk189_2(0x16, 0, 0x14);
    Map_unk190_2(0x16, 0x5000, 0x14);
    Map_unk104_3(0x16, 2);
    Map_unk86(0x14);
    Map_unk191_2(0x16, 0, 0x14);
    Map_unk192_2(0x16, 0x150, 0xD8);
    Map_unk105_3((s32)&Value_0000003a, 2);
    do {
        gCell[0x22B] = 3;
    } while (0);
    Map_unk106_3(0x24, 2);
    Map_unk107_3();
}

/* map/locations/heidia/village_story/village_story_scene/run_scene_59_sequence.c */
/* Ordinary C field access retained from the reviewed disassembly dataflow. */
extern u8 *gWork;
extern u8 *gCam;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
#define SCENE_REQUEST(workspace) FIELD((workspace), s32 *, 0x1C0)
#define SCENE_SKIP_COUNT(workspace) FIELD((workspace), u16 *, 0x1D8)

/*
 * resource_39d owner 0x02001af0..0x02002ddb (0x12ec = 4844 bytes).
 *
 * Scene 59 setup sequence: imported service calls with constant arguments,
 * branch-selected paths, record-field updates, and workspace counters.
 * ROM services are reached through loader-relocated call words; each call
 * names the pre-relocation word the image holds, as the retained assembly
 * does with its per-site targets.
 */

extern u8 *Map_unk264_2();

extern u8 *Map_unk265_2();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

void RunScene59Sequence(void)
{
    s32 actor9_fixed_y;
    void *actor_one_record;
    void *scene_counter_initial;
    void *scene_counter_initial_alt;
    void *scene_counter_mid_a;
    void *scene_counter_mid_b;
    void *scene_counter_first_a;
    void *scene_counter_first_b;
    void *scene_counter_system_a;
    void *scene_counter_system_b;
    void *scene_counter_later_a;
    void *scene_counter_later_b;
    void *scene_counter_final_a;
    void *scene_counter_final_b;

    Map_unk108_3();
    FIELD(Map_unk20(9), s8 *, 0x55) = 0;
    Map_unk193_2(0, 0x01580000, 0xE00000);
    Map_unk194_2(1, 0x01480000, 0xE00000);
    Map_unk195_2(2, 0x01580000, 0xE80000);
    Map_unk196_2(3, 0x01480000, 0xE80000);
    Map_unk197_2(0, 0xA000, 0);
    Map_unk198_2(1, 0xC000, 0);
    Map_unk199_2(2, 0xC000, 0);
    Map_unk200_2(3, 0xC000, 0);
    Map_unk201_2(0x16, 0x01500000, 0xB00000);
    Map_unk109_3(0x16, 9);
    Map_unk21(Map_unk50(0x16), 0);
    Map_unk110_3(0x01500000, -1, 0xD00000, 0);
    Map_unk87(1);
    Map_unk111_3();
    Map_unk88(1);
    /* The ROM loads this IWRAM pointer cell before the request store. */
    SCENE_REQUEST(gWork) = 0x100;
    Map_unk112_3();
    Map_unk113_3();
    Map_unk89(0x3C);
    Map_unk114_3(1, 2);
    Map_unk90(0x14);
    Map_unk202_2(1, 0, 0x14);
    Map_unk91(0x000015d4);
    Map_unk203_2(1, 0, 0x14);
    Map_unk204_2(0, 0x8000, 0x14);
    Map_unk115_3(0, 3);
    Map_unk92(0x14);
    Map_unk116_3(0x16, 2);
    Map_unk93(0x14);
    Map_unk205_2(0x16, 0, 0x14);
    Map_unk206(0, 0xC000, 0);
    Map_unk207(1, 0xC000, 0x14);
    Map_unk208(2, 0x00000101, 0x3C);
    Map_unk209(2, 0, 0x14);
    Map_unk117_3(0x16, 2);
    Map_unk94(0x14);
    Map_unk210(0x16, 0, 0x1E);
    Map_unk211(0x18, 0x01380000, 0x700000);
    Map_unk95(0x120);
    Map_unk212(0x18, 0, 0x14);
    Map_unk213(3, 0x100, 0x3C);
    Map_unk214(3, 0, 0x1E);
    Map_unk96(0x1D);
    Map_unk118_3(0x0000cccc, 0x00001999);
    FIELD(Map_unk119_3(), s8 *, 0x55) = 0;
    Map_unk120_3(0x01500000, -1, 0xA80000, 1);
    Map_unk97(0x14);
    Map_unk215(0x18, 0x0000cccc, 0x00006666);
    Map_unk216(0x18, 0x158, 0x88);
    Map_unk217(0x18, 0x5000, 0x14);
    Map_unk121_3();
    Map_unk218(3, 0, 0x1E);
    Map_unk122_3(0x01500000, -1, 0xB80000, 1);
    Map_unk219(0x18, 0x158, 0xA0);
    Map_unk220(0x18, 0x148, 0xA8);
    Map_unk221(0x18, 0x138, 0xB0);
    Map_unk222(0x18, 0x3000, 0x14);
    Map_unk123_3();
    Map_unk124_3(0x18, 3);
    Map_unk98(0x14);
    Map_unk223(0x18, 0, 0x14);
    Map_unk224(0, 0x6000, 0);
    Map_unk225(1, 0x4000, 0);
    Map_unk226(2, 0x8000, 0x14);
    Map_unk125_3(3, 2);
    Map_unk99(0x14);
    Map_unk227(3, 0, 0x14);
    Map_unk126_3(0x18, 4);
    Map_unk100(0x14);
    Map_unk228(0x18, 0, 0x14);
    Map_unk229(1, 0x2000, 0);
    Map_unk230(2, 0xA000, 0);
    Map_unk231(3, 0xE000, 0x3C);
    Map_unk232(0x18, 0xB000, 0x14);
    Map_unk233(0, 0xA000, 0);
    Map_unk234(1, 0xC000, 0);
    Map_unk235(2, 0xA000, 0);
    Map_unk236(3, 0xC000, 0x14);
    Map_unk237(0x18, 0, 0x14);
    Map_unk127_3(3, 4);
    Map_unk101(0x14);
    Map_unk238(3, 0, 0x14);
    Map_unk128_3(0x18, 2);
    Map_unk239(0x18, 0x100, 0x3C);
    Map_unk240(0x18, 0x3000, 0x14);
    Map_unk241(0x18, 0, 0x14);
    Map_unk242(3, 0x00000101, 0x3C);
    Map_unk129_3(0x18, 4);
    Map_unk102(0x14);
    Map_unk243(0x18, 0, 0x14);
    Map_unk130_3(3, 2);
    Map_unk103(0x14);
    Map_unk244(3, 0, 0x14);
    Map_unk131_3(0x18, 3);
    Map_unk104(0x14);
    Map_unk245(0x18, 0, 0x14);
    Map_unk132_3(3, 1);
    Map_unk105(0x14);
    Map_unk246(3, 0, 0x14);
    Map_unk247(0x18, 0x138, 0xB8);
    Map_unk248(0x18, 0x3000, 0x14);
    Map_unk249(0x18, 0, 0x14);
    Map_unk133_3(1, 2);
    Map_unk106(0xA);
    Map_unk250(1, 0, 0x14);
    Map_unk22(1, 0);
    if (Map_unk23(0, 0) == 0) {
        Map_unk134_3(1, 2);
        Map_unk251(1, 0, 0x14);
        scene_counter_initial = gWork;
        SCENE_SKIP_COUNT(scene_counter_initial) = (u16)(SCENE_SKIP_COUNT(scene_counter_initial) + 1);
    } else {
        scene_counter_initial_alt = gWork;
        SCENE_SKIP_COUNT(scene_counter_initial_alt) = (u16)(SCENE_SKIP_COUNT(scene_counter_initial_alt) + 1);
        Map_unk135_3(1, 1);
        Map_unk252(1, 0, 0x14);
    }
    Map_unk253(2, 0, 0x14);
    Map_unk254(1, 0xC000, 0x14);
    Map_unk136_3(0x18, 3);
    Map_unk107(0x14);
    Map_unk255(0x18, 0, 0x14);
    Map_unk256(0x18, 0xD000, 0x14);
    Map_unk257(0x18, 0, 0x14);
    Map_unk258(0x18, 0x3000, 0x14);
    Map_unk137_3(0x18, 4);
    Map_unk108(0x14);
    Map_unk259(0x18, 0, 0x14);
    Map_unk260(3, 0x00000101, 0x3C);
    Map_unk261(3, 0, 0x14);
    Map_unk138_3(0x18, 4);
    Map_unk109(0x14);
    Map_unk262(0x18, 0, 0x14);
    Map_unk263(0x18, 0xB000, 0x14);
    Map_unk264(0x18, 0, 0x14);
    Map_unk110(0xA);
    Map_unk139_3(0x18, 2);
    Map_unk111(0x1E);
    Map_unk140_3(0x18, 0);
    Map_unk112(0xA);
    Map_unk141_3(1, 2);
    Map_unk113(0x1E);
    Map_unk142_3(1, 0);
    Map_unk114(0xA);
    Map_unk265(0x18, 0x2000, 0);
    Map_unk115(0x1E);
    Map_unk143_3(0x18, 4);
    Map_unk116(0x14);
    Map_unk144_3(0x18, 0);
    Map_unk117(0x14);
    Map_unk145_3(0x18, 2);
    Map_unk118(0x1E);
    Map_unk146_3(0x18, 0);
    Map_unk119(0xA);
    Map_unk266(2, 0x100, 0x3C);
    Map_unk147_3(2, 0);
    Map_unk120(0xA);
    Map_unk148_3(0x18, 3);
    Map_unk121(0x1E);
    Map_unk149_3(0x18, 0);
    Map_unk122(0x14);
    Map_unk150_3(0x18, 2);
    Map_unk123(0x1E);
    Map_unk151_3(0x18, 0);
    Map_unk124(0xA);
    Map_unk267(0x18, 0x1000, 0);
    Map_unk125(0x1E);
    Map_unk268(0x18, 0x108, 0x3C);
    Map_unk126(0xA);
    Map_unk152_3(0x16, 2);
    Map_unk127(0x1E);
    Map_unk153_3(0x16, 8);
    Map_unk128(0x2D);
    Map_unk154_3(0x16, 1);
    Map_unk24(Map_unk51(0x16), 1);
    Map_unk129(0x28);
    Map_unk269(2, 0x102, 0x3C);
    Map_unk155_3(2, 0);
    Map_unk130(0xA);
    Map_unk270(0x16, 0x2000, 0);
    Map_unk131(0x14);
    Map_unk156_3(0x16, 4);
    Map_unk132(0x1E);
    Map_unk157_3(0x16, 0);
    Map_unk133(0x14);
    Map_unk271(0x18, 0x2000, 0);
    Map_unk134(0x1E);
    Map_unk158_3(0x18, 0);
    Map_unk135(0xA);
    Map_unk159_3(3, 2);
    Map_unk136(0x1E);
    Map_unk160_3(3, 0);
    Map_unk137(0xA);
    Map_unk272(0x18, 0x16, 0);
    Map_unk138(0x23);
    Map_unk161_3(0x16, 3);
    Map_unk139(0x1E);
    Map_unk162_3(0x18, 3);
    Map_unk140(0x14);
    Map_unk273(0x18, 0x148, 0xB0);
    Map_unk274(0x18, 0x3000, 0x14);
    Map_unk163_3(0x18, 5);
    Map_unk164_3(0x16, 7);
    Map_unk141(0x14);
    Map_unk165_3(1, 1);
    Map_unk142(0x14);
    Map_unk166_3(1, 0);
    Map_unk143(0xA);
    Map_unk275(0x18, 0x00000101, 0x3C);
    Map_unk25(0x18, 0);
    Map_unk144(0x1E);
    Map_unk167_3(1, 2);
    Map_unk168_3(2, 2);
    Map_unk169_3(3, 2);
    Map_unk276(1, 0, 0);
    Map_unk277(2, 0xC000, 0);
    Map_unk278(3, 0xE000, 0);
    Map_unk145(0x14);
    if (Map_unk26(0, 0) == 0) {
        Map_unk146(0x1E);
        Map_unk170_3(0x18, 0);
        scene_counter_first_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_first_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_first_a) + 1);
    } else {
        Map_unk147(0x1E);
        /* This branch needs its own workspace load; stale register contents
         * are not a valid C dependency. */
        scene_counter_first_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_first_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_first_b) + 1);
        Map_unk171_3(0x18, 0);
    }
    Map_unk148(0x14);
    Map_unk172_3(0x18, 4);
    Map_unk149(0x14);
    Map_unk279(1, 0xC000, 0);
    Map_unk280(2, 0xA000, 0);
    Map_unk281(3, 0xC000, 0);
    Map_unk150(0x14);
    Map_unk173_3(0x18, 0);
    Map_unk174_3(0x16, 1);
    Map_unk151(0x14);
    Map_unk282(0x16, 0, 0x14);
    Map_unk175_3();
    Map_unk152(0x14);
    Map_unk176_3(0, 2);
    Map_unk177_3(1, 2);
    Map_unk178_3(2, 2);
    Map_unk179_3(3, 2);
    Map_unk283(0, 0, 5);
    Map_unk284(1, 0xE000, 5);
    Map_unk285(2, 0xA000, 5);
    Map_unk286(3, 0x8000, 5);
    Map_unk287(0, 0xE000, 5);
    Map_unk288(1, 0xA000, 5);
    Map_unk289(2, 0x8000, 5);
    Map_unk290(3, 0, 5);
    FIELD(Map_unk180_3(), s8 *, 0x55) = 0;
    Map_unk181_3(0x01180000, -1, 0xE80000, 1);
    Map_unk182_3();
    Map_unk183_3(0x16, 0xF);
    Map_unk184_3(0x18, 0xF);
    Map_unk291(0x16, 0xF00000, 0xD00000);
    Map_unk292(0x18, 0xE80000, 0xD00000);
    Map_unk293(0x16, 0x5000, 0);
    Map_unk294(0x18, 0x3000, 0);
    Map_unk185_3();
    Map_unk295(0, 0x8000, 0xA);
    Map_unk296(1, 0x8000, 0xF);
    Map_unk297(2, 0x8000, 0);
    Map_unk298(3, 0x8000, 0x14);
    Map_unk299(0, 0x148, 0xD0);
    Map_unk300(2, 0x150, 0xE0);
    Map_unk301(1, 0x138, 0xD8);
    Map_unk302(1, 0x8000, 0);
    Map_unk153(2);
    Map_unk303(2, 0x8000, 0);
    Map_unk154(0);
    Map_unk304(0, 0x8000, 0x1E);
    Map_unk305(1, 0, 0x14);
    Map_unk186_3(3, 2);
    Map_unk155(0x14);
    Map_unk306(3, 0, 0x14);
    Map_unk307(1, 0x5000, 0x14);
    Map_unk308(1, 0, 0x14);
    Map_unk309(1, 0x8000, 0x14);
    Map_unk187_3(2, 1);
    Map_unk310(2, 0x6000, 0x14);
    Map_unk311(2, 0, 0x14);
    Map_unk188_3(3, 4);
    Map_unk156(0x14);
    Map_unk312(3, 0, 0x14);
    Map_unk313(2, 0x8000, 0x14);
    Map_unk314(0x18, 0, 0x14);
    Map_unk189_3(0x30000, 0x6000);
    {
        void **scene_system_cell;
        Map_unk190_3(0x980000, -1, 0xD80000, 1);
        scene_system_cell = (void **)(gCam + 0x164);
        FIELD(scene_system_cell, s32 *, 0xC) = 0x03800000;
        scene_system_cell = (void **)&gCam;
        Map_unk191_3();
        Map_unk157(1);
        FIELD(Map_unk27(9), s8 *, 0x55) = 0;
        Map_unk315(9, 0x680000, 0x01080000);
        actor9_fixed_y = 0xffe00000;
        FIELD(Map_unk28(9), s32 *, 0xC) = actor9_fixed_y;
        FIELD(Map_unk52(9), s32 *, 0x3C) = actor9_fixed_y;
        Map_SetRect(0x1D, 0x4A, 4, 0x4A, 5, 4);
        Map_unk192_3(0x11, 0);
        Map_unk193_3(0x12, 0);
        Map_unk194_3();
        Map_unk195_3(0x11, 1);
        Map_unk196_3(0x12, 1);
        Map_unk197_3();
        Map_unk158(0x1E);
        Map_unk316(0x18, 0x8000, 0x14);
        Map_unk317(0x18, 0, 0x14);
        Map_unk198_3(0x01180000, -1, 0xD80000, 1);
        Map_unk199_3();
        Map_unk318(0x18, 0x3000, 0x14);
        Map_unk200_3(0x18, 5);
        Map_unk319(0x18, 0, 0x14);
        Map_unk201_3(2, 2);
        Map_unk159(0x14);
        Map_unk320(2, 0xA000, 0x14);
        Map_unk321(2, 0, 0x14);
        Map_unk202_3(1, 1);
        Map_unk160(0x14);
        Map_unk322(1, 0, 0x14);
        Map_unk323(0x18, 0x00000101, 0x3C);
        Map_unk29(0x18, 0);
        if (Map_unk30(0, 0) == 0) {
            Map_unk161(0x14);
            Map_unk203_3(0x16, 2);
            Map_unk266_2(0x14);
            Map_unk324(0x16, 0, 0x14);
            scene_counter_system_a = FIELD(scene_system_cell, void **, 0x4C);
            SCENE_SKIP_COUNT(scene_counter_system_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_system_a) + 1);
        } else {
            scene_counter_system_b = FIELD(scene_system_cell, void **, 0x4C);
            SCENE_SKIP_COUNT(scene_counter_system_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_system_b) + 1);
            Map_unk162(0x14);
            Map_unk325(0x16, 0, 0x14);
        }
    }
    Map_unk204_3(0x18, 2);
    Map_unk163(0x14);
    Map_unk326(0x18, 0, 0x14);
    Map_unk327(0, 0x00000101, 0);
    Map_unk328(1, 0x00000101, 0);
    Map_unk329(2, 0x00000101, 0);
    Map_unk330(3, 0x00000101, 0x3C);
    Map_unk31(0x18, 0);
    if (Map_unk32(0, 0) == 0) {
        Map_unk164(0x14);
        Map_unk205_3(0x18, 2);
        Map_unk267_2(0x14);
        Map_unk331(0x18, 0, 0x14);
        scene_counter_mid_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_mid_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_mid_a) + 1);
    } else {
        scene_counter_mid_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_mid_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_mid_b) + 1);
        Map_unk165(0x14);
        Map_unk206_2(0x18, 1);
        Map_unk166(0x14);
        Map_unk332(0x18, 0, 0x14);
    }
    Map_unk167(0x14);
    Map_unk207_2();
    Map_unk168(0x14);
    Map_unk208_2(0x800000, -1, 0xC80000, 1);
    Map_unk209_2();
    Map_unk210_2(0x16, 1);
    Map_unk211_2(0x18, 1);
    Map_unk212_2(0x16, 0xF);
    Map_unk213_2(0x18, 0xF);
    Map_unk333(0x16, 0x780000, 0x980000);
    Map_unk334(0x18, 0x700000, 0xA00000);
    Map_unk335(0x16, 0x5000, 0);
    Map_unk336(0x18, 0x3000, 0);
    Map_unk214_2();
    Map_unk169(0x1E);
    Map_unk337(0x18, 0, 0x14);
    Map_unk33(0x18, 0);
    if (Map_unk34(0, 0) == 0) {
        Map_unk215_2(0x18, 3);
        Map_unk170(0x14);
        Map_unk338(0x18, 0, 0x14);
        scene_counter_later_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_later_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_later_a) + 1);
    } else {
        scene_counter_later_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_later_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_later_b) + 1);
        Map_unk216_2(0x18, 4);
        Map_unk171(0x14);
        Map_unk339(0x18, 0, 0x14);
    }
    Map_unk340(0x18, 0, 0x14);
    Map_unk217_2(0x16, 1);
    Map_unk172(0x14);
    Map_unk341(0x16, 0, 0x14);
    Map_unk342(0x16, 0, 0x14);
    Map_unk173(0x14);
    Map_unk343(0x18, 0, 0x14);
    Map_unk218_2(0x11, 0);
    Map_unk219_2(0x12, 0);
    Map_unk220_2();
    Map_unk221_2(0x11, 1);
    Map_unk222_2(0x12, 1);
    Map_unk174(0x11);
    Map_unk223_2(0x01500000, -1, 0xD80000, 1);
    Map_unk224_2();
    Map_unk225_2(2, 4);
    Map_unk175(0x14);
    Map_unk226_2();
    Map_unk344(2, 0, 0x14);
    Map_unk345(0, 0x4000, 0);
    Map_unk346(1, 0x3000, 0x16);
    Map_unk227_2(1, 2);
    Map_unk176(0xA);
    Map_unk347(1, 0, 0x14);
    Map_unk348(3, 0x00000105, 0x3C);
    Map_unk349(3, 0, 0x14);
    Map_unk350(3, 0xA000, 0x14);
    Map_unk351(3, 0, 0x14);
    Map_unk352(3, 0x8000, 0x14);
    Map_unk228_2(3, 0x10);
    FIELD(Map_unk35(3), s32 *, 0x18) = (s32) 0xffff0000;
    Map_unk177(0x14);
    Map_unk353(3, 0, 0x28);
    Map_unk229_2(2, 2);
    Map_unk178(0x14);
    Map_unk354(2, 0xA000, 0x14);
    Map_unk230_2(2, 4);
    Map_unk179(0x14);
    Map_unk36(2, 0);
    Map_unk231_2(3, 1);
    FIELD(Map_unk37(3), s32 *, 0x18) = 0x10000;
    Map_unk355(0, 0x4000, 0);
    Map_unk356(1, 0xE000, 0);
    Map_unk357(3, 0xC000, 0x14);
    if (Map_unk38(0, 0) == 0) {
        Map_unk232_2(2, 3);
        Map_unk180(0x14);
        Map_unk358(2, 0, 0x14);
        scene_counter_final_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_final_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_final_a) + 1);
    } else {
        scene_counter_final_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_final_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_final_b) + 1);
        Map_unk233_2(2, 2);
        Map_unk181(0x14);
        Map_unk359(2, 0, 0x14);
    }
    Map_unk360(2, 0x6000, 0x14);
    Map_unk234_2(2, 3);
    Map_unk182(0x14);
    Map_unk361(2, 0, 0x14);
    Map_unk362(3, 0xE000, 0x1E);
    Map_unk363(1, 0x2000, 0x14);
    Map_unk235_2(1, 1);
    Map_unk183(0xA);
    Map_unk364(1, 0, 0x14);
    Map_unk365(3, 0xA000, 0x14);
    Map_unk236_2(3, 3);
    Map_unk184(0x14);
    Map_unk366(3, 0, 0x14);
    Map_unk237_2(3, 1);
    Map_unk367(3, 0, 0x14);
    Map_unk368(0, 0x00000101, 0);
    Map_unk369(1, 0x00000101, 0);
    Map_unk370(2, 0x00000101, 0x3C);
    Map_unk371(3, 0x0000cccc, 0x00006666);
    Map_unk372(1, 0x0000cccc, 0x00006666);
    Map_unk373(3, 0x148, 0xD8);
    Map_unk374(2, 0xA000, 0);
    actor_one_record = Map_unk39(1);
    FIELD(actor_one_record, u8 *, 0x5A) = (u8)(0xFE & FIELD(actor_one_record, u8 *, 0x5A));
    Map_unk375(1, 0x138, 0xC8);
    Map_unk376(3, 0x118, 0xD8);
    Map_unk185(1);
    Map_unk377(0, 0x6000, 0);
    Map_unk378(1, 0x6000, 0);
    Map_unk186(3);
    Map_unk238_2(3, 2);
    Map_unk187(0x1E);
    Map_unk379(3, 0, 0x14);
    Map_unk380(3, 0, 0x14);
    Map_unk239_2(1, 1);
    Map_unk188(0x14);
    Map_unk381(1, 0, 0x14);
    Map_unk240_2(3, 3);
    Map_unk241_2(0, 2);
    Map_unk242_2(2, 2);
    Map_unk243_2(1, 2);
    Map_unk244_2(0, 0x102);
    Map_unk245_2(1, 0x102);
    Map_unk246_2(2, 0x102);
    Map_unk189(0x3C);
    Map_unk382(2, 0, 0x14);
    Map_unk247_2(3, 4);
    Map_unk190(0x14);
    Map_unk383(3, 0, 0x14);
    Map_unk384(3, 0xE000, 0x14);
    Map_unk385(3, 0, 0x14);
    Map_unk386(3, 0, 0x14);
    Map_unk248_2(3, 4);
    Map_unk191(0x14);
    Map_unk387(3, 0, 0x1E);
    Map_unk388(1, 0x2000, 0);
    Map_unk389(2, 0xA000, 0x14);
    Map_unk249_2(0, 3);
    Map_unk250_2(1, 3);
    Map_unk251_2(2, 3);
    Map_unk192(0x1E);
    Map_unk390(0, 0x6000, 0);
    Map_unk391(1, 0x6000, 0);
    Map_unk392(2, 0x6000, 0x28);
    Map_unk252_2(0, 3);
    Map_unk253_2(1, 3);
    Map_unk254_2(2, 3);
    Map_unk193(0x14);
    Map_unk255_2(3, 3);
    Map_unk194(0x14);
    Map_unk393(3, 0, 0x14);
    Map_unk394(0, 0x138, 0xD8);
    Map_unk395(1, 0x138, 0xD8);
    Map_unk396(2, 0x138, 0xD8);
    Map_unk397(3, 0x138, 0xD8);
    Map_unk195(0);
    Map_unk196(1);
    Map_unk398(1, 0, 0);
    Map_unk197(2);
    Map_unk399(2, 0, 0);
    Map_unk198(3);
    Map_unk400(3, 0, 0);
    {
        void **record;
        record = (void **)(gCam + 0x164);
        FIELD(record, s32 *, 0xC) = 0x04000000;
    }
    Map_unk256_2();
    Map_unk199(1);
    Map_unk2_5(4, 0x46, 4, 0x4A, 5, 4);
    Map_unk200(0x880);
    Map_unk201(0x00000881);
    Map_unk257_2();
}

/* map/locations/heidia/village_story/village_story_scene/scene_data.c */
extern u8 gOv[];

void *SceneData_GetTableB938(void)
{
    return gOv;
}

/* map/locations/heidia/village_story/village_story_scene/scene_primary_script.c */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define ACTOR_FIELD_0XA(record) (*(s16 *)((record) + 10))
#define ACTOR_FIELD_0X12(record) (*(s16 *)((record) + 18))

extern u8 gOv2[];
extern u8 gWork[];

u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_3();
u8 *Scene_GetRecord_4();

u8 *Scene_GetRecord_6();

u8 *Scene_GetRecord_7();

u8 *Scene_GetRecord_8();

u8 *Scene_GetRecord_9();

u8 *Scene_GetRecord_10();

u8 *Scene_GetRecord_11();

u8 *Scene_GetRecord_5();

/* Newly named call sites: the mechanical pass left these spelled raw because
 * their calling form (a cast, or an odd argument count) did not match its
 * patterns, or because the engine function itself had no name yet. */

/* Status/phase word at 0x1c0 of the shared scene work record. */

/* Two s16 fields (offsets 0xa and 0x12) read together from an actor slot
 * record and forwarded as a pair of arguments. */

/* Configures four actor slots (0-3), advances the shared scene phase, then
 * drives handles 1-3 through a sequence of timed calls, some of which use
 * the two s16 fields read back from an actor slot record. */
void Scene_RunFourActorPresentation(void)
{
    u32 i;
    u8 *record;

    Battle_Reset_1();
    Map_unk268_2(); /* main:08077268 */
    record = Scene_GetRecord_1(0); /* main:0808a080 */
    Map_unk269_2(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_2(1); /* main:0808a080 */
    Map_unk270_2(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_3(2); /* main:0808a080 */
    Map_unk271_2(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_4(3); /* main:0808a080 */
    Map_unk272_2(record, 0); /* main:080091e0 */
    Motion_CamBounds_1(0x1300000, -1, 0x780000, 0);
    Map_unk273_2(1); /* main:080000c0 */
    Map_unk274_2(); /* main:08009128 */
    Map_unk275_2(1); /* main:080000c0 */
    Audio_PlayCue_1(141);
    Map_unk401(0x50000, 0x50000, 0x10000); /* main:080091f0 */
    Audio_PlayCue_2(0x121);
    Map_unk402(-1, -1, 0xe666); /* main:080091f0 */
    SCENE_PHASE = 0x100;
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(); /* main:0808a370 */
    Map_unk276_2(); /* main:080091f8 */
    Battle_WaitMode0_2(30);
    /* Clear the byte at offset 85 of the record RuntimeBlock_GetOffset1e0Pointer_1() returns. */
    *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = 0;
    Motion_SetSpeedLim_1(0xcccc, 0x1999); /* speed_limit, acceleration */
    Motion_CamBounds_2(0x2000000, -0x180000, 0xa00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleFx_ApplyColorToSourceBuffer_1(0x10000, 0);
    Battle_WaitMode0_3(0x10005, 0); /* main:0808a330 */
    Battle_WaitMode0_4(50); /* main:0808a348 */
    Battle_WaitMode0_5(50);
    Battle_WaitMode0_6(0x7fff, 0); /* main:0808a330 */
    Battle_WaitMode0_7(30); /* main:0808a348 */
    Battle_WaitMode0_8(30);
    Motion_SetHPosTerrain_1(0, 0x1f80000, 0xa80000);
    Motion_SetHPosTerrain_2(1, 0x2100000, 0x900000);
    Motion_SetHPosTerrain_3(2, 0x1e80000, 0x900000);
    Motion_SetHPosTerrain_4(3, 0x2000000, 0x980000);
    Object_SetModeById_12(0, 19); /* object 0, action 19 */
    Object_SetModeById_1(1, 19);
    Object_SetModeById_2(2, 19);
    Object_SetModeById_3(3, 19);
    Battle_WaitMode0_9(10);
    Battle_WaitMode0_10(0x10000, 0); /* main:0808a330 */
    Battle_WaitMode0_11(30); /* main:0808a348 */
    Battle_WaitMode0_12(30);
    Battle_WaitMode0_13(80);
    record = Scene_GetRecord_5(0);
    Map_unk277_2(record, 1); /* main:080091e0 */
    Object_SetModeById_4(0, 1);
    Battle_WaitMode0_14(30);
    Motion_CallWaitAnim_1(0, 4);
    Motion_ArmCb_1(0, 0xc000, 20);
    Motion_SetVarCb_1(0, 2);
    Battle_WaitMode0_15(60);
    record = Scene_GetRecord_6(1); /* main:0808a080 */
    Map_unk278_2(record, 1); /* main:080091e0 */
    Object_SetModeById_5(1, 1);
    Battle_WaitMode0_16(20);
    Motion_ArmCb_2(1, 0x2000, 0);
    Battle_WaitMode0_17(20);
    Motion_ArmCb_3(1, 0x6000, 0);
    Battle_WaitMode0_18(20);
    Motion_ArmCb_4(1, 0, 0);
    record = Scene_GetRecord_7(2);
    Map_unk279_2(record, 1); /* main:080091e0 */
    Object_SetModeById_6(2, 1);
    Motion_ArmCb_5(1, 0x6000, 0);
    Battle_WaitMode0_19(40);
    record = Scene_GetRecord_8(3); /* main:0808a080 */
    Map_unk280_2(record, 1); /* main:080091e0 */
    Object_SetModeById_7(3, 1);
    Battle_WaitMode0_20(20);
    Motion_CallWaitAnim_2(3, 3);
    Battle_WaitMode0_21(20);
    Motion_SetSpeed_2(1, 0xcccc, 0x6666);
    Motion_SetSpeed_3(2, 0xcccc, 0x6666);
    Motion_SetSpeed_4(3, 0xcccc, 0x6666);
    Object_SetModeById_8(1, 2);
    Object_SetModeById_9(2, 2);
    Object_SetModeById_10(3, 2);
    record = Scene_GetRecord_9(0); /* main:0808a080 */
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_10(0); /* main:0808a080 */
    if (record != 0) {
        Motion_ResetAndSetPosition_2(2, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_11(0); /* main:0808a080 */
    if (record != 0) {
        Motion_ResetAndSetPosition_3(3, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    Motion_CommitPos_1(3);
    Motion_SetHPosTerrain_5(3, 0, 0);
    Motion_CommitPos_2(2);
    Motion_SetHPosTerrain_6(2, 0, 0);
    Motion_CommitPos_3(1);
    Motion_SetHPosTerrain_7(1, 0, 0);
    Battle_SchedShoulder_1();
}

void Scene_RunScene39d(void)
{
    u32 i;
    u8 *rec;
    u8 *rec8;
    s32 record;
    s32 base5_200bc50;

    rec = Map_unk40(0);
    Map_unk281_2();
    record = Map_unk53();
    base5_200bc50 = (s32)gOv2;
    *(s32 *)base5_200bc50 = record;
    if (record != 0) {
        Map_unk202(0x250);
        rec8 = Map_unk41(*(s32 *)base5_200bc50);
        rec8[85] = 0;
        rec[85] &= 254;
        *(s32 *)((s32)rec8 + 12) += -0x30000;
        *(s32 *)((s32)rec + 12) += -0x30000;
        *(s32 *)((s32)rec + 20) += -0x30000;
        Map_unk282_2(2);
        *(s32 *)((s32)rec8 + 12) += -0x20000;
        *(s32 *)((s32)rec + 12) += -0x20000;
        *(s32 *)((s32)rec + 20) += -0x20000;
        Map_unk283_2(10);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Map_unk284_2(4);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Map_unk285_2(4);
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        *(s32 *)((s32)rec + 12) += 0x10000;
        *(s32 *)((s32)rec + 20) += 0x10000;
    }
    Map_unk286_2();
}

void Scene_RunScene39d(void)
{
    u32 i;
    s32 record;
    u8 *p5;
    u8 *work;

    p5 = *(volatile s32 *)0x03001e70;
    work = p5 + 0x164;
    Map_unk287_2();
    *(volatile s32 *)(work + 12) = 0x3800000;
    Map_unk288_2();
    Map_unk289_2(1);
    *(u8 *)(Map_unk54(9) + 85) = 0;
    Map_unk403(9, 0x680000, 0x1080000);
    record = Map_unk55(9);
    *(volatile s32 *)(record + 12) = -0x200000;
    record = Map_unk42(9);
    *(volatile s32 *)(record + 60) = -0x200000;
    *(u8 *)(Map_unk56() + 85) = 0;
    Map_unk258_2(0xcccc, 0x1999);
    Map_unk259_2(0x800000, -1, 0xb80000, 1);
    Map_unk290_2();
    Map_unk291_2(30);
    Map_unk3_5(29, 74, 4, 74, 5, 4);
    Map_unk292_2(17, 0);
    Map_unk293_2(18, 0);
    Map_unk294_2();
    Map_unk295_2(17, 1);
    Map_unk296_2(18, 1);
    Map_unk297_2(20);
    Map_unk203(0x251);
    Map_unk298_2();
}

void Scene_RunScene39d(void)
{
    u32 i;
    s32 record;

    Map_unk299_2();
    Map_unk404(0, 0x8000, 0x4000);
    Map_unk300_2(0, 104, 152);
    Map_unk405(0, 0x4000, 60);
    Map_unk301_2(17, 0);
    Map_unk302_2(18, 0);
    Map_unk303_2();
    Map_unk260_2(-1, -1, -1, 0);
    Map_unk304_2(1);
    Map_unk305_2();
}

/* map/locations/heidia/village_story/village_story_scene/shared.c */
s32 get_runtime_default_result(void)
{
    return 0;
}

/* map/locations/heidia/village_story/village_story_scene/village_story.c */
/*
 * resource_39d owner at 0x02000ad0, 82 bytes.
 *
 * Sets one byte on seven records in a row. Each record is fetched by id through
 * its own per-site veneer -- seven calls, seven veneers, as every other owner in
 * this overlay does -- and the byte at +85 is written immediately after.
 *
 * The ids are 0 and then 14 through 19, and the first record takes 3 where the
 * rest take 4. Nothing in the span says what the byte means, so it keeps an
 * offset-derived name.
 */
struct Record {
    u8 pad00[85];
    u8 mode55;              /* +85, 0x55 */
};

extern u8 gOv3[];
extern s16 gCell[];
extern u8 gOv4[];
extern u8 gOv5[];

struct Record *Map_unk307_2();
struct Record *Map_unk308_2();
struct Record *Map_unk309_2();
struct Record *Map_unk310_2();
struct Record *Map_unk311_2();
struct Record *Map_unk312_2();
struct Record *Map_unk313_2();

/* Contiguous unnamed leaf-owner run for resource_39d. */

/* Configure and publish the scene's presentation layer. */

void *SceneData_GetTableb9c8(void)
{
    return gOv3;
}

void *SceneData_GetTableB9d4AfterStateCheck(void)
{
    if (gCell[225] != 1) {
        Map_unk204(0x253);
    }
    return gOv4;
}

void *SceneData_GetTablebbe4(void)
{
    return gOv5;
}

void Actor_SetMode55OnSevenRecords(void)
{
    Map_unk307_2(0)->mode55 = 3;
    Map_unk308_2(14)->mode55 = 4;
    Map_unk309_2(15)->mode55 = 4;
    Map_unk310_2(16)->mode55 = 4;
    Map_unk311_2(17)->mode55 = 4;
    Map_unk312_2(18)->mode55 = 4;
    Map_unk313_2(19)->mode55 = 4;
}

void Scene_ConfigureValue93Scene(void)
{
    Map_Apply(93, 1);
    Map_Apply2(24, 9);
    Map_unk314_2();
    Map_unk205(1);
    Map_unk315_2();
    Map_unk316_2();
}
