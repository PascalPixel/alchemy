#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "GLOBAL_CELLS.H"
#ifndef ALCHEMY_STATUE_HALL_H
#define ALCHEMY_STATUE_HALL_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum StatueHallFlag {
    FLAG_STATUE_TRAP_SPRUNG = 0x813,
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

struct SealScene;
extern struct SealScene gSealScene;
extern const u8 SoruSekizo_SpriteRowsGfx[];

void SoruSekizo_RunStatueDropScene(void);
void Scene_RunGuardSequenceC();
void Resource_DecodeByteLz();
void InitializeSceneRecordBuffer();
void BattleFx_SetQueuedSoundAndPlay();
void SetMapCellCollision();
void SoruSekizo_RunEventSequence(s32, s32, s32, s32, s32);
void SoruSekizo_OpenNorthwestSeal(void);
void SoruSekizo_OpenNortheastSeal(void);
void SoruSekizo_OpenSoutheastSeal(void);
void SoruSekizo_OpenSouthwestSeal(void);
void Scene_ShineLeftBeam(void);
void Scene_ShineRightBeam(void);
void SoruSekizo_OpenSeal(void);

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z);

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    /* FAKEMATCH: a direct call changes ConfigureSceneAndCheckActors from mov r0, #2 to lsl r1, r1, #16 (22/22 assembly lines). */
    SetMapCellCollision(actor, x, y, mode);
}

static __inline__ void SoruSekizo_ConfigureAlternateCell(s32 actor, s32 x, s32 y, s32 mode)
{
    /* FAKEMATCH: a direct call changes ConfigureAlternateSceneAndCheckActors from mov r0, #2 to lsl r1, r1, #17 (22/22 assembly lines). */
    SetMapCellCollision(actor, x, y, mode);
}

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
void SoruSekizo_BlockLeftTriggerCell(void);
void SoruSekizo_BlockRightTriggerCell(void);

#endif
