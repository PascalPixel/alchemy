#include "types.h"
#include "configured_effect_spawn.h"
#include "configured_effect_spawn_body.inc"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"

/* overlays/scene/story/actor_sequence/actor_position.c */
#define Actor_SetActor8FlagByLeaderDepth Func_02000468
u8 *Func_02001e6e(s32);
u8 *Func_02001e7e(s32);
u8 *Func_02001e8e(s32);

/*
 * Sets a flag on actor 8 according to the party leader's depth.
 */

void Actor_SetActor8FlagByLeaderDepth(void)
{
    u8 *obj = Func_02001e6e(0);
    if (*(s32 *)(obj + 12) >= 0x100000) {
        u8 *p = Func_02001e7e(8);
        /*
         * The flag is a local initialised first and ORed with the loaded
         * byte afterwards; folding it into one expression swaps the two
         * registers.
         */
        u32 flag = 2;
        flag = flag | p[0x23];
        p[0x23] = (u8)flag;
    } else {
        Func_02001e8e(8)[0x23] = 1;
    }
}
#undef Actor_SetActor8FlagByLeaderDepth

/* overlays/scene/story/actor_sequence/actor_presentation.c */
#define ActorDraw_RunActorZeroCameraScene Func_02000b0c
void Func_020024f0(void);
void Func_02002528(const void *);
void Func_02002544(void);
void Func_02002560(s32, s32);
void Func_02002574(s32, s32, s32);
void Func_02002582_a();
void Func_02002582_b();
void Func_0200258e(s32);
void Func_0200259a(s32, s32);
void Func_020025a6(s32, s32);
void Func_020025d2(s32, s32, s32);

static __inline__ void SetActorDirection(s32 actor, s32 direction, s32 duration)
{
    Func_020025d2(actor, direction, duration);
}

void ActorDraw_RunActorZeroCameraScene(void)
{
    Func_020024f0();
    Func_02002560(0, 1);
    Func_0200258e(9423);
    Func_020025a6(1, 0);
    SetActorDirection(0, 258, 100);
    Func_02002582_a(0, 2);
    Func_02002574(0, 0, 12);
    Func_02002582_b(0);
    Func_0200259a(0, 1);
    Func_02002528((const void *)0x243);
    Func_02002544();
}
#undef ActorDraw_RunActorZeroCameraScene

/* overlays/scene/story/actor_sequence/orbiting_effect.c */
#define GetOrbitingSceneObject Func_0200324c
#define NormalizeOrbitingSceneObject Func_02003216
#define IsGameFlagSet Func_02003254
#define AllocateEffectTransfer Func_02003214
#define LoadEffectResource Func_02003284
#define ConfigurePaletteTransfer Func_0200323c
#define ReleaseEffectTransfer Func_0200323a
#define UpdateOrbitingSceneObject Value_020097d9
#define InitializeSpawnedOrbitingEffect Func_02001840

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

extern u8 Value_020097d9;

OrbitingSceneObject *Func_0200324c(void);
void Func_02003216(OrbitingSceneObject *, s32);
s32 Func_02003254(s32);
u8 *Func_02003214(s32, s32);
void Func_02003284(s32);
s32 Func_0200323c(u8, s32, u8 *);
void Func_0200323a(s32);

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
#undef GetOrbitingSceneObject
#undef NormalizeOrbitingSceneObject
#undef IsGameFlagSet
#undef AllocateEffectTransfer
#undef LoadEffectResource
#undef ConfigurePaletteTransfer
#undef ReleaseEffectTransfer
#undef UpdateOrbitingSceneObject
#undef InitializeSpawnedOrbitingEffect

/* overlays/scene/story/actor_sequence/orbiting_effect_spawn.c */
#define EffectDescriptorTable Data_02009be0
#define GetPartyEffect Func_02001b5e
#define SpawnEffect Func_02001afc
#define SetEffectVariant Func_02001b06
#define SetEffectDescriptor Func_02001b20
#define SetEffectMode Func_02001c82
#define ScaleEffectDeltaFromAccumulated Func_02001b94
#define ScaleEffectDeltaFromOrigin Func_02001bac
#define ScaleEffectVerticalDelta Func_02001bba
#define SetEffectCallbackMode Func_02001c20
#define SetEffectCallbackArgument Func_02001c30
#define SpawnConfiguredEffect Func_0200013c


s32 Func_02003122(s32);
s32 Func_02003138(s32);
s32 Func_02003152(s32);
s32 Func_02003150(void);
s32 Func_02003156(void);

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

s32 Func_020017d8(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_02003122(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_02003138(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_02003152(p[12] + 0x8000) / 8;
    a = Func_02003150();
    b = Func_02003156();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}
#undef EffectDescriptorTable
#undef GetPartyEffect
#undef SpawnEffect
#undef SetEffectVariant
#undef SetEffectDescriptor
#undef SetEffectMode
#undef ScaleEffectDeltaFromAccumulated
#undef ScaleEffectDeltaFromOrigin
#undef ScaleEffectVerticalDelta
#undef SetEffectCallbackMode
#undef SetEffectCallbackArgument
#undef SpawnConfiguredEffect

/* overlays/scene/story/actor_sequence/overlay_object.c */
#define NULL ((void *)0)
#define CreateOverlayObject Func_02001a2a
#define SetOverlayObjectMode Func_02001a74
#define SetOverlayObjectSlot Func_02001b4c
#define OvObj_CreateConfigured Func_02000048
#define OvObj_CreateConfiguredObject Func_020000a0


void *Func_020019d2(s32, s32, s32, s32);
void Func_02001a18(void *, s32);
void Func_02001a30(void *, s32);
void Func_02001af0(void *, s32);

void *OvObj_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_020019d2(arg3, arg0, arg1, arg2);

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
        Func_02001a18(obj, 0);
        Func_02001af0(obj, 14);
        Func_02001a30(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}
#undef NULL
#undef CreateOverlayObject
#undef SetOverlayObjectMode
#undef SetOverlayObjectSlot
#undef OvObj_CreateConfigured
#undef OvObj_CreateConfiguredObject

/* overlays/scene/story/actor_sequence/scene_data.c */
#define SceneData_SelectDataByRuntimeSelector Func_02000314
#define SceneData_ReturnZero Func_0200034c
#define SceneData_SelectTableBySelector224 Func_02000350
#define SceneData_SelectTableByScene9f Func_02000388
extern s16 Data_02000240[];
extern u8 Value_00000068;
extern u8 Value_0000009f;
extern u8 Data_02009d3c[];
extern u8 Data_02009bec[];
extern u8 Data_02009e04[];
extern u8 Data_02009dcc[];
extern u8 Data_02009f64[];
extern u8 Data_02009e14[];

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009d3c;
    }
    return (s32)Data_02009bec;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_SelectTableBySelector224(void)
{
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009e04;
    }
    return (s32)Data_02009dcc;
}

