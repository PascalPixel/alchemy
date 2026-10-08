#include "LOG_ROLLING.H"

s32 *FindActorAtWholeTilePosition(s32 *position, void *mover)
{
    s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
    u32 actor_index;

    for (actor_index = 8; actor_index <= 65; actor_index++) {
        s32 *actor = slots[actor_index];

        if ((position[0] >> 20) == (actor[2] >> 20)
            && (position[1] >> 20) == (actor[3] >> 20)
            && (position[2] >> 20) == (actor[4] >> 20)) {
            return actor;
        }
    }
    return 0;
}

void ColossoLogRollingStage_PushStagedActor(void)
{
    extern s32 Object_CheckMovementCollision(MarutaSceneRecord *, Position3 *);
    extern void Object_CommitPosition(MarutaSceneRecord *);

    MarutaSceneRecord *subject;
    MarutaSceneRecord *target;
    MarutaSceneRecord *blocker;
    u32 step;
    u32 direction;
    Position3 position;
    u32 data_index = 250;
    s32 zero;
    s32 subject_handle;

    subject_handle = *(s32 *)((u8 *)&gGameState + (data_index << 1));
    subject = Object_GetById(subject_handle);

    direction = subject->facing >> 12;

    step = StagedActor_DirectionSteps[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = FindActorAtWholeTilePosition(&position, subject);
    if (target == 0) {
        return;
    }

    step = StagedActor_DirectionSteps[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = FindActorAtWholeTilePosition(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    position.x = target->x;
    position.y = target->y + 0x100000;
    position.z = target->z;

    blocker = FindActorAtWholeTilePosition(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = StagedActor_DirectionSteps[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    if (Object_CheckMovementCollision(target, &position) > 0) {
        return;
    }

    Object_SetMode(subject, 8);
    Engine_TaskWait(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    ((void (*)(MarutaSceneRecord *, s32, s32, s32))Engine_ObjectSetPosition)(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    ((void (*)(MarutaSceneRecord *, s32, s32, s32))Engine_ObjectSetPosition)(subject, position.x, position.y, position.z);

    Engine_AudioPlayCue(0xee);
    Object_CommitPosition(target);
    Engine_AudioPlayCue(0x120);

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Object_SetMode(subject, 1);
}

s32 *ColossoLogRollingStage_FindActorAhead(void)
{
    extern void Vector_AddPolarOffset();

    struct FieldActor *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Object_GetById(gGameState.selected_actor);

    facing = (record->facing + 0x2000) & 0xc000;

    position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
    position[1] = record->y.fixed;
    position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, facing, position);

    occupant = FindActorAtWholeTilePosition(position, record);
    if (occupant == 0) {
        position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
        position[1] = record->y.fixed;
        position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, facing, position);

        occupant = FindActorAtWholeTilePosition(position, record);
    }

    return occupant;
}
