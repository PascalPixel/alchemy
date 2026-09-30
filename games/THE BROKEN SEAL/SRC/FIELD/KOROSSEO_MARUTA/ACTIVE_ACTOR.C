/* Offsetting the active actor, clamped or not. */
#include "LOG_ROLLING.H"

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
