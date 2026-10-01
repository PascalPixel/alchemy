/* NONMATCHING: Japanese compile-known dialogue message, 2026-10-01.
 * The complete actor-three scene compiles with the approved TBS flags,
 * but builds message 5888 from a shifted byte instead of the game's
 * pool load, moving the following instructions and pool entries.
 * Production uses the canonical PO symbol and matches every edition.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PROBE.H"
#include "TYPES.H"
#include "text/MSG_IDS.H"

TEXT_MESSAGE_ENUM(MsgFieldDoorTightlyLocked);
TEXT_MESSAGE_ENUM(MsgImiruHermesHealingWaterFountain);
TEXT_MESSAGE_ENUM(MsgImiruSomebodyHere);
TEXT_MESSAGE_ENUM(MsgImiruStatueBlocksEntrance);
TEXT_MESSAGE_ENUM(MsgMakyuriHeyaFountainFlowsWithWater);
TEXT_MESSAGE_ENUM(MsgMakyuriHeyaFountainSeemsDry);
TEXT_MESSAGE_ENUM(MsgMakyuriHeyaRobinGot);
#include "MAKYURI.H"
#include "CALL.H"
#include "MAKYURI_HEYA.H"
#include "FIELD_SCENE.H"
#include "MAP_RENDER_WORK.H"

void MakyuriHeya_UpdateLeaderEffectTarget(void);

extern const u8 MakyuriHeya_PushScriptA[];
extern const u8 MakyuriHeya_PushScriptB[];
extern const u8 MakyuriHeya_PushScriptC[];
extern const u8 MakyuriHeya_PushScriptD[];

struct ColumnProbe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct ColumnProbe *probe);
void SceneActor_MoveAndRedraw(struct ColumnProbe probe);
s32 MakyuriHeya_StartPillarPush(void);
void SceneEffect_SpawnParticleRowsByMode(s32 mode);
void FieldScene_RunPrimarySequence(s32 mode);
s32 SceneData_ApplyTableA2c5AndReturnZero(void);
void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth);
void Engine_ActorStartAction(s32 actor);
void Engine_EventBegin();
void Engine_GameFlagClear();
void Engine_EventEnd();
extern u8 MsgMakyuriDidThat[];
void SetEffectRecordMode();
void MakyuriHeya_FadePaletteToWhite();
void MakyuriHeya_CastPsynergyAtActor11(void);
void SceneEffect_RotatePaletteEntries40To47(void);
extern const u16 MakyuriHeya_ColumnCells[];
void Engine_ActorSetChildValue(s32 actor, s32 value);
void Audio_PlayCue(s32 cue);
void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z);
extern const s32 MakyuriHeya_SparkBurstScript[];
void WaitFrames(s32 frames);
void MakyuriHeya_FadePaletteToBlack();
void SceneEffect_RotatePaletteEntries97To103(void);
void BattleFx_PlayQueuedSound();
void MakyuriHeya_SinkActorWithSparks();
void BattleFx_RunRisingObjectSequence();

void Engine_MapCopyCellsTo(s32 sx, s32 sy, s32 dx, s32 dy, s32 w, s32 h);
void Battle_WaitMode0(s32 frames);
extern u8 MsgMakyuriHonorsGoddessRainbows[];
void Makyuri_SpawnLightObjects(s32 count, s32 base);
void Makyuri_ClearPalette(void);
void Makyuri_CyclePalette(void);
void SceneActor_UseActorNinePositionWithYOffset(void);
void MakyuriHeya_RideLift(void);
void BattleFx_StartFadeOverlay(s32 value);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
void DisplayBlend_EnableRunScript(void);
void DisplayBlend_DisableRunScript(void);
void UiText_ShowCenteredMessage(s32 message, s32 a1, s32 a2);
void Engine_MapRenderWaitForValues(void);
void FieldScene_RedrawActorFootprint(s32 actor);
void FieldScene_RunSupplementalSequenceOne(s32 mode);
void MakyuriHeya_ArriveWithSparks(void);
void Engine_ColorBufferInterpolate(s32 frames);

extern u8 MsgMakyuriSavedMeAgain[];
extern u8 MsgMakyuriDontHideTruth[];
extern u8 MsgMakyuriThoughtSo[];
void Event_PrepareObjectAndApplyValue();
s32 Djinn_AddToOwner();
void Djinn_Activate();
void Owner_RecalculateStats();

/* The map layer the lift is drawn on, scrolled by its vertical offset. */
struct LiftLayer {
    u8 unknown_00[12];
    s32 offset;
    u8 unknown_10[12];
    s32 unknown_1c;
};

void FieldScene_RunActorThreeBranchSequence(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(3, FX16_0_8, FX16_0_4);
    Actor_SetSpeed(0, FX16_0_8, FX16_0_4);
    Engine_EventSetMessage(MsgImiruSomebodyHere);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_WalkToAndWait(3, 0x348, 0x288);
    Actor_ShowEmote(3, 0x100, 60);
    Actor_FaceDirection(3, FX16_0_5, 20);
    Object_SetModeById(3, 16);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to -1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = -FX16_1_0;
    Battle_WaitMode0(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Object_SetModeById(3, 1);
    record = (s32)Object_GetById(3);
    /* Set the +24 field of actor 3's record to 1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = FX16_1_0;
    Battle_WaitMode0(20);
    Actor_FaceDirection(3, FX16_0_25, 20);
    Event_OpenMessage(3, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Event_ShowMessageAndWait(3, 0, 20);
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(u8 **)&gEventWork + 0x1d8)) += 1;
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(3, 4);
        Event_ShowMessageAndWait(3, 0, 20);
    }
    Battle_WaitMode0(20);
    Actor_FaceDirection(3, FX16_0_75, 20);
    Camera_SetSpeed(FX16_0_8, FX16_0_1);
    Camera_MoveTo(0x3480000, -1, 0x2780000, 1);
    Actor_WalkToAndWait(3, 0x348, 0x278);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(20);
    Engine_ActorRunRepeatedMotion(3, 2);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(3, 4);
    Battle_WaitMode0(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Engine_GameFlagSet(0x870);
    Engine_EventEnd();
}
