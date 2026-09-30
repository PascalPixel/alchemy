#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

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
void BattleFx_PlayQueuedSound();
extern u8 GomaIriguchi_SceneTableA[];
extern u8 GomaIriguchi_SceneTableB[];
extern u8 GomaIriguchi_SceneTableC[];
extern u8 GomaIriguchi_SceneTableD[];

/*
 * The owner exists for the argument shuffle: the frame count is saved before
 * the first call clobbers its register, so it survives to reach the second.
 * The twenty-two byte owner loads no literal and has no pool.
 */
void FieldScene_RequestAndWaitFrames(s32 selector, s32 frames)
{
    Event_ShowMessage(selector, 0);
    Event_Wait(frames);
}

/* Contiguous unnamed leaf-owner run for resource_387. */

void *SceneData_GetTable92f8(void)
{
    return GomaIriguchi_SceneTableA;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTable9358(void)
{
    return GomaIriguchi_SceneTableB;
}

void *SceneData_GetTable9368(void)
{
    return GomaIriguchi_SceneTableC;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 record;
    s32 v3;

    record = Engine_ActorGet(9);
    v3 = *(s32 *)(record + 8) / 0x100000;
    GameFlag_Clear(0x861);
    GameFlag_Clear(0x862);
    if (v3 == 15) {
        Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    } else if (v3 == 16) {
        Map_CopyCellAttributes(48, 18, 1, 2, v3, 18);
        GameFlag_Set(0x861);
    } else {
        Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
        GameFlag_Set(0x862);
    }
}

void FieldScene_RunScene387SequenceC(void)
{
    struct FieldActor *actor;
    s32 tile_x;

    actor = (struct FieldActor *)Engine_ActorGet(10);
    tile_x = actor->x.fixed / 0x100000;
    if (tile_x == 23) {
        Event_Wait(10);
        Actor_Get(10)->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_Get(10)->motion_flags = 0;
        Actor_SetSpriteFlags(Actor_Get(10), 0);
        Map_CopyCellAttributes(54, 17, 1, 1, tile_x, 17);
        GameFlag_Set(0x863);
    }
}

void FieldScene_RunScene387SequenceD(void)
{
    struct EventWork *p5;
    s32 v5;

    p5 = gEventWork;
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    Actor_SetSpeed(9, 0x3333, 0x1999);
    Audio_PlayCue(185);
    v5 = (11 - (p5->touched_trigger << 1)) << 4;
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, v5, 0);
    Actor_SetDestinationOffset(9, v5, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WaitForMove(9);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    FieldScene_RunOpeningAuxiliarySequence();
    BattleFx_PlayQueuedSound();
    Event_End();
}

void Resource387_NoOpCallbackA(void)
{
}

void Resource387_NoOpCallbackB(void)
{
}

void FieldScene_RunStepWithValue866(void)
{
    Event_Begin();
    GameFlag_Set(0x866);
    Event_End();
}

void *SceneData_GetTable9488(void)
{
    return GomaIriguchi_SceneTableD;
}
