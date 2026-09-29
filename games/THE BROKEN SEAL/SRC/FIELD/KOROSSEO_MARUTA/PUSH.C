/* Finding an actor on a tile and pushing a staged actor. */
#include "LOG_ROLLING.H"

/* The actor standing on the tile of position, if any. Callers pass the actor
 * that is moving as well; the lookup does not skip it. */
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
    extern s32 Object_CheckMovementCollision(SceneRecord *, Position3 *);
    extern void Object_SetPosition(SceneRecord *, s32, s32, s32);
    extern void Object_CommitPosition(SceneRecord *);


    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 direction;
    Position3 position;
    u32 data_index = 250;
    s32 zero;
    s32 subject_handle;

    subject_handle = *(s32 *)((u8 *)&gGameState + (data_index << 1));
    subject = Engine_ActorGet(subject_handle);

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

    /* Is the cell one step beyond the target already taken? */
    step = StagedActor_DirectionSteps[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = FindActorAtWholeTilePosition(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position.x = target->x;
    position.y = target->y + 0x100000;      /* 128 << 13 */
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

    Object_SetAnimation(subject, 8);
    Task_Wait(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Object_SetPosition(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Object_SetPosition(subject, position.x, position.y, position.z);

    Audio_PlayCue(0xee);
    Object_CommitPosition(target);
    Audio_PlayCue(0x120);                                /* 144 << 1 */

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Object_SetAnimation(subject, 1);
}

/* This overlay's own byte-exact occupancy lookup. */
s32 *ColossoLogRollingStage_FindActorAhead(void)
{
    extern void Vector_AddPolarOffset();


    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Engine_ActorGet(gGameState.selected_actor);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, facing, position);          /* 128 << 13 */

    occupant = FindActorAtWholeTilePosition(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, facing, position);      /* 128 << 14 */

        occupant = FindActorAtWholeTilePosition(position, record);
    }

    return occupant;
}
