#include "types.h"

#define NULL ((void *)0)
#define CreateOverlayObject Func_020012d6
#define SetOverlayObjectMode Func_02001318
#define SetOverlayObjectSlot Func_020013d8
#define OverlayObject_CreateAndInitialize Func_02000048
#define OverlayObject_CreateConfigured Func_020000a0

#include "create_configured_overlay_object.h"

void *Func_0200127e(s32, s32, s32, s32);
void Func_020012bc(void *, s32);
void Func_020012d4(void *, s32);
void Func_0200137c(void *, s32);

#include "types.h"

#define EffectDescriptorTable Data_02009424
#define GetPartyEffect Func_020013ea
#define SpawnEffect Func_020013a8
#define SetEffectVariant Func_020013b2
#define SetEffectDescriptor Func_020013cc
#define SetEffectMode Func_0200150e
#define ScaleEffectDeltaFromAccumulated Func_02001468
#define ScaleEffectDeltaFromOrigin Func_02001480
#define ScaleEffectVerticalDelta Func_0200148e
#define SetEffectCallbackMode Func_020014cc
#define SetEffectCallbackArgument Func_020014dc
#define SpawnConfiguredEffect Func_0200013c

#include "configured_effect_spawn.h"

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct Selection_02000400 {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor_02000400 {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork_02000400 {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern volatile s32 Data_03001e40;

struct Actor_02000400 *Func_0200169a(s32 actor);
s32 Func_02001686(s32 state);
s32 Func_0200168c(s32 flag);
void Func_020016bc(s32 state, s32 value);

#include "types.h"

#define SceneState_SendRequest15With45 Func_02000314
#define SceneState_SetStateByte52 Func_02000b14
void Func_02001660();

/* Named shorthand for one fixed state request, in overlay resource_3c0. */

/*
 * The 14-byte owner at 0x02000314 loads no literal and has no pool word.
 * Both arguments are immediates, and Func_02001660 spells the overlay's own
 * relocated call word rather than a runtime address.
 */

#include "types.h"

#define SceneData_SelectDataByRuntimeSelector Func_0200034c
#define SceneData_ReturnZero Func_020003a0
#define SceneData_GetTable95c0 Func_020003a4

extern s16 Data_02000240[];
extern u8 Value_000000a4;
extern u8 Value_000000a5;
extern u8 Value_000000a6;
extern u8 Data_02009488[];
extern u8 Data_020094d0[];
extern u8 Data_02009548[];
extern u8 Data_02009458[];

/* Contiguous unnamed leaf-owner run for resource_3c0. */



/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */


/* Complete eight-byte literal-address getter, including its sole pool word. */

#include "types.h"

#define SceneData_SelectOverlayDataByRuntimeSelector Func_020003ac
#define SceneData_SelectTable99C4Or97B4ByStateA6 Func_02000e5c

extern s16 Data_02000240[];
extern u8 Value_000000a4;
extern u8 Value_000000a5;
extern u8 Value_000000a6;
extern u8 Data_02009610[];
extern u8 Data_020096b8[];
extern u8 Data_02009790[];
extern u8 Data_020095f8[];

extern s16 Data_02000240[];
extern u8 Value_000000a6;
extern u8 Data_020099c4[];
extern u8 Data_020097b4[];

#include "types.h"

#define GameFlag_IsSet_1(a0) Value1(Func_02001a8c, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02001a98, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02001aa6, a0)
#define GameFlag_Set_1(a0) Call1(Func_02001aba, a0)
#define BattleRuntime_Reset_1(args...) Func_02001ae6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001b6c, a0)
#define Scene_GetRecord_1(args...) Func_02001b02(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02001b50(args)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_02001b7c, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02001b5a, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02001bbe, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02001b74, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02001be0, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02001be4, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02001bb4(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02001be4_a(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02001c06, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02001c10, a0, a1, a2)
#define BattleEvent_RunActionAndWait_2(args...) Func_02001c00(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02001b76(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02001bee(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02001c16(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02001c32, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(args...) Func_02001c32_a(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02001ba0(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02001c54, a0, a1, a2)
#define BattleEvent_RunActionAndWait_4(args...) Func_02001c44(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02001c24(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_02001c54_a(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02001bee_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02001c24_a, a0, a1, a2)
#define BattleEvent_RunActionAndWait_6(args...) Func_02001c74(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02001c54_b(args)
#define Object_SetModeById_1(args...) Func_02001c54_c(args)
#define Scene_GetRecord_2(args...) Func_02001c12(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02001c44_a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02001c62(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02001c74_a(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001c30(args)
#define ACTOR_ID 13
#define FieldScene_RunMiddleAuxiliarySequence Func_02000464
#define FieldScene_RunActorThirteenRestoration Func_02000838

extern u8 Data_03001ebc[];
extern u8 Data_000000a5[];
extern u8 Data_02009a00[];
extern s16 Data_02000240_t[][1];

s32 Func_020016d4();
s32 Func_020016da();
s32 Func_0200170c();
void Func_02001714();
void Func_02001714_a();
void Func_0200171c();
s32 Func_0200173a();
void Func_02001762();
void Func_0200176c();
void Func_0200177c();
void Func_02001784();
void Func_0200178a();
void Func_0200178e();
void Func_020017aa();
void Func_020017ba();
void Func_020017c4();
void Func_02001840();
s32 Func_02001a8c();
s32 Func_02001a98();
s32 Func_02001aa6();
void Func_02001aba();
void Func_02001ae6();
u8 *Func_02001b02();
void Func_02001b50();
void Func_02001b5a();
void Func_02001b6c();
void Func_02001b74();
void Func_02001b76();
void Func_02001b7c();
void Func_02001ba0();
void Func_02001bb4();
void Func_02001bbe();
void Func_02001be0();
void Func_02001be4();
void Func_02001be4_a();
void Func_02001bee();
void Func_02001bee_a();
void Func_02001c00();
void Func_02001c06();
void Func_02001c10();
u8 *Func_02001c12();
void Func_02001c16();
void Func_02001c24();
void Func_02001c24_a();
void Func_02001c30();
void Func_02001c32();
void Func_02001c32_a();
void Func_02001c44();
void Func_02001c44_a();
void Func_02001c54();
void Func_02001c54_a();
void Func_02001c54_b();
void Func_02001c54_c();
void Func_02001c62();
void Func_02001c74();
void Func_02001c74_a();
void Func_02001fa2();
s32 Func_02001fa8();
void Func_02001fbe();
s32 Func_02001fca();
s32 Func_02001fd2();
s32 Func_02001fd6();
void Func_02002026();
void Func_020020d4();
s32 Func_02002042();
void Func_0200204a();
s32 Func_0200206e();
void Func_02002070();
s32 Func_0200207a();
void Func_020020ce();
void Func_02002180();
void Func_0200218c();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Actor id operated on throughout this scene. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Resolved from the main-image function's own source (not registered in the
 * overlay veneer table): each macro names what that source shows the
 * function doing, keeping the site's own calling form. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000838(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000d24(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value2_02000dc8(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}



/* Runs a scene step only when three preceding gate checks all pass;
 * otherwise the function does nothing. Drives actor ACTOR_ID through a
 * long sequence of position/pose/flag updates, applying two record-driven
 * position fixups read back from lookup calls partway through. */

#include "types.h"

#define FieldScene_RunActor8Step Func_02000558
#define FieldScene_RunActor9Step Func_02000564
#define FieldScene_RunActor10Step Func_02000570
#define FieldScene_RunActor11Step Func_0200057c
#define FieldScene_RunActor12Step Func_02000588
#define FieldScene_RunLateActor8Step Func_020007fc
#define FieldScene_RunLateActor9Step Func_02000808
#define FieldScene_RunLateActor10Step Func_02000814
#define FieldScene_RunLateActor11Step Func_02000820
#define FieldScene_RunLateActor12Step Func_0200082c
#define PlaceActorTwelveWhenFlagClear Func_02000adc
void Func_020009c2(int actor);
void Func_020009ce(int actor);
void Func_020009da(int actor);
void Func_020009e6(int actor);
void Func_020009f2(int actor);
void Func_02000d96(int actor);
void Func_02000da2(int actor);
void Func_02000dae(int actor);
void Func_02000dba(int actor);
void Func_02000dc6(int actor);
s32 Func_02001d2e(s32);
void Func_02001d40(s32);
void Func_02001d9e(s32, const void *);
void Func_02001dc6(s32, s32, s32);

/* Actor-step entry points for the two scene phases. */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02001dc6(actor, x, y);
}

#include "types.h"

#define FrameCounter (*(u32 *)0x03001e40)
#define EncounterPalette (*(volatile u16 *)0x0500019e)
#define EncounterPalette_Pulse Func_02000ce4

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

#define SetEffectRecordMode Func_02000030
#define OverlayObject_ApplyZeroAndClearByte89 Func_02000324
#define OverlayObject_ToggleField84Bit0 Func_0200033c
void Func_02001568();

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_0200127e(arg3, arg0, arg1, arg2);

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
        Func_020012bc(obj, 0);
        Func_0200137c(obj, 14);
        Func_020012d4(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

void SceneState_SendRequest15With45(void)
{
    Func_02001660(15, 45);
}

s32 OverlayObject_ApplyZeroAndClearByte89(u8 *obj)
{
    Func_02001568(obj, 0);
    obj[89] = 0;
    return 0;
}

s32 OverlayObject_ToggleField84Bit0(u8 *obj)
{
    obj[84] ^= 1;
    return 1;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 selector = Data_02000240[224];

    if (selector == (s32)&Value_000000a4) {
        return (s32)Data_02009488;
    }
    if (selector == (s32)&Value_000000a5) {
        return (s32)Data_020094d0;
    }
    if (selector == (s32)&Value_000000a6) {
        return (s32)Data_02009548;
    }
    return (s32)Data_02009458;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable95c0(void)
{
    return (u8 *)0x020095c0;
}

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
    s16 selector = Data_02000240[224];

    if (selector == (s32)&Value_000000a4) {
        return (s32)Data_02009610;
    }
    if (selector == (s32)&Value_000000a5) {
        return (s32)Data_020096b8;
    }
    if (selector == (s32)&Value_000000a6) {
        return (s32)Data_02009790;
    }
    return (s32)Data_020095f8;
}

void Func_02000400(void)
{
    struct Actor_02000400 *actor;
    struct SceneWork_02000400 *scene;
    s32 progress;

    actor = Func_0200169a(((struct Selection_02000400 *)Data_02000240)->actor_id);
    scene = *(struct SceneWork_02000400 **)Data_03001ebc;
    actor->presentation = (u16)(Data_03001e40 << 12);

    progress = Func_02001686(0x210);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Func_0200168c(0x106) == 0) {
            progress -= 1;
        }
    }
    Func_020016bc(0x210, progress);
}

