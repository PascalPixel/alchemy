/* The log-rolling stage: its scene task, grid and object setup, the
 * opening, closing and final sequences, the obstacle checks and the scene
 * event. */
#include "LOG_ROLLING.H"
#include "CALL.H"

/* The four rows of log kinds the stage cycles through, one per log. */
extern s8 KorosseoMaruta_LogPattern[][6];

/* The stage's per-frame task. Every eighteenth frame it advances the log
 * pattern, animating the five logs and their shadows and opening or closing
 * the floor under each; in between, a selected actor standing in a log's
 * column on row 11 or 12 raises that log's trigger. */
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

/* The log-rolling stage's setup: publish the scene phase, install its
 * per-frame task and lay out the stage's props, drifting obstacles, the
 * two banks of paired actors and the presentation extras, choosing among
 * alternative layouts by the story flags; then run the beat the scene's
 * sub-state names. The overlay's exported entry. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "COLOSSO_LOG_ROLLING_STAGE.H"

extern u8 MsgKorosseoDidntThinkBattles[];

void ColossoLogRollingStage_NoopSceneHook(void);
s32 ColossoLogRollingStage_PositionActiveActor(s32 slot, s32 frames);
void ColossoLogRollingStage_WaitForBalanceState(void);
void Object_RefreshSelectorById(s32 object);
void BattleFx_SetWeightedResult(s32 kind, s32 weight);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetPair1d4(s32 scene, s32 entrance);

/* The object script record 30 runs. */
extern u8 KorosseoMaruta_Record30Script[];

/* The stage picture the scene control decodes. */
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
void ColossoLogRollingStage_ShowActorPositionMessage(void);
void ColossoLogRollingStage_MarkSceneProgress(void);
void ColossoLogRollingStage_SceneTask(void);

extern u8 MsgKorosseoRobin[];
extern u8 MsgKorosseoRobinFellAsleep[];

extern u8 MsgKorosseoSiteThirdFinals[];
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

extern u8 MsgKorosseoObjectiveGetAcross[];
extern u8 MsgKorosseoStageCalledScales[];
void Korosseo_FinishSoloRound();
void FieldScene_RunMiddleSequence();
void Object_SetPosition();
void Object_CommitPosition();

extern u8 MsgKorosseoSteppingStoneStage[];
extern u8 MsgKorosseoYourGoalInStageSimple[];

extern u8 MsgKorosseoOperatorWallsCheer[];
extern u8 MsgKorosseoPlaceCalledWall[];
s32 Korosseo_FadeInCompetitor();
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
    Engine_TaskAddCallback((s32)ColossoLogRollingStage_SceneTask, 0xC80);
}

