#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "LOG_ROLLING.H"
#include "CALL.H"
#include "FIELD_SERVICE.H"


void ColossoLogRollingStage_SceneTask(void)
{
    struct EventWork *work = gEventWork;
    struct FieldActor *actor = Object_GetById(gGameState.selected_actor);
    s32 row = actor->z.fixed >> 20;
    s32 i;
    s32 kind;

    if (gColossoSceneTaskState == 0) {
        gColossoSceneTaskStatus = (gColossoSceneTaskStatus + 1) & 3;
        for (i = 18; i <= 22; i++) {
            kind = KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][i - 18];
            Engine_ActorSetAnimation(i, kind);
            Engine_ActorSetAnimation(i + 5, kind + 8);
            Engine_MapCopyCellAttributes(32, 11, 1, 2, (i - 18) * 2 + 33, 11);
            if (kind != 7) {
                Engine_MapCopyCellAttributes(74, 12, 1, 1, (i - 18) * 2 + 33, 11);
            }
        }
        Engine_ActorSetAnimation(28, KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][5]);
    } else {
        for (i = 18; i <= 22; i++) {
            kind = KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][i - 18];
            if ((u32)(actor->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe) {
                if (row == 11 && kind == 4)
                    work->raised_trigger = kind;
                if (row == 12 && kind == 5)
                    work->raised_trigger = kind;
            }
        }
    }
    if ((u32)++gColossoSceneTaskState > 17) {
        gColossoSceneTaskState = 0;
    }
}

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "COLOSSO_LOG_ROLLING_STAGE.H"
#include "FLAG_IDS.H"
#include "text/MSG_IDS.H"


void Object_RefreshSelectorById(s32 object);
void BattleFx_SetWeightedResult(s32 kind, s32 weight);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetDefeatDestination(s32 scene, s32 entrance);


extern char ResourceId_RivalPathC;
s32 Engine_GameFlagIsSet();
s32 GameFlag_GetByte();
s32 Engine_TaskAddCallback();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsTo();
s32 Map_GetTerrainHeight();
void Engine_ActorSetSpriteFlags();
void Object_SetMode();
void Engine_ObjectSetScript();
void Map_SetLayerEntryFlag();
void Engine_ActorSetAnimation();
void Engine_ActorDestroy();
void Engine_ActorSetChildValue();
void Object_LinkObjectAndSetCallback();
void BattleEffect_PauseObject();
void Engine_AudioPlayCue();
void Engine_EventRequestExit();
s32 Korosseo_ShowItemIcon();
void ColossoLogRollingStage_InitializeSceneControl();
void ColossoLogRollingStage_RestoreActorPositions();
void Korosseo_SelectSoloCompetitor();
void ColossoLogRollingStage_RunScriptedTransition();
void FieldScene_RunMultiPhaseActorSequence();
void Korosseo_RunGreetScene();
void ColossoLogRollingStage_ClearSavedActorPositions();


void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_CameraFollowActor();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventSetMessage();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessage();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorSetDestination();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_EventEnd();

void Korosseo_FinishSoloRound();
s32 FieldScene_RunMiddleSequence();
void Object_SetPosition();
void Object_CommitPosition();


void Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
void Korosseo_RestoreCompetitor();

void ColossoLogRollingStage_ResetAndRunSceneTask(void)
{
    SceneTaskEntry entry;

    gColossoSceneTaskState = 0;
    entry = ColossoLogRollingStage_SceneTask;
    gColossoSceneTaskStatus = 0;
    Engine_TaskRemoveCallback(entry);
    entry();
}

void ColossoLogRollingStage_StartSceneTask(void)
{
    Engine_TaskAddCallback(ColossoLogRollingStage_SceneTask, 0xC80);
}

void ColossoLogRollingStage_WaitForSceneTask(void)
{
    s32 polls;

    Engine_TaskWait(10);

    polls = 0;
    while (gColossoSceneTaskStatus != 3 || gColossoSceneTaskState != 1) {
        Engine_TaskWait(1);
        polls++;
        if (polls > 119) {
            return;
        }
    }
}

void ColossoLogRollingStage_NudgeActorsLeft(void)
{
    u8 *workspace = (u8 *)gEventWork;
    s16 *table = (s16 *)&gGameState;
    s32 id = ((struct GameState *)table)->selected_actor;
    StageObstacleActor *subject = (StageObstacleActor *)((struct EventWork *)workspace)->view_center;
    StageObstacleActor *actor = Object_GetById(id);
    s32 z = ((struct FieldActor *)actor)->z.part.pixel;

    if ((u32)(z - 183) <= 3) {
        subject->x += -0xcccc;
        actor->x += -0xcccc;
    }
}

void ColossoLogRollingStage_ConfigureGridRegion(void)
{
    Engine_GameFlagSet(0x360);
    {
        s32 width = 49;
        s32 height = 61;

        Engine_MapCopyCellAttributes(47, 61, 1, 4, width, height);
    }
}

void ColossoLogRollingStage_ConfigurePrimaryObjects(void)
{
#ifdef MODERN
    // modern GCC holds one type for a name in a file
    extern void Object_SetPosition_modern(PrimaryStageObject *, s32, s32, s32) __asm__("Object_SetPosition");
#define Object_SetPosition Object_SetPosition_modern
#else
    extern void Object_SetPosition(PrimaryStageObject *, s32, s32, s32);
#endif

    PrimaryStageObject *object;

    object = Object_GetById(9);
    object->scale_x = 0x10000;
    object->scale_z = 0x10000;

    object = Object_GetById(11);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Object_SetPosition(object, object->x, 0x200000, object->y);

    object = Object_GetById(10);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Object_SetPosition(object, object->x, 0x40000, object->y);

    Engine_GameFlagSet(0x362);
    Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, 12);
    Engine_MapCopyCellAttributes(14, 12, 1, 1, 9, 12);
}
#ifdef MODERN
#undef Object_SetPosition
#endif

