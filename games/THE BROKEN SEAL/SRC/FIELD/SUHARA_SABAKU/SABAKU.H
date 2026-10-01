#ifndef SUHARA_SABAKU_SABAKU_H
#define SUHARA_SABAKU_SABAKU_H

/* The Suhara desert (resource_3c0): the declarations, views and call
 * helpers its scene sources share. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

extern u8 gEffectWork[];

struct Selection_02000400 {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor_02000400 {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork_02000400 {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern u8 gSuharaSabakuExits[];
extern u8 gSuharaSabakuActor12Action[];

/* The encounter palette's level, kept while the encounter pulses it. */
extern u16 gSuharaSabakuShownLevel;
s32 GameFlag_GetByte();
void GameFlag_SetByte();
void BattleFx_SetPhaseRequest();
void BattleFx_EmitRandomParticle();

#define ACTOR_ID 13

/* Actor id operated on throughout this scene. */

#define EncounterPalette (*(volatile u16 *)0x0500019e)

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

void SetEffectRecordMode(struct EffectWork *work, s32 mode);

void *OverlayObject_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void SceneState_SendRequest15With45(void);

s32 OverlayObject_ApplyZeroAndClearByte89(u8 *obj);

s32 OverlayObject_ToggleField84Bit0(u8 *obj);

s32 SceneData_ReturnZero(void);

u8 *SuharaSabaku_GetExits(void);

void SuharaSabaku_SyncSelectedActorProgress(void);

void FieldScene_RunMiddleAuxiliarySequence(s32 a0);

void FieldScene_RunActor8Step(void);

void FieldScene_RunActor9Step(void);

void FieldScene_RunActor10Step(void);

void FieldScene_RunActor11Step(void);

void FieldScene_RunActor12Step(void);

void FieldScene_RunLateActor8Step(void);

void FieldScene_RunLateActor9Step(void);

void FieldScene_RunLateActor10Step(void);

void FieldScene_RunLateActor11Step(void);

void FieldScene_RunLateActor12Step(void);

void FieldScene_RunActorThirteenRestoration(void);

void PlaceActorTwelveWhenFlagClear(void);

void SceneState_SetStateByte52(void);

void EncounterPalette_Pulse(void);

s32 FieldScene_RunOpeningAuxiliarySequence(void);

s32 FieldScene_RunScene3c0SequenceA(void);

void SuharaSabaku_ApplyVisitFlagBlend(void);

void SuharaSabaku_ApplyAltarFlagBlend(void);

#endif
