#include "TYPES.H"
#include "STAGED_ACTOR.H"

void Engine_EventBegin();
void Object_SetModeById();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorSetSpritePriority();
void Engine_GameFlagSet();
void Engine_EventEnd();
void Audio_PlayCue();

/* Haidia Cave: when the probe lands in map column 17, lower the probed actor
 * into place, fill the cleared grid rectangles, set flag 0x203 and play cue
 * 240. */
void HaidiaDou_RunProbedColumnScene(void)
{
    s32 record;
    s32 v6;
    s32 v5;
    s32 two;
    struct StagedActorProbe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if ((probe.position_x >> 20) == 17) {
            Object_SetModeById(probe.actor_slot, 3);
            v6 = 0;
            *(u8 *)((s32)Object_GetById(probe.actor_slot) + 85) = v6;
            record = (s32)Object_GetById(probe.actor_slot);
            *(s32 *)(record + 68) = v6;
            ObjectMotion_OffsetPositionAndResetMotion(probe.actor_slot, -12, 0);
            ObjectMotion_CommitCurrentPositionAndActivate(probe.actor_slot);
            Object_SetModeById(probe.actor_slot, 3);
            Engine_ActorSetSpritePriority(10, 3);
            *(u8 *)((s32)Object_GetById(probe.actor_slot) + 85) = 3;
            ObjectMotion_OffsetPositionAndResetMotion(probe.actor_slot, -6, 0);
            SceneActor_WaitActorDescent((u8 *)Object_GetById(probe.actor_slot));
            Object_SetModeById(probe.actor_slot, 8);
            {
                u8 *obj = (u8 *)Object_GetById(probe.actor_slot);

                two = 2;
                obj[35] = two;
            }
            v5 = 4;
            StagedActor_FillGridAttributeRectangle(0, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, 16, 18, 1, two, v6);
            StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, v5, v6);
            Engine_GameFlagSet(0x203);
            Audio_PlayCue(240);
        }
    }
    Engine_EventEnd();
}