s32 SceneData_SelectTableByScene9f(void)
{
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009f64;
    }
    return (s32)Data_02009e14;
}
#undef SceneData_SelectDataByRuntimeSelector
#undef SceneData_ReturnZero
#undef SceneData_SelectTableBySelector224
#undef SceneData_SelectTableByScene9f

/* overlays/scene/story/actor_sequence/scene_dialogue.c */
#define Dialogue_ShowLine1B9C Func_02000738
#define Dialogue_RunActor9Line Func_02000844
#define Dialogue_ShowLine1BAA Func_02000864
#define Dialogue_ShowLine1BB0 Func_02000884
#define Dialogue_RunActor10Scene Func_020008a4
#define Dialogue_RunActorTwelveDialogue Func_020008cc
#define Dialogue_RunActorNineFlaggedDialogue Func_02000a90
extern u8 Data_00001bc0[];

void Func_0200211c(void);
void Func_020021e8(s32, s32, s32);
void Func_020021be(s32);
s32 Func_020021de(s32, s32);
void Func_02002142(void);
void Func_02002228(void);
void Func_020022be(s32);
s32 Func_020022de(s32, s32);
void Func_02002242(void);
void Func_02002248(void);
void Func_020022de_a(s32);
s32 Func_020022fe(s32, s32);
void Func_02002262(void);
void Func_02002268(void);
void Func_020022fe_a(s32);
s32 Func_0200231e(s32, s32);
void Func_02002282(void);
void Func_02002288(void);
void Func_02002308(s32, s32);
void Func_02002326(s32);
void Func_0200233e(s32, s32);
void Func_020022aa(void);
void Func_020022b0(void);
void Func_02002346(s32);
s32 Func_02002366(s32, s32);
void Func_020022ca(void);
s32 Func_02002456(s32);
void Func_02002510(s32);
void Func_02002518(s32);
void Func_02002530(s32, s32);

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

void Dialogue_ShowLine1B9C(void)
{
    Func_0200211c();
    Func_020021e8(17, 0x102, 60);
    Func_020021be(0x1b9c);
    Func_020021de(17, 0);
    Func_02002142();
}

void Dialogue_RunActor9Line(void) { Func_02002228(); Func_020022be(0x1ba6); Func_020022de(9, 0); Func_02002242(); }

void Dialogue_ShowLine1BAA(void) { Func_02002248(); Func_020022de_a(0x1baa); Func_020022fe(11, 0); Func_02002262(); }

void Dialogue_ShowLine1BB0(void) { Func_02002268(); Func_020022fe_a(0x1bb0); Func_0200231e(15, 0); Func_02002282(); }

void Dialogue_RunActor10Scene(void) { Func_02002288(); Func_02002308(10, 3); Func_02002326(0x24d1); Func_0200233e(10, 0); Func_020022aa(); }

void Dialogue_RunActorTwelveDialogue(void) { Func_020022b0(); Func_02002346(0x24d3); Func_02002366(12, 0); Func_020022ca(); }

void Dialogue_RunActorNineFlaggedDialogue(void)
{
    if (Func_02002456(0x941) != 0)
        Func_02002510(0x24e8);
    else
        Func_02002518((s32)Data_00001bc0);
    Func_02002530(9, 0);
}
#undef Dialogue_ShowLine1B9C
#undef Dialogue_RunActor9Line
#undef Dialogue_ShowLine1BAA
#undef Dialogue_ShowLine1BB0
#undef Dialogue_RunActor10Scene
#undef Dialogue_RunActorTwelveDialogue
#undef Dialogue_RunActorNineFlaggedDialogue

