#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define CreateOverlayObject Func_020012d6
#define SetOverlayObjectMode Func_02001318
#define SetOverlayObjectSlot Func_020013d8

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

#include "TYPES.H"

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

s32 Func_02001686(s32 state);
void Func_020016bc(s32 state, s32 value);

#include "TYPES.H"

void Func_02001660();

/* Named shorthand for one fixed state request, in overlay resource_3c0. */

/*
 * The 14-byte owner at 0x02000314 loads no literal and has no pool word.
 * Both arguments are immediates, and Func_02001660 spells the overlay's own
 * relocated call word rather than a runtime address.
 */

#include "TYPES.H"

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

#include "TYPES.H"

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

#include "TYPES.H"

#define ACTOR_ID 13

extern u8 Data_03001ebc[];
extern u8 Data_000000a5[];
extern u8 Data_02009a00[];
extern s16 Data_02000240_t[][1];

s32 Func_020016d4();
void Func_0200171c();
void Func_02001784();
void Func_0200178e();
s32 Func_02001fa8();
s32 Func_02001fca();
s32 Func_02001fd6();
s32 Func_02002042();
s32 Func_0200206e();
s32 Func_0200207a();
void Func_0200218c();

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

/* Resolved from the main-image function's own source (not registered in the
 * overlay veneer table): each macro names what that source shows the
 * function doing, keeping the site's own calling form. */

