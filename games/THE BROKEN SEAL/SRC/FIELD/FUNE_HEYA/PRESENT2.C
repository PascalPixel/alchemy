#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const s32 FuneHeya_ActionScriptE[];
#include "FIELD_SCENE.H"
#include "HEYA.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneImTurning[];

/* Message ids. */

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
extern u8 FuneHeya_EntryActionScript[];
void FieldScene_RunSceneStep();
void FieldScene_RunPositionTransferPresentation();

void SceneActor_SetFlagBit3ForActors28To35(void);

void FieldScene_RunScene3b1_0200413c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    FieldScene_RunSceneStep(15, 1, 1);
    Actor_RunRepeatedMotion(8, 1);
    Event_SetMessage((s32)MsgFuneImTurning);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0xd000, 40);
    FieldScene_RunSceneStep(9, 15, 0);
}

void FieldScene_RunScene3b1_02004198(void)
{
    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 1, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    FieldScene_RunSceneStep(19, 11, 12);
    Actor_SetAnimation(10, 6);
    Value2(Engine_ActorEnableActionCallback, 12, (s32)FuneHeya_ActionScriptE);
    base5_200e8e4 = (s32)FuneHeya_EntryActionScript;
    Actor_EnableActionCallback(36, base5_200e8e4);
    Value2(Engine_ActorEnableActionCallback, 37, base5_200e8e4);
    Value2(Engine_ActorEnableActionCallback, 38, base5_200e8e4);
    Actor_SetChildValue(36, 3);
    Actor_SetChildValue(37, 3);
    Actor_SetChildValue(38, 3);
    FieldScene_RunPositionTransferPresentation();
    Event_End();
}

void FieldScene_RunActors24And25Setup(void)
{
    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    FieldScene_RunSceneStep(19, 11, 12);
    FieldScene_RunFormationAndEffectPresentation();
    GameFlag_Set(0x928);
    Event_End();
}
