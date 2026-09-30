#include "TYPES.H"
#include "CALL.H"

struct Resource39fProbe {
    s32 word[6];
};

void Engine_EventBegin(void);
s32 StagedActor_FindClearPosition(struct Resource39fProbe *probe);
void SceneActor_MoveAndRedraw(struct Resource39fProbe probe);
void Engine_GameFlagSet(s32 flag);
void Object_SetModeById(s32 actor, s32 animation);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Battle_WaitMode0(s32 frames);
void Object_SetModeById(s32 actor, s32 animation);
void Audio_PlayCue(s32 cue);
void Engine_ActorSetSpritePriority(s32 actor, s32 value);
u8 *Engine_ActorGet(s32 actor);
void Map_CopyCellAttributeRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Engine_EventEnd(void);

/* Mogoru Forest: when the probe finds actor 9 in map column 26, set flag
 * 0x310, step actor 9 back, play cue 240 and copy the cleared cells. */
void MogoruMori_RunProbedActorNineScene(void)
{
    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if (probe.word[1] == 9 && (probe.word[4] >> 20) == 26) {
            Engine_GameFlagSet(0x310);
            Object_SetModeById(9, 3);
            Call3(ObjectMotion_SetSpeedParameters, 9, 0x4000, 0x8000);
            ObjectMotion_OffsetPositionAndResetMotion(9, 0, -16);
            Battle_WaitMode0(45);
            Object_SetModeById(9, 8);
            Audio_PlayCue(240);
            Engine_ActorSetSpritePriority(9, 1);
            Engine_ActorGet(9)[35] = 2;
            fifth = 31;
            sixth = 25;
            Map_CopyCellAttributeRect(38, 27, 4, 2, fifth, sixth);
        }
    }
    Engine_EventEnd();
}