void FieldScene_RunMiddleAuxiliarySequence(s32 a0)
{
    s32 p10;
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = (u8 *)Data_02000240;
    p6 = *(volatile s32 *)(base + 500);
    p10 = a0;
    rec7 = Value1(Func_0200170c, (s32)p6);
    Func_02001714(p10);
    rec2 = Value1(Func_020016da, 0x20f);
    if (rec2 == 0) {
        Func_02001714_a();
        Call2(Func_020017c4, (s32)p6, 0x101);
        Func_0200177c((s32)p6, 9);
        record = Value1(Func_0200173a, p10);
        if (record != 0) {
            Func_0200176c((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200178a((s32)p6);
        Func_02001840(244);
        Value2(Func_020016d4, 0x2008401, 0xc80);
        rec7[85] = rec2;
        {
            s32 v12 = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;
            Func_0200171c((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), v12, *(volatile s32 *)((s32)rec7 + 16));
        }
        Func_020017ba((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Call1(Func_02001762, 0x20f);
        Call2(Func_02001784, 0x218, p10);
        Call2(Func_0200178e, 0x210, 180);
        Func_020017aa();
        *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17c)) = rec2;
    }
}

void FieldScene_RunActor8Step(void) { Func_020009c2(8); }

void FieldScene_RunActor9Step(void) { Func_020009ce(9); }

void FieldScene_RunActor10Step(void) { Func_020009da(10); }

void FieldScene_RunActor11Step(void) { Func_020009e6(11); }

void FieldScene_RunActor12Step(void) { Func_020009f2(12); }

void FieldScene_RunLateActor8Step(void) { Func_02000d96(8); }

void FieldScene_RunLateActor9Step(void) { Func_02000da2(9); }

void FieldScene_RunLateActor10Step(void) { Func_02000dae(10); }

void FieldScene_RunLateActor11Step(void) { Func_02000dba(11); }

void FieldScene_RunLateActor12Step(void) { Func_02000dc6(12); }

void FieldScene_RunActorThirteenRestoration(void)
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
                BattleRuntime_Reset_1();
                SceneWork_SetStepValue_1(0x2633);
                /* Record layout observed here: s32 at +8, s32 at +16. */
                record = Scene_GetRecord_1(0);
                if (record != 0) {
                    ObjectMotion_SetHorizontalPositionWithTerrain_1(ACTOR_ID, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                ObjectMotion_SetAngleToward_1(ACTOR_ID, 0xc000, 0);
                ObjectMotion_SetPositionAndReset_1(0, 0x1b8, 0x4e8);
                ObjectMotion_ArmCallback_1(ACTOR_ID, 0x4000, 0);
                ObjectMotion_SetPositionAndReset_2(0, 0x1bc, 0x4d8);
                BattleEffect_SpawnLinkedResourceObject_1(0, 0x100, 40);
                ObjectMotion_ArmCallback_2(0, 0x4000, 30);
                ObjectMotion_CallThenWaitForAnimationChange_1(ACTOR_ID, 4);
                BattleEvent_RunActionAndWait_1(ACTOR_ID, 0);
                BattleEffect_SpawnLinkedResourceObject_2(0, 0x105, 60);
                BattleEffect_SpawnLinkedResourceObject_3(ACTOR_ID, 0x105, 60);
                BattleEvent_RunActionAndWait_2(ACTOR_ID, 0);
                BattleRuntime_WaitIfModeZero_1(30);
                ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_ID, 2);
                BattleEvent_RunActionAndWait_3(ACTOR_ID, 0);
                ObjectMotion_ArmCallback_3(ACTOR_ID, 0xc000, 30);
                BattleEventRuntime_ProcessAction_1(ACTOR_ID, 0);
                BattleRuntime_WaitIfModeZero_2(30);
                BattleEffect_SpawnLinkedResourceObject_4(ACTOR_ID, 0x106, 60);
                BattleEvent_RunActionAndWait_4(ACTOR_ID, 0);
                ObjectMotion_CallThenWaitForAnimationChange_2(ACTOR_ID, 3);
                BattleEvent_RunActionAndWait_5(ACTOR_ID, 0);
                ObjectMotion_SetSpeedParameters_1(ACTOR_ID, 0xb333, 0x5999);
                ObjectMotion_SetPositionAndReset_3(ACTOR_ID, 0x1b8, 0x4e8);
                BattleEvent_RunActionAndWait_6(ACTOR_ID, 0);
                ObjectMotion_CallThenWaitForAnimationChange_3(0, 3);
                Object_SetModeById_1(ACTOR_ID, 2);
                /* Record layout observed here: s16 at +10, s16 at +18. */
                record = Scene_GetRecord_2(0);
                if (record != 0) {
                    ObjectMotion_ResetAndSetPosition_1(ACTOR_ID, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                ObjectMotion_CommitCurrentPositionAndActivate_1(ACTOR_ID);
                ObjectMotion_SetHorizontalPositionWithTerrain_2(ACTOR_ID, 0, 0);
                BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
            }
        }
    }
}

void PlaceActorTwelveWhenFlagClear(void)
{
    if (Func_02001d2e(2487) == 0) {
        Func_02001d40(526);
        PlaceActor(12, 240 << 15, 206 << 18);
        Func_02001d9e(12, (const void *)0x020097a8);
    }
}

void SceneState_SetStateByte52(void)
{
    u8 *state = *(u8 *volatile *)0x03001f30;
    state[52] = 1;
}

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

s32 Func_02000d24(s32 a0)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_000000a5) {
        *(u8 *)(Func_02001fca(14) + 35) = 2;
        *(u8 *)(Func_02001fd6(14) + 85) = 3;
        Func_02002026(14, 0, 0);
        Call6(Func_02001fa2, 16, 44, 1, 1, 15, 44);
        Func_020020d4(100, 0, 0);
        Call6(Func_02001fbe, 12, 71, 1, 1, 127, 127);
        Value6(Func_02001fd2, 11, 71, 1, 1, 12, 71);
        record = Value1_02000d24(Func_02001fa8, 0x2008ce5);
        do {
            s32 shown = *(u16 *)Data_02009a00;

            *(volatile u16 *)0x0500019e = shown;
        } while (0);
        return record;
    }
    return a0;
}

s32 Func_02000dc8(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    if (Data_02000240_t[224][0] == (s32)Data_000000a5) {
        *(u8 *)(Func_0200206e(14) + 35) = 2;
        v5 = 0;
        *(u8 *)(Func_0200207a(14) + 85) = v5;
        Call3(Func_020020ce, 14, 0xf80000, 0x2c80000);
        Call6(Func_0200204a, 31, 95, 1, 1, 15, 44);
        Call3(Func_02002180, 100, -1, -1);
        Func_0200218c();
        Call6(Func_02002070, 127, 127, 1, 1, 12, 71);
        record = Value2_02000dc8(Func_02002042, 0x2008ce5, 0xc80);
        return record;
    }
    return a0;
}

s32 SceneData_SelectTable99C4Or97B4ByStateA6(void)
{
    if (Data_02000240[224] == (s32)&Value_000000a6) {
        return (s32)Data_020099c4;
    }
    return (s32)Data_020097b4;
}
