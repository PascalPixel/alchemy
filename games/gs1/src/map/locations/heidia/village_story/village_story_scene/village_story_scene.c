#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village_story/village_story_scene/village_story_scene.h"
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

void Map_Run261(u8 *work)
{
    s32 parent;
    s32 phase;
    s32 next;
    s32 x;
    s32 y;

    parent = *(s32 *)(work + 104);
    phase = *(u16 *)(work + 100);
    x = Map_Check43(phase);
    *(volatile s32 *)(work + 8) = (*(s32 *)(parent + 8) + (x *(*(s32 *)(work + 48) + 28)));
    y = Map_Check44(phase);
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

extern u8 *Map_Run262();

extern u8 *Map_Run263();

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
    Map_Place2(1, 0x0000cccc, 0x00006666);
    Map_Place3(2, 0x0000cccc, 0x00006666);
    Map_Place4(3, 0x0000cccc, 0x00006666);
    temp_r0_2 = Map_Check2(0);
    if (temp_r0_2 != 0) {
        Map_Place5(1, FIELD(temp_r0_2, s32 *, 8), FIELD(temp_r0_2, s32 *, 0x10));
    }
    temp_r0_3 = Map_Check3(0);
    if (temp_r0_3 != 0) {
        Map_Place6(2, FIELD(temp_r0_3, s32 *, 8), FIELD(temp_r0_3, s32 *, 0x10));
    }
    temp_r0_4 = Map_Check4(0);
    if (temp_r0_4 != 0) {
        Map_Place7(3, FIELD(temp_r0_4, s32 *, 8), FIELD(temp_r0_4, s32 *, 0x10));
    }
    Map_Do(1);
    Map_Place8(0, 0x158, 0xE8);
    Map_Place9(1, 0x148, 0xE8);
    Map_Place10(2, 0x158, 0xF8);
    Map_Place11(3, 0x148, 0xF8);
    Map_Do2(0);
    Map_Place12(0, 0xA000, 0);
    Map_Do3(1);
    Map_Place13(1, 0xC000, 0);
    Map_Do4(2);
    Map_Place14(2, 0xA000, 0);
    Map_Do5(3);
    Map_Place15(3, 0xC000, 0);
    Map_Do6(0x32);
    Map_Run2(1, 2);
    Map_Do7(0x14);
    Map_Run3(0x18000, 0x3000);
    Map_Run4(0x01480000, 0x280000, 0xB00000, 1);
    FIELD(Map_Run5(), s8 *, 0x55) = 0;
    Map_Place16(1, 0x18000, 0xC000);
    Map_Place17(1, 0x148, 0xD8);
    Map_Run6();
    Map_Place18(1, 0x0000cccc, 0x00006666);
    Map_Do8(0x3C);
    Map_Run7(0x01580000, 0x180000, 0xE80000, 1);
    Map_Place19(1, 0x2000, 0x14);
    Map_Run8();
    Map_Run9(1, 4);
    Map_Do9(SCENE_STEP_VALUE);
    Map_Place20(1, 0, 0x14);
    Map_Run10(3, 0x102);
    Map_Do10(0x3C);
    Map_Place21(3, 0x18000, 0xC000);
    Map_Place22(3, 0x148, 0xE8);
    Map_Do11(0xA);
    Map_Place23(3, 0, 0x14);
    Map_Run11(3, 4);
    Map_Do12(0x14);
    Map_Place24(3, 0, 0x1E);
    Map_Place25(2, 0, 0);
    Map_Do13(0x3C);
    Map_Place26(2, 0x150, 0xF8);
    Map_Place27(2, 0xA000, 0);
    Map_Place28(0, 0x8000, 0);
    Map_Do14(0x1E);
    Map_Place29(2, 0, 0x14);
    Map_Place30(3, 0, 0x1E);
    Map_Check5(3, 0);
    if (Map_Check6(0, 0) == 0) {
        Map_Do15(0x14);
        Map_Place31(3, 0x100, 0x3C);
        Map_Place32(1, 0x4000, 0xA);
        Map_Run12(1, 3);
        Map_Place33(1, 0, 0x14);
        Map_Place34(3, 0xC000, 0);
        Map_Run13(1, 4);
        Map_Do16(0x14);
        Map_Place35(1, 0, 0x14);
        temp_r2 = gWork;
        FIELD(temp_r2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2, u16 *, 0x1D8) + 2);
    } else {
        temp_r2_2 = gWork;
        FIELD(temp_r2_2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_2, u16 *, 0x1D8) + 2);
        Map_Do17(0x14);
        Map_Place36(3, 0x00000101, 0x3C);
        Map_Place37(1, 0x4000, 0xA);
        Map_Run14(1, 4);
        Map_Place38(1, 0, 0x14);
        Map_Place39(3, 0xC000, 0);
        Map_Run15(1, 2);
        Map_Do18(0x14);
        Map_Place40(1, 0, 0x14);
    }
    Map_Place41(3, 0x00000101, 0x3C);
    Map_Place42(3, 0, 0x14);
    Map_Place43(0, 1, 0);
    Map_Do19(0x3C);
    Map_Place44(0, 0x8000, 0);
    Map_Place45(1, 0x4000, 0x14);
    Map_Run16(0, 3);
    Map_Run17(1, 3);
    Map_Do20(0x14);
    Map_Place46(1, 0, 0x14);
    Map_Run18(3, 2);
    Map_Place47(3, 0, 0x14);
    Map_Run19(0, 3);
    Map_Run20(1, 3);
    Map_Run21(2, 3);
    Map_Do21(0x14);
    Map_Run22(3, 4);
    Map_Do22(0xA);
    Map_Place48(3, 0, 0xA);
    Map_Run23(3, 0x10);
    Map_Place49(3, 0, 0x3C);
    Map_Do23(0x11);
    Map_Place50(5, 0xD80000, 0xC80000);
    Map_Place51(5, 0, 0x14);
    Map_Place52(5, 0x780000, 0xA00000);
    Map_Place53(5, 0, 0);
    Map_Run24(3, 1);
    Map_Run25(0, 1);
    Map_Run26(1, 1);
    Map_Place54(0, 0, 0xA);
    Map_Place55(1, 0xE000, 0x14);
    Map_Place56(2, 0xA000, 0xA);
    Map_Place57(3, 0x8000, 0x14);
    Map_Place58(0, 0xA000, 5);
    Map_Place59(1, 0xC000, 0xA);
    Map_Place60(2, 0x8000, 5);
    Map_Place61(0, 0x8000, 5);
    Map_Run27(0, 2);
    Map_Place62(1, 0x8000, 0xA);
    Map_Run28(1, 2);
    Map_Do24(0x14);
    Map_Run29(0x30000, 0x6000);
    Map_Run30(0x780000, 0xffe80000, 0xA80000, 1);
    Map_Run31();
    Map_Do25(0x28);
    Map_Run32(0x15, 1);
    Map_Do26(0x14);
    Map_Do27(0x3D);
    Map_Place63(0x15, 0, 0x14);
    Map_Run33(0x17, 1);
    Map_Do28(0x14);
    Map_Place64(0x17, 0, 0x14);
    Map_Place65(0x17, 0xC000, 0x3C);
    Map_Place66(0x17, 0, 0x14);
    Map_Place67(0x17, 0, 0x14);
    Map_Place68(1, 0x01080000, 0x01200000);
    Map_Place69(1, 0, 0x14);
    Map_Place70(1, 0x01480000, 0xD80000);
    Map_Run34(0x17, 4);
    Map_Do29(0x14);
    Map_Place71(0x17, 0, 0x14);
    Map_Run35(0x17, 3);
    Map_Do30(0x14);
    Map_Place72(0x17, 0, 0x14);
    Map_Place73(0x17, 0xA000, 0xA);
    Map_Place74(0x15, 0x2000, 0xA);
    Map_Run36(0x15, 1);
    Map_Do31(0x14);
    Map_Place75(5, 0x8000, 0);
    Map_Place76(0x14, 0x8000, 0x1E);
    Map_Place77(0x15, 0, 0x1E);
    Map_Run37(5, 2);
    Map_Run38(0x15, 4);
    Map_Do32(0x14);
    Map_Place78(0x15, 0x2000, 0xA);
    Map_Run39(0x15, 3);
    Map_Do33(0x14);
    Map_Run40(0x17, 3);
    Map_Place79(0x15, 0x0000cccc, 0x00006666);
    Map_Place80(0x15, 0x68, 0xA8);
    Map_Do34(0x14);
    Map_Place81(0x17, 0, 0x14);
    Map_Place82(0x14, 0, 0xA);
    Map_Place83(5, 0x102, 0);
    Map_Place84(0x14, 0x102, 0x46);
    Map_Place85(0x16, 0x01080000, 0x01200000);
    Map_Place86(0x16, 0, 0x14);
    Map_Place87(0x16, 0x01280000, 0x780000);
    Map_Place88(0, 0xA000, 0);
    Map_Place89(1, 0xC000, 0);
    Map_Place90(2, 0xC000, 0);
    Map_Place91(3, 0xC000, 0);
    Map_Place92(5, 0, 0);
    Map_Place93(0x14, 0, 0);
    Map_Run41(0x18000, 0x3000);
    Map_Run42(0xE80000, 0x280000, 0x980000, 1);
    Map_Place94(0x16, 0x0000cccc, 0x00006666);
    Map_Place95(0x16, 0x110, 0x80);
    Map_Place96(0x16, 0x108, 0x98);
    Map_Place97(0x16, 0x118, 0xA8);
    Map_Run43();
    Map_Place98(0x16, 0x5000, 0x14);
    Map_Do35(0x14);
    Map_Run44(0x17, 0);
    Map_Do36(0x14);
    Map_Run45(0x16, 3);
    Map_Do37(0x14);
    Map_Run46(0x16, 0);
    Map_Run47(0x01280000, 0x280000, 0xD80000, 1);
    Map_Place99(0x16, 0x130, 0xB0);
    Map_Place100(0x16, 0x2000, 0);
    Map_Run48();
    Map_Do38(0x14);
    Map_Run49(0, 1);
    Map_Run50(1, 1);
    Map_Run51(2, 1);
    Map_Run52(3, 1);
    Map_Do39(0x14);
    Map_Do40(0x14);
    Map_Run53(0x16, 3);
    Map_Do41(0x14);
    Map_Run54(0x16, 0);
    Map_Do42(0xA);
    Map_Place101(0, 0x00000107, 0);
    Map_Place102(1, 0x00000107, 0);
    Map_Place103(3, 0x00000107, 0);
    Map_Place104(2, 0x00000107, 0x46);
    Map_Run55(0x16, 1);
    Map_Do43(0x1E);
    Map_Run56(0x16, 0);
    Map_Do44(0xA);
    Map_Place105(0, 0x00000105, 0);
    Map_Place106(1, 0x00000105, 0);
    Map_Place107(2, 0x00000105, 0);
    Map_Place108(3, 0x00000105, 0x46);
    Map_Run57(0x16, 3);
    Map_Do45(0x14);
    Map_Run58(0x16, 0);
    Map_Do46(0x14);
    Map_Run59(0x17, 0);
    Map_Do47(0xA);
    Map_Place109(0x16, 0x5000, 0);
    Map_Do48(0x28);
    Map_Run60(0x16, 0);
    Map_Do49(0x1E);
    Map_Run61(0x17, 0);
    Map_Do50(0xA);
    Map_Place110(0x16, 0x3000, 0);
    Map_Do51(0x1E);
    Map_Run62(0x16, 0);
    Map_Do52(0x14);
    temp_r0_5 = Map_Check7(1);
    FIELD(temp_r0_5, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_5, u8 *, 0x5A));
    Map_Place111(1, 0x148, 0xE0);
    Map_Place112(0, 0x158, 0xE0);
    Map_Place113(2, 0x158, 0xE8);
    Map_Do53(1);
    flag = 1;
    temp_r0_6 = Map_Check8(1);
    bits = FIELD(temp_r0_6, u8 *, 0x5A) | flag;
    FIELD(temp_r0_6, u8 *, 0x5A) = bits;
    Map_Place114(1, 0xC000, 0);
    Map_Do54(0);
    Map_Place115(0, 0xA000, 0);
    Map_Do55(2);
    Map_Place116(2, 0xC000, 0);
    Map_Do56(0x1E);
    Map_Place117(0x17, 0xA80000, 0xC80000);
    Map_Place118(0x17, 0, 0x14);
    Map_Place119(0x17, 0x680000, 0xC80000);
    Map_Run63(0x16, 1);
    Map_Do57(0x14);
    Map_Place120(0x16, 0, 0x14);
    Map_Run64(0x30000, 0x6000);
    Map_Run65(0x780000, 0xffe80000, 0xA80000, 1);
    Map_Run66();
    Map_Run67(0x17, 3);
    Map_Do58(0xA);
    Map_Place121(0x17, 0xD000, 0x14);
    Map_Run68(0x17, 3);
    Map_Do59(0x14);
    Map_Place122(0x17, 0, 0x14);
    Map_Place123(5, 0x5000, 0);
    Map_Place124(0x14, 0x3000, 0x46);
    Map_Place125(5, 0x14, 0);
    Map_Do60(0x32);
    Map_Place126(5, 0x5000, 0);
    Map_Place127(0x14, 0x3000, 0x14);
    Map_Run69(5, 4);
    Map_Do61(0x14);
    Map_Place128(0x17, 0x100, 0x3C);
    Map_Place129(0x17, 0, 0x14);
    Map_Place130(5, 0x00000105, 0x3C);
    Map_Place131(0x15, 0xD000, 0x14);
    Map_Run70(0x15, 1);
    Map_Run71(0x17, 3);
    Map_Do62(0x14);
    Map_Place132(0x17, 0, 0x14);
    Map_Run72(0x17, 4);
    Map_Do63(0x14);
    Map_Place133(0x17, 0, 0x14);
    Map_Run73(0x15, 0x102);
    Map_Do64(0x3C);
    Map_Run74(0x15, 4);
    Map_Do65(0x14);
    Map_Place134(0x15, 0, 0x14);
    Map_Run75(5, 0x102);
    Map_Do66(0x3C);
    Map_Place135(5, 0, 0x28);
    Map_Run76(0x15, 3);
    Map_Do67(0x3C);
    Map_Place136(5, 0xA000, 0xA);
    Map_Run77(0x14, 3);
    Map_Do68(0x14);
    Map_Place137(0x14, 0, 0x1E);
    Map_Place138(5, 0x0000b333, 0x00005999);
    Map_Place139(0x14, 0x0000b333, 0x00005999);
    Map_Place140(5, 0x80, 0x90);
    Map_Place141(0x14, 0x78, 0x88);
    Map_Place142(0x14, 0x5000, 0);
    Map_Do69(5);
    Map_Place143(5, 0, 0x14);
    Map_Place144(0x15, 0x3000, 0x14);
    temp_r0_7 = Map_Check45(0x15);
    FIELD(temp_r0_7, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_7, u8 *, 0x5A));
    Map_Place145(0x15, 0x58, 0x98);
    temp_r0_8 = Map_Check9(0x15);
    bits = FIELD(temp_r0_8, u8 *, 0x5A) | flag;
    FIELD(temp_r0_8, u8 *, 0x5A) = bits;
    Map_Place146(0x17, 0xB000, 0x14);
    Map_Run78(0x17, 3);
    Map_Run79(0x15, 3);
    Map_Do70(0x28);
    Map_Place147(0x17, 0x30000, 0x20000);
    FIELD(Map_Check10(0x17), s32 *, 0x28) = 0x40000;
    Map_Do71(0x98);
    temp_r0_9 = Map_Check11(0x17);
    FIELD(temp_r0_9, u8 *, 0x55) = (u8)(0x7E & FIELD(temp_r0_9, u8 *, 0x55));
    Map_Run80(Map_Check46(0x17), 0);
    FIELD(Map_Check47(0x11), s8 *, 0x55) = 4;
    Map_Place148(0x17, 0x68, 0xA8);
    Map_Run81(Map_Check48(0x17), 1);
    FIELD(Map_Check12(0x17), s8 *, 0x55) = 3;
    Map_Place149(0x17, 0, 0x1E);
    Map_Place150(0x15, 0, 0xA);
    Map_Place151(0x15, 0, 0x14);
    Map_Run82(5, 2);
    Map_Run83(5, 4);
    Map_Do72(0x14);
    Map_Place152(5, 0, 0x14);
    Map_Place153(0x17, 0, 0x14);
    Map_Run84(0x11, 0);
    Map_Run85(0x12, 0);
    Map_Run86();
    Map_Run87(0x11, 1);
    Map_Run88(0x12, 1);
    Map_Place154(0, 0x6000, 0);
    Map_Place155(1, 0x6000, 0);
    Map_Place156(2, 0x6000, 0);
    Map_Place157(3, 0x6000, 0);
    Map_Run89(0x01300000, 0x200000, 0xD80000, 1);
    Map_Run90();
    Map_Place158(0x14, 0x01100000, 0x01180000);
    Map_Place159(0x14, 0, 0x14);
    Map_Place160(0x14, 0, 0);
    Map_Run91(1, 1);
    Map_Do73(0x14);
    Map_Place161(1, 0, 0x14);
    Map_Place162(1, 0x18000, 0xC000);
    Map_Place163(1, 0x138, 0xD8);
    Map_Place164(1, 0x6000, 0xA);
    Map_Place165(0x16, 0, 0x14);
    Map_Place166(0, 0xA000, 0);
    Map_Place167(1, 0xC000, 0);
    Map_Place168(2, 0xC000, 0);
    Map_Place169(3, 0xC000, 0);
    Map_Place170(0x16, 0x138, 0xB8);
    Map_Place171(0x16, 0x3000, 0x14);
    Map_Place172(3, 0, 0x14);
    Map_Place173(1, 0x100, 0x3C);
    Map_Place174(1, 0x20000, 0x10000);
    temp_r0_10 = Map_Check13(1);
    FIELD(temp_r0_10, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_10, u8 *, 0x5A));
    Map_Place175(1, 0x148, 0xE0);
    Map_Do74(1);
    temp_r0_11 = Map_Check14(1);
    flag |= FIELD(temp_r0_11, u8 *, 0x5A);
    FIELD(temp_r0_11, u8 *, 0x5A) = flag;
    Map_Run92(0, 1);
    Map_Run93(1, 1);
    Map_Run94(2, 1);
    Map_Run95(3, 1);
    Map_Do75(0x14);
    Map_Run96(0x16, 1);
    Map_Do76(0xA);
    Map_Place176(0x16, 0, 0x14);
    Map_Run97(1, 2);
    Map_Do77(0xA);
    Map_Check15(1, 4);
    Map_Do78(0xA);
    Map_Place177(1, 0, 0x14);
    Map_Run98(2, 1);
    Map_Do79(0xA);
    Map_Run99(2, 3);
    Map_Do80(0x14);
    Map_Place178(2, 0, 0x14);
    Map_Place179(0x16, 0x00000101, 0x3C);
    Map_Check16(0x16, 0);
    if (Map_Check17(0, 0) == 0) {
        Map_Do81(0x14);
        Map_Run100(0x16, 4);
        Map_Check18(0x14);
        Map_Place180(0x16, 0, 0x14);
        temp_r2_3 = gWork;
        FIELD(temp_r2_3, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_3, u16 *, 0x1D8) + 1);
    } else {
        Map_Do82(0x14);
        Map_Run101(0x16, 4);
        Map_Do83(0x14);
        temp_r2_4 = gWork;
        FIELD(temp_r2_4, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_4, u16 *, 0x1D8) + 1);
        Map_Place181(0x16, 0, 0x14);
    }
    Map_Run102(0x16, 2);
    Map_Do84(0x14);
    Map_Place182(0x16, 0, 0x14);
    Map_Place183(0x16, 0x148, 0xC8);
    Map_Run103(0x16, 2);
    Map_Place184(0x16, 0xB000, 0x14);
    temp_r0_12 = Map_Check19(0x16);
    FIELD(temp_r0_12, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_12, u8 *, 0x5A));
    Map_Place185(0x16, 0x150, 0xD0);
    Map_Do85(1);
    temp_r0_13 = Map_Check49(0x16);
    flag2 = 1;
    flag2 |= FIELD(temp_r0_13, u8 *, 0x5A);
    FIELD(temp_r0_13, u8 *, 0x5A) = flag2;
    Map_Place186(0x16, 0x102, 0x3C);
    Map_Place187(3, 0x00000101, 0x3C);
    Map_Place188(3, 0, 0x14);
    Map_Place189(0x16, 0, 0x14);
    Map_Place190(0x16, 0x5000, 0x14);
    Map_Run104(0x16, 2);
    Map_Do86(0x14);
    Map_Place191(0x16, 0, 0x14);
    Map_Place192(0x16, 0x150, 0xD8);
    Map_Run105((s32)&Value_0000003a, 2);
    do {
        gCell[0x22B] = 3;
    } while (0);
    Map_Run106(0x24, 2);
    Map_Run107();
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

