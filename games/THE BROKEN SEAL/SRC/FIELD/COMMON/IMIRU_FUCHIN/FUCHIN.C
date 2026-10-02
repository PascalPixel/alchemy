/* The two page effects the cave's opening runs. */
#include "IMIRU_FUCHIN.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
extern u8 MsgFuchinDragonFlameShowsPath[];
extern u8 MsgFuchinSecretKiRevealed[];
extern u8 MsgFuchinLightRevealsShadows[];
extern u8 MsgFuchinEyelessDragon[];
extern u8 MsgFuchinDragonRedEyes[];
#include "FIELD_EFFECT.H"

extern const struct SceneEntrance gImiruFuchinEntrances1[];
extern const struct SceneEntrance gImiruFuchinEntrances2[];
extern const struct SceneEntrance gImiruFuchinEntrances3[];
extern const struct SceneEntrance gImiruFuchinEntrances4[];
extern const struct SceneEntrance gImiruFuchinEntrances5[];
extern const struct SceneEntrance gImiruFuchinEntrances6[];
extern const struct SceneEntrance gImiruFuchinEntrances7[];
extern const struct SceneEntrance gImiruFuchinEntrancesOther[];
extern const struct ScenePlacement gImiruFuchinPlacements1[];
extern const struct ScenePlacement gImiruFuchinPlacements2[];
extern const struct ScenePlacement gImiruFuchinPlacements3[];
extern const struct ScenePlacement gImiruFuchinPlacements4[];
extern const struct ScenePlacement gImiruFuchinPlacements5[];
extern const struct ScenePlacement gImiruFuchinPlacements7[];
extern const struct ScenePlacement gImiruFuchinPlacementsOther[];
extern const struct SceneEvent gImiruFuchinEvents1[];
extern const struct SceneEvent gImiruFuchinEvents2[];
extern const struct SceneEvent gImiruFuchinEvents3[];
extern const struct SceneEvent gImiruFuchinEvents4[];
extern const struct SceneEvent gImiruFuchinEvents5[];
extern const struct SceneEvent gImiruFuchinEvents6[];
extern const struct SceneEvent gImiruFuchinEvents7[];
extern const struct SceneEvent gImiruFuchinEventsOther[];

/* Raised by the entry setup in the scenes that track the leader. It follows
 * the overlay's image. */
s32 ImiruFuchin_TrackLeader;

void ImiruFuchin_ApplyEntrySetup(void);
void FieldScene_RunScene39aSequenceA(void);
extern s32 ImiruFuchin_TrackLeader;
void ImiruFuchin_ApplyRoomLayout(void);
void ImiruFuchin_PlaceDragonsEye(void);
struct Actor_39a *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 sprite);
void BattleFx_StartFadeOverlay(s32 mode);
void DialogueLayout_ConfigureGroupOne(void);
void DialogueLayout_ConfigureGroupTwo(void);
void DialogueLayout_ConfigureGroupThree(void);
void FieldScene_RunFlagBranchedLayoutSteps(void);
void OverlayObject_AdvancePositionByDelta();

/* The overlay's three effect scripts, at the start of its read-only data. */
extern const s32 *const gEffectScripts[];

struct ScriptTable {
    const s32 *script[3];
};

extern u32 gFrameCount;
void Engine_AudioPlayCue();
void Effect_Spawn();

struct DustParams {
    s32 count;
    s32 kind;
    s32 spreadX;
    s32 spreadY;
    s32 growX;
    s32 growY;
};

/* The work in slot 56, whose byte at +52 marks a fade under way. */
struct FadeWork {
    u8 unknown_00[52];
    u8 active;
};

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */
void SceneState_ApplyValues8And2And1(void)
{
    BattleFx_RunPageEffectForSlot(8, 2, 1);
}

void SceneState_ApplyValues11And62(void)
{
    BattleFx_SetPhaseRequest(0xB, 0x3E);
}

/* Where the party appears in each of the seven areas around Imil. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ImiruFuchin1) {
        return gImiruFuchinEntrances1;
    }
    if (v == (s32)&SceneId_ImiruFuchin2) {
        return gImiruFuchinEntrances2;
    }
    if (v == (s32)&SceneId_ImiruFuchin3) {
        return gImiruFuchinEntrances3;
    }
    if (v == (s32)&SceneId_ImiruFuchin4) {
        return gImiruFuchinEntrances4;
    }
    if (v == (s32)&SceneId_ImiruFuchin5) {
        return gImiruFuchinEntrances5;
    }
    if (v == (s32)&SceneId_ImiruFuchin6) {
        return gImiruFuchinEntrances6;
    }
    if (v == (s32)&SceneId_ImiruFuchin7) {
        return gImiruFuchinEntrances7;
    }
    return gImiruFuchinEntrancesOther;
}

/* Two of the scene hooks the entry veneers export: no follow-up, and the
 * cave's exits. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 *ImiruFuchin_GetExits(void)
{
    return gImiruFuchinExits;
}

/* The actors placed in each area; the sixth area places the table the other scenes take. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ImiruFuchin1) {
        return gImiruFuchinPlacements1;
    }
    if (v == (s32)&SceneId_ImiruFuchin2) {
        return gImiruFuchinPlacements2;
    }
    if (v == (s32)&SceneId_ImiruFuchin3) {
        return gImiruFuchinPlacements3;
    }
    if (v == (s32)&SceneId_ImiruFuchin4) {
        return gImiruFuchinPlacements4;
    }
    if (v == (s32)&SceneId_ImiruFuchin5) {
        return gImiruFuchinPlacements5;
    }
    if (v == (s32)&SceneId_ImiruFuchin7) {
        return gImiruFuchinPlacements7;
    }
    return gImiruFuchinPlacementsOther;
}

/* The cave's flag steps: each places or moves actors 8 to 11. */
void SceneActor_PlacePairAtOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Object_GetById(gGameState.selected_actor);
    q = Object_GetById(a0);
    Engine_EventBegin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Object_SetPosition(p, x, p->f0c, y);
    }
    Object_SetMode(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Object_SetPosition(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetMode(q, 4);
    } else {
        Object_SetMode(q, 3);
    }
    Object_CommitPosition(p);
    Engine_EventEnd();
}

