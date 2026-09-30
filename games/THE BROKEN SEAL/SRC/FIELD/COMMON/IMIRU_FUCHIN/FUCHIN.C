/* The two page effects the cave's opening runs. */
#include "IMIRU_FUCHIN.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

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

void SceneActor_PlacePairAtOffset(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Engine_ActorGet(gGameState.selected_actor);
    q = Engine_ActorGet(a0);
    Event_Begin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Object_SetPosition(p, x, p->f0c, y);
    }
    Object_SetAnimation(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Object_SetPosition(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetAnimation(q, 4);
    } else {
        Object_SetAnimation(q, 3);
    }
    Object_CommitPosition(p);
    Event_End();
}

void ActorPresentation_SetupActorEightForFlag301(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0x70, 0);
    SceneActor_PlacePairAtOffset(8, 0x70, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x301);
    Task_Wait(2);
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
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

/* Imports; the queried ones are typed for their return value. */
void FieldScene_RunActor9Transition302(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x302);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void ActorPresentation_SetupActorNineForFlag302(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x302);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void ActorPresentation_SetupActorTenForFlag303(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x303);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void FieldScene_RunActor10Transition303(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xA, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x303);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupOne();
}

void FieldScene_RunActor8Transition304(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0x90, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x304);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8Transition304And305(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, -144, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x304);
    GameFlag_Set(0x305);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor8Transition305(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, -14, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x305);
    Task_Wait(2);
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
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void ActorPresentation_SetupActorNineForFlag306(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, 0x40);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x306);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupTwo();
}

void FieldScene_RunActor9Flag306Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0, -64);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x306);
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor8Transition308And309(void)
{
    Audio_PlayCue(0xF1);
    GameFlag_Clear(0x308);
    GameFlag_Clear(0x309);
    SceneActor_PlacePairAtOffset(8, 0x30, 0);
    Audio_PlayCue(0x121);
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorNineForFlag30a(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, -32, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x30A);
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor9Transition30A(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0x20, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x30A);
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
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
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_112(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 112);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_64(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, 0x40);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorElevenAt0_80(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(0xB, 0, 0x50);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void FieldScene_RunActor11Transition(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(11, 0, 48);
    Audio_PlayCue(0x121);
    FieldScene_RunSteps30FTo312();
    Task_Wait(2);
    DialogueLayout_ConfigureGroupThree();
}

void ActorPresentation_SetupActorEightForFlag313(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0, 0x70);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x313);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor8Transition313(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(8, 0, -112);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x313);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor9Flag314Sequence(void)
{
    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, -128, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x314);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_SetupActorNineForFlag314(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(0xF1);
    SceneActor_PlacePairAtOffset(9, 0x80, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x314);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_SetupActorTenForFlag315(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, 160, 0);
    Audio_PlayCue(0x121);
    GameFlag_Set(0x315);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void FieldScene_RunActor10Flag315Sequence(void)
{
    Audio_PlayCue(241);
    SceneActor_PlacePairAtOffset(10, -160, 0);
    Audio_PlayCue(0x121);
    GameFlag_Clear(0x315);
    Task_Wait(2);
    FieldScene_RunFlagBranchedLayoutSteps();
}

void ActorPresentation_AdvanceActorEightStates(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Actor_SetAnimation(8, 1);
    Actor_SetAnimation(8, 2);
}

void FieldScene_SetActor9Values1And2(void)
{
    Actor_SetAnimation(9, 1);
    Actor_SetAnimation(9, 2);
}

void ActorPresentation_AdvanceActorTenStates(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Actor_SetAnimation(10, 1);
    Actor_SetAnimation(10, 2);
}

void SceneActor_SetActor11Values1And2(void)
{
    void SceneActor_PlacePairAtOffset(s32, s32, s32);

    Actor_SetAnimation(11, 1);
    Actor_SetAnimation(11, 2);
}

struct Actor_39a *OverlayObject_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
    struct Actor_39a *actor = Engine_ObjectCreate(d, a, b, c);

    if (actor != 0) {
        actor->f80->mode = 1;
        actor->f85 = 0;
        Actor_SetSpriteFlags(actor, 0);
        Object_SetPalette(actor, 15);
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

void SceneState_SetServiceZeroValue06(void)
{
    struct SceneService *work;

    Event_Begin();
    work = Engine_ActorGet(0);
    work->value06 = 0x4000;
    Audio_PlayCue(123);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(1);
}

void FieldScene_RunSingleStep(void)
{
    SceneActor_StepSubjectAlongHeading();
}

void SceneActor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Engine_ActorGet(id);

    if (rec != 0) {
        Actor_SetSpritePriority(id, 3);
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
