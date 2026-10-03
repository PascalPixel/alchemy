/* NONMATCHING: localized moving-sidewalk attached effect, 2026-10-01.
 * The complete opening owner compiles with approved TBS flags but retains
 * the attached-effect 0x100 call after animation one. Spanish, French and
 * Italian omit that call, shortening the complete owner by eight bytes.
 */
/* The four-step actor motion, actor placement and the opening sequence. */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/LOG_ROLLING.H"
#include "CALL.H"
extern u8 MsgKorosseoAnotherLogRollingArea[];
extern u8 MsgKorosseoCalledMovingSidewalkStage[];
extern u8 MsgKorosseoClearStageMustAbleChange[];
extern u8 MsgKorosseoHereMustFigureOutHow[];
extern u8 MsgKorosseoInStageMustTryOutpace[];
extern u8 MsgKorosseoPlaceNormallyCalledFreeClimb[];

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    extern void Korosseo_FinishSoloRound();
    extern s32 FieldScene_RunMiddleSequence();
    extern void Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
    extern void Korosseo_RestoreCompetitor();

    s32 i;
    s32 rec2;
    s32 rec7;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec2 = ColossoLogRollingStage_RunStateInteraction(a0, 5);
        if (rec2 != 0) {
        } else {
            Engine_EventSetMessage((s32)MsgKorosseoCalledMovingSidewalkStage);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4380000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x3d8, 184);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            ColossoLogRollingStage_PositionAndActivateActor(0, 0x3e0, 184);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
            ColossoLogRollingStage_PositionActor(0, 0x460, 184);
            Engine_EventWait(120);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
            Engine_EventWait(120);
            ColossoLogRollingStage_ResetActorMotion(0);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
            rec7 = Object_GetById(0);
            for (i = 119; i >= 0; i--) {
                if (*(s32 *)(rec7 + 8) > 0x3e00000) {
                    *(s32 *)(rec7 + 8) += -0x13333;
                }
                Engine_TaskWait(1);
            }
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x103, 60);
            ColossoLogRollingStage_PositionActor(0, 0x460, 184);
            Event_ShowMessage(a0, 0);
            ColossoLogRollingStage_ResetActorMotion(0);
            {
                u8 *flag = (u8 *)&gGameState;

                flag[498] = 1;
            }
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 5);
            goto L_02002494;
        }
        if (rec2 == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoInStageMustTryOutpace);
            Event_ShowMessage(a0, 0);
        }
        L_02002494:;
        FieldScene_RunMiddleSequence(rec2, a0, 5);
        Engine_EventEnd();
    }
}