void FieldScene_RunClosingAuxiliarySequence(void)
{
    extern void Object_SetPosition();

    s32 actor;
    s32 lowered;
    struct FieldActor *log;
    struct FieldActor *rider;
    u8 *flags;

    actor = gGameState.selected_actor;
    lowered = GameFlag_IsSet(0x362);
    if (lowered == 0) {
        log = Object_GetById(10);
        if (log != NULL) {
            Actor_SetDestination(actor, log->x.part.pixel, log->z.part.pixel);
        }
        Engine_ActorWaitForMove(actor);
        log = Object_GetById(11);
        log->motion_flags = lowered;
        log->acceleration = 0x6666;
        log->speed = 0xcccc;
        Object_SetPosition(log, log->x.fixed, 0x200000, log->z.fixed);
        log = Object_GetById(10);
        log->motion_flags = lowered;
        log->acceleration = 0x6666;
        log->speed = 0xcccc;
        Call4(Object_SetPosition, (s32)log, log->x.fixed, 0x40000, log->z.fixed);
        rider = Object_GetById(actor);
        flags = &rider->motion_flags;
        *flags = lowered;
        rider->acceleration = 0x6666;
        rider->speed = 0xcccc;
        Object_SetPosition(rider, rider->x.fixed, 0x40000, rider->z.fixed);
        Engine_ActorSetSpriteFlags(rider, 1);
        Engine_ActorWaitForMove(actor);
        Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
        Engine_TaskWait(2);
        Engine_ActorSetSpriteFlags(rider, 1);
        *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        rider->terrain_height = rider->y.fixed;
        Engine_GameFlagSet(0x367);
    }
}

void ColossoLogRollingStage_ConfigureSecondaryObjects(void)
{
    extern void Object_SetPosition();

    s16 *table;
    SecondaryStageObject *object;

    table = (s16 *)&gGameState;

    Engine_ActorSetAnimation(*(s32 *)&table[250], 1);

    object = Object_GetById(11);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Object_SetPosition(object, object->x, 0x40000, object->z);

    object = Object_GetById(10);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Object_SetPosition(object, object->x, 0x200000, object->z);

    Engine_ActorWaitForMove(10);
    {
        s32 stack_first = 9;
        s32 stack_second = 12;
        Engine_MapCopyCellAttributes(0, 25, 1, 1, stack_first, stack_second);
    }
    Engine_TaskWait(2);
    Engine_GameFlagClear(0x367);
}

void FieldScene_RunFinalAuxiliarySequence(void)
{
    extern void Object_SetPosition();

    struct FieldActor *probe;
    struct FieldActor *log;
    struct FieldActor *upper;
    struct FieldActor *lower;
    s32 x;
    s32 z;

    probe = Object_GetById(12);
    x = probe->x.fixed >> 20;
    if (x == 9) {
        z = probe->z.fixed >> 20;
        if (z == 12) {
            log = Object_GetById(12);
            Engine_ActorSetSpriteFlags(log, 0);
            log->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            log->motion_flags = 0;
            log->acceleration = 0x6666;
            log->speed = 0xcccc;
            Call4(Object_SetPosition, (s32)log, log->x.fixed, 0x40000, log->z.fixed);
            upper = Object_GetById(11);
            upper->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            upper->acceleration = 0x6666;
            upper->speed = 0xcccc;
            Object_SetPosition(upper, upper->x.fixed, 0x200000, upper->z.fixed);
            lower = Object_GetById(10);
            lower->acceleration = 0x6666;
            lower->speed = 0xcccc;
            Object_SetPosition(lower, lower->x.fixed, 0x40000, lower->z.fixed);
            Engine_GameFlagSet(0x368);
            Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, z);
            Engine_MapCopyCellAttributes(1, 25, 1, 1, x, z);
        }
    }
}

void ColossoLogRollingStage_RunSetupCompletionHooks(void)
{
    ColossoLogRollingStage_PushStagedActor();
    FieldScene_RunFinalAuxiliarySequence();
}

void ColossoLogRollingStage_ConfigureActorThirteen(void)
{
    extern void GameFlag_SetByte(s32, s32);

    StageObstacleActor *actor;
    s32 x;

    actor = Object_GetById(13);
    x = actor->x >> 20;
    GameFlag_SetByte(880, x);
    Engine_MapCopyCellAttributes(18, 10, 3, 1, 18, 11);
    Engine_MapCopyCellAttributes(17, 11, 1, 1, x, 11);
}

void ColossoLogRollingStage_NoopSetupHook(void)
{
}

void ColossoLogRollingStage_RunSetupHook(void)
{
    ColossoLogRollingStage_PushStagedActor();
}

void ColossoLogRollingStage_ActivateClearObstacleActors(void)
{
    extern s32 Map_GetTerrainHeight(s32, s32, s32);

    StageObstacleActor *actor;
    s32 slot;
    s32 x;
    s32 z;
    s32 x2;
    s32 z2;

    for (slot = 15; slot <= 17; slot++) {
        actor = Object_GetById(slot);
        if (Map_GetTerrainHeight(0, actor->x, actor->z) == 0) {
            actor->direction_and_kind = 2;
            actor->state = 0;
            x = actor->x >> 20;
            z = actor->z >> 20;
            Engine_MapCopyCellAttributes(83, 13, 1, 1, x, z);
            x2 = actor->x >> 20;
            z2 = actor->z >> 20;
            Engine_MapCopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
            Engine_GameFlagSet(slot + 517);
        }
    }
}

void ColossoLogRollingStage_ShowActorPositionMessage(void)
{
    extern void SetMapCellCollision(s32, s32, s32, s32);

    StageObstacleActor *actor;
    s16 *table;
    s32 x;
    s32 z;
    s32 message_id;

    table = (s16 *)&gGameState;
    actor = Object_GetById(*(s32 *)&table[250]);
    x = actor->x >> 20;
    message_id = 23;
    z = actor->z >> 20;
    if (x == 81 && z == 12) {
        if ((actor->attributes & 0xE000) == 0x4000) {
            message_id = 253;
        }
        SetMapCellCollision(0, x << 20, z << 20, message_id);
    }
}

