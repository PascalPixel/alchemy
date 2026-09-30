#include "TYPES.H"

struct Probe {
    s32 word[6];
};

s32 StagedActor_FillGridAttributeRectangle();
s32 StagedActor_FindClearPosition(struct Probe *probe);
void SceneActor_MoveAndRedraw(struct Probe probe);
void SceneActor_WaitActorDescent();
void Engine_EventBegin();
s32 Object_GetById();
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
    struct Probe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if ((probe.word[2] >> 20) == 17) {
            Object_SetModeById(probe.word[1], 3);
            v6 = 0;
            *(u8 *)(Object_GetById(probe.word[1]) + 85) = v6;
            record = Object_GetById(probe.word[1]);
            *(s32 *)(record + 68) = v6;
            ObjectMotion_OffsetPositionAndResetMotion(probe.word[1], -12, 0);
            ObjectMotion_CommitCurrentPositionAndActivate(probe.word[1]);
            Object_SetModeById(probe.word[1], 3);
            Engine_ActorSetSpritePriority(10, 3);
            *(u8 *)(Object_GetById(probe.word[1]) + 85) = 3;
            ObjectMotion_OffsetPositionAndResetMotion(probe.word[1], -6, 0);
            Object_GetById(probe.word[1]);
            SceneActor_WaitActorDescent();
            Object_SetModeById(probe.word[1], 8);
            {
                u8 *obj = (u8 *)Object_GetById(probe.word[1]);

                two = 2;
                obj[35] = two;
            }
            v5 = 4;
            StagedActor_FillGridAttributeRectangle(0, (probe.word[2] >> 20), ((probe.word[4] >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, (probe.word[2] >> 20), ((probe.word[4] >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, 16, 18, 1, two, v6);
            StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, v5, v6);
            Engine_GameFlagSet(0x203);
            Audio_PlayCue(240);
        }
    }
    Engine_EventEnd();
}
