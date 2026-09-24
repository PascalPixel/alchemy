#include "TYPES.H"

void Local_02001b5c();
void Engine_EventBegin();
s32 SceneDialogue_RunFlagGatedPromptInteraction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventShowMessage();
s32 Local_02002910();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_EventWait();
void Engine_ActorShowEmote();
void Engine_ActorFaceDirection();
s32 OverlayObject_PlaceWithScale14000();
void StagedActor_PushActorAhead();
void OverlayObject_ResetMotionFields();
void Engine_ActorSetAnimation();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void Engine_ActorSetPosition();
void SceneState_SendIdBySceneId();
s32 FieldScene_RunMiddleSequence();
void Engine_EventEnd();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Colosso river stage: unless the stage is already cleared, walk the player
 * through the introduction, push the pillars and hand over to the stage. */
void KorosseoKawa_RunStageIntro(s32 a0)
{
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Local_02001b5c();
    } else {
        Engine_EventBegin();
        rec = Value2(SceneDialogue_RunFlagGatedPromptInteraction, a0, 1);
        if (rec != 0) {
        } else {
            Call1(Engine_EventSetMessage, 0x208c);
            Call2(Engine_CameraSetSpeed, 0x30000, 0x6000);
            Call4(Engine_CameraMoveTo, 0x1480000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            Value3(Local_02002910, 0, 0x118, 200);
            Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
            Call3(Engine_ActorWalkToAndWait, 0, 0x168, 200);
            Engine_EventWait(30);
            Call3(Engine_ActorShowEmote, 0, 0x102, 60);
            Engine_EventShowMessage(a0, 0);
            Call3(Engine_ActorWalkToAndWait, 0, 0x138, 200);
            Engine_EventWait(30);
            Call3(Engine_ActorFaceDirection, 0, 0xc000, 10);
            Call3(Engine_ActorShowEmote, 0, 0x106, 60);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            OverlayObject_PlaceWithScale14000(0, 0x128, 184);
            OverlayObject_PlaceWithScale14000(0, 0x128, 152);
            Value3(OverlayObject_PlaceWithScale14000, 0, 0x138, 152);
            Call3(Engine_ActorFaceDirection, 0, 0x4000, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            Call3(Engine_ActorWalkToAndWait, 0, 0x130, 184);
            Engine_ActorWalkToAndWait(0, 0x128, 192);
            Engine_ActorWalkToAndWait(0, 0x128, 200);
            Engine_ActorFaceDirection(0, 0, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Engine_ActorSetAnimation(0, 1);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            Call3(Engine_ActorSetPosition, 9, 0x1380000, 0xa80000);
            SceneState_SendIdBySceneId(a0, 1);
            goto L_020013c2;
        }
        if (rec == 1) {
            Call1(Engine_EventSetMessage, 0x208b);
            Engine_EventShowMessage(a0, 0);
        }
        L_020013c2:;
        Value3(FieldScene_RunMiddleSequence, rec, a0, 1);
        Engine_EventEnd();
    }
}
