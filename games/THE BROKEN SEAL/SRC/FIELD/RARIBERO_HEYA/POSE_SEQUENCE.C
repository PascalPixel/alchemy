#include "FIELD_EVENT.H"

extern const u8 gRariberoPoseAction[];
void Engine_ResetSceneEffectCounter(void);
void Engine_SetActorModeAndWait(s32 actor, s32 mode);
void Engine_SetActorMotionAndRefresh(s32 actor, s32 mode);
void Engine_CommitActorPosition(s32 actor, s32 x, s32 z);
void Engine_ArmActorCallback(s32 actor, s32 angle, s32 value);

void RariberoScene_PlayPoseSequence(void)
{
    s16 facing;

    facing = (Actor_Get(0)->facing + 0x2000) & ~0x3fff;
    GameFlag_Set(0x300);
    Event_Begin();
    Engine_ResetSceneEffectCounter();
    Event_SetMessage(0x26ec);
    Event_Wait(50);
    Actor_ShowEmote(14, 0x102, 50);
    Actor_FaceActor(14, 0, 20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_SetActorModeAndWait(14, 4);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_SetActorMotionAndRefresh(14, 2);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_SetActorModeAndWait(14, 3);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    if ((u16)facing == 0x8000) {
        Engine_CommitActorPosition(0, 0, 16);
        Engine_ArmActorCallback(0, 0xc000, 0);
        Event_Wait(20);
    }
    Actor_EnableActionCallback(14, gRariberoPoseAction);
    Event_End();
}