void ActorPresentation_SetupActorEightForFlag301(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0x70, 0);
    SceneActor_PlacePairAtOffset(8, 0x70, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x301);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupOne();
}

void SceneState_RunSlot8OffsetStep(void)
{
    s32 offset = 112;

    offset = -offset;
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, offset, 0);
    SceneActor_PlacePairAtOffset(8, offset, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x301);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupOne();
}

/* Imports; the queried ones are typed for their return value. */
void FieldScene_RunActor9Transition302(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x302);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupOne();
}

void ActorPresentation_SetupActorNineForFlag302(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x302);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupOne();
}

void ActorPresentation_SetupActorTenForFlag303(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x303);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupOne();
}

void FieldScene_RunActor10Transition303(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x303);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupOne();
}

void FieldScene_RunActor8Transition304(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0x90, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x304);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8Transition304And305(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, -144, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x304);
    GameFlag_Set(0x305);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8Transition305(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, -14, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x305);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8FlaggedSequence(void)
{
    Audio_PlayCue((s32) 0xF1);

    if (GameFlag_IsSet((s32) 0x306) != 0) {
        SceneActor_PlacePairAtOffset(8, 16, 0);
        GameFlag_Clear((s32) 0x305);
    } else {
        SceneActor_PlacePairAtOffset(8, 144, 0);
        /* movs r0,#0xc1 / lsls r0,#2 builds 0x304. */
        GameFlag_Set((s32) 0x304);
    }

    Audio_PlayCue((s32) 0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void ActorPresentation_SetupActorNineForFlag306(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x306);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor9Flag306Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x306);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void ActorPresentation_SetupActorTenForFlag307(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(10, 0, 144);
    SceneActor_PlacePairAtOffset(10, 0, 128);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x307);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

/* Three sites of one import, so three names. */
void FieldScene_RunActorTenDepthSequence(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    /* r5 = -96 (movs #0x60 / negs), live across the first import call. */
    s32 depth = -96;

    Audio_PlayCue((s32) 0xF1);
    SceneActor_PlacePairAtOffset(10, 0, depth);
    SceneActor_PlacePairAtOffset(10, 0, depth);
    SceneActor_PlacePairAtOffset(10, 0, -80);
    Audio_PlayCue((s32) 0x121);
    GameFlag_Set((s32) 0x307);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_PlaceActorEightByFlags(void)
{
    Audio_PlayCue((s32) 0xF1);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. The second test is only reached
     * when the first fails, and both truths take the same path. */
    if (GameFlag_IsSet((s32) 0x310) != 0 || GameFlag_IsSet((s32) 0x30D) != 0) {
        SceneActor_PlacePairAtOffset(8, -48, 0);
        GameFlag_Clear((s32) 0x308);
        GameFlag_Set((s32) 0x309);
    } else {
        SceneActor_PlacePairAtOffset(8, -96, 0);
        GameFlag_Set((s32) 0x308);
        GameFlag_Clear((s32) 0x309);
    }

    Audio_PlayCue((s32) 0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor8Transition308And309(void)
{
    Audio_PlayCue(0xF1);
    GameFlag_Clear(0x308);
    GameFlag_Clear(0x309);
    SceneActor_PlacePairAtOffset(8, 0x30, 0);
    Audio_PlayCue(0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorEightForFlags308And309Guarded(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (GameFlag_IsSet((s32) 0x310) != 0) {
        return;
    }
    if (GameFlag_IsSet((s32) 0x30D) != 0) {
        return;
    }

    Audio_PlayCue((s32) 0xF1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    GameFlag_Set((s32) 0x308);
    GameFlag_Clear((s32) 0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    SceneActor_PlacePairAtOffset(8, -48, 0);
    Audio_PlayCue((s32) 0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorEightForFlags308And309(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    GameFlag_Clear(0x308);
    GameFlag_Clear(0x309);
    SceneActor_PlacePairAtOffset(8, 0x60, 0);
    Audio_PlayCue(0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorNineForFlag30a(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, -32, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30A);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor9Transition30A(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0x20, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x30A);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_PlaceActorTenByFlags(void)
{
    Audio_PlayCue(241);
    if (GameFlag_IsSet(0x308) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, -64);
        GameFlag_Clear(0x30b);
        GameFlag_Set(0x30c);
        GameFlag_Clear(0x30d);
        GameFlag_Clear(0x30e);
    } else {
        SceneActor_PlacePairAtOffset(10, 0, -128);
        GameFlag_Set(0x30b);
        GameFlag_Clear(0x30c);
        GameFlag_Clear(0x30d);
        GameFlag_Clear(0x30e);
    }
    Audio_PlayCue(0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  The tail call shared by the first
 * three arms is written out in each arm rather than adding control flow the
 * reference does not have.  Imports are named by the address their call site
 * computes, and are old-style because arity varies between sites.
 */
void FieldScene_RunFlag308DialogueBranch(void)
{
    Audio_PlayCue((s32)0xf1);
    /* 0x308 is built by shifting. */
    if (GameFlag_IsSet((s32)0x308) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 16);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Set((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (GameFlag_IsSet((s32)0x310) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 16);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Set((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    } else if (GameFlag_IsSet((s32)0x311) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 64);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Clear((s32)0x30c);
        GameFlag_Set((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    } else {
        SceneActor_PlacePairAtOffset(10, 0, 128);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Clear((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    }
    Audio_PlayCue((s32)0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Transition30BTo30E(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 0, -16);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30b);
    GameFlag_Clear(0x30c);
    GameFlag_Clear(0x30d);
    GameFlag_Clear(0x30e);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  Engine_GameFlagClear serves both a query
 * and a setter, so its result is dropped at the setter site.  The tail call
 * shared by the first two arms is written out in each arm rather than adding
 * a flag the reference does not have.  Imports are named by the address their
 * call site computes, and are old-style because arity varies between sites.
 */
void FieldScene_RunFlag311DialogueBranch(void)
{
    Audio_PlayCue((s32)0xf1);
    if (GameFlag_IsSet((s32)0x311) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 48);
        GameFlag_Clear((s32)0x30b);
        /* 0x30c is built by shifting; the result is unused. */
        GameFlag_Clear((s32)0x30c);
        GameFlag_Set((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (GameFlag_IsSet((s32)0x310) != 0) {
        SceneActor_PlacePairAtOffset(10, 0, 32);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Set((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    } else {
        SceneActor_PlacePairAtOffset(10, 0, 112);
        GameFlag_Clear((s32)0x30b);
        GameFlag_Clear((s32)0x30c);
        GameFlag_Clear((s32)0x30d);
        GameFlag_Clear((s32)0x30e);
    }
    Audio_PlayCue((s32)0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorTenForFlags30bAnd30d(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30B);
    GameFlag_Clear(0x30D);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Flags30bTo30eSequenceA(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 0, 64);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x30b);
    GameFlag_Clear(0x30c);
    GameFlag_Clear(0x30d);
    GameFlag_Clear(0x30e);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Flags30bTo30eSequenceB(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 0, -80);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30b);
    GameFlag_Clear(0x30c);
    GameFlag_Clear(0x30d);
    GameFlag_Clear(0x30e);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor10Flags30bTo30eSequenceC(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x30B);
    GameFlag_Clear(0x30C);
    GameFlag_Clear(0x30D);
    GameFlag_Clear(0x30E);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

/*
 * Field scene step for overlay resource_39a.  Imports are named by the address
 * their call site computes, not by a location in this image, and their
 * interfaces are left open.  Engine_GameFlagClear is reached both as a setter and as
 * a query, so its result is dropped at the setter site.  The first two arms
 * share one tail call, which is why Engine_GameFlagClear is spelled out in each arm.
 */
void FieldScene_RunActorElevenFlaggedSteps(void)
{
    Audio_PlayCue((s32)0xf1);
    /* 0x308 is built by shifting a small immediate, not loaded whole. */
    if (GameFlag_IsSet((s32)0x308) != 0 || GameFlag_IsSet((s32)0x30d) != 0) {
        SceneActor_PlacePairAtOffset(11, 0, -64);
        GameFlag_Clear((s32)0x30f);
        GameFlag_Clear((s32)0x310);
        GameFlag_Set((s32)0x311);
        GameFlag_Clear((s32)0x312);
        /* 0x30c is built by shifting a small immediate, not loaded whole. */
    } else if (GameFlag_IsSet((s32)0x30c) != 0) {
        SceneActor_PlacePairAtOffset(11, 0, -112);
        GameFlag_Clear((s32)0x30f);
        GameFlag_Set((s32)0x310);
        GameFlag_Clear((s32)0x311);
        GameFlag_Clear((s32)0x312);
    } else {
        SceneActor_PlacePairAtOffset(11, 0, -128);
        GameFlag_Set((s32)0x30f);
        GameFlag_Clear((s32)0x310);
        GameFlag_Clear((s32)0x311);
        GameFlag_Clear((s32)0x312);
    }
    Audio_PlayCue((s32)0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunSteps30FTo312(void)
{
    GameFlag_Clear(0x30F);
    GameFlag_Clear(0x310);
    GameFlag_Clear(0x311);
    GameFlag_Clear(0x312);
}

void FieldScene_RunActor11Offset128Sequence(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 128);
    FieldScene_RunSteps30FTo312();
    Audio_PlayCue(0x121);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor11Flags30fTo312Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, -16);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30F);
    GameFlag_Clear(0x310);
    GameFlag_Clear(0x311);
    GameFlag_Clear(0x312);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_112(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 112);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_64(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, 0x40);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_80(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, 0x50);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor11Transition(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 48);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Engine_TaskWait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorEightForFlag313(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0, 0x70);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x313);
    Engine_TaskWait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor8Transition313(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0, -112);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x313);
    Engine_TaskWait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor9Flag314Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, -128, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x314);
    Engine_TaskWait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_SetupActorNineForFlag314(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0x80, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x314);
    Engine_TaskWait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_SetupActorTenForFlag315(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 160, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x315);
    Engine_TaskWait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor10Flag315Sequence(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, -160, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x315);
    Engine_TaskWait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_AdvanceActorEightStates(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(8, 2);
}

void FieldScene_SetActor9Values1And2(void)
{
    Engine_ActorSetAnimation(9, 1);
    Engine_ActorSetAnimation(9, 2);
}

void ActorPresentation_AdvanceActorTenStates(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Engine_ActorSetAnimation(10, 1);
    Engine_ActorSetAnimation(10, 2);
}

void SceneActor_SetActor11Values1And2(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Engine_ActorSetAnimation(11, 1);
    Engine_ActorSetAnimation(11, 2);
}

struct Actor_39a *OverlayObject_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
    struct Actor_39a *actor = Engine_ObjectCreate(d, a, b, c);

    if (actor != 0) {
        actor->f80->mode = 1;
        actor->f85 = 0;
        Engine_ActorSetSpriteFlags(actor, 0);
        ObjectGroup_SetChildValue(actor, 15);
        actor->f35 |= 2;
        return actor;
    }
    return 0;
}

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ImiruFuchin1) {
        return gImiruFuchinEvents1;
    }
    if (v == (s32)&SceneId_ImiruFuchin2) {
        return gImiruFuchinEvents2;
    }
    if (v == (s32)&SceneId_ImiruFuchin3) {
        return gImiruFuchinEvents3;
    }
    if (v == (s32)&SceneId_ImiruFuchin4) {
        return gImiruFuchinEvents4;
    }
    if (v == (s32)&SceneId_ImiruFuchin5) {
        return gImiruFuchinEvents5;
    }
    if (v == (s32)&SceneId_ImiruFuchin6) {
        return gImiruFuchinEvents6;
    }
    if (v == (s32)&SceneId_ImiruFuchin7) {
        return gImiruFuchinEvents7;
    }
    return gImiruFuchinEventsOther;
}

/* The service step and the cave's dialogue layouts. */
void SceneState_SetServiceZeroValue06(void)
{
    struct SceneService *work;

    Engine_EventBegin();
    work = Object_GetById(0);
    work->value06 = 0x4000;
    Audio_PlayCue(123);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(1);
}

void FieldScene_RunSingleStep(void)
{
    SceneActor_StepSubjectAlongHeading();
}

void SceneActor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Object_GetById(id);

    if (rec != 0) {
        Engine_ActorSetSpritePriority(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* Imports; the queried ones are typed for their return value. */
void DialogueLayout_ConfigureGroupOne(void)
{
    { s32 f1 = 8; s32 g1 = 29; Map_CopyCellAttributes(8, 42, 15, 5,  f1, g1); }

    if (GameFlag_IsSet((s32)0x301) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Map_CopyCellAttributes(9, 30, 1, 3,  f2, g2); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Map_CopyCellAttributes(9, 30, 1, 3,  f3, g3); }
    }

    if (GameFlag_IsSet((s32)0x302) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Map_CopyCellAttributes(14, 33, 3, 1,  f4, g4); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Map_CopyCellAttributes(14, 29, 3, 1,  f5, g5); }
    }

    if (GameFlag_IsSet((s32)0x303) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Map_CopyCellAttributes(14, 33, 3, 1,  f6, g6); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Map_CopyCellAttributes(14, 29, 3, 1,  f7, g7); }
    }
}

void DialogueLayout_ConfigureGroupTwo(void)
{
    { s32 f1 = 12; s32 g1 = 8; Map_CopyCellAttributes(0, 28, 10, 18,  f1, g1); }

    if (GameFlag_IsSet((s32)0x304) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 21, 20);
        { s32 f2 = 13; s32 g2 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f2, g2); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 13, 20);
        { s32 f3 = 21; s32 g3 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f3, g3); }
    }

    if (GameFlag_IsSet((s32)0x305) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 12, 20);
        { s32 f4 = 12; s32 g4 = 19; Map_CopyCellAttributes(5, 19, 1, 3,  f4, g4); }
        { s32 f5 = 13; s32 g5 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f5, g5); }
        if (GameFlag_IsSet((s32)0x304) != 0) {
            SceneActor_PlaceAtTileAndMark(8, 21, 20);
            { s32 f6 = 13; s32 g6 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f6, g6); }
            { s32 f7 = 12; s32 g7 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f7, g7); }
        }
    }

    if (GameFlag_IsSet((s32)0x306) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 15, 21);
        { s32 f8 = 14; s32 g8 = 17; Map_CopyCellAttributes(14, 18, 3, 1,  f8, g8); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 15, 17);
        { s32 f9 = 14; s32 g9 = 21; Map_CopyCellAttributes(14, 18, 3, 1,  f9, g9); }
    }

    if (GameFlag_IsSet((s32)0x307) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 19, 8);
        { s32 f10 = 18; s32 g10 = 25; Map_CopyCellAttributes(14, 18, 3, 1,  f10, g10); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 19, 25);
        { s32 f11 = 18; s32 g11 = 8; Map_CopyCellAttributes(14, 18, 3, 1,  f11, g11); }
    }
}

void DialogueLayout_ConfigureGroupThree(void)
{
    { s32 k5 = 12, k6 = 21; Map_CopyCellAttributes(12, 3, 9, 16, k5, k6); }

    if (GameFlag_IsSet((s32)0x308) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Map_CopyCellAttributes(16, 24, 1, 3, k5, k6); }
    } else if (GameFlag_IsSet((s32)0x309) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 17, 25);
        { s32 k6 = 24;
          Map_CopyCellAttributes(18, 24, 1, 3, 20, k6);
          Map_CopyCellAttributes(18, 24, 1, 3, 14, k6);
          Map_CopyCellAttributes(8, 41, 1, 3, 17, k6);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Map_CopyCellAttributes(16, 24, 1, 3, k5, k6); }
    }

    if (GameFlag_IsSet((s32)0x30a) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Map_CopyCellAttributes(14, 34, 1, 3, k5, k6); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Map_CopyCellAttributes(14, 34, 1, 3, k5, k6); }
    }

    if (GameFlag_IsSet((s32)0x30b) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 22);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(5, 41, 3, 1, k5, 22);
        }
    } else if (GameFlag_IsSet((s32)0x30c) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 23);
        { s32 k5 = 14;
          Map_CopyCellAttributes(5, 42, 3, 1, k5, 23);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(10, 44, 3, 1, k5, 21);
        }
    } else if (GameFlag_IsSet((s32)0x30d) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 26);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 22);
          Map_CopyCellAttributes(5, 43, 3, 1, k5, 26);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
        }
    } else if (GameFlag_IsSet((s32)0x30e) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 27);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 22);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(5, 44, 3, 1, k5, 27);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 15, 30);
    }

    if (GameFlag_IsSet((s32)0x30f) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 23);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 40, 3, 1, k5, 23);
        }
    } else if (GameFlag_IsSet((s32)0x310) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 24);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 41, 3, 1, k5, 24);
        }
    } else if (GameFlag_IsSet((s32)0x311) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 27);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 42, 3, 1, k5, 27);
        }
    } else if (GameFlag_IsSet((s32)0x312) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 28);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 43, 3, 1, k5, 28);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(11, 15, 31);
    }
}

