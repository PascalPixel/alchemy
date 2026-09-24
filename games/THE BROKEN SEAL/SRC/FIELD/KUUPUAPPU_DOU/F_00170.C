#include "TYPES.H"
#include "FIELD_EVENT.H"

struct FieldActor *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz);
s32 Main_080091d8(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);

/* One cell's step for each sixteenth of a turn: pixels across in the high
 * half, pixels along in the low half. */
extern s32 Data_02009844[];


/* The leader pushes the block in the cell ahead one cell further, unless it
 * is one of actors 11 to 14, something solid lies beyond it or the landing
 * cell is higher; leader and block then move together. */
void KuupuappuDou_Func02000170(void)
{
    struct FieldActor *leader = Engine_ActorGet(0);
    s32 direction = leader->facing >> 12;
    struct FieldActor *block;
    struct FieldActor *beyond;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    s32 zero;
    s32 i;
    s32 cx;
    s32 cz;

    cx = (leader->x.part.pixel + (Data_02009844[direction] >> 16)) >> 4;
    cz = (leader->z.part.pixel + (s16)Data_02009844[direction]) >> 4;
    block = SceneData_FindActiveSlotAtCell(cx, cz);
    if (block->collision_flags == 0 || block == 0) {
        return;
    }
    for (i = 0; i <= 3; i++) {
        if (block == Engine_ActorGet(i + 11)) {
            return;
        }
    }
    cx = (block->x.part.pixel + (Data_02009844[direction] >> 16)) >> 4;
    cz = (block->z.part.pixel + (s16)Data_02009844[direction]) >> 4;
    beyond = SceneData_FindActiveSlotAtCell(cx, cz);
    if (beyond != 0 && (beyond->collision_flags & 1)) {
        return;
    }
    block->unknown_22 = 2;
    zero = 0;
    p = pos;
    p[0].fixed = block->x.fixed + (Data_02009844[direction] & 0xffff0000);
    p[1].fixed = block->y.fixed;
    p[2].fixed = block->z.fixed + (Data_02009844[direction] << 16);
    if (Main_080091d8(block, p) > 0) {
        return;
    }
    Engine_ObjectSetAnimation(leader, 8);
    Engine_TaskWait(15);
    Engine_AudioPlayCue(238);
    block->speed = 0x3333;
    block->acceleration = 0x3333;
    Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
    leader->speed = 0x3333;
    leader->acceleration = 0x3333;
    Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
    Engine_ObjectCommitPosition(block);
    Engine_AudioPlayCue(288);
    block->x.fixed = p[0].fixed;
    block->z.fixed = p[2].fixed;
    block->velocity_x = zero;
    block->velocity_z = zero;
    leader->velocity_x = zero;
    leader->velocity_z = zero;
    leader->target_x = ACTOR_NO_TARGET;
    leader->target_z = ACTOR_NO_TARGET;
    Engine_ObjectSetAnimation(leader, 1);
}
