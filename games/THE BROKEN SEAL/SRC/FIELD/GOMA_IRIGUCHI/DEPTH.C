#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
extern u8 MsgGomaNoUsePsynergy[];

/* Shared 22-byte head leaf proved identical for this overlay family. */
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

struct OverlayActorPosition {
    u8 pad00[8];
    s32 depth_fixed;
};

struct OverlayActorState {
    u8 pad00[35];
    u8 flags;
};

void GomaIriguchi_RunSpinningLeap();
void BattleFx_PlayQueuedSound();
void GomaIriguchi_SetEntranceFlag();
void GomaIriguchi_GiveShamansRod();
void Event_PrepareObjectAndApplyValue();

void FieldScene_RequestAndWaitFrames(s32 selector, s32 frames);

void FieldScene_RunScene387SequenceA(void)
{
    u32 i;
    s32 record;

    BattleFx_PlayQueuedSound();
    Event_Begin();
    Event_Wait(30);
    Event_SetMessage((s32)MsgGomaNoUsePsynergy);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x108, 168);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    if (GameFlag_IsSet(0x855) == 0) {
        Actor_SetAnimation(ACTOR_GERALD, 2);
        record = Engine_ActorGet(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
        Event_End();
    } else {
        Actor_SetPosition(ACTOR_IVAN, 0x1680000, 0xf80000);
        Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
        Actor_WalkToAndWait(ACTOR_IVAN, 0x110, 248);
        Actor_WalkToAndWait(ACTOR_IVAN, 0x110, 208);
        Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
        Event_Wait(20);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x100, 60);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Actor_WalkToAndWait(ACTOR_IVAN, 0x108, 200);
        Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 168);
        Actor_WalkToAndWait(ACTOR_IVAN, 248, 184);
        Actor_WaitForMove(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
        Actor_WalkToAndWait(ACTOR_IVAN, 232, 184);
        Event_Wait(20);
        Actor_ShowEmote(ACTOR_IVAN, 0x105, 60);
        Actor_FaceDirection(ACTOR_IVAN, 0xe000, 20);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(20);
        Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 120);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 60);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
        Event_Wait(60);
        Actor_ShowEmote(ACTOR_IVAN, 0x106, 0);
        Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
        Event_Wait(30);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 30);
        Actor_FaceDirection(ACTOR_IVAN, 0xe000, 20);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
        GomaIriguchi_SetEntranceFlag();
        Actor_SetAnimation(ACTOR_IVAN, 1);
        Event_Wait(20);
        BattleFx_PlayQueuedSound();
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x100, 60);
        Actor_Jump(ACTOR_GERALD, 2, 0);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(1, 20);
        Actor_WalkToAndWait(ACTOR_IVAN, 0x108, 184);
        Event_Wait(10);
        Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
        Event_Wait(10);
        FieldScene_RequestAndWaitFrames(1, 20);
        Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
        Event_Wait(60);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
        Event_Wait(60);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
        Event_Wait(80);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
        Event_Wait(60);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
        Event_Wait(80);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 40);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(20);
        Event_PrepareObjectAndApplyValue(2, 1);
        Event_Wait(60);
        GomaIriguchi_GiveShamansRod();
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Event_Wait(20);
        Actor_WalkToAndWait(ACTOR_IVAN, 248, 184);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
        Event_Wait(120);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
        Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(50);
        Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
        Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
        Actor_WalkTo(ACTOR_GERALD, 248, 168);
        Actor_WalkToAndWait(ACTOR_IVAN, 248, 168);
        Actor_SetPosition(ACTOR_IVAN, 0, 0);
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
        Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
        GameFlag_Set(0x865);
        Event_End();
    }
}

void Overlay387_ConfigureActorEightAtDepth(void)
{
    s32 depth;
    s32 span;
    struct OverlayActorState *state;

    Event_Begin();
    depth = ((struct OverlayActorPosition *)Engine_ActorGet(8))->depth_fixed >> 20;
    if (depth == 11) {
        GomaIriguchi_RunSpinningLeap(8);
        state = Actor_Get(8);
        state->flags |= 2;
        span = 12;
        Map_CopyCellAttributes(39, 12, 3, 1, 8, span);
        Map_CopyCellAttributes(43, 11, 3, 1, span, depth);
        GameFlag_Set(2144);
    }
    Event_End();
}