/* overlays/scene/story/actor_sequence/scene_primary_script.c */
#define Scene_GetRecord_1(args...) Func_02001ff8(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02002042, a0, a1, a2)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_0200204c, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_1(a0, a1, a2) Call3(Func_0200214c, a0, a1, a2)
#define GameFlag_Clear_1(a0) Call1(Func_02002050, a0)
#define Object_SetModeById_1(a0, a1) Call2(Func_020020d0, a0, a1)
#define Scene_GetRecord_2(args...) Func_0200208e(args)
#define Scene_GetRecord_3(a0) Value1(Func_02002098, a0)
#define GameFlag_Set_1(a0) Value1(Func_02002072, a0)
#define GameFlag_IsSet_1(a0) Value1(Func_020026fe, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_0200270a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02000d30(args...) Func_0200279a(args)
#define Audio_PlayCue_1(args...) Func_02002890(args)
#define GameFlag_Set_1_02000d30(a0) Call1(Func_0200285e, a0)
#define GUARD_FLAG_1 0x201
#define GUARD_FLAG_2 0x202
#define CELL_W 3
#define CELL_H 4
#define GameFlag_IsSet_1_02000ff0(a0) Value1(Func_020029b8, a0)
#define GameFlag_Set_1_02000ff0(a0) Call1(Func_020029ce, a0)
#define Battle_Reset_1(args...) Func_020029ea(args)
#define Scene_GetRecord_1_02000ff0(args...) Func_02002a10(args)
#define Motion_SetAngleToward_1(a0, a1, a2) Value3(Func_02002a7c_a, a0, a1, a2)
#define Motion_SetAngleToward_2(args...) Func_02002a86_a(args)
#define Motion_SetVarCb_1(args...) Func_02002a7e(args)
#define Motion_SetVarCb_2(args...) Func_02002a86_b(args)
#define Battle_WaitMode0_1(args...) Func_02002a14(args)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02002ae8, a0, a1, a2)
#define SceneWork_SetStepValue_1(args...) Func_02002ac0(args)
#define BattleEv_RunWait_1(args...) Func_02002ad8(args)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02002a6e, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02002a7c_b, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02002a8a, a0, a1, a2)
#define Object_SetModeById_1_02000ff0(args...) Func_02002ad2(args)
#define Battle_WaitMode0_2(args...) Func_02002a68(args)
#define SceneWork_SetStepValue_2(args...) Func_02002b06_a(args)
#define BattleEv_RunWait_2(args...) Func_02002b1e_a(args)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02002b48, a0, a1, a2)
#define SceneWork_SetStepValue_3(args...) Func_02002b1e_b(args)
#define BattleEv_RunWait_3(args...) Func_02002b36(args)
#define Object_SetModeById_2(args...) Func_02002b06_b(args)
#define Battle_WaitMode0_3(args...) Func_02002a9e(args)
#define SceneWork_SetStepValue_4(args...) Func_02002b3c(args)
#define BattleEv_RunWait_4(args...) Func_02002b54(args)
#define Motion_ResetPosMode2_1(args...) Func_02002b04(args)
#define Motion_CommitPos_1(args...) Func_02002b22(args)
#define Motion_ResetPosMode2_2(args...) Func_02002b14(args)
#define Motion_ResetPosMode2_3(args...) Func_02002b1e_c(args)
#define Motion_ResetPosMode2_4(args...) Func_02002b28(args)
#define Motion_CommitPos_2(args...) Func_02002b46(args)
#define Motion_CommitPos_3(args...) Func_02002b4c(args)
#define Motion_CommitPos_4(args...) Func_02002b52(args)
#define Motion_SetAngleToward_3(args...) Func_02002b84(args)
#define Motion_SetAngleToward_4(args...) Func_02002b8e(args)
#define Battle_WaitMode0_4(args...) Func_02002b0c(args)
#define Motion_ResetPosMode2_5(a0, a1, a2) Call3(Func_02002b60, a0, a1, a2)
#define Motion_ResetPosMode2_6(a0, a1, a2) Call3(Func_02002b6c, a0, a1, a2)
#define Motion_ResetPosMode2_7(a0, a1, a2) Call3(Func_02002b78, a0, a1, a2)
#define Motion_CommitPos_5(args...) Func_02002b96(args)
#define Motion_CommitPos_6(args...) Func_02002b9c(args)
#define Motion_CommitPos_7(args...) Func_02002ba2(args)
#define Battle_SchedShoulder_1(args...) Func_02002b56(args)
#define DIALOGUE_GATE_FLAG 0x244
#define ACTOR_8 8
#define ACTOR_9 9
#define Scene_RunStepFD1WithActor181 Func_020003c0
#define Scene_RunSupplementalSequenceOne Func_0200049c
#define Scene_RunOpeningSequence Func_020005f0
#define Scene_RunScriptedSteps947And29E1 Func_020006bc
#define Scene_RunActorEighteenPromptDialogue Func_02000764
#define Scene_RunSupplementalSequenceTwo Func_020008ec
#define Scene_RunScene3abSequenceA Func_02000980
#define Scene_RunScene3abSequenceB Func_020009d4
#define Scene_RunSupplementalSequenceThree Func_02000b6c
#define Scene_RunScene3abSequenceC Func_02000c10
#define Scene_RunScene3abSequenceE Func_02000c74
#define Scene_BuildFlaggedMapLayout Func_02000d30
#define FieldScene_RunScene3ab_02000eb0 Func_02000eb0
#define Scene_RunFlagGatedActorDialogue Func_02000ff0
#define FieldScene_RunScene3ab_020011d0 Func_020011d0
#define Scene_RunScene3abSequenceG Func_02001240

extern u8 Value_02008469;
extern u8 Data_00000068[];
extern u8 Data_0000009f[];
extern u8 Data_02000240[];
extern u8 Data_000024db[];
extern u8 Data_0200a50c[];
extern u8 Data_00002409[];

