/* The cave's flag steps: each places or moves actors 8 to 11. */
#include "IMIRU_FUCHIN.H"

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
