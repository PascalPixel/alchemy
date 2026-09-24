#include "TYPES.H"
#include "FIELD_EVENT.H"

void Engine_ObjectCommitPosition(struct FieldActor *object);
void Main_080090e0(s32 handle, struct FieldActor *object);
void *Engine_AllocateBlock(s32 slot, s32 size);

/* The log's rolling animation for each quarter of the pusher's heading. */
extern u8 Data_0200c0c0[];

/* The leader rolls log id to cell (column / 2, row) on the Board Walk: the
 * log rolls there with the animation for the push direction while the
 * leader follows half the distance behind it. */
void KorosseoKabe_Func020004c0(s32 id, s32 column, s32 row)
{
    s32 pusher = gGameState.selected_actor;
    struct FieldActor *leader = Engine_ActorGet(pusher);
    struct FieldActor *log = Engine_ActorGet(id);
    s32 heading = (leader->facing + 0x1000) & 0xe000;
    s32 along_x = (log->x.fixed >> 20) != column / 2;
    s32 dx;
    s32 dz;

    column <<= 19;
    row <<= 19;
    if (along_x) {
        dx = (column - log->x.fixed) / 2;
        dz = 0;
    } else {
        dx = 0;
        dz = (row - log->z.fixed) / 2;
    }
    Engine_EventBegin();
    Engine_ActorSetAnimation(pusher, 8);
    Engine_EventWait(6);
    log->speed = 0x8000;
    log->acceleration = 0x3333;
    Engine_ObjectSetAnimation(log, Data_0200c0c0[heading / 0x4000]);
    Engine_ObjectSetPosition(log, column, 0, row);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(pusher, 2);
    Main_080090e0(*(s32 *)((u8 *)Engine_AllocateBlock(27, 0xccc) + 480), log);
    Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
    Engine_ObjectSetAnimation(leader, 2);
    Engine_ObjectSetPosition(leader, leader->x.fixed + dx, 0, leader->z.fixed + dz);
    Engine_AudioPlayCue(239);
    Engine_ObjectCommitPosition(leader);
    Engine_ObjectSetAnimation(leader, 1);
    Engine_ObjectCommitPosition(log);
    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(213);
    Engine_ObjectSetAnimation(log, 1);
    Engine_EventWait(15);
    Engine_EventEnd();
}