/*
 * Four flag-branched layout steps.  Nothing is returned; the three pool words
 * after the return belong to the owner.  The eight bytes of frame are the
 * fifth and sixth arguments of the six-argument layout calls.  Imports are
 * named by the address their call site computes, and are old-style because
 * arity varies between sites.
 */
void FieldScene_RunFlagBranchedLayoutSteps(void)
{

    /*
     * The byte offset 450 is built by shifting, giving entry 225.  The test
     * is (entry - 1) << 16 against 0x10000 with an unsigned compare, which
     * selects exactly entries 1 and 2.
     */
    if ((u32)((u32)((u16)gGameState.entrance - 1) << 16) <= (u32)0x10000) {
        { s32 f1 = 14; s32 g1 = 10; Map_CopyCellAttributes(22, 20, 9, 8,  f1, g1); }
    } else {
        { s32 f2 = 7; s32 g2 = 45; Map_CopyCellAttributes(20, 45, 11, 4,  f2, g2); }
    }

    if (GameFlag_IsSet((s32)0x313) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 20, 17);
        { s32 f3 = 19; s32 g3 = 10; Map_CopyCellAttributes(19, 11, 3, 1,  f3, g3); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 20, 10);
        { s32 f4 = 19; s32 g4 = 17; Map_CopyCellAttributes(19, 11, 3, 1,  f4, g4); }
    }

    /* 0x314 is built by shifting. */
    if (GameFlag_IsSet((s32)0x314) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 14, 16);
        { s32 f5 = 22; s32 g5 = 15; Map_CopyCellAttributes(16, 15, 1, 3,  f5, g5); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 22, 16);
        { s32 f6 = 14; s32 g6 = 15; Map_CopyCellAttributes(16, 15, 1, 3,  f6, g6); }
    }

    if (GameFlag_IsSet((s32)0x315) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 17, 46);
        { s32 f7 = 7; s32 g7 = 45; Map_CopyCellAttributes(15, 15, 1, 3,  f7, g7); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 7, 46);
        { s32 f8 = 17; s32 g8 = 45; Map_CopyCellAttributes(15, 15, 1, 3,  f8, g8); }
    }
}

