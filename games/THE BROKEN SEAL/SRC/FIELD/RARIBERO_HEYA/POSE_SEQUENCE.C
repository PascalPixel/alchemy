#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum PoseSequenceMessage {
    MSG_IM_SORRY = 0x26ec
};


extern const u8 gRariberoPoseAction[];
void Engine_ResetSceneEffectCounter(void);
void Engine_ActorSetAnimationAndWait(s32 actor, s32 mode);
void Engine_ActorRunRepeatedMotion(s32 actor, s32 mode);
void Engine_ActorWalkByAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorFaceDirection(s32 actor, s32 angle, s32 value);

void RariberoScene_PlayPoseSequence(void)
{
    s16 facing;

    facing = (Actor_Get(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
    GameFlag_Set(0x300);
    Event_Begin();
    Engine_ResetSceneEffectCounter();
    Event_SetMessage(MSG_IM_SORRY);
    Event_Wait(50);
    Actor_ShowEmote(14, 0x102, 50);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    if ((u16)facing == 0x8000) {
        Engine_ActorWalkByAndWait(0, 0, 16);
        Engine_ActorFaceDirection(0, 0xc000, 0);
        Event_Wait(20);
    }
    Actor_EnableActionCallback(14, gRariberoPoseAction);
    Event_End();
}
