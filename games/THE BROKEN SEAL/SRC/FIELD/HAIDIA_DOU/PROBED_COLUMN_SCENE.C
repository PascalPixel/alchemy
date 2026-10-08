#include "TYPES.H"
#include "STAGED_ACTOR.H"
#include "FIELDOBJ.H"

void Engine_EventBegin();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestinationOffset();
void Engine_ActorWaitForMove();
void Engine_ActorSetSpritePriority();
void Engine_GameFlagSet();
void Engine_EventEnd();
void Engine_AudioPlayCue();

void HaidiaDou_RunProbedColumnScene(void)
{
    struct FieldActor *record;
    s32 v6;
    s32 v5;
    s32 two;
    struct StagedActorProbe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if ((probe.position_x >> 20) == 17) {
            Engine_ActorSetAnimation(probe.actor_slot, 3);
            v6 = 0;
            ((struct FieldActor *)Object_GetById(probe.actor_slot))->motion_flags = v6;
            record = Object_GetById(probe.actor_slot);
            record->bounce = v6;
            Engine_ActorSetDestinationOffset(probe.actor_slot, -12, 0);
            Engine_ActorWaitForMove(probe.actor_slot);
            Engine_ActorSetAnimation(probe.actor_slot, 3);
            Engine_ActorSetSpritePriority(10, 3);
            ((struct FieldActor *)Object_GetById(probe.actor_slot))->motion_flags = 3;
            Engine_ActorSetDestinationOffset(probe.actor_slot, -6, 0);
            SceneActor_WaitActorDescent(Object_GetById(probe.actor_slot));
            Engine_ActorSetAnimation(probe.actor_slot, 8);
            {
                struct FieldActor *obj = Object_GetById(probe.actor_slot);

                two = 2;
                obj->priority_flags = two;
            }
            v5 = 4;
            StagedActor_FillGridAttributeRectangle(0, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, 16, 18, 1, two, v6);
            StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, v5, v6);
            Engine_GameFlagSet(0x203);
            Engine_AudioPlayCue(240);
        }
    }
    Engine_EventEnd();
}
