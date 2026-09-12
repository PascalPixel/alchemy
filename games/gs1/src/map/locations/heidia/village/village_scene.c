#include "types.h"
#include "scene.h"
#include "staged_actor.h"
#include "staged_actor_movement.h"
#include "object_runtime.h"
#include "add_parts_record.h"
#include "add_parts_body.inc"

/* map/locations/heidia/village/village_scene/village_scene.c */
/* map/locations/heidia/village/village_scene/actor_activation.c */
typedef struct MovingActor {
    u8 pad0[6];
    u16 kind;       /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    u8 pad14[0x10];
    s32 vx;         /* 0x24 */
    u8 pad28[4];
    s32 vy;         /* 0x2c */
    u8 pad30[8];
    s32 a;          /* 0x38 */
    u8 pad3c[4];
    s32 b;          /* 0x40 */
} MovingActor;

typedef struct BlockingActor {
    u8 pad0[0x50];
    s32 *info;      /* 0x50 */
} BlockingActor;

extern s32 StagedActorStepTable[];
extern s32 gOv[];
extern u8 gWork[];

BlockingActor *Map_unk4_4(s32 *position, MovingActor *actor);

s32 StopBlockedActorMotion(MovingActor *actor)
{
    s32 destination[3];
    u32 direction_index = actor->kind >> 12;
    s32 step = StagedActorStepTable[direction_index];
    BlockingActor *blocking_actor;

    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    blocking_actor = Map_unk4_4(destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = ((s16 *)*(s32 *)((u8 *)blocking_actor->info + 0x28))[kind_index];

        {
            s32 *p = gOv;

            do {
                if (blocking_kind == *p++) {
                    goto done;
                }
                kind_index++;
            } while (kind_index <= 5);
        }
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
    step = StagedActorStepTable[direction_index];
    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    if (Map_Apply(actor, destination) > 0) {
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
done:
    return 0;
}

void ActivateSceneActor17(void)
{
    Map_unk14_2(17);
    Map_unk5_4();
}

void ActivateSceneActor18(void)
{
    Map_unk15_2(18);
    Map_unk6_4();
}

void FinishActor20SceneSequence(void)
{
    if (Map_unk14(0x226)) {
        Map_unk16_2(0x2435);
        Map_Apply2(20, 0);
    } else {
        s16 *q = (s16 *)(gWork + 382);

        *q = 0;
        Map_unk7_4();
        Map_unk8_4();
    }
}

void ActivateFiveActorGroupFromFlags(void)
{
    if (Map_unk15(0x350)) {
        Map_unk17_2(0);
    }
    if (Map_unk16(0x351)) {
        Map_unk18_2(1);
    }
    if (Map_unk17(0x352)) {
        Map_unk19_2(2);
    }
    if (Map_unk18(0x353)) {
        Map_unk20_2(3);
    }
    if (Map_unk19(0x354)) {
        Map_unk21_2(4);
    }
}

/* map/locations/heidia/village/village_scene/actor_presentation.c */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Map_unk3_3(actor, x, y);
}

void PlaceActorTwelveAndFinishScene(void)
{
    PlaceActor(12, 0x00f80000, 0x01780000);
    Map_unk9_4();
}

/* map/locations/heidia/village/village_scene/actor_setup.c */
extern u8 Value_00002440;
extern u8 Value_00002459;
extern u8 Value_0000256c;

void ConfigureSceneActor9(void)
{
    Map_unk10_4();
    Map_unk22_2(9);
    Map_unk4_3(9, 0, 0);
    Map_Apply3(9, 0);
    Map_unk5_3(9, 0, 0);
    Map_unk6_3(9, 256, 0);
    Map_unk23_2(10);
    Map_unk11_4();
}

void ConfigureInteractionRegionA(void)
{
    Map_unk5_5(2, 82, 1, 2, 21, 81);
    Map_unk6_5(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    Map_unk7_5(2, 84, 1, 2, 6, 55);
    Map_unk8_5(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    Map_unk9_5(2, 86, 1, 2, 27, 62);
    Map_unk10_5(26, 16, 1, 1, 27, 17);
}

void ConfigurePrimaryInteractionRegions(void)
{
    Map_unk11_5(5, 77, 1, 2, 17, 82);
    Map_unk12_5(5, 77, 1, 2, 3, 55);
    Map_unk13_5(15, 33, 1, 1, 17, 35);
    Map_unk14_5(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    Map_unk15_5(8, 77, 1, 2, 17, 82);
    Map_unk16_5(8, 77, 1, 2, 3, 55);
    Map_unk17_5(18, 35, 1, 1, 17, 35);
    Map_unk18_5(2, 10, 1, 1, 3, 10);
}

void ConfigureSceneActor26(void)
{
    Map_unk7_3(26, 1, 5);
    Map_unk24_2(0x94e);
}

void ConfigureSceneActor14(void)
{
    Map_Apply4(14, 2);
    Map_unk25_2(0x2441);
    Map_Apply5(14, 0);
}

void ConfigureSceneActor13(void)
{
    Map_Apply6(13, 2);
    Map_unk26_2((s32)&Value_00002440);
    Map_Apply7(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    Map_Apply8(12, 2);
    Map_unk27_2(0x243f);
    Map_Apply9(12, 0);
}

void ConfigureSceneActor18(void)
{
    Map_unk28_2((s32)&Value_00002459);
    Map_Apply10(18, 0);
}

void ConfigureActor13Interaction(void)
{
    u8 *interaction_resources = &Value_0000256c;

    Map_unk29_2((s32)interaction_resources);
    Map_Apply11(0x800d, 0);
    if (Map_unk20(234) != -1) {
        Map_Apply12((s32)(interaction_resources + 2), 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    Map_unk30_2(0x256d);
    Map_Apply13(13, 0);
}

/* map/locations/heidia/village/village_scene/messages.c */
extern s32 Value_00000953;
extern s32 Value_00000f2a;
extern s32 Value_00000f2b;
extern s32 Value_00000f2c;
extern s32 Value_00000f2d;

void ShowLockedDoorMessageOnce(void)
{
    if (Map_unk21(0xea) == -1) {
        Map_Apply14((s32)&Value_00000953, 1);
    }
}

void ShowLockedDoorMessage(void)
{
    Map_Apply15((s32)&Value_00000953, 1);
}

void ShowGeraldDontMessage(void)
{
    if (Map_Apply16(8, 8) != 0) {
        Map_unk31_2((s32)&Value_00000f2a);
    }
}

void ShowThatWasExcitingMessage(void)
{
    if (Map_Apply17(9, 7) != 0) {
        Map_unk32_2((s32)&Value_00000f2b);
    }
}

void ShowSorryIScaredYouMessage(void)
{
    if (Map_Apply18(10, 6) != 0) {
        Map_unk33_2((s32)&Value_00000f2c);
    }
}

void ShowWellMessage(void)
{
    if (Map_Apply19(11, 5) != 0) {
        Map_unk34_2((s32)&Value_00000f2d);
    }
}

/* map/locations/heidia/village/village_scene/no_op.c */
void NoOpSceneCallbackA(void)
{
}

void NoOpSceneCallbackB(void)
{
}

void NoOpSceneCallbackC(void)
{
}

void NoOpSceneCallbackD(void)
{
}

void NoOpInteractionCallback(void)
{
}

void NoOpActorCallback(void)
{
}

/* map/locations/heidia/village/village_scene/object_inspection.c */
extern s16 gCell[];

void InspectOrdinaryObject(void)
{
    extern u8 *gWork;

    Map_Apply20(0, 1);
    Map_unk35_2(113);
    Map_unk8_3(15, 256, 60);
    Map_unk36_2(15);
    *(s32 *)(gWork + 448) = 512;
    gCell[0x22b] = 3;
    Map_Apply21(98, 2);
    Map_unk9_3(15, 0, 0);
    Map_unk12_4();
    Map_unk37_2(2380);
}

void InspectEmptyChest(void)
{
    extern u8 *gWork;

    Map_Apply22(0, 1);
    Map_unk38_2(113);
    Map_unk10_3(11, 256, 60);
    Map_unk39_2(11);
    *(s32 *)(gWork + 448) = 512;
    gCell[0x22b] = 3;
    Map_Apply23(98, 2);
    Map_unk11_3(11, 0, 0);
    Map_unk13_4();
    Map_unk40_2(2377);
}

void InspectEmptySceneObject(void)
{
    extern u8 *gWork;

    Map_Apply24(0, 1);
    Map_unk41_2(113);
    Map_unk12_3(16, 256, 60);
    Map_unk42_2(16);
    *(s32 *)(gWork + 448) = 512;
    gCell[0x22b] = 3;
    Map_Apply25(98, 2);
    Map_unk13_3(16, 0, 0);
    Map_unk14_4();
    Map_unk43_2(2379);
}

void InspectVillageWell(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)gWork + 0xcb8)) != 0) {
        if (Map_Check(0x947) == 0) {
            Map_Run(0x1528, 1);
            Map_unk15_4(188);
            v5 = 17;
            v6 = 82;
            Map_unk16_4(1);
            Map_SetRect(6, 77, 1, 2, v5, v6);
            Map_unk17_4(5);
            Map_unk2_5(7, 77, 1, 2, v5, v6);
            Map_unk18_4(1);
            Map_unk19_4();
            Map_Do(0x947);
        }
    }
}

void InspectWardrobe(void)
{
    Map_unk44_2(2372);
    Map_unk45_2(535);
    Map_unk14_3(8, 0, 0);
}

void InspectFirewood(void)
{
    Map_unk46_2(2373);
    Map_unk20_4();
    Map_unk15_3(9, 0, 0);
}

void InspectBooks(void)
{
    Map_unk47_2(2374);
    Map_unk48_2(536);
    Map_unk16_3(10, 0, 0);
}

/* map/locations/heidia/village/village_scene/scene_data.c */
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 Value_000000a3;
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 Value_0000006a;
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];

/* No scene data is available for this slot. */

s32 SelectPrimarySceneData(void)
{
    s16 scene_variant = gCell[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)gOv2;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)gOv3;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)gOv4;
    }
    return (s32)gOv5;
}

s32 GetEmptySceneData(void) { return 0; }

s32 SelectSecondarySceneData(void)
{
    s16 scene_variant = gCell[224];

    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)gOv6;
    }
    if (scene_variant == (s32)&Value_000000a2 || scene_variant == (s32)&Value_000000a3) {
        return (s32)gOv7;
    }
    return (s32)gOv8;
}

s32 SelectTertiarySceneData(void)
{
    s16 scene_variant = gCell[224];

    if (scene_variant == (s32)&Value_0000006a) {
        return (s32)gOv9;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)gOv10;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)gOv11;
    }
    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)gOv12;
    }
    if (scene_variant == (s32)&Value_000000a3) {
        return (s32)gOv13;
    }
    return (s32)gOv14;
}

s32 SelectQuaternarySceneData(void)
{
    s16 scene_variant = gCell[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)gOv15;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)gOv16;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)gOv17;
    }
    return (s32)gOv18;
}

