#include "types.h"
#include "scene.h"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "configured_effect_spawn.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/actor/selected_presentation/selected_presentation.c */
/* overlays/scene/actor/selected_presentation/actor_presentation.c */

/* Actor-step entry points for the two scene phases. */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_unk3_3(actor, x, y);
}

void Scene_RunActor8Step(void) { Actor_unk2_2(8); }

void Scene_RunActor9Step(void) { Actor_unk3_2(9); }

void Scene_RunActor10Step(void) { Actor_unk4_2(10); }

void Scene_RunActor11Step(void) { Actor_unk5_2(11); }

void Scene_RunActor12Step(void) { Actor_unk6_2(12); }

void Scene_RunLateActor8Step(void) { Actor_unk7_2(8); }

void Scene_RunLateActor9Step(void) { Actor_unk8_2(9); }

void Scene_RunLateActor10Step(void) { Actor_unk9_2(10); }

void Scene_RunLateActor11Step(void) { Actor_unk10_2(11); }

void Scene_RunLateActor12Step(void) { Actor_unk11_2(12); }

void PlaceActorTwelveWhenFlagClear(void)
{
    if (Actor_unk7(2487) == 0) {
        Actor_unk12_2(526);
        PlaceActor(12, 240 << 15, 206 << 18);
        Actor_Apply(12, (const void *)0x020097a8);
    }
}

/* overlays/scene/actor/selected_presentation/encounter_palette.c */
#define FrameCounter (*(u32 *)0x03001e40)
#define EncounterPalette (*(volatile u16 *)0x0500019e)

void EncounterPalette_Pulse(void)
{
    u16 phase = FrameCounter & 63;
    s32 level;

    if (phase > 31)
        phase = 64 - phase;
    level = (phase >> 1) + 7;
    level |= (level << 10) | (level << 5);
    EncounterPalette = ((u32)level << 16) >> 16;
}

/* overlays/scene/actor/selected_presentation/overlay_object_head.c */

void *Actor_unk4_4(s32, s32, s32, s32);

