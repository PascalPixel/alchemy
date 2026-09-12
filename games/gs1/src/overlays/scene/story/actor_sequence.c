#include "types.h"
#include "scene.h"
#include "configured_effect_spawn.h"
#include "configured_effect_spawn_body.inc"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"

/* overlays/scene/story/actor_sequence/actor_sequence.c */
/* overlays/scene/story/actor_sequence/actor_position.c */
u8 *Story_unk5_4(s32);
u8 *Story_unk6_4(s32);
u8 *Story_unk7_4(s32);

/*
 * Sets a flag on actor 8 according to the party leader's depth.
 */

void Actor_SetActor8FlagByLeaderDepth(void)
{
    u8 *obj = Story_unk5_4(0);
    if (*(s32 *)(obj + 12) >= 0x100000) {
        u8 *p = Story_unk6_4(8);
        /*
         * The flag is a local initialised first and ORed with the loaded
         * byte afterwards; folding it into one expression swaps the two
         * registers.
         */
        u32 flag = 2;
        flag = flag | p[0x23];
        p[0x23] = (u8)flag;
    } else {
        Story_unk7_4(8)[0x23] = 1;
    }
}

/* overlays/scene/story/actor_sequence/actor_presentation.c */

static __inline__ void SetActorDirection(s32 actor, s32 direction, s32 duration)
{
    Story_unk19_3(actor, direction, duration);
}

void ActorDraw_RunActorZeroCameraScene(void)
{
    Story_unk8_4();
    Story_Apply(0, 1);
    Story_unk33_2(9423);
    Story_Apply2(1, 0);
    SetActorDirection(0, 258, 100);
    Story_unk9_4(0, 2);
    Story_unk20_3(0, 0, 12);
    Story_unk10_4(0);
    Story_Apply3(0, 1);
    Story_unk34_2((const void *)0x243);
    Story_unk11_4();
}

/* overlays/scene/story/actor_sequence/orbiting_effect.c */
#define UpdateOrbitingSceneObject Value

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Value;

OrbitingSceneObject *GetOrbitingSceneObject(void);

u8 *AllocateEffectTransfer(s32, s32);

void InitializeSpawnedOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}

/* overlays/scene/story/actor_sequence/orbiting_effect_spawn.c */

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