/* Two map triggers of the cave: while the entry setup has raised the cave's
 * tracking flag, one hands the leader to the tracking work, the other takes
 * it away. */
void ImiruFuchin_StartTrackingLeader(void)
{
    if (ImiruFuchin_TrackLeader != 0) {
        struct TrackingWork *work = *(gWorkSlot + 36);

        work->actor = Object_GetById(ACTOR_PARTY_LEADER);
    }
}

void ImiruFuchin_StopTrackingLeader(void)
{
    if (ImiruFuchin_TrackLeader != 0) {
        struct TrackingWork *work = *(gWorkSlot + 36);

        work->actor = NULL;
    }
}

/* The scene start around Imil: open with the window transition; the first
   area runs its opening sequence until flag 0x109 is set, and otherwise the
   areas are set up for the entrance. */
s32 ImiruFuchin_ApplyEntryHook(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0x109) == 0 && gGameState.scene == (s32)&SceneId_ImiruFuchin1) {
        GameFlag_Set(0x144);
        FieldScene_RunScene39aSequenceA();
    } else {
        ImiruFuchin_ApplyEntrySetup();
    }
    return 0;
}

/* The leader's arrival when the cave first opens. */
void FieldScene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    record = Object_GetById(8);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x108, 196);
    Engine_EventEnd();
}

