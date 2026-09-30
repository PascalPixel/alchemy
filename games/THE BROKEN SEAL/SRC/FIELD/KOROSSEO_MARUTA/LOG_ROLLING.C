/* The log-rolling stage: its scene task, grid and object setup, the
 * opening, closing and final sequences, the obstacle checks and the scene
 * event. */
#include "LOG_ROLLING.H"
#include "CALL.H"

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
        Actor_SetSpriteFlags(rec7, 1);
        Actor_WaitForMove((s32)p6);
        Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
        Task_Wait(2);
        Actor_SetSpriteFlags(rec7, 1);
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
            Actor_SetSpriteFlags((s32)b1, 0);
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
    Event_Begin();
    Actor_SetAnimation(selected_actor, 8);
    Event_Wait(6);
    *(s32 *)(log_actor + 48) = 0x8000;
    acceleration = 0x3333;
    *(s32 *)(log_actor + 52) = acceleration;
    Object_SetMode(log_actor, direction);
    Object_SetPosition(log_actor, x, 0, z);
    Event_Wait(6);
    Actor_SetAnimation(selected_actor, 2);
    record = Runtime_AllocateBlock(27, 0xccc);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(record + 0x1e0), log_actor);
    Actor_SetSpeed(selected_actor, 0x8000, acceleration);
    Audio_PlayCue(239);
    Object_SetAnimation(leader, 2);
    Object_SetPosition(leader, ((steps * cell_step) << 16) + *(s32 *)(leader + 8), 0,
                       *(s32 *)(leader + 16));
    Object_CommitPosition(leader);
    Object_SetMode(leader, 1);
    Object_CommitPosition(log_actor);
    if (x >= 0x5300000) {
        GameFlag_Set(0x369);
        Actor_SetAnimation(31, 3);
        Actor_SetDestinationOffset(31, 18, 6);
        Event_Wait(30);
        Object_SetAnimation(log_actor, 8);
        Object_CommitPosition(log_actor);
        *(u8 *)(log_actor + 35) = 2;
        column = 84;
        Map_CopyCellAttributes(86, 10, 1, 2, column, 10);
        Map_CopyCellAttributes(86, 9, 1, 1, column, 12);
        Audio_PlayCue(0x120);
        Audio_PlayCue(240);
    } else {
        Object_SetAnimation(log_actor, 1);
        Audio_PlayCue(0x120);
        Audio_PlayCue(213);
        column = x >> 20;
        Map_CopyCellAttributes(85, 9, 1, 4, column, 9);
        Map_CopyCellAttributes(85, 9, 1, 4, column, 61);
    }
    Event_Wait(15);
    Event_End();
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