void Func_02001da4(void);
void Func_02001e0e(s32, s32, s32);
void Func_02001d9c(s32);
void Func_02001ebc(s32, s32);
s32 Func_02001dd4(s32, s32);
void Func_02001dd0(void);
s32 Func_02001e82();
void Func_02001e8a();
void Func_02001e96();
s32 Func_02001ea4();
void Func_02001eb0();
void Func_02001ede();
s32 Func_02001eee();
void Func_02001ef0();
s32 Func_02001efe();
void Func_02001f00();
void Func_02001f2c();
u8 *Func_02001f34();
void Func_02001f6e();
u8 *Func_02001f8e();
s32 Func_02001f9a();
void Func_02001f9c();
void Func_02001fc8();
void Func_02001fce();
void Func_02002000();
void Func_02001fa8();
void Func_02001fc2();
void Func_02001fd4();
void Func_02001fd6();
void Func_02001fea();
void Func_02002000_a();
void Func_02002042();
void Func_02002046();
void Func_0200204c();
void Func_02002050();
void Func_02002072();
void *Func_0200208e();
s32 Func_02002098();
void Func_020020d0();
void Func_0200214c();
void *Func_02001ff8();
void Func_020020a0(void);
void Func_02002078(s32, s32);
void Func_02002080(s32, s32);
void Func_020020bc(void);
void Func_020020bc_a();
s32 Func_020020cc();
void Func_02002148(void);
s32 Func_0200212e(s32);
void Func_020021e8_a(s32);
void Func_02002200(s32, s32);
void Func_020021f8(s32);
s32 Func_02002208(s32, s32);
s32 Func_02002190(s32, s32);
s32 Func_0200222c(s32, s32);
s32 Func_020021b4(s32, s32);
void Func_02002256(s32, s32);
void Func_020021c2(void);
s32 Func_020022ca_a();
s32 Func_020022d4();
void Func_02002310();
void Func_02002372();
void Func_0200238c();
void Func_0200239a();
void Func_020023b2_a();
void Func_020023b2_b();
void Func_020023c0();
void Func_020023c2();
void Func_020023d8();
s32 Func_0200235e();
void Func_02002406();
void Func_02002418();
void Func_02002420();
void Func_02002438();
s32 Func_020023aa();
s32 Func_020023b4();
s32 Func_020023be();
void Func_020023fa();
void Func_0200247c();
void Func_0200249a();
void Func_0200249c();
void Func_020024aa();
void Func_020024c0();
void Func_020024c2();
void Func_020024c6();
void Func_020024c8();
void Func_020024e0();
void Func_0200252a();
s32 Func_02002532();
void Func_02002554();
s32 Func_0200257e();
void Func_020025c2();
s32 Func_020025c6();
void Func_020025c8();
void Func_020025e6();
void Func_0200260c();
void Func_0200261c();
void Func_020026aa();
void Func_020026c4();
void Func_020026c6();
void Func_020026d2();
s32 Func_020025d6();
s32 Func_020025e0();
void Func_0200262c();
s32 Func_02002636();
void Func_02002638();
void Func_02002676();
void Func_0200268a();
void Func_02002712();
void Func_02002644();
void Func_0200264c();
void Func_02002660();
void Func_02002662();
s32 Func_02002690();
void Func_02002692();
void Func_020026a2();
void Func_020026b8();
void Func_020026da();
void Func_020026e6();
void Func_0200279e();
void Func_020027ba();
void Func_020027c6();
void Func_020027ca();
void Func_020027e0();
void Func_02002686();
void Func_020026c8();
s32 Func_020026fe();
void Func_02002702_a();
void Func_02002702_b();
s32 Func_0200270a();
void Func_02002716();
void Func_0200272a();
void Func_0200273c();
void Func_02002740();
void Func_02002752();
void Func_02002764();
void Func_02002776();
void Func_0200277a();
void Func_0200278c();
void Func_0200279a();
void Func_0200279e_a();
void Func_020027b0();
void Func_020027b4();
void Func_020027c6_a();
void Func_020027d8();
void Func_020027ee();
void Func_02002800();
void Func_02002812();
void Func_0200285e();
void Func_02002890();
void Func_020028a6();
void Func_020028bc();
void Func_020028f2();
void Func_02002904();
void Func_0200290e();
void Func_02002916_a();
void Func_02002916_b();
void Func_02002918();
void Func_0200291a();
void Func_02002934();
void Func_02002936();
void Func_0200293c();
void Func_0200295a();
void Func_02002972_a();
void Func_02002972_b();
void Func_02002974();
void Func_0200297e();
void Func_020029ae_a();
void Func_020029ae_b();
void Func_020029c6();
void Func_020029c8();
s32 Func_020029b8();
void Func_020029ce();
void Func_020029ea();
void Func_02002a14();
void Func_02002a68();
void Func_02002a6e();
s32 Func_02002a7c_a();
s32 Func_02002a7c_b();
void Func_02002a7e();
void Func_02002a86_a();
void Func_02002a86_b();
void Func_02002a8a();
void Func_02002a9e();
void Func_02002aa0();
void Func_02002ac0();
void Func_02002ad2();
void Func_02002ad8();
void Func_02002ae8();
void Func_02002b04();
void Func_02002b06_a();
void Func_02002b06_b();
void Func_02002b0c();
void Func_02002b14();
void Func_02002b1e_a();
void Func_02002b1e_b();
void Func_02002b1e_c();
void Func_02002b22();
void Func_02002b28();
void Func_02002b36();
void Func_02002b3c();
void Func_02002b46();
void Func_02002b48();
void Func_02002b4c();
void Func_02002b52();
void Func_02002b54();
void Func_02002b56();
void Func_02002b60();
void Func_02002b6c();
void Func_02002b78();
void Func_02002b84();
void Func_02002b8e();
void Func_02002b96();
void Func_02002b9c();
void Func_02002ba2();
void Func_020023a6();
void Func_02002b3c_a();
void Func_02002baa();
void Func_02002bb2();
s32 Func_02002be8();
void Func_02002bf2();
void Func_02002bb0();
s32 Func_02002c4a();

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Flag ids checked as guard conditions before the layout runs; neither
 * unused local below is read anywhere in the body. */

/* Fixed cell size used by every placement call below (args 3 and 4). */

/* One-shot flag gate: the scene flag this function is named for. */

