#include "types.h"
#include "scene.h"
#include "staged_actor.h"

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
extern u8 *gWork;

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
extern u8 gCell[];

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
extern s16 gCell[];
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
extern u8 Value_000000a3;
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
        Map_unk57_4();
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

extern u8 *gWork;

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