s32 ColossoLogRollingStage_CheckObstacleDestination(s32 x, s32 z)
{
    extern s32 GetMapCellCollision(s32, s32, s32);

    StageObstacleActor *actor;

    if (GetMapCellCollision(0, x, z) == 255) {
        return -2;
    }
    actor = Object_GetById(15);
    x = x >> 20;
    z = z >> 20;
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Object_GetById(16);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Object_GetById(17);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    return 0;
}

s32 ColossoLogRollingStage_CheckPathClearance(s32 x, s32 y)
{
    if (ColossoLogRollingStage_CheckObstacleDestination(x, y - 0x180000) != 0
     || ColossoLogRollingStage_CheckObstacleDestination(x, y - 0x80000) != 0
     || ColossoLogRollingStage_CheckObstacleDestination(x, y + 0x80000) != 0
     || ColossoLogRollingStage_CheckObstacleDestination(x, y + 0x180000) != 0) {
        return -1;
    }
    return 0;
}

void FieldScene_RunEarlySequence(void)
{
    extern void Vector_AddPolarOffset();
    extern void Object_SetPosition();
    extern void ObjectDispatch_InitFromTable4WithArgument();
    extern void Object_CommitPosition();

    extern u16 gColossoEarlySequenceData[];
    struct FieldActor *rider;
    struct FieldActor *log;
    struct EventWork *work;
    s32 angle;
    s32 actor;
    s32 x;
    s32 z;
    s32 steps;
    s32 next_x;
    s32 column;
    s32 acceleration;
    s32 mode;
    s32 step;
    s32 ahead[3];

    rider = Object_GetById(gGameState.selected_actor);
    log = Object_GetById(31);
    steps = 0;
    angle = gColossoEarlySequenceData[rider->facing >> 13];
    actor = gGameState.selected_actor;
    ahead[0] = (rider->x.fixed & -0x100000) + 0x80000;
    ahead[1] = rider->y.fixed;
    ahead[2] = (rider->z.fixed & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x100000, angle, ahead);
    x = log->x.fixed;
    z = log->z.fixed;
    if ((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > 0x80000
        || (ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > 0x200000) {
        goto far;
    }
    if (gKeysHeld & KEY_LEFT) {
        mode = 2;
        step = -8;
        for (;;) {
            next_x = x - 0x100000;
            if (ColossoLogRollingStage_CheckPathClearance(next_x, z) != 0)
                goto moved;
            steps++;
            x = next_x;
        }
    }
    if (!(gKeysHeld & KEY_RIGHT)) {
        return;
    }
    mode = 3;
    step = 8;
    for (;;) {
        next_x = x + 0x100000;
        if (ColossoLogRollingStage_CheckPathClearance(next_x, z) != 0)
            goto moved;
        steps++;
        x = next_x;
    }
moved:
    if (steps == 0) {
        return;
    }
    Map_CopyCellAttributes(74, 8, 1, 4, log->x.fixed >> 20, 9);
    Map_CopyCellAttributes(120, 60, 8, 5, 74, 60);
    Engine_EventBegin();
    Engine_ActorSetAnimation(actor, 8);
    Engine_EventWait(6);
    log->speed = 0x8000;
    acceleration = 0x3333;
    log->acceleration = acceleration;
    Object_SetMode(log, mode);
    Object_SetPosition(log, x, 0, z);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(actor, 2);
    work = Runtime_AllocateBlock(27, 0xccc);
    ObjectDispatch_InitFromTable4WithArgument(work->view_center, log);
    Actor_SetSpeed(actor, 0x8000, acceleration);
    Audio_PlayCue(239);
    Object_SetMode(rider, 2);
    Object_SetPosition(rider, ((steps * step) << 16) + rider->x.fixed, 0, rider->z.fixed);
    Object_CommitPosition(rider);
    Object_SetMode(rider, 1);
    Object_CommitPosition(log);
    if (x >= 0x5300000) {
        GameFlag_Set(0x369);
        Engine_ActorSetAnimation(31, 3);
        Actor_SetDestinationOffset(31, 18, 6);
        Engine_EventWait(30);
        Object_SetMode(log, 8);
        Object_CommitPosition(log);
        log->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        column = 84;
        Map_CopyCellAttributes(86, 10, 1, 2, column, 10);
        Map_CopyCellAttributes(86, 9, 1, 1, column, 12);
        Audio_PlayCue(0x120);
        Audio_PlayCue(240);
    } else {
        Object_SetMode(log, 1);
        Audio_PlayCue(0x120);
        Audio_PlayCue(213);
        column = x >> 20;
        Map_CopyCellAttributes(85, 9, 1, 4, column, 9);
        Map_CopyCellAttributes(85, 9, 1, 4, column, 61);
    }
    Engine_EventWait(15);
    Engine_EventEnd();
    return;
far:
    ColossoLogRollingStage_PushStagedActor();
    ColossoLogRollingStage_ActivateClearObstacleActors();
}

s32 ColossoLogRollingStage_SetSceneEventValues(void)
{
    extern void Map_SetLayerEntryFlag(s32);
    extern void Map_ClearLayerEntryFlag(s32);

    Map_SetLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(217);
    return 0;
}

void ColossoLogRollingStage_ConfigureSceneEventEffect(void)
{
    StageMotionEffect *effect;
    s32 move_rate;

    effect = Object_GetById(30);
    effect->state = 0;
    move_rate = 0x19999;
    effect->move_rate_z = move_rate;
    effect->move_rate_x = move_rate;
    Object_SetMode(effect, 2);
    Engine_ObjectSetScript(effect, (s32)gColossoSceneEventEffect);
    Engine_GameFlagSet(0x363);
}

void ColossoLogRollingStage_WaitForSceneEventTask(void)
{
    extern void BattleEffect_PauseObject(s32);

    BattleEffect_PauseObject(28);
    Engine_GameFlagSet(0x361);
    Engine_TaskWait(10);
    while (gColossoSceneTaskStatus != 1 && gColossoSceneTaskStatus != 3)
        Engine_TaskWait(1);
    Engine_TaskWait(1);
    Engine_TaskRemoveCallback(ColossoLogRollingStage_SceneTask);
}

void ColossoLogRollingStage_OffsetActiveActor(void)
{
    extern void Object_SetPosition(StageObstacleActor *, s32, s32, s32);
    extern void Object_CommitPosition(StageObstacleActor *);

    StageObstacleActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = (s16 *)&gGameState;
    slot = (s32 *)&table[250];
    actor = Object_GetById(*slot);
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Engine_ActorSetAttachedEffect(*slot, 258);
    Object_SetMode(actor, 5);
    z = actor->z & 0xFFF00000;
    Object_SetPosition(actor, actor->x, actor->y, z + 0x180000);
    Object_CommitPosition(actor);
}

void ColossoLogRollingStage_ClampAndOffsetActiveActor(void)
{
    extern void Object_SetPosition(StageObstacleActor *, s32, s32, s32);
    extern void Object_CommitPosition(StageObstacleActor *);
    extern void BattleFx_RunRisingObjectSequence(s32, s32, s32);

    StageObstacleActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = (s16 *)&gGameState;
    slot = (s32 *)&table[250];
    actor = Object_GetById(*slot);
    if (actor->x > 0x2980000) {
        actor->x = 0x2980000;
    }
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Object_SetMode(actor, 5);
    z = actor->z & 0xFFF00000;
    Object_SetPosition(actor, actor->x, actor->y, z + 0xC0000);
    Object_CommitPosition(actor);
    Engine_ActorSetAttachedEffect(*slot, 258);
    BattleFx_RunRisingObjectSequence(*slot, 6, 0);
}

void ColossoLogRollingStage_NoopSceneEventHook(void)
{
}

void KorosseoMaruta_RunCompetitorTalk(s32 a0)
{
    struct EventWork *work;
    s32 owner;
    s32 msg;
    s32 lines;
    s32 entrance;

    work = gEventWork;
    owner = gGameState.selected_actor;
    entrance = gGameState.entrance;
    if (entrance == 2) {
        Engine_EventBegin();
        msg = (s32)MsgKorosseoDidntThinkBattles;
        lines = a0 * 3;
        Engine_EventSetMessage(lines + msg);
        Engine_EventOpenMessage(a0, 0);
        if (Engine_EventChooseYesNo(owner, 0) == 0) {
            /* FAKEMATCH: each answer's line goes through its own local, here and below, which keeps the reference's addition order. */
            s32 yes = msg + 1;
            Engine_EventSetMessage(lines + yes);
            Engine_EventShowMessage(a0, 0);
            work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
            work->transition_frames = 15;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
            Korosseo_SelectSoloCompetitor(a0);
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
        } else {
            s32 no = msg + 2;
            Engine_EventSetMessage(lines + no);
            Engine_EventShowMessage(a0, 0);
        }
        Engine_EventEnd();
    }
}

void KorosseoMaruta_RunStageStart(void)
{
    s32 place;
    s32 i;
    s32 last;

    /* FAKEMATCH: the last place is set at the top as a forced temporary, so the weight stays 4 - place + 1 instead of folding to 5 - place. */
    last = 4;
    ColossoLogRollingStage_NoopSceneHook();
    Engine_EventBegin();
    place = ColossoLogRollingStage_PositionActiveActor(3, 17);
    ColossoLogRollingStage_WaitForBalanceState();
    for (i = 0; i < 10; i++) {
        Object_RefreshSelectorById(8);
    }
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkTo(8, 0x5f8, 192);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x5d8, 192);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(8, 0x20000, 0x10000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x5e0, 192);
    Actor_WalkToAndWait(8, 0x5f0, 192);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Engine_ActorSetAnimation(8, 9);
    Engine_EventWait(10);
    BattleFx_SetWeightedResult(72, last - place + 1);
    /* FAKEMATCH: the do/while keeps the stage flag store ahead of the scene load that follows it. */
    do {
        gGameState.battle_start = 3;
    } while (0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoMaruta, 4);
    Event_SetDefeatDestination((s32)&SceneId_KorosseoMaruta, 5);
    Engine_GameFlagSet(0x11a);
}

void ColossoLogRollingStage_RunSceneEventIfReady(void)
{
    if (ColossoLogRollingStage_FindActorAhead() == 0) {
        Engine_LeaderCheckAhead();
    }
}

void ColossoLogRollingStage_FinishOrContinueSceneEvent(void)
{
    if (ColossoLogRollingStage_FindActorAhead() == 0) {
        Engine_LeaderCheckAhead();
    } else {
        ColossoLogRollingStage_RunSetupCompletionHooks();
    }
}

s32 ColossoLogRollingStage_GetSceneEventState(void)
{
    return (s32)gKorosseoMarutaEvents;
}

s32 StageSetup_BuildAndDispatch(void)
{
    struct FieldActor *actor;
    s32 i;
    s32 x;
    s32 height;
    s32 column;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_GameFlagSet(324);
    Engine_TaskAddCallback(ColossoLogRollingStage_ShowActorPositionMessage, 3200);
    Map_CopyCellAttributes(74, 60, 8, 6, 120, 60);

    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);

    actor = Object_GetById(10);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->motion_flags = 0;
    actor->y.fixed = 32 << 16;

    actor = Object_GetById(11);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->motion_flags = 0;
    actor->y.fixed = 4 << 16;

    if (Engine_GameFlagIsSet(866) != 0) {
        Engine_ActorSetAnimation(9, 5);
        actor = Object_GetById(10);
        actor->y.fixed = 4 << 16;
        actor = Object_GetById(11);
        actor->y.fixed = 32 << 16;
        Map_CopyCellAttributes(15, 12, 1, 1, 13, 12);
    } else {
        actor = Object_GetById(9);
        actor->scale_x = 0x18000;
        actor->scale_y = 0x18000;
        if (Engine_GameFlagIsSet(871) != 0) {
            Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
        } else {
            Map_CopyCellAttributes(0, 25, 1, 1, 9, 12);
        }
    }

    if (Engine_GameFlagIsSet(872) != 0) {
        Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, 12);
        Engine_MapCopyCellAttributes(1, 25, 1, 1, 9, 12);
        actor = Object_GetById(12);
        Engine_ActorSetSpriteFlags(actor, 0);
        actor->motion_flags = 0;
        actor->y.fixed = 2 << 16;
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        actor = Object_GetById(10);
        actor->y.fixed = 4 << 16;
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        actor = Object_GetById(11);
        actor->y.fixed = 32 << 16;
    }

    column = GameFlag_GetByte(880);
    if (column == 0) {
        column = 19;
    }
    actor = Object_GetById(13);
    actor->x.fixed = (column << 20) + (8 << 16);
    actor->motion_flags = 0;
    actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(18, 10, 3, 1, 18, 11);
    Engine_MapCopyCellAttributes(17, 11, 1, 1, column, 11);

    for (i = 15; i <= 17; i++) {
        actor = Object_GetById(i);
        height = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
        if (actor->y.fixed == 0 && height == 0) {
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            actor->motion_flags = height;
            Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
            Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20, (actor->z.fixed >> 20) + 52);
        }
    }

    if (Engine_GameFlagIsSet(865) != 0) {
        s32 still;
        s32 underfoot;
        s32 row;

        /* FAKEMATCH: the loop's constants live in locals so they stay in r8-sl across the calls, as native keeps them; written inline they do not. */
        i = 18;
        still = 0;
        underfoot = ACTOR_PRIORITY_UNDERFOOT;
        x = 33;
        row = 11;
        for (; i <= 22; i++) {
            actor = Object_GetById(i);
            actor->priority_flags = underfoot;
            Object_SetMode(actor, 2);
            actor = Object_GetById(i + 5);
            actor->priority_flags = underfoot;
            actor->motion_flags = still;
            actor->y.fixed = 32 << 16;
            Object_SetMode(actor, 10);
            Engine_MapCopyCellAttributes(74, 12, 1, 1, x, row);
            x += 2;
        }
        Engine_ActorSetAnimation(28, 10);
        BattleEffect_PauseObject(28);
    } else {
        for (i = 18; i <= 22; i++) {
            actor = Object_GetById(i);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            actor = Object_GetById(i + 5);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            actor->motion_flags = 0;
            actor->y.fixed = 32 << 16;
        }
        Engine_TaskAddCallback(ColossoLogRollingStage_SceneTask, 3200);
    }

    if (Engine_GameFlagIsSet(864) != 0) {
        Engine_ActorSetAnimation(29, 4);
        Map_CopyCellAttributes(47, 61, 1, 4, 49, 61);
    }

    if (Engine_GameFlagIsSet(867) != 0) {
        Map_SetLayerEntryFlag(1);
        actor = Object_GetById(30);
        actor->motion_flags = 0;
        actor->x.fixed = 1130 << 16;
        actor->z.fixed = 184 << 16;
        Engine_ActorSetSpriteFlags(actor, 0);
        Object_SetMode(actor, 3);
        Engine_ObjectSetScript(actor, KorosseoMaruta_Record30Script);
    } else {
        Map_SetLayerEntryFlag(2);
    }

    if (Engine_GameFlagIsSet(873) != 0) {
        actor = Object_GetById(31);
        Object_SetMode(actor, 8);
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_MapCopyCellAttributes(86, 10, 1, 2, 84, 10);
        Engine_MapCopyCellAttributes(86, 9, 1, 1, 84, 12);
    } else {
        actor = Object_GetById(31);
        Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 9);
        Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 61);
    }

    actor = Object_GetById(9);
    actor->motion_flags = 0;
    actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Object_GetById(10);
    actor->motion_flags = 0;
    actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Object_GetById(11);
    actor->motion_flags = 0;
    actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetAnimation(8, 9);
    gGameState.movement_mode = 0;
    Korosseo_ShowItemIcon(39, 3);
    Korosseo_ShowItemIcon(40, 17);
    Engine_ActorSetChildValue(8, 2);

    switch (gGameState.entrance) {
    case 1:
        /* FAKEMATCH: called as returning s32 it sets r0 before the constants, as native does; the void call sets it after them. */
        ((s32 (*)())ColossoLogRollingStage_SetupSceneDescriptor)(0, 8, 6, 1512 << 16, 192 << 16, 39, 40);
        Map_CopyCellsTo(127, 0, 1, 2, 5, 2);
        Engine_ActorDestroy(32);
        Engine_ActorDestroy(33);
        Engine_ActorDestroy(34);
        Engine_ActorDestroy(35);
        Engine_ActorDestroy(36);
        Engine_ActorDestroy(37);
        Engine_ActorDestroy(38);
        if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
            Engine_AudioPlayCue(17);
            Korosseo_SelectSoloCompetitor(0);
            ColossoLogRollingStage_RestoreActorPositions();
            Object_LinkObjectAndSetCallback(1, 0);
            ColossoLogRollingStage_RunScriptedTransition(3);
        }
        Object_LinkObjectAndSetCallback(1, 0);
        Object_LinkObjectAndSetCallback(2, 0);
        Object_LinkObjectAndSetCallback(3, 0);
        ColossoLogRollingStage_InitializeSceneControl((s32)&ResourceId_RivalPathC);
        break;

    case 2:
        Engine_TaskAddCallback(ColossoLogRollingStage_MarkSceneProgress, 3200);
        Engine_ActorDestroy(39);
        Engine_ActorDestroy(40);
        if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
            break;
        }
        ColossoLogRollingStage_RestoreActorPositions();
        Korosseo_SelectSoloCompetitor(1);
        ColossoLogRollingStage_RunScriptedTransition(0);
        break;

    case 3:
        if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
            break;
        }
        Korosseo_RunGreetScene(32);
        ColossoLogRollingStage_ClearSavedActorPositions();
        break;

    case 4:
        FieldScene_RunMultiPhaseActorSequence(1);
        Engine_EventRequestExit(4);
        Engine_GameFlagSet(2384);
        Engine_GameFlagSet(2385);
        break;

    case 5:
        FieldScene_RunMultiPhaseActorSequence(-1);
        Engine_EventRequestExit(5);
        Engine_GameFlagSet(2384);
        break;
    }
    return 0;
}

