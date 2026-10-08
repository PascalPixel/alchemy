#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MAPCOPY.H"
#include "TYPES.H"
#include "TBS_EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KORIMA_MURA.H"
#include "STAGED_ACTOR.H"
#include "STAGED_ACTOR_EFFECT.H"
#include "text/MSG_IDS.H"

struct Struct3848 {
    u8 pad00[8];
    u32 field08;
    s32 field0c;
    u32 field10;
};

struct Struct2798 {
    u8 pad00[0x18];
    s32 field18;
    u8 pad1c[0x38 - 0x1c];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Sub { u8 pad00[9]; u8 f09; u8 pad0a[28]; u8 f26; };

struct Obj {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

struct Struct288c {
    u8 pad00[8];
    s32 field08;
    u8 pad0c[4];
    s32 field10;
};

s32 KorimaHashi_TriggerPending __attribute__((section(".bss")));
s32 KorimaHashi_PartyFlag __attribute__((section(".bss")));
s32 KorimaHashi_SparkleSound __attribute__((section(".bss")));


void Engine_TaskWait(s32 frames);
void Engine_EventWait(s32 frames);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorSetAnimation(s32 actor, s32 mode);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);

static __inline__ void Actor_SetMotionSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: a direct call changes FieldScene_RunBranchingFormationPresentation from ldr r6, .L0 to ldr r7, .L0 (2674/2678 assembly lines). */
    Engine_ActorSetSpeed(actor, speed, acceleration);
}

static __inline__ void Actor_OffsetDestination(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: a direct call changes FieldScene_RunBranchingFormationPresentation from mov r0, #3 to neg r1, r1 (2674/2674 assembly lines). */
    Engine_ActorSetDestinationOffset(actor, dx, dz);
}

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

static __inline__ void DrawPlacement(
    s32 left, s32 top, s32 width, s32 height, s32 tile, s32 palette)
{
    /* FAKEMATCH: a direct call changes FieldScene_RunTile10x20Transition from mov r2, #17 to mov r3, #19 (79/79 assembly lines). */
    Engine_MapCopyCellAttributes(left, top, width, height, tile, palette);
}

s32 Object_CheckMovementCollision(struct StagedActorEffect *actor, struct StagedActorEffectRequest *request);

s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object)
{
    s32 *pending = &KorimaHashi_TriggerPending;
    if (*pending) {
        Object_SetMode(object, 2);
        *pending = 0;
    }
    return 1;
}

s32 SceneActor_UpdateRandomCounterMode(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + (((u32)Engine_RandomNext() * 100) >> 16));
    if ((s16)*counter > 1000) ObjectGroup_SetChildValue(object, 7);
    else ObjectGroup_SetChildValue(object, 10);
    if (*(s16 *)counter > 1200) { u16 z = 0; *counter = z; }
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return KorimaHashi_Entrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return KorimaHashi_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) { return KorimaHashi_Placements; }

