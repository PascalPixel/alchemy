#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);

/* Steps the leader half a cell ahead of the snapped cell it faces and
 * hops it there when nothing blocks the way. */
void ShianMura_HopLeaderAhead(void)
{
    s32 flags;
    struct FieldActor *leader;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = (struct FieldActor *)((s32 (*)())Engine_ActorGet)(0);
    flags = leader->motion_flags;
    if (Engine_GameFlagIsSet(0x200) != 0) {
        p = pos;
        p[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
        p[1].fixed = leader->y.fixed;
        p[2].fixed = (leader->z.fixed & -0x100000) + 0x80000;
        Call3((void (*)())Vector_AddPolarOffset, 0x200000, (leader->facing + 0x2000) & 0xc000, (s32)p);
        if (((s32 (*)())Object_CheckMovementCollision)((s32)leader, (s32)p) == 0) {
            Engine_EventBegin();
            Engine_ObjectSetAnimation(leader, 6);
            Engine_TaskWait(6);
            Engine_AudioPlayCue(152);
            Engine_ObjectSetAnimation(leader, 7);
            leader->speed = 0x30000;
            leader->acceleration = 0x20000;
            leader->velocity_y = 0x40000;
            leader->motion_flags &= 126;
            Engine_ActorSetSpriteFlags(leader, 0);
            Engine_ObjectMotionSetPositionAndCommit(0, p[0].part.pixel, p[2].part.pixel);
            Engine_ObjectSetAnimation(leader, 6);
            Engine_ActorSetSpriteFlags(leader, 1);
            leader->motion_flags = flags;
            Engine_EventEnd();
        }
    }
}
