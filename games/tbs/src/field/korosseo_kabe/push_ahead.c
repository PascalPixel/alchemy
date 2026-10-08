#include "task.h"


s32 *SceneData_FindSlotAtPosition(s32 *pos, void *self)
{
    s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((pos[0] >> 20) == (p[2] >> 20)
            && (pos[1] >> 20) == (p[3] >> 20)
            && (pos[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{

    KabeSceneRecord *subject;
    KabeSceneRecord *target;
    KabeSceneRecord *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)&gGameState + (idx << 1));
    subject = (KabeSceneRecord *)Object_GetById(handle);

    dir = subject->facing >> 12;

    step = KorosseoKabe_DirectionSteps[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = (KabeSceneRecord *)SceneData_FindSlotAtPosition((s32 *)&pos, subject);
    if (target == 0) {
        return;
    }

    step = KorosseoKabe_DirectionSteps[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = (KabeSceneRecord *)SceneData_FindSlotAtPosition((s32 *)&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    pos.x = target->x;
    pos.y = target->y + 0x100000;
    pos.z = target->z;

    blocker = (KabeSceneRecord *)SceneData_FindSlotAtPosition((s32 *)&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = KorosseoKabe_DirectionSteps[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    if (Object_CheckMovementCollision(target, &pos) > 0) {
        return;
    }

    Object_SetMode(subject, 8);
    Engine_TaskWait(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Engine_ObjectSetPosition(target, pos.x, pos.y, pos.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Engine_ObjectSetPosition(subject, pos.x, pos.y, pos.z);

    Engine_AudioPlayCue(0xee);
    Object_CommitPosition(target);
    Engine_AudioPlayCue(0x120);

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Object_SetMode(subject, 1);
}

s32 *SceneActor_FindOccupantAheadOfSubject(void)
{

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

    occupant = SceneData_FindSlotAtPosition(position, record);
    if (occupant == 0) {
        position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
        position[1] = record->y.fixed;
        position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, facing, position);

        occupant = SceneData_FindSlotAtPosition(position, record);
    }

    return occupant;
}