void SelectActor25SceneVariant(void)
{
    if (Map_unk22(0x941)) {
        Map_unk49_2(0x2568);
        Map_Apply26(25, 0);
    } else {
        Map_unk50_2(0x2458);
        Map_Apply27(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    if (Map_unk23(0x941)) {
        Map_unk51_2(0x2569);
        Map_Apply28(24, 0);
    } else {
        Map_unk52_2(0x244e);
        Map_Apply29(24, 0);
    }
}

/* map/locations/heidia/village/village_scene/scene_scripts.c */
#define SHARED_RECORD_FIELD_448 (*(u32 *)(*(u8 **)0x03001ebc + 448))

extern u8 Value_0000240d;
extern u8 Value_0000241e;
extern u8 Value_00002428;
extern u8 Value_0000247d;
extern u8 Value_00002464;
extern u8 Value_0000247e;
extern u8 gVal[];
extern u8 gVal2[];

/*
 * Actor script -- resource_3bf.  It walks a run of consecutive resource
 * identifiers starting at 0x241e, re-supplying the actor handle to every
 * service that takes one, and returns nothing.
 */

/*
 * Symbol names use the offsets as printed while absolute data addresses keep
 * their raw pool values; the module's own pool constants sit 0x8000 above
 * the printed offsets.  Map_unk21_4 is reached with two different argument
 * shapes -- once as (handle, 0) and once as (identifier) -- so these services
 * keep old-style declarations rather than an invented common prototype.
 */

/* Byte 0x22B of gCell, a flag/state table indexed by other code. */

/* Word at +448 of the record pointed to by the global at 0x03001ebc. */

/*
 * Keeping this call in its own helper preserves the interleaving of the
 * handle move with the two-instruction constant.
 */

#if defined(GS1_EDITION_JA)
#define Value_00002421 Value_000025aa
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define Value_00002421 Value_00002403
#endif

extern u8 Value_00002421;

void Scene_RunScene3bfSequenceA(void)
{
    extern s16 gCell[];
    extern u8 *gWork;

    u8 *p5;

    p5 = *(volatile s32 *)&gWork;
    if (Map_unk2(0x214) == 0) {
        if (Map_unk2_4() == 0) {
            if (gCell[294] == 0) {
                if (Map_unk3(17) != 0) {
                    Map_unk2_2(0x215);
                    Map_unk3_2(0x214);
                }
            }
            if (Map_unk4(0x214) != 0) {
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 94;

                    *target = shown;
                }
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    extern s16 gCell[];
    extern u8 *gWork;

    Map_unk53_2(17);
    Map_unk22_4();
}

void TriggerSceneStage95FromActor12(void)
{
    extern s16 gCell[];
    extern u8 *gWork;

    u8 *scene_state = gWork;

    if (Map_unk24(12) != 0 && gCell[294] == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Map_unk54_2(0x02009719);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}

void Scene_RunScene3bfSequenceB(void)
{
    extern s16 gCell[];
    extern u8 *gWork;

    u8 *p5;

    p5 = *(volatile s32 *)&gWork;
    if (Map_unk5(0x225) == 0) {
        if (Map_unk6(13) != 0) {
            if (gCell[294] == 0) {
                Map_unk4_2(0x225);
                Map_unk5_2(0x200975d);
                Map_unk6_2(0x20097bd);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}

void Scene_RunScene3bfSequenceC(void)
{
    extern s16 gCell[];
    extern u8 *gWork;

    u8 *p5;

    p5 = *(volatile s32 *)&gWork;
    if (Map_unk7(0x225) == 0) {
        if (Map_unk8(21) != 0) {
            if (gCell[294] == 0) {
                Map_unk7_2(0x225);
                Map_unk8_2(0x20097bd);
                Map_unk9_2(0x200975d);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}

void RunActor9ScriptedSequence(void)
{
    Map_unk23_4();
    Map_unk24_4(9, 0, 0);
    Map_Apply30(9, 1);
    Map_unk25_4(9);
    Map_Apply31(9, 0);
    Map_unk26_4(0, 1);
    {
        u8 *t = &Value_0000240d;

        Map_unk55_2((s32)t);
        Map_Apply32(9, 0);
        Map_unk17_3(0, 258, 60);
        Map_unk56_2((s32)(t + 1));
    }
    Map_Apply33(9, 0);
    Map_unk57_2(60);
    Map_unk27_4();
    Map_unk28_4();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
    Map_unk29_4();
    Map_unk30_4();
    Map_unk18_3(actor_id, 256, 1);
    Map_unk19_3(actor_id, 0, 0);
    Map_Apply34(actor_id, 1);
    Map_Apply35(actor_id, 0);
    Map_unk31_4(actor_id, 0, 0);
    Map_Apply36(0, 1);
    Map_unk20_3(actor_id, 0, 0);
    Map_Apply37(actor_id, 1);
    Map_unk32_4(actor_id);
    Map_unk33_4(actor_id, 0);
    Map_unk34_4(0, 1);
    {
        u8 *t = &Value_0000240d;

        Map_unk58_2((s32)t);
        Map_Apply38(actor_id, 0);
        Map_unk21_3(0, actor_id, 0);
        Map_unk22_3(0, 258, 60);
        Map_unk59_2((s32)(t + 1));
    }
    Map_Apply39(actor_id, 0);
    Map_unk35_4();
    Map_unk60_2(60);
    Map_unk61_2(60);
    Map_unk36_4();
}

/*
 * The resource run is taken as the address of Value_0000241e rather than as
 * an integer constant, which preserves its pointer identity and materialises
 * it after the first call.
 */
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Map_unk37_4(handle, 1);
    id = &Value_0000241e;
    Map_unk38_4((s32)id);
    Map_unk39_4(handle, 0);
    Call(handle);
    Map_unk40_4((s32)(id + 1));
    Map_unk41_4(handle, 0);
    id += 2;
    Map_unk42_4(handle, 4);
    Map_unk43_4((s32)id);
    Map_unk44_4(handle, 0);
}

void RunActorScriptedSequenceC(s32 actor_id)
{
    u8 *t = &Value_00002421;

    Map_unk62_2((s32)t);
    Map_Apply40(actor_id, 0);
    Map_Apply41(actor_id, 1);
    Map_unk63_2((s32)(t + 1));
    Map_unk45_4(actor_id, 0);
    Map_Apply42(actor_id, 4);
    Map_unk46_4((s32)(t + 2));
    Map_Apply43(actor_id, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = &Value_00002428;

    Map_unk64_2((s32)t);
    Map_Apply44(actor_id, 0);
    Map_Apply45(actor_id, 4);
    Map_unk65_2((s32)(t + 1));
    Map_Apply46(actor_id, 0);
    Map_Apply47(actor_id, 1);
    Map_unk66_2((s32)(t + 2));
    Map_unk47_4(actor_id, 0);
    Map_Apply48(actor_id, 3);
    Map_unk48_4((s32)(t + 3));
    Map_Apply49(actor_id, 0);
}

void RunActor12InteractionSequence(void)
{
    Map_unk49_4();
    Map_unk23_3(0, 0, 0);
    Map_Apply50(0, 1);
    Map_Apply51(0, 1);
    Map_unk24_3(12, 0, 0);
    Map_unk67_2(113);
    Map_unk25_3(12, 256, 60);
    {
        u8 *t = &Value_0000240d;

        Map_unk68_2((s32)t);
        Map_Apply52(12, 0);
        Map_unk26_3(0, 258, 50);
        Map_unk69_2((s32)(t + 1));
    }
    Map_Apply53(12, 0);
    Map_unk50_4();
    Map_unk70_2(60);
    Map_unk71_2(60);
    Map_unk51_4();
    Map_unk72_2(548);
}

void RunSecondaryMapInteraction(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)gWork + 0xcb8)) != 0) {
        if (Map_unk9(0x948) == 0) {
            Map_unk3_4(0x1528, 1);
            Map_unk52_4(188);
            v5 = 3;
            v6 = 55;
            Map_unk53_4(1);
            Map_unk3_5(6, 77, 1, 2, v5, v6);
            Map_unk54_4(5);
            Map_unk4_5(7, 77, 1, 2, v5, v6);
            Map_unk55_4(1);
            Map_unk56_4();
            Map_unk10_2(0x948);
        }
    }
}

/* Runs one of two long fixed sequences (chosen by a status check), each
 * placing actors, panning/animating them, and running dialogue lines built
 * from a base text pointer plus a running line offset. */
void PlayStoryScene(void)
{
    extern u8 gCell[];

    s32 text_line;

    if (GameFlag_IsSet_1(769) != 0) {
        AddSoftFloatParts();
    }
    GameFlag_Set_1(624);
    Battle_Reset_1();
    if (GameFlag_IsSet_2(2370) != 0) {
        Motion_SetSpeed_1(0, 32768, 16384);
        Motion_SetPosReset_1(0, 456, 216);
        Motion_SetAngleToward_1(0, 12, 0);
        Map_unk58_4();
        BattleFx_SpawnLinked_1(12, 256, 60);
        Motion_ArmCb_1(12, 32768, 0);
        Motion_Launch_1(12, 4, 0);
        Map_unk59_4(Scene_GetRecord_1(12), 1);
        Battle_WaitMode0_1(30);
        Motion_SetSpeed_2(2, 45875, 22937);
        Motion_ResetPosMode2_1(2, 464, 192);
        Motion_CommitPos_1(2);
        Battle_WaitMode0_2(30);
        Motion_ArmCb_2(0, 16384, 0);
        Motion_ArmCb_3(2, 16384, 0);
        Motion_ArmCb_4(1, 16384, 0);
        Motion_ArmCb_5(3, 16384, 0);
        Motion_SetHPosTerrain_1(13, 29884416, 20971520);
        Motion_SetSpeedLim_1(131072, 16384);
        text_line = (s32)&Value_0000247d;
        SceneWork_SetStepValue_1(text_line);
        BattleEv_RunWait_1(13, 0);
        Motion_ResetPosMode2_2(13, 458, 272);
        Motion_CommitPos_2(13);
        Motion_ArmCb_6(13, 20480, 0);
        Battle_WaitMode0_3(40);
        Motion_OffsetPositionAndResetMotion_1(13, -8, 8);
        Motion_CommitPos_3(13);
        Battle_WaitMode0_4(60);
        Audio_PlayCue_1(155);
        UiText_DrawMessage_1(text_line + 1, 1);
        Motion_OffsetPositionAndResetMotion_2(13, 8, -8);
        Map_unk60_4();
        Battle_WaitMode0_5(120);
        Motion_SetVarCb_1(0, 2);
        Motion_SetVarCb_2(2, 2);
        Motion_SetVarCb_3(1, 2);
        Motion_SetVarCb_4(3, 2);
        Battle_WaitMode0_6(20);
        text_line += 2;
        Map_unk61_4();
        Motion_SetAngleToward_2(13, 0, 0);
        SceneWork_SetStepValue_2(text_line);
        BattleEv_RunWait_2(13, 0);
        Map_unk62_4();
        SHARED_RECORD_FIELD_448 = 512;
        Battle_WaitMode0_7(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_1((s32)&Value_000000a3, 4);
        BattleFx_ComputeWeightedResultAndDispatch_1(98, 4);
    } else {
        Object_SetModeById_1(0, 1);
        Audio_PlayCue_2(17);
        Battle_WaitMode0_8(30);
        text_line = (s32)&Value_00002464;
        SceneWork_SetStepValue_3(text_line);
        BattleEv_RunWait_3(12, 0);
        Object_LinkPair_1(0, 12, 0);
        Battle_WaitMode0_9(140);
        Motion_ArmCb_7(12, 32768, 0);
        Motion_Launch_2(12, 4, 0);
        Map_unk63_4(Scene_GetRecord_2(12), 1);
        SceneWork_SetStepValue_4(text_line + 1);
        BattleEv_RunWait_4(12, 0);
        Motion_SetSpeed_3(0, 32768, 16384);
        Motion_SetPosReset_2(0, 456, 216);
        Motion_SetAngleToward_3(0, 12, 0);
        Map_unk64_4();
        Motion_SetSpeed_4(2, 45875, 22937);
        Motion_ResetPosMode2_3(2, 464, 192);
        Motion_CommitPos_4(2);
        Battle_WaitMode0_10(30);
        SceneWork_SetStepValue_5(text_line + 2);
        BattleEv_RunWait_5(2, 0);
        BattleFx_SpawnLinked_2(12, 256, 0);
        Battle_WaitMode0_11(110);
        Audio_PlayCue_3(60);
        SceneWork_SetStepValue_6(text_line + 3);
        BattleEv_RunWait_6(12, 0);
        Battle_WaitMode0_12(30);
        Motion_CallWaitAnim_1(2, 3);
        Battle_WaitMode0_13(20);
        Object_SetModeById_2(2, 1);
        Motion_SetVarCb_5(12, 1);
        Battle_WaitMode0_14(20);
        Motion_SetSpeed_5(12, 26214, 13107);
        Motion_ResetPosMode2_4(12, 520, 208);
        Motion_CommitPos_5(12);
        Object_SetModeById_3(12, 1);
        Battle_WaitMode0_15(20);
        Motion_ArmCb_8(12, 45056, 0);
        Battle_WaitMode0_16(30);
        Motion_ArmCb_9(12, 20480, 0);
        Battle_WaitMode0_17(30);
        Motion_SetAngleToward_4(12, 2, 0);
        Battle_WaitMode0_18(20);
        SceneWork_SetStepValue_7(text_line + 4);
        BattleEv_RunWait_7(12, 0);
        Battle_WaitMode0_19(40);
        Motion_CallWaitAnim_2(2, 3);
        Battle_WaitMode0_20(20);
        BattleFx_SpawnLinked_3(12, 264, 0);
        Battle_WaitMode0_21(120);
        SceneWork_SetStepValue_8(text_line + 5);
        BattleEv_RunWait_8(12, 0);
        Battle_WaitMode0_22(25);
        Motion_CallWaitAnim_3(2, 3);
        Battle_WaitMode0_23(30);
        Motion_CallWaitAnim_4(12, 3);
        Battle_WaitMode0_24(40);
        Motion_ResetPosMode2_5(2, 480, 200);
        Motion_CommitPos_6(2);
        Object_LinkPair_2(2, 12, 0);
        Battle_WaitMode0_25(60);
        SceneWork_SetStepValue_9(text_line + 6);
        BattleEv_RunWait_9(2, 0);
        Battle_WaitMode0_26(20);
        Object_SetModeById_4(12, 4);
        Battle_WaitMode0_27(80);
        SceneWork_SetStepValue_10(text_line + 7);
        BattleEv_RunWait_10(12, 0);
        Motion_SetHPosTerrain_2(13, 29884416, 20971520);
        Audio_PlayCue_4(19);
        SceneWork_SetStepValue_11(text_line + 8);
        BattleEv_RunWait_11(13, 0);
        Motion_SetAngleToward_5(0, 13, 0);
        Motion_SetAngleToward_6(2, 13, 0);
        Motion_SetAngleToward_7(1, 13, 0);
        Battle_WaitMode0_28(5);
        Motion_ArmCb_10(3, 16384, 0);
        Motion_SetAngleToward_8(12, 13, 0);
        Battle_WaitMode0_29(30);
        Audio_PlayCue_5(61);
        Motion_SetSpeedLim_2(131072, 16384);
        Motion_PlaceLookedUpObjectWithinCameraBounds_1(13, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        Motion_SetSpeed_6(13, 52428, 26214);
        Motion_ResetPosMode2_6(13, 456, 304);
        Motion_SetPosReset_3(13, 1);
        Motion_CommitPos_7(13);
        Motion_SetPosReset_4(1, 1);
        BattleFx_SpawnLinked_4(0, 258, 0);
        BattleFx_SpawnLinked_5(2, 258, 0);
        BattleFx_SpawnLinked_6(1, 258, 0);
        BattleFx_SpawnLinked_7(3, 258, 0);
        BattleFx_SpawnLinked_8(12, 258, 0);
        Battle_WaitMode0_30(60);
        Motion_SetAngleToward_9(12, 13, 0);
        Motion_SetVarCb_6(12, 2);
        Battle_WaitMode0_31(60);
        SceneWork_SetStepValue_12(text_line + 9);
        BattleEv_RunWait_12(12, 0);
        Motion_SetAngleToward_10(13, 12, 0);
        SceneWork_SetStepValue_13(text_line + 10);
        BattleEv_RunWait_13(13, 0);
        Battle_WaitMode0_32(60);
        Motion_SetAngleToward_11(13, 2, 0);
        Battle_WaitMode0_33(30);
        SceneWork_SetStepValue_14(text_line + 11);
        BattleEv_RunWait_14(13, 0);
        Object_LinkPair_3(3, 2, 0);
        Object_LinkPair_4(0, 1, 0);
        Battle_WaitMode0_34(60);
        Motion_SetAngleToward_12(0, 13, 0);
        Motion_SetAngleToward_13(2, 13, 0);
        Motion_SetAngleToward_14(1, 13, 0);
        Motion_SetAngleToward_15(3, 13, 0);
        Motion_SetVarCb_7(13, 1);
        Battle_WaitMode0_35(60);
        SceneWork_SetStepValue_15(text_line + 12);
        BattleEv_RunWait_15(13, 0);
        BattleFx_SpawnLinked_9(1, 259, 0);
        Battle_WaitMode0_36(60);
        Object_SetModeById_5(13, 4);
        SceneWork_SetStepValue_16(text_line + 13);
        BattleEv_RunWait_16(13, 0);
        Motion_ResetPosMode2_7(1, 456, 248);
        Motion_CommitPos_8(1);
        Motion_ArmCb_11(1, 16384, 0);
        SceneWork_SetStepValue_17(text_line + 14);
        BattleEv_RunWait_17(1, 0);
        Motion_ResetPosMode2_8(2, 472, 216);
        Motion_CommitPos_9(2);
        Motion_ArmCb_12(2, 16384, 0);
        Battle_WaitMode0_37(10);
        SceneWork_SetStepValue_18(text_line + 15);
        BattleEv_RunWait_18(2, 0);
        BattleFx_SpawnLinked_10(12, 261, 0);
        Battle_WaitMode0_38(60);
        SceneWork_SetStepValue_19(text_line + 16);
        BattleEv_RunWait_19(12, 0);
        Motion_ResetPosMode2_9(3, 440, 216);
        Motion_CommitPos_10(3);
        Motion_SetAngleToward_16(3, 13, 0);
        Motion_CallWaitAnim_5(3, 3);
        Battle_WaitMode0_39(10);
        SceneWork_SetStepValue_20(text_line + 17);
        BattleEv_RunWait_20(3, 0);
        Object_SetModeById_6(13, 4);
        SceneWork_SetStepValue_21(text_line + 18);
        BattleEv_RunWait_21(13, 0);
        BattleFx_SpawnLinked_11(0, 258, 0);
        BattleFx_SpawnLinked_12(1, 258, 0);
        BattleFx_SpawnLinked_13(3, 258, 0);
        BattleFx_SpawnLinked_14(2, 258, 0);
        BattleFx_SpawnLinked_15(13, 264, 0);
        Battle_WaitMode0_40(60);
        SceneWork_SetStepValue_22(text_line + 19);
        BattleEv_RunWait_22(13, 0);
        Battle_WaitMode0_41(20);
        BattleFx_SpawnLinked_16(1, 259, 0);
        Battle_WaitMode0_42(60);
        SceneWork_SetStepValue_23(text_line + 20);
        BattleEv_RunWait_23(1, 0);
        Motion_SetVarCb_8(13, 1);
        Battle_WaitMode0_43(60);
        SceneWork_SetStepValue_24(text_line + 21);
        BattleEv_RunWait_24(13, 0);
        Motion_ResetPosMode2_10(13, 456, 280);
        Motion_CommitPos_11(13);
        Motion_ArmCb_13(13, 20480, 0);
        Battle_WaitMode0_44(80);
        Motion_OffsetPositionAndResetMotion_3(13, -8, 8);
        Motion_CommitPos_12(13);
        Battle_WaitMode0_45(60);
        Audio_PlayCue_6(155);
        UiText_DrawMessage_2((s32)&Value_0000247e, 1);
        Motion_OffsetPositionAndResetMotion_4(13, 8, -8);
        Motion_SetAngleToward_17(13, 11, 0);
        Map_unk65_4();
        Audio_PlayCue_7(52);
        SceneWork_SetStepValue_25(text_line + 23);
        BattleEv_RunWait_25(13, 0);
        Battle_WaitMode0_46(60);
        Motion_SetAngleToward_18(0, 11, 0);
        Motion_SetAngleToward_19(1, 11, 0);
        Motion_SetAngleToward_20(2, 11, 0);
        Motion_SetAngleToward_21(3, 11, 0);
        Motion_SetAngleToward_22(12, 11, 0);
        Map_unk66_4();
        Map_unk67_4();
        GameFlag_Set_2(2370);
        SHARED_RECORD_FIELD_448 = 512;
        Battle_WaitMode0_47(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_2((s32)&Value_000000a3, 4);
        BattleFx_ComputeWeightedResultAndDispatch_2(98, 4);
    }
    Battle_SchedShoulder_1();
}

void RunActor20SceneSequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_242e;
    s32 base5_2430;

    if (Map_unk10(0x226) != 0) {
        Map_unk11_2(0x2434);
        Map_unk68_4(20, 0);
    } else {
        Map_unk69_4();
        Map_unk70_4(20, 0, 0);
        if (Map_unk11(0x227) == 0) {
            Map_unk71_4(20, 4, 0);
            Map_unk72_4(20);
            Map_unk73_4(20);
            Map_unk74_4(20);
            base5_242e = (s32)gVal;
            Map_unk75_4(base5_242e);
            Map_unk76_4(20, 0);
            Map_Place(20, 0x102, 30);
            Map_unk77_4((base5_242e + 1));
            Map_unk78_4(20, 0);
            Map_unk79_4(30);
            Map_unk80_4(20, 4);
            Map_unk81_4(30);
        }
        base5_2430 = (s32)gVal2;
        Map_unk82_3(base5_2430);
        Map_unk83_3(20, 0);
        Map_unk2_3(20, 0x101, 40);
        Map_unk84_3((base5_2430 + 1));
        Map_unk12(20, 0);
        if (Map_unk13(0, 0) == 0) {
            Map_unk85_3((base5_2430 + 2));
            Map_unk86_3(20, 0);
            Map_unk12_2(0x226);
        } else {
            Map_unk87_3((base5_2430 + 3));
            Map_unk88_3(20, 0);
        }
        Map_unk13_2(0x227);
        Map_unk89_3();
    }
}

/* map/locations/heidia/village/village_scene/trigger_tests.c */
typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

typedef struct SceneActor {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;


SceneActor *Map_unk90_3(s32);
SceneActor *Map_unk91_3(s32);
SceneActor *Map_unk92_3(s32);
SceneActor *Map_unk93_3(s32);
SceneActor *Map_unk94_3(s32);
SceneActor *Map_unk95_3(void);
SceneActor *Map_unk96_3(s32);
SceneActor *Map_unk97_3(void);
SceneActor *Map_unk98_3(s32);

SceneActor *Map_unk99_3(void);
SceneActor *Map_unk100_3(s32);
SceneActor *Map_unk101_3(void);
SceneActor *Map_unk102_3(void);
SceneActor *Map_unk103_3(s32);

SceneActor *Map_unk104_3(s32);

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    s32 started = 0;
    s32 interaction;

    Map_unk105_3();
    interaction = Map_unk27_3(0, actor_id, interaction_id);
    if (Map_Apply54(interaction_id, 0) != -1) {
        Map_Apply55(actor_id, 2);
        started = 1;
    } else {
        Map_unk73_2(0x7d);
        Map_Apply56(actor_id, 5);
    }
    Map_unk74_2(interaction);
    Map_unk106_3();
    return started;
}

s32 IsPlayerInAccidentTriggerArea(void)
{
    SceneActor *player = Map_unk90_3(0);
    s32 z = player->z;
    s32 x;
    s32 zz, xx;

    if (z < 0) {
        z += 0xfffff;
    }
    x = player->x;
    zz = z >> 20;
    if (x < 0) {
        x += 0xfffff;
    }
    xx = x >> 20;
    if ((u32)(zz - 5) <= 2 && xx <= 10) {
        return 1;
    }
    if ((u32)(xx - 8) <= 1 && zz > 22) {
        return 1;
    }
    return 0;
}

s32 AreSceneActorsInPassingLane(void)
{
    SceneActor *player = Map_unk91_3(0);
    SceneActor *passing_actor = Map_unk92_3(17);
    s32 ox = player->x;
    s32 pz;
    s32 px;
    s32 oxx, pzz, pxx;

    if (ox < 0) {
        ox += 0xfffff;
    }
    oxx = ox >> 20;
    pz = passing_actor->z;
    if (pz < 0) {
        pz += 0xfffff;
    }
    px = passing_actor->x;
    pzz = pz >> 20;
    if (px < 0) {
        px += 0xfffff;
    }
    pxx = px >> 20;
    if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    return 0;
}

s32 IsPlayerInSecondaryTriggerArea(void)
{
    SceneActor *player = Map_unk93_3(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
        return 1;
    }
    if (xx == 41 && zz > 37 && zz <= 41) {
        return 1;
    }
    if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
        return 1;
    }
    return 0;
}

s32 IsPlayerOutsideSceneRectangle(void)
{
    SceneActor *player = Map_unk94_3(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}

s32 IsSceneActorVerticallyNearPlayer(void)
{
    SceneActor *scene_actor = Map_unk95_3();
    SceneActor *player = Map_unk96_3(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 z_distance = actor_z - player_z;

    if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorHorizontallyNearPlayer(void)
{
    SceneActor *scene_actor = Map_unk97_3();
    SceneActor *player = Map_unk98_3(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;

    if (x_distance < -6 || x_distance > 6) {
        return 0;
    }
    if (actor_z - 2 < player_z && actor_z + 2 > player_z) {
        return 1;
    }
    return 0;
}

s32 IsActorInteractionAvailable(s32 actor_id)
{
    if (Map_unk25() == 0) {
        return 0;
    }
    if (Map_unk26(actor_id)!= 0) {
        return 1;
    }
    {
        s32 result = Map_unk27(actor_id);

        /* branchless "result != 0" */
        return (u32)(result | -result) >> 31;
    }
}

s32 IsSceneActorWithinFourSteps(void)
{
    SceneActor *scene_actor = Map_unk99_3();
    SceneActor *player = Map_unk100_3(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    actor_z += 1;
    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance + z_distance <= 4) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorWithinTriggerBox(void)
{
    SceneActor *scene_actor = Map_unk101_3();
    SceneActor *player = Map_unk102_3();
    s32 actor_x = scene_actor->x / 0x100000;
    s32 actor_z = scene_actor->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance > 7 || z_distance > 5) {
        return 0;
    }
    return 1;
}

void TriggerScene41AtVillagePath(void)
{
    SceneActor *player = Map_unk103_3(0);

    if (Map_unk28(859) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 43 && player_z > 28 && player_z <= 31) {
            s16 *q = (s16 *)(gWork + 364);
            s32 v = 41;

            *q = v;
            Map_unk107_3();
        }
    }
}

void TriggerScene40AtVillagePath(void)
{
    SceneActor *player = Map_unk104_3(0);

    if (Map_unk29(856) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 16 && player_z > 55 && player_z <= 58
            && (player->dir == 0xc000 || player->dir == 0x4000)) {
            s16 *q = (s16 *)(gWork + 364);
            s32 v = 40;

            *q = v;
            Map_unk108_3();
        }
    }
}

/* map/locations/heidia/village/village_scene/village.c */

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

typedef struct Req {
    s32 kind;       /* 0x00 */
    s32 f4;         /* 0x04 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    s32 f14;        /* 0x14 */
} Req;

extern s32 gOv19[];
extern s32 Value_00000f2e;
extern s32 gOv20;

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

Obj *Map_unk109_3(s32 *, s32 *, Req *);

void AdvanceStagedActorPair(void)
{
    extern u32 StagedActorStepTable[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 FindClearActorPosition(Req *request)
{
    extern s32 StagedActorStepTable[];

    s32 out;
    s32 vec[3];
    Obj *r;
    s32 n;
    s32 nx, ny;
    u8 *flag;
    s32 i, j;
    s32 w1, w2;
    s32 off;

    request->f14 = 0;
    r = Map_unk109_3(&out, &request->f4, request);
    if (r == 0) {
        return 0;
    }
    flag = (u8 *)r + 0x22;
    *flag = 2;
    {
        s32 k = request->kind;

        n = 0;
        off = k * 4;
        w1 = gOv19[off + 1];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = gOv19[off + 3];
        if (w2 < 0) {
            w2 = -w2;
        }
        ny = (w1 + w2) >> 4;
        w1 = gOv19[off];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = gOv19[off + 2];
        if (w2 < 0) {
            w2 = -w2;
        }
        nx = (w1 + w2) >> 4;
    }
    vec[0] = r->x + (StagedActorStepTable[out] & 0xffff0000);
    {
        s32 ry = r->y;

        vec[1] = ry;
        vec[2] = r->z + (StagedActorStepTable[out] << 16);
        request->y = ry;
    }
    for (;;) {
        request->z = vec[2] + (gOv19[request->kind * 4 + 1] << 16);
        for (j = 0; j < ny; j++) {
            request->x = vec[0] + (gOv19[request->kind * 4] << 16);
            for (i = 0; i < nx; i++) {
                if (Map_Apply57(r, &request->x) == 2) {
                    goto found;
                }
                request->x += 0x100000;
            }
            request->z += 0x100000;
        }
        n++;
        vec[0] += StagedActorStepTable[out] & 0xffff0000;
        vec[2] += StagedActorStepTable[out] << 16;
    }
found:
    *flag = 0;
    if (n == 0) {
        return 0;
    }
    request->x = r->x + (StagedActorStepTable[out] & 0xffff0000) * n;
    request->y = r->y;
    request->z = r->z + (StagedActorStepTable[out] << 16) * n;
    return 1;
}

void StartDoraHouseAccidentScene(void)
{
    Map_unk28_3(0, 0x108, 0x318);
    Map_unk75_2(0);
    Map_unk29_3(0, 0x4000, 0);
    Map_unk76_2(10);
    Map_Apply58(0, 1);
    Map_Apply59(0xea, 3);
    Map_Apply60(0, 1);
    Map_Apply61(0xea, 0);
    Map_unk77_2((s32)&Value_00000f2e);
    Map_unk30_3(8, 0, 0);
}

void TurnActorToSceneDirection(s32 actor_id)
{
    Map_unk31_3(actor_id, 0, 0);
    Map_unk32_3(0, actor_id, 0);
    switch (gOv20 & 3) {
    case 0:
        Map_unk78_2(actor_id);
        break;
    case 1:
        Map_unk79_2(actor_id);
        break;
    case 2:
        Map_unk80_2(actor_id);
        break;
    case 3:
        Map_unk81_2(actor_id);
        break;
    default:
        Map_unk82(actor_id);
        break;
    }
}

/* map/locations/heidia/village/scene/move_redraw.c */
void Map_RunMoveRedraw(
    StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}

/* map/locations/heidia/village/village.c */
/* map/locations/heidia/village/actor_pair_sequence.c */
extern u8 gValVillage[];

void Scene_RunScene3bfVillage(void)
{
    u32 i;
    s32 record;
    s32 base5_240d;

    Map_unk2_4Village();
    Map_unk3_4Village(0, 0, 0);
    Map_unk4_4Village(0, 1);
    Map_unk5_4Village(0, 1);
    Map_unk6_4Village(113);
    Map_Place(21, 0x100, 0);
    Map_unk2_3(13, 0x100, 60);
    Map_unk7_4Village(21, 0, 0);
    Map_unk8_4Village(13, 0, 0);
    base5_240d = (s32)gValVillage;
    Map_unk9_4Village(base5_240d);
    Map_unk10_4Village(13, 0);
    Map_unk3_3(0, 0x102, 30);
    Map_unk11_4Village((base5_240d + 1));
    Map_unk12_4Village(13, 0);
    Map_unk13_4Village();
    Map_unk14_4Village(60);
    Map_unk15_4Village(60);
    Map_unk16_4Village();
    Map_Do(0x225);
}

/* map/locations/heidia/village/actor_scenes.c */
/* External Func aliases name loader-relocated call words, not runtime addresses. */

u8 *Map_unk17_4Village(s32);


extern s32 gOvVillage[];

extern s32 gOv2Village[];

extern s32 gOv3Village[];

extern u32 gOv4Village;

extern u8 gVal2Village[];

extern u8 gCam[];
extern u32 gIw;
extern u8 Value_00002092;
struct ObjectRuntime *Map_unk18_4Village();
struct ObjectRuntime *Map_unk19_4Village();

struct ObjectRuntime *Map_unk20_4Village();

struct ObjectRuntime *Map_unk21_4Village();

void ConfigureSceneActor12(void)
{
    s32 actor_slot = 15;
    u8 *actor;

    Map_unk6_5Village(15, 20, 1, 1, actor_slot, 22);
    Map_unk7_5Village(17, 23, 1, 3, actor_slot, 23);
    actor = Map_unk17_4Village(12);
    if (actor != 0) {
        Map_ApplyVillage(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}

void RunSceneObjectSetup(void)
{
    Map_unk22_4Village();
}

void Scene_StartActorTwelveTransition(void)
{
    Map_unk4_3(12, 0x10000, 0x8000);
    Map_unk5_3(12, 248, 0x178);
    Map_unk23_4Village(12);
    Map_unk24_4Village(215);
    Map_unk25_4Village(60);
    Map_unk26_4Village();
    Map_unk2_2(0x943);
}

void Scene_UpdateActorTwelveTransition(void)
{
    u8 *rec7;
    s32 record;

    record = Map_Check(12);
    if ((*(volatile s32 *)(record + 16) >> 20) > 22) {
        Map_unk6_3(0x40000, 0x40000, 0x10000);
        Map_unk7_3(-1, -1, 0xe666);
        Map_unk27_4Village(144);
        Map_SetRect(15, 20, 1, 1, 15, 22);
        Map_unk2_5(17, 23, 1, 3, 15, 23);
        rec7 = Map_unk2(12);
        if ((s32)rec7 != 0) {
            Map_unk12_2((s32)rec7, 0);
            rec7[35] = 2;
        }
        Map_unk3_2(0x943);
    }
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{
    s32 position_x = gOvVillage[table_index * 2];
    s32 position_z = gOvVillage[table_index * 2 + 1];

    Map_unk8_5Village(0, 0x4d, 1, 3, position_x, position_z);
    Map_unk9_5Village(1, 0x4d, 1, 1, position_x + 1, position_z);
    Map_unk10_5Village(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void Scene_UpdateObjectPairA(void)
{
    s32 v6;
    s32 raw;
    s32 d;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    v6 = 1;
    if (Map_unk13_2(234) != -v6) {
        raw = *(s16 *)(((s32)p5 + 0x16c));
        d = raw - 40;
        if (Map_unk3(0x941) != 0) {
            if (d == 4) {
                goto done;
            }
        }
        Map_unk28_4Village(d);
        Map_unk29_4Village(157);
        Map_unk8_3(0x30000, 0x30000, 0x10000);
        Map_unk9_3(-v6, -v6, 0xe666);
        Map_unk30_4Village(raw + 0x328);
    }
done:;
}

void Scene_UpdateObjectPairB(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)gWork;
    if (Map_unk23Village(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Map_unk31_4Village(field - 40);
        Map_unk32_4Village(157);
        Map_unk10_3(0x30000, 0x30000, 0x10000);
        Map_unk11_3(-1, -1, 0xe666);
        Map_unk33_4Village(field + 0x32d);
    }
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{
    s32 position_x = gOv2Village[table_index * 2];
    s32 position_z = gOv2Village[table_index * 2 + 1];

    Map_unk11_5Village(0x37, 0x79, 1, 3, position_x, position_z);
    Map_unk12_5Village(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Map_unk13_5Village(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void Scene_UpdateTableBObjectPair(void)
{
    u8 *work;
    s16 field;

    work = *(volatile s32 *)gWork;
    if (Map_unk24Village(234) != -1) {
        field = *(s16 *)((s32)work + 0x16c);
        Map_unk34_4Village(field - 40);
        Map_unk35_4Village(157);
        Map_unk12_3(0x30000, 0x30000, 0x10000);
        Map_unk13_3(-1, -1, 0xe666);
        Map_unk36_4Village(field + 0x330);
    }
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{
    s32 position_x = gOv3Village[table_index * 2];
    s32 position_z = gOv3Village[table_index * 2 + 1];

    Map_unk14_5Village(1, 0x50, 1, 3, position_x, position_z);
    Map_unk15_5Village(2, 0x50, 1, 1, position_x + 1, position_z);
    Map_unk16_5Village(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void Scene_UpdateObjectPairC(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)gWork;
    if (Map_unk25Village(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Map_unk37_4Village(field - 40);
        Map_unk38_4Village(157);
        Map_unk14_3(0x30000, 0x30000, 0x10000);
        Map_unk15_3(-1, -1, 0xe666);
        Map_unk39_4Village(field + 0x332);
    }
}

static __inline__ void SetSceneValue(s16 *field, s32 value)
{
    *field = value;
}

void Scene_UpdateActorPairInteraction(void)
{
    struct ObjectRuntime *actor = Map_unk18_4Village(9);
    struct ObjectRuntime *other = Map_unk19_4Village(10);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gIw & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Map_unk26Village(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
        other->movement_state = 1;
    } else if (!Map_unk4(0x214)) {
        actor->movement_state = 0;
        other->movement_state = 0;
        if (!Map_unk4(0x214) && actor->movement_state == 0) {
            work[8] = Map_unk27Village(Map_Apply2Village(8912896.0, Map_unk14_2Village(actor->x)));
        }
        if (!Map_unk28Village()) {
            if (gCell[294] != 0) {
                if (Map_unk29Village(9) && gCell[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
                if (Map_unk29Village(10) && gCell[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
            }
            if (gCell[294] == 0) {
                if (Map_unk30(9)) {
                    Map_unk40_4Village(0x215);
                    Map_unk40_4Village(0x214);
                }
                if (Map_unk30(10)) {
                    Map_unk40_4Village(0x215);
                    Map_unk40_4Village(0x214);
                }
            }
            if (Map_unk4(0x214)) {
                SetSceneValue(&scene[193], 91);
            }
        }
    }
}

void Scene_UpdateActorSeventeenInteraction(void)
{
    struct ObjectRuntime *actor = Map_unk20_4Village(17);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    Map_unk41_4Village(actor);
    if (gIw & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Map_unk31(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Map_unk5(0x214)) {
        actor->movement_state = 0;
        if (!Map_unk5(0x214) && actor->movement_state == 0) {
            work[8] = 0x3400000 - actor->x;
            work[9] = 0x2400000 - actor->z;
        }
        if (!Map_unk32()) {
            Map_unk33(17);
            if (Map_unk34(17) && gCell[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (gCell[294] == 0) {
                if (Map_unk35(17)) {
                    Map_unk42_4Village(0x215);
                    Map_unk42_4Village(0x214);
                }
            }
            if (Map_unk5(0x214)) {
                SetSceneValue(&scene[193], 92);
            }
        }
    }
}

void Scene_UpdateActorEighteenInteraction(void)
{
    struct ObjectRuntime *actor = Map_unk21_4Village(18);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gIw & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Map_unk36(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Map_unk6(0x214)) {
        actor->movement_state = 0;
        if (!Map_unk6(0x214) && actor->movement_state == 0) {
            work[8] = 0x2f00000 - actor->x;
            work[9] = 0x1f00000 - actor->z;
        }
        if (!Map_unk37()) {
            if (Map_unk38(18) && gCell[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (gCell[294] == 0) {
                if (Map_unk39(18)) {
                    Map_unk43_4Village(0x215);
                    Map_unk43_4Village(0x214);
                }
            }
            if (Map_unk6(0x214)) {
                SetSceneValue(&scene[193], 93);
            }
        }
    }
}

void Scene_SelectActorTwentyOneMessage(void)
{
    switch (gOv4Village) {
    case 0:
        Map_unk44_4Village(0x2414);
        Map_unk45_4Village(21, 0);
        break;
    case 1:
        Map_unk44_4Village(0x2415);
        Map_unk45_4Village(21, 0);
        break;
    case 2:
        Map_unk44_4Village(0x2416);
        Map_unk45_4Village(21, 0);
        break;
    case 3:
        Map_unk44_4Village(0x2417);
        Map_unk45_4Village(21, 0);
        break;
    case 4:
        Map_unk44_4Village(0x2418);
        Map_unk45_4Village(21, 0);
        break;
    case 6:
        Map_unk44_4Village(0x241a);
        Map_unk45_4Village(21, 0);
        break;
    case 7:
        Map_unk44_4Village(0x241b);
        Map_unk45_4Village(21, 0);
        break;
    case 5:
        Map_unk16_3(21, 0xd000, 0);
        Map_unk46_4Village(50);
        Map_unk17_3(21, 0xb000, 0);
        Map_unk47_4Village(50);
        Map_unk18_3(21, 0x5000, 0);
        Map_unk48_4Village(50);
        Map_unk49_4Village(0x2419);
        Map_unk50_4Village(21, 0);
        break;
    }
}

void Scene_RunActorTwentyOneSequence(void)
{
    s32 base5_2411;

    Map_unk19_3(21, 0x101, 30);
    Map_unk20_3(21, 0xd000, 0);
    Map_unk51_4Village(50);
    Map_unk21_3(21, 0xb000, 0);
    Map_unk52_4Village(50);
    Map_unk22_3(21, 0x5000, 0);
    Map_unk53_4Village(50);
    base5_2411 = (s32)gVal2Village;
    Map_unk54_4Village(base5_2411);
    Map_unk55_4Village(21, 0);
    Map_unk56_4Village(21, 4);
    Map_unk57_4(60);
    Map_unk23_3(21, 0xb000, 0);
    Map_unk58_4Village(40);
    Map_unk59_4Village((base5_2411 + 1));
    Map_unk60_4Village(21, 0);
}

/* map/locations/heidia/village/actor_setup.c */

struct ObjectRuntime *Map_unk61_4Village();
struct ObjectRuntime *Map_unk62_4Village();
struct ObjectRuntime *Village_GetActor();
struct ObjectRuntime *Map_unk63_4Village();

struct ObjectRuntime *Map_unk64_4Village();
struct ObjectRuntime *Map_unk65_4Village();

void Village_FinishActorRestore();

extern u8 gOv5Village[], gOv6Village[], gOv7Village[];

struct ObjectRuntime *Map_unk66_4Village();

struct ObjectRuntime *Map_unk67_4Village();
struct ObjectRuntime *Map_unk68_4Village();

extern u8 gOv8Village[], gOv9Village[], gOv10Village[];
extern u8 gOv11Village[], gOv12Village[], gOv13Village[];

/* Scheduler_AddOrUpdateCallback returns an index even when it is ignored. */

void Scene_InstallSceneTasks(void)
{
    Map_unk69_4Village();
    switch (gCell[225]) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk7((s32)gOv8Village, 3200);
        Map_unk8((s32)gOv9Village, 3200);
        Map_unk9((s32)gOv10Village, 3200);
        Map_unk70_4Village(0xe00);
        break;
    case 12:
    case 19:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x209;
        Map_unk70_4Village(0xc00);
        break;
    case 16:
    case 17:
    case 18:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk10((s32)gOv11Village, 3200);
        Map_unk11((s32)gOv12Village, 3200);
        Map_unk71_4Village(1);
        Map_unk72_4Village();
        Map_unk73_4Village(1);
        Map_unk3_5(101, 9, 10, 8, 110, 9);
        Map_unk70_4Village(0xe00);
        break;
    case 13:
    case 14:
    case 15:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk12((s32)gOv13Village, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk74_4Village(0xe00);
        break;
    }
    Map_unk75_4Village(18, 1);
    Map_unk76_4Village(17, 1);
    Map_unk77_4Village(21, 1);
    Map_unk78_4Village(12, 1);
    Map_unk79_4Village(13, 1);
    Map_unk80_4Village(1);
}

void Scene_SetupActorsForScene(void)
{
    struct ObjectRuntime *actor;

    Map_unk81_4Village();
    Map_unk82_3Village(9, 1);
    Map_unk83_3Village(10, 1);
    Map_unk84_3Village(17, 1);
    if (Map_unk40(0x94c)) {
        Map_unk85_3Village(15, 0, 0);
    }
    if (Map_unk41(0x949)) {
        Map_unk86_3Village(11, 0, 0);
    }
    if (Map_unk42(0x94b)) {
        Map_unk87_3Village(16, 0, 0);
    }
    if (Map_unk43(0xf2e)) {
        Map_unk88_3Village(8, 0, 0);
    }
    switch (gCell[225]) {
    case 1:
    case 2:
    case 3:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk89_3Village(0xe00);
        Map_unk90_3Village(gOv5Village, 3200);
        Map_unk91_3Village(1);
        Map_unk92_3Village();
        Map_unk93_3Village(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x209;
        Map_unk94_3Village(0xc00);
        Map_unk95_3Village(Map_unk66_4Village(24), 0);
        if (Map_unk44(0x314)) {
            Map_unk24_3(25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk96_3Village(0xe00);
        Map_unk90_3Village(gOv6Village, 3200);
        Map_unk91_3Village(1);
        Map_unk92_3Village();
        Map_unk93_3Village(1);
        break;
    case 11:
    case 12:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        if (Map_unk45(0x94a)) {
            Map_unk97_3Village();
        }
        break;
    case 31:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk98_3Village();
        break;
    case 14:
    case 15:
    case 16:
        Map_unk99_3Village(gOv7Village, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_unk100_3Village(0xe00);
        break;
    }
    actor = Map_unk67_4Village(8);
    Map_unk101_3Village(Map_unk68_4Village(8), 0);
    Map_unk102_3Village(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void Scene_RestoreActorsFromFlags(void)
{
    struct ObjectRuntime *actor;

    *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
    Map_unk103_3Village();
    if (Map_unk46(0x943)) {
        Map_unk104_3Village();
    }
    Map_unk4_2(0x217);
    Map_unk5_2(0x218);
    if (Map_unk47(0x944)) {
        Map_unk105_3Village(8, 0, 0);
        Map_unk6_2(0x217);
    }
    if (Map_unk48(0x945)) {
        Map_unk106_3Village(9, 0, 0);
        Map_unk107_3Village();
    }
    if (Map_unk49(0x946)) {
        Map_unk108_3Village(10, 0, 0);
        Map_unk7_2(0x218);
    }
    if (Map_unk50(0x947)) {
        Map_unk109_3Village();
    }
    if (Map_unk51(0x948)) {
        Map_unk110_3();
    }
    Map_unk111_3();
    actor = Map_unk61_4Village(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Map_unk62_4Village(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Village_GetActor(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Map_unk63_4Village(11);
    if (actor != 0) {
        Map_unk112_3(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Map_unk64_4Village(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Map_unk113_3(Map_unk65_4Village(11), 0);
    Map_unk114_3();
    Village_FinishActorRestore(0xe00);
}

void Scene_ActivateThreeActorGroup(void)
{
    if (Map_unk52(0x35a)) {
        Map_unk15_2Village(0);
    }
    if (Map_unk53(0x35b)) {
        Map_unk16_2Village(1);
    }
    if (Map_unk54(0x35c)) {
        Map_unk17_2Village(2);
    }
}

void Scene_ActivateTwoActorGroup(void)
{
    if (Map_unk55(0x358)) {
        Map_unk13_2(0);
    }
    if (Map_unk56(0x359)) {
        Map_unk18_2Village(1);
    }
}

void Scene_ActivateAlternateActorGroup(void)
{
    if (Map_unk57(0x355)) {
        Map_unk12_2(0);
    }
    if (Map_unk58(0x356)) {
        Map_unk19_2Village(1);
    }
    if (Map_unk59(0x357)) {
        Map_unk20_2Village(2);
    }
}

/* map/locations/heidia/village/actor_steps.c */
extern u8 gVal3[];

void Scene_RunScene3bf2(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = (s32)gVal3;
    Map_unk115_3(base6_2424);
    Map_unk116_3(a0, 0);
    Map_unk117_3(120);
    Map_unk25_3(a0, 0x101, 60);
    Map_unk118_3((base6_2424 + 1));
    Map_unk119_3(a0, 0);
    Map_unk120_3(a0, 1);
    Map_unk121_3((base6_2424 + 2));
    Map_unk122_3(a0, 0);
    Map_unk123_3(a0, 4);
    Map_unk124_3((base6_2424 + 3));
    Map_unk125_3(a0, 0);
}

/* map/locations/heidia/village/conditional_scene.c */
extern u8 gVal4[];
extern u8 gVal5[];

void Scene_RunScene3bf3(void)
{
    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Map_unk126_3();
    if (Map_unk13(0x941) != 0) {
        Map_unk8_2(0x2566);
        Map_unk127_3(18, 0);
        Map_unk128_3();
    } else {
        if (Map_unk14(0x313) != 0) {
            Map_unk9_2(0x2457);
            Map_unk15(25, 0);
            Map_unk129_3();
        } else {
            Map_unk26_3(25, 0x102, 30);
            Map_unk130_3(25, 0, 0);
            base5_244f = (s32)gVal4;
            Map_unk131_3(base5_244f);
            Map_unk132_3(25, 0);
            Map_unk133_3(25, 24, 0);
            Map_unk134_3(24, 1);
            Map_unk135_3();
            Map_unk136_3(60);
            Map_unk137_3(0, 1);
            Map_unk55_4Village(20);
            Map_unk27_3(25, 0x105, 60);
            Map_unk138_3((base5_244f + 1));
            Map_unk139_3(25, 0);
            Map_unk28_3(25, 0x107, 60);
            Map_unk140_3((base5_244f + 2));
            Map_unk141_3(25, 0);
            Map_unk142_3(70);
            Map_unk29_3(25, 0x100, 60);
            Map_unk143_3(25, 0, 0);
            Map_unk144_3((base5_244f + 3));
            Map_unk16(25, 0);
            if (Map_unk17(0, 0) == 0) {
                Map_unk145_3((base5_244f + 4));
                Map_unk146_3(25, 0);
            } else {
                Map_unk147_3((base5_244f + 5));
                Map_unk148_3(25, 0);
            }
            Map_unk149_3(60);
            Map_unk30_3(25, 0x105, 60);
            base5_2455 = (s32)gVal5;
            Map_unk150_3(base5_2455);
            Map_unk18(25, 0);
            Map_unk151_3(25, 1);
            Map_unk152_3((base5_2455 + 1));
            Map_unk19(25, 0);
            Map_unk153_3(25, 3);
            Map_unk154_3((base5_2455 + 2));
            Map_Run(25, 0);
            Map_unk10_2(0x313);
            Map_unk155_3();
        }
    }
}

/* map/locations/heidia/village/main_scene.c */
extern u8 gVal6[];
extern u8 gVal7[];
extern u8 gVal8[];

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_3();

u8 *Motion_CommitPos_22();

u8 *Scene_GetRecord_4();
u8 *Battle_WaitMode0_92();

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* Long fixed sequence of setup, positioning, and per-actor animation calls
 * against actor slots 0-3, 11-15, driven by three script line tables, with
 * two two-way branches on the outcome of a query call. Ends by writing the
 * scene phase word and issuing a final batch of calls. */
void Scene_RunMainScriptSequence(void)
{
    u32 i;
    u8 *record;
    s32 script_a;
    s32 script_b;
    s32 script_c;

    GameFlag_Set_1(0x301);
    GameFlag_Set_2(0x941);
    Map_unk4_5(16, 75, 7, 4, 26, 55);
    Map_unk156_3(4);
    Battle_Reset_1();
    record = Scene_GetRecord_1(12);
    Map_unk157_3(record, 1);
    Motion_SetHPosTerrain_1(0, 0x1c80000, 0xb80000);
    Motion_ArmCb_1(0, 0x4000, 0);
    Motion_SetHPosTerrain_2(1, 0x1b80000, 0xc00000);
    Motion_ArmCb_2(1, 0x4000, 0);
    Motion_SetHPosTerrain_3(3, 0x1e80000, 0xb80000);
    Motion_ArmCb_3(3, 0x4000, 0);
    Motion_SetHPosTerrain_4(2, 0x1d80000, 0xb80000);
    Motion_ArmCb_4(2, 0x4000, 0);
    Motion_SetHPosTerrain_5(12, 0x2080000, 0xe00000);
    Motion_ArmCb_5(12, 0xb000, 0);
    Motion_SetHPosTerrain_6(11, 0x1c00000, 0xed0000);
    Motion_ArmCb_6(11, 0x8000, 0);
    Motion_SetHPosTerrain_7(15, 0x1c00000, 0xee0000);
    Motion_SetActionVariant_1(15, 3);
    Object_SetModeById_1(11, 3);
    Motion_SetHPosTerrain_8(13, 0x1ca0000, 0xf30000);
    Motion_ArmCb_7(13, 0x4000, 0);
    Object_SetModeById_2(13, 5);
    Battle_WaitMode0_1();
    Battle_WaitMode0_2(120);
    /* Script line bases are overlay data symbols: an integer base would be
     * constant-propagated into every offset instead of staying in r5. */
    script_a = (s32)gVal6;
    SceneWork_SetStepValue_1(script_a);
    BattleEv_RunWait_1(13, 0);
    BattleFx_SpawnLinked_1(1, 0x101, 60);
    SceneWork_SetStepValue_2((script_a + 1));
    BattleEv_RunWait_2(1, 0);
    BattleFx_SpawnLinked_2(0, 0x101, 1);
    BattleFx_SpawnLinked_3(2, 0x101, 1);
    BattleFx_SpawnLinked_4(3, 0x101, 1);
    Battle_WaitMode0_3(60);
    Motion_SetAngleToward_1(12, 13, 0);
    Battle_WaitMode0_4(60);
    SceneWork_SetStepValue_3((script_a + 2));
    BattleEv_RunWait_3(12, 0);
    Motion_SetAngleToward_2(0, 12, 0);
    Motion_SetAngleToward_3(1, 12, 0);
    Motion_SetAngleToward_4(2, 12, 0);
    Motion_SetAngleToward_5(3, 12, 0);
    Battle_WaitMode0_5(60);
    Motion_ResetPosMode2_1(12, 0x200, 232);
    Object_SetModeById_3(12, 4);
    Battle_WaitMode0_6(60);
    SceneWork_SetStepValue_4((script_a + 3));
    BattleEv_RunWait_4(12, 0);
    Battle_WaitMode0_7(15);
    Motion_SetVarCb_1(13, 2);
    SceneWork_SetStepValue_5((script_a + 4));
    BattleEv_RunWait_5(13, 0);
    Motion_SetAngleToward_6(0, 13, 0);
    Motion_SetAngleToward_7(2, 13, 0);
    Motion_SetAngleToward_8(3, 13, 0);
    Motion_SetAngleToward_9(1, 13, 0);
    Motion_SetVarCb_2(2, 1);
    SceneWork_SetStepValue_6((script_a + 5));
    BattleEv_RunWait_6(2, 0);
    BattleFx_SpawnLinked_5(3, 0x105, 0);
    Object_SetModeById_4(3, 4);
    Battle_WaitMode0_8(80);
    Object_SetModeById_5(3, 1);
    SceneWork_SetStepValue_7((script_a + 6));
    BattleEv_RunWait_7(3, 0);
    BattleFx_SpawnLinked_6(12, 0x102, 65);
    Motion_SetVarCb_3(12, 2);
    Battle_WaitMode0_9(100);
    SceneWork_SetStepValue_8((script_a + 7));
    BattleEv_RunWait_8(13, 0);
    Motion_SetAngleToward_10(1, 0, 0);
    Battle_WaitMode0_10(10);
    SceneWork_SetStepValue_9((script_a + 8));
    BattleEv_RunWait_9(1, 0);
    Motion_SetVarCb_4(2, 1);
    Battle_WaitMode0_11(30);
    SceneWork_SetStepValue_10((script_a + 9));
    BattleEv_RunWait_10(2, 0);
    Motion_SetAngleToward_11(3, 0, 0);
    SceneWork_SetStepValue_11((script_a + 10));
    Motion_SetSpeed_1(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Object_SetModeById_6(2, 3);
        Object_SetModeById_7(1, 3);
        Object_SetModeById_8(3, 3);
        Motion_SetAngleToward_12(12, 13, 0);
        Battle_WaitMode0_12(60);
    } else {
        SceneWork_SetStepValue_12((script_a + 11));
        BattleEv_RunWait_11(3, 0);
        Motion_SetAngleToward_13(0, 12, 0);
        Motion_SetAngleToward_14(1, 12, 0);
        Motion_SetAngleToward_15(2, 12, 0);
        Motion_SetAngleToward_16(3, 12, 0);
        Battle_WaitMode0_13(20);
        Motion_SetAngleToward_17(12, 0, 0);
        Battle_WaitMode0_14(60);
        Motion_SetAngleToward_18(12, 13, 0);
        Battle_WaitMode0_15(80);
        Motion_CallWaitAnim_1(12, 3);
        Battle_WaitMode0_16(30);
        Motion_SetAngleToward_19(12, 0, 0);
        SceneWork_SetStepValue_13((script_a + 12));
        BattleEv_RunWait_12(12, 0);
        Battle_WaitMode0_17(60);
        Motion_SetAngleToward_20(2, 12, 0);
        Motion_SetAngleToward_21(1, 12, 0);
        Motion_SetAngleToward_22(3, 12, 0);
        Motion_SetAngleToward_23(0, 12, 0);
        Object_SetModeById_9(2, 3);
        Object_SetModeById_10(1, 3);
        Object_SetModeById_11(3, 3);
        Object_SetModeById_12(0, 3);
        Battle_WaitMode0_18(80);
        Motion_SetAngleToward_24(12, 13, 0);
        Motion_SetAngleToward_25(2, 13, 0);
        Motion_SetAngleToward_26(1, 13, 0);
        Motion_SetAngleToward_27(3, 13, 0);
        Motion_SetAngleToward_28(0, 13, 0);
    }
    Motion_SetSpeed_2(0, 0x8000, 0x4000);
    Motion_SetSpeed_3(1, 0x8000, 0x4000);
    Motion_SetSpeed_4(3, 0x8000, 0x4000);
    Motion_SetSpeed_5(2, 0x8000, 0x4000);
    Motion_ResetPosMode2_2(1, 0x1a0, 216);
    Motion_CommitPos_1(1);
    Motion_ResetPosMode2_3(1, 0x1a0, 248);
    Motion_CommitPos_2(1);
    Motion_ResetPosMode2_4(1, 0x1b8, 248);
    Motion_CommitPos_3(1);
    Motion_ArmCb_8(1, 0xc000, 0);
    Motion_ResetPosMode2_5(0, 0x1b8, 216);
    Motion_CommitPos_4(0);
    Motion_ArmCb_9(0, 0, 0);
    Motion_ResetPosMode2_6(3, 0x1e8, 248);
    Motion_CommitPos_5(3);
    Motion_ResetPosMode2_7(3, 0x1c8, 248);
    Motion_CommitPos_6(3);
    Motion_ArmCb_10(3, 0xc000, 0);
    Motion_ResetPosMode2_8(2, 0x1c8, 216);
    Motion_CommitPos_7(2);
    Motion_ArmCb_11(2, 0x8000, 0);
    Object_LinkPair_1(1, 0, 0);
    Object_LinkPair_2(3, 2, 0);
    Battle_WaitMode0_19(60);
    Motion_CallWaitAnim_2(0, 3);
    Object_SetModeById_13(2, 3);
    Object_SetModeById_14(1, 3);
    Object_SetModeById_15(3, 3);
    Battle_WaitMode0_20(100);
    Audio_PlayCue_1(226);
    Object_SetModeById_16(13, 7);
    Motion_SetSpeed_6(0, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_1(0, -24, 0);
    Motion_SetSpeed_7(1, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_2(1, -24, 0);
    Motion_SetSpeed_8(3, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_3(3, -24, 0);
    Motion_SetSpeed_9(2, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_4(2, -24, 0);
    Motion_SetSpeed_10(11, 0x1999, 0xccc);
    Motion_SetSpeed_11(15, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_5(11, -24, 0);
    Motion_OffsetPositionAndResetMotion_6(15, -24, 0);
    Motion_CommitPos_8(0);
    Audio_PlayCue_2(0x120);
    Battle_WaitMode0_21(60);
    Motion_ArmCb_12(0, 0x4000, 0);
    Motion_ArmCb_13(1, 0xc000, 0);
    Motion_ArmCb_14(2, 0x4000, 0);
    Motion_ArmCb_15(3, 0xc000, 0);
    Battle_WaitMode0_22(60);
    Object_SetModeById_17(0, 3);
    Object_SetModeById_18(1, 3);
    Object_SetModeById_19(2, 3);
    Object_SetModeById_20(3, 3);
    Battle_WaitMode0_23(100);
    Motion_SetSpeed_12(0, 0xb333, 0x5999);
    Motion_SetSpeed_13(1, 0xb333, 0x5999);
    Motion_SetSpeed_14(2, 0xb333, 0x5999);
    Motion_SetSpeed_15(3, 0xb333, 0x5999);
    Motion_ResetPosMode2_9(0, 0x1c8, 184);
    Motion_CommitPos_9(0);
    Object_SetModeById_21(0, 1);
    Motion_ArmCb_16(0, 0x4000, 0);
    Motion_ResetPosMode2_10(1, 0x1d0, 0x100);
    Motion_CommitPos_10(1);
    Motion_ResetPosMode2_11(1, 0x1e0, 248);
    Motion_CommitPos_11(1);
    Motion_ResetPosMode2_12(1, 0x1b8, 192);
    Motion_CommitPos_12(1);
    Object_SetModeById_22(1, 1);
    Motion_ArmCb_17(1, 0x4000, 0);
    Motion_ResetPosMode2_13(3, 0x1e8, 248);
    Motion_CommitPos_13(3);
    Motion_ResetPosMode2_14(3, 0x1e8, 184);
    Motion_ResetPosMode2_15(2, 0x1d8, 184);
    Motion_CommitPos_14(2);
    Object_SetModeById_23(2, 1);
    Motion_ArmCb_18(2, 0x4000, 0);
    Motion_CommitPos_15(3);
    Object_SetModeById_24(3, 1);
    Motion_ArmCb_19(3, 0x4000, 0);
    Battle_WaitMode0_24(30);
    Battle_WaitMode0_25(60);
    script_b = (s32)gVal7;
    SceneWork_SetStepValue_14(script_b);
    BattleEv_RunWait_13(1, 0);
    Object_SetModeById_25(13, 6);
    Battle_WaitMode0_26(120);
    Motion_SetVarCb_5(13, 2);
    Battle_WaitMode0_27(60);
    Object_SetModeById_26(13, 7);
    SceneWork_SetStepValue_15((script_b + 1));
    BattleEv_RunWait_14(13, 0);
    Battle_WaitMode0_28(20);
    Object_LinkPair_3(3, 2, 0);
    Battle_WaitMode0_29(10);
    BattleFx_SpawnLinked_7(3, 0x101, 80);
    Motion_SetAngleToward_29(3, 13, 0);
    Motion_SetAngleToward_30(2, 13, 0);
    SceneWork_SetStepValue_16((script_b + 2));
    BattleEv_RunWait_15(3, 0);
    BattleFx_SpawnLinked_8(13, 0x102, 70);
    Motion_SetVarCb_6(13, 2);
    Battle_WaitMode0_30(60);
    Object_SetModeById_27(13, 5);
    Battle_WaitMode0_31(70);
    SceneWork_SetStepValue_17((script_b + 3));
    BattleEv_RunWait_16(13, 0);
    Object_SetModeById_28(13, 7);
    BattleFx_SpawnLinked_9(2, 0x108, 40);
    SceneWork_SetStepValue_18((script_b + 4));
    BattleEv_RunWait_17(2, 0);
    Motion_CallWaitAnim_3(12, 3);
    BattleFx_SpawnLinked_10(1, 0x103, 60);
    Motion_ResetPosMode2_16(1, 0x1b8, 208);
    Motion_CommitPos_16(1);
    Motion_SetAngleToward_31(1, 0, 0);
    SceneWork_SetStepValue_19((script_b + 5));
    BattleEv_RunWait_18(1, 0);
    Battle_WaitMode0_32(20);
    BattleFx_SpawnLinked_11(0, 0x101, 0);
    BattleFx_SpawnLinked_12(2, 0x101, 0);
    BattleFx_SpawnLinked_13(3, 0x101, 0);
    BattleFx_SpawnLinked_14(12, 0x101, 0);
    Battle_WaitMode0_33(70);
    Motion_SetAngleToward_32(1, 2, 0);
    BattleFx_SpawnLinked_15(1, 0x102, 75);
    Motion_SetAngleToward_33(1, 0, 0);
    Object_SetModeById_29(1, 4);
    SceneWork_SetStepValue_20((script_b + 6));
    BattleEv_RunWait_19(1, 0);
    BattleFx_SpawnLinked_16(12, 0x101, 0);
    Battle_WaitMode0_34(60);
    SceneWork_SetStepValue_21((script_b + 7));
    BattleEv_RunWait_20(12, 0);
    Motion_ResetPosMode2_17(1, 0x1c0, 208);
    Motion_CommitPos_17(1);
    Object_SetModeById_30(1, 1);
    Motion_SetAngleToward_34(1, 12, 0);
    SceneWork_SetStepValue_22((script_b + 8));
    BattleEv_RunWait_21(1, 0);
    Motion_SetVarCb_7(12, 1);
    Battle_WaitMode0_35(60);
    Motion_SetAngleToward_35(1, 12, 0);
    Battle_WaitMode0_36(60);
    SceneWork_SetStepValue_23((script_b + 9));
    BattleEv_RunWait_22(12, 0);
    Object_SetModeById_31(1, 4);
    Battle_WaitMode0_37(60);
    SceneWork_SetStepValue_24((script_b + 10));
    BattleEv_RunWait_23(1, 0);
    Motion_SetVarCb_8(2, 1);
    Battle_WaitMode0_38(30);
    SceneWork_SetStepValue_25((script_b + 11));
    BattleEv_RunWait_24(2, 0);
    Motion_SetAngleToward_36(1, 3, 0);
    Battle_WaitMode0_39(20);
    SceneWork_SetStepValue_26((script_b + 12));
    BattleEv_RunWait_25(1, 0);
    Battle_WaitMode0_40(30);
    BattleFx_SpawnLinked_17(3, 0x100, 80);
    Motion_SetAngleToward_37(3, 1, 0);
    SceneWork_SetStepValue_27((script_b + 13));
    BattleEv_RunWait_26(3, 0);
    Motion_SetAngleToward_38(3, 13, 0);
    SceneWork_SetStepValue_28((script_b + 14));
    BattleEv_RunWait_27(3, 0);
    Motion_SetAngleToward_39(0, 13, 0);
    Motion_SetAngleToward_40(2, 13, 0);
    Motion_SetAngleToward_41(1, 13, 0);
    Motion_SetAngleToward_42(3, 13, 0);
    Battle_WaitMode0_41(120);
    BattleFx_SpawnLinked_18(13, 0x102, 30);
    Motion_SetVarCb_9(13, 1);
    Battle_WaitMode0_42(120);
    Motion_SetVarCb_10(12, 1);
    Battle_WaitMode0_43(60);
    SceneWork_SetStepValue_29((script_b + 15));
    BattleEv_RunWait_28(12, 0);
    BattleFx_SpawnLinked_19(1, 0x107, 110);
    Motion_ResetPosMode2_18(1, 0x1c8, 212);
    Motion_CommitPos_18(1);
    Object_SetModeById_32(1, 1);
    SceneWork_SetStepValue_30((script_b + 16));
    BattleEv_RunWait_29(1, 0);
    Motion_SetAngleToward_43(2, 1, 0);
    Battle_WaitMode0_44(60);
    Motion_SetVarCb_11(2, 1);
    SceneWork_SetStepValue_31((script_b + 17));
    BattleEv_RunWait_30(2, 0);
    Motion_SetAngleToward_44(1, 13, 0);
    Battle_WaitMode0_45(80);
    Motion_SetAngleToward_45(1, 2, 0);
    Battle_WaitMode0_46(60);
    Motion_SetAngleToward_46(1, 13, 0);
    Battle_WaitMode0_47(30);
    Motion_SetAngleToward_47(0, 13, 0);
    Motion_SetAngleToward_48(3, 13, 0);
    Motion_SetAngleToward_49(13, 13, 0);
    Motion_SetAngleToward_50(12, 13, 0);
    Battle_WaitMode0_48(80);
    Motion_CallWaitAnim_4(1, 3);
    Battle_WaitMode0_49(30);
    SceneWork_SetStepValue_32((script_b + 18));
    BattleEv_RunWait_31(1, 0);
    Motion_SetVarCb_12(13, 2);
    Battle_WaitMode0_50(70);
    SceneWork_SetStepValue_33((script_b + 19));
    BattleEv_RunWait_32(13, 0);
    BattleFx_SpawnLinked_20(3, 0x100, 60);
    SceneWork_SetStepValue_34((script_b + 20));
    BattleEv_RunWait_33(3, 0);
    Motion_SetAngleToward_51(2, 0, 0);
    Battle_WaitMode0_51(80);
    SceneWork_SetStepValue_35((script_b + 21));
    Motion_SetSpeed_16(2, 0);
    Motion_SetAngleToward_52(1, 0, 0);
    Motion_SetAngleToward_53(3, 0, 0);
    Motion_SetAngleToward_54(2, 0, 0);
    Motion_SetAngleToward_55(12, 0, 0);
    Motion_SetHPosTerrain_9(14, 0x1c80000, 0x1300000);
    Motion_SetSpeed_17(14, 0x8000, 0x4000);
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        Audio_PlayCue_3(19);
        SceneWork_SetStepValue_36((script_b + 22));
        BattleEv_RunWait_34(14, 0);
    } else {
        Audio_PlayCue_4(19);
        SceneWork_SetStepValue_37((script_b + 23));
        BattleEv_RunWait_35(14, 0);
    }
    Motion_SetAngleToward_56(0, 14, 0);
    Motion_SetAngleToward_57(1, 14, 0);
    Motion_SetAngleToward_58(3, 14, 0);
    Motion_SetAngleToward_59(2, 14, 0);
    Motion_SetAngleToward_60(12, 14, 0);
    Motion_SetAngleToward_61(13, 14, 0);
    Motion_CamBounds_1(0x1c80000, -1, 0xf00000, 1);
    Motion_SetVarCb_13(13, 1);
    Battle_WaitMode0_52(60);
    Audio_PlayCue_5(8);
    script_c = (s32)gVal8;
    SceneWork_SetStepValue_38(script_c);
    BattleEv_RunWait_36(13, 0);
    Motion_ResetPosMode2_19(14, 0x1c8, 0x118);
    Motion_CommitPos_19(14);
    Motion_ResetPosMode2_20(14, 0x1b8, 0x100);
    Motion_CamBounds_2(0x1c80000, -1, 0xe00000, 1);
    SceneWork_SetStepValue_39((script_c + 1));
    BattleEv_RunWait_37(14, 0);
    Motion_CommitPos_20(14);
    Motion_ArmCb_20(14, 0xd000, 0);
    BattleFx_SpawnLinked_21(13, 0x102, 80);
    Object_SetModeById_33(14, 4);
    Battle_WaitMode0_53(89);
    SceneWork_SetStepValue_40((script_c + 2));
    BattleEv_RunWait_38(14, 0);
    Motion_SetVarCbObj_1(13, 2);
    Motion_CallWaitAnim_5(14, 3);
    SceneWork_SetStepValue_41((script_c + 3));
    BattleEv_RunWait_39(14, 0);
    BattleFx_SpawnLinked_22(13, 0x100, 80);
    Object_SetModeById_34(14, 4);
    Battle_WaitMode0_54(80);
    Motion_ArmCb_21(14, 0x3000, 0);
    Battle_WaitMode0_55(20);
    SceneWork_SetStepValue_42((script_c + 4));
    BattleEv_RunWait_40(14, 0);
    Motion_SetVarCb_14(13, 1);
    SceneWork_SetStepValue_43((script_c + 5));
    BattleEv_RunWait_41(13, 0);
    Battle_WaitMode0_56(30);
    Motion_CallWaitAnim_6(14, 3);
    Motion_SetAngleToward_62(14, 13, 0);
    Battle_WaitMode0_57(20);
    SceneWork_SetStepValue_44((script_c + 6));
    BattleEv_RunWait_42(14, 0);
    Motion_SetVarCb_15(13, 1);
    BattleFx_SpawnLinked_23(13, 0x102, 80);
    SceneWork_SetStepValue_45((script_c + 7));
    BattleEv_RunWait_43(13, 0);
    BattleFx_SpawnLinked_24(14, 0x103, 60);
    SceneWork_SetStepValue_46((script_c + 8));
    BattleEv_RunWait_44(14, 0);
    Motion_CallWaitAnim_7(14, 3);
    SceneWork_SetStepValue_47((script_c + 9));
    BattleEv_RunWait_45(14, 0);
    Battle_WaitMode0_58(20);
    Battle_WaitMode0_59(13, 0x101);
    Battle_WaitMode0_60(80);
    Battle_WaitMode0_61(13, 0);
    Motion_ResetPosMode2_21(14, 0x1f0, 240);
    Motion_CommitPos_21(14);
    Motion_SetAngleToward_63(14, 12, 0);
    Battle_WaitMode0_62(20);
    Motion_SetAngleToward_64(12, 14, 0);
    SceneWork_SetStepValue_48((script_c + 10));
    BattleEv_RunWait_46(14, 0);
    Motion_SetVarCb_16(12, 1);
    SceneWork_SetStepValue_49((script_c + 11));
    BattleEv_RunWait_47(12, 0);
    Battle_WaitMode0_63(40);
    Motion_CallWaitAnim_8(14, 3);
    Battle_WaitMode0_64(20);
    SceneWork_SetStepValue_50((script_c + 12));
    BattleEv_RunWait_48(14, 0);
    Object_SetModeById_35(14, 3);
    BattleFx_SpawnLinked_25(12, 0x102, 60);
    SceneWork_SetStepValue_51((script_c + 13));
    BattleEv_RunWait_49(12, 0);
    BattleFx_SpawnLinked_26(14, 0x100, 70);
    SceneWork_SetStepValue_52((script_c + 14));
    BattleEv_RunWait_50(14, 0);
    Object_SetModeById_36(12, 3);
    Battle_WaitMode0_65(140);
    Object_SetModeById_37(14, 3);
    Battle_WaitMode0_66(120);
    Object_LinkPair_4(0, 14, 0);
    Object_SetModeById_38(0, 3);
    Object_SetModeById_39(2, 3);
    Object_SetModeById_40(1, 3);
    Object_SetModeById_41(3, 3);
    Battle_WaitMode0_67(120);
    BattleFx_SpawnLinked_27(14, 0x108, 180);
    SceneWork_SetStepValue_53((script_c + 15));
    BattleEv_RunWait_51(14, 0);
    BattleFx_SpawnLinked_28(12, 0x101, 80);
    SceneWork_SetStepValue_54((script_c + 16));
    BattleEv_RunWait_52(12, 0);
    Object_LinkPair_5(0, 1, 0);
    Object_LinkPair_6(3, 2, 0);
    Battle_WaitMode0_68(80);
    Motion_SetAngleToward_65(14, 0, 0);
    Battle_WaitMode0_69(60);
    Motion_SetAngleToward_66(0, 14, 0);
    Motion_SetAngleToward_67(1, 14, 0);
    Motion_SetAngleToward_68(2, 14, 0);
    Motion_SetAngleToward_69(3, 14, 0);
    Battle_WaitMode0_70(60);
    SceneWork_SetStepValue_55((script_c + 17));
    BattleEv_RunWait_53(14, 0);
    BattleFx_SpawnLinked_29(0, 0x101, 0);
    BattleFx_SpawnLinked_30(1, 0x101, 0);
    BattleFx_SpawnLinked_31(2, 0x101, 0);
    BattleFx_SpawnLinked_32(3, 0x101, 0);
    Battle_WaitMode0_71(100);
    SceneWork_SetStepValue_56((script_c + 18));
    BattleEv_RunWait_54(1, 0);
    Motion_SetVarCb_17(2, 1);
    Battle_WaitMode0_72(60);
    SceneWork_SetStepValue_57((script_c + 19));
    BattleEv_RunWait_55(2, 0);
    Motion_SetAngleToward_70(14, 2, 0);
    Battle_WaitMode0_73(20);
    SceneWork_SetStepValue_58((script_c + 20));
    BattleEv_RunWait_56(14, 0);
    BattleFx_SpawnLinked_33(0, 0x101, 0);
    BattleFx_SpawnLinked_34(1, 0x101, 0);
    BattleFx_SpawnLinked_35(2, 0x101, 0);
    BattleFx_SpawnLinked_36(3, 0x101, 0);
    Battle_WaitMode0_74(120);
    Object_SetModeById_42(14, 4);
    Battle_WaitMode0_75(120);
    SceneWork_SetStepValue_59((script_c + 21));
    BattleEv_RunWait_57(14, 0);
    BattleFx_SpawnLinked_37(14, 0x102, 90);
    SceneWork_SetStepValue_60((script_c + 22));
    BattleEv_RunWait_58(14, 0);
    Object_SetModeById_43(0, 3);
    Object_SetModeById_44(2, 3);
    Object_SetModeById_45(3, 3);
    Object_SetModeById_46(1, 3);
    Object_SetModeById_47(12, 3);
    Battle_WaitMode0_76(80);
    Object_SetModeById_48(14, 4);
    Battle_WaitMode0_77(120);
    SceneWork_SetStepValue_61((script_c + 23));
    BattleEv_RunWait_59(14, 0);
    BattleFx_SpawnLinked_38(12, 0x100, 60);
    SceneWork_SetStepValue_62((script_c + 24));
    BattleEv_RunWait_60(12, 0);
    Motion_SetAngleToward_71(14, 12, 0);
    Battle_WaitMode0_78(20);
    Motion_SetVarCb_18(14, 1);
    Battle_WaitMode0_79(50);
    SceneWork_SetStepValue_63((script_c + 25));
    BattleEv_RunWait_61(14, 0);
    Object_SetModeById_49(1, 3);
    Battle_WaitMode0_80(70);
    SceneWork_SetStepValue_64((script_c + 26));
    BattleEv_RunWait_62(1, 0);
    Object_SetModeById_50(3, 3);
    SceneWork_SetStepValue_65((script_c + 27));
    BattleEv_RunWait_63(3, 0);
    Motion_SetAngleToward_72(14, 0, 0);
    Motion_CallWaitAnim_9(14, 3);
    Battle_WaitMode0_81(20);
    SceneWork_SetStepValue_66((script_c + 28));
    BattleEv_RunWait_64(14, 0);
    BattleFx_SpawnLinked_39(2, 0x101, 90);
    SceneWork_SetStepValue_67((script_c + 29));
    BattleEv_RunWait_65(2, 0);
    Motion_ArmCb_22(14, 0x5000, 0);
    Battle_WaitMode0_82(20);
    SceneWork_SetStepValue_68((script_c + 30));
    BattleEv_RunWait_66(14, 0);
    Motion_SetVarCb_19(12, 1);
    Battle_WaitMode0_83(60);
    SceneWork_SetStepValue_69((script_c + 31));
    BattleEv_RunWait_67(12, 0);
    Motion_SetAngleToward_73(14, 12, 0);
    Battle_WaitMode0_84(20);
    Motion_CallWaitAnim_10(14, 3);
    Battle_WaitMode0_85(20);
    SceneWork_SetStepValue_70((script_c + 32));
    BattleEv_RunWait_68(14, 0);
    BattleFx_SpawnLinked_40(1, 0x100, 70);
    SceneWork_SetStepValue_71((script_c + 33));
    BattleEv_RunWait_69(1, 0);
    Motion_SetAngleToward_74(14, 0, 0);
    Battle_WaitMode0_86(30);
    SceneWork_SetStepValue_72((script_c + 34));
    BattleEv_RunWait_70(14, 0);
    BattleFx_SpawnLinked_41(3, 0x100, 80);
    SceneWork_SetStepValue_73((script_c + 35));
    BattleEv_RunWait_71(3, 0);
    Motion_CallWaitAnim_11(14, 3);
    SceneWork_SetStepValue_74((script_c + 36));
    BattleEv_RunWait_72(14, 0);
    Motion_SetAngleToward_75(2, 0, 0);
    Battle_WaitMode0_87(30);
    Object_SetModeById_51(2, 3);
    Battle_WaitMode0_88(100);
    SceneWork_SetStepValue_75((script_c + 37));
    Motion_SetSpeed_18(2, 0);
    if (UiWork_WaitThenFinalizeCapacity_3(0, 0) != 0) {
        SceneWork_SetStepValue_76((script_c + 38));
        BattleEv_RunWait_73(14, 0);
    }
    Battle_WaitMode0_89(30);
    Object_SetModeById_52(1, 3);
    Object_SetModeById_53(2, 3);
    Object_SetModeById_54(3, 3);
    Battle_WaitMode0_90(100);
    Object_LinkPair_7(0, 12, 0);
    Object_SetModeById_55(0, 3);
    Object_SetModeById_56(12, 3);
    Battle_WaitMode0_91(100);
    Object_SetModeById_57(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_22(1);
    Motion_SetHPosTerrain_10(1, 0, 0);
    Object_SetModeById_58(2, 2);
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_2(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_23(2);
    Motion_SetHPosTerrain_11(2, 0, 0);
    Object_SetModeById_59(3, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_24(3);
    Motion_SetHPosTerrain_12(3, 0, 0);
    Battle_WaitMode0_92(30);
    Motion_SetSpeed_19(12, 0x6666, 0x3333);
    Motion_ResetPosMode2_22(12, 0x1d8, 184);
    Motion_CommitPos_25(12);
    Object_SetModeById_60(12, 1);
    Motion_ArmCb_23(12, 0x3000, 0);
    Motion_SetAngleToward_76(14, 0, 0);
    Object_SetModeById_61(14, 3);
    Battle_WaitMode0_93(20);
    Motion_SetAngleToward_77(14, 13, 0);
    Battle_WaitMode0_94(20);
    SceneWork_SetStepValue_77((script_c + 39));
    BattleEv_RunWait_74(14, 0);
    Motion_SetVarCbObj_2(13, 2);
    SceneWork_SetStepValue_78((script_c + 40));
    BattleEv_RunWait_75(13, 0);
    Object_SetModeById_62(0, 3);
    Battle_WaitMode0_95(30);
    Motion_SetSpeed_20(0, 0x10000, 0x8000);
    Motion_SetSpeed_21(12, 0x10000, 0x8000);
    Motion_SetPosReset_1(0, 1);
    Motion_ResetPosMode2_23(0, 0x1e0, 248);
    Battle_WaitMode0_96(40);
    Motion_SetActionVariant_2(12, 0);
    Motion_SetActionVariant_3(0, 0);
    Motion_ResetPosMode2_24(12, 0x1e0, 216);
    Motion_CommitPos_26(0);
    Motion_SetAngleToward_78(0, 14, 0);
    Motion_CommitPos_27(12);
    Motion_SetAngleToward_79(12, 14, 0);
    Motion_ArmCb_24(12, 0x3000, 0);
    Object_SetModeById_63(0, 3);
    Battle_WaitMode0_97(5);
    Object_SetModeById_64(12, 3);
    Battle_WaitMode0_98(100);
    Object_SetModeById_65(14, 3);
    Battle_WaitMode0_99(100);
    Motion_ResetPosMode2_25(12, 0x1e0, 248);
    Motion_ResetPosMode2_26(0, 0x1c8, 248);
    Motion_CommitPos_28(12);
    Motion_ResetPosMode2_27(12, 0x1c8, 248);
    Motion_CommitPos_29(0);
    Motion_ResetPosMode2_28(0, 0x1c8, 0x168);
    Motion_CommitPos_30(12);
    Motion_ResetPosMode2_29(12, 0x1c8, 0x168);
    Motion_CommitPos_31(0);
    Motion_ResetPosMode2_30(0, 0x160, 0x168);
    Motion_CommitPos_32(12);
    Motion_ResetPosMode2_31(12, 0x160, 0x168);
    Battle_WaitMode0_100(20);
    Audio_PlayCue_6(17);
    SCENE_PHASE = 0x203;
    Map_unk158_3();
    Battle_WaitMode0_101(1);
    Battle_WaitMode0_102(210);
    Map_unk159_3(4);
    Battle_SchedShoulder_1();
}

/* map/locations/heidia/village/party_animation.c */

void Map_unk160_3(); void Map_unk161_3();

void Scene_RunScene3bf4(void)
{
    u32 i;
    s32 record;

    Map_unk31_3(2, 0x10000, 0x8000);
    Map_unk32_3(2, 0x1f8, 216);
    Map_unk33_3(3, 0x10000, 0x8000);
    Map_unk34_3(3, 0x1b8, 232);
    Map_unk35_3(1, 0x10000, 0x8000);
    Map_unk36_3(1, 0x1e0, 224);
    Map_unk162_3(1);
    Map_unk37_3(1, 0xc000, 0);
    Map_unk163_3(2);
    Map_unk38_3(2, 0xc000, 0);
    Map_unk164_3(3);
    Map_unk39_3(3, 0xc000, 0);
    Map_unk40_3(0, 0xc000, 0);
}

/* map/locations/heidia/village/scene_party_placement.c */

void Scene_RunScene3bf5(void)
{
    u32 i;
    s32 record;

    record = Map_unk20(0);
    if (record != 0) {
        Map_unk165_3(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Map_unk21(0);
    if (record != 0) {
        Map_unk166_3(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Map_unk22(0);
    if (record != 0) {
        Map_unk167_3(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Map_unk168_3(0, 0, 0);
    Map_unk41_3(2, 0xb333, 0x5999);
    Map_unk42_3(2, 0x1c8, 192);
    Map_unk43_3(3, 0xb333, 0x5999);
    Map_unk44_3(3, 0x1b8, 184);
    Map_unk45_3(1, 0xb333, 0x5999);
    Map_unk46_3(1, 0x1c0, 240);
    Map_unk169_3(2);
    Map_unk170_3(2, 12, 0);
    Map_unk171_3(1);
    Map_unk172_3(3);
    Map_unk173_3(1, 12, 0);
    Map_unk174_3(3, 12, 0);
    Map_unk175_3(15);
}

/* map/locations/heidia/village/scene_sequence.c */
/* Audited retained supplemental scene body.
 * The complete production span preserves 49 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

/* Two id arguments recur across most of the calls below: 24 is passed as the
 * first argument throughout the main sequence, and 25 is introduced only
 * after Motion_SetAngleToward_2(25, 24, 0), whose second argument is 24 -- so 25 is
 * derived from 24 partway through the sequence. */
#define PRIMARY_ID 24
#define DERIVED_ID 25

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); a sequence id counting up from
 * 9569 is threaded through four of the calls one apart. Each of the two
 * outer gating checks has its own short fallback branch on PRIMARY_ID. */
void Scene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet_1(2369) != 0) {
        if (GameFlag_IsSet_2(2382) == 0 && GameFlag_IsSet_3(788) == 0) {
        sequence_id = 9569;
        SceneWork_SetStepValue_1(sequence_id);
        BattleEv_RunWait_1(PRIMARY_ID, 0);
        Motion_SetVarCbObj_1(PRIMARY_ID, 1);
        Battle_WaitMode0_1(30);
        Motion_SetSpeed_1(PRIMARY_ID, 6553, 3276);
        Motion_OffsetPositionAndResetMotion_1(PRIMARY_ID, -4, 0);
        Motion_CommitPos_1(PRIMARY_ID);
        Object_SetModeById_1(PRIMARY_ID, 3);
        Battle_WaitMode0_2(60);
        Motion_SetSpeed_2(PRIMARY_ID, 13107, 6553);
        Motion_OffsetPositionAndResetMotion_2(PRIMARY_ID, -6, 0);
        Motion_SetAngleToward_1(PRIMARY_ID, 0, 0);
        Motion_CommitPos_2(PRIMARY_ID);
        SceneWork_SetStepValue_2(sequence_id + 1);
        BattleEv_RunWait_2(PRIMARY_ID, 0);
        Motion_SetVarCbObj_2(PRIMARY_ID, 1);
        Motion_SetAngleToward_2(DERIVED_ID, PRIMARY_ID, 0);
        SceneWork_SetStepValue_3(sequence_id + 2);
        BattleEv_RunWait_3(PRIMARY_ID, 0);
        Battle_WaitMode0_3(70);
        Object_SetModeById_2(DERIVED_ID, 3);
        Battle_WaitMode0_4(60);
        Motion_SetSpeed_3(DERIVED_ID, 26214, 13107);
        Motion_ResetPosMode2_1(DERIVED_ID, 880, 112);
        Motion_CommitPos_3(DERIVED_ID);
        Motion_ArmCb_1(DERIVED_ID, 53248, 0);
        SceneWork_SetStepValue_4(sequence_id + 3);
        BattleEv_RunWait_4(PRIMARY_ID, 0);
        Object_SetModeById_3(PRIMARY_ID, 3);
        Battle_WaitMode0_5(70);
        Motion_OffsetPositionAndResetMotion_3(PRIMARY_ID, 8, 0);
        Motion_CommitPos_4(PRIMARY_ID);
        Object_SetModeById_4(PRIMARY_ID, 5);
        SceneWork_SetStepValue_5(sequence_id + 4);
        BattleEv_RunWait_5(PRIMARY_ID, 0);
        Motion_ResetPosMode2_2(0, 896, 120);
        Motion_CommitPos_5(0);
        Object_LinkPair_1(0, DERIVED_ID, 0);
        Battle_WaitMode0_6(60);
        Object_SetModeById_5(DERIVED_ID, 3);
        Battle_WaitMode0_7(30);
        GameFlag_Set_1(788);
        } else {
            SceneWork_SetStepValue_6(9575);
            BattleEv_RunWait_6(PRIMARY_ID, 0);
        }
    } else {
        SceneWork_SetStepValue_7(0x244d);
        BattleEv_RunWait_7(PRIMARY_ID, 0);
    }
}

/* map/locations/heidia/village/scene_transition.c */

void Scene_RunScene3bf6(void)
{
    u32 i;
    s32 record;

    Map_unk47_3(0x40000, 0x40000, 0x10000);
    Map_unk176_3(141);
    Map_unk177_3(80);
    Map_unk11_2(0x120);
    Map_unk178_3(5);
    Map_unk179_3(145);
    Map_unk5_5(16, 75, 7, 4, 26, 55);
    Map_unk48_3(-1, -1, 0xe666);
    Map_unk49_3(0, 0x100, 0);
    Map_unk50_3(1, 0x100, 0);
    Map_unk51_3(2, 0x100, 0);
    Map_unk52_3(3, 0x100, 0);
    Map_unk53_3(12, 0x100, 0);
    Map_unk180_3(60);
}

/* map/locations/heidia/village/supplemental_scene.c */
extern u8 gVal9[];
extern u8 gVal10[];

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at gVal10 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at gCell
 * before handing off to the next step. */
void Scene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;
    s32 sequence_2438;
    s32 status_record_2000240;

    Object_SetModeById_1(0, 1);
    Object_SetModeById_2(12, 1);
    Object_SetModeById_3(13, 1);
    Object_SetModeById_4(14, 1);
    Audio_PlayCue_1(113);
    BattleFx_SpawnLinked_1(12, 0x100, 0);
    Battle_WaitMode0_1(30);
    Motion_SetAngleToward_1(12, 0, 0);
    sequence_2438 = (s32)gVal10;
    SceneWork_SetStepValue_1(sequence_2438);
    BattleEv_RunWait_1(12, 0);
    BattleFx_SpawnLinked_2(13, 0x100, 0);
    BattleFx_SpawnLinked_3(14, 0x100, 0);
    Motion_SetAngleToward_2(13, 0, 0);
    Motion_SetAngleToward_3(14, 0, 0);
    Motion_SetAngleToward_4(0, 13, 0);
    Battle_WaitMode0_2(65);
    Motion_ArmCb_1(13, 0x5000, 0);
    Motion_ArmCb_2(14, 0xd000, 0);
    SceneWork_SetStepValue_2((sequence_2438 + 1));
    BattleEv_RunWait_2(13, 0);
    Motion_CallWaitAnim_1(14, 3);
    SceneWork_SetStepValue_3((sequence_2438 + 2));
    BattleEv_RunWait_3(14, 0);
    SceneWork_SetStepValue_4((sequence_2438 + 3));
    BattleEv_RunWait_4(12, 0);
    Motion_SetVarCbObj_1(13, 1);
    SceneWork_SetStepValue_5((sequence_2438 + 4));
    BattleEv_RunWait_5(13, 0);
    Motion_CallWaitAnim_2(14, 3);
    SceneWork_SetStepValue_6((sequence_2438 + 5));
    BattleEv_RunWait_6(14, 0);
    Motion_CallWaitAnim_3(14, 3);
    Battle_WaitMode0_3(60);
    Motion_SetAngleToward_5(13, 0, 0);
    Motion_SetAngleToward_6(14, 0, 0);
    Battle_WaitMode0_4(70);
    Motion_ResetPosMode2_1(12, 0x2a0, 88); /* object_id 12, x 0x2a0, z 88 */
    Motion_CommitPos_1(12);
    Motion_SetAngleToward_7(12, 0, 0);
    Motion_CallWaitAnim_4(12, 3);
    Battle_WaitMode0_5(30);
    SceneWork_SetStepValue_7((sequence_2438 + 6));
    BattleEv_RunWait_7(12, 0);
    SCENE_PHASE = 0x200;
    SharedWorkData_SetFirstAndSecondFields_1((s32)gVal9, 31);
    status_record_2000240 = (s32)gCell;
    /* Status byte at +0x22b of the record. */
    *(u8 *)((status_record_2000240 + 0x22b)) = 3;
    BattleFx_ComputeWeightedResultAndDispatch_1(98, 3);
    Motion_SetHPosTerrain_1(12, 0, 0);
    Motion_SetHPosTerrain_2(13, 0, 0);
    Motion_SetHPosTerrain_3(14, 0, 0);
    Battle_SchedShoulder_1();
    GameFlag_Set_1(0x94a); /* main:080770c8 */
}

/* map/locations/heidia/village/scene/seq_tail.c */
void Scene_RunSequenceTail(void)
{
    Battle_Reset_1();
    Motion_SetHPosTerrain_1(12, 45088768, 5767168); /* object_id 12, x, z */
    Motion_SetHPosTerrain_2(13, 46137344, 5767168); /* object_id 13, x, z */
    Motion_SetHPosTerrain_3(14, 47185920, 6291456); /* object_id 14, x, z */
    Object_SetModeById_1(12, 5); /* object_id 12, action 5 */
    Object_SetModeById_2(13, 5); /* object_id 13, action 5 */
    Object_SetModeById_3(14, 5); /* object_id 14, action 5 */
    Motion_SetAngleToward_1(0, 13, 0);
    Battle_SchedShoulder_1();
    Battle_WaitMode0_1(); /* main:0808a360 */
}

/* map/locations/heidia/village/scene/actor_pair.c */

void Scene_RunScene3bfActorPair(void)
{
    u32 i;
    s32 record;

    Map_RunActorPair(11, 1);
    Map_unk2_4ActorPair();
    Map_unk3_4ActorPair(60);
    Map_Do(0x247c);
    Map_unk4_4ActorPair(13, 0);
    Map_Place(11, 0x10000, 0x8000);
    Map_unk2_3(15, 0x10000, 0x8000);
    Map_unk3_3(11, 0x1d8, 180);
    Map_unk4_3(15, 0x1d8, 180);
    Map_unk5_4ActorPair(11, 1);
    Map_unk6_4ActorPair(11);
    Map_unk7_4ActorPair(11, 4);
    Map_unk8_4ActorPair(30);
}

/* map/locations/heidia/village/add_soft_float_parts.c */
/* map/locations/heidia/village/soft/add_parts.c */
/* Second embedded copy of the compiler runtime's soft-float addition core. */

extern SoftFloatRecord *GetSoftFloatNaNRecord(void);

SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
}

/* map/locations/heidia/village/soft/add.c */
/*
 * Soft-float double addition for overlay resource_3bf.  A packed double is
 * carried as SoftDouble, a u64 whose low half is the r0 word: this library
 * keeps the sign and exponent in r0 and the low word in r1, which is not the
 * native layout of a C `double`.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *Map_RunSoftFloatParts(SoftFloatRecord *left, SoftFloatRecord *right, SoftFloatRecord *result);

/*
 * Unpacks both operands into 20-byte records, runs the arithmetic core, and
 * repacks the result.  The frame members are declared in the order that
 * reproduces the stack layout.  The record layout past the sign word at
 * offset 4 is named by position and not verified, so it is kept opaque.
 */
SoftDouble AddSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packedB;
        SoftDouble packedA;
        SoftFloatRecord result;
        SoftFloatRecord recordB;
        SoftFloatRecord recordA;
    } frame;
    u32 *packedAWords = (u32 *)&frame.packedA;
    volatile u32 *packedBWords = (u32 *)&frame.packedB;

    packedAWords[0] = a0;
    packedAWords[1] = a1;
    packedBWords[0] = b0;
    packedBWords[1] = b1;

    Map_ApplySoftFloatParts(&frame.packedA, &frame.recordA);
    Map_Apply2SoftFloatParts(&frame.packedB, &frame.recordB);

    return Map_Do(Map_RunSoftFloatParts(&frame.recordA, &frame.recordB, &frame.result));
}

/* map/locations/heidia/village/soft/sub.c */
/*
 * Soft-float double subtraction for overlay resource_3bf.  A packed double is
 * carried as SoftDouble, a u64 whose low half is the r0 word: this library
 * keeps the sign and exponent in r0 and the low word in r1, which is not the
 * native layout of a C `double`.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *Map_RunSoftFloatParts(SoftFloatRecord *left, SoftFloatRecord *right, SoftFloatRecord *result);

/*
 * Unpacks both operands into 20-byte records, runs the shared addition core,
 * and repacks the result.  The frame members are declared in the order that
 * reproduces the stack layout.  The record layout past the sign word at
 * offset 4 is named by position and not verified, so it is kept opaque.
 */
SoftDouble SubtractSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packedB;
        SoftDouble packedA;
        SoftFloatRecord result;
        SoftFloatRecord recordB;
        SoftFloatRecord recordA;
    } frame;
    u32 *packedAWords = (u32 *)&frame.packedA;
    volatile u32 *packedBWords = (u32 *)&frame.packedB;

    packedAWords[0] = a0;
    packedAWords[1] = a1;
    packedBWords[0] = b0;
    packedBWords[1] = b1;

    Map_ApplySoftFloatParts(&frame.packedA, &frame.recordA);
    {
        SoftFloatRecord *recordB = &frame.recordB;

        Map_Apply2SoftFloatParts(&frame.packedB, recordB);

        /* Toggling the sign word of the unpacked second operand turns the
         * shared addition core into a subtraction. */
        recordB->word[1] ^= 1u;

        return Map_Do(Map_RunSoftFloatParts(&frame.recordA, recordB, &frame.result));
    }
}

/* map/locations/heidia/village/scene/status.c */
extern u8 gOvSoftFloatParts;

u8 *GetSoftFloatStatusStorage(void)
{
    return &gOvSoftFloatParts;
}

/* map/locations/heidia/village/soft/a_nan.c */
s32 SoftFloatRecordAIsNaN(u32 *state)
{
    return *state <= 1;
}

/* map/locations/heidia/village/soft/a_inf.c */
s32 SoftFloatRecordAIsInfinity(s32 *state)
{
    return *state == 4;
}

/* map/locations/heidia/village/soft/a_zero.c */
s32 SoftFloatRecordAIsZero(s32 *state)
{
    s32 match = 0;

    if (*state == 2) {
        match = 1;
    }
    return match;
}

/* map/locations/heidia/village/soft/from_int.c */
/*
 * Signed integer to soft-float conversion for resource_3bf.  The 124-byte
 * owner is 112 bytes of code plus its three pool words.  It is a leaf on
 * the non-interworking ABI, with r4 callee-saved.
 */

/*
 * The 20-byte unpacked record the library packs from:
 *   +0  class (2 = zero, 3 = finite)
 *   +4  sign (1 for a negative input)
 *   +8  exponent seed (60)
 *   +12 significand, 64 bits
 *
 * The significand must stay one 64-bit field.  Split into two words, the
 * normalising loop keeps the pair in registers instead of loading and
 * storing it through the record on every iteration.  r0 carries the high
 * word of the packed double, so the packed value is a u64 and not a double.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

/* The packer, reached by a direct call rather than through a veneer. */

/*
 * A zero input takes class 2 straight to the pack call, and 0x80000000
 * returns its packed value directly.  The guard before the normalising loop
 * is an unsigned compare, so a significand already above 0x0fffffffffffffff
 * skips normalisation entirely.
 */
SoftDouble SignedIntToSoftDouble(s32 value)
{
    SoftFloatRecord record;
    u32 sign;

    record.cls = 3u;
    sign = (u32)value >> 31;
    record.sign = sign;

    if (value == 0) {
        record.cls = 2u;
        goto pack;
    }

    record.exponent = 60;

    if (sign != 0u) {
        if ((u32)value == 0x80000000u) {
            return (SoftDouble)0xC1E00000u;
        }
        record.fraction = (u64)(s64)(-value);
    } else {
        record.fraction = (u64)(s64)value;
    }

    while (record.fraction <= 0x0FFFFFFFFFFFFFFFu) {
        record.fraction = record.fraction << 1;
        record.exponent -= 1;
    }

pack:
    return Map_Do(&record);
}

/* map/locations/heidia/village/soft/to_int.c */
/*
 * Resource 3bf soft-double to signed-integer conversion at 0x02005b6c.
 * The packed value arrives as the library's two ABI words: r0 is the high
 * word and r1 is the low word.  The unpacker consumes their address, so the
 * packed stack slot remains volatile while the unpacked record does not.
 */
typedef u64 SoftDouble;

typedef union FloUnion { SoftDouble value; struct { u32 lo; u32 hi; } words; } FloUnion;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

s32 SoftDoubleToSignedInt(u32 high, u32 low)
{
    SoftFloatRecord record;
    volatile FloUnion au;
    volatile FloUnion *slot;
    s32 exponent;
    u64 shifted;

    slot = &au;
    slot->words.lo = high;
    slot->words.hi = low;
    Map_ApplySoftFloatParts((FloUnion *)slot, &record);

    if (Map_Do(&record) != 0u) {
        return 0;
    }
    if (Map_unk2_2(&record) != 0u) {
        return 0;
    }
    if (Map_unk3_2(&record) == 0u) {
        exponent = record.exponent;
        if (exponent < 0) {
            return 0;
        }
        if (exponent <= 30) {
            goto convert;
        }
    }
    return 0x7fffffff + (s32)(record.sign != 0u);

convert:
    shifted = Map_Apply2SoftFloatParts(record.fraction, (u32)(60 - exponent));
    if (record.sign != 0u) {
        return -(s32)shifted;
    }
    return (s32)shifted;
}

/* map/locations/heidia/village/soft/b_nan.c */
s32 SoftFloatRecordBIsNaN(u32 *state)
{
    return *state <= 1;
}

/* map/locations/heidia/village/soft/b_inf.c */
s32 SoftFloatRecordBIsInfinity(s32 *state)
{
    return *state == 4;
}

/* map/locations/heidia/village/soft/b_zero.c */
s32 SoftFloatRecordBIsZero(s32 *state)
{
    return *state == 2;
}

/* map/locations/heidia/village/soft/lsr64.c */
/* resource_3bf soft-float library: 64-bit logical right shift at 0x02005c08. */

/*
 * r0/r1 hold the low and high words and r2 the count -- the ordinary pair
 * placement for an unsigned long long, so it is spelled that way here.  A
 * count of 32 or more takes the `32 - n <= 0' path with a shift distance of
 * zero, avoiding an undefined `>> 32'; counts of 64 and over are unmasked
 * here and undefined in the original too.  The else arm's statement order --
 * carry, shifted low, high, then the OR -- is load-bearing, as is zeroing the
 * high half first in the other arm.
 */
typedef union {
    u64 whole;
    struct {
        u32 lo;
        u32 hi;
    } parts;
} SplitU64;

u64 LogicalShiftRight64(u64 value, u32 count)
{
    SplitU64 in;
    SplitU64 out;
    s32 spare;
    u32 carry;
    u32 shifted;

    if (count == 0u) {
        return value;
    }
    in.whole = value;
    spare = 32 - (s32)count;
    if (spare <= 0) {
        spare = -spare;
        out.parts.hi = 0u;
        out.parts.lo = in.parts.hi >> spare;
    } else {
        carry = in.parts.hi << spare;
        shifted = in.parts.lo >> count;
        out.parts.hi = in.parts.hi >> count;
        out.parts.lo = shifted | carry;
    }
    return out.whole;
}

/* map/locations/heidia/village/soft/pack.c */
/*
 * Double-precision packer from the GCC soft-float runtime shape.  This overlay
 * copy calls the shared runtime predicates and shift helper at its witnessed
 * targets.  The final word swap is the runtime's FLOAT_WORD_ORDER_MISMATCH
 * path and preserves the packed-word order observed by its callers.
 */
typedef u64 SoftDouble;

enum SoftFloatClass {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
};

typedef struct SoftFloatRecord {
    enum SoftFloatClass cls;
    u32 sign;
    s32 exponent;
    union {
        u64 ll;
        u32 words[2];
    } fraction;
} SoftFloatRecord;

typedef union SoftDoubleUnion {
    SoftDouble value;
    u64 raw;
    u32 words[2];
    struct {
        u64 fraction : 52 __attribute__((packed));
        u32 exponent : 11 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
} SoftDoubleUnion;

SoftDouble PackSoftDouble(SoftFloatRecord *src)
{
    SoftDoubleUnion dst;
    u64 fraction = src->fraction.ll;
    s32 sign = (s32)src->sign;
    s32 exp = 0;

    if (Map_Check(src)) {
        exp = 0x7ff;
        if (src->cls == CLASS_QNAN || 1) {
            fraction |= 0x8000000000000ULL;
        }
    } else if (Map_unk2(src)) {
        exp = 0x7ff;
        fraction = 0;
    } else if (Map_unk3(src)) {
        exp = 0;
        fraction = 0;
    } else if (fraction == 0) {
        exp = 0;
    } else {
        if (src->exponent < -1022) {
            s32 shift = -1022 - src->exponent;

            exp = 0;
            if (shift > 56) {
                fraction = 0;
            } else {
                s32 lowbit = (fraction & ((1 << shift) - 1)) ? 1 : 0;
                fraction = Map_ApplySoftFloatParts(fraction, (u32)shift) | (u32)lowbit;
            }
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 60)) {
                exp += 1;
            }
            fraction >>= 8;
        } else if (src->exponent > 1023) {
            exp = 0x7ff;
            fraction = 0;
        } else {
            exp = src->exponent + 1023;
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 61)) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 8;
        }
    }

    dst.bits.fraction = fraction;
    dst.bits.exponent = (u32)exp;
    dst.bits.sign = (u32)sign;

    {
        u32 tmp = dst.words[0];
        dst.words[0] = dst.words[1];
        dst.words[1] = tmp;
    }

    return dst.value;
}

/* map/locations/heidia/village/soft/c_nan.c */
s32 SoftFloatRecordCIsNaN(u32 *state)
{
    return *state <= 1;
}

/* map/locations/heidia/village/soft/c_inf.c */
s32 SoftFloatRecordCIsInfinity(s32 *state)
{
    return *state == 4;
}

/* map/locations/heidia/village/soft/c_zero.c */
s32 SoftFloatRecordCIsZero(s32 *state)
{
    return *state == 2;
}

/* map/locations/heidia/village/soft/unpack.c */
/*
 * Soft-float double unpack for overlay resource_3bf.
 */
typedef float DoubleType __attribute__((mode(DF)));
typedef unsigned int HalfFractionType __attribute__((mode(SI)));
typedef unsigned int FractionType __attribute__((mode(DI)));

typedef enum FloatClass {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
} FloatClass;

/*
 * The exponent is unbiased and the significand is normalised so that bit 28
 * of its high half is set.
 */
typedef struct SoftFloatRecord {
    FloatClass cls;
    unsigned int sign;
    int exponent;
    union {
        FractionType whole;
        HalfFractionType halves[2];
    } fraction;
} SoftFloatRecord;

typedef union PackedDouble {
    DoubleType value;
    FractionType raw;
    HalfFractionType words[2];
    struct {
        FractionType fraction : 52 __attribute__((packed));
        unsigned int exponent : 11 __attribute__((packed));
        unsigned int sign : 1 __attribute__((packed));
    } bits;
} PackedDouble;

/*
 * Unpacks a packed double into record. The pair is copied to a local union
 * with its two words swapped, so the sign and exponent come from the IEEE
 * high word and are read as packed bitfields rather than reassembled.
 */
void UnpackSoftDouble(PackedDouble *source, SoftFloatRecord *record)
{
    FractionType fraction;
    int exponent;
    int sign;
    PackedDouble swapped;

    swapped.words[0] = source->words[1];
    swapped.words[1] = source->words[0];
    source = &swapped;

    fraction = source->bits.fraction;
    exponent = source->bits.exponent;
    sign = source->bits.sign;

    record->sign = sign;
    if (exponent == 0) {
        if (fraction == 0) {
            record->cls = CLASS_ZERO;
        } else {
            record->exponent = exponent - 1023 + 1;
            fraction <<= 8;
            record->cls = CLASS_NUMBER;
            while (fraction < (((FractionType)1) << (52 + 8))) {
                fraction <<= 1;
                record->exponent--;
            }
            record->fraction.whole = fraction;
        }
    } else if (exponent == 0x7ff) {
        if (fraction == 0) {
            record->cls = CLASS_INFINITY;
        } else {
            if (fraction & 0x8000000000000LL) {
                record->cls = CLASS_QNAN;
            } else {
                record->cls = CLASS_SNAN;
            }
            record->fraction.whole = fraction;
        }
    } else {
        record->exponent = exponent - 1023;
        record->cls = CLASS_NUMBER;
        record->fraction.whole = (fraction << 8) | (((FractionType)1) << (52 + 8));
    }
}