extern u8 *Map_Run264();

extern u8 *Map_Run265();

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

    Map_Run108();
    FIELD(Map_Check20(9), s8 *, 0x55) = 0;
    Map_Place193(0, 0x01580000, 0xE00000);
    Map_Place194(1, 0x01480000, 0xE00000);
    Map_Place195(2, 0x01580000, 0xE80000);
    Map_Place196(3, 0x01480000, 0xE80000);
    Map_Place197(0, 0xA000, 0);
    Map_Place198(1, 0xC000, 0);
    Map_Place199(2, 0xC000, 0);
    Map_Place200(3, 0xC000, 0);
    Map_Place201(0x16, 0x01500000, 0xB00000);
    Map_Run109(0x16, 9);
    Map_Check21(Map_Check50(0x16), 0);
    Map_Run110(0x01500000, -1, 0xD00000, 0);
    Map_Do87(1);
    Map_Run111();
    Map_Do88(1);
    /* The ROM loads this IWRAM pointer cell before the request store. */
    SCENE_REQUEST(gWork) = 0x100;
    Map_Run112();
    Map_Run113();
    Map_Do89(0x3C);
    Map_Run114(1, 2);
    Map_Do90(0x14);
    Map_Place202(1, 0, 0x14);
    Map_Do91(0x000015d4);
    Map_Place203(1, 0, 0x14);
    Map_Place204(0, 0x8000, 0x14);
    Map_Run115(0, 3);
    Map_Do92(0x14);
    Map_Run116(0x16, 2);
    Map_Do93(0x14);
    Map_Place205(0x16, 0, 0x14);
    Map_Place206(0, 0xC000, 0);
    Map_Place207(1, 0xC000, 0x14);
    Map_Place208(2, 0x00000101, 0x3C);
    Map_Place209(2, 0, 0x14);
    Map_Run117(0x16, 2);
    Map_Do94(0x14);
    Map_Place210(0x16, 0, 0x1E);
    Map_Place211(0x18, 0x01380000, 0x700000);
    Map_Do95(0x120);
    Map_Place212(0x18, 0, 0x14);
    Map_Place213(3, 0x100, 0x3C);
    Map_Place214(3, 0, 0x1E);
    Map_Do96(0x1D);
    Map_Run118(0x0000cccc, 0x00001999);
    FIELD(Map_Run119(), s8 *, 0x55) = 0;
    Map_Run120(0x01500000, -1, 0xA80000, 1);
    Map_Do97(0x14);
    Map_Place215(0x18, 0x0000cccc, 0x00006666);
    Map_Place216(0x18, 0x158, 0x88);
    Map_Place217(0x18, 0x5000, 0x14);
    Map_Run121();
    Map_Place218(3, 0, 0x1E);
    Map_Run122(0x01500000, -1, 0xB80000, 1);
    Map_Place219(0x18, 0x158, 0xA0);
    Map_Place220(0x18, 0x148, 0xA8);
    Map_Place221(0x18, 0x138, 0xB0);
    Map_Place222(0x18, 0x3000, 0x14);
    Map_Run123();
    Map_Run124(0x18, 3);
    Map_Do98(0x14);
    Map_Place223(0x18, 0, 0x14);
    Map_Place224(0, 0x6000, 0);
    Map_Place225(1, 0x4000, 0);
    Map_Place226(2, 0x8000, 0x14);
    Map_Run125(3, 2);
    Map_Do99(0x14);
    Map_Place227(3, 0, 0x14);
    Map_Run126(0x18, 4);
    Map_Do100(0x14);
    Map_Place228(0x18, 0, 0x14);
    Map_Place229(1, 0x2000, 0);
    Map_Place230(2, 0xA000, 0);
    Map_Place231(3, 0xE000, 0x3C);
    Map_Place232(0x18, 0xB000, 0x14);
    Map_Place233(0, 0xA000, 0);
    Map_Place234(1, 0xC000, 0);
    Map_Place235(2, 0xA000, 0);
    Map_Place236(3, 0xC000, 0x14);
    Map_Place237(0x18, 0, 0x14);
    Map_Run127(3, 4);
    Map_Do101(0x14);
    Map_Place238(3, 0, 0x14);
    Map_Run128(0x18, 2);
    Map_Place239(0x18, 0x100, 0x3C);
    Map_Place240(0x18, 0x3000, 0x14);
    Map_Place241(0x18, 0, 0x14);
    Map_Place242(3, 0x00000101, 0x3C);
    Map_Run129(0x18, 4);
    Map_Do102(0x14);
    Map_Place243(0x18, 0, 0x14);
    Map_Run130(3, 2);
    Map_Do103(0x14);
    Map_Place244(3, 0, 0x14);
    Map_Run131(0x18, 3);
    Map_Do104(0x14);
    Map_Place245(0x18, 0, 0x14);
    Map_Run132(3, 1);
    Map_Do105(0x14);
    Map_Place246(3, 0, 0x14);
    Map_Place247(0x18, 0x138, 0xB8);
    Map_Place248(0x18, 0x3000, 0x14);
    Map_Place249(0x18, 0, 0x14);
    Map_Run133(1, 2);
    Map_Do106(0xA);
    Map_Place250(1, 0, 0x14);
    Map_Check22(1, 0);
    if (Map_Check23(0, 0) == 0) {
        Map_Run134(1, 2);
        Map_Place251(1, 0, 0x14);
        scene_counter_initial = gWork;
        SCENE_SKIP_COUNT(scene_counter_initial) = (u16)(SCENE_SKIP_COUNT(scene_counter_initial) + 1);
    } else {
        scene_counter_initial_alt = gWork;
        SCENE_SKIP_COUNT(scene_counter_initial_alt) = (u16)(SCENE_SKIP_COUNT(scene_counter_initial_alt) + 1);
        Map_Run135(1, 1);
        Map_Place252(1, 0, 0x14);
    }
    Map_Place253(2, 0, 0x14);
    Map_Place254(1, 0xC000, 0x14);
    Map_Run136(0x18, 3);
    Map_Do107(0x14);
    Map_Place255(0x18, 0, 0x14);
    Map_Place256(0x18, 0xD000, 0x14);
    Map_Place257(0x18, 0, 0x14);
    Map_Place258(0x18, 0x3000, 0x14);
    Map_Run137(0x18, 4);
    Map_Do108(0x14);
    Map_Place259(0x18, 0, 0x14);
    Map_Place260(3, 0x00000101, 0x3C);
    Map_Place261(3, 0, 0x14);
    Map_Run138(0x18, 4);
    Map_Do109(0x14);
    Map_Place262(0x18, 0, 0x14);
    Map_Place263(0x18, 0xB000, 0x14);
    Map_Place264(0x18, 0, 0x14);
    Map_Do110(0xA);
    Map_Run139(0x18, 2);
    Map_Do111(0x1E);
    Map_Run140(0x18, 0);
    Map_Do112(0xA);
    Map_Run141(1, 2);
    Map_Do113(0x1E);
    Map_Run142(1, 0);
    Map_Do114(0xA);
    Map_Place265(0x18, 0x2000, 0);
    Map_Do115(0x1E);
    Map_Run143(0x18, 4);
    Map_Do116(0x14);
    Map_Run144(0x18, 0);
    Map_Do117(0x14);
    Map_Run145(0x18, 2);
    Map_Do118(0x1E);
    Map_Run146(0x18, 0);
    Map_Do119(0xA);
    Map_Place266(2, 0x100, 0x3C);
    Map_Run147(2, 0);
    Map_Do120(0xA);
    Map_Run148(0x18, 3);
    Map_Do121(0x1E);
    Map_Run149(0x18, 0);
    Map_Do122(0x14);
    Map_Run150(0x18, 2);
    Map_Do123(0x1E);
    Map_Run151(0x18, 0);
    Map_Do124(0xA);
    Map_Place267(0x18, 0x1000, 0);
    Map_Do125(0x1E);
    Map_Place268(0x18, 0x108, 0x3C);
    Map_Do126(0xA);
    Map_Run152(0x16, 2);
    Map_Do127(0x1E);
    Map_Run153(0x16, 8);
    Map_Do128(0x2D);
    Map_Run154(0x16, 1);
    Map_Check24(Map_Check51(0x16), 1);
    Map_Do129(0x28);
    Map_Place269(2, 0x102, 0x3C);
    Map_Run155(2, 0);
    Map_Do130(0xA);
    Map_Place270(0x16, 0x2000, 0);
    Map_Do131(0x14);
    Map_Run156(0x16, 4);
    Map_Do132(0x1E);
    Map_Run157(0x16, 0);
    Map_Do133(0x14);
    Map_Place271(0x18, 0x2000, 0);
    Map_Do134(0x1E);
    Map_Run158(0x18, 0);
    Map_Do135(0xA);
    Map_Run159(3, 2);
    Map_Do136(0x1E);
    Map_Run160(3, 0);
    Map_Do137(0xA);
    Map_Place272(0x18, 0x16, 0);
    Map_Do138(0x23);
    Map_Run161(0x16, 3);
    Map_Do139(0x1E);
    Map_Run162(0x18, 3);
    Map_Do140(0x14);
    Map_Place273(0x18, 0x148, 0xB0);
    Map_Place274(0x18, 0x3000, 0x14);
    Map_Run163(0x18, 5);
    Map_Run164(0x16, 7);
    Map_Do141(0x14);
    Map_Run165(1, 1);
    Map_Do142(0x14);
    Map_Run166(1, 0);
    Map_Do143(0xA);
    Map_Place275(0x18, 0x00000101, 0x3C);
    Map_Check25(0x18, 0);
    Map_Do144(0x1E);
    Map_Run167(1, 2);
    Map_Run168(2, 2);
    Map_Run169(3, 2);
    Map_Place276(1, 0, 0);
    Map_Place277(2, 0xC000, 0);
    Map_Place278(3, 0xE000, 0);
    Map_Do145(0x14);
    if (Map_Check26(0, 0) == 0) {
        Map_Do146(0x1E);
        Map_Run170(0x18, 0);
        scene_counter_first_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_first_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_first_a) + 1);
    } else {
        Map_Do147(0x1E);
        /* This branch needs its own workspace load; stale register contents
         * are not a valid C dependency. */
        scene_counter_first_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_first_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_first_b) + 1);
        Map_Run171(0x18, 0);
    }
    Map_Do148(0x14);
    Map_Run172(0x18, 4);
    Map_Do149(0x14);
    Map_Place279(1, 0xC000, 0);
    Map_Place280(2, 0xA000, 0);
    Map_Place281(3, 0xC000, 0);
    Map_Do150(0x14);
    Map_Run173(0x18, 0);
    Map_Run174(0x16, 1);
    Map_Do151(0x14);
    Map_Place282(0x16, 0, 0x14);
    Map_Run175();
    Map_Do152(0x14);
    Map_Run176(0, 2);
    Map_Run177(1, 2);
    Map_Run178(2, 2);
    Map_Run179(3, 2);
    Map_Place283(0, 0, 5);
    Map_Place284(1, 0xE000, 5);
    Map_Place285(2, 0xA000, 5);
    Map_Place286(3, 0x8000, 5);
    Map_Place287(0, 0xE000, 5);
    Map_Place288(1, 0xA000, 5);
    Map_Place289(2, 0x8000, 5);
    Map_Place290(3, 0, 5);
    FIELD(Map_Run180(), s8 *, 0x55) = 0;
    Map_Run181(0x01180000, -1, 0xE80000, 1);
    Map_Run182();
    Map_Run183(0x16, 0xF);
    Map_Run184(0x18, 0xF);
    Map_Place291(0x16, 0xF00000, 0xD00000);
    Map_Place292(0x18, 0xE80000, 0xD00000);
    Map_Place293(0x16, 0x5000, 0);
    Map_Place294(0x18, 0x3000, 0);
    Map_Run185();
    Map_Place295(0, 0x8000, 0xA);
    Map_Place296(1, 0x8000, 0xF);
    Map_Place297(2, 0x8000, 0);
    Map_Place298(3, 0x8000, 0x14);
    Map_Place299(0, 0x148, 0xD0);
    Map_Place300(2, 0x150, 0xE0);
    Map_Place301(1, 0x138, 0xD8);
    Map_Place302(1, 0x8000, 0);
    Map_Do153(2);
    Map_Place303(2, 0x8000, 0);
    Map_Do154(0);
    Map_Place304(0, 0x8000, 0x1E);
    Map_Place305(1, 0, 0x14);
    Map_Run186(3, 2);
    Map_Do155(0x14);
    Map_Place306(3, 0, 0x14);
    Map_Place307(1, 0x5000, 0x14);
    Map_Place308(1, 0, 0x14);
    Map_Place309(1, 0x8000, 0x14);
    Map_Run187(2, 1);
    Map_Place310(2, 0x6000, 0x14);
    Map_Place311(2, 0, 0x14);
    Map_Run188(3, 4);
    Map_Do156(0x14);
    Map_Place312(3, 0, 0x14);
    Map_Place313(2, 0x8000, 0x14);
    Map_Place314(0x18, 0, 0x14);
    Map_Run189(0x30000, 0x6000);
    {
        void **scene_system_cell;
        Map_Run190(0x980000, -1, 0xD80000, 1);
        scene_system_cell = (void **)(gCam + 0x164);
        FIELD(scene_system_cell, s32 *, 0xC) = 0x03800000;
        scene_system_cell = (void **)&gCam;
        Map_Run191();
        Map_Do157(1);
        FIELD(Map_Check27(9), s8 *, 0x55) = 0;
        Map_Place315(9, 0x680000, 0x01080000);
        actor9_fixed_y = 0xffe00000;
        FIELD(Map_Check28(9), s32 *, 0xC) = actor9_fixed_y;
        FIELD(Map_Check52(9), s32 *, 0x3C) = actor9_fixed_y;
        Map_SetRect(0x1D, 0x4A, 4, 0x4A, 5, 4);
        Map_Run192(0x11, 0);
        Map_Run193(0x12, 0);
        Map_Run194();
        Map_Run195(0x11, 1);
        Map_Run196(0x12, 1);
        Map_Run197();
        Map_Do158(0x1E);
        Map_Place316(0x18, 0x8000, 0x14);
        Map_Place317(0x18, 0, 0x14);
        Map_Run198(0x01180000, -1, 0xD80000, 1);
        Map_Run199();
        Map_Place318(0x18, 0x3000, 0x14);
        Map_Run200(0x18, 5);
        Map_Place319(0x18, 0, 0x14);
        Map_Run201(2, 2);
        Map_Do159(0x14);
        Map_Place320(2, 0xA000, 0x14);
        Map_Place321(2, 0, 0x14);
        Map_Run202(1, 1);
        Map_Do160(0x14);
        Map_Place322(1, 0, 0x14);
        Map_Place323(0x18, 0x00000101, 0x3C);
        Map_Check29(0x18, 0);
        if (Map_Check30(0, 0) == 0) {
            Map_Do161(0x14);
            Map_Run203(0x16, 2);
            Map_Run266(0x14);
            Map_Place324(0x16, 0, 0x14);
            scene_counter_system_a = FIELD(scene_system_cell, void **, 0x4C);
            SCENE_SKIP_COUNT(scene_counter_system_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_system_a) + 1);
        } else {
            scene_counter_system_b = FIELD(scene_system_cell, void **, 0x4C);
            SCENE_SKIP_COUNT(scene_counter_system_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_system_b) + 1);
            Map_Do162(0x14);
            Map_Place325(0x16, 0, 0x14);
        }
    }
    Map_Run204(0x18, 2);
    Map_Do163(0x14);
    Map_Place326(0x18, 0, 0x14);
    Map_Place327(0, 0x00000101, 0);
    Map_Place328(1, 0x00000101, 0);
    Map_Place329(2, 0x00000101, 0);
    Map_Place330(3, 0x00000101, 0x3C);
    Map_Check31(0x18, 0);
    if (Map_Check32(0, 0) == 0) {
        Map_Do164(0x14);
        Map_Run205(0x18, 2);
        Map_Run267(0x14);
        Map_Place331(0x18, 0, 0x14);
        scene_counter_mid_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_mid_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_mid_a) + 1);
    } else {
        scene_counter_mid_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_mid_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_mid_b) + 1);
        Map_Do165(0x14);
        Map_Run206(0x18, 1);
        Map_Do166(0x14);
        Map_Place332(0x18, 0, 0x14);
    }
    Map_Do167(0x14);
    Map_Run207();
    Map_Do168(0x14);
    Map_Run208(0x800000, -1, 0xC80000, 1);
    Map_Run209();
    Map_Run210(0x16, 1);
    Map_Run211(0x18, 1);
    Map_Run212(0x16, 0xF);
    Map_Run213(0x18, 0xF);
    Map_Place333(0x16, 0x780000, 0x980000);
    Map_Place334(0x18, 0x700000, 0xA00000);
    Map_Place335(0x16, 0x5000, 0);
    Map_Place336(0x18, 0x3000, 0);
    Map_Run214();
    Map_Do169(0x1E);
    Map_Place337(0x18, 0, 0x14);
    Map_Check33(0x18, 0);
    if (Map_Check34(0, 0) == 0) {
        Map_Run215(0x18, 3);
        Map_Do170(0x14);
        Map_Place338(0x18, 0, 0x14);
        scene_counter_later_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_later_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_later_a) + 1);
    } else {
        scene_counter_later_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_later_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_later_b) + 1);
        Map_Run216(0x18, 4);
        Map_Do171(0x14);
        Map_Place339(0x18, 0, 0x14);
    }
    Map_Place340(0x18, 0, 0x14);
    Map_Run217(0x16, 1);
    Map_Do172(0x14);
    Map_Place341(0x16, 0, 0x14);
    Map_Place342(0x16, 0, 0x14);
    Map_Do173(0x14);
    Map_Place343(0x18, 0, 0x14);
    Map_Run218(0x11, 0);
    Map_Run219(0x12, 0);
    Map_Run220();
    Map_Run221(0x11, 1);
    Map_Run222(0x12, 1);
    Map_Do174(0x11);
    Map_Run223(0x01500000, -1, 0xD80000, 1);
    Map_Run224();
    Map_Run225(2, 4);
    Map_Do175(0x14);
    Map_Run226();
    Map_Place344(2, 0, 0x14);
    Map_Place345(0, 0x4000, 0);
    Map_Place346(1, 0x3000, 0x16);
    Map_Run227(1, 2);
    Map_Do176(0xA);
    Map_Place347(1, 0, 0x14);
    Map_Place348(3, 0x00000105, 0x3C);
    Map_Place349(3, 0, 0x14);
    Map_Place350(3, 0xA000, 0x14);
    Map_Place351(3, 0, 0x14);
    Map_Place352(3, 0x8000, 0x14);
    Map_Run228(3, 0x10);
    FIELD(Map_Check35(3), s32 *, 0x18) = (s32) 0xffff0000;
    Map_Do177(0x14);
    Map_Place353(3, 0, 0x28);
    Map_Run229(2, 2);
    Map_Do178(0x14);
    Map_Place354(2, 0xA000, 0x14);
    Map_Run230(2, 4);
    Map_Do179(0x14);
    Map_Check36(2, 0);
    Map_Run231(3, 1);
    FIELD(Map_Check37(3), s32 *, 0x18) = 0x10000;
    Map_Place355(0, 0x4000, 0);
    Map_Place356(1, 0xE000, 0);
    Map_Place357(3, 0xC000, 0x14);
    if (Map_Check38(0, 0) == 0) {
        Map_Run232(2, 3);
        Map_Do180(0x14);
        Map_Place358(2, 0, 0x14);
        scene_counter_final_a = gWork;
        SCENE_SKIP_COUNT(scene_counter_final_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_final_a) + 1);
    } else {
        scene_counter_final_b = gWork;
        SCENE_SKIP_COUNT(scene_counter_final_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_final_b) + 1);
        Map_Run233(2, 2);
        Map_Do181(0x14);
        Map_Place359(2, 0, 0x14);
    }
    Map_Place360(2, 0x6000, 0x14);
    Map_Run234(2, 3);
    Map_Do182(0x14);
    Map_Place361(2, 0, 0x14);
    Map_Place362(3, 0xE000, 0x1E);
    Map_Place363(1, 0x2000, 0x14);
    Map_Run235(1, 1);
    Map_Do183(0xA);
    Map_Place364(1, 0, 0x14);
    Map_Place365(3, 0xA000, 0x14);
    Map_Run236(3, 3);
    Map_Do184(0x14);
    Map_Place366(3, 0, 0x14);
    Map_Run237(3, 1);
    Map_Place367(3, 0, 0x14);
    Map_Place368(0, 0x00000101, 0);
    Map_Place369(1, 0x00000101, 0);
    Map_Place370(2, 0x00000101, 0x3C);
    Map_Place371(3, 0x0000cccc, 0x00006666);
    Map_Place372(1, 0x0000cccc, 0x00006666);
    Map_Place373(3, 0x148, 0xD8);
    Map_Place374(2, 0xA000, 0);
    actor_one_record = Map_Check39(1);
    FIELD(actor_one_record, u8 *, 0x5A) = (u8)(0xFE & FIELD(actor_one_record, u8 *, 0x5A));
    Map_Place375(1, 0x138, 0xC8);
    Map_Place376(3, 0x118, 0xD8);
    Map_Do185(1);
    Map_Place377(0, 0x6000, 0);
    Map_Place378(1, 0x6000, 0);
    Map_Do186(3);
    Map_Run238(3, 2);
    Map_Do187(0x1E);
    Map_Place379(3, 0, 0x14);
    Map_Place380(3, 0, 0x14);
    Map_Run239(1, 1);
    Map_Do188(0x14);
    Map_Place381(1, 0, 0x14);
    Map_Run240(3, 3);
    Map_Run241(0, 2);
    Map_Run242(2, 2);
    Map_Run243(1, 2);
    Map_Run244(0, 0x102);
    Map_Run245(1, 0x102);
    Map_Run246(2, 0x102);
    Map_Do189(0x3C);
    Map_Place382(2, 0, 0x14);
    Map_Run247(3, 4);
    Map_Do190(0x14);
    Map_Place383(3, 0, 0x14);
    Map_Place384(3, 0xE000, 0x14);
    Map_Place385(3, 0, 0x14);
    Map_Place386(3, 0, 0x14);
    Map_Run248(3, 4);
    Map_Do191(0x14);
    Map_Place387(3, 0, 0x1E);
    Map_Place388(1, 0x2000, 0);
    Map_Place389(2, 0xA000, 0x14);
    Map_Run249(0, 3);
    Map_Run250(1, 3);
    Map_Run251(2, 3);
    Map_Do192(0x1E);
    Map_Place390(0, 0x6000, 0);
    Map_Place391(1, 0x6000, 0);
    Map_Place392(2, 0x6000, 0x28);
    Map_Run252(0, 3);
    Map_Run253(1, 3);
    Map_Run254(2, 3);
    Map_Do193(0x14);
    Map_Run255(3, 3);
    Map_Do194(0x14);
    Map_Place393(3, 0, 0x14);
    Map_Place394(0, 0x138, 0xD8);
    Map_Place395(1, 0x138, 0xD8);
    Map_Place396(2, 0x138, 0xD8);
    Map_Place397(3, 0x138, 0xD8);
    Map_Do195(0);
    Map_Do196(1);
    Map_Place398(1, 0, 0);
    Map_Do197(2);
    Map_Place399(2, 0, 0);
    Map_Do198(3);
    Map_Place400(3, 0, 0);
    {
        void **record;
        record = (void **)(gCam + 0x164);
        FIELD(record, s32 *, 0xC) = 0x04000000;
    }
    Map_Run256();
    Map_Do199(1);
    Map_SetRect2(4, 0x46, 4, 0x4A, 5, 4);
    Map_Do200(0x880);
    Map_Do201(0x00000881);
    Map_Run257();
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
    Map_Run268(); /* main:08077268 */
    record = Scene_GetRecord_1(0); /* main:0808a080 */
    Map_Run269(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_2(1); /* main:0808a080 */
    Map_Run270(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_3(2); /* main:0808a080 */
    Map_Run271(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_4(3); /* main:0808a080 */
    Map_Run272(record, 0); /* main:080091e0 */
    Motion_CamBounds_1(0x1300000, -1, 0x780000, 0);
    Map_Run273(1); /* main:080000c0 */
    Map_Run274(); /* main:08009128 */
    Map_Run275(1); /* main:080000c0 */
    Audio_PlayCue_1(141);
    Map_Place401(0x50000, 0x50000, 0x10000); /* main:080091f0 */
    Audio_PlayCue_2(0x121);
    Map_Place402(-1, -1, 0xe666); /* main:080091f0 */
    SCENE_PHASE = 0x100;
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(); /* main:0808a370 */
    Map_Run276(); /* main:080091f8 */
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
    Map_Run277(record, 1); /* main:080091e0 */
    Object_SetModeById_4(0, 1);
    Battle_WaitMode0_14(30);
    Motion_CallWaitAnim_1(0, 4);
    Motion_ArmCb_1(0, 0xc000, 20);
    Motion_SetVarCb_1(0, 2);
    Battle_WaitMode0_15(60);
    record = Scene_GetRecord_6(1); /* main:0808a080 */
    Map_Run278(record, 1); /* main:080091e0 */
    Object_SetModeById_5(1, 1);
    Battle_WaitMode0_16(20);
    Motion_ArmCb_2(1, 0x2000, 0);
    Battle_WaitMode0_17(20);
    Motion_ArmCb_3(1, 0x6000, 0);
    Battle_WaitMode0_18(20);
    Motion_ArmCb_4(1, 0, 0);
    record = Scene_GetRecord_7(2);
    Map_Run279(record, 1); /* main:080091e0 */
    Object_SetModeById_6(2, 1);
    Motion_ArmCb_5(1, 0x6000, 0);
    Battle_WaitMode0_19(40);
    record = Scene_GetRecord_8(3); /* main:0808a080 */
    Map_Run280(record, 1); /* main:080091e0 */
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

    rec = Map_Check40(0);
    Map_Run281();
    record = Map_Check53();
    base5_200bc50 = (s32)gOv2;
    *(s32 *)base5_200bc50 = record;
    if (record != 0) {
        Map_Do202(0x250);
        rec8 = Map_Check41(*(s32 *)base5_200bc50);
        rec8[85] = 0;
        rec[85] &= 254;
        *(s32 *)((s32)rec8 + 12) += -0x30000;
        *(s32 *)((s32)rec + 12) += -0x30000;
        *(s32 *)((s32)rec + 20) += -0x30000;
        Map_Run282(2);
        *(s32 *)((s32)rec8 + 12) += -0x20000;
        *(s32 *)((s32)rec + 12) += -0x20000;
        *(s32 *)((s32)rec + 20) += -0x20000;
        Map_Run283(10);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Map_Run284(4);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Map_Run285(4);
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        *(s32 *)((s32)rec + 12) += 0x10000;
        *(s32 *)((s32)rec + 20) += 0x10000;
    }
    Map_Run286();
}

void Scene_RunScene39d(void)
{
    u32 i;
    s32 record;
    u8 *p5;
    u8 *work;

    p5 = *(volatile s32 *)0x03001e70;
    work = p5 + 0x164;
    Map_Run287();
    *(volatile s32 *)(work + 12) = 0x3800000;
    Map_Run288();
    Map_Run289(1);
    *(u8 *)(Map_Check54(9) + 85) = 0;
    Map_Place403(9, 0x680000, 0x1080000);
    record = Map_Check55(9);
    *(volatile s32 *)(record + 12) = -0x200000;
    record = Map_Check42(9);
    *(volatile s32 *)(record + 60) = -0x200000;
    *(u8 *)(Map_Check56() + 85) = 0;
    Map_Run258(0xcccc, 0x1999);
    Map_Run259(0x800000, -1, 0xb80000, 1);
    Map_Run290();
    Map_Run291(30);
    Map_SetRect3(29, 74, 4, 74, 5, 4);
    Map_Run292(17, 0);
    Map_Run293(18, 0);
    Map_Run294();
    Map_Run295(17, 1);
    Map_Run296(18, 1);
    Map_Run297(20);
    Map_Do203(0x251);
    Map_Run298();
}

void Scene_RunScene39d(void)
{
    u32 i;
    s32 record;

    Map_Run299();
    Map_Place404(0, 0x8000, 0x4000);
    Map_Run300(0, 104, 152);
    Map_Place405(0, 0x4000, 60);
    Map_Run301(17, 0);
    Map_Run302(18, 0);
    Map_Run303();
    Map_Run260(-1, -1, -1, 0);
    Map_Run304(1);
    Map_Run305();
}

/* map/locations/heidia/village_story/village_story_scene/shared.c */
s32 Map_Run306(void)
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

struct Record *Map_Run307();
struct Record *Map_Run308();
struct Record *Map_Run309();
struct Record *Map_Run310();
struct Record *Map_Run311();
struct Record *Map_Run312();
struct Record *Map_Run313();

/* Contiguous unnamed leaf-owner run for resource_39d. */

/* Configure and publish the scene's presentation layer. */

void *SceneData_GetTableb9c8(void)
{
    return gOv3;
}

void *SceneData_GetTableB9d4AfterStateCheck(void)
{
    if (gCell[225] != 1) {
        Map_Do204(0x253);
    }
    return gOv4;
}

void *SceneData_GetTablebbe4(void)
{
    return gOv5;
}

void Actor_SetMode55OnSevenRecords(void)
{
    Map_Run307(0)->mode55 = 3;
    Map_Run308(14)->mode55 = 4;
    Map_Run309(15)->mode55 = 4;
    Map_Run310(16)->mode55 = 4;
    Map_Run311(17)->mode55 = 4;
    Map_Run312(18)->mode55 = 4;
    Map_Run313(19)->mode55 = 4;
}

void Scene_ConfigureValue93Scene(void)
{
    Map_Apply(93, 1);
    Map_Apply2(24, 9);
    Map_Run314();
    Map_Do205(1);
    Map_Run315();
    Map_Run316();
}
