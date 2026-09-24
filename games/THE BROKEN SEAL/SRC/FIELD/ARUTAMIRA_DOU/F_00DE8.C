#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08000128(s32 distance, s32 angle, union FieldCoordinate *pos);
s32 Main_080091d8(struct FieldActor *actor, union FieldCoordinate *pos);

/* Hops the selected actor one step (two cells) the way it faces, snapping to
 * the facing's sixteenth, unless something blocks the landing cell. */
void ArutamiraDou_Func02000de8(void)
{
    struct FieldActor *actor = Engine_ActorGet(gGameState.selected_actor);
    s32 flags = actor->motion_flags;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p = pos;

    p[0].fixed = actor->x.fixed;
    p[1].fixed = actor->y.fixed;
    p[2].fixed = actor->z.fixed;
    {
        s32 angle = actor->facing & 0xf000;

        Main_08000128(0x200000, angle, p);
    }
    if (Main_080091d8(actor, p) == 0) {
        Engine_EventBegin();
        Engine_ObjectSetAnimation(actor, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Engine_ObjectSetAnimation(actor, 7);
        actor->speed = 0x30000;
        actor->acceleration = 0x20000;
        actor->velocity_y = 0x40000;
        actor->motion_flags &= 126;
        Engine_ActorSetSpriteFlags(actor, 0);
        Engine_ObjectMotionSetPositionAndCommit(0, p[0].part.pixel, p[2].part.pixel);
        Engine_ObjectSetAnimation(actor, 6);
        Engine_ActorSetSpriteFlags(actor, 1);
        actor->motion_flags = flags;
        Engine_EventEnd();
    }
}
