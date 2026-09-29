#include "TYPES.H"
#include "FIELD_EVENT.H"

void Object_CommitPosition(struct FieldActor *object);
void Object_SetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);

/* The leader pushes wall block id to cell (column / 2, row): the block slides
 * there and the leader follows half the distance behind it. */
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row)
{
    s32 pusher = gGameState.selected_actor;
    struct FieldActor *leader = Engine_ActorGet(pusher);
    struct FieldActor *block = Engine_ActorGet(id);
    s32 along_x = (block->x.fixed >> 20) != column / 2;
    s32 dx;
    s32 dz;

    column <<= 16;
    row <<= 16;
    if (along_x) {
        dx = (column - block->x.fixed) / 2;
        dz = 0;
    } else {
        dx = 0;
        dz = (row - block->z.fixed) / 2;
    }
    Engine_ActorSetAnimation(pusher, 8);
    Engine_EventWait(6);
    block->speed = 0x8000;
    block->acceleration = 0x3333;
    Engine_AudioPlayCue(239);
    Engine_ObjectSetAnimation(block, 3);
    Object_SetPosition(block, column, 0, row);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(pusher, 2);
    Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
    Engine_ObjectSetAnimation(leader, 2);
    Object_SetPosition(leader, leader->x.fixed + dx, 0, leader->z.fixed + dz);
    Object_CommitPosition(leader);
    Engine_ObjectSetAnimation(leader, 1);
    Object_CommitPosition(block);
    Engine_ObjectSetAnimation(block, 1);
    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(213);
    Engine_EventWait(15);
}