void ImiruFuchin_ApplyEntrySetup(void)
{
    u8 *actor;
    s32 value;

    ImiruFuchin_ApplyRoomLayout();
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin4) {
        if (!GameFlag_IsSet(0xf13) && gGameState.entrance == 1) {
            ImiruFuchin_PlaceDragonsEye();
        }
        if ((u16)(gGameState.entrance - 2) <= 3) {
            OverlayObject_CreateAndInitialize(0x9c0000, 0, 0x1c40000, 223);
            OverlayObject_CreateAndInitialize(0xbc0000, 0, 0x1c40000, 223);
        }
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin7) {
        /* FAKEMATCH: one zero clears the flag and both of actor 8's words,
         * and the variable is reused for the tracking work below, so the zero
         * and then the work share one register. */
        value = 0;
        actor = (u8 *)Actor_Get(8);
        ImiruFuchin_TrackLeader = value;
        actor[85] = value;
        *(s32 *)(actor + 12) = value;
        Engine_ActorSetSpritePriority(8, 1);
        Actor_SetChildValue(8, 15);
        switch (gGameState.entrance) {
        case 1:
        case 2:
            BattleFx_StartFadeOverlay(0);
            ImiruFuchin_TrackLeader = 1;
            break;
        case 5:
            BattleFx_StartFadeOverlay(0);
            ImiruFuchin_TrackLeader = 1;
            value = *(s32 *)(gWorkSlot + 36);
            ((struct TrackingWork *)value)->actor = NULL;
            break;
        }
        if (gGameState.entrance <= 6) {
            if (GameFlag_IsSet(0x820)) {
                Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
                Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
            } else {
                gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
                ColorBuffer_ApplySource(0x203108, 1);
                ColorBuffer_ApplyTarget(0x203108, 1);
                Engine_ColorBufferInterpolate(1);
                Engine_TaskWait(1);
            }
        }
    }
}