static __inline__ void Call1_02000838(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

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

static __inline__ s32 Value2_02000dc8(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Runs a scene step only when three preceding gate checks all pass;
 * otherwise the function does nothing. Drives actor ACTOR_ID through a
 * long sequence of position/pose/flag updates, applying two record-driven
 * position fixups read back from lookup calls partway through. */

#include "TYPES.H"

void Func_02000d96(int actor);
void Func_02000da2(int actor);
void Func_02000dae(int actor);
void Func_02000dba(int actor);
void Func_02000dc6(int actor);

/* Actor-step entry points for the two scene phases. */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

#include "TYPES.H"

#define FrameCounter (*(u32 *)0x03001e40)
#define EncounterPalette (*(volatile u16 *)0x0500019e)

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

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Object_Create(arg3, arg0, arg1, arg2);

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
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 14);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

void SceneState_SendRequest15With45(void)
{
    Func_02001660(15, 45);
}

s32 OverlayObject_ApplyZeroAndClearByte89(u8 *obj)
{
    Actor_SetSpriteFlags(obj, 0);
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
    s16 selector = gGameState.scene;

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
    s16 selector = gGameState.scene;

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

    actor = Actor_Get(((struct Selection_02000400 *)Data_02000240)->actor_id);
    scene = *(struct SceneWork_02000400 **)Data_03001ebc;
    actor->presentation = (u16)(Data_03001e40 << 12);

    progress = Func_02001686(0x210);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (GameFlag_IsSet(0x106) == 0) {
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
    p6 = *(u8 **)(base + 500);
    p10 = a0;
    rec7 = Value1(Engine_ActorGet, (s32)p6);
    Actor_Get(p10);
    rec2 = GameFlag_IsSet(0x20f);
    if (rec2 == 0) {
        Event_Begin();
        Actor_SetAttachedEffect((s32)p6, 0x101);
        Actor_SetAnimation((s32)p6, 9);
        record = Value1(Engine_ActorGet, p10);
        if (record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove((s32)p6);
        Audio_PlayCue(244);
        Value2(Func_020016d4, 0x2008401, 0xc80);
        rec7[85] = rec2;
        Func_0200171c((s32)rec7, *(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12) + 0x200000, *(s32 *)(rec7 + 16));
        Actor_WaitForMove((s32)p6);
        *(s32 *)(rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        GameFlag_Set(0x20f);
        Call2(Func_02001784, 0x218, p10);
        Call2(Func_0200178e, 0x210, 180);
        Event_End();
        *(u16 *)(*(u8 **)Data_03001ebc + 0x17c) = rec2;
    }
}

void FieldScene_RunActor8Step(void) { FieldScene_RunMiddleAuxiliarySequence(8); }

void FieldScene_RunActor9Step(void) { FieldScene_RunMiddleAuxiliarySequence(9); }

void FieldScene_RunActor10Step(void) { FieldScene_RunMiddleAuxiliarySequence(10); }

void FieldScene_RunActor11Step(void) { FieldScene_RunMiddleAuxiliarySequence(11); }

void FieldScene_RunActor12Step(void) { FieldScene_RunMiddleAuxiliarySequence(12); }

void FieldScene_RunLateActor8Step(void) { Func_02000d96(8); }

void FieldScene_RunLateActor9Step(void) { Func_02000da2(9); }

void FieldScene_RunLateActor10Step(void) { Func_02000dae(10); }

void FieldScene_RunLateActor11Step(void) { Func_02000dba(11); }

void FieldScene_RunLateActor12Step(void) { Func_02000dc6(12); }

void FieldScene_RunActorThirteenRestoration(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet(0x1b7) != 0) {
        } else {
            if (GameFlag_IsSet(0x9b0) == 0) {
            } else {
                GameFlag_Set(0x9b5);
                Event_Begin();
                Event_SetMessage(0x2633);
                /* Record layout observed here: s32 at +8, s32 at +16. */
                record = Actor_Get(0);
                if (record != 0) {
                    Actor_SetPosition(ACTOR_ID, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                Actor_FaceActor(ACTOR_ID, 0xc000, 0);
                Actor_WalkToAndWait(0, 0x1b8, 0x4e8);
                Actor_FaceDirection(ACTOR_ID, 0x4000, 0);
                Actor_WalkToAndWait(0, 0x1bc, 0x4d8);
                Actor_ShowEmote(0, 0x100, 40);
                Actor_FaceDirection(0, 0x4000, 30);
                Actor_SetAnimationAndWait(ACTOR_ID, 4);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_ShowEmote(0, 0x105, 60);
                Actor_ShowEmote(ACTOR_ID, 0x105, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Event_Wait(30);
                Actor_RunRepeatedMotion(ACTOR_ID, 2);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_FaceDirection(ACTOR_ID, 0xc000, 30);
                Event_AskYesNo(ACTOR_ID, 0);
                Event_Wait(30);
                Actor_ShowEmote(ACTOR_ID, 0x106, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetAnimationAndWait(ACTOR_ID, 3);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetSpeed(ACTOR_ID, 0xb333, 0x5999);
                Actor_WalkToAndWait(ACTOR_ID, 0x1b8, 0x4e8);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetAnimationAndWait(0, 3);
                Actor_SetAnimation(ACTOR_ID, 2);
                /* Record layout observed here: s16 at +10, s16 at +18. */
                record = Actor_Get(0);
                if (record != 0) {
                    Actor_SetDestination(ACTOR_ID, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Actor_WaitForMove(ACTOR_ID);
                Actor_SetPosition(ACTOR_ID, 0, 0);
                Event_End();
            }
        }
    }
}

void PlaceActorTwelveWhenFlagClear(void)
{
    if (GameFlag_IsSet(2487) == 0) {
        GameFlag_Set(526);
        PlaceActor(12, 240 << 15, 206 << 18);
        Engine_ActorEnableActionCallback(12, (const void *)0x020097a8);
    }
}

void SceneState_SetStateByte52(void)
{
    u8 *state = *(u8 **)0x03001f30;
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
        Actor_SetPosition(14, 0, 0);
        Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
        MapObject_SetPosition(100, 0, 0);
        Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
        Value6(Engine_MapCopyCellAttributes, 11, 71, 1, 1, 12, 71);
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
        Actor_SetPosition(14, 0xf80000, 0x2c80000);
        Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
        MapObject_SetPosition(100, -1, -1);
        Func_0200218c();
        Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
        record = Value2_02000dc8(Func_02002042, 0x2008ce5, 0xc80);
        return record;
    }
    return a0;
}

s32 SceneData_SelectTable99C4Or97B4ByStateA6(void)
{
    if (gGameState.scene == (s32)&Value_000000a6) {
        return (s32)Data_020099c4;
    }
    return (s32)Data_020097b4;
}
