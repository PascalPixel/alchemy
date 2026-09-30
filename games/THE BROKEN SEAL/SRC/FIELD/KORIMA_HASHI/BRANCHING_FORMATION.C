#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KORIMA_MURA.H"
#include "STAGED_ACTOR.H"
#include "STAGED_ACTOR_EFFECT.H"
extern u8 MsgKorimaHesAsStumpedAsWe[];
extern u8 MsgKorimaKnowThoseFieldsWere[];
extern u8 MsgKorimaMatterIvan[];
extern u8 MsgKorimaSparklyStuffOnGround[];
extern u8 MsgKorimaThatsReliefRobinThoughtYoud[];
extern u8 MsgKorimaWasOurPsynergy[];
extern u8 MsgKorimaWatchOutItsHappeningAgain[];
extern u8 MsgKorimaYoureRobinThereIsntMuch[];

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

/* The bridge's own work, past the overlay image. */
s32 KorimaHashi_TriggerPending __attribute__((section(".bss")));
s32 KorimaHashi_PartyFlag __attribute__((section(".bss")));
s32 KorimaHashi_SparkleSound __attribute__((section(".bss")));

/* Tables laid out after the code. */
extern const struct SceneEntrance KorimaHashi_Entrances[];
extern const u32 KorimaHashi_Exits[];
extern const struct ScenePlacement KorimaHashi_Placements[];
extern const struct SceneEvent KorimaHashi_Events[];
extern u8 KorimaHashi_Object26Script[];
extern u8 KorimaHashi_TriggerScript[];


/* The main-image services this bridge reaches through veneers the
   staged-actor module names. */
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Object_SetModeById(s32 actor, s32 mode);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);

/* Motion services in the shape of the engine header's, through the names
   the staged-actor module gives their veneers. */
static __inline__ void Actor_SetMotionSpeed(s32 actor, s32 speed, s32 acceleration)
{
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Actor_OffsetDestination(s32 actor, s32 dx, s32 dz)
{
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}
void Map_CopyCellAttributeRect(s32 x, s32 z, s32 width, s32 height, s32 dest_x, s32 dest_z);
void Object_SetPosition(struct Obj *object, s32 x, s32 y, s32 z);

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

static __inline__ void DrawPlacement(
    s32 left, s32 top, s32 width, s32 height, s32 tile, s32 palette)
{
    Map_CopyCellAttributeRect(left, top, width, height, tile, palette);
}

s32 Object_CheckMovementCollision(struct StagedActorEffect *actor,
                                  struct StagedActorEffectRequest *request);

/* Clear the pending trigger after restoring the object's mode. */
s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object)
{
    s32 *pending = &KorimaHashi_TriggerPending;
    if (*pending) {
        Object_SetAnimation(object, 2);
        *pending = 0;
    }
    return 1;
}

s32 SceneActor_UpdateRandomCounterMode(u8 *object)
{
    u16 *counter = (u16 *)(object + 100);
    *counter = (u16)(*counter + (((u32)Random_Next() * 100) >> 16));
    if ((s16)*counter > 1000) Object_SetPalette(object, 7);
    else Object_SetPalette(object, 10);
    if (*(s16 *)counter > 1200) { u16 z = 0; *counter = z; }
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return KorimaHashi_Entrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return KorimaHashi_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) { return KorimaHashi_Placements; }

