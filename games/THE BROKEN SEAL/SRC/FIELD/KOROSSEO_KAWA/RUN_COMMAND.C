#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"

/* FAKEMATCH: calls through a cast of Object_GetById keep the unprototyped call
 * this file's code made before it shared the header's declaration. */
extern u8 MsgKorosseoAskAttendantsForExplanationsStages[];
extern u8 MsgKorosseoRobinYoureContestantInFinals[];
extern u8 MsgKorosseoSiteFirstFinalsBattle[];
extern u8 MsgKorosseoWarriorsEnterFinalsWithoutAny[];

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

typedef struct Rec {
    u8 pad00[216];
    u16 fd8[15];
} Rec;

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 HexDigits[];

typedef void(*SceneTask)(void);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
Rec *Owner_GetState(s32);

/* Contiguous unnamed leaf-owner run for resource_3ba. */

/*
 * Scene setup for resource_3ba: allocates a scene descriptor, stamps its
 * parameter block, uploads image and palette, and installs the per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */

/* The per-frame task this owner installs: in-image code, published below as
 * its entry address plus the Thumb bit. */

/* AUDITED GENERATED CALL SCRIPT for Scene_RunSceneFourCoordinator:
 * A phase-two fast path, full and revisit branches, and all 42 calls across
 * the complete scene-four coordinator. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

/* A countdown word this overlay owns at KorosseoKawa_Countdown: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

s32 *SceneActor_FindOccupantAheadOfSubject(void);

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

void FieldScene_RunCommandSequence(s32 a0)
{
    struct FieldActor *actor;
    s32 x;
    s32 z;

    actor = (struct FieldActor *)((s32 (*)())Object_GetById)();
    x = actor->x.part.pixel;
    z = actor->z.part.pixel;
    Event_Begin();
    Actor_SetSpeed(a0, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    ((void (*)())Engine_ActorSetPosition)(0, x << 16, (z << 16) - 0x300000);
    Actor_SetPosition(ACTOR_GERALD, (x << 16) - 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(ACTOR_IVAN, (x << 16) + 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(ACTOR_MIA, x << 16, (z << 16) - 0x200000);
    Actor_SetPosition(a0, x << 16, (z << 16) - 0x500000);
    actor = (struct FieldActor *)Object_GetById(0);
    actor->facing = 0xc000;
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_SetMessage((s32)MsgKorosseoSiteFirstFinalsBattle);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_StartRepeatedMotion(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(a0, ACTOR_IVAN, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(a0, 0x102, 60);
    if (Event_AskYesNo(a0, 0) == 0) {
        do {
            Event_SetMessage((s32)MsgKorosseoWarriorsEnterFinalsWithoutAny);
            Actor_SetAnimation(ACTOR_IVAN, 3);
            Event_Wait(2);
            Actor_SetAnimation(ACTOR_GERALD, 3);
            Event_Wait(2);
            Actor_SetAnimation(ACTOR_MIA, 3);
            Event_Wait(1);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Actor_SetAnimationAndWait(a0, 3);
            Event_ShowMessage(a0, 0);
            Actor_FaceDirection(a0, 0xa000, 0);
            Event_Wait(20);
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x1380000, -1, 0x680000, 1);
            Camera_WaitForMove();
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x18000, 0x3000);
            Camera_MoveTo(0x3080000, -1, 0x680000, 1);
            Event_ShowMessage(a0, 0);
            Camera_WaitForMove();
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4d80000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Actor_FaceActor(a0, 0x6000, 0);
            Event_ShowMessage(a0, 0);
            Camera_MoveTo(0x5180000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Actor_FaceActor(a0, ACTOR_PARTY_LEADER, 0);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            Actor_RunRepeatedMotion(a0, 2);
        } while (Event_AskYesNo(a0, 0) != 0);
        Actor_RunRepeatedMotion(a0, 2);
        Event_SetMessage((s32)MsgKorosseoAskAttendantsForExplanationsStages);
        Event_ShowMessage(a0, 0);
    }
    Event_SetMessage((s32)MsgKorosseoRobinYoureContestantInFinals);
    Actor_RunRepeatedMotion(a0, 2);
    Event_ShowMessage(a0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(1);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Event_Wait(2);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Event_Wait(1);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(6);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_IVAN, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_MIA, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WalkToAndWait(a0, x - 16, z - 64);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_WalkToAndWait(a0, x - 16, z - 16);
    Actor_WalkToAndWait(a0, x, z);
    Actor_FaceDirection(a0, 0xc000, 10);
    Event_End();
}
