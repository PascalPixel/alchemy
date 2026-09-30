#include "TYPES.H"
#include "CALL.H"

struct Probe {
    s32 word[6];
};

void Object_SetModeById(s32 id, s32 mode);

s32 StagedActor_FillGridAttributeRectangle();
s32 StagedActor_FindClearPosition(struct Probe *probe);
void SceneActor_MoveAndRedraw(struct Probe probe);
void Engine_EventBegin();
void Battle_WaitMode0();
void ObjectMotion_SetSpeedParameters();
void Map_CopyCellAttributeRect();
s32 Object_GetById();
void Engine_GameFlagSet();
void Func_020032f6();
void ObjectMotion_OffsetPositionAndResetMotion();
void Engine_EventEnd();
void Audio_PlayCue();

/* Haidia Cave: move the probed actor, lower actor 11 and fill the cleared
 * rectangle; at column 20 set flag 0x205, otherwise set 0x204 and copy the
 * opened cells. */
void HaidiaDou_RunLoweredActorScene(void)
{
    s32 record;
    struct Probe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        Object_SetModeById(11, 3);
        Call3(ObjectMotion_SetSpeedParameters, 11, 0x4000, 0x8000);
        ObjectMotion_OffsetPositionAndResetMotion(11, 0, -16);
        Battle_WaitMode0(45);
        Audio_PlayCue(240);
        Object_SetModeById(11, 8);
        {
            u8 *obj = (u8 *)Object_GetById(11);
            s32 two = 2;
            s32 zero = 0;

            obj[35] = two;
            StagedActor_FillGridAttributeRectangle(0, 13, (probe.word[4] >> 20) - 1, 4, two, zero);
        }
        if ((probe.word[4] >> 20) == 20) {
            Engine_GameFlagSet(0x205);
        } else {
            ((void (*)())Engine_GameFlagSet)(0x204);
            {
                s32 column = 14;

                Map_CopyCellAttributeRect(14, 17, 2, 1, column, 16);
                Map_CopyCellAttributeRect(14, 13, 1, 1, column, 15);
            }
        }
    }
    Engine_EventEnd();
}