/* Placement query followed by the tile-(10,20) scene transition. */
void FieldScene_RunTile10x20Transition(void)
{
    struct StagedActorProbe res;
    Event_Begin();

    if (StagedActor_FindClearPosition(&res)) {
        SceneActor_MoveAndRedraw(res);
        if (res.actor_slot == 10 && (res.position_x >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Object_SetModeById(10, 3);
            Actor_OffsetDestination(10, -18, 6);
            Battle_WaitMode0(30);
            Audio_PlayCue(240);
            Object_SetModeById(10, 8);
            ((u8 *)Object_GetById(10))[35] = 2;
            zero = 0;
            DrawPlacement(0, 17, 2, 4, 19, 17);
            StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, zero);
            GameFlag_Set(0x200);
            actor = (void *)Object_GetById(10);
            Actor_SetSpriteFlags((struct FieldActor *)actor, 0);
        }
    }

    Event_End();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
    struct StagedActorEffect *actor = (void *)Object_GetById(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 ret = Object_CheckMovementCollision(actor, request);

    if (ret == 0) {
        Event_Begin();
        Object_SetAnimation((struct FieldActor *)actor, 6);
        WaitFrames(6);
        Audio_PlayCue(152);
        Object_SetAnimation((struct FieldActor *)actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        Actor_SetSpriteFlags((struct FieldActor *)actor, 0);
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, request->cell_x, request->cell_z);
        Object_SetAnimation((struct FieldActor *)actor, 6);
        Actor_SetSpriteFlags((struct FieldActor *)actor, 1);
        *flags = (u8)ret;
        Object_SetModeById(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        WaitFrames(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        WaitFrames(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        WaitFrames(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        Event_End();
        return 1;
    }
    return 0;
}

/* The selected actor is read through a register offset into the game state. */
void SceneActor_PassSubjectOffsetPosition(void)
{
    u32 buf[3];
    s32 off = 500;
    struct Struct3848 *p = (void *)Object_GetById(*(s32 *)((u8 *)&gGameState + off));
    u32 base = p->field08 & 0xfff00000;

    buf[0] = base + 0x80000;
    buf[1] = p->field0c;
    buf[2] = (p->field10 & 0xfff00000) + 0x80000;
    buf[0] = base + 0x280000;
    StagedActor_RunStepEffect((struct StagedActorEffectRequest *)buf);
}

const struct SceneEvent *Scene_GetEvents(void) { return KorimaHashi_Events; }

void FieldScene_RunBranchingFormationPresentation(void);

s32 Scene_Initialize(void)
{
    s32 record;
    s32 zero;

    FieldScene_RedrawActorFootprint(10);
    if (GameFlag_IsSet(0x200) != 0) {
        zero = 0;
        *((u8 *)Object_GetById(10) + 35) = 2;
        DrawPlacement(0, 17, 2, 4, 19, 17);
        record = StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, zero);
        record = (s32)Object_GetById(10);
        Actor_SetSpriteFlags((struct FieldActor *)record, 0);
    }
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    if (gGameState.entrance == 4) {
        if (GameFlag_IsSet(0x843) == 0) {
            FieldScene_RunBranchingFormationPresentation();
        }
    }
    if (GameFlag_IsSet(0x845) != 0) {
        Actor_SetPosition(17, 0, 0);
        Actor_SetPosition(18, 0, 0);
        Actor_SetPosition(19, 0, 0);
        Actor_SetPosition(20, 0, 0);
        Actor_SetPosition(21, 0, 0);
    }
    return 0;
}

void Object_RefreshSelectorById(s32 id);
void SceneActor_SetPairZeroAndValue(s32 actor, s32 facing, s32 frames);
void Event_SayThenWait(s32 speaker, s32 frames);
void Object_SetActionCallbackAndRefreshById(s32 id, s32 callback);
void Audio_PlayCueFromEventWork(void);
void SceneActor_AlternateSlots13To16Field0c(void);

extern u8 KorimaHashi_GeraldAction[];
extern u8 KorimaHashi_IvanAction[];
extern u8 KorimaHashi_MiaAction[];
extern u8 KorimaHashi_PartyAction[];
extern u8 KorimaHashi_ResetAction[];
extern u8 KorimaHashi_FormationAction[];
extern u8 KorimaHashi_FinishAction[];
extern u8 KorimaHashi_EntryAction[];

void FieldScene_RunBranchingFormationPresentation(void)
{
    u8 *record;
    s32 *flag_work;
    s32 entry_action;
    s32 value;
    s32 flag;
    s32 motion_action;
    s32 *party_flag;
    s32 *formation_flag;
    s32 *sequence_flag;
    s32 *finish_flag;
    s32 reset_action;
    s32 *effect_phase;
    s32 *formation_phase;
    s32 formation_action;
    s32 finish_action;
    s32 *sequence_phase;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    WaitFrames(1);
    Camera_MoveTo(0xf60000, -1, 0x25c0000, 0);
    flag_work = &KorimaHashi_PartyFlag;
    flag = GameFlag_IsSet(3);
    *flag_work = flag;
    record = (void *)Object_GetById(13);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(14);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(15);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(16);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(17);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(18);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(19);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(20);
    Actor_SetSpriteFlags((s32)record, 0);
    record = (void *)Object_GetById(21);
    Actor_SetSpriteFlags((s32)record, 0);
    entry_action = (s32)KorimaHashi_EntryAction;
    Actor_EnableActionCallback(17, entry_action);
    Actor_EnableActionCallback(18, entry_action);
    Actor_EnableActionCallback(19, entry_action);
    Actor_EnableActionCallback(20, entry_action);
    Actor_EnableActionCallback(21, entry_action);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x740000, 0x25a0000);
    WaitFrames(1);
    Map_Redraw();
    WaitFrames(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetMotionSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 0x251);
    Actor_SetMotionSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetMotionSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    {
        u8 *record = (void *)Object_GetById(0);

        if (record != 0) {
            Actor_SetPosition(ACTOR_GERALD, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    {
        u8 *record = (void *)Object_GetById(0);

        if (record != 0) {
            Actor_SetPosition(ACTOR_IVAN, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    Actor_EnableActionCallback(ACTOR_GERALD, (s32)KorimaHashi_GeraldAction);
    Actor_EnableActionCallback(ACTOR_IVAN, (s32)KorimaHashi_IvanAction);
    if (*flag_work != 0) {
        Actor_SetMotionSpeed(ACTOR_MIA, 0x9999, 0x4ccc);
        {
            u8 *record = (void *)Object_GetById(0);

            if (record != 0) {
                Actor_SetPosition(ACTOR_MIA, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
        }
        Actor_EnableActionCallback(ACTOR_MIA, (s32)KorimaHashi_MiaAction);
    }
    Object_RefreshSelectorById(2);
    SceneActor_SetPairZeroAndValue(2, 0x2000, 40);
    SceneActor_SetPairZeroAndValue(2, 0x8000, 20);
    SceneActor_SetPairZeroAndValue(2, 0x4000, 40);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Battle_WaitMode0(60);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
    value = 160;
    SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    SceneActor_SetPairZeroAndValue(0, (value << 8), 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Battle_WaitMode0(40);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SetMessage((s32)MsgKorimaMatterIvan);
    Event_SayThenWait(ACTOR_GERALD, 10);
    Event_SetMessage((s32)MsgKorimaSparklyStuffOnGround);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SayThenWait(ACTOR_IVAN, 20);
    SceneActor_SetPairZeroAndValue(1, 0, 20);
    SceneActor_SetPairZeroAndValue(0, (value << 8), 40);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 30);
    SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
    SceneActor_SetPairZeroAndValue(0, 0xe000, 30);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Battle_WaitMode0(40);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Audio_PlayCue(17);
    Audio_PlayCue(206);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(1);
    WaitFrames(1);
    KorimaHashi_SparkleSound = 1;
    Engine_TaskAddCallback((s32)SceneEffect_SpawnObject26EveryEightFrames, 0xc80);
    WaitFrames(20);
    ColorBuffer_ApplyTarget(0x405210, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(120);
    WaitFrames(60);
    motion_action = (s32)KorimaHashi_PartyAction;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, motion_action);
    Actor_EnableActionCallback(ACTOR_GERALD, motion_action);
    Actor_EnableActionCallback(ACTOR_IVAN, motion_action);
    Actor_EnableActionCallback(ACTOR_MIA, motion_action);
    Battle_WaitMode0(100);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Event_SayThenWait(ACTOR_IVAN, 40);
    if (KorimaHashi_PartyFlag != 0) {
        Battle_WaitMode0(40);
        Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
        Battle_WaitMode0(40);
        Event_SayThenWait(ACTOR_MIA, 40);
    } else {
        gEventWork->message += 1;
    }
    Battle_WaitMode0(20);
    party_flag = &KorimaHashi_PartyFlag;
    if (*party_flag != 0) {
        value = 128;
        record = (void *)Object_GetById(3);
        *(s32 *)((s32)record + 40) = (value << 10);
        Battle_WaitMode0(10);
        Actor_SetMotionSpeed(ACTOR_MIA, (value << 10), (value << 10));
        Actor_OffsetDestination(ACTOR_MIA, -2, 0);
        Actor_EnableActionCallback(ACTOR_MIA, (s32)KorimaHashi_ResetAction);
        record = (void *)Object_GetById(3);
        Actor_SetSpriteFlags((s32)record, 0);
        Object_SetModeById(ACTOR_MIA, 19);
        Battle_WaitMode0(10);
    }
    value = 128;
    record = (void *)Object_GetById(0);
    *(s32 *)((s32)record + 40) = (value << 10);
    Battle_WaitMode0(10);
    Actor_SetMotionSpeed(ACTOR_PARTY_LEADER, (value << 10), (value << 10));
    reset_action = (s32)KorimaHashi_ResetAction;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, reset_action);
    record = (void *)Object_GetById(0);
    Actor_SetSpriteFlags((s32)record, 0);
    Object_SetModeById(ACTOR_PARTY_LEADER, 19);
    Battle_WaitMode0(20);
    record = (void *)Object_GetById(1);
    *(s32 *)((s32)record + 40) = (value << 10);
    Battle_WaitMode0(10);
    Actor_SetMotionSpeed(ACTOR_GERALD, (value << 10), (value << 10));
    Actor_EnableActionCallback(ACTOR_GERALD, reset_action);
    record = (void *)Object_GetById(1);
    Actor_SetSpriteFlags((s32)record, 0);
    Object_SetModeById(ACTOR_GERALD, 19);
    Battle_WaitMode0(40);
    record = (void *)Object_GetById(2);
    *(s32 *)((s32)record + 40) = (value << 10);
    Battle_WaitMode0(10);
    Actor_EnableActionCallback(ACTOR_IVAN, reset_action);
    record = (void *)Object_GetById(2);
    Actor_SetSpriteFlags((s32)record, 0);
    Object_SetModeById(ACTOR_IVAN, 19);
    KorimaHashi_SparkleSound = 0;
    Battle_WaitMode0(160);
    Engine_TaskRemoveCallback((s32)SceneEffect_SpawnObject26EveryEightFrames);
    Battle_WaitMode0(120);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    WaitFrames(60);
    gFallingEffectWidth = 0;
    effect_phase = &gFallingEffectState;
    gFallingEffectOffset = 0x800000;
    *effect_phase = 1;
    Engine_TaskAddCallback((s32)FieldScene_UpdateFallingEffect, 0xc80);
    Battle_WaitMode0(180);
    Audio_PlayCue(21);
    Event_SayThenWait(ACTOR_GERALD, 80);
    Event_SayThenWait(ACTOR_IVAN, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
    Battle_WaitMode0(60);
    Event_SayThenWait(ACTOR_IVAN, 20);
    *effect_phase = 2;
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Battle_WaitMode0(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 1);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 3);
    Battle_WaitMode0(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Battle_WaitMode0(20);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_SayThenWait(ACTOR_GERALD, 20);
    if (*party_flag != 0) {
        Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
        Event_SayThenWait(ACTOR_MIA, 10);
    } else {
        gEventWork->message += 1;
    }
    formation_phase = &gFallingEffectState;
    *formation_phase = 3;
    *(u8 *)((void *)Object_GetById(0) + 35) &= 254;
    *(u8 *)((void *)Object_GetById(1) + 35) &= 254;
    *(u8 *)((void *)Object_GetById(2) + 35) &= 254;
    *(u8 *)((void *)Object_GetById(3) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpritePriority(ACTOR_GERALD, 3);
    Actor_SetSpritePriority(ACTOR_IVAN, 3);
    value = 0;
    Actor_SetSpritePriority(ACTOR_MIA, 3);
    KorimaHashi_TriggerPending = value;
    Engine_TaskAddCallback((s32)SceneActor_AlternateSlots13To16Field0c, 0xc80);
    Audio_PlayCue(220);
    *(u8 *)((void *)Object_GetById(13) + 35) &= 254;
    Actor_SetSpritePriority(13, 2);
    Actor_SetPosition(13, 0xfd0000, 0x25b0000);
    formation_action = (s32)KorimaHashi_FormationAction;
    Actor_EnableActionCallback(13, formation_action);
    *(u8 *)((void *)Object_GetById(14) + 35) &= 254;
    Actor_SetSpritePriority(14, 2);
    Actor_SetPosition(14, 0xe90000, 0x2750000);
    Actor_EnableActionCallback(14, formation_action);
    if (KorimaHashi_PartyFlag != 0) {
        *(u8 *)((void *)Object_GetById(15) + 35) &= 254;
        Actor_SetSpritePriority(15, 2);
        Actor_SetPosition(15, 0xcf0000, 0x2610000);
        Actor_EnableActionCallback(15, formation_action);
    }
    *(u8 *)((void *)Object_GetById(16) + 35) &= 254;
    Actor_SetSpritePriority(16, 2);
    Actor_SetPosition(16, 0xe30000, 0x2440000);
    Actor_EnableActionCallback(16, formation_action);
    if (*formation_phase != 0) {
        do {
            WaitFrames(1);
        } while (gFallingEffectState != 0);
    }
    Battle_WaitMode0(0x12c);
    Engine_TaskRemoveCallback((s32)FieldScene_UpdateFallingEffect);
    Battle_WaitMode0(120);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    WaitFrames(60);
    Actor_Stop(13);
    Actor_Stop(14);
    formation_flag = &KorimaHashi_PartyFlag;
    if (*formation_flag != 0) {
        Actor_Stop(15);
    }
    Actor_Stop(16);
    WaitFrames(1);
    finish_action = (s32)KorimaHashi_FinishAction;
    Actor_EnableActionCallback(13, finish_action);
    Actor_EnableActionCallback(14, finish_action);
    if (*formation_flag != 0) {
        Actor_EnableActionCallback(15, finish_action);
    }
    Object_SetActionCallbackAndRefreshById(16, finish_action);
    Battle_WaitMode0(80);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Battle_WaitMode0(40);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_SetPosition(11, 0xdc0000, 0x1ee0000);
    Actor_SetPosition(12, 0xdc0000, 0x1ee0000);
    WaitFrames(1);
    if (Event_ChooseYesNo(11, 0) == 1) {
        gEventWork->message += 1;
    }
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Battle_WaitMode0(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_SayThenWait(ACTOR_IVAN, 20);
    if (*formation_flag != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        Battle_WaitMode0(10);
        Event_SetMessage((s32)MsgKorimaKnowThoseFieldsWere);
        Event_SayThenWait(ACTOR_MIA, 40);
    }
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Battle_WaitMode0(80);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_SetMessage((s32)MsgKorimaWasOurPsynergy);
    Event_SayThenWait(ACTOR_IVAN, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Battle_WaitMode0(40);
    Actor_SetSpritePriority(ACTOR_GERALD, 2);
    *(u8 *)((void *)Object_GetById(1) + 35) |= 1;
    record = (void *)Object_GetById(1);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(ACTOR_GERALD, 6, 0);
    Actor_OffsetDestination(ACTOR_GERALD, -3, 0);
    Object_SetModeById(ACTOR_GERALD, 1);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 60);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_SayThenWait(ACTOR_GERALD, 10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Battle_WaitMode0(40);
    SceneActor_SetPairZeroAndValue(1, 0x6000, 40);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
    SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
    Actor_Jump(ACTOR_GERALD, 2, 0);
    Battle_WaitMode0(40);
    Actor_Jump(ACTOR_GERALD, 2, 0);
    Battle_WaitMode0(10);
    Actor_Jump(ACTOR_GERALD, 4, 0);
    Battle_WaitMode0(20);
    Event_SayThenWait(ACTOR_GERALD, 20);
    if (*formation_flag != 0) {
        Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
        Battle_WaitMode0(60);
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        Battle_WaitMode0(80);
        Actor_SetSpritePriority(ACTOR_MIA, 2);
        *(u8 *)((void *)Object_GetById(3) + 35) |= 1;
        record = (void *)Object_GetById(3);
        Actor_SetSpriteFlags((s32)record, 1);
        Actor_Jump(ACTOR_MIA, 4, 0);
        Actor_OffsetDestination(ACTOR_MIA, -2, 0);
        Object_SetModeById(ACTOR_MIA, 1);
        SceneActor_SetPairZeroAndValue(3, 0xe000, 60);
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        Battle_WaitMode0(20);
        Event_SayThenWait(ACTOR_MIA, 20);
    } else {
        gEventWork->message += 1;
    }
    Actor_Jump(ACTOR_GERALD, 2, 0);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Battle_WaitMode0(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Battle_WaitMode0(40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Battle_WaitMode0(20);
    value = 1;
    Actor_SetSpritePriority(ACTOR_IVAN, 2);
    *(u8 *)((void *)Object_GetById(2) + 35) |= value;
    record = (void *)Object_GetById(2);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(ACTOR_IVAN, 4, 0);
    Object_SetModeById(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Battle_WaitMode0(10);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    {
        u8 *record = (void *)Object_GetById(0);
        u8 flags = (u8)(value | record[35]);

        record[35] = flags;
    }
    record = (void *)Object_GetById(0);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Battle_WaitMode0(60);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
    SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SayThenWait(ACTOR_IVAN, 20);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Object_SetModeById(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Battle_WaitMode0(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Battle_WaitMode0(20);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Object_SetModeById(ACTOR_IVAN, 3);
    Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Object_SetModeById(ACTOR_IVAN, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        gEventWork->message += 1;
    } else {
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
        Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
        Event_ShowMessage(ACTOR_GERALD, 0);
    }
    SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_SayThenWait(ACTOR_GERALD, 20);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SayThenWait(ACTOR_IVAN, 10);
    if (KorimaHashi_PartyFlag != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        SceneActor_SetPairZeroAndValue(3, 0, 20);
        SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
        Object_SetModeById(ACTOR_MIA, 4);
        Event_SayThenWait(ACTOR_MIA, 10);
    } else {
        gEventWork->message += 1;
    }
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 10);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    value = 128;
    Battle_WaitMode0(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    SceneActor_SetPairZeroAndValue(1, (value << 7), 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Battle_WaitMode0(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Battle_WaitMode0(80);
    SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_SayThenWait(ACTOR_IVAN, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    SceneActor_SetPairZeroAndValue(0, 0xa000, 40);
    Actor_FaceDirection(ACTOR_GERALD, (value << 7), 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SayThenWait(ACTOR_IVAN, 10);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Battle_WaitMode0(40);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Battle_WaitMode0(20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Battle_WaitMode0(40);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Object_SetModeById(ACTOR_IVAN, 3);
    Event_SayThenWait(ACTOR_IVAN, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    } else {
        Battle_WaitMode0(20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Battle_WaitMode0(40);
        gEventWork->message += 1;
    }
    Event_ShowMessage(ACTOR_GERALD, 0);
    Audio_PlayCue(21);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    WaitFrames(60);
    gFallingEffectWidth = 0;
    gFallingEffectOffset = 0x800000;
    sequence_phase = &gFallingEffectState;
    *sequence_phase = 1;
    Engine_TaskAddCallback((s32)FieldScene_UpdateFallingEffect, 0xc80);
    Battle_WaitMode0(80);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Battle_WaitMode0(60);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
    Event_SetMessage((s32)MsgKorimaWatchOutItsHappeningAgain);
    Event_SayThenWait(ACTOR_IVAN, 10);
    SceneActor_SetPairZeroAndValue(1, 0xc000, 10);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
    sequence_flag = &KorimaHashi_PartyFlag;
    if (*sequence_flag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
    }
    *(u8 *)((void *)Object_GetById(0) + 35) &= 254;
    *(u8 *)((void *)Object_GetById(1) + 35) &= 254;
    *(u8 *)((void *)Object_GetById(2) + 35) &= 254;
    *(u8 *)((void *)Object_GetById(3) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpritePriority(ACTOR_GERALD, 3);
    Actor_SetSpritePriority(ACTOR_IVAN, 3);
    Actor_SetSpritePriority(ACTOR_MIA, 3);
    *sequence_phase = 2;
    Audio_PlayCue(220);
    Actor_SetPosition(13, 0xfd0000, 0x25b0000);
    formation_action = (s32)KorimaHashi_FormationAction;
    Actor_EnableActionCallback(13, formation_action);
    Actor_SetPosition(14, 0xe90000, 0x2750000);
    Actor_EnableActionCallback(14, formation_action);
    if (*sequence_flag != 0) {
        Actor_SetPosition(15, 0xcf0000, 0x2610000);
        Actor_EnableActionCallback(15, formation_action);
    }
    Actor_SetPosition(16, 0xe30000, 0x2440000);
    Actor_EnableActionCallback(16, formation_action);
    Battle_WaitMode0(120);
    *sequence_phase = 3;
    do {
        WaitFrames(1);
    } while (gFallingEffectState != 0);
    Event_SayThenWait(11, 80);
    Event_SayThenWait(12, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Battle_WaitMode0(60);
    Event_SayThenWait(12, 20);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Object_SetModeById(ACTOR_GERALD, 3);
    Object_SetModeById(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SayThenWait(12, 10);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Object_SetModeById(ACTOR_GERALD, 3);
    Object_SetModeById(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SayThenWait(12, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Battle_WaitMode0(40);
    Event_SayThenWait(11, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0, 0);
    SceneActor_SetPairZeroAndValue(2, 0xc000, 40);
    Event_SayThenWait(12, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneActor_SetPairZeroAndValue(3, 0xc000, 80);
    Event_SayThenWait(12, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneActor_SetPairZeroAndValue(3, 0, 40);
    Event_SayThenWait(11, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
    Object_SetModeById(ACTOR_PARTY_LEADER, 4);
    Object_SetModeById(ACTOR_GERALD, 4);
    Object_SetModeById(ACTOR_MIA, 4);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Battle_WaitMode0(60);
    Event_SayThenWait(12, 10);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Object_SetModeById(ACTOR_GERALD, 3);
    Object_SetModeById(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SayThenWait(12, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneActor_SetPairZeroAndValue(3, 0, 20);
    Event_SayThenWait(12, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Event_SayThenWait(12, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneActor_SetPairZeroAndValue(3, 0, 20);
    Event_SayThenWait(11, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Battle_WaitMode0(40);
    Event_SayThenWait(12, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
    Event_SayThenWait(12, 10);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Object_SetModeById(ACTOR_GERALD, 3);
    Object_SetModeById(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Battle_WaitMode0(60);
    Event_ShowMessage(12, 0);
    Event_ShowMessage(11, 0);
    Engine_TaskRemoveCallback((s32)FieldScene_UpdateFallingEffect);
    Battle_WaitMode0(80);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    WaitFrames(80);
    Actor_Stop(13);
    Actor_Stop(14);
    finish_flag = &KorimaHashi_PartyFlag;
    Actor_Stop(15);
    Actor_Stop(16);
    WaitFrames(1);
    finish_action = (s32)KorimaHashi_FinishAction;
    Actor_EnableActionCallback(13, finish_action);
    Actor_EnableActionCallback(14, finish_action);
    if (*finish_flag != 0) {
        Actor_EnableActionCallback(15, finish_action);
    }
    Object_SetActionCallbackAndRefreshById(16, finish_action);
    Battle_WaitMode0(20);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpritePriority(ACTOR_GERALD, 2);
    Actor_SetSpritePriority(ACTOR_IVAN, 2);
    value = 1;
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    *(u8 *)((void *)Object_GetById(0) + 35) |= value;
    *(u8 *)((void *)Object_GetById(1) + 35) |= value;
    *(u8 *)((void *)Object_GetById(2) + 35) |= value;
    {
        u8 *record = (void *)Object_GetById(3);
        u8 flags = (u8)(value | record[35]);

        record[35] = flags;
    }
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
    } else {
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Battle_WaitMode0(10);
        Event_OpenMessage(ACTOR_GERALD, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            SceneActor_SetPairZeroAndValue(3, 0, 20);
            Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
            Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
            Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
            Battle_WaitMode0(40);
            SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
            Event_SayThenWait(ACTOR_GERALD, 10);
            SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
            SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
            Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
            Event_SayThenWait(ACTOR_IVAN, 20);
            SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
        } else {
            SceneActor_SetPairZeroAndValue(3, 0, 20);
            Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
            Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
            Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
            Battle_WaitMode0(40);
            SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
            Event_SetMessage((s32)MsgKorimaHesAsStumpedAsWe);
            Event_SayThenWait(ACTOR_GERALD, 20);
            Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
            Event_SayThenWait(ACTOR_IVAN, 20);
        }
        Object_SetModeById(ACTOR_MIA, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        goto L_02002528;
    }
    Battle_WaitMode0(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Battle_WaitMode0(10);
    Event_SetMessage((s32)MsgKorimaYoureRobinThereIsntMuch);
    Event_SayThenWait(ACTOR_GERALD, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
    Object_SetModeById(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Battle_WaitMode0(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
    } else {
        Battle_WaitMode0(20);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 0);
        Battle_WaitMode0(40);
        SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
        Event_SayThenWait(ACTOR_IVAN, 10);
        if (*finish_flag != 0) {
            SceneActor_SetPairZeroAndValue(3, 0, 10);
            Actor_StartRepeatedMotion(ACTOR_MIA, 3);
            Event_SayThenWait(ACTOR_MIA, 20);
        } else {
            gEventWork->message += 1;
        }
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
        Battle_WaitMode0(40);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_SayThenWait(ACTOR_GERALD, 20);
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
        Battle_WaitMode0(120);
        Event_SayThenWait(ACTOR_IVAN, 40);
        if (KorimaHashi_PartyFlag != 0) {
            SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
            Actor_SetAnimationAndWait(ACTOR_MIA, 4);
            Event_SayThenWait(ACTOR_MIA, 10);
        } else {
            gEventWork->message += 1;
        }
        Battle_WaitMode0(60);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
        if (KorimaHashi_PartyFlag != 0) {
            SceneActor_SetPairZeroAndValue(2, 0xa000, 40);
            SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
        }
        Event_SayThenWait(ACTOR_IVAN, 10);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Battle_WaitMode0(40);
        Event_SayThenWait(ACTOR_IVAN, 20);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Battle_WaitMode0(20);
        Object_SetModeById(ACTOR_MIA, 3);
        L_02002528:;
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        goto L_02002660;
    }
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Battle_WaitMode0(40);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SetMessage((s32)MsgKorimaThatsReliefRobinThoughtYoud);
    Event_SayThenWait(ACTOR_IVAN, 20);
    if (*finish_flag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 1);
        Event_SayThenWait(ACTOR_MIA, 20);
    } else {
        gEventWork->message += 1;
    }
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Battle_WaitMode0(40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Battle_WaitMode0(80);
    Event_SayThenWait(ACTOR_IVAN, 40);
    if (KorimaHashi_PartyFlag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0x2000, 20);
        Object_SetModeById(ACTOR_MIA, 4);
        Event_SayThenWait(ACTOR_MIA, 40);
    } else {
        gEventWork->message += 1;
    }
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Battle_WaitMode0(20);
    Event_SayThenWait(ACTOR_IVAN, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Battle_WaitMode0(40);
    Event_SayThenWait(ACTOR_IVAN, 20);
    L_02002660:;
    Audio_PlayCue(17);
    Actor_SetMotionSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetMotionSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Actor_SetMotionSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Object_SetModeById(ACTOR_GERALD, 2);
    {
        u8 *record = (void *)Object_GetById(0);

        if (record != 0) {
            Actor_SetDestination(ACTOR_GERALD, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Object_SetModeById(ACTOR_IVAN, 2);
    {
        u8 *record = (void *)Object_GetById(0);

        if (record != 0) {
            Actor_SetDestination(ACTOR_IVAN, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    if (KorimaHashi_PartyFlag != 0) {
        Object_SetModeById(ACTOR_MIA, 2);
        {
            u8 *record = (void *)Object_GetById(0);

            if (record != 0) {
                Actor_SetDestination(ACTOR_MIA, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
            }
        }
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
        Actor_SetPosition(ACTOR_MIA, 0, 0);
    }
    GameFlag_Set(0x843);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Audio_PlayCueFromEventWork();
    Event_End();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Battle_WaitMode0(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceDirection(a, b, 0);
    Battle_WaitMode0(c);
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
    if (KorimaHashi_SparkleSound != 0) Audio_PlayCue(200);
    obj = (struct Obj *)Object_Create(26, c1, 0, c2);
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
    Object_SetAnimation((struct FieldActor *)obj, 2);
    Object_SetPosition(obj, c3, 0, c4);
    Object_SetScript((struct FieldActor *)obj, (const s32 *)KorimaHashi_Object26Script);
}

s32 OverlayObject_SelectValueByFrameBit1(s32 obj)
{

    if (((gFrameCount >> 1) & 1) != 0) {
        Object_SetPalette(obj, 10);
    } else {
        Object_SetPalette(obj, 7);
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
    Audio_PlayCue(106);
    ((s32 (*)())Engine_ObjectSetScript)((s32)arg0, (s32)KorimaHashi_TriggerScript);
    KorimaHashi_TriggerPending = 1;
    return 0;
}