s32 ColossoLogRollingStage_AdvanceParticleMotion(SceneParticle *particle)
{
    particle->x += particle->velocity_x << 8;
    particle->y += particle->velocity_y << 8;
    particle->scale_x += 0x666;
    particle->scale_y += 0x666;
    particle->velocity_x += 5;
    particle->velocity_y -= 1;
    return 0;
}

void ColossoLogRollingStage_SpawnPeriodicParticle(void)
{

    SceneParticle *particle;
    SceneParticle *source;
    s32 x;
    s32 y;
    s32 kind;
    s32 count;

    particle = Object_GetById(0);
    count = gColossoParticleCount + 1;
    kind = 41;
    x = particle->x;
    y = particle->y;
    gColossoParticleCount = count;
    switch (count % 180) {
    case 10:
        break;
    case 20:
        kind = 42;
        break;
    case 30:
        kind = 43;
        break;
    default:
        return;
    }
    particle = Object_GetById(kind);
    if (particle == 0) {
        return;
    }
    source = Object_GetById(0);
    if (source != 0) {
        Engine_ActorSetPosition(kind, source->x, source->z);
    }
    Engine_ActorSetSpriteFlags(Object_GetById(kind), 0);
    particle->state = 0;
    particle->scale_x = 0x6666;
    particle->scale_y = 0x6666;
    {
        s32 t = 0x40000;
        particle->x = x + t;
        t += y;
        particle->y = t;
        particle->anchor_y = t;
    }
    particle->velocity_x = 25;
    particle->velocity_y = 128;
    Engine_ActorEnableActionCallback(kind, (s32)gColossoParticleKinds);
}