/* Actor ids referenced throughout this scene. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void *Func_02002a10();

    return f();
}

static __inline__ void Call0(void (*f)())
{
    void *Func_02002a10();

    f();
}

static __inline__ void Call1_020005f0(void (*f)(), s32 a0)
{
    void *Func_02002a10();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void *Func_02002a10();

    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000b6c(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000c10(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

void Scene_RunStepFD1WithActor181(void)
{
    Func_02001da4();
    Func_02001e0e(20, 0, 0);
    Func_02001d9c(0xfd1);
    Func_02001ebc(181, 3);
    Func_02001dd4(181, 0);
    Func_02001dd0();
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;
    s32 v3;
    s32 v0;

    record = Value1(Func_02001ea4, 0);
    v3 = *(volatile s32 *)((s32)record + 8) / 0x100000;
    v0 = *(volatile s32 *)((s32)record + 16) / 0x100000;
    if (Value1(Func_02001e82, 0xf27) == 0) {
        if (v3 == 7) {
            if (v0 == 16) {
                Func_02002000(0, 0, 16);
            }
        }
        Call3(Func_02001fce, 102, -1, -1);
        Call6(Func_02001e8a, 28, 31, 1, 1, 7, 16);
    }
    Call6(Func_02001e96, 47, 4, 1, 1, 46, 4);
    Call6(Func_02001eb0, 34, 37, 3, 3, 13, 3);
    Call3(Func_02001f6e, 8, 0xe80000, 0x480000);
    record = Func_02001f34(8);
    *(volatile s32 *)((s32)record + 12) = 0;
    if (Value1(Func_02001efe, 0x202) != 0) {
        Call6(Func_02001ede, 41, 49, 3, 4, 1, 14);
        Call6(Func_02001ef0, 44, 49, 3, 4, 33, 14);
        Call6(Func_02001f00, 47, 49, 3, 4, 1, 46);
    } else {
        Call3(Func_02001fc8, 19, 0x380000, 0x10c0000);
        record = Func_02001f8e(19);
        Func_02001f2c((s32)record, 0);
        record = Value1(Func_02001f9a, 19);
        if ((s32)record != 0) {
            record[85] = 8;
            *(volatile s32 *)((s32)record + 12) = 0x100000;
            *(u8 *)((((s32)record + 85) - 50)) = 2;
            *(volatile s32 *)((s32)record + 24) = 0x13333;
            *(volatile s32 *)((s32)record + 28) = 0x18000;
        }
    }
    Value2(Func_02001eee, 0x2008469, 0xc80);
    Call1(Func_02001f9c, 0x201);
}

/* Sets up a handful of numbered records with position/pose-shaped argument
 * groups, then clears the s32 field at +108 of record 8 and feeds record 8's
 * id back into two more calls before two final single-argument calls. */
void Scene_RunOpeningSequence(void)
{
    void *Func_02002a10();

    void *unused;
    void *record8;

    unused = Scene_GetRecord_1(0);
    Motion_SetHPosTerrain_1(19, 0, 0);
    Motion_SetHPosTerrain_2(8, 0, 0);
    Call6(Func_02001fa8, 38, 38, 1, 1, 46, 4); /* main:080091b8 */
    Call6(Func_02001fc2, 37, 37, 3, 3, 13, 3); /* main:080091c0 */
    Call6(Func_02001fd6, 37, 37, 1, 1, 14, 2); /* main:080091c0 */
    Call6(Func_02001fea, 8, 16, 1, 1, 7, 16); /* main:080091c0 */
    EffectRuntime_SetCurrentPosition_1(102, 0, 0);
    Call6(Func_02002000_a, 32, 42, 3, 2, 1, 15); /* main:080091b8 */
    GameFlag_Clear_1(512);
    Object_SetModeById_1(8, 1);
    record8 = Scene_GetRecord_2(8);
    /* Clear the s32 field at offset 108 of record 8. */
    *(s32 *)(record8 + 108) = 0;
    Value2(Func_02002046, Scene_GetRecord_3(8), 0); /* main:08009240 */
    Value1(Func_02001fd4, (s32)&Value_02008469); /* main:080000d8 */
    GameFlag_Set_1(0x201);
}

void Scene_RunScriptedSteps947And29E1(void)
{
    Func_020020a0();
    Func_02002078(0x947, 1);
    Func_02002080(0x29e1, 1);
    Func_020020bc();
}

s32 Func_020006e4(void)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_0000009f) {
        Call1(Func_020020bc_a, 0x941);
        return 0x200a3b4;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000068) {
            record = Value1(Func_020020cc, 0x941);
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
    extern u8 *Data_03001ebc;

    Func_02002148();
    if (Func_0200212e(0x85a) == 0) {
        Func_020021e8_a(0x1be1);
        Func_02002200(18, 0);
    } else {
        Func_020021f8(0x1b9f);
        Func_02002208(18, 0);
        if (Func_02002190(0, 0) == 0) {
            u8 **base = &Data_03001ebc;
            (*(u16 *)(*base + 472))++;
            Func_0200222c(18, 0);
            if (Func_020021b4(0, 0) == 1)
                (*(u16 *)(*base + 472))++;
        }
        Func_02002256(18, 0);
    }
    Func_020021c2();
}

void Scene_RunSupplementalSequenceTwo(void)
{
    extern s16 Data_02000240_t[][2];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_24db;

    if (Data_02000240_t[147][0] != 0) {
        Call1(Func_02002372, 0x2411);
    } else {
        if (Value1(Func_020022ca_a, 0x941) != 0) {
            record = Value1(Func_020022d4, 0x94d);
            if (record != 0) {
                goto L_0200094c;
            }
            Call3(Func_020023c2, 8, 0x101, 60);
            base5_24db = (s32)Data_000024db;
            Func_0200239a(base5_24db);
            Func_020023b2_a(8, 0);
            Func_0200238c(8, 1);
            Func_020023b2_b((base5_24db + 1));
            Call1(Func_02002310, 0x9af);
        } else {
            L_0200094c:;
            Call1(Func_020023c0, 0x1bb5);
        }
    }
    Func_020023d8(8, 0);
}

void Scene_RunScene3abSequenceA(void)
{
    extern s16 Data_02000240_t[][2];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Data_02000240_t[147][0] != 0) {
        Call1(Func_02002406, 0x2412);
    } else {
        if (Value1(Func_0200235e, 0x941) != 0) {
            Call1(Func_02002418, 0x24dd);
        } else {
            Call1(Func_02002420, 0x1bb6);
        }
    }
    Func_02002438(9, 0);
}

void Scene_RunScene3abSequenceB(void)
{
    extern s16 Data_02000240_t[][2];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Data_02000240_t[147][0] == 0) {
        record = Value1(Func_020023aa, 0x941);
        if (record == 0) {
            goto L_02000a54;
        }
        record = Value1(Func_020023b4, 0x94d);
        if (record != 0) {
            goto L_02000a54;
        }
        record = Value1(Func_020023be, 0x9af);
        if (record == 0) {
            *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17e)) = record;
            Func_0200252a();
            Func_0200247c(8, 0, 0);
            Call3(Func_020024c6, 8, 0x101, 60);
            Call1(Func_0200249c, 0x24db);
            Call1(Func_020023fa, 0x9af);
        } else {
            Call1(Func_020024aa, 0x24e7);
        }
        Func_020024c2(8, 0);
        Func_0200249a(8, 1);
        Call1(Func_020024c0, 0x24dc);
    } else {
        L_02000a54:;
        Call1(Func_020024c8, 0x1bbf);
    }
    Func_020024e0(8, 0);
}

