#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08000128(s32 distance, s32 angle, union FieldCoordinate *pos);
s32 Main_080091d8(struct FieldActor *actor, union FieldCoordinate *pos);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Steps the leader half a cell ahead of the snapped cell it faces and
 * hops it there when nothing blocks the way. */
void ShianMura_HopLeaderAhead(void)
{
    s32 flags;
    struct FieldActor *leader;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = (struct FieldActor *)Value1((s32 (*)())Engine_ActorGet, 0);
    flags = leader->motion_flags;
    if (Value1(Engine_GameFlagIsSet, 0x200) != 0) {
        p = pos;
        p[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
        p[1].fixed = leader->y.fixed;
        p[2].fixed = (leader->z.fixed & -0x100000) + 0x80000;
        Call3((void (*)())Main_08000128, 0x200000, (leader->facing + 0x2000) & 0xc000, (s32)p);
        if (Value2((s32 (*)())Main_080091d8, (s32)leader, (s32)p) == 0) {
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