void FieldScene_RunTile10x20Transition(void)
{
    struct StagedActorProbe res;
    Engine_EventBegin();

    if (StagedActor_FindClearPosition(&res)) {
        SceneActor_MoveAndRedraw(res);
        if (res.actor_slot == 10 && (res.position_x >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Engine_ActorSetAnimation(10, 3);
            Actor_OffsetDestination(10, -18, 6);
            Engine_EventWait(30);
            Engine_AudioPlayCue(240);
            Engine_ActorSetAnimation(10, 8);
            Object_GetById(10)->priority_flags = 2;
            zero = 0;
            DrawPlacement(0, 17, 2, 4, 19, 17);
            StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, zero);
            Engine_GameFlagSet(0x200);
            actor = (void *)Object_GetById(10);
            Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
        }
    }

    Engine_EventEnd();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    struct StagedActorEffect *actor = (void *)Object_GetById(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 ret = Object_CheckMovementCollision(actor, request);

    if (ret == 0) {
        Engine_EventBegin();
        Object_SetMode((struct FieldActor *)actor, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode((struct FieldActor *)actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
        Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, request->cell_x, request->cell_z);
        Object_SetMode((struct FieldActor *)actor, 6);
        Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 1);
        *flags = (u8)ret;
        Engine_ActorSetAnimation(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        Engine_TaskWait(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        Engine_TaskWait(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        Engine_TaskWait(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

void SceneActor_PassSubjectOffsetPosition(void)
{
    u32 buf[3];
#if !EDITION_INTERNATIONAL
    buf[0] = ((struct Struct3848 *)Object_GetById(0))->field08 + 0x1e0000;
    buf[1] = ((struct Struct3848 *)Object_GetById(0))->field0c;
    buf[2] = ((struct Struct3848 *)Object_GetById(0))->field10;
#else
    s32 off = 500;
    struct Struct3848 *p = (void *)Object_GetById(*(s32 *)((u8 *)&gGameState + off));
    u32 base = p->field08 & 0xfff00000;

    buf[0] = base + 0x80000;
    buf[1] = p->field0c;
    buf[2] = (p->field10 & 0xfff00000) + 0x80000;
    buf[0] = base + 0x280000;
#endif
    StagedActor_RunStepEffect((struct StagedActorEffectRequest *)buf);
}

const struct SceneEvent *Scene_GetEvents(void) { return KorimaHashi_Events; }


s32 Scene_Initialize(void)
{
    s32 record;
    s32 zero;

    FieldScene_RedrawActorFootprint(10);
    if (Engine_GameFlagIsSet(0x200) != 0) {
        zero = 0;
        Object_GetById(10)->priority_flags = 2;
        DrawPlacement(0, 17, 2, 4, 19, 17);
        record = StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, zero);
        record = (s32)Object_GetById(10);
        Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
    }
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    if (gGameState.entrance == 4) {
        if (Engine_GameFlagIsSet(0x843) == 0) {
            FieldScene_RunBranchingFormationPresentation();
        }
    }
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Engine_ActorSetPosition(17, 0, 0);
        Engine_ActorSetPosition(18, 0, 0);
        Engine_ActorSetPosition(19, 0, 0);
        Engine_ActorSetPosition(20, 0, 0);
        Engine_ActorSetPosition(21, 0, 0);
    }
    return 0;
}

void Object_RefreshSelectorById(s32 id);
void SceneActor_SetPairZeroAndValue(s32 actor, s32 facing, s32 frames);
void Event_SayThenWait(s32 speaker, s32 frames);
void Object_SetActionCallbackAndRefreshById(s32 id, const u8 *callback);
void Audio_PlayCueFromEventWork(void);


void FieldScene_RunBranchingFormationPresentation(void)
{
    struct FieldActor *record;
    s32 *party;
    const u8 *entry_action;
    const u8 *motion_action;
    s32 *party_flag;
    const u8 *reset_action;
    const u8 *formation_action;
    const u8 *finish_action;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Camera_MoveTo(0xf60000, -1, 0x25c0000, 0);
    party = &KorimaHashi_PartyFlag;
    *party = GameFlag_IsSet(3);
    record = Object_GetById(13);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(14);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(15);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(16);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(17);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(18);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(19);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(20);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(21);
    Engine_ActorSetSpriteFlags(record, 0);
    entry_action = KorimaHashi_EntryAction;
    Engine_ActorEnableActionCallback(17, entry_action);
    Engine_ActorEnableActionCallback(18, entry_action);
    Engine_ActorEnableActionCallback(19, entry_action);
    Engine_ActorEnableActionCallback(20, entry_action);
    Engine_ActorEnableActionCallback(21, entry_action);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x740000, 0x25a0000);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Actor_SetMotionSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 0x251);
    Actor_SetMotionSpeed(GERALD, 0x9999, 0x4ccc);
    Actor_SetMotionSpeed(IWAN, 0x9999, 0x4ccc);
    {
        struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

        if (leader != 0) {
            Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
        }
    }
    {
        struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

        if (leader != 0) {
            Actor_SetPosition(IWAN, leader->x.fixed, leader->z.fixed);
        }
    }
    Engine_ActorEnableActionCallback(GERALD, KorimaHashi_GeraldAction);
    Engine_ActorEnableActionCallback(IWAN, KorimaHashi_IvanAction);
    if (*party != 0) {
        Actor_SetMotionSpeed(MEARI, 0x9999, 0x4ccc);
        {
            struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

            if (leader != 0) {
                Actor_SetPosition(MEARI, leader->x.fixed, leader->z.fixed);
            }
        }
        Engine_ActorEnableActionCallback(MEARI, KorimaHashi_MiaAction);
    }
    Object_RefreshSelectorById(2);
    SceneActor_SetPairZeroAndValue(2, 0x2000, 40);
    SceneActor_SetPairZeroAndValue(2, 0x8000, 20);
    SceneActor_SetPairZeroAndValue(2, 0x4000, 40);
    Actor_ShowEmote(IWAN, 0x101, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
    SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 10);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventSetMessage((s32)MsgKorimaMatterIvan);
    Event_SayThenWait(GERALD, 10);
    Engine_EventSetMessage((s32)MsgKorimaSparklyStuffOnGround);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_SayThenWait(IWAN, 20);
    SceneActor_SetPairZeroAndValue(1, 0, 20);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 40);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 30);
    SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
    SceneActor_SetPairZeroAndValue(0, 0xe000, 30);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Audio_PlayCue(17);
    Audio_PlayCue(206);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    KorimaHashi_SparkleSound = 1;
    Engine_TaskAddCallback(SceneEffect_SpawnObject26EveryEightFrames, 0xc80);
    Engine_TaskWait(20);
    ColorBuffer_ApplyTarget(0x405210, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(60);
    motion_action = KorimaHashi_PartyAction;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, motion_action);
    Engine_ActorEnableActionCallback(GERALD, motion_action);
    Engine_ActorEnableActionCallback(IWAN, motion_action);
    Engine_ActorEnableActionCallback(MEARI, motion_action);
    Engine_EventWait(100);
    Event_SayThenWait(GERALD, 20);
    Event_SayThenWait(IWAN, 40);
    if (KorimaHashi_PartyFlag != 0) {
        Engine_EventWait(40);
        Actor_ShowEmote(MEARI, 0x102, 0);
        Engine_EventWait(40);
        Event_SayThenWait(MEARI, 40);
    } else {
        gEventWork->message += 1;
    }
    Engine_EventWait(20);
    if (KorimaHashi_PartyFlag != 0) {
        record = Object_GetById(3);
        record->velocity_y = 0x20000;
        Engine_EventWait(10);
        Actor_SetMotionSpeed(MEARI, 0x20000, 0x20000);
        Actor_OffsetDestination(MEARI, -2, 0);
        Engine_ActorEnableActionCallback(MEARI, KorimaHashi_ResetAction);
        record = Object_GetById(3);
        Engine_ActorSetSpriteFlags(record, 0);
        Engine_ActorSetAnimation(MEARI, 19);
        Engine_EventWait(10);
    }
    record = Object_GetById(0);
    record->velocity_y = 0x20000;
    Engine_EventWait(10);
    Actor_SetMotionSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
    reset_action = KorimaHashi_ResetAction;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, reset_action);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
    Engine_EventWait(20);
    record = Object_GetById(1);
    record->velocity_y = 0x20000;
    Engine_EventWait(10);
    Actor_SetMotionSpeed(GERALD, 0x20000, 0x20000);
    Engine_ActorEnableActionCallback(GERALD, reset_action);
    record = Object_GetById(1);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(GERALD, 19);
    Engine_EventWait(40);
    record = Object_GetById(2);
    record->velocity_y = 0x20000;
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(IWAN, reset_action);
    record = Object_GetById(2);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(IWAN, 19);
    KorimaHashi_SparkleSound = 0;
    Engine_EventWait(160);
    Engine_TaskRemoveCallback(SceneEffect_SpawnObject26EveryEightFrames);
    Engine_EventWait(120);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    gFallingEffectWidth = 0;
    gFallingEffectOffset = 0x800000;
    gFallingEffectState = 1;
    Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
    Engine_EventWait(180);
    Audio_PlayCue(21);
    Event_SayThenWait(GERALD, 80);
    Event_SayThenWait(IWAN, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Actor_ShowEmote(IWAN, 0x102, 0);
    Actor_ShowEmote(MEARI, 0x102, 0);
    Engine_EventWait(60);
    Event_SayThenWait(IWAN, 20);
    gFallingEffectState = 2;
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(IWAN, 3);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Event_SayThenWait(GERALD, 20);
    if (KorimaHashi_PartyFlag != 0) {
        Actor_ShowEmote(MEARI, 0x102, 0);
        Event_SayThenWait(MEARI, 10);
    } else {
        gEventWork->message += 1;
    }
    gFallingEffectState = 3;
    Object_GetById(0)->priority_flags &= 254;
    Object_GetById(1)->priority_flags &= 254;
    Object_GetById(2)->priority_flags &= 254;
    Object_GetById(3)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetSpritePriority(GERALD, 3);
    Engine_ActorSetSpritePriority(IWAN, 3);
    Engine_ActorSetSpritePriority(MEARI, 3);
    KorimaHashi_TriggerPending = 0;
    Engine_TaskAddCallback(KorimaHashi_AlternateBridgePostHeights, 0xc80);
    Audio_PlayCue(220);
    Object_GetById(13)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(13, 2);
    Actor_SetPosition(13, 0xfd0000, 0x25b0000);
    formation_action = KorimaHashi_FormationAction;
    Engine_ActorEnableActionCallback(13, formation_action);
    Object_GetById(14)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(14, 2);
    Actor_SetPosition(14, 0xe90000, 0x2750000);
    Engine_ActorEnableActionCallback(14, formation_action);
    if (KorimaHashi_PartyFlag != 0) {
        Object_GetById(15)->priority_flags &= 254;
        Engine_ActorSetSpritePriority(15, 2);
        Actor_SetPosition(15, 0xcf0000, 0x2610000);
        Engine_ActorEnableActionCallback(15, formation_action);
    }
    Object_GetById(16)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(16, 2);
    Actor_SetPosition(16, 0xe30000, 0x2440000);
    Engine_ActorEnableActionCallback(16, formation_action);
    if (gFallingEffectState != 0) {
        do {
            Engine_TaskWait(1);
        } while (gFallingEffectState != 0);
    }
    Engine_EventWait(0x12c);
    Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
    Engine_EventWait(120);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x10000, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Engine_ActorStop(13);
    Engine_ActorStop(14);
    if (KorimaHashi_PartyFlag != 0) {
        Engine_ActorStop(15);
    }
    Engine_ActorStop(16);
    Engine_TaskWait(1);
    finish_action = KorimaHashi_FinishAction;
    Engine_ActorEnableActionCallback(13, finish_action);
    Engine_ActorEnableActionCallback(14, finish_action);
    if (KorimaHashi_PartyFlag != 0) {
        Engine_ActorEnableActionCallback(15, finish_action);
    }
    Object_SetActionCallbackAndRefreshById(16, finish_action);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(40);
    Event_OpenMessage(GERALD, 0);
    Actor_SetPosition(11, 0xdc0000, 0x1ee0000);
    Actor_SetPosition(12, 0xdc0000, 0x1ee0000);
    Engine_TaskWait(1);
    if (Engine_EventChooseYesNo(11, 0) == 1) {
        gEventWork->message += 1;
    }
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Event_SayThenWait(IWAN, 20);
    if (KorimaHashi_PartyFlag != 0) {
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgKorimaKnowThoseFieldsWere);
        Event_SayThenWait(MEARI, 40);
    }
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventSetMessage((s32)MsgKorimaWasOurPsynergy);
    Event_SayThenWait(IWAN, 40);
    Engine_ActorRunRepeatedMotion(GERALD, 3);
    Engine_EventWait(40);
    Engine_ActorSetSpritePriority(GERALD, 2);
    Object_GetById(1)->priority_flags |= 1;
    record = Object_GetById(1);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorJump(GERALD, 6, 0);
    Actor_OffsetDestination(GERALD, -3, 0);
    Engine_ActorSetAnimation(GERALD, 1);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 60);
    Event_SayThenWait(GERALD, 20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Event_SayThenWait(GERALD, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Engine_EventWait(40);
    SceneActor_SetPairZeroAndValue(1, 0x6000, 40);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
    SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
    Engine_ActorJump(GERALD, 2, 0);
    Engine_EventWait(40);
    Engine_ActorJump(GERALD, 2, 0);
    Engine_EventWait(10);
    Engine_ActorJump(GERALD, 4, 0);
    Engine_EventWait(20);
    Event_SayThenWait(GERALD, 20);
    if (KorimaHashi_PartyFlag != 0) {
        Actor_ShowEmote(MEARI, 0x100, 0);
        Engine_EventWait(60);
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        Engine_EventWait(80);
        Engine_ActorSetSpritePriority(MEARI, 2);
        Object_GetById(3)->priority_flags |= 1;
        record = Object_GetById(3);
        Engine_ActorSetSpriteFlags(record, 1);
        Engine_ActorJump(MEARI, 4, 0);
        Actor_OffsetDestination(MEARI, -2, 0);
        Engine_ActorSetAnimation(MEARI, 1);
        SceneActor_SetPairZeroAndValue(3, 0xe000, 60);
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        Engine_EventWait(20);
        Event_SayThenWait(MEARI, 20);
    } else {
        gEventWork->message += 1;
    }
    Engine_ActorJump(GERALD, 2, 0);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
    Event_SayThenWait(GERALD, 20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Engine_ActorSetSpritePriority(IWAN, 2);
    Object_GetById(2)->priority_flags |= 1;
    record = Object_GetById(2);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorJump(IWAN, 4, 0);
    Engine_ActorSetAnimation(IWAN, 1);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(10);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Object_GetById(0)->priority_flags |= 1;
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(IWAN, 0x105, 0);
    Engine_EventWait(60);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_SayThenWait(IWAN, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    Event_OpenMessage(IWAN, 0);
    Engine_ActorSetAnimation(IWAN, 3);
    Actor_FaceDirection(IWAN, FACING_NORTHEAST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimation(IWAN, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        gEventWork->message += 1;
    } else {
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
        Engine_ActorStartRepeatedMotion(GERALD, 2);
        Event_ShowMessage(GERALD, 0);
    }
    SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    Event_SayThenWait(GERALD, 20);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_SayThenWait(IWAN, 10);
    if (KorimaHashi_PartyFlag != 0) {
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        SceneActor_SetPairZeroAndValue(3, 0, 20);
        SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
        Engine_ActorSetAnimation(MEARI, 4);
        Event_SayThenWait(MEARI, 10);
    } else {
        gEventWork->message += 1;
    }
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Engine_EventWait(80);
    SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    Event_SayThenWait(IWAN, 20);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 40);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_SayThenWait(IWAN, 10);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_EventWait(40);
    Event_SayThenWait(GERALD, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Engine_EventWait(40);
    Event_SayThenWait(GERALD, 20);
    Engine_ActorSetAnimation(IWAN, 3);
    Event_SayThenWait(IWAN, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
    Event_OpenMessage(GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(GERALD, 3);
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(40);
        gEventWork->message += 1;
    }
    Event_ShowMessage(GERALD, 0);
    Audio_PlayCue(21);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    gFallingEffectWidth = 0;
    gFallingEffectOffset = 0x800000;
    gFallingEffectState = 1;
    Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
    Engine_EventWait(80);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(60);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Engine_EventSetMessage((s32)MsgKorimaWatchOutItsHappeningAgain);
    Event_SayThenWait(IWAN, 10);
    SceneActor_SetPairZeroAndValue(1, 0xc000, 10);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
    if (KorimaHashi_PartyFlag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
    }
    Object_GetById(0)->priority_flags &= 254;
    Object_GetById(1)->priority_flags &= 254;
    Object_GetById(2)->priority_flags &= 254;
    Object_GetById(3)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetSpritePriority(GERALD, 3);
    Engine_ActorSetSpritePriority(IWAN, 3);
    Engine_ActorSetSpritePriority(MEARI, 3);
    gFallingEffectState = 2;
    Audio_PlayCue(220);
    Actor_SetPosition(13, 0xfd0000, 0x25b0000);
    formation_action = KorimaHashi_FormationAction;
    Engine_ActorEnableActionCallback(13, formation_action);
    Actor_SetPosition(14, 0xe90000, 0x2750000);
    Engine_ActorEnableActionCallback(14, formation_action);
    if (KorimaHashi_PartyFlag != 0) {
        Actor_SetPosition(15, 0xcf0000, 0x2610000);
        Engine_ActorEnableActionCallback(15, formation_action);
    }
    Actor_SetPosition(16, 0xe30000, 0x2440000);
    Engine_ActorEnableActionCallback(16, formation_action);
    Engine_EventWait(120);
    gFallingEffectState = 3;
    do {
        Engine_TaskWait(1);
    } while (gFallingEffectState != 0);
    Event_SayThenWait(11, 80);
    Event_SayThenWait(12, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(IWAN, 0x101, 0);
    Actor_ShowEmote(MEARI, 0x101, 0);
    Engine_EventWait(60);
    Event_SayThenWait(12, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_SayThenWait(12, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_SayThenWait(12, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 0);
    Engine_EventWait(40);
    Event_SayThenWait(11, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(MEARI, FACING_EAST, 0);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 40);
    Event_SayThenWait(12, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Actor_ShowEmote(IWAN, 0x102, 0);
    Actor_ShowEmote(MEARI, 0x102, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    SceneActor_SetPairZeroAndValue(3, 0xc000, 80);
    Event_SayThenWait(12, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    SceneActor_SetPairZeroAndValue(3, 0, 40);
    Event_SayThenWait(11, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimation(GERALD, 4);
    Engine_ActorSetAnimation(MEARI, 4);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventWait(60);
    Event_SayThenWait(12, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_SayThenWait(12, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    SceneActor_SetPairZeroAndValue(3, 0, 20);
    Event_SayThenWait(12, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    Event_SayThenWait(12, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    SceneActor_SetPairZeroAndValue(3, 0, 20);
    Event_SayThenWait(11, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Actor_ShowEmote(MEARI, 0x102, 0);
    Actor_ShowEmote(IWAN, 0x102, 0);
    Engine_EventWait(40);
    Event_SayThenWait(12, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
    Event_SayThenWait(12, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(60);
    Event_ShowMessage(12, 0);
    Event_ShowMessage(11, 0);
    Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
    Engine_EventWait(80);
    ColorBuffer_ApplyTarget(0x10000, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(80);
    Engine_ActorStop(13);
    Engine_ActorStop(14);
    party_flag = &KorimaHashi_PartyFlag;
    Engine_ActorStop(15);
    Engine_ActorStop(16);
    Engine_TaskWait(1);
    finish_action = KorimaHashi_FinishAction;
    Engine_ActorEnableActionCallback(13, finish_action);
    Engine_ActorEnableActionCallback(14, finish_action);
    if (*party_flag != 0) {
        Engine_ActorEnableActionCallback(15, finish_action);
    }
    Object_SetActionCallbackAndRefreshById(16, finish_action);
    Engine_EventWait(20);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetSpritePriority(GERALD, 2);
    Engine_ActorSetSpritePriority(IWAN, 2);
    Engine_ActorSetSpritePriority(MEARI, 2);
    Object_GetById(0)->priority_flags |= 1;
    Object_GetById(1)->priority_flags |= 1;
    Object_GetById(2)->priority_flags |= 1;
    Object_GetById(3)->priority_flags |= 1;
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
    Event_OpenMessage(IWAN, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_EAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(10);
        Event_OpenMessage(GERALD, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            SceneActor_SetPairZeroAndValue(3, 0, 20);
            Actor_ShowEmote(GERALD, 0x101, 0);
            Actor_ShowEmote(IWAN, 0x101, 0);
            Actor_ShowEmote(MEARI, 0x101, 0);
            Engine_EventWait(40);
            SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
            Event_SayThenWait(GERALD, 10);
            SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
            SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
            Engine_ActorSetAnimationAndWait(IWAN, 3);
            Event_SayThenWait(IWAN, 20);
            SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
        } else {
            SceneActor_SetPairZeroAndValue(3, 0, 20);
            Actor_ShowEmote(GERALD, 0x102, 0);
            Actor_ShowEmote(IWAN, 0x102, 0);
            Actor_ShowEmote(MEARI, 0x102, 0);
            Engine_EventWait(40);
            SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
            Engine_EventSetMessage((s32)MsgKorimaHesAsStumpedAsWe);
            Event_SayThenWait(GERALD, 20);
            Engine_ActorSetAnimationAndWait(IWAN, 3);
            Event_SayThenWait(IWAN, 20);
        }
        Engine_ActorSetAnimation(MEARI, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        goto ivan_bows;
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgKorimaYoureRobinThereIsntMuch);
    Event_SayThenWait(GERALD, 10);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Event_OpenMessage(IWAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Actor_ShowEmote(IWAN, 0x103, 0);
        Engine_EventWait(40);
        SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
        Event_SayThenWait(IWAN, 10);
        if (*party_flag != 0) {
            SceneActor_SetPairZeroAndValue(3, 0, 10);
            Engine_ActorStartRepeatedMotion(MEARI, 3);
            Event_SayThenWait(MEARI, 20);
        } else {
            gEventWork->message += 1;
        }
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
        Actor_ShowEmote(GERALD, 0x102, 0);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Event_SayThenWait(GERALD, 20);
        Actor_ShowEmote(GERALD, 0x105, 0);
        Engine_EventWait(120);
        Event_SayThenWait(IWAN, 40);
        if (KorimaHashi_PartyFlag != 0) {
            SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
            Engine_ActorSetAnimationAndWait(MEARI, 4);
            Event_SayThenWait(MEARI, 10);
        } else {
            gEventWork->message += 1;
        }
        Engine_EventWait(60);
        Engine_ActorRunRepeatedMotion(IWAN, 2);
        if (KorimaHashi_PartyFlag != 0) {
            SceneActor_SetPairZeroAndValue(2, 0xa000, 40);
            SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
        }
        Event_SayThenWait(IWAN, 10);
        Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(40);
        Event_SayThenWait(IWAN, 20);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimation(MEARI, 3);
        ivan_bows:;
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        goto leave;
    }
    Actor_ShowEmote(IWAN, 0x105, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventSetMessage((s32)MsgKorimaThatsReliefRobinThoughtYoud);
    Event_SayThenWait(IWAN, 20);
    if (*party_flag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0, 10);
        Engine_ActorStartRepeatedMotion(MEARI, 1);
        Event_SayThenWait(MEARI, 20);
    } else {
        gEventWork->message += 1;
    }
    Actor_ShowEmote(GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Event_SayThenWait(GERALD, 20);
    Actor_ShowEmote(IWAN, 0x105, 0);
    Engine_EventWait(80);
    Event_SayThenWait(IWAN, 40);
    if (KorimaHashi_PartyFlag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0x2000, 20);
        Engine_ActorSetAnimation(MEARI, 4);
        Event_SayThenWait(MEARI, 40);
    } else {
        gEventWork->message += 1;
    }
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Event_SayThenWait(IWAN, 20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    Event_SayThenWait(IWAN, 20);
    leave:;
    Audio_PlayCue(17);
    Actor_SetMotionSpeed(GERALD, 0x13333, 0x9999);
    Actor_SetMotionSpeed(IWAN, 0x13333, 0x9999);
    Actor_SetMotionSpeed(MEARI, 0x13333, 0x9999);
    Engine_ActorSetAnimation(GERALD, 2);
    {
        struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

        if (leader != 0) {
            Actor_SetDestination(GERALD, leader->x.part.pixel, leader->z.part.pixel);
        }
    }
    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Engine_ActorSetAnimation(IWAN, 2);
    {
        struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

        if (leader != 0) {
            Actor_SetDestination(IWAN, leader->x.part.pixel, leader->z.part.pixel);
        }
    }
    Engine_ActorWaitForMove(IWAN);
    Actor_SetPosition(IWAN, 0, 0);
    if (KorimaHashi_PartyFlag != 0) {
        Engine_ActorSetAnimation(MEARI, 2);
        {
            struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

            if (leader != 0) {
                Actor_SetDestination(MEARI, leader->x.part.pixel, leader->z.part.pixel);
            }
        }
        Engine_ActorWaitForMove(MEARI);
        Actor_SetPosition(MEARI, 0, 0);
    }
    GameFlag_Set(0x843);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Audio_PlayCueFromEventWork();
    Engine_EventEnd();
}

void Event_SayThenWait(s32 speaker, s32 frames)
{
    Engine_EventShowMessage(speaker, 0);
    Engine_EventWait(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Engine_ActorFaceDirection(a, b, 0);
    Engine_EventWait(c);
}

s32 SceneEffect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p)
{
    p->field18 += 0x1eb8;
    if (p->field38 == (s32)0x80000000
        && p->field3c == (s32)0x80000000
        && p->field40 == (s32)0x80000000) {
        Engine_ObjectDispatchRelease(p);
    }
    return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void)
{

    struct Obj *obj;
    struct Sub *sprite;
    s32 phase;
    s32 v;
    s32 w;
    s32 c1 = 0x00e70000;
    s32 c2 = 0x01cc0000;
    s32 c3 = 0x00e70000;
    s32 c4 = 0x02700000;

    phase = gFrameCount & 7;
    if (phase != 0) return;
    if (KorimaHashi_SparkleSound != 0) Engine_AudioPlayCue(200);
    obj = (struct Obj *)Engine_ObjectCreate(26, c1, 0, c2);
    if (obj == 0) return;
    sprite = obj->f50;
    sprite->f26 = phase;
    v = 0xfe;
    v &= obj->f23;
    obj->f23 = v;
    w = ~12;
    w &= sprite->f09;
    w |= 4;
    sprite->f09 = w;
    obj->f18 = 0x1999;
    obj->f30 = 0x80000;
    obj->f34 = 0x80000;
    obj->f55 = phase;
    Object_SetMode((struct FieldActor *)obj, 2);
    ((void (*)(struct Obj *object, s32 x, s32 y, s32 z))Engine_ObjectSetPosition)(obj, c3, 0, c4);
    Engine_ObjectSetScript((struct FieldActor *)obj, (const s32 *)KorimaHashi_Object26Script);
}

s32 OverlayObject_SelectValueByFrameBit1(s32 obj)
{

    if (((gFrameCount >> 1) & 1) != 0) {
        ObjectGroup_SetChildValue(obj, 10);
    } else {
        ObjectGroup_SetChildValue(obj, 7);
    }
    return 0;
}

s32 SceneActor_CheckRegionTrigger(struct Struct288c *arg0)
{
    s32 x;

    if (KorimaHashi_PartyFlag != 0) {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0x1120000
            && arg0->field10 > 0x2360000 && arg0->field10 < 0x2640000) {
            goto hit;
        }
        if (x > 0xca0000 && x < 0xff0000
            && arg0->field10 > 0x2250000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    } else {
        x = arg0->field08;
        if (x > 0xc00000 && x < 0xf40000
            && arg0->field10 > 0x2250000 && arg0->field10 <= 0x248ffff) {
            goto hit;
        }
        if (x > 0xf40000 && x < 0x1120000
            && arg0->field10 > 0x23b0000 && arg0->field10 <= 0x25cffff) {
            goto hit;
        }
        if (x > 0xd30000 && x < 0xff0000
            && arg0->field10 > 0x2540000 && arg0->field10 < 0x2780000) {
            goto hit;
        }
    }
    return 0;
hit:
    Engine_AudioPlayCue(106);
    ((s32 (*)())Engine_ObjectSetScript)((s32)arg0, (s32)KorimaHashi_TriggerScript);
    KorimaHashi_TriggerPending = 1;
    return 0;
}