/* Copy the room's cell attributes for the way it is entered, then run the
 * room's layout step. */
void ImiruFuchin_ApplyRoomLayout(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin2) {
        Map_CopyCellAttributes(8, 29, 15, 5, 8, 42);
        DialogueLayout_ConfigureGroupOne();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
        Map_CopyCellAttributes(12, 8, 10, 18, 0, 28);
        DialogueLayout_ConfigureGroupTwo();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && gGameState.entrance != 1) {
        Map_CopyCellAttributes(12, 21, 9, 16, 12, 3);
        DialogueLayout_ConfigureGroupThree();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin5) {
        if (gGameState.entrance == 1 || gGameState.entrance == 2) {
            Map_CopyCellAttributes(14, 10, 9, 8, 22, 20);
        } else {
            Map_CopyCellAttributes(7, 45, 11, 4, 20, 45);
        }
        FieldScene_RunFlagBranchedLayoutSteps();
    }
}

/* Hop the leader across the gap the touched trigger stands for. */
void ImiruFuchin_HopOnTrigger(void)
{
    s32 trigger = gEventWork->touched_trigger;

    if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
        if (trigger == 17) {
            ImiruFuchin_HopBy(0, -32);
        } else {
            ImiruFuchin_HopBy(-32, 0);
        }
    }
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && trigger == 25 && GameFlag_IsSet(0x309)) {
        ImiruFuchin_HopBy(0, 32);
    }
}

/* The leader's hops and the Dragon's Eye: placing it and taking it. */
void FieldScene_ApplyOffset0Neg32(void)
{
    ImiruFuchin_HopBy(0, -32);
}

void SceneState_ApplyOffsetMinus32(void)
{
    ImiruFuchin_HopBy(-32, 0);
}

void ImiruFuchin_HopBy(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    Engine_ActorSetDestinationOffset(0, a0, a1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 7);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
    Engine_EventEnd();
}

void ImiruFuchin_PlaceDragonsEye(void)
{

    u8 *rec;
    s32 rec7;
    s32 record;
    u8 *p6;

    record = 0;
    rec = Value4(Engine_ObjectCreate, 22, 0xf80000, 0x80000, 0x980000);
    if ((s32)rec != 0) {
        p6 = *(u8 **)(rec + 80);
        p6[38] = record;
        p6[39] = record;
        *((s8 *)p6 + 5) &= -33;
        p6[9] &= 15;
        rec[85] = record;
        rec[92] = 1;
        rec7 = Engine_HeapAllocate(17, 0x608);
        Engine_ItemLoadIcon(ITEM_DRAGONS_EYE);
        Engine_VramLoad(p6[28], 128, (rec7 + 0x400));
        Engine_HeapRelease(17);
        *(s32 *)gImiruFuchinDragonsEye = (s32)rec;
    }
}

/* Returns a value: the reference sets r1 before r0 at this site. */
void ImiruFuchin_TakeDragonsEye(void)
{

    Engine_EventBegin();

    /* r5 holds &gImiruFuchinDragonsEye across the calls; the word is reloaded before
     * the second test. */
    if (gImiruFuchinDragonsEye[0] != 0) {
        Engine_RunRisingObjectSequence(gImiruFuchinDragonsEye[0], 3);
    }

    Engine_PartyGiveItem((s32) 0xE6, 0);
    GameFlag_Set((s32) 0xF13);

    if (gImiruFuchinDragonsEye[0] != 0) {
        Engine_ObjectDispatchRelease(gImiruFuchinDragonsEye[0]);
    }

    Engine_EventEnd();
}

void OverlayObject_AdvancePositionByDelta(struct MovingObject *object)
{
    object->x += object->dx;
    object->y += object->dy;
    object->z += object->dz;
    object->sub_x += object->sub_dx;
    object->sub_y += object->sub_dy;
}