s32 update_orbiting_effect(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Story_unk23(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Story_unk24(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Story_unk25(p[12] + 0x8000) / 8;
    a = Story_unk26();
    b = Story_unk27();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

/* overlays/scene/story/actor_sequence/overlay_object.c */

void *Story_unk13_4(s32, s32, s32, s32);

void *OvObj_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Story_unk13_4(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Story_Apply4(obj, 0);
        Story_Apply5(obj, 14);
        Story_Apply6(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/* overlays/scene/story/actor_sequence/scene_data.c */
extern s16 gCell[];
extern u8 Value_00000068;
extern u8 Value_0000009f;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gCell[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_SelectTableBySelector224(void)
{
    s16 v = gCell[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)gOv3;
    }
    return (s32)gOv4;
}

s32 SceneData_SelectTableByScene9f(void)
{
    s16 v = gCell[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)gOv5;
    }
    return (s32)gOv6;
}

/* overlays/scene/story/actor_sequence/scene_dialogue.c */
extern u8 gVal[];

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

void Dialogue_ShowLine1B9C(void)
{
    Story_unk14_4();
    Story_unk21_3(17, 0x102, 60);
    Story_unk35_2(0x1b9c);
    Story_Apply7(17, 0);
    Story_unk15_4();
}

void Dialogue_RunActor9Line(void) { Story_unk16_4(); Story_unk36_2(0x1ba6); Story_Apply8(9, 0); Story_unk17_4(); }

void Dialogue_ShowLine1BAA(void) { Story_unk18_4(); Story_unk37_2(0x1baa); Story_Apply9(11, 0); Story_unk19_4(); }

void Dialogue_ShowLine1BB0(void) { Story_unk20_4(); Story_unk38_2(0x1bb0); Story_Apply10(15, 0); Story_unk21_4(); }

void Dialogue_RunActor10Scene(void) { Story_unk22_4(); Story_Apply11(10, 3); Story_unk39_2(0x24d1); Story_Apply12(10, 0); Story_unk23_4(); }

void Dialogue_RunActorTwelveDialogue(void) { Story_unk24_4(); Story_unk40_2(0x24d3); Story_Apply13(12, 0); Story_unk25_4(); }

void Dialogue_RunActorNineFlaggedDialogue(void)
{
    if (Story_unk28(0x941) != 0)
        Story_unk41_2(0x24e8);
    else
        Story_unk42_2((s32)gVal);
    Story_Apply14(9, 0);
}

/* overlays/scene/story/actor_sequence/scene_primary_script.c */
#define GUARD_FLAG_1 0x201
#define GUARD_FLAG_2 0x202
#define CELL_W 3
#define CELL_H 4
#define DIALOGUE_GATE_FLAG 0x244
#define ACTOR_8 8
#define ACTOR_9 9

extern u8 Value4;
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern u8 gOv7[];
extern u8 gVal5[];

u8 *Story_unk26_4();

u8 *Story_unk27_4();

void *Scene_GetRecord_2();

void *Scene_GetRecord_1();

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

/* Flag ids checked as guard conditions before the layout runs; neither
 * unused local below is read anywhere in the body. */

/* Fixed cell size used by every placement call below (args 3 and 4). */

/* One-shot flag gate: the scene flag this function is named for. */

/* Actor ids referenced throughout this scene. */

void Scene_RunStepFD1WithActor181(void)
{
    Story_unk28_4();
    Story_unk22_3(20, 0, 0);
    Story_unk43_2(0xfd1);
    Story_Apply15(181, 3);
    Story_Apply16(181, 0);
    Story_unk29_4();
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 gWork[];

    u32 i;
    u8 *record;
    s32 v3;
    s32 v0;

    record = Story_Check(0);
    v3 = *(volatile s32 *)((s32)record + 8) / 0x100000;
    v0 = *(volatile s32 *)((s32)record + 16) / 0x100000;
    if (Story_unk2(0xf27) == 0) {
        if (v3 == 7) {
            if (v0 == 16) {
                Story_unk30_4(0, 0, 16);
            }
        }
        Story_Place(102, -1, -1);
        Story_SetRect(28, 31, 1, 1, 7, 16);
    }
    Story_unk2_5(47, 4, 1, 1, 46, 4);
    Story_unk3_5(34, 37, 3, 3, 13, 3);
    Story_unk2_3(8, 0xe80000, 0x480000);
    record = Story_unk26_4(8);
    *(volatile s32 *)((s32)record + 12) = 0;
    if (Story_unk3(0x202) != 0) {
        Story_unk4_5(41, 49, 3, 4, 1, 14);
        Story_unk5_5(44, 49, 3, 4, 33, 14);
        Story_unk6_5(47, 49, 3, 4, 1, 46);
    } else {
        Story_unk3_3(19, 0x380000, 0x10c0000);
        record = Story_unk27_4(19);
        Story_unk31_4((s32)record, 0);
        record = Story_unk4(19);
        if ((s32)record != 0) {
            record[85] = 8;
            *(volatile s32 *)((s32)record + 12) = 0x100000;
            *(u8 *)((((s32)record + 85) - 50)) = 2;
            *(volatile s32 *)((s32)record + 24) = 0x13333;
            *(volatile s32 *)((s32)record + 28) = 0x18000;
        }
    }
    Story_unk5(0x2008469, 0xc80);
    Story_Do(0x201);
}

/* Sets up a handful of numbered records with position/pose-shaped argument
 * groups, then clears the s32 field at +108 of record 8 and feeds record 8's
 * id back into two more calls before two final single-argument calls. */
void Scene_RunOpeningSequence(void)
{
    void *Scene_GetRecord_1();

    void *unused;
    void *record8;

    unused = Scene_GetRecord_1(0);
    Motion_SetHPosTerrain_1(19, 0, 0);
    Motion_SetHPosTerrain_2(8, 0, 0);
    Story_unk7_5(38, 38, 1, 1, 46, 4); /* main:080091b8 */
    Story_unk8_5(37, 37, 3, 3, 13, 3); /* main:080091c0 */
    Story_unk9_5(37, 37, 1, 1, 14, 2); /* main:080091c0 */
    Story_unk10_5(8, 16, 1, 1, 7, 16); /* main:080091c0 */
    EffectRuntime_SetCurrentPosition_1(102, 0, 0);
    Story_unk11_5(32, 42, 3, 2, 1, 15); /* main:080091b8 */
    GameFlag_Clear_1(512);
    Object_SetModeById_1(8, 1);
    record8 = Scene_GetRecord_2(8);
    /* Clear the s32 field at offset 108 of record 8. */
    *(s32 *)(record8 + 108) = 0;
    Story_Run(Scene_GetRecord_3(8), 0); /* main:08009240 */
    Story_unk6((s32)&Value4); /* main:080000d8 */
    GameFlag_Set_1(0x201);
}

void Scene_RunScriptedSteps947And29E1(void)
{
    Story_unk32_4();
    Story_Apply17(0x947, 1);
    Story_Apply18(0x29e1, 1);
    Story_unk33_4();
}

s32 Scene_RunScene3abSequenceF(void)
{
    extern s16 gCell2[][1];
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (gCell2[224][0] == (s32)gVal3) {
        Story_unk2_2(0x941);
        return 0x200a3b4;
    } else {
        if (gCell2[224][0] == (s32)gVal2) {
            record = Story_unk7(0x941);
            if (record == 0) {
                goto L_02000716;
            }
            return 0x200a1bc;
        } else {
            L_02000716:;
        }
    }
    return 0x2009fc4;
}

void Scene_RunActorEighteenPromptDialogue(void)
{
    extern u8 *gWork;

    Story_unk35_4();
    if (Story_unk29(0x85a) == 0) {
        Story_unk44_2(0x1be1);
        Story_Apply19(18, 0);
    } else {
        Story_unk45_2(0x1b9f);
        Story_Apply20(18, 0);
        if (Story_Apply21(0, 0) == 0) {
            u8 **base = &gWork;
            (*(u16 *)(*base + 472))++;
            Story_Apply22(18, 0);
            if (Story_Apply23(0, 0) == 1)
                (*(u16 *)(*base + 472))++;
        }
        Story_Apply24(18, 0);
    }
    Story_unk36_4();
}

void Scene_RunSupplementalSequenceTwo(void)
{
    extern s16 gCell2[][2];
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_24db;

    if (gCell2[147][0] != 0) {
        Story_unk3_2(0x2411);
    } else {
        if (Story_unk8(0x941) != 0) {
            record = Story_unk9(0x94d);
            if (record != 0) {
                goto L_0200094c;
            }
            Story_unk4_3(8, 0x101, 60);
            base5_24db = (s32)gVal4;
            Story_unk37_4(base5_24db);
            Story_unk38_4(8, 0);
            Story_unk39_4(8, 1);
            Story_unk40_4((base5_24db + 1));
            Story_unk4_2(0x9af);
        } else {
            L_0200094c:;
            Story_unk5_2(0x1bb5);
        }
    }
    Story_unk41_4(8, 0);
}

void Scene_RunScene3abSequenceA(void)
{
    extern s16 gCell2[][2];
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (gCell2[147][0] != 0) {
        Story_unk6_2(0x2412);
    } else {
        if (Story_unk10(0x941) != 0) {
            Story_unk7_2(0x24dd);
        } else {
            Story_unk8_2(0x1bb6);
        }
    }
    Story_unk42_4(9, 0);
}

void Scene_RunScene3abSequenceB(void)
{
    extern s16 gCell2[][2];
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (gCell2[147][0] == 0) {
        record = Story_unk11(0x941);
        if (record == 0) {
            goto L_02000a54;
        }
        record = Story_unk12(0x94d);
        if (record != 0) {
            goto L_02000a54;
        }
        record = Story_unk13(0x9af);
        if (record == 0) {
            *(u16 *)((*(u8 *volatile *)gWork + 0x17e)) = record;
            Story_unk43_4();
            Story_unk44_4(8, 0, 0);
            Story_unk5_3(8, 0x101, 60);
            Story_unk9_2(0x24db);
            Story_unk10_2(0x9af);
        } else {
            Story_unk11_2(0x24e7);
        }
        Story_unk45_4(8, 0);
        Story_unk46_4(8, 1);
        Story_unk12_2(0x24dc);
    } else {
        L_02000a54:;
        Story_unk13_2(0x1bbf);
    }
    Story_unk47_4(8, 0);
}

void Scene_RunSupplementalSequenceThree(void)
{
    extern u8 gWork[];

    u8 *work;
    s16 *slot;
    u32 i;
    u8 *record;
    s32 kind;
    s32 off;
    s32 off4;
    u8 *table;

    work = *(u8 **)gWork;
    Story_unk48_4();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Story_unk14(i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    Story_unk49_4(158);
    slot = (s16 *)(work + 0x16c);
    kind = *slot - 4;
    off = kind << 3;
    table = gOv7;
    off4 = off + 4;
    Story_unk6_3(*(s32 *)(table + off), *(u16 *)(table + off4), *(u16 *)(table + off4 + 2));
    Story_unk7_3(0, 0x8000, 0x4000);
    *(u8 *)(Story_unk30(0) + 85) = 0;
    Story_unk50_4(0, 2);
    if (kind != 6) {
        Story_unk8_3(0, 2, -8);
        Story_unk51_4(10);
    }
    Story_unk52_3(*slot);
    Story_unk53_3();
    Story_unk54_3();
    Story_unk55_3();
}

void Scene_RunScene3abSequenceC(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    if (Story_unk15(0x202) != 0) {
        rec7 = Story_unk16(0x201);
        if (rec7 == 0) {
            Story_unk9_3(0, 0x3333, 0x1999);
            *(u8 *)(Story_unk31(0) + 85) = rec7;
            Story_unk56_3(0, 2);
            Story_unk10_3(0, 2, -8);
            Story_unk57_3(13);
            Story_unk58_3(12);
        }
    }
}

void Scene_RunScene3abSequenceE(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Story_unk14_2(0x242);
    Story_unk59_3();
    Story_unk11_3(0, 0x3333, 0x1999);
    *(u8 *)(Story_unk32(0) + 85) = 0;
    Story_unk60_3(0, 2);
    Story_unk12_3(0, 0, -8);
    v5 = 41;
    v6 = 4;
    Story_unk61_3(158);
    Story_unk12_5(53, 4, 2, 2, v5, v6);
    Story_unk62_3(10);
    Story_unk13_5(53, 6, 2, 2, v5, v6);
    Story_unk63_3(10);
    Story_unk64_3(1);
    Story_unk65_3();
    Story_unk66_3();
    Story_unk67_3();
}

/* Lays out two rows of tile cells (three cells per row-segment, five
 * row-segments total) and commits each row-segment with a trailing
 * call taking the constant 10, then marks GUARD_FLAG_2. */
void Scene_BuildFlaggedMapLayout(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(GUARD_FLAG_1)!= 0) {
    } else {
        if (GameFlag_IsSet_2(GUARD_FLAG_2)!= 0) {
        } else {
            ObjectMotion_SetHorizontalPositionWithTerrain_1(19, 0, 0);
            Audio_PlayCue_1(210);
            Story_unk68_3(1);
            Story_unk14_5(32, 45, CELL_W, CELL_H, 1, 14);
            Story_unk15_5(35, 45, CELL_W, CELL_H, 33, 14);
            Story_unk16_5(38, 45, CELL_W, CELL_H, 1, 46);
            Story_unk69_3(10);
            Story_unk17_5(41, 45, CELL_W, CELL_H, 1, 14);
            Story_unk18_5(44, 45, CELL_W, CELL_H, 33, 14);
            Story_unk19_5(47, 45, CELL_W, CELL_H, 1, 46);
            Story_unk70_3(10);
            Story_unk20_5(50, 45, CELL_W, CELL_H, 1, 14);
            Story_unk21_5(53, 45, CELL_W, CELL_H, 33, 14);
            Story_unk22_5(56, 45, CELL_W, CELL_H, 1, 46);
            Story_unk71_3(10);
            Story_unk23_5(32, 49, CELL_W, CELL_H, 1, 14);
            Story_unk24_5(35, 49, CELL_W, CELL_H, 33, 14);
            Story_unk25_5(38, 49, CELL_W, CELL_H, 1, 46);
            Story_unk72_3(10);
            Story_unk26_5(41, 49, CELL_W, CELL_H, 1, 14);
            Story_unk27_5(44, 49, CELL_W, CELL_H, 33, 14);
            Story_unk28_5(47, 49, CELL_W, CELL_H, 1, 46);
            Story_unk73_3(10);
            GameFlag_Set_1(GUARD_FLAG_2);
        }
    }
}

void Scene_RunScene3ab(void)
{
    extern s16 gCell2[][2];
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (gCell2[147][0] == 0) {
        Story_unk74_3();
        Story_unk13_3(8, 0x100, 2);
        Story_unk14_3(9, 0x100, 15);
        Story_unk75_3(30);
        Story_unk76_3(8, 152, 168);
        Story_unk77_3(9, 168, 168);
        Story_unk78_3(8);
        Story_unk79_3(9);
        Story_unk80_3(8);
        Story_unk81_3(8, 0);
        Story_unk15_3(8, 0x3000, 0);
        Story_unk82_3(9);
        Story_unk83_3(9, 0);
        Story_unk16_3(9, 0x5000, 0);
        Story_unk15_2(0x24da);
        Story_unk84_3(8, 0);
        Story_unk16_2(0x240);
        Story_unk29_5(6, 11, 1, 1, 7, 11);
        Story_unk30_5(6, 11, 1, 1, 8, 11);
        Story_unk31_5(6, 11, 1, 1, 9, 11);
        Story_unk85_3();
    }
}

/* Runs once per save: if DIALOGUE_GATE_FLAG is already set the function does
 * nothing, otherwise it sets the flag and then positions actor 0's record,
 * actor 8, and actor 9, feeds four sequential bytes of the gVal5
 * buffer to their setup calls, and steps actors 8/9 and camera-target 0
 * through a sequence of move/pose calls. */
void Scene_RunFlagGatedActorDialogue(void)
{
    s32 Scene_GetRecord_1();

    u32 unused_counter;
    s32 actor0_record;
    s32 unused_record;
    s32 script_data;

    if (GameFlag_IsSet_1(DIALOGUE_GATE_FLAG)!= 0) {
    } else {
        GameFlag_Set_1(DIALOGUE_GATE_FLAG);
        Battle_Reset_1();
        actor0_record = Scene_GetRecord_1(0);
        Motion_SetAngleToward_1(ACTOR_8, 0, 0);
        Motion_SetAngleToward_2(ACTOR_9, 0, 0);
        Motion_SetVarCb_1(ACTOR_8, 1);
        Motion_SetVarCb_2(ACTOR_9, 1);
        Battle_WaitMode0_1(20);
        BattleFx_SpawnLinked_1(ACTOR_8, 0x102, 60);
        script_data = (s32)gVal5;
        SceneWork_SetStepValue_1(script_data);
        BattleEv_RunWait_1(ACTOR_8, 0);
        Motion_SetSpeed_1(0, 0x20000, 0x10000);
        Motion_SetSpeed_2(ACTOR_8, 0x20000, 0x10000);
        Motion_SetSpeed_3(ACTOR_9, 0x20000, 0x10000);
        Object_SetModeById_1(ACTOR_9, 4);
        Battle_WaitMode0_2(35);
        SceneWork_SetStepValue_2((script_data + 1));
        BattleEv_RunWait_2(ACTOR_9, 0);
        BattleFx_SpawnLinked_2(ACTOR_8, 0x103, 30);
        SceneWork_SetStepValue_3((script_data + 2));
        BattleEv_RunWait_3(ACTOR_8, 0);
        Object_SetModeById_2(ACTOR_9, 3); /* object_id ACTOR_9, action 3 */
        Battle_WaitMode0_3(25);
        SceneWork_SetStepValue_4((script_data + 3));
        BattleEv_RunWait_4(ACTOR_9, 0);
        /* +10 and +18 of actor 0's record read as halfwords: the first,
         * minus one, and the second are fed to actor 8's placement call. */
        Motion_ResetPosMode2_1(ACTOR_8, (*(s16 *)(actor0_record + 10) - 1),
                       *(s16 *)(actor0_record + 18));
        Motion_CommitPos_1(ACTOR_8);
        Motion_ResetPosMode2_2(0, 160, 216);
        Motion_ResetPosMode2_3(ACTOR_8, 152, 200); /* object_id ACTOR_8, x 152, z 200 */
        Motion_ResetPosMode2_4(ACTOR_9, 168, 200);
        Motion_CommitPos_2(ACTOR_8);
        Motion_CommitPos_3(ACTOR_9);
        Motion_CommitPos_4(0);
        Motion_SetAngleToward_3(ACTOR_8, 0, 0);
        Motion_SetAngleToward_4(ACTOR_9, 0, 0);
        Battle_WaitMode0_4(12);
        Motion_ResetPosMode2_5(0, 160, 0x110);
        Motion_ResetPosMode2_6(ACTOR_8, 152, 0x100);
        Motion_ResetPosMode2_7(ACTOR_9, 168, 0x100);
        Motion_CommitPos_5(ACTOR_8);
        Motion_CommitPos_6(ACTOR_9);
        Motion_CommitPos_7(0);
        Battle_SchedShoulder_1();
        Story_unk2_4(0x2009241, 0xc80); /* main:080000d0 */
    }
}

void Scene_RunScene3ab(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(s32 *)gWork;
    Story_unk17_2(0x241);
    Story_unk18_2(0x240);
    record = Story_unk33(0);
    if ((u32)(*(s32 *)(record + 8) + -0x680001) <= 0x87fffe) {
        if (*(s32 *)(record + 16) > 0xa00000) {
            if (*(s32 *)(record + 16) < 0xf80000) {
                Story_unk19_2(0x2009241);
                {
                    u16 *target = (u16 *)(((s32)p5 + 0x182));
                    s32 shown = 91;

                    *target = shown;
                }
            }
        }
    }
    Story_unk86_3();
    Story_unk20_2(0x244);
}

void Scene_RunScene3abSequenceG(void)
{
    extern s16 gCell2[][2];
    extern u8 gWork[];

    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(s32 *)gWork;
    record = Story_unk34(0);
    if (gCell2[147][0] == 0) {
        if ((u32)(*(s32 *)(record + 8) + -0x900000) <= 0x200000) {
            if (*(s32 *)(record + 16) >= 0xa80000) {
                if (*(s32 *)(record + 16) < 0xb00000) {
                    Story_unk21_2(0x2009241);
                    {
                        u16 *target = (u16 *)(((s32)p5 + 0x182));
                        s32 shown = 91;

                        *target = shown;
                    }
                }
            }
        }
    }
}

/* overlays/scene/story/actor_sequence/scene_setup.c */
extern u8 gWork[];
extern s16 gCell2[][1];

u8 *Story_unk87_3(s32);
u8 *Story_unk88_3(s32);

void Scene_DrawTilesAndResetActor8(void)
{
    u8 *actor = Story_unk87_3(8);
    Story_unk88_3(0);
    {
        s32 cell = 14;
        s32 row = 4;
        Story_unk32_5(17, 4, 1, 1, cell, row);
        Story_unk33_5(15, 3, 1, 1, 15, row);
        Story_unk34_5(15, 3, 1, 1, 13, row);
    }
    if (actor != 0) {
        Story_Apply25(actor, 0);
        actor[0x55] = 2;
        actor[0x23] = 1;
    }
    Story_unk46_2(0x200);
}

void Scene_RunLeaderMovementSequence(void)
{
    Story_unk89_3();
    Story_unk90_3();
    Story_unk91_3(0, 152, 168);
    Story_unk92_3(0);
    Story_unk93_3(20);
    Story_unk94_3(146, 1);
    Story_unk95_3(0, 0);
    Story_unk96_3();
    Story_unk97_3(1);
    Story_unk98_3();
    Story_unk99_3(0, 144, 184);
    Story_unk100_3(0);
    Story_unk101_3(0, 88, 184);
    Story_unk102_3(0);
    Story_unk103_3(0, 88, 200);
    Story_unk104_3(0);
    Story_unk105_3(0, 72, 200);
    Story_unk106_3(0);
    Story_unk17_3(0, 72, 0x120);
    Story_unk107_3(0);
    Story_unk18_3(0, 88, 0x120);
    Story_unk108_3(0);
    Story_unk109_3();
}

s32 Scene_InitializeStoryActors(void)
{
    s32 record;

    if (gCell2[224][0] == (s32)gVal2) {
        *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x100;
        Story_unk110_3();
        if (Story_unk17(0xfd1) == 0) {
            Story_unk111_3(20);
        }
        record = Story_unk18(8);
        if (record != 0) {
            Story_unk112_3(record, 0);
        }
        Story_unk22_2(0x201);
    }
    if (gCell2[224][0] == (s32)gVal3) {
        *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x100;
        gCell2[289][0] = 10;
        if (gCell2[225][0] == 4) {
            if (Story_unk19(0x109) == 0) {
                Story_unk113_3();
            }
        }
        if (gCell2[225][0] == 3) {
            if (Story_unk20(0x109) == 0) {
                Story_unk114_3();
            }
        }
        if (Story_unk21(0x941) != 0) {
            if (Story_unk22(0x94d) == 0) {
                Story_unk3_4(0x2008ac5, 0xc80);
            }
        }
        Story_unk4_4(0x2009241, 0xc80);
        Story_unk23_2(0x944);
        Story_unk24_2(0x945);
        Story_unk25_2(0x946);
        Story_unk26_2(0x947);
        Story_unk27_2(0x948);
        Story_unk28_2(0x943);
        Story_unk29_2(0x949);
        Story_unk30_2(0x94a);
        Story_unk31_2(0x94b);
        Story_unk32_2(0x94c);
    }
    return 0;
}

/* overlays/scene/story/actor_sequence/scene_wrapper.c */

void Scene_CallHelper(void) { Story_unk115_3(); }

/* overlays/scene/story/actor_sequence/run_83_when_flag_clear.c */
void State_Run83WhenFlag201Clear(void) { if (Story_Run(0x201) == 0) Story_unk2_4(83); }

/* overlays/scene/story/actor_sequence/scene_no_op.c */
/* overlays/scene/story/actor_sequence/no_op.c */
void Scene_NoOp(void) { }

/* overlays/scene/story/actor_sequence/set_three_cells_6_11_flag_241.c */
typedef signed int s32;

void State_SetThreeCellsAt6x11AndFlag241(void)
{
    Story_SetRectNoOp(6, 11, 1, 1, 7, 11);
    Story_unk2_5NoOp(6, 11, 1, 1, 8, 11);
    Story_unk3_5NoOp(6, 11, 1, 1, 9, 11);
    Story_Do(0x241);
}

/* overlays/scene/story/actor_sequence/state_set_three_cells_at7x12.c */
/* Three adjacent collision cells used by the scene's lower doorway. */

void State_SetThreeCellsAt7x12(void)
{
    Story_SetRectThreeCellsAt7x12(7, 12, 1, 1, 7, 11);
    Story_unk2_5ThreeCellsAt7x12(7, 12, 1, 1, 8, 11);
    Story_unk3_5ThreeCellsAt7x12(7, 12, 1, 1, 9, 11);
}