void Scene_RunSupplementalSequenceThree(void)
{
    extern u8 Data_03001ebc[];

    u8 *work;
    s16 *slot;
    u32 i;
    u8 *record;
    s32 kind;
    s32 off;
    s32 off4;
    u8 *table;

    work = *(u8 **)Data_03001ebc;
    Func_02002554();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Value1_02000b6c(Func_0200257e, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    Func_020026c4(158);
    slot = (s16 *)(work + 0x16c);
    kind = *slot - 4;
    off = kind << 3;
    table = Data_0200a50c;
    off4 = off + 4;
    Value3(Func_02002532, *(s32 *)(table + off), *(u16 *)(table + off4),
           *(u16 *)(table + off4 + 2));
    Call3(Func_020025c8, 0, 0x8000, 0x4000);
    *(u8 *)(Func_020025c6(0) + 85) = 0;
    Func_0200261c(0, 2);
    if (kind != 6) {
        Call3(Func_0200260c, 0, 2, -8);
        Func_020025c2(10);
    }
    Func_020026aa(*slot);
    Func_020026c6();
    Func_020026d2();
    Func_020025e6();
}

void Scene_RunScene3abSequenceC(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    if (Value1_02000c10(Func_020025d6, 0x202) != 0) {
        rec7 = Value1_02000c10(Func_020025e0, 0x201);
        if (rec7 == 0) {
            Call3(Func_02002638, 0, 0x3333, 0x1999);
            *(u8 *)(Func_02002636(0) + 85) = rec7;
            Func_0200268a(0, 2);
            Call3(Func_02002676, 0, 2, -8);
            Func_0200262c(13);
            Func_02002712(12);
        }
    }
}

void Scene_RunScene3abSequenceE(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Call1(Func_02002644, 0x242);
    Func_02002660();
    Call3(Func_02002692, 0, 0x3333, 0x1999);
    *(u8 *)(Func_02002690(0) + 85) = 0;
    Func_020026e6(0, 2);
    Call3(Func_020027ca, 0, 0, -8);
    v5 = 41;
    v6 = 4;
    Func_020027e0(158);
    Call6(Func_0200264c, 53, 4, 2, 2, v5, v6);
    Func_020026a2(10);
    Call6(Func_02002662, 53, 6, 2, 2, v5, v6);
    Func_020026b8(10);
    Func_0200279e(1);
    Func_020027ba();
    Func_020027c6();
    Func_020026da();
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
            ObjectMotion_SetHorizontalPositionWithTerrain_1_02000d30(19, 0, 0);
            Audio_PlayCue_1(210);
            Func_02002686(1);
            Call6(Func_02002702_a, 32, 45, CELL_W, CELL_H, 1, 14);
            Call6(Func_02002716, 35, 45, CELL_W, CELL_H, 33, 14);
            Call6(Func_0200272a, 38, 45, CELL_W, CELL_H, 1, 46);
            Func_020026c8(10);
            Call6(Func_02002740, 41, 45, CELL_W, CELL_H, 1, 14);
            Call6(Func_02002752, 44, 45, CELL_W, CELL_H, 33, 14);
            Call6(Func_02002764, 47, 45, CELL_W, CELL_H, 1, 46);
            Func_02002702_b(10);
            Call6(Func_0200277a, 50, 45, CELL_W, CELL_H, 1, 14);
            Call6(Func_0200278c, 53, 45, CELL_W, CELL_H, 33, 14);
            Call6(Func_0200279e_a, 56, 45, CELL_W, CELL_H, 1, 46);
            Func_0200273c(10);
            Call6(Func_020027b4, 32, 49, CELL_W, CELL_H, 1, 14);
            Call6(Func_020027c6_a, 35, 49, CELL_W, CELL_H, 33, 14);
            Call6(Func_020027d8, 38, 49, CELL_W, CELL_H, 1, 46);
            Func_02002776(10);
            Call6(Func_020027ee, 41, 49, CELL_W, CELL_H, 1, 14);
            Call6(Func_02002800, 44, 49, CELL_W, CELL_H, 33, 14);
            Call6(Func_02002812, 47, 49, CELL_W, CELL_H, 1, 46);
            Func_020027b0(10);
            GameFlag_Set_1_02000d30(GUARD_FLAG_2);
        }
    }
}

void FieldScene_RunScene3ab_02000eb0(void)
{
    extern s16 Data_02000240_t[][2];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Data_02000240_t[147][0] == 0) {
        Func_020028a6();
        Call3(Func_02002972_a, 8, 0x100, 2);
        Call3(Func_0200297e, 9, 0x100, 15);
        Func_020028bc(30);
        Func_0200290e(8, 152, 168);
        Func_02002918(9, 168, 168);
        Func_02002936(8);
        Func_0200293c(9);
        Func_0200291a(8);
        Func_0200295a(8, 0);
        Call3(Func_020029ae_a, 8, 0x3000, 0);
        Func_02002934(9);
        Func_02002974(9, 0);
        Call3(Func_020029c8, 9, 0x5000, 0);
        Call1(Func_020029ae_b, 0x24da);
        Func_020029c6(8, 0);
        Call1(Func_02002916_a, 0x240);
        Call6(Func_020028f2, 6, 11, 1, 1, 7, 11);
        Call6(Func_02002904, 6, 11, 1, 1, 8, 11);
        Call6(Func_02002916_b, 6, 11, 1, 1, 9, 11);
        Func_02002972_b();
    }
}

/* Runs once per save: if DIALOGUE_GATE_FLAG is already set the function does
 * nothing, otherwise it sets the flag and then positions actor 0's record,
 * actor 8, and actor 9, feeds four sequential bytes of the Data_00002409
 * buffer to their setup calls, and steps actors 8/9 and camera-target 0
 * through a sequence of move/pose calls. */