s32 OverlayObject_ApplyValue15(s32 obj)
{
    ObjectGroup_SetChildValue(obj, 15);
    return 0;
}

void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
                  const struct EffectOptions *extra)
{
    /* Spawn a scripted effect with optional palette, priority and scale rates.
     * Complete 352-byte owner, including its three-word pool, matches exactly.
     * FAKEMATCH: retain the local script-table copy and branch-local divide
     * tails so the compiler reloads the script and prepares both call arguments
     * in the observed lifetime. Shared FIELD_EFFECT types recover the remaining
     * object, sprite and options layout without private byte-offset casts. */
    struct ScriptTable table;
    struct FieldEffect *obj;
    struct FieldSprite *spr;
    const s32 *script;

    table = *(struct ScriptTable *)gEffectScripts;
    obj = (struct FieldEffect *)Engine_ObjectCreate(222, x, y, z);
    if (obj == 0)
        return;
    spr = obj->sprite;
    Object_SetMode((struct FieldActor *)obj, (flags + 1) & EFFECT_SCRIPT_MASK);
    Engine_ObjectSetScript((struct FieldActor *)obj, table.script[flags & EFFECT_SCRIPT_MASK]);
    obj->motion_flags = 0;
    spr->flags = 0;
    obj->update = OverlayObject_AdvancePositionByDelta;
    obj->velocity_x = velocity_x;
    obj->velocity_y = velocity_y;
    obj->velocity_z = velocity_z;
    obj->scale_rate_x = 0;
    obj->scale_rate_y = 0;
    spr->priority = 1;
    if ((flags & 0xffff0000) == 0 || extra == 0)
        return;
    if (flags & EFFECT_USE_PALETTE)
        ObjectGroup_SetChildValue((struct FieldActor *)obj, extra->palette);
    if (flags & EFFECT_USE_PRIORITY) {
        obj->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
        spr->priority = extra->priority;
    }
    if (flags & EFFECT_USE_START_SCALE) {
        obj->scale_x = extra->start_scale_x;
        obj->scale_y = extra->start_scale_y;
    }
    if (flags & EFFECT_SCALE_TO_TARGET) {
        script = table.script[flags & EFFECT_SCRIPT_MASK];
        if (flags & EFFECT_USE_START_SCALE) {
            obj->scale_rate_x = (extra->target_scale_x - obj->scale_x) / script[3];
            obj->scale_rate_y = (extra->target_scale_y - obj->scale_y) / script[3];
        } else {
            obj->scale_rate_x = (extra->target_scale_x - 0x10000) / script[3];
            obj->scale_rate_y = (extra->target_scale_y - 0x10000) / script[3];
        }
    }
}

/* Every fourth frame, blow a puff of dust across the cave mouth. */
void ImiruFuchin_BlowCaveMouthDust(void)
{
    struct DustParams params;
    struct DustParams *p;
    s32 phase;
    s32 dx;
    s32 dy;

    /* FAKEMATCH: retain both volatile frame-count loads in this callback. */
    phase = *(volatile s32 *)&gFrameCount & 3;
    if (phase != 0)
        return;
    p = &params;
    p->kind = 10;
    p->spreadX = 0x8000;
    p->spreadY = 0x8000;
    p->growX = 0x1cccc;
    p->growY = 0x1cccc;
    if ((*(volatile s32 *)&gFrameCount & 7) == 0)
        Engine_AudioPlayCue(136);
    dx = -0x10000 - ((((u32)Engine_RandomNext() << 1) >> 16) << 16);
    dy = -(s32)((((u32)Engine_RandomNext() * 3) >> 16) * 0x3333);
    Effect_Spawn(0x1340000, 0x400000, 0xde0000, dx, dy, phase, 0xd0001, p);
}

/*
 * The cave-mouth wind: four passes of the dust blower, scheduled as a
 * callback while the map opens.
 */
void FieldScene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Audio_PlayCue(19);
    Audio_PlayCue(182);
    Engine_EventBegin();
    Battle_ResetEffectCounter();

    /* FAKEMATCH: 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        ColorBuffer_ApplyTarget((s32)0x204318, 1);
        Engine_ColorBufferInterpolate(1);
        Engine_TaskWait(2);
        if (pass == 0) {
            Map_CopyCellsTo(30, 8, 12, 8, step, span);
            Map_CopyCellsTo(30, 57, 19, 57, one, one);
        }
        ColorBuffer_ApplyTarget((s32)0x203108, 1);
        Engine_ColorBufferInterpolate(1);
        Engine_TaskWait(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Engine_TaskWait(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Engine_TaskAddCallback((void *)ImiruFuchin_BlowCaveMouthDust, (s32)0xc80);
    Engine_TaskWait(40);
    ColorBuffer_ApplyTarget((s32)0x201090, 1);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(80);
    Engine_TaskRemoveCallback((void *)ImiruFuchin_BlowCaveMouthDust);
    Engine_TaskWait(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    ColorBuffer_ApplyTarget((s32)0x10000, 1);
    Engine_ColorBufferInterpolate(80);
    /* Same import as in the loop, one argument here. */
    Engine_TaskWait(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    GameFlag_Set((s32)0x820);
    PartyInventory_Discard(230);
    Audio_PlayCueFromEventWork();
    /* Same import as the first call, no argument register written here. */
    Engine_EventEnd();
}

