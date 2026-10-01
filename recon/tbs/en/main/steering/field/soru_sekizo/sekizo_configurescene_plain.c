/* NONMATCHING: 2026-10-01 brief Wave2 ConfigureScene plain-source attempt.
 * Removing this one source device changes ConfigureSceneAndCheckActors.
 * Remaining difference: a direct call changes ConfigureSceneAndCheckActors from mov r0, #2 to lsl r1, r1, #16 (22/22 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
#include "../../../../../../../games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/STATUE_HALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

extern u8 MsgSoruSukuretaYouFoundIt[];

enum StatueHallActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

void Event_SayThenWait(s32 speaker, s32 frames);
extern u8 MsgSoruHmphWellTold[];
extern u8 MsgSoruHonestlyDoubtUnderstand[];
extern u8 MsgSoruTryFindSolution[];
extern u8 MsgSoruWait[];
void Event_SayThenWait();
void ObjectMotion_ResetAndSetPositionInMode2();
s32 Inventory_PromptAndSetObjectMode();
s32 UiText_OpenMessageAtObject();

/* A point the camera follows, in 16.16 fixed point. */
struct FocusPoint {
    s32 x;
    s32 y;
    s32 z;
};

/* The map work begins with the point the camera follows. */
struct FocusWork {
    struct FocusPoint *focus;
};

/* The scrolling sprite rows: three rows of nine, after the seal scene. */
struct Ent SoruSekizo_SpriteRows[27];

void SetMapCellCollision();
extern u8 MsgSoruSomethingClicked[];
void Engine_EventBegin();
s32 Engine_GameFlagIsSet();
void Engine_ActorWalkToAndWait();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorSetSpritePriority();
void Engine_AudioPlayCue();
void Engine_ActorSetDestination();
void Engine_ActorSetPosition();
void Engine_WorkSetValuesIfNonNegative();
void Engine_MessageShowCentered();
void Engine_MapCopyCellAttributes();
void Engine_EventEnd();
void Engine_ActorSetAnimation();
void Engine_ActorWaitForMove();

u8 *SceneEventRuntime_GetScriptData(void)
;

s32 SceneEventRuntime_ReturnZero(void)
;

u8 *SceneEventRuntime_GetMessageData(void)
;

u8 *SceneEventRuntime_GetActorData(void)
;

u8 *SceneEventRuntime_GetEffectData(void)
;

s32 SceneEventRuntime_SelectInitialSceneByFlags(void)
;

void Scene_OpenTheHole(void)
;

/*
 * The statue hall of Sol Sanctum, after Robin drops the statue into the
 * hole it opened. Sukureta comes down to look, Gerald and Jasmine tell him
 * what happened, and he decides the trap is disarmed before withdrawing to
 * the Luna room to watch from safety.
 */

/* Each line follows the one before; only the first is set. */
void FieldScene_SetupStagedActors(void)
;

/* Staged scene for actors 8, 5, 1 and 0. The shared work pointer is fetched
 * again at the tail after the intervening calls. Runtime veneer bindings
 * belong to this module's translation-unit declaration. */
void FieldScene_RunStagedActorScene(void)
;

/* After the seal opens: the leader turns toward the seal from whichever side
 * of it he stands, the camera slides thirty pixels toward it, the seal's
 * cells pulse faster and faster, the opened seal is drawn, and the camera
 * slides back. */
void SoruSekizo_RunSealOpenedSequence(void)
;

void SceneEffect_UpdateScrollingSpriteRows(void)
;

void SceneState_RunWhenSlotZeroFacingC000(void)
;

void SceneState_RunWhenActorZeroFacing4000(void)
;

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */
void FieldScene_RunPrimarySequenceHead(void)
;

s32 Scene_RunGuardSequenceB(void)
;

void Scene_RunGuardSequenceC(void)
;

void FieldScene_CallWhenCheck9_31_9(void)
;

void FieldScene_RunGuardedStep11(void)
;

void FieldScene_RunGuardedStep13(void)
;

void FieldScene_RunGuardedStep15(void)
;

void ConfigureSceneAndCheckActors(void)
{
    SetMapCellCollision(2, 0x00d00000, 0x00700000, 0);
    if (SceneActor_IsActorAtTile(10, 14, 7) != 0) {
        Scene_ShineLeftBeam();
    }
}

void ConfigureAlternateSceneAndCheckActors(void)
;

void FieldScene_RunClosingSequence(void)
;

void SoruSekizo_CheckTileTrigger0166C(void)
;

void SoruSekizo_CheckTileTrigger016A4(void)
;

void SoruSekizo_RunStatueDropScene(void)
;

void FieldScene_RunScene37bSequenceA(void)
;

void FieldScene_RunFiveValueStep9(void)
;

void FieldScene_RunFiveValueStep11(void)
;

void FieldScene_ApplyRect13_31_12_30_12(void)
;

void FieldScene_RunFiveValueStep15(void)
;

void FieldScene_ApplyRect10_14_7_13_7(void)
;

/*
 * Fetches scene record 10 and, when it exists, hands a coarse coordinate
 * derived from it to a five-argument routine, which receives both the
 * coordinate and the coordinate plus one; the fifth argument travels on the
 * stack. The `>> 20` reduction to a cell index is by analogy with the rest of
 * the tree and is not verified, and the repeated 13 is as written.
 */
void SceneActor_UseActorTenCellAndNext(void)
;

void SceneActor_MoveActor10ByRow(void)
;

void FieldScene_ApplyRect12_21_7_22_7(void)
;

void SceneActor_ApplyActorTwelveZCellPair(void)
;

void SceneActor_RunSlot12ColumnStep(void)
;

void SoruSekizo_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
;

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z)
;

void SceneData_InitTableA980(void)
;

void SceneData_FillTableA980(void)
;

void SceneData_InitTableA980AndRunB(void)
;

void SceneData_BuildTableA980(void)
;