void *OvObj_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Actor_unk4_4(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *sprite = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = sprite[9];
        mask = -mask;
        mask &= flags;
        sprite[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Actor_Apply2(obj, 0);
        Actor_Apply3(obj, 14);
        Actor_Apply4(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/* overlays/scene/actor/selected_presentation/scene_data_head.c */
#define PrimaryRuntimeSelector Value_000000a4
#define SecondaryRuntimeSelector Value_000000a5
#define TertiaryRuntimeSelector Value_000000a6

/* Contiguous unnamed leaf-owner run for resource_3c0. */

s32 SceneData_SelectDataByRuntimeSelector(void)
{
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTable95c0(void)
{
    return (u8 *)0x020095c0;
}

/* overlays/scene/actor/selected_presentation/scene_data_tail.c */
#define PrimaryRuntimeSelector Value_000000a4
#define SecondaryRuntimeSelector Value_000000a5
#define TertiaryRuntimeSelector Value_000000a6

extern s16 RuntimeSelectorTable[];
extern u8 Value_000000a6;
extern u8 gOv[];
extern u8 gOv2[];

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
}

s32 SceneData_SelectTable99C4Or97B4ByStateA6(void)
{
    if (RuntimeSelectorTable[224] == (s32)&Value_000000a6) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

/* overlays/scene/actor/selected_presentation/scene_primary_script.c */
#define ACTOR_ID 13

extern u8 gWork[];
extern u8 gVal[];
extern u8 gOv3[];
extern s16 gCell[][1];

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

/* Actor id operated on throughout this scene. */

/* Resolved from the main-image function's own source (not registered in the
 * overlay veneer table): each macro names what that source shows the
 * function doing, keeping the site's own calling form. */

void Scene_RunMiddleAuxiliarySequence(s32 a0)
{
    s32 p10;
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = RuntimeSelectorTable;
    p6 = *(volatile s32 *)(base + 500);
    p10 = a0;
    rec7 = Actor_Check((s32)p6);
    Actor_unk5_4(p10);
    rec2 = Actor_unk2(0x20f);
    if (rec2 == 0) {
        Actor_unk6_4();
        Actor_Run((s32)p6, 0x101);
        Actor_unk7_4((s32)p6, 9);
        record = Actor_unk3(p10);
        if (record != 0) {
            Actor_unk8_4((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_unk9_4((s32)p6);
        Actor_unk10_4(244);
        Actor_unk4(0x2008401, 0xc80);
        rec7[85] = rec2;
        {
            s32 v12 = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;
            Actor_unk11_4((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), v12, *(volatile s32 *)((s32)rec7 + 16));
        }
        Actor_unk12_4((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Actor_Do(0x20f);
        Actor_unk2_4(0x218, p10);
        Actor_unk3_4(0x210, 180);
        Actor_unk13_4();
        *(u16 *)((*(u8 *volatile *)gWork + 0x17c)) = rec2;
    }
}

/* Runs a scene step only when three preceding gate checks all pass;
 * otherwise the function does nothing. Drives actor ACTOR_ID through a
 * long sequence of position/pose/flag updates, applying two record-driven
 * position fixups read back from lookup calls partway through. */
void Scene_RunActorThirteenRestoration(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet_2(0x1b7) != 0) {
        } else {
            if (GameFlag_IsSet_3(0x9b0) == 0) {
            } else {
                GameFlag_Set_1(0x9b5);
                Battle_Reset_1();
                SceneWork_SetStepValue_1(0x2633);
                /* Record layout observed here: s32 at +8, s32 at +16. */
                record = Scene_GetRecord_1(0);
                if (record != 0) {
                    Motion_SetHPosTerrain_1(ACTOR_ID, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                Motion_SetAngleToward_1(ACTOR_ID, 0xc000, 0);
                Motion_SetPosReset_1(0, 0x1b8, 0x4e8);
                Motion_ArmCb_1(ACTOR_ID, 0x4000, 0);
                Motion_SetPosReset_2(0, 0x1bc, 0x4d8);
                BattleFx_SpawnLinked_1(0, 0x100, 40);
                Motion_ArmCb_2(0, 0x4000, 30);
                Motion_CallWaitAnim_1(ACTOR_ID, 4);
                BattleEv_RunWait_1(ACTOR_ID, 0);
                BattleFx_SpawnLinked_2(0, 0x105, 60);
                BattleFx_SpawnLinked_3(ACTOR_ID, 0x105, 60);
                BattleEv_RunWait_2(ACTOR_ID, 0);
                Battle_WaitMode0_1(30);
                Motion_SetVarCbObj_1(ACTOR_ID, 2);
                BattleEv_RunWait_3(ACTOR_ID, 0);
                Motion_ArmCb_3(ACTOR_ID, 0xc000, 30);
                BattleEventRuntime_ProcessAction_1(ACTOR_ID, 0);
                Battle_WaitMode0_2(30);
                BattleFx_SpawnLinked_4(ACTOR_ID, 0x106, 60);
                BattleEv_RunWait_4(ACTOR_ID, 0);
                Motion_CallWaitAnim_2(ACTOR_ID, 3);
                BattleEv_RunWait_5(ACTOR_ID, 0);
                Motion_SetSpeed_1(ACTOR_ID, 0xb333, 0x5999);
                Motion_SetPosReset_3(ACTOR_ID, 0x1b8, 0x4e8);
                BattleEv_RunWait_6(ACTOR_ID, 0);
                Motion_CallWaitAnim_3(0, 3);
                Object_SetModeById_1(ACTOR_ID, 2);
                /* Record layout observed here: s16 at +10, s16 at +18. */
                record = Scene_GetRecord_2(0);
                if (record != 0) {
                    Motion_ResetAndSetPosition_1(ACTOR_ID, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Motion_CommitPos_1(ACTOR_ID);
                Motion_SetHPosTerrain_2(ACTOR_ID, 0, 0);
                Battle_SchedShoulder_1();
            }
        }
    }
}

s32 Scene_RunOpeningAuxiliarySequence(s32 a0)
{
    u32 i;
    s32 record;

    if (gCell[224][0] == (s32)gVal) {
        *(u8 *)(Actor_unk8(14) + 35) = 2;
        *(u8 *)(Actor_unk9(14) + 85) = 3;
        Actor_unk15_4(14, 0, 0);
        Actor_SetRect(16, 44, 1, 1, 15, 44);
        Actor_unk16_4(100, 0, 0);
        Actor_unk2_5(12, 71, 1, 1, 127, 127);
        Actor_unk3_5(11, 71, 1, 1, 12, 71);
        record = Actor_unk5(0x2008ce5);
        do {
            s32 shown = *(u16 *)gOv3;

            *(volatile u16 *)0x0500019e = shown;
        } while (0);
        return record;
    }
    return a0;
}

s32 Scene_RunScene3c0SequenceA(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    if (gCell[224][0] == (s32)gVal) {
        *(u8 *)(Actor_unk10(14) + 35) = 2;
        v5 = 0;
        *(u8 *)(Actor_unk11(14) + 85) = v5;
        Actor_Place(14, 0xf80000, 0x2c80000);
        Actor_unk4_5(31, 95, 1, 1, 15, 44);
        Actor_unk2_3(100, -1, -1);
        Actor_unk18_4();
        Actor_unk5_5(127, 127, 1, 1, 12, 71);
        record = Actor_unk6(0x2008ce5, 0xc80);
        return record;
    }
    return a0;
}

/* overlays/scene/actor/selected_presentation/selected_actor_effect.c */

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct Selection {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern volatile s32 gIw;

struct Actor *Actor_unk19_4(s32 actor);

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

void sync_selected_actor_progress(void)
{
    struct Actor *actor;
    struct SceneWork *scene;
    s32 progress;

    actor = Actor_unk19_4(RuntimeSelectorTable.actor_id);
    scene = gWork;
    actor->presentation = (u16)(gIw << 12);

    progress = Actor_unk12(0x210);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Actor_unk13(0x106) == 0) {
            progress -= 1;
        }
    }
    Actor_Apply5(0x210, progress);
}

/* overlays/scene/actor/selected_presentation/state_update.c */

/* Named shorthand for one fixed state request, in overlay resource_3c0. */

/*
 * The 14-byte owner at 0x02000314 loads no literal and has no pool word.
 * Both arguments are immediates, and Actor_unk21_4 spells the overlay's own
 * relocated call word rather than a runtime address.
 */
void State_SendRequest15With45(void)
{
    Actor_unk21_4(15, 45);
}

void State_SetStateByte52(void)
{
    u8 *state = *(u8 *volatile *)0x03001f30;
    state[52] = 1;
}

/* overlays/scene/actor/selected_presentation/ov_obj_apply_zero_and_clear_byte89.c */
/* overlays/scene/actor/selected_presentation/apply_zero_and_clear_byte_89.c */
/* Complete 24-byte entity reset owner. */
s32 OvObj_ApplyZeroAndClearByte89(u8 *obj)
{
    Actor_Run(obj, 0);
    obj[89] = 0;
    return 0;
}

/* overlays/scene/actor/selected_presentation/toggle_field_84_bit_0.c */
/* Complete 16-byte flag-toggle leaf, including its alignment halfword. */
s32 OvObj_ToggleField84Bit0(u8 *obj)
{
    obj[84] ^= 1;
    return 1;
}