void Scene_RunFlagGatedActorDialogue(void)
{
    s32 Func_02002a10();

    u32 unused_counter;
    s32 actor0_record;
    s32 unused_record;
    s32 script_data;

    if (GameFlag_IsSet_1_02000ff0(DIALOGUE_GATE_FLAG)!= 0) {
    } else {
        GameFlag_Set_1_02000ff0(DIALOGUE_GATE_FLAG);
        Battle_Reset_1();
        actor0_record = Scene_GetRecord_1_02000ff0(0);
        Motion_SetAngleToward_1(ACTOR_8, 0, 0);
        Motion_SetAngleToward_2(ACTOR_9, 0, 0);
        Motion_SetVarCb_1(ACTOR_8, 1);
        Motion_SetVarCb_2(ACTOR_9, 1);
        Battle_WaitMode0_1(20);
        BattleFx_SpawnLinked_1(ACTOR_8, 0x102, 60);
        script_data = (s32)Data_00002409;
        SceneWork_SetStepValue_1(script_data);
        BattleEv_RunWait_1(ACTOR_8, 0);
        Motion_SetSpeed_1(0, 0x20000, 0x10000);
        Motion_SetSpeed_2(ACTOR_8, 0x20000, 0x10000);
        Motion_SetSpeed_3(ACTOR_9, 0x20000, 0x10000);
        Object_SetModeById_1_02000ff0(ACTOR_9, 4);
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
        Call2(Func_02002aa0, 0x2009241, 0xc80); /* main:080000d0 */
    }
}

void FieldScene_RunScene3ab_020011d0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(s32 *)Data_03001ebc;
    Call1(Func_02002baa, 0x241);
    Call1(Func_02002bb2, 0x240);
    record = Func_02002be8(0);
    if ((u32)(*(s32 *)(record + 8) + -0x680001) <= 0x87fffe) {
        if (*(s32 *)(record + 16) > 0xa00000) {
            if (*(s32 *)(record + 16) < 0xf80000) {
                Call1(Func_02002b3c_a, 0x2009241);
                {
                    u16 *target = (u16 *)(((s32)p5 + 0x182));
                    s32 shown = 91;

                    *target = shown;
                }
            }
        }
    }
    Func_020023a6();
    Call1(Func_02002bf2, 0x244);
}