void ColossoLogRollingStage_WaitForSceneTask(void)
{
    s32 polls;

    /* 素直な while ループ。goto 版では初回の読みがテストへ沈む。
     * A plain while loop. The goto-scaffolded version let gcc sink the first
     * read of gColossoSceneTaskStatus into the test block, where the reference loads it
     * before the loop. And the frame count is a literal ten: the reference
     * emits `movs r0, #10`, which a Value_ symbol cannot produce. */
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
    s32 id = *(s32 *)&table[250];
    StageObstacleActor *subject = *(StageObstacleActor **)(workspace + 480);
    StageObstacleActor *actor = Object_GetById(id);
    s32 z = *(s16 *)((u8 *)actor + 0x12);

    /* Nudge both records left while the actor occupies rows 183 through 186. */
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
    extern void Object_SetPosition(PrimaryStageObject *, s32, s32, s32);

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

void FieldScene_RunClosingAuxiliarySequence(void)
{
    extern void Object_SetPosition();

    u32 i;
    u8 *p9;
    s32 rec;
    s32 rec7;
    u8 *record;
    u8 *p6;

    u8 *base = (u8 *)&gGameState;

    p6 = *(u8 **)(base + 500);
    rec = GameFlag_IsSet(0x362);
    if (rec == 0) {
        record = Object_GetById(10);
        if ((s32)record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Engine_ActorWaitForMove((s32)p6);
        record = Object_GetById(11);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Object_SetPosition((s32)record, *(s32 *)((s32)record + 8), 0x200000, *(s32 *)((s32)record + 16));
        record = Object_GetById(10);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Call4(Object_SetPosition, (s32)record, *(s32 *)((s32)record + 8), 0x40000, *(s32 *)((s32)record + 16));
        rec7 = Object_GetById((s32)p6);
        p9 = rec7 + 85;
        *p9 = rec;
        *(s32 *)(rec7 + 52) = 0x6666;
        *(s32 *)(rec7 + 48) = 0xcccc;
        Object_SetPosition(rec7, *(s32 *)(rec7 + 8), 0x40000, *(s32 *)(rec7 + 16));
        Engine_ActorSetSpriteFlags(rec7, 1);
        Engine_ActorWaitForMove((s32)p6);
        Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
        Engine_TaskWait(2);
        Engine_ActorSetSpriteFlags(rec7, 1);
        *p9 = 3;
        *(s32 *)(rec7 + 20) = *(s32 *)(rec7 + 12);
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

    u8 *rec;
    u8 *b1;
    u8 *t;
    u8 *b2;
    u8 *b3;
    s32 two;
    s32 zero;
    s32 a;
    s32 b;

    rec = (u8 *)Object_GetById(12);
    a = (*(s32 *)((s32)rec + 8) >> 20);
    if (a == 9) {
        b = (*(s32 *)((s32)rec + 16) >> 20);
        if (b == 12) {
            b1 = Object_GetById(12);
            Engine_ActorSetSpriteFlags((s32)b1, 0);
            t = b1 + 35;
            zero = 0;
            two = 2;
            *t = two;
            t += 50;
            *t = zero;
            *(s32 *)((s32)b1 + 52) = 0x6666;
            *(s32 *)((s32)b1 + 48) = 0xcccc;
            Call4(Object_SetPosition, (s32)b1, *(s32 *)((s32)b1 + 8), 0x40000, *(s32 *)((s32)b1 + 16));
            b2 = (u8 *)Object_GetById(11);
            b2[35] = two;
            *(s32 *)((s32)b2 + 52) = 0x6666;
            *(s32 *)((s32)b2 + 48) = 0xcccc;
            Object_SetPosition((s32)b2, *(s32 *)((s32)b2 + 8), 0x200000, *(s32 *)((s32)b2 + 16));
            b3 = Object_GetById(10);
            *(s32 *)((s32)b3 + 52) = 0x6666;
            *(s32 *)((s32)b3 + 48) = 0xcccc;
            Object_SetPosition((s32)b3, *(s32 *)((s32)b3 + 8), 0x40000, *(s32 *)((s32)b3 + 16));
            GameFlag_Set(0x368);
            Map_CopyCellAttributes(15, 12, 1, 1, 13, b);
            Map_CopyCellAttributes(1, 25, 1, 1, a, b);
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
    extern s32 Runtime_AllocateBlock();
    extern void Object_SetPosition();
    extern void ObjectDispatch_InitFromTable4WithArgument();
    extern void Object_CommitPosition();

    extern u16 gColossoEarlySequenceData[];
    s32 leader;
    s32 log_actor;
    u8 *state;
    s32 tile;
    s32 x;
    s32 steps;
    s32 next_x;
    s32 column;
    s32 acceleration;
    s32 direction;
    s32 cell_step;
    s32 record;
    s32 selected_actor;
    s32 z;
    volatile s32 *keys;
    s32 cell_center[3];

    state = (u8 *)&gGameState;
    leader = Object_GetById(*(s32 *)(state + 500));
    log_actor = Object_GetById(31);
    steps = 0;
    tile = gColossoEarlySequenceData[*(u16 *)(leader + 6) >> 13];
    selected_actor = *(s32 *)(state + 500);
    cell_center[0] = (*(s32 *)(leader + 8) & -0x100000) + 0x80000;
    cell_center[1] = *(s32 *)(leader + 12);
    cell_center[2] = (*(s32 *)(leader + 16) & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x100000, tile, (s32)cell_center);
    x = *(s32 *)(log_actor + 8);
    z = *(s32 *)(log_actor + 16);
    if ((cell_center[0] - x >= 0 ? cell_center[0] - x : x - cell_center[0]) > 0x80000
        || (cell_center[2] - z >= 0 ? cell_center[2] - z : z - cell_center[2]) > 0x200000) {
        goto far;
    }
    keys = (volatile s32 *)gKeysHeld;
    if ((*keys & 32) != 0) {
        direction = 2;
        cell_step = -8;
        for (;;) {
            next_x = x - 0x100000;
            if (ColossoLogRollingStage_CheckPathClearance(next_x, z) != 0) {
                goto moved;
            }
            steps++;
            x = next_x;
        }
    }
    if ((*keys & 16) == 0) {
        return;
    }
    direction = 3;
    cell_step = 8;
    for (;;) {
        next_x = x + 0x100000;
        if (ColossoLogRollingStage_CheckPathClearance(next_x, z) != 0) {
            goto moved;
        }
        steps++;
        x = next_x;
    }
moved:
    if (steps == 0) {
        return;
    }
    Map_CopyCellAttributes(74, 8, 1, 4, *(s32 *)(log_actor + 8) >> 20, 9);
    Map_CopyCellAttributes(120, 60, 8, 5, 74, 60);
    Engine_EventBegin();
    Engine_ActorSetAnimation(selected_actor, 8);
    Engine_EventWait(6);
    *(s32 *)(log_actor + 48) = 0x8000;
    acceleration = 0x3333;
    *(s32 *)(log_actor + 52) = acceleration;
    Object_SetMode(log_actor, direction);
    Object_SetPosition(log_actor, x, 0, z);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(selected_actor, 2);
    record = Runtime_AllocateBlock(27, 0xccc);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(record + 0x1e0), log_actor);
    Actor_SetSpeed(selected_actor, 0x8000, acceleration);
    Audio_PlayCue(239);
    Object_SetMode(leader, 2);
    Object_SetPosition(leader, ((steps * cell_step) << 16) + *(s32 *)(leader + 8), 0,
                       *(s32 *)(leader + 16));
    Object_CommitPosition(leader);
    Object_SetMode(leader, 1);
    Object_CommitPosition(log_actor);
    if (x >= 0x5300000) {
        GameFlag_Set(0x369);
        Engine_ActorSetAnimation(31, 3);
        Actor_SetDestinationOffset(31, 18, 6);
        Engine_EventWait(30);
        Object_SetMode(log_actor, 8);
        Object_CommitPosition(log_actor);
        *(u8 *)(log_actor + 35) = 2;
        column = 84;
        Map_CopyCellAttributes(86, 10, 1, 2, column, 10);
        Map_CopyCellAttributes(86, 9, 1, 1, column, 12);
        Audio_PlayCue(0x120);
        Audio_PlayCue(240);
    } else {
        Object_SetMode(log_actor, 1);
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

/* Pause object 28, raise flag 0x361, then wait until the scene task reports
 * status 1 or 3 before dropping it from the task list. */
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

/* Offsetting the active actor, clamped or not. */
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

/* A competitor's question at entrance 2. Each competitor has three lines, the
 * question and its two answers, counted from MsgKorosseoDidntThinkBattles; a
 * yes closes the screen and selects that competitor. */
void KorosseoMaruta_RunCompetitorTalk(s32 a0)
{
    u8 *work;
    s32 owner;
    s32 msg;
    s32 lines;
    s32 entrance;

    work = (u8 *)gEventWork;
    owner = gGameState.selected_actor;
    entrance = gGameState.entrance;
    if (entrance == 2) {
        Engine_EventBegin();
        msg = (s32)MsgKorosseoDidntThinkBattles;
        lines = a0 * 3;
        Engine_EventSetMessage(lines + msg);
        Event_OpenMessage(a0, 0);
        if (Engine_EventChooseYesNo(owner, 0) == 0) {
            /* FAKEMATCH: each answer's line goes through its own local, here
             * and below, which keeps the reference's addition order. */
            s32 yes = msg + 1;
            Engine_EventSetMessage(lines + yes);
            Event_ShowMessage(a0, 0);
            *(s32 *)(work + 0x1c0) = 0x200;
            *(s32 *)(work + 0x1c8) = 15;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
            Korosseo_SelectSoloCompetitor(a0);
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
        } else {
            s32 no = msg + 2;
            Engine_EventSetMessage(lines + no);
            Event_ShowMessage(a0, 0);
        }
        Engine_EventEnd();
    }
}

/* The log-rolling stage's start: the leader and the competitor walk out onto
 * the logs, face each other and take their places, the result is weighted
 * by the leader's place, and the stage records where the party returns. */
void KorosseoMaruta_RunStageStart(void)
{
    s32 place;
    s32 i;
    s32 last;

    /* FAKEMATCH: the last place is set at the top as a forced temporary, so
     * the weight stays 4 - place + 1 instead of folding to 5 - place. */
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
    /* FAKEMATCH: the do/while keeps the stage flag store ahead of the
     * scene load that follows it. */
    do {
        gGameState.unknown_200[0x22b - 0x200] = 3;
    } while (0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoMaruta, 4);
    Event_SetPair1d4((s32)&SceneId_KorosseoMaruta, 5);
    Engine_GameFlagSet(0x11a);
}

/* The scene event's state hooks. */
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

/* Cell holding the shared scene-work pointer; +448 is the scene phase word. */
/* Shared cross-overlay scene-record block: +450 is the scene sub-state, read as
 * a signed halfword, and +498 is a byte this owner clears on the way in. */

/* Builds the log-rolling stage, then runs the beat named by the scene
 * sub-state.  Returns 0 on every path. */
s32 StageSetup_BuildAndDispatch(void)
{
    u8 *rec;
    s32 i;
    s32 val;
    s32 hit;
    s32 cnt;

    *(s32 *)(((u8 *)gEventWork) + 448) = 256;
    Engine_GameFlagSet(324);
    Engine_TaskAddCallback(ColossoLogRollingStage_ShowActorPositionMessage, 3200);
    Call6(Engine_MapCopyCellAttributes, 74, 60, 8, 6, 120, 60);

    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);

    rec = Object_GetById(10);
    Engine_ActorSetSpriteFlags(rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x200000;

    rec = (u8 *)Object_GetById(11);
    Engine_ActorSetSpriteFlags(rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x40000;

    if (Engine_GameFlagIsSet(866) != 0) {
        Engine_ActorSetAnimation(9, 5);
        rec = (u8 *)Object_GetById(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec = (u8 *)Object_GetById(11);
        *(s32 *)(rec + 12) = 0x200000;
        Call6(Engine_MapCopyCellAttributes, 15, 12, 1, 1, 13, 12);
    } else {
        rec = (u8 *)Object_GetById(9);
        *(s32 *)(rec + 24) = 0x18000;
        *(s32 *)(rec + 28) = 0x18000;
        if (Engine_GameFlagIsSet(871) != 0) {
            Call6(Engine_MapCopyCellAttributes, 0, 24, 1, 1, 9, 12);
        } else {
            Call6(Engine_MapCopyCellAttributes, 0, 25, 1, 1, 9, 12);
        }
    }

    if (Engine_GameFlagIsSet(872) != 0) {
        Engine_MapCopyCellAttributes(15, 12, 1, 1, 13, 12);
        Engine_MapCopyCellAttributes(1, 25, 1, 1, 9, 12);
        rec = (u8 *)Object_GetById(12);
        Engine_ActorSetSpriteFlags(rec, 0);
        rec[85] = 0;
        *(s32 *)(rec + 12) = 0x20000;
        rec[35] = 2;
        rec = (u8 *)Object_GetById(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec[35] = 2;
        rec = (u8 *)Object_GetById(11);
        *(s32 *)(rec + 12) = 0x200000;
    }

    cnt = GameFlag_GetByte(880);
    if (cnt == 0) {
        cnt = 19;
    }
    rec = (u8 *)Object_GetById(13);
    *(s32 *)(rec + 8) = (cnt << 20) + 0x80000;
    rec[85] = 0;
    rec[35] = 2;
    Call6(Engine_MapCopyCellAttributes, 18, 10, 3, 1, 18, 11);
    Engine_MapCopyCellAttributes(17, 11, 1, 1, cnt, 11);

    /* The three drifting obstacles: any one still at rest and clear of the
     * grid is planted and drawn twice, once on its own row and once 52 rows
     * further down. */
    for (i = 15; i <= 17; i++) {
        rec = (u8 *)Object_GetById(i);
        hit = Map_GetTerrainHeight(0, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
        if (*(s32 *)(rec + 12) == 0 && hit == 0) {
            rec[35] = 2;
            rec[85] = hit;
            Call6(Engine_MapCopyCellAttributes, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20, *(s32 *)(rec + 16) >> 20);
            Call6(Engine_MapCopyCellAttributes, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20, (*(s32 *)(rec + 16) >> 20) + 52);
        }
    }

    /* Two banks of five paired actors, 18..22 and 23..27.  The cleared arm
     * also puts each pair on the grid; the uncleared arm only prepares the
     * records and installs the second per-frame task. */
    if (Engine_GameFlagIsSet(865) != 0) {
        s32 state;
        s32 mode;
        s32 row;

        i = 18;
        state = 0;
        mode = 2;
        val = 33;
        row = 11;
        for (; i <= 22; i++) {
            rec = (u8 *)Object_GetById(i);
            rec[35] = mode;
            Object_SetMode(rec, 2);
            rec = (u8 *)Object_GetById(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
            Object_SetMode(rec, 10);
            Engine_MapCopyCellAttributes(74, 12, 1, 1, val, row);
            val += 2;
        }
        Engine_ActorSetAnimation(28, 10);
        BattleEffect_PauseObject(28);
    } else {
        s32 state;
        s32 mode;

        i = 18;
        state = 0;
        mode = 2;
        for (; i <= 22; i++) {
            rec = (u8 *)Object_GetById(i);
            rec[35] = mode;
            rec = (u8 *)Object_GetById(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
        }
        Engine_TaskAddCallback(ColossoLogRollingStage_SceneTask, 3200);
    }

    if (Engine_GameFlagIsSet(864) != 0) {
        Engine_ActorSetAnimation(29, 4);
        Call6(Engine_MapCopyCellAttributes, 47, 61, 1, 4, 49, 61);
    }

    if (Engine_GameFlagIsSet(867) != 0) {
        Map_SetLayerEntryFlag(1);
        rec = (u8 *)Object_GetById(30);
        rec[85] = 0;
        *(s32 *)(rec + 8) = 0x046a0000;
        *(s32 *)(rec + 16) = 0xb80000;
        Engine_ActorSetSpriteFlags(rec, 0);
        Object_SetMode(rec, 3);
        Engine_ObjectSetScript(rec, KorosseoMaruta_Record30Script);
    } else {
        Map_SetLayerEntryFlag(2);
    }

    if (Engine_GameFlagIsSet(873) != 0) {
        rec = (u8 *)Object_GetById(31);
        Object_SetMode(rec, 8);
        rec[35] = 2;
        Engine_MapCopyCellAttributes(86, 10, 1, 2, 84, 10);
        Engine_MapCopyCellAttributes(86, 9, 1, 1, 84, 12);
    } else {
        rec = (u8 *)Object_GetById(31);
        Call6(Engine_MapCopyCellAttributes, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 9);
        Call6(Engine_MapCopyCellAttributes, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 61);
    }

    {
        s32 state;
        s32 mode;

        state = 0;
        mode = 2;
        rec = (u8 *)Object_GetById(9);
        rec[85] = state;
        rec[35] = mode;
        rec = (u8 *)Object_GetById(10);
        rec[85] = state;
        rec[35] = mode;
        rec = (u8 *)Object_GetById(11);
        rec[85] = state;
        rec[35] = mode;
        Engine_ActorSetAnimation(8, 9);
        gGameState.movement_mode = state;
    }
    Korosseo_ShowItemIcon(39, 3);
    Korosseo_ShowItemIcon(40, 17);
    Engine_ActorSetChildValue(8, 2);

    switch (*(s16 *)(((u8 *)&gGameState) + 450)) {
    case 1:
        ((s32 (*)())ColossoLogRollingStage_SetupSceneDescriptor)(0, 8, 6, 0x5e80000, 0xc00000, 39, 40);
        Call6(Engine_MapCopyCellsTo, 127, 0, 1, 2, 5, 2);
        Engine_ActorDestroy(32);
        Engine_ActorDestroy(33);
        Engine_ActorDestroy(34);
        Engine_ActorDestroy(35);
        Engine_ActorDestroy(36);
        Engine_ActorDestroy(37);
        Engine_ActorDestroy(38);
        if (Engine_GameFlagIsSet(265) == 0) {
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
        if (Engine_GameFlagIsSet(265) != 0) {
            break;
        }
        ColossoLogRollingStage_RestoreActorPositions();
        Korosseo_SelectSoloCompetitor(1);
        ColossoLogRollingStage_RunScriptedTransition(0);
        break;

    case 3:
        if (Engine_GameFlagIsSet(265) != 0) {
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

/* The periodic particles and the multi-phase actor sequence. */
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

    s32 record;
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
    Engine_ActorEnableActionCallback(8, (s32)KorosseoMaruta_Actor8Action);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5e00000, 0xc00000);
    record = Object_GetById(0);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Engine_ActorEnableActionCallback(0, (s32)KorosseoMaruta_LeaderActionA);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_SetPosition(ACTOR_GERALD, 0x5b80000, 0xb80000);
    Actor_SetPosition(ACTOR_IVAN, 0x5b80000, 0xc80000);
    Actor_SetPosition(ACTOR_MIA, 0x5a80000, 0xc00000);
    record = Object_GetById(1);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Object_GetById(2);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Object_GetById(3);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Engine_TaskWait(1);
    Engine_CameraFollowActor(0, 0);
    gEventWork->start_transition = 0x100;
    ColorBuffer_ApplyTarget(0x10001, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventSetMessage((s32)MsgKorosseoRobin);
    Engine_EventWait(60);
    data_table_addr = (s32)gColossoMultiPhaseData;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
    record = Object_GetById(0);
    *(s32 *)(record + 24) = 0x10000;
    record = Object_GetById(0);
    *(s32 *)(record + 28) = 0x10000;
    Engine_ActorSetAnimationAndWait(0, 36);
    record = Object_GetById(0);
    *(s32 *)(record + 8) += 0x30000;
    Engine_EventWait(10);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(0, (s32)KorosseoMaruta_LeaderActionB);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorWalkToAndWait, 1, 0x5e0, 176);
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
    Engine_ActorEnableActionCallback(0, (s32)KorosseoMaruta_LeaderActionC);
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
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Engine_EventWait(5);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(55);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Engine_EventWait(20);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Engine_EventWait(35);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(25);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 60);
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Engine_EventWait(35);
    Engine_TaskRemoveCallback(callback_target);
    Engine_EventWait(25);
    Call3(Engine_ActorShowEmote, 2, 0x102, 60);
    Object_LinkObjectAndSetCallback(3, 2);
    Object_LinkObjectAndSetCallback(2, 3);
    Engine_EventWait(60);
    Object_LinkObjectAndSetCallback(3, 0);
    Object_LinkObjectAndSetCallback(2, 0);
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
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
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
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

/* The third finals' greeting: the site announcement the other trial
   overlays share, with this stage's line. */

/* Colosso: line the other competitors up around actor a0, show its message
 * and walk it back to its place while the others gather on actor 0. */
/* Colosso: the competitors are greeted before a trial. The same scene sits
 * in the wall and log-rolling trial overlays. */
void Korosseo_RunGreetScene(s32 a0)
{
    s32 p10;
    s32 p9;
    struct FieldActor *record;

    record = Object_GetById(a0);
    p9 = record->x.part.pixel;
    p10 = record->z.part.pixel;
    Engine_EventBegin();
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
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
    void *p17;
    void *p19;
    void *p23;
    void *p25;
    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    state = ColossoLogRollingStage_RunStateInteraction(scene, 1);
    if (state == 0) {
        Engine_EventSetMessage((s32)MsgKorosseoStageCalledScales);
        Camera_SetSpeed(196608, 24576);
        Camera_MoveTo(9961472, -1, 13107200, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
        Engine_EventWait(60);
        ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
        Engine_EventWait(70);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Engine_TaskWait(2);
        p17 = Object_GetById(10);
        *(u8 *)((u8 *)p17 + 85) = 0;
        *(s32 *)(p17 + 52) = 26214;
        *(s32 *)(p17 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p17, *(s32 *)(p17 + 8), 262144, *(s32 *)(p17 + 16));
        p19 = Object_GetById(11);
        *(u8 *)((u8 *)p19 + 85) = 0;
        *(s32 *)(p19 + 52) = 26214;
        *(s32 *)(p19 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p19, *(s32 *)(p19 + 8), 2097152, *(s32 *)(p19 + 16));
        Object_CommitPosition(p19);
        Engine_EventWait(45);
        p23 = Object_GetById(10);
        *(u8 *)((u8 *)p23 + 85) = 0;
        *(s32 *)(p23 + 52) = 26214;
        *(s32 *)(p23 + 48) = 52428;
        Object_SetPosition((s32)p23, *(s32 *)(p23 + 8), 2097152, *(s32 *)(p23 + 16));
        p25 = Object_GetById(11);
        *(u8 *)((u8 *)p25 + 85) = 0;
        *(s32 *)(p25 + 52) = 26214;
        *(s32 *)(p25 + 48) = 52428;
        Object_SetPosition((s32)p25, *(s32 *)(p25 + 8), 262144, *(s32 *)(p25 + 16));
        Object_CommitPosition(p25);
        Engine_EventWait(15);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
        Engine_EventWait(30);
        ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
        Engine_EventWait(40);
        ColossoLogRollingStage_StartPaletteTaskFromState(104, 68, 10);
        Engine_EventWait(70);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Engine_TaskWait(2);
        Engine_CameraFollowActor(0, 0);
        ColossoLogRollingStage_InitializeStateInteraction(scene, 1);
    } else if (state == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoObjectiveGetAcross);
        Event_ShowMessage(scene, 0);
    }
    /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
    Value3(FieldScene_RunMiddleSequence, state, scene, 1);
    Engine_EventEnd();
}

/* The second competitor's arrival. */
void FieldScene_RunSecondArrivalSequence(s32 scene)
{
    extern void Korosseo_FinishSoloRound();
    extern void FieldScene_RunMiddleSequence();
    extern s32 Korosseo_FadeInCompetitor();
    extern void Korosseo_RestoreCompetitor();

    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    state = ColossoLogRollingStage_RunStateInteraction(scene, 2);
    if (state == 0) {
    Engine_EventSetMessage((s32)MsgKorosseoSteppingStoneStage);
    Camera_SetSpeed(196608, 24576);
    Camera_MoveTo(24641536, -1, 9961472, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Event_ShowMessage(scene, 0);
    Korosseo_FadeInCompetitor(0, 280, 200);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 98304, 49152);
    /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
    Value3(ColossoLogRollingStage_SpawnPositionedObject, 0, 280, 152);
    ColossoLogRollingStage_SpawnPositionedObject(0, 296, 152);
    Engine_EventWait(10);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ActorFaceDirection(0, 49152, 15);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ActorFaceDirection(0, 0, 15);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 15);
    Event_ShowMessage(scene, 0);
    /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
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
    /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
    Value3(FieldScene_RunMiddleSequence, state, scene, 2);
    Engine_EventEnd();
}

/* Colosso log stage: unless the stage is already cleared, show the
 * introduction, wait up to 240 frames for the log to settle and hand over to
 * the stage. */
void KorosseoMaruta_RunStageIntro(s32 a0)
{
    s32 p8;
    s32 rec8;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec8 = ColossoLogRollingStage_RunStateInteraction(a0, 3);
        if (rec8 == 0) {
            p8 = *(s32 *)&gEventWork;
            Engine_EventSetMessage((s32)MsgKorosseoPlaceCalledWall);
            ColossoLogRollingStage_ResetAndRunSceneTask();
            Engine_CameraSetSpeed(0x30000, 0x6000);
            Engine_CameraMoveTo(0x2680000, -1, 0xb80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Engine_EventShowMessage(a0, 0);
            ColossoLogRollingStage_StartSceneTask();
            Engine_EventWait(60);
            Engine_EventShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x1f8, 200);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(Engine_ActorFaceDirection, 0, 0, 0);
            ColossoLogRollingStage_WaitForSceneTask();
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            ColossoLogRollingStage_PositionScaledObject(0, 0x2a8, 200);
            if (*(s16 *)(p8 + 0x182) != 5) {
                do {
                    Engine_TaskWait(1);
                    if (++rec8 > 239) {
                        break;
                    }
                } while (*(s16 *)(p8 + 0x182) != 5);
            }
            ColossoLogRollingStage_ClampAndOffsetActiveActor();
            Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
            Call3(Engine_ActorShowEmote, 0, 0x103, 60);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 3);
            {
                /* FAKEMATCH: the slot pointer and word temporary order the address before the zero */
                u8 *slot = (u8 *)(p8 + 0x182);
                s32 shown = 0;

                *(u16 *)slot = shown;
            }
        } else {
            if (rec8 == 1) {
                Engine_EventSetMessage((s32)MsgKorosseoOperatorWallsCheer);
                Engine_EventShowMessage(a0, 0);
            }
        }
        ((s32 (*)())FieldScene_RunMiddleSequence)(rec8, a0, 3);
        Engine_EventEnd();
    }
}
