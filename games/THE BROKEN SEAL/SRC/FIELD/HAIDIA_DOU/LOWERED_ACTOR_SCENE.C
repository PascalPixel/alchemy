#include "MAPCOPY.H"
#include "TYPES.H"
#include "STAGED_ACTOR.H"
#include "FIELDOBJ.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"

void HaidiaDou_RunLoweredActorScene(void)
{
    s32 record;
    struct StagedActorProbe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        Engine_ActorSetAnimation(11, 3);
        Actor_SetSpeed(11, 0x4000, 0x8000);
        Engine_ActorSetDestinationOffset(11, 0, -16);
        Engine_EventWait(45);
        Engine_AudioPlayCue(240);
        Engine_ActorSetAnimation(11, 8);
        {
            struct FieldActor *obj = Object_GetById(11);
            s32 two = 2;
            s32 zero = 0;

            obj->priority_flags = two;
            StagedActor_FillGridAttributeRectangle(0, 13, (probe.position_z >> 20) - 1, 4, two, zero);
        }
        if ((probe.position_z >> 20) == 20) {
            Engine_GameFlagSet(0x205);
        } else {
            Engine_GameFlagSet(0x204);
            {
                s32 column = 14;

                Engine_MapCopyCellAttributes(14, 17, 2, 1, column, 16);
                Engine_MapCopyCellAttributes(14, 13, 1, 1, column, 15);
            }
        }
    }
    Engine_EventEnd();
}