void Scene_RunScene3abSequenceG(void)
{
    extern s16 Data_02000240_t[][2];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(s32 *)Data_03001ebc;
    record = Func_02002c4a(0);
    if (Data_02000240_t[147][0] == 0) {
        if ((u32)(*(s32 *)(record + 8) + -0x900000) <= 0x200000) {
            if (*(s32 *)(record + 16) >= 0xa80000) {
                if (*(s32 *)(record + 16) < 0xb00000) {
                    Call1(Func_02002bb0, 0x2009241);
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
#undef Scene_GetRecord_1
#undef Motion_SetHPosTerrain_1
#undef Motion_SetHPosTerrain_2
#undef EffectRuntime_SetCurrentPosition_1
#undef GameFlag_Clear_1
#undef Object_SetModeById_1
#undef Scene_GetRecord_2
#undef Scene_GetRecord_3
#undef GameFlag_Set_1
#undef GameFlag_IsSet_1
#undef GameFlag_IsSet_2
#undef ObjectMotion_SetHorizontalPositionWithTerrain_1_02000d30
#undef Audio_PlayCue_1
#undef GameFlag_Set_1_02000d30
#undef GUARD_FLAG_1
#undef GUARD_FLAG_2
#undef CELL_W
#undef CELL_H
#undef GameFlag_IsSet_1_02000ff0
#undef GameFlag_Set_1_02000ff0
#undef Battle_Reset_1
#undef Scene_GetRecord_1_02000ff0
#undef Motion_SetAngleToward_1
#undef Motion_SetAngleToward_2
#undef Motion_SetVarCb_1
#undef Motion_SetVarCb_2
#undef Battle_WaitMode0_1
#undef BattleFx_SpawnLinked_1
#undef SceneWork_SetStepValue_1
#undef BattleEv_RunWait_1
#undef Motion_SetSpeed_1
#undef Motion_SetSpeed_2
#undef Motion_SetSpeed_3
#undef Object_SetModeById_1_02000ff0
#undef Battle_WaitMode0_2
#undef SceneWork_SetStepValue_2
#undef BattleEv_RunWait_2
#undef BattleFx_SpawnLinked_2
#undef SceneWork_SetStepValue_3
#undef BattleEv_RunWait_3
#undef Object_SetModeById_2
#undef Battle_WaitMode0_3
#undef SceneWork_SetStepValue_4
#undef BattleEv_RunWait_4
#undef Motion_ResetPosMode2_1
#undef Motion_CommitPos_1
#undef Motion_ResetPosMode2_2
#undef Motion_ResetPosMode2_3
#undef Motion_ResetPosMode2_4
#undef Motion_CommitPos_2
#undef Motion_CommitPos_3
#undef Motion_CommitPos_4
#undef Motion_SetAngleToward_3
#undef Motion_SetAngleToward_4
#undef Battle_WaitMode0_4
#undef Motion_ResetPosMode2_5
#undef Motion_ResetPosMode2_6
#undef Motion_ResetPosMode2_7
#undef Motion_CommitPos_5
#undef Motion_CommitPos_6
#undef Motion_CommitPos_7
#undef Battle_SchedShoulder_1
#undef DIALOGUE_GATE_FLAG
#undef ACTOR_8
#undef ACTOR_9
#undef Scene_RunStepFD1WithActor181
#undef Scene_RunSupplementalSequenceOne
#undef Scene_RunOpeningSequence
#undef Scene_RunScriptedSteps947And29E1
#undef Scene_RunActorEighteenPromptDialogue
#undef Scene_RunSupplementalSequenceTwo
#undef Scene_RunScene3abSequenceA
#undef Scene_RunScene3abSequenceB
#undef Scene_RunSupplementalSequenceThree
#undef Scene_RunScene3abSequenceC
#undef Scene_RunScene3abSequenceE
#undef Scene_BuildFlaggedMapLayout
#undef FieldScene_RunScene3ab_02000eb0
#undef Scene_RunFlagGatedActorDialogue
#undef FieldScene_RunScene3ab_020011d0
#undef Scene_RunScene3abSequenceG

/* overlays/scene/story/actor_sequence/scene_setup.c */
#define Scene_DrawTilesAndResetActor8 Func_020003f4
#define Scene_RunLeaderMovementSequence Func_020015bc
#define Scene_InitializeStoryActors Func_02001668
extern u8 Data_03001ebc[];
extern u8 Data_00000068[];
extern u8 Data_0000009f[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

u8 *Func_02001dfc(s32);
u8 *Func_02001e04(s32);
void Func_02001da8(s32, s32, s32, s32, s32, s32);
void Func_02001dba(s32, s32, s32, s32, s32, s32);
void Func_02001dcc(s32, s32, s32, s32, s32, s32);
void Func_02001de0(u8 *, s32);
void Func_02001e26(s32);
void Func_02002fa0();
void Func_02002fb2();
void Func_02002fee();
void Func_0200300c();
void Func_02003022();
void Func_02003032();
void Func_02003040();
void Func_02003042();
void Func_02003048();
void Func_02003050();
void Func_02003052();
void Func_02003060();
void Func_02003064();
void Func_02003070();
void Func_02003076();
void Func_02003082();
void Func_0200308c();
void Func_02003094();
void Func_020030da();
void Func_020030e4();
void Func_020030ea();
void Func_020030fe();
void Func_02003100();
void Func_02001c7c_init();
void Func_02002a10_init();
void Func_02002cce_init();
void Func_02002edc_init();
void Func_02003044_init();
s32 Func_02003052_init();
void Func_02003058_init();
void Func_02003062_init();
void Func_0200307a_init();
s32 Func_020030a2_init();
s32 Func_020030ac_init();
s32 Func_020030ca_init();
s32 Func_020030d8_init();
s32 Func_020030e2_init();
void Func_02003110_init();
void Func_02003116_init();
void Func_0200311c_init();
void Func_02003122_init();
void Func_02003128_init();
void Func_0200312e_init();
void Func_02003134_init();
void Func_0200313a_init();
void Func_02003140_init();
void Func_02003146_init();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void Scene_DrawTilesAndResetActor8(void)
{
    u8 *actor = Func_02001dfc(8);
    Func_02001e04(0);
    {
        s32 cell = 14;
        s32 row = 4;
        Func_02001da8(17, 4, 1, 1, cell, row);
        Func_02001dba(15, 3, 1, 1, 15, row);
        Func_02001dcc(15, 3, 1, 1, 13, row);
    }
    if (actor != 0) {
        Func_02001de0(actor, 0);
        actor[0x55] = 2;
        actor[0x23] = 1;
    }
    Func_02001e26(0x200);
}

void Scene_RunLeaderMovementSequence(void)
{
    Func_02002fa0();
    Func_0200308c();
    Func_02002fee(0, 152, 168);
    Func_0200300c(0);
    Func_02002fb2(20);
    Func_020030da(146, 1);
    Func_020030ea(0, 0);
    Func_020030fe();
    Func_020030e4(1);
    Func_02003100();
    Func_02003022(0, 144, 184);
    Func_02003040(0);
    Func_02003032(0, 88, 184);
    Func_02003050(0);
    Func_02003042(0, 88, 200);
    Func_02003060(0);
    Func_02003052(0, 72, 200);
    Func_02003070(0);
    Call3(Func_02003064, 0, 72, 0x120);
    Func_02003082(0);
    Call3(Func_02003076, 0, 88, 0x120);
    Func_02003094(0);
    Func_02003048();
}

s32 Scene_InitializeStoryActors(void)
{
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_00000068) {
        *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
        Func_02001c7c_init();
        if (Value1(Func_02003052_init, 0xfd1) == 0) {
            Func_02002edc_init(20);
        }
        record = Value1(Func_020030a2_init, 8);
        if (record != 0) {
            Func_02003044_init(record, 0);
        }
        Call1(Func_0200307a_init, 0x201);
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000009f) {
        *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
        Data_02000240_t[289][0] = 10;
        if (Data_02000240_t[225][0] == 4) {
            if (Value1(Func_020030ac_init, 0x109) == 0) {
                Func_02002a10_init();
            }
        }
        if (Data_02000240_t[225][0] == 3) {
            if (Value1(Func_020030ca_init, 0x109) == 0) {
                Func_02002cce_init();
            }
        }
        if (Value1(Func_020030d8_init, 0x941) != 0) {
            if (Value1(Func_020030e2_init, 0x94d) == 0) {
                Call2(Func_02003058_init, 0x2008ac5, 0xc80);
            }
        }
        Call2(Func_02003062_init, 0x2009241, 0xc80);
        Call1(Func_02003110_init, 0x944);
        Call1(Func_02003116_init, 0x945);
        Call1(Func_0200311c_init, 0x946);
        Call1(Func_02003122_init, 0x947);
        Call1(Func_02003128_init, 0x948);
        Call1(Func_0200312e_init, 0x943);
        Call1(Func_02003134_init, 0x949);
        Call1(Func_0200313a_init, 0x94a);
        Call1(Func_02003140_init, 0x94b);
        Call1(Func_02003146_init, 0x94c);
    }
    return 0;
}
#undef Scene_DrawTilesAndResetActor8
#undef Scene_RunLeaderMovementSequence
#undef Scene_InitializeStoryActors

/* overlays/scene/story/actor_sequence/scene_wrapper.c */
#define Scene_CallHelper27cc Func_02000d08
void Func_020027cc(void);

void Scene_CallHelper27cc(void) { Func_020027cc(); }
#undef Scene_CallHelper27cc
