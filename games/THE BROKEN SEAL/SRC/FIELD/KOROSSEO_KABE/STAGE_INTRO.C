#include "TYPES.H"
#include "CALL.H"
extern u8 MsgKorosseoMainThingStage[];
extern u8 MsgKorosseoStageDubbedMini[];

void Korosseo_FinishSoloRound();
void Engine_EventBegin();
s32 KorosseoKabe_RunStateInteraction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventShowMessage();
s32 Korosseo_FadeInCompetitor();
void Engine_ActorSetSpeed();
s32 SceneActor_PlaceWithScale14000();
void Engine_EventWait();
s32 Object_GetById();
void Engine_ActorSetAnimation();
void Engine_ActorSetAttachedEffect();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void KorosseoKabe_ShowFollowUpPrompt();
s32 FieldScene_RunMiddleSequence();
void Engine_EventEnd();

extern s16 gCell[];

/* Colosso wall stage: unless the stage is already cleared, walk the player
 * along the wall, show the introduction and hand over to the stage. */
void KorosseoKabe_RunStageIntro(s32 a0)
{
    s32 rec;
    s32 record;

    if (gCell[225] == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec = KorosseoKabe_RunStateInteraction(a0, 1);
        if (rec == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoStageDubbedMini);
            Engine_CameraSetSpeed(0x30000, 0x6000);
            Engine_CameraMoveTo(0x4c80000, -1, 0xb80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x4f8, 168);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            SceneActor_PlaceWithScale14000(0, 0x508, 184);
            SceneActor_PlaceWithScale14000(0, 0x508, 216);
            SceneActor_PlaceWithScale14000(0, 0x4c8, 216);
            Engine_EventShowMessage(a0, 0);
            SceneActor_PlaceWithScale14000(0, 0x4c8, 248);
            SceneActor_PlaceWithScale14000(0, 0x4a8, 248);
            Engine_EventWait(3);
            record = Object_GetById(0);
            *(s32 *)(record + 40) = 0x40000;
            Engine_ActorSetAnimation(0, 28);
            Engine_ActorSetAttachedEffect(0, 0x102);
            Engine_EventWait(30);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            KorosseoKabe_ShowFollowUpPrompt(a0, 1);
        } else {
            if (rec == 1) {
                Engine_EventSetMessage((s32)MsgKorosseoMainThingStage);
                Engine_EventShowMessage(a0, 0);
            }
        }
        FieldScene_RunMiddleSequence(rec, a0, 1);
        Engine_EventEnd();
    }
}
