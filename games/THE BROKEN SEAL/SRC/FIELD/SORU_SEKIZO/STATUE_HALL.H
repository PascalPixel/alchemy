#include "GLOBAL_CELLS.H"
#ifndef ALCHEMY_STATUE_HALL_H
#define ALCHEMY_STATUE_HALL_H

/* Declarations shared by the Sol Sanctum statue hall scene sources. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The two statues at the head of the statue hall each shine a beam of light
 * into the floor; once both shine, the floor flickers and a hole opens.
 */
enum StatueHallFlag {
    FLAG_STATUE_TRAP_SPRUNG = 0x813,
    /* The beams drawn at map columns 15 and 17. */
    FLAG_LEFT_BEAM_SHINING = 0x816,
    FLAG_RIGHT_BEAM_SHINING = 0x817
};

#define NULL ((void *)0)
#define TARGET_ID 9
#define GATE_CODE 2059

struct Ent {
    s32 f00;
    u8 f04;
    u8 f05;
    u16 f06:9;
    u16 f07:7;
    s32 f08;
};

struct Cam {
    u8 pad[228];
    s32 x;
    s32 y;
};

extern u8 SceneEventRuntime_ScriptData[];
extern u8 SceneEventRuntime_MessageData[];
extern u8 SceneEventRuntime_ActorData[];
extern u8 SceneEventRuntime_EffectData[];
extern s32 SoruSekizo_ScrollPhase;
/* The saved game as halfwords. */
extern u16 gCell[][1];

/* The seal scene the guarded steps fill in (OPEN_SEAL.C), the rows of
 * scrolling sprites that follow it, and their graphics in the overlay's data. */
struct SealScene;
extern struct SealScene gSealScene;
extern const u8 SoruSekizo_SpriteRowsGfx[];

void SoruSekizo_RunStatueDropScene(void);
void Runtime_PushSlotEntry(struct Ent *, s32);
void Scene_RunGuardSequenceC();
s32 Runtime_AllocateBlock();
void Resource_DecodeByteLz();
s32 Resource_FindFreeEntry();
void InitializeSceneRecordBuffer();
void BattleFx_SetQueuedSoundAndPlay();
void SetMapCellCollision();
void SoruSekizo_RunEventSequence(s32, s32, s32, s32, s32);
void SceneData_InitTableA980(void);
void SceneData_FillTableA980(void);
void SceneData_BuildTableA980(void);
void SceneData_InitTableA980AndRunB(void);
void Scene_ShineLeftBeam(void);
void Scene_ShineRightBeam(void);
void SoruSekizo_OpenSeal(void);

/* Scene-record accessor. */

/* Old-style: the interface is not established, and the call site passes five
 * arguments. */

/* Argument shared by every configuration call below. */

/* Condition code passed to the gating check; the body below runs only when
 * that check reports 0. */

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z);

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    /* FAKEMATCH: a direct call changes ConfigureSceneAndCheckActors from mov r0, #2 to lsl r1, r1, #16 (22/22 assembly lines). */
    SetMapCellCollision(actor, x, y, mode);
}

static __inline__ void ConfigureScene_02003a30(s32 actor, s32 x, s32 y, s32 mode)
{
    /* FAKEMATCH: a direct call changes ConfigureAlternateSceneAndCheckActors from mov r0, #2 to lsl r1, r1, #17 (22/22 assembly lines). */
    SetMapCellCollision(actor, x, y, mode);
}

/* Functions of the overlay's other sources. */
u8 *SceneEventRuntime_GetScriptData(void);
s32 SceneEventRuntime_ReturnZero(void);
u8 *SceneEventRuntime_GetMessageData(void);
u8 *SceneEventRuntime_GetActorData(void);
u8 *SceneEventRuntime_GetEffectData(void);
s32 SceneEventRuntime_SelectInitialSceneByFlags(void);
void Scene_OpenTheHole(void);
void SceneEffect_UpdateScrollingSpriteRows(void);
void SceneState_RunWhenSlotZeroFacingC000(void);
void SceneState_RunWhenActorZeroFacing4000(void);
void FieldScene_RunPrimarySequenceHead(void);
s32 Scene_RunGuardSequenceB(void);
void FieldScene_CallWhenCheck9_31_9(void);
void FieldScene_RunGuardedStep11(void);
void FieldScene_RunGuardedStep13(void);
void FieldScene_RunGuardedStep15(void);
void ConfigureSceneAndCheckActors(void);
void ConfigureAlternateSceneAndCheckActors(void);
void FieldScene_RunClosingSequence(void);
void FieldScene_RunScene37bSequenceA(void);
void FieldScene_RunFiveValueStep9(void);
void FieldScene_RunFiveValueStep11(void);
void FieldScene_ApplyRect13_31_12_30_12(void);
void FieldScene_RunFiveValueStep15(void);
void FieldScene_ApplyRect10_14_7_13_7(void);
void SceneActor_UseActorTenCellAndNext(void);
void SceneActor_MoveActor10ByRow(void);
void FieldScene_ApplyRect12_21_7_22_7(void);
void SceneActor_ApplyActorTwelveZCellPair(void);
void SceneActor_RunSlot12ColumnStep(void);
void Event_SayThenWait(s32 speaker, s32 frames);
void FieldScene_SetupStagedActors(void);
void FieldScene_RunStagedActorScene(void);
void SoruSekizo_CheckTileTrigger0166C(void);
void SoruSekizo_CheckTileTrigger016A4(void);

#endif