void FieldScene_RunMultiPhaseActorSequence(s32 a0)
{
    extern void Object_LinkObjectAndSetCallback();

    extern void Owner_RefreshActiveRatios();
    extern void Graphics_EnableObjLayerAndCallbacks();

    struct FieldActor *record;
    s32 data_table_addr;
    s32 callback_target;

    Engine_ActorDestroy(39);
    Engine_ActorDestroy(40);
    Owner_RefreshActiveRatios(1);
    Engine_AudioPlayCue(17);
    Engine_EventBegin();
    Actor_SetPosition(8, 0x6080000, 0xc00000);
    if (a0 < 0) {
        Engine_ActorSetAnimation(8, 10);
    } else {
        Engine_ActorSetAnimation(8, 8);
    }
    Engine_ActorEnableActionCallback(8, KorosseoMaruta_Actor8Action);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5e00000, 0xc00000);
    record = Object_GetById(0);
    {
        u16 shown = 0;

        record->facing = shown;
    }
    Engine_ActorEnableActionCallback(0, KorosseoMaruta_LeaderActionA);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_SetPosition(ACTOR_GERALD, 0x5b80000, 0xb80000);
    Actor_SetPosition(ACTOR_IVAN, 0x5b80000, 0xc80000);
    Actor_SetPosition(ACTOR_MIA, 0x5a80000, 0xc00000);
    record = Object_GetById(1);
    {
        u16 shown = 0;

        record->facing = shown;
    }
    record = Object_GetById(2);
    {
        u16 shown = 0;

        record->facing = shown;
    }
    record = Object_GetById(3);
    {
        u16 shown = 0;

        record->facing = shown;
    }
    Engine_TaskWait(1);
    Engine_CameraFollowActor(0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplyTarget(0x10001, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventSetMessage((s32)MsgKorosseoRobin);
    Engine_EventWait(60);
    data_table_addr = (s32)gColossoMultiPhaseData;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
    record = Object_GetById(0);
    record->scale_x = 0x10000;
    record = Object_GetById(0);
    record->scale_y = 0x10000;
    Engine_ActorSetAnimationAndWait(0, 36);
    record = Object_GetById(0);
    record->x.fixed += 0x30000;
    Engine_EventWait(10);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(0, KorosseoMaruta_LeaderActionB);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Actor_WalkToAndWait(1, 0x5e0, 176);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 10);
    Engine_ActorShowEmote(1, 0x100, 20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Object_LinkObjectAndSetCallback(1, 2);
    Engine_EventWait(30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x5d0, 176);
    Actor_WalkTo(ACTOR_GERALD, 0x5f0, 184);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x5e0, 176);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(10);
    Object_LinkObjectAndSetCallback(2, 1);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Object_LinkObjectAndSetCallback(1, 3);
    Object_LinkObjectAndSetCallback(2, 3);
    Actor_WalkToAndWait(ACTOR_MIA, 0x5d0, 184);
    Object_LinkObjectAndSetCallback(2, 0);
    Object_LinkObjectAndSetCallback(1, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Object_LinkObjectAndSetCallback(2, 1);
    Object_LinkObjectAndSetCallback(1, 2);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(10);
    Object_LinkObjectAndSetCallback(2, 3);
    Object_LinkObjectAndSetCallback(1, 3);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_ActorEnableActionCallback(0, KorosseoMaruta_LeaderActionC);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Object_LinkObjectAndSetCallback(1, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Object_LinkObjectAndSetCallback(2, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Object_LinkObjectAndSetCallback(3, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
    Engine_EventWait(60);
    callback_target = (s32)ColossoLogRollingStage_SpawnPeriodicParticle;
    gColossoParticleCount = 9;
    Task_AddCallback((void (*)(void))callback_target, 0xc80);
    Engine_EventWait(5);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(55);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Task_AddCallback((void (*)(void))callback_target, 0xc80);
    Engine_EventWait(20);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    gColossoParticleCount = 9;
    Task_AddCallback((void (*)(void))callback_target, 0xc80);
    Engine_EventWait(35);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(25);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 60);
    gColossoParticleCount = 9;
    Task_AddCallback((void (*)(void))callback_target, 0xc80);
    Engine_EventWait(35);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(25);
    Actor_ShowEmote(2, 0x102, 60);
    Object_LinkObjectAndSetCallback(3, 2);
    Object_LinkObjectAndSetCallback(2, 3);
    Engine_EventWait(60);
    Object_LinkObjectAndSetCallback(3, 0);
    Object_LinkObjectAndSetCallback(2, 0);
    gColossoParticleCount = 9;
    Task_AddCallback((void (*)(void))callback_target, 0xc80);
    Engine_EventWait(35);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(25);
    Actor_ShowEmote(ACTOR_MIA, 0x108, 60);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 3);
    Engine_ActorStartRepeatedMotion(2, 3);
    Engine_ActorRunRepeatedMotion(3, 3);
    Object_LinkObjectAndSetCallback(3, 2);
    Object_LinkObjectAndSetCallback(1, 2);
    gColossoParticleCount = 9;
    Task_AddCallback((void (*)(void))callback_target, 0xc80);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_EventWait(60);
    Actor_WalkTo(ACTOR_MIA, 0x5b8, 200);
    Engine_EventWait(5);
    Actor_WalkTo(ACTOR_IVAN, 0x558, 184);
    Engine_EventWait(3);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x5e8, 184);
    Actor_WalkTo(ACTOR_GERALD, 0x558, 184);
    Engine_ActorWaitForMove(ACTOR_MIA);
    Engine_ActorSetAnimation(3, 1);
    Object_LinkObjectAndSetCallback(3, 0);
    Engine_EventWait(60);
    Actor_WalkToAndWait(ACTOR_MIA, 0x598, 200);
    Actor_WalkTo(ACTOR_MIA, 0x558, 184);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Actor_SetPosition(ACTOR_GERALD, 0x5e80000, 0xb00000);
    Actor_SetPosition(ACTOR_IVAN, 0x5b80000, 0xc00000);
    Actor_SetPosition(ACTOR_MIA, 0x6180000, 0xc80000);
    Graphics_EnableObjLayerAndCallbacks();
    ColorBuffer_ApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(1);
    Engine_EventSetMessage((s32)MsgKorosseoRobinFellAsleep);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(60);
    Engine_EventEnd();
}

void Korosseo_RunGreetScene(s32 a0)
{
    s32 p10;
    s32 p9;
    struct FieldActor *record;

    record = Object_GetById(a0);
    p9 = record->x.part.pixel;
    p10 = record->z.part.pixel;
    Engine_EventBegin();
    Actor_SetSpeed(a0, 0x10000, 0x8000);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Engine_ActorSetPosition(0, (p9 << 16), ((p10 << 16) - 0x300000));
    Engine_ActorSetPosition(1, (p9 << 16) - 0x100000, (p10 << 16) - 0x280000);
    Engine_ActorSetPosition(2, (p9 << 16) + 0x100000, (p10 << 16) - 0x280000);
    Engine_ActorSetPosition(3, (p9 << 16), ((p10 << 16) - 0x200000));
    Engine_ActorSetPosition(a0, (p9 << 16), ((p10 << 16) - 0x500000));
    record = Object_GetById(0);
    record->facing = 0xc000;
    Engine_CameraFollowActor(0, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventSetMessage((s32)MsgKorosseoSiteThirdFinals);
    Engine_ActorSetAnimationAndWait(a0, 3);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(1, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorSetAnimation(2, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorSetAnimation(3, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(3, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 64));
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_ActorSetPosition(3, 0, 0);
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 16));
    Engine_ActorWalkToAndWait(a0, p9, p10);
    Engine_ActorFaceDirection(a0, 0xc000, 10);
    Engine_EventEnd();
}

void FieldScene_RunDualArrivalSequence(s32 scene)
{
    struct FieldActor *pan1;
    struct FieldActor *pan2;
    struct FieldActor *back1;
    struct FieldActor *back2;
    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    state = ColossoLogRollingStage_RunStateInteraction(scene, 1);
    if (state == 0) {
        Engine_EventSetMessage((s32)MsgKorosseoStageCalledScales);
        Engine_CameraSetSpeed(3 << 16, 0x6000);
        Engine_CameraMoveTo(152 << 16, -1, 200 << 16, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Engine_EventShowMessage(scene, 0);
        ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
        Engine_EventWait(60);
        ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
        Engine_EventWait(70);
        Engine_EventShowMessage(scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Engine_TaskWait(2);
        pan1 = Object_GetById(10);
        pan1->motion_flags = 0;
        pan1->acceleration = 0x6666;
        pan1->speed = 0xcccc;
        Call4(Object_SetPosition, (s32)pan1, pan1->x.fixed, 4 << 16, pan1->z.fixed);
        pan2 = Object_GetById(11);
        pan2->motion_flags = 0;
        pan2->acceleration = 0x6666;
        pan2->speed = 0xcccc;
        Call4(Object_SetPosition, (s32)pan2, pan2->x.fixed, 32 << 16, pan2->z.fixed);
        Object_CommitPosition(pan2);
        Engine_EventWait(45);
        back1 = Object_GetById(10);
        back1->motion_flags = 0;
        back1->acceleration = 0x6666;
        back1->speed = 0xcccc;
        Object_SetPosition(back1, back1->x.fixed, 32 << 16, back1->z.fixed);
        back2 = Object_GetById(11);
        back2->motion_flags = 0;
        back2->acceleration = 0x6666;
        back2->speed = 0xcccc;
        Object_SetPosition(back2, back2->x.fixed, 4 << 16, back2->z.fixed);
        Object_CommitPosition(back2);
        Engine_EventWait(15);
        Engine_EventShowMessage(scene, 0);
        ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
        Engine_EventWait(30);
        ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
        Engine_EventWait(40);
        ColossoLogRollingStage_StartPaletteTaskFromState(104, 68, 10);
        Engine_EventWait(70);
        Engine_EventShowMessage(scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Engine_TaskWait(2);
        Engine_CameraFollowActor(0, 0);
        ColossoLogRollingStage_InitializeStateInteraction(scene, 1);
    } else if (state == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoObjectiveGetAcross);
        Engine_EventShowMessage(scene, 0);
    }
    FieldScene_RunMiddleSequence(state, scene, 1);
    Engine_EventEnd();
}

void FieldScene_RunSecondArrivalSequence(s32 scene)
{
    s32 state;
    s32 x;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    state = ColossoLogRollingStage_RunStateInteraction(scene, 2);
    if (state == 0) {
        Engine_EventSetMessage((s32)MsgKorosseoSteppingStoneStage);
        Camera_SetSpeed(3 << 16, 0x6000);
        Camera_MoveTo(188 << 17, -1, 152 << 16, 1);
        x = 280;
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Event_ShowMessage(scene, 0);
        /* FAKEMATCH: native sets r0 after the other arguments; direct and Call3 calls set it first. */
        Value3(Korosseo_FadeInCompetitor, 0, x, 200);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
        /* FAKEMATCH: native sets r0 after the other arguments; direct and Call3 calls set it first. */
        Value3(ColossoLogRollingStage_SpawnPositionedObject, 0, x, 152);
        ColossoLogRollingStage_SpawnPositionedObject(0, 296, 152);
        Engine_EventWait(10);
        Engine_LeaderCheckAhead();
        Camera_MoveTo(-1, -1, -1, 0);
        Engine_ActorFaceDirection(0, 0xc000, 15);
        Engine_LeaderCheckAhead();
        Camera_MoveTo(-1, -1, -1, 0);
        Engine_ActorFaceDirection(0, 0, 15);
        Engine_LeaderCheckAhead();
        Camera_MoveTo(-1, -1, -1, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 15);
        Event_ShowMessage(scene, 0);
        /* FAKEMATCH: native sets r0 after the other arguments; direct and Call3 calls set it first. */
        Value3(ColossoLogRollingStage_StartPaletteTask, 96, 40, 0);
        ColossoLogRollingStage_StartPaletteTaskFromState(128, 40, 10);
        Engine_EventWait(30);
        ColossoLogRollingStage_StartPaletteTaskFromState(160, 40, 10);
        Engine_EventWait(30);
        ColossoLogRollingStage_StartPaletteTaskFromState(160, 72, 10);
        Engine_EventWait(30);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
        ColossoLogRollingStage_InitializeStateInteraction(scene, 2);
    } else if (state == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoYourGoalInStageSimple);
        Event_ShowMessage(scene, 0);
    }
    FieldScene_RunMiddleSequence(state, scene, 2);
    Engine_EventEnd();
}

void KorosseoMaruta_RunStageIntro(s32 scene)
{
    struct EventWork *work;
    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        state = ColossoLogRollingStage_RunStateInteraction(scene, 3);
        if (state == 0) {
            work = gEventWork;
            Engine_EventSetMessage((s32)MsgKorosseoPlaceCalledWall);
            ColossoLogRollingStage_ResetAndRunSceneTask();
            Engine_CameraSetSpeed(3 << 16, 0x6000);
            Engine_CameraMoveTo(154 << 18, -1, 184 << 16, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Event_ShowMessage(scene, 0);
            ColossoLogRollingStage_StartSceneTask();
            Engine_EventWait(60);
            Engine_EventShowMessage(scene, 0);
            /* FAKEMATCH: native sets r0 after the other arguments; direct and Call3 calls set it first. */
            Value3(Korosseo_FadeInCompetitor, 0, 504, 200);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(Engine_ActorFaceDirection, 0, 0, 0);
            ColossoLogRollingStage_WaitForSceneTask();
            /* FAKEMATCH: native sets r0 before the constants; a direct call sets it after. */
            Actor_SetSpeed(0, 0x18000, 0xc000);
            ColossoLogRollingStage_PositionScaledObject(0, 680, 200);
            if (work->raised_trigger != 5) {
                do {
                    Engine_TaskWait(1);
                    if (++state > 239) {
                        break;
                    }
                } while (work->raised_trigger != 5);
            }
            ColossoLogRollingStage_ClampAndOffsetActiveActor();
            /* FAKEMATCH: native sets r0 before the constants; a direct call sets it after. */
            Actor_FaceDirection(0, 0xc000, 20);
            /* FAKEMATCH: native sets r0 before the constants; a direct call sets it after. */
            Actor_ShowEmote(0, 0x103, 60);
            Engine_EventShowMessage(scene, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            ColossoLogRollingStage_InitializeStateInteraction(scene, 3);
            work->raised_trigger = 0;
        } else if (state == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoOperatorWallsCheer);
            Engine_EventShowMessage(scene, 0);
        }
        FieldScene_RunMiddleSequence(state, scene, 3);
        Engine_EventEnd();
    }
}