void SceneState_SetValue17e1(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFuchinDragonFlameShowsPath, 1);
    Engine_EventEnd();
}

void SceneDialogue_RunLine17e2(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFuchinSecretKiRevealed, 1);
    Engine_EventEnd();
}

void FieldScene_RunScriptedStep17E3(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFuchinLightRevealsShadows, 1);
    Engine_EventEnd();
}

/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void SceneState_SetWorkspace370ByFlag820(void)
{

    Engine_EventBegin();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (GameFlag_IsSet((s32)0x820) != 0) {
        Engine_MessageShowCentered((s32)MsgFuchinDragonRedEyes, 1);
    } else {
        Engine_MessageShowCentered((s32)MsgFuchinEyelessDragon, 1);
        if (PartyInventory_FindOwner((s32)0xe6) != -1) {
            u8 *workspace = (u8 *)gEventWork;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            /*
             * FAKEMATCH: the store goes through a pointer local and an s32 value local,
             * in that order. Storing the literal directly builds the constant
             * in HImode and loads it from the literal pool, costing a pool
             * word; splitting the address out first also fixes which register
             * holds it.
             */
            {
                u16 *slot = (u16 *)(workspace + 370);
                s32 one = 1;

                *slot = (u16)one;
            }
        }
    }
    Engine_EventEnd();
}

/* The cave's fade-in, installed in its event table. */

/* While the stage is early enough, start the fade-in: raise the fade flag,
 * set the three light flags of the work in slot 31 and interpolate the
 * palette over 16 frames. */
void ImiruFuchin_StartFadeIn(void)
{
    struct FadeWork *fade;
    u8 *work;

    if (gGameState.entrance <= 6) {
        fade = *(gWorkSlot + 56);
        work = *(gWorkSlot + 31);
        fade->active = 1;
        work[0x53e] = 0;
        work[0x53c] = 1;
        work[0x53d] = 1;
        ColorBuffer_ApplySource(0, 1);
        ColorBuffer_ApplyTarget(0x203108, 1);
        Engine_ColorBufferInterpolate(16);
        Engine_TaskWait(16);
    }
}

/* Turning and stepping an actor along the heading the held direction gives. */
void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    /* FAKEMATCH: retain the argument reused as the timer index, zero and -1
     * so the signed halfword view and its values keep their lifetime. */
    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = gImiruFuchinKeyHeadings[(*(u32 *)gKeysHeld >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Object_SetMode(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetMode(o, 2);
    ObjectDispatch_ApplyValueToChildren(o, 0x30);
}

/*
 * Pathing step for resource_39a.  r0 holds the popped return address, so
 * nothing is returned, and the seven pool words after the return belong to
 * the owner.  Frame: sp+0 is the goal marker, sp+4 the heading, and
 * sp+8..sp+19 the three-word probe position handed to the stepping imports by
 * address.  The x and z assignment order and the inline stepping wrapper are
 * what reproduce the reference; do not reorder or respell them.
 */
void SceneActor_StepSubjectAlongHeading(void)
{

    struct PathSubject *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *subject_id;

    /* FAKEMATCH: retain the x/z assignment order and AdvanceProbe inline
     * boundary so sp+8 is rematerialized before the split 0x100000 constant
     * is completed for its second argument. */
    subject = ObjectTable_Get(gGameState.selected_actor);

    for (;;) {
        heading = gImiruFuchinHeadings[(*(u32 *)gKeysHeld >> 4) & 15];
        /*
         * The test is on heading << 16 against 0xffff0000, the signed
         * halfword -1 meaning "no heading".
         */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* No argument register is written before this branch. */
        Engine_EventBegin();

        /* The 0x80000 bias is built by shifting, not loaded as a constant. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        subject_id = (u8 *)subject;
        subject_id += 34;
        goal = GetMapCellCollision((s32)*subject_id, x, z);
        /*
         * 0x100000 is built by shifting, not loaded as a constant.  The probe
         * block is passed by address and is advanced by the callee.
         */
        Vector_AddPolarOffset((s32)0x100000, heading, probe);

        marker = GetMapCellCollision((s32)*subject_id, probe[0], probe[2]);
        if (marker == 255
                || Map_GetTerrainHeight((s32)*subject_id, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before 0x02004392. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Object_SetPosition(subject, x, subject->y, z);
        /*
         * Same call word as the marker lookup, but a two-argument command, so
         * it keeps its own declaration.
         */
        Object_SetMode(subject, 2);
        ObjectDispatch_ApplyValueToChildren(subject, 48);
        Object_CommitPosition(subject);
        subject->callback = (void *)SceneActor_TurnTowardTableAngle;

        goto advance_probe;
continue_probe:
        if (Map_GetTerrainHeight((s32)*subject_id, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finish_probe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Object_SetPosition(subject, probe[0], probe[1], probe[2]);
        Object_CommitPosition(subject);
        if (marker != goal) {
            goto blocked;
        }

advance_probe:
        AdvanceProbe(heading, probe);
        marker = GetMapCellCollision((s32)*subject_id, probe[0], probe[2]);
        if (marker != 255) {
            goto continue_probe;
        }

finish_probe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Object_SetPosition(subject, x, subject->y, z);
        Object_CommitPosition(subject);
        Engine_TaskWait(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built by shifting, not loaded as a constant. */
    subject->state_052 = 0x4000;

tail:
    Engine_TaskWait(10);
    Engine_EventEnd();
}
